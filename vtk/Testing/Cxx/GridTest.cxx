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
  int cc;
  for ( cc = 0; cc < reader->GetNumberOfDomains(); cc ++ )
    {
    cout << "Domain [" << cc << "] = " << reader->GetDomainName(cc) << endl;
    }
  reader->SetDomainName("Domain0");
  reader->UpdateInformation();
  for ( cc = 0; cc < reader->GetNumberOfGrids(); cc ++ )
    {
    cout << "Grid [" << cc << "] = " << reader->GetGridName(cc) << endl;
    }
  reader->EnableGrid("Atoms");
  reader->UpdateInformation();
  reader->Update();

  reader->EnableGrid("Bonds");
  reader->UpdateInformation();

  reader->Update();

  reader->Print(cout);
  
  vtkUnstructuredGrid* gr = vtkUnstructuredGrid::SafeDownCast(reader->GetOutput(0));
  if ( gr )
    {
    cout << "Write vtu 0" << endl;
    vtkXMLUnstructuredGridWriter* writer = vtkXMLUnstructuredGridWriter::New();
    writer->SetFileName("foo_atoms1.vtu");
    writer->SetInput(gr);
    writer->Write();
    writer->Delete();
    }
  gr = vtkUnstructuredGrid::SafeDownCast(reader->GetOutput(1));
  if ( gr )
    {
    cout << "Write vtu 1" << endl;
    vtkXMLUnstructuredGridWriter* writer = vtkXMLUnstructuredGridWriter::New();
    writer->SetFileName("foo_bonds1.vtu");
    writer->SetInput(gr);
    writer->Write();
    writer->Delete();
    }
  reader->DisableAllGrids();
  reader->EnableGrid("Bonds");
  reader->Update();

  gr = vtkUnstructuredGrid::SafeDownCast(reader->GetOutput(0));
  if ( gr )
    {
    cout << "Write vtu" << endl;
    vtkXMLUnstructuredGridWriter* writer = vtkXMLUnstructuredGridWriter::New();
    writer->SetFileName("foo_bonds2.vtu");
    writer->SetInput(gr);
    writer->Write();
    writer->Delete();
    gr->GetFieldData()->Print(cout);
    }

  reader->EnableGrid("Atoms");
  reader->Update();
  gr = vtkUnstructuredGrid::SafeDownCast(reader->GetOutput(0));
  if ( gr )
    {
    cout << "Write vtu" << endl;
    vtkXMLUnstructuredGridWriter* writer = vtkXMLUnstructuredGridWriter::New();
    writer->SetFileName("foo_atoms2.vtu");
    writer->SetInput(gr);
    writer->Write();
    writer->Delete();
    gr->GetFieldData()->Print(cout);
    }

  reader->Delete();
  cout << "Done..." << endl;
  return 0;
}
