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

# print 'Loading Xdmf'
# import Xdmf

print 'Loading vtk'
from libVTKCommonPython import *
from libVTKGraphicsPython import *
from libVTKParallelPython import *
from libVTKContribPython import *

VtkSock = vtkSocketController()
VtkSock.Initialize()

VtkInput = vtkInputPort()
VtkInput.SetController( VtkSock )
VtkInput.SetTag( 12 )
VtkInput.SetRemoteProcessId( 1 )
VtkInput.ReleaseDataFlagOff
# VtkSock.ConnectTo( 'crab', 11000 )
argc = len( sys.argv )
VtkSock.ConnectTo( sys.argv[ argc - 1 ], 11000 )

Mapper = vtkPolyDataMapper()
Mapper.SetInput( VtkInput.GetPolyDataOutput() )

Actor = vtkActor()
Actor.SetMapper( Mapper )

Render = vtkRenderer()
Render.AddActor( Actor )

RenWin = vtkRenderWindow()
RenWin.AddRenderer( Render )

IRen = vtkRenderWindowInteractor()
IRen.SetRenderWindow( RenWin )

vtkObject.GlobalWarningDisplayOn()


RenWin.Render()
VtkInput.Update()
VtkInput.UnRegisterAllOutputs()
IRen.Start()
