#include "vtkOpenGLCamera.h"

class VTK_EXPORT vtkXdmfOpenGLCamera : public vtkOpenGLCamera
{
public:
	static vtkXdmfOpenGLCamera *New();
	vtkTypeMacro(vtkXdmfOpenGLCamera, vtkOpenGLCamera);
	virtual void SetReallyLeft( int ReallyLeft ){
		this->ReallyLeft = ReallyLeft;
		}
	virtual int GetReallyLeft(void){
		return(this->ReallyLeft);
		}
	virtual void Render(vtkRenderer *ren);
protected:
	int	ReallyLeft;
};

