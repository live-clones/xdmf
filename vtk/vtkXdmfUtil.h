/*******************************************************************/
/*                               XDMF                              */
/*                   eXtensible Data Model and Format              */
/*                                                                 */
/*  Id : Id  */
/*  Date : $Date$ */
/*  Version : $Revision$ */
/*                                                                 */
/*  Author:                                                        */
/*     Jerry A. Clarke                                             */
/*     clarke@arl.army.mil                                         */
/*     US Army Research Laboratory                                 */
/*     Aberdeen Proving Ground, MD                                 */
/*                                                                 */
/*     Copyright @ 2002 US Army Research Laboratory                */
/*     All Rights Reserved                                         */
/*     See Copyright.txt or http://www.arl.hpc.mil/ice for details */
/*                                                                 */
/*     This software is distributed WITHOUT ANY WARRANTY; without  */
/*     even the implied warranty of MERCHANTABILITY or FITNESS     */
/*     FOR A PARTICULAR PURPOSE.  See the above copyright notice   */
/*     for more information.                                       */
/*                                                                 */
/*******************************************************************/
#ifndef _vtkXdmfUtil_h
#define _vtkXdmfUtil_h

#include <vtkCell.h>
#include <vtkUnstructuredGrid.h>
#include <vtkIntArray.h>

class VTK_EXPORT vtkXdmfUtil : public vtkObject
{
public:

  vtkXdmfUtil();
  static vtkXdmfUtil *New();
  vtkTypeMacro(vtkXdmfUtil,vtkObject);

  int IsInside( vtkCell *Cell, float X, float Y, float Z );  
  vtkIntArray *GetInside( vtkUnstructuredGrid *Cells, vtkPoints *Points );
  vtkIntArray *GetInsideRect( vtkUnstructuredGrid *Cells, int Kdim, float zstart, float dz, int Jdim, float ystart, float dy, int Idim, float xstart, float dx);


};
#endif /* _vtkXdmfUtil_h */
