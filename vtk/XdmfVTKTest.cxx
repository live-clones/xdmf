#include "vtkXdmfReader.h"

int main()
{
  vtkXdmfReader* xdmf = vtkXdmfReader::New();
  xdmf->Delete();
  return 0;
}
