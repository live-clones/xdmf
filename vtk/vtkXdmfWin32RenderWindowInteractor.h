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
#include <vtkWin32RenderWindowInteractor.h>

class VTK_EXPORT vtkXdmfWin32RenderWindowInteractor : public vtkWin32RenderWindowInteractor
{
public:
  static vtkXdmfWin32RenderWindowInteractor *New();
  vtkTypeMacro(vtkXdmfWin32RenderWindowInteractor,vtkWin32RenderWindowInteractor);
  virtual void LoopOnce();
  virtual void Start( int Block = 0 );
};
