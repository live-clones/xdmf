#include "vtkXdmfReader.h"
#include "vtkXMLUnstructuredGridWriter.h"
#include "vtkUnstructuredGrid.h"
#include "vtkFieldData.h"


int main(int argc, char* argv[])
{
  if ( argc < 2 )
    {
    cout << "Usage: " << argv[0] << " <file>" << endl;
    return 0;
    }
  vtkXdmfReader* reader = vtkXdmfReader::New();
  reader->SetFileName(argv[1]);
  reader->UpdateInformation();
  reader->DisableAllGrids();
  reader->EnableGrid(0);
//  reader->GetOutput()->SetUpdateExtent(0, 1, 0, 1, 0, 1);
//reader->SetStride(1,2,1);
  reader->Update();
//  reader->GetOutput(0)->Print(cout);

  reader->Delete();
  cout << "Done..." << endl;
  return 0;
}
