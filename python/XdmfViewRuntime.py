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
import threading
from time import sleep

print 'Loading Xdmf'
import Xdmf

print 'Loading vtk'
from libVTKCommonPython import *
from libVTKGraphicsPython import *

def SetColor ( Actor, Index ) :
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

def UpdateAll () :
	print "Update All Actors"
	Index = 0
	for Grid in Grids :
		print "Update Attribute %d " % Attributes[ Index ]
		Grid.AssignAttribute( Attributes[ Index ] )
		Index += 1

		
argc = len( sys.argv )
FileName = sys.argv[ argc - 1 ]
# FileName = "can.xml"
print 'Parsing ' + FileName
DOM = Xdmf.XdmfDOM()
DOM.SetInputFileName( FileName )
DOM.Parse()
Grids = []
Attributes = []


print 'Reading Heavy Data'
Ren = vtkRenderer()
Index = 0
node  = DOM.FindElement("Grid", Index)
while node != None :
	Grid = Xdmf.XdmfGrid()
	Grid.SetDOM( DOM )
	Grid.SetGridFromElement( node )
	Grids.append( Grid )

	VtkGrid = vtkDataSet( Grid.GetBaseGrid() )


	GeometryFilter = vtkGeometryFilter()
	GeometryFilter.SetInput( VtkGrid )
	Mapper = vtkPolyDataMapper()

	GeometryFilter.Update()
	Mapper.SetInput( GeometryFilter.GetOutput() )
	for i in range(0, Grid.GetNumberOfAttributes() - 1 ) :
		Attr = Grid.GetAttribute( i )
		Name = Attr.GetName()
		if Name == "DISPL" :
			print "Assigning Displacement Vector"
			Attributes.append(i)
			Grid.AssignAttribute( i )
			print "Setting Warp Vector"
			WarpVector = vtkWarpVector()
			WarpVector.SetInput( GeometryFilter.GetOutput() )
			print "Updating"
			WarpVector.Update()
			Mapper.SetInput( WarpVector.GetOutput() )
			break

	Actor = vtkActor()
	Actor.SetMapper( Mapper )
	SetColor( Actor, Index )
	Actor.GetProperty().SetRepresentationToWireframe()

	Ren.AddActor( Actor )

	Index += 1
	node  = DOM.FindElement("Grid", Index)

RenWin = vtkRenderWindow()
RenWin.AddRenderer(Ren)

Style = vtkInteractorStyleTrackball()
Style.SetTrackballModeToTrackball()
iRen = vtkRenderWindowInteractor()
iRen.SetInteractorStyle( Style )
iRen.SetRenderWindow(RenWin)
iRen.SetUserMethod( UpdateAll )

RenWin.SetSize(500,500)
Ren.SetBackground(0, 0, 0)

iRen.Start()


