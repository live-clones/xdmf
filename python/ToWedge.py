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


import getopt
import sys
import time
import string


print 'Loading vtk...'
from libVTKCommonPython import *
from libVTKGraphicsPython import *
from libVTKParallelPython import *
from libVTKPatentedPython import *
from libVTKImagingPython import *
from libVTKLocalPython import *

print 'Loading Xdmf...'
import Xdmf

class ToFDTD :

	def __init__( self ):
		self.Thickness = .5
		self.Frequency = 1.0	# in Ghz
		self.CellWidth = .015	# l for 1Ghz / 20
		self.IsTriangle = 0
		self.Flip = 0
		self.Verbose = 0
		self.Adjust = 1
		self.CellsPerLength = 20.0

	def SetCellsPerLength( self, CellsPerLength ) :
		self.CellsPerLength = CellsPerLength

	def GetCellsPerLength( self ) :
		return self.CellsPerLength

	def SetThickness( self, Thickness ) :
		self.Thickness = Thickness

	def GetThickness( self ) :
		return self.Thickness

	def SetCellWidth( self, CellWidth ) :
		self.CellWidth = CellWidth

	def GetCellWidth( self ) :
		return self.CellWidth

	def SetFrequency( self, Frequency ) :
		self.Frequency = Frequency
		WaveLength = 0.3 / float(Frequency) # l = 3x10^8 / hz
		self.SetCellWidth( WaveLength / self.CellsPerLength ) # 20 cells / l
		print 'Cell Width for ' + str( Frequency ) + 'Ghz = ' + str( self.GetCellWidth() )

	def GetFrequency( self ) :
		return self.Frequency

	def SetFlip( self, Flip ) :
		self.Flip = Flip

	def GetFlip( self ) :
		return self.Flip

	def CheckCellWidth( self ) :
		ideal = self.Thickness / 4.0
		if self.CellWidth > ideal  :
			print 'CellWidth Too Big'
			print 'Was ' + str( self.CellWidth )
			if self.Adjust == 1 :
				print 'Setting to ' + str(ideal)
				self.CellWidth = ideal
			else :
				print 'Suggest ' + str(ideal) + ' but --noadjust was set'

	def Parse( self, FileName ) :
		BaseList = string.split(FileName, '.')
		if len( BaseList ) == 1 :
			self.BaseName = BaseList[0]
		else :
			self.BaseName = string.join( BaseList[ : len( BaseList ) - 1 ] )
		self.CheckCellWidth()
		print 'Parsing ' + FileName
		self.DOM = Xdmf.XdmfDOM()
		self.DOM.SetInputFileName( FileName )
		self.DOM.Parse()
		node  = self.DOM.FindElement("Grid")
		if node == None :
			print 'XML file has no <Grid> Element'
			return None
		top = self.DOM.FindElement("Topology", 0, node )
		type = self.DOM.Get( top, "Type" )
		print 'Grid Topology = ' + type
		if type == 'Triangle' :
			self.IsTriangle = 1
		

	def Execute( self ) :
		Ren = vtkRenderer()
		self.Ren = Ren
		Index = 0
		Thickness = self.Thickness
		node  = self.DOM.FindElement("Grid", Index)
		if node != None :
			print 'Reading Heavy Data'
			Grid = Xdmf.XdmfGrid()
			Grid.SetDOM( self.DOM )
			Grid.SetGridFromElement( node )
			# Grid.AssignAttribute( 0 )

			
			# CurrentAttribute = Grid.GetAssignedAttribute()
			# ValueArray = CurrentAttribute.GetValues()
			# ValueArray.Generate(Thickness, Thickness)
			# CurrentAttribute.SetBaseAttribute( Grid, Grid.GetBaseGrid() )

			VtkGrid = vtkDataSet( Grid.GetBaseGrid() )
			Scalars = vtkScalars()
			Scalars.SetNumberOfScalars( VtkGrid.GetNumberOfPoints() )
			for i in range( VtkGrid.GetNumberOfPoints() ) :
				Scalars.SetScalar( i, self.Thickness )
			VtkGrid.GetPointData().SetScalars( Scalars )


			print 'Calculating Normals'
			GeometryFilter = vtkGeometryFilter()
			GeometryFilter.SetInput( VtkGrid )
			GeometryFilter.Update()

			Normals = vtkPolyDataNormals()

			if self.IsTriangle == 0 :
				Tri = vtkTriangleFilter()
				Tri.SetInput( GeometryFilter.GetOutput() )
				Tri.Update()
				Normals.SetInput( Tri.GetOutput() )
			else :
				Normals.SetInput( GeometryFilter.GetOutput() )

			Normals.SplittingOff()
			Normals.SetFeatureAngle(0)
			Normals.FlipNormalsOff()
			Normals.ConsistencyOn()
			Normals.SetNonManifoldTraversal( 1 )
			Normals.Update()

			print 'Building Wedges'
			warp = vtkWarpScalar()
			warp.SetInput( Normals.GetOutput() )
			if self.Flip == 0 :
				warp.SetScaleFactor( 1.0  )
			else :
				warp.SetScaleFactor( -1.0  )
			warp.Update()

			if self.IsTriangle == 0 :
				Wedges = self.BuildWedges( Tri.GetOutput(), warp.GetOutput() )
			else :
				Wedges = self.BuildWedges( GeometryFilter.GetOutput(), warp.GetOutput() )

			Bounds = Wedges.GetBounds()
			xmin = Bounds[0]
			xmax = Bounds[1]
			ymin = Bounds[2]
			ymax = Bounds[3]
			zmin = Bounds[4]
			zmax = Bounds[5]

			self.Wedges = Wedges

			# Writer = vtkPolyDataWriter()
			Writer = vtkUnstructuredGridWriter()
			Writer.SetInput( Wedges )
			Writer.SetFileName( self.BaseName + 'Wedges.vtk' )
			Writer.SetFileTypeToBinary()
			Writer.Write()

			CellWidth = self.CellWidth
			# Add Two Cells Around the Edges
			# Grid sizw is 1 + Cell Size
			idim = 5 + int(( xmax - xmin ) / CellWidth )
			jdim = 5 + int(( ymax - ymin ) / CellWidth )
			kdim = 5 + int(( zmax - zmin ) / CellWidth )

			print 'X : ' + str( idim ) + ' Cells, ' + str( xmin ) + ' -> ' + str( xmax )
			print 'Y : ' + str( jdim ) + ' Cells, ' + str( ymin ) + ' -> ' + str( ymax )
			print 'Z : ' + str( kdim ) + ' Cells, ' + str( zmin ) + ' -> ' + str( zmax )
	
	def Show( self ) :

		SgridGeo = vtkGeometryFilter()
		SgridGeo.SetInput( self.Wedges )

		SgridMapper = vtkPolyDataMapper()
		SgridMapper.SetInput( SgridGeo.GetOutput() )

		self.SgridActor = vtkActor()
		self.SgridActor.SetMapper( SgridMapper )
		self.SgridActor.GetProperty().SetPointSize( 4 )

		self.Ren = vtkRenderer()
		self.Ren.AddActor( self.SgridActor )
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

	def PercentDone( self ) :
		pd = self.Modeller.GetProgress()
		if self.Verbose == 1 :
			print 'Filter  ' + str( int( pd * 100.0 ) ) + "% Done"

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
		Cntr = 0
		Total = 0
		for i in range( NumberOfPolys ) :
			old = OrigTri.GetCell( i )
			new = WarpTri.GetCell( i )
			NewWedge.GetPointIds().SetId(0, old.GetPointId(0) )
			NewWedge.GetPointIds().SetId(1, old.GetPointId(1) )
			NewWedge.GetPointIds().SetId(2, old.GetPointId(2) )

			NewWedge.GetPointIds().SetId(3, NumberOfPoints + new.GetPointId(0) )
			NewWedge.GetPointIds().SetId(4, NumberOfPoints + new.GetPointId(1) )
			NewWedge.GetPointIds().SetId(5, NumberOfPoints + new.GetPointId(2) )

			NewGrid.InsertNextCell( NewWedge.GetCellType(), NewWedge.GetPointIds())
			if Cntr >= 10000 :
				Percent = int(100.0 * float(Total) / float(NumberOfPolys))
				print str(Percent) + '% Completed ' + str( Total ) + ' of ' + str( NumberOfPolys) +' Cells'
				Cntr = 0
			Total += 1
			Cntr += 1
		NewGrid.GetPointData().SetScalars(  Append.GetOutput().GetPointData().GetScalars() )
		return NewGrid
			
	def usage( self ) :
		print 'Usage : ' + sys.argv[0] + ' [--cells=c --noadjust --thickness=t --reverse --frequency=f --verbose] Input.xml'

	def main( self ) :
		argc = len( sys.argv )
		try :
			opts, args = getopt.getopt(sys.argv[1:],
					"chotrfvn:",
					["help", "cells=", "output=", "thickness=",
						"noadjust", "reverse",
						"frequency=", "verbose" ])
		except getopt.GetoptError:
			self.usage()
			sys.exit(2)
		output = None
		for o, a in opts:
			if o in ("-h", "--help"):
				self.usage()
				sys.exit()
			if o in ("-c", "--cells"):
				self.SetCellsPerLength( int(a) )
			if o in ("-t", "--thickness"):
				self.SetThickness( float(a) )
			if o in ("-r", "--reverse"):
				self.Flip = 1
			if o in ("-n", "--noadjust"):
				self.Adjust = 0
			if o in ("-f", "--frequency"):
				self.SetFrequency( float(a) )
			if o in ("-v", "--verbose"):
				self.Verbose = 1
		self.Parse( sys.argv[ argc - 1 ] )
		gridder.Execute()
				



if __name__ == '__main__' :
	gridder = ToFDTD()
	gridder.main()
	gridder.Show()
