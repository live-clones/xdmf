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

from vtk import *
from libvtkXdmfPython import *
from Xdmf import *
# from libVTKLocalPython import *
# from libVTKGraphicsPython import *
# from libVTKCommonPython import *



con = vtkConeSource()
con.SetRadius(1)
con.SetResolution(5)
con.Update()

t = vtkTriangleFilter()
t.SetInput( con.GetOutput() )
t.Update()

DOM = XdmfDOM()
DOM.SetInputFileName('Wall.xml')
DOM.Parse()
Grid = XdmfGrid()
Grid.SetDOM( DOM )
Grid.SetGridFromElement( DOM.FindElement( "Grid" ))
Data = vtkDataSet( Grid.GetBaseGrid())

x = vtkXdmfDataSetWriter()
x.SetInput( Data )
# x.SetInput( t.GetOutput() )
# x.SetInput( con.GetOutput() )
x.SetHeavyDataSetName("Jerry.h5")
x.SetGridName("My Grid");
x.WriteXdmf()

GridXML = x.GetXML()

XMLFile = open( 'Jerry.xml', 'w' )

XMLFile.write('<?xml version="1.0" ?>\n')
XMLFile.write('<!DOCTYPE Xdmf SYSTEM "Xdmf.dtd" [\n')
XMLFile.write('<!ENTITY InitialHeavyData "' + 'Jerry.h5">\n')
XMLFile.write(']>\n')
XMLFile.write('\n\n\n')
XMLFile.write('<Xdmf>\n')
XMLFile.write( GridXML )
XMLFile.write('</Xdmf>')
XMLFile.close()

