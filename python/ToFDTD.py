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

import os
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
		self.FileName = FileName
		self.CheckCellWidth()
		self.IsWedge = 0
		BaseList = string.split(FileName, '.')
		if len( BaseList ) == 1 :
			self.BaseName = BaseList[0]
		else :
			self.BaseName = string.join( BaseList[ : len( BaseList ) - 1 ] )
			if BaseList[-1] == 'vtk' :
				print 'Using Vtk Wedge File'
				self.IsTriangle = 1
				self.IsWedge = 1
				return
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
		if self.IsWedge :
			Reader = vtkUnstructuredGridReader()
			Reader.SetFileName( self.FileName )
			Reader.Update()
			Wedges = Reader.GetOutput()
		else :
			node  = self.DOM.FindElement("Grid", Index)
			if node != None :
				print 'Reading Heavy Data'
				Grid = Xdmf.XdmfGrid()
				Grid.SetDOM( self.DOM )
				Grid.SetGridFromElement( node )

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

				Clean = vtkCleanPolyData()

				if self.IsTriangle == 0 :
					Tri = vtkTriangleFilter()
					Tri.SetInput( GeometryFilter.GetOutput() )
					Tri.Update()
					Clean.SetInput( Tri.GetOutput() )
				else :
					Clean.SetInput( GeometryFilter.GetOutput() )
	
				Normals = vtkPolyDataNormals()
				Normals.SetInput( Clean.GetOutput() )
				Normals.SplittingOn()
				Normals.ConsistencyOn()
				Normals.SetFeatureAngle(0)
				Normals.FlipNormalsOff()
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

		CellWidth = self.CellWidth
		# Add Two Cells Around the Edges
		# Grid sizw is 1 + Cell Size
		idim = 5 + int(( xmax - xmin ) / CellWidth )
		jdim = 5 + int(( ymax - ymin ) / CellWidth )
		kdim = 5 + int(( zmax - zmin ) / CellWidth )

		print 'X : ' + str( idim ) + ' Cells, ' + str( xmin ) + ' -> ' + str( xmax )
		print 'Y : ' + str( jdim ) + ' Cells, ' + str( ymin ) + ' -> ' + str( ymax )
		print 'Z : ' + str( kdim ) + ' Cells, ' + str( zmin ) + ' -> ' + str( zmax )

		print 'Creating Structured Grid'
		self.Modeller = vtkImplicitModeller()
		self.Modeller.SetSampleDimensions( idim, jdim, kdim )
		self.Modeller.SetModelBounds(xmin, xmax, ymin, ymax, zmin, zmax )
		self.Modeller.SetInput( Wedges )
		self.Modeller.SetProcessModeToPerVoxel()
		self.Modeller.SetNumberOfThreads( 10 )
		self.Modeller.SetMaximumDistance( 0.001 )
		self.Modeller.SetProgressMethod( self.PercentDone )
		self.Modeller.Update()

	def WriteFiles( self ) :

			print 'Convert Node Data To Cell Data'
			PointToCell = vtkPointDataToCellData()
			PointToCell.SetInput( self.Modeller.GetOutput() )
			PointToCell.Update()
			ScalarRange = PointToCell.GetOutput().GetScalarRange()
			print 'Scalar Range = ' + str( ScalarRange )
			print 'Writing HDF5 Data'
			Writer = vtkXdmfDataSetWriter()
			Writer.SetInput(  PointToCell.GetOutput() )
			HeavyDataName = self.BaseName + 'FDTD.h5'
			if os.path.exists( HeavyDataName ) :
				print 'Removing Old ' + HeavyDataName
				os.remove( HeavyDataName )
			Writer.SetHeavyDataSetName(self.BaseName + 'FDTD.h5')
			Writer.WriteGrid()
			Writer.WriteAttributes()
			XML = ''
			XML += '<?xml version="1.0" ?>\n'
			XML += '<!DOCTYPE Xdmf SYSTEM "Xdmf.dtd" [\n'
			XML += '<!ENTITY HeavyData "' + \
				self.BaseName + 'FDTD.h5" >\n'
			XML += ']>\n'
			XML += '<!--\n'
			XML += '\t Generated on\n'
			XML += '\t ' + time.strftime("%a, %d, %b %Y %H:%M", time.localtime()) + '\n'
			XML += '\t\t Frequency = ' + str( self.Frequency ) + ' GHz\n'
			XML += '\t\t Thickness = ' + str( self.Thickness ) + '\n'
			XML += '\t\t Cell Width = ' + str( self.CellWidth) + '\n'
			XML += '-->\n'
			XML += '<Xdmf>\n'
			XML += '<Domain>\n'
			XML += '<Grid>>\n'
			XML += Writer.GetXML()
			XML += '</Grid>>\n'
			XML += '</Domain>\n'
			XML += '</Xdmf>\n'
			fd = open(self.BaseName + 'FDTD.xml', "w" )
			fd.write( XML )
			fd.close()

			Dims = self.Modeller.GetSampleDimensions()
			idim = Dims[0] - 1
			jdim = Dims[1] - 1
			kdim = Dims[2] - 1
			Bounds = self.Modeller.GetModelBounds()
			print 'Writing FDTD Input File'
			fd = open(self.BaseName + 'FDTD.in', "w" )
			fd.write( str( idim ) + ' ')
			fd.write( str( jdim ) + ' ')
			fd.write( str( kdim ) + '\n')
			fd.write('2\n')
			fd.write('1 255 255 255\n')
			fd.write('2 255 255 0\n')
			Data = PointToCell.GetOutput().GetCellData().GetScalars()
			for i in range( Data.GetNumberOfScalars() ) :
				Value = Data.GetScalar( i )
				if ( Value <= 0 ) :
					fd.write('2\n')
				else :
					fd.write('1\n')
			fd.close()

	def Show( self ) :
		print 'Thresholding'
		SgridThresh = vtkThreshold()
		SgridThresh.SetInput( self.Modeller.GetOutput() )
		SgridThresh.ThresholdByLower( 1.0 )
		# SgridThresh.SetAttributeModeToUseCellData()
		SgridThresh.SetProgressMethod( self.PercentDone )

		SgridGeo = vtkGeometryFilter()
		SgridGeo.SetInput( SgridThresh.GetOutput() )

		x = vtkStructuredPointsGeometryFilter()
		x.SetInput( self.Modeller.GetOutput() )
		x.SetExtent( 0, 10000, 0, 10000, 0, 10000 )

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
		gridder.WriteFiles()
				



if __name__ == '__main__' :
	gridder = ToFDTD()
	gridder.main()
	gridder.Show()
