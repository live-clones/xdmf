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

print 'Loading Xdmf'
import Xdmf
print 'Loading vtk'
from vtk import *
from libvtkXdmfPython import *

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

			VtkGrid = vtkDataSet( Grid.GetBaseGrid() )


			GeometryFilter = vtkGeometryFilter()
			GeometryFilter.SetInput( VtkGrid )
			GeometryFilter.Update()

			Mapper = vtkPolyDataMapper()
			Mapper.SetInput( GeometryFilter.GetOutput() )

			Actor = vtkActor()
			Actor.SetMapper( Mapper )
			self.SetColor( Actor, Index )
			Actor.GetProperty().SetRepresentationToWireframe()

			Ren.AddActor( Actor )

			Index += 1
			node  = DOM.FindElement("Grid", Index)

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
		self.Ren.SetBackground(1, 1, 1)

		iRen.Initialize()
		iRen.Start()

if __name__ == '__main__' :
	argc = len( sys.argv )
	viewer = XdmfMeshViewer( sys.argv[ argc - 1 ] )
	viewer.Show()
