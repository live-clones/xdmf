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
from libVTKCommonPython import *
from libVTKGraphicsPython import *
from libVTKParallelPython import *
from libVTKContribPython import *

VtkSock = vtkSocketController()
VtkSock.Initialize()
VtkOutput = vtkOutputPort()
VtkOutput.SetController( VtkSock )
VtkOutput.SetTag( 12 )
VtkSock.WaitForConnection( 11000, -1 )

argc = len( sys.argv )
FileName = sys.argv[ argc - 1 ]
print 'Parsing ' + FileName
DOM = Xdmf.XdmfDOM()
DOM.SetInputFileName( FileName )
DOM.Parse()
node  = DOM.FindElement("Grid", 2)

print 'Reading Heavy Data'
Grid = Xdmf.XdmfGrid()
Grid.SetDOM( DOM )
Grid.SetGridFromElement( node )
Grid.AssignAttribute(0)

VtkGrid = vtkDataSet( Grid.GetBaseGrid() )


print 'Calling vtk'
GeometryFilter = vtkGeometryFilter()
GeometryFilter.SetInput( VtkGrid )
GeometryFilter.Update()


VtkCone = vtkConeSource()
# VtkOutput.SetInput( VtkCone.GetOutput() )
VtkOutput.SetInput( GeometryFilter.GetOutput() )
print 'Waiting for connection ....'
# VtkOutput.WaitForUpdate()
VtkSock.ProcessRMIs()
print "I'm Done"
VtkSock.Finalize()
sys.exit(0)
