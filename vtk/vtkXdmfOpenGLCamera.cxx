#include "vtkXdmfOpenGLCamera.h"

#include "vtkObjectFactory.h"
#include "vtkOpenGLRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkCommand.h"

//----------------------------------------------------------------------------
vtkStandardNewMacro(vtkXdmfOpenGLCamera);

// Implement base class method.
void vtkXdmfOpenGLCamera::Render(vtkRenderer *ren)
{
  // if were on a stereo renderer draw to special parts of screen
/*
  if (this->Stereo) {
    switch ((ren->GetRenderWindow())->GetStereoType())
      {
      case VTK_STEREO_LEFT :
        if( this->ReallyLeft) {
                vtkOpenGLCamera::Render(ren);
        }else{
                (ren->GetRenderWindow())->SetStereoTypeToRight();
                vtkOpenGLCamera::Render(ren);
                (ren->GetRenderWindow())->SetStereoTypeToLeft();
        }
        break;
      default:
        vtkOpenGLCamera::Render(ren);
        break;
      }
    }
*/

  if( this->ReallyLeft) 
    {
    vtkOpenGLCamera::Render(ren);
    }
  else
    {
    (ren->GetRenderWindow())->SetStereoTypeToRight();
    vtkOpenGLCamera::Render(ren);
    (ren->GetRenderWindow())->SetStereoTypeToLeft();
    }
}
