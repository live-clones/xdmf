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
		node  = DOM.FindElement("Grid", Index)
		while node != None :
			Grid = Xdmf.XdmfGrid()
			Grid.SetDOM( DOM )
			Grid.SetGridFromElement( node )
			Grid.AssignAttributeByName('FakeColor')

			VtkGrid = vtkDataSet( Grid.GetBaseGrid() )


			GeometryFilter = vtkGeometryFilter()
			GeometryFilter.SetInput( VtkGrid )
			GeometryFilter.Update()

			# Deci = vtkDecimate()
			# Deci.SetInput( GeometryFilter.GetOutput() )
			# Deci.SetInitialFeatureAngle( 3 )

			Smooth = vtkPolyDataNormals()
			Smooth.SetInput( GeometryFilter.GetOutput() )
			Smooth.SetFeatureAngle( 20 )


		
			Mapper = vtkPolyDataMapper()
			Mapper.SetInput( Smooth.GetOutput() )
			self.CreateTable( Mapper )
			Mapper.Update()
			# Mapper.SetScalarRange( GeometryFilter.GetOutput().GetScalarRange() )
			Mapper.SetScalarRange( 0, 4095)
			# print 'Range ' + str( Mapper.GetScalarRange() )
			# print 'Values ' + Grid.GetAssignedAttribute().GetValues().GetValues()

			Actor = vtkActor()
			Actor.SetMapper( Mapper )
			# self.SetColor( Actor, Index )
			# Actor.GetProperty().SetRepresentationToWireframe()
			Bounds = Actor.GetBounds()
			print 'Bounds = ' + str( Bounds )

			Ren.AddActor( Actor )

			Index += 1
			node  = DOM.FindElement("Grid", Index)

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
