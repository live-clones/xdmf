#include "vtkXMLUnstructuredGridReader.h"
#include "vtkXdmfDataSetWriter.h"
#include "vtkUnstructuredGrid.h"

int main(int argc, char* argv[])
{
  if ( argc != 3 )
    {
    cout << "Usage: " << argv[0] << " <infile> <outfile>" << endl;
    return 1;
    }
  
  vtkXMLUnstructuredGridReader* reader = vtkXMLUnstructuredGridReader::New();
  vtkXdmfDataSetWriter* writer = vtkXdmfDataSetWriter::New();

  writer->SetInput(reader->GetOutput());

  
  reader->SetFileName(argv[1]);
  writer->SetFileName(argv[2]);

  writer->Write();

  reader->Delete();
  writer->Delete();
  
  return 0;
}
