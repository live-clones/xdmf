#include "vtkMyXdmfReader.h"

int main()
{
  vtkMyXdmfReader* vr = vtkMyXdmfReader::New();
  vr->Print(cout);
  vr->Delete();
  return 0;
}
