#!/usr/bin/env python
#/*******************************************************************/
#/*                               XDMF                              */
#/*                   eXtensible Data Model and Format              */
#/*                                                                 */
#/*  Id : Id  */
#/*  Date : $Date$ */
#/*  Version : $Revision$ */
#/*                                                                 */
#/*  Author:                                                        */
#/*     Jerry A. Clarke                                             */
#/*     clarke@arl.army.mil                                         */
#/*     US Army Research Laboratory                                 */
#/*     Aberdeen Proving Ground, MD                                 */
#/*                                                                 */
#/*     Copyright @ 2002 US Army Research Laboratory                */
#/*     All Rights Reserved                                         */
#/*     See Copyright.txt or http://www.arl.hpc.mil/ice for details */
#/*                                                                 */
#/*     This software is distributed WITHOUT ANY WARRANTY; without  */
#/*     even the implied warranty of MERCHANTABILITY or FITNESS     */
#/*     FOR A PARTICULAR PURPOSE.  See the above copyright notice   */
#/*     for more information.                                       */
#/*                                                                 */
#/*******************************************************************/
import sys
import string


print 'Loading vtk'
from libVTKCommonPython import *
from libVTKGraphicsPython import *
from libVTKPatentedPython import *
from libVTKImagingPython import *
from libVTKLocalPython import *

print 'Loading Xdmf'
import Xdmf

class XdmfMeshViewer :

	def __init__( self, FileName ):
		print 'Parsing ' + FileName
		DOM = Xdmf.XdmfDOM()
		DOM.SetInputFileName( FileName )
		DOM.Parse()
		print 'Reading Heavy Data'
		Ren = vtkRenderer()
		self.Ren = Ren
		Index = 0
		# Thickness = .01
		Thickness = 2 
		node  = DOM.FindElement("Grid", Index)
		while node != None :
			Grid = Xdmf.XdmfGrid()
			Grid.SetDOM( DOM )
			Grid.SetGridFromElement( node )
			Grid.AssignAttributeByName('FakeColor')

			CurrentAttribute = Grid.GetAssignedAttribute()
			ValueArray = CurrentAttribute.GetValues()
			ValueArray.Generate(Thickness, Thickness)
			# print 'Values = ' + ValueArray.GetValues()
			CurrentAttribute.SetBaseAttribute( Grid, Grid.GetBaseGrid() )

			VtkGrid = vtkDataSet( Grid.GetBaseGrid() )


			GeometryFilter = vtkGeometryFilter()
			GeometryFilter.SetInput( VtkGrid )
			GeometryFilter.Update()

			# Edges = vtkExtractEdges()
			# Edges.SetInput( GeometryFilter.GetOutput() )
			# Tubes = vtkTubeFilter()
			# Tubes.SetRadius( .05 )
			# Tubes.SetInput( Edges.GetOutput() )

			Tri = vtkTriangleFilter()
			Tri.SetInput( GeometryFilter.GetOutput() )
			Tri.Update()

			Normals = vtkPolyDataNormals()
			Normals.SetInput( Tri.GetOutput() )
			Normals.SplittingOn()
			Normals.SetFeatureAngle(0)
			Normals.FlipNormalsOff()
			# Normals.FlipNormalsOn()
			Normals.Update()


			warp = vtkWarpScalar()
			warp.SetInput( Normals.GetOutput() )
			warp.SetScaleFactor( -1.0  )
			warp.Update()

			Wedges = self.BuildWedges( Tri.GetOutput(), warp.GetOutput() )
			GeoWedge = vtkGeometryFilter()
			GeoWedge.SetInput( Wedges )
			GeoWedge.Update()

			Writer = vtkXdmfDataSetWriter()
			Writer.SetInput( Wedges )
			Writer.SetHeavyDataSetName('Extrude.h5')
			Writer.SetAllLight( 1 )
			Writer.WriteGrid()
			Writer.WriteAttributes()
			# print 'XML = ' + Writer.GetXML()

			Mapper = vtkPolyDataMapper()
			Mapper.SetInput( warp.GetOutput() )
			self.CreateTable( Mapper )
			Mapper.Update()
			Mapper.SetScalarRange( 0, 4095)

			Actor = vtkActor()
			Actor.SetMapper( Mapper )
			# Actor.GetProperty().SetOpacity( 0.5 )
		
			# OrigActor = vtkActor()
			# OrigMapper = vtkPolyDataMapper()
			# OrigMapper.SetInput( Tubes.GetOutput() )
			# OrigMapper.ScalarVisibilityOff();
			# OrigActor.SetMapper( OrigMapper )
			# OrigActor.GetProperty().SetColor( 1.0, 0.0, 0.0 )

			WedgeActor = vtkActor()
			WedgeMapper = vtkPolyDataMapper()
			WedgeMapper.SetInput( GeoWedge.GetOutput() )
			WedgeActor.SetMapper( WedgeMapper )
			# WedgeActor.GetProperty().SetOpacity( 0.5 )
			WedgeActor.GetProperty().SetRepresentationToWireframe()
			WedgeActor.GetProperty().SetLineWidth( 2.0 )
			WedgeMapper.Update()

			Bounds = WedgeActor.GetBounds()
			print 'Bounds = ' + str( Bounds )
			xmin = Bounds[0]
			xmax = Bounds[1]
			ymin = Bounds[2]
			ymax = Bounds[3]
			zmin = Bounds[4]
			zmax = Bounds[5]

			CellWidth = .2
			Sgrid = vtkStructuredPoints()
			Sgrid.SetDimensions( 5 + int(( xmax - xmin ) / CellWidth ) ,
				5 + int(( ymax - ymin ) / CellWidth ) ,
				5 + int(( zmax - zmin ) / CellWidth ) )
			Sgrid.SetOrigin( float(xmin) - (2.0 * CellWidth),
				float(ymin) - (2.0 * CellWidth),
				float(zmin) - (2.0 * CellWidth))
			Sgrid.SetSpacing(CellWidth, CellWidth, CellWidth )
			idim, jdim, kdim = Sgrid.GetDimensions()
			print 'Dims = ' + str( Sgrid.GetDimensions() )
	
			# SgridProbe = vtkProbeFilter()
			# SgridProbe.SetInput( Sgrid )
			# SgridProbe.SetSource( Wedges )
			# SgridProbe.Update()


			# SgridProbe = vtkImplicitDataSet()
			# SgridProbe.SetDataSet( Wedges )

			# a = vtkVoxelModeller()
			a = vtkImplicitModeller()
			a.SetSampleDimensions( idim, jdim, kdim )
			a.SetModelBounds(xmin, xmax, ymin, ymax, zmin, zmax )
			# a.SetInput( Wedges )
			#  a.StartAppend()
			#  a.Append( Wedges )
			#  a.EndAppend()
			a.SetInput( Wedges )
			a.SetProcessModeToPerVoxel()
			a.SetNumberOfThreads( 10 )
			a.SetMaximumDistance( 0.001 )
			self.Modeller = a
			a.SetProgressMethod( self.PercentDone )
			a.Update()

			srange = a.GetOutput().GetScalarRange()
			print 'Range = ' + str( srange )	
			scalars = a.GetOutput().GetPointData().GetScalars()
			# for i in range( scalars.GetNumberOfScalars() ) :
			# 	print 'Scalar ' + str(i) + ' = ' + str( scalars.GetScalar( i ) )

			
			PointToCell = vtkPointDataToCellData()
			PointToCell.SetInput( a.GetOutput() )
			SgridThresh = vtkThreshold()
			SgridThresh.SetInput( PointToCell.GetOutput() )
			SgridThresh.ThresholdByLower( 1.0 )
			SgridThresh.SetAttributeModeToUseCellData()
			SgridThresh.SetProgressMethod( self.PercentDone )

			SgridGeo = vtkGeometryFilter()
			SgridGeo.SetInput( SgridThresh.GetOutput() )

			x = vtkStructuredPointsGeometryFilter()
			x.SetInput( a.GetOutput() )
			x.SetExtent( 0, 1000, 0, 1000, 0, 1000 )

			SgridMapper = vtkPolyDataMapper()
			# SgridMapper = vtkDataSetMapper()
			# SgridMapper.SetInput( SgridGeo.GetOutput() )

			# SgridMapper.SetInput( x.GetOutput() )
			SgridMapper.SetInput( SgridGeo.GetOutput() )

			SgridActor = vtkActor()
			SgridActor.SetMapper( SgridMapper )
			# SgridActor.GetProperty().SetRepresentationToWireframe()
			SgridActor.GetProperty().SetPointSize( 4 )
			# SgridActor.GetProperty().SetOpacity( 0.5 )

			# Ren.AddActor( Actor )
			# Ren.AddActor( OrigActor )
			Ren.AddActor( SgridActor )
			Ren.AddActor( WedgeActor )

			Index += 1
			node  = DOM.FindElement("Grid", Index)

	def PercentDone( self ) :
		pd = self.Modeller.GetProgress()
		print str( pd ) + " % Done"

	def BuildWedges( self, OrigTri, WarpTri ) :
		NumberOfPolys = OrigTri.GetNumberOfPolys()
		NumberOfPoints = OrigTri.GetNumberOfPoints()
		Append = vtkAppendPolyData()
		Append.AddInput( OrigTri )
		Append.AddInput( WarpTri )
		Append.Update()
		NewGrid = vtkUnstructuredGrid()
		NewGrid.SetPoints( Append.GetOutput().GetPoints())
		NewWedge = vtkWedge()
		# Scalars = Append.GetOutput().GetPointData().GetScalars()
		# for i in range( 10 ) :
		# 	print str(i) + ' = ' + str( Scalars.GetScalar(i))
		Cntr = 0
		Total = 0
		for i in range( NumberOfPolys ) :
			old = OrigTri.GetCell( i )
			new = WarpTri.GetCell( i )
			# NewWedge.GetPointIds().SetId(0, old.GetPointId(0) )
			# NewWedge.GetPointIds().SetId(1, old.GetPointId(1) )
			# NewWedge.GetPointIds().SetId(2, old.GetPointId(2) )
			#
			# NewWedge.GetPointIds().SetId(3, NumberOfPoints + new.GetPointId(0) )
			# NewWedge.GetPointIds().SetId(4, NumberOfPoints + new.GetPointId(1) )
			# NewWedge.GetPointIds().SetId(5, NumberOfPoints + new.GetPointId(2) )

		
			NewWedge.GetPointIds().SetId(0, old.GetPointId(0) )
			NewWedge.GetPointIds().SetId(1, old.GetPointId(1) )
			NewWedge.GetPointIds().SetId(2, old.GetPointId(2) )
			NewWedge.GetPointIds().SetId(3, NumberOfPoints + new.GetPointId(0) )
			NewWedge.GetPointIds().SetId(4, NumberOfPoints + new.GetPointId(1) )
			NewWedge.GetPointIds().SetId(5, NumberOfPoints + new.GetPointId(2) )



			NewGrid.InsertNextCell( NewWedge.GetCellType(), NewWedge.GetPointIds())
			if Cntr >= 5000 :
				print 'Completed ' + str( Total ) + ' Cells'
				Cntr = 0
			Total += 1
			Cntr += 1
		NewGrid.GetPointData().SetScalars(  Append.GetOutput().GetPointData().GetScalars() )
		return NewGrid
			



	def CreateTable( self, Mapper ) :
		Table = vtkLookupTable()
		Table.SetNumberOfColors(4096)
		print 'Setting Color Table'
		for i in range( 4096 ) :
			red = ( i & 0xF00 ) >> 4
			red = float( red ) / 255.0
			green = ( i & 0x0F0 )
			green = float( green ) / 255.0
			blue = ( i & 0x0F ) << 4
			blue = float( blue ) / 255.0
			# print str(i) + ' = ' + \
			# 	str( red ) + ' ' + \
			# 	str( green ) + ' ' + \
			# 	str( blue )
			Table.SetTableValue( i, float(red), float(green), float(blue), 1.0 )
		Mapper.SetLookupTable( Table )

	def SetColor ( self, Actor, Index ) :
		Color = Index % 5
		if Color == 0 :
			Actor.GetProperty().SetColor( 1.0, 1.0, 0.0 )
		elif Color == 1 :
			Actor.GetProperty().SetColor( 1.0, 0.0, 0.0 )
		elif Color == 2 :
			Actor.GetProperty().SetColor( 0.0, 0.0, 1.0 )
		elif Color == 3 :
			Actor.GetProperty().SetColor( 0.0, 1.0, 0.0 )
		elif Color == 4 :
			Actor.GetProperty().SetColor( 0.0, 1.0, 1.0 )
		else :
			Actor.GetProperty().SetColor( 1.0, 1.0, 1.0 )
		



	def Show( self ) :
		RenWin = vtkRenderWindow()
		RenWin.AddRenderer(self.Ren)

		Style = vtkInteractorStyleTrackball()
		Style.SetTrackballModeToTrackball()
		iRen = vtkRenderWindowInteractor()
		iRen.SetInteractorStyle( Style )
		iRen.SetRenderWindow(RenWin)

		RenWin.SetSize(500,500)
		self.Ren.SetBackground(.6, .6, .6 )

		iRen.Initialize()
		iRen.Start()

if __name__ == '__main__' :
	argc = len( sys.argv )
	viewer = XdmfMeshViewer( sys.argv[ argc - 1 ] )
	viewer.Show()
