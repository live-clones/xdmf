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
import Xdmf

print 'Create Array'
a = Xdmf.XdmfArray()
a.SetShapeFromString('20 3')
a.Generate( 1, 60 )


print 'Set Array'
v = vtkXdmfDataArray()
v.SetArray( a.GetTagName() )


print 'Get VTK DataArray'
v1 = v.GetVtkArray()
print 'Second Tuple = ' + str(v1.GetTuple3(1))
v1.SetTuple3( 1, 100, 200, 300 )
print 'Convert Back To Array'
v.ToArray()
print 'Values = ' + a.GetValues()
