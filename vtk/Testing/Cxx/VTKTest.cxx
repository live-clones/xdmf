#include "vtkMyXdmfReader.h"
#include "vtkDataSet.h"
#include "vtkRectilinearGrid.h"
#include "vtkCellData.h"
#include "vtkDataArray.h"
#include "vtkDataArraySelection.h"

int main(int argc, char* argv[])
{
  if ( argc == 1 )
    {
    cout << "Usage: " << argv[0] << " <filename>" << endl;
    return 1;
    }
  vtkMyXdmfReader* vr = vtkMyXdmfReader::New();
  vr->SetFileName(argv[1]);
  cout << "------------- UpdateInformation --------" << endl;
  vr->UpdateInformation();

  vtkDataArraySelection* ds = vr->GetCellDataArraySelection();
  ds->DisableAllArrays();
  ds->EnableArray(ds->GetArrayName(0));
  vr->SetStride(2, 3, 4);
  cout << "------------- Update -------------------" << endl;
  vr->Update();
  cout << "------------- Reader: ------------------" << endl;
  //vr->Print(cout);
  cout << "------------- Output: ------------------" << endl;
  cout << "Number of cells: " << vr->GetOutput()->GetNumberOfCells() << endl;
  //vr->GetOutput()->Print(cout);
  vtkRectilinearGrid* rg = vtkRectilinearGrid::SafeDownCast( vr->GetOutput() );
  if ( rg )
    {
    vtkDataArray* array = rg->GetCellData()->GetArray(0);
    //array->Print(cout);
    cout << "Number of tuples of array: " << array->GetNumberOfTuples() << endl;
    cout << "Number of X coords: " << rg->GetXCoordinates()->GetNumberOfTuples() << endl;
    cout << "Number of y coords: " << rg->GetYCoordinates()->GetNumberOfTuples() << endl;
    cout << "Number of z coords: " << rg->GetZCoordinates()->GetNumberOfTuples() << endl;
    }
  cout << "------------- Delete  ------------------" << endl;
  vr->Delete();
  return 0;
}
