#include "vtkXMLUnstructuredGridReader.h"
#include "vtkXdmfWriter.h"
#include "vtkUnstructuredGrid.h"

int main(int argc, char* argv[])
{
  if ( argc != 3 )
    {
    cout << "Usage: " << argv[0] << " <infile> <outfile>" << endl;
    return 1;
    }
  
  vtkXMLUnstructuredGridReader* reader = vtkXMLUnstructuredGridReader::New();
  vtkXdmfWriter* writer = vtkXdmfWriter::New();

  writer->SetInput(reader->GetOutput());
  writer->SetGridName("GridName");
  //writer->AllLightOn();

  
  reader->SetFileName(argv[1]);
  writer->SetFileName(argv[2]);

  writer->Write();

  reader->Delete();
  writer->Delete();
  
  return 0;
}
