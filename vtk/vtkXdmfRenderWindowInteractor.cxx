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
#include <vtkXdmfRenderWindowInteractor.h>


#include <vtkObjectFactory.h>
#include <vtkCommand.h>

//----------------------------------------------------------------------------
vtkXdmfRenderWindowInteractor* vtkXdmfRenderWindowInteractor::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkXdmfRenderWindowInteractor");
  if(ret)
    {
    return (vtkXdmfRenderWindowInteractor*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkXdmfRenderWindowInteractor;
}

void vtkXdmfRenderWindowInteractor::Start( int Block ) {

if ( Block ) {
// Check for WIN32 but without Cygwin with X11
#if defined(_WIN32) && !defined(VTK_USE_OGLR)
  vtkWin32RenderWindowInteractor::Start();
#else
  vtkXRenderWindowInteractor::Start();
#endif
} else {
  this->LoopOnce();
}

}

void vtkXdmfRenderWindowInteractor::LoopOnce( )
{
// Check for WIN32 but without Cygwin with X11
#if defined(_WIN32) && !defined(VTK_USE_OGLR)
    MSG msg;
    while (PeekMessage ( &msg, NULL, 0, 0, PM_NOREMOVE)) {
	if(GetMessage(&msg, NULL, 0, 0)){
		TranslateMessage(&msg);
		DispatchMessage(&msg);	
		}
    }
#else
    XEvent event;

  if (!this->Initialized)
    {
    this->Initialize();
    }
  if (! this->Initialized )
    {
    return;
    }

    this->BreakLoopFlag = 0;
  while( XtAppPending( this->App )) {
        XtAppNextEvent(this->App, &event);
        XtDispatchEvent(&event);
  }
#endif
}
