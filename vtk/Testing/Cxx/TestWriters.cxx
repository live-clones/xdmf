#include "vtkXdmfWriter.h"
#include "vtkSphereSource.h"
#include "vtkPolyData.h"
#include "vtkXdmfReader.h"
#include "vtkUnstructuredGrid.h"
#include "vtkIdList.h"
#include "vtkCellData.h"
#include "vtkPointData.h"

#define OutVector3(x) (x)[0] << "," << (x)[1] << "," << (x)[2]
#define ABS(x) (((x)<0)?-(x):(x))

#define TestCompareTwoItems(x, y, str) \
  if ( (x) != (y) ) \
    { \
    cerr << __LINE__ << " " << str << " " << (x) << " <> " << (y) << endl; \
    return 0; \
    }

template<class InputDataType, class OutputDataType>
OutputDataType* TestWriter(InputDataType* input, const char* fname, 
  OutputDataType*)
{
  vtkXdmfWriter *writer = vtkXdmfWriter::New();
  writer->SetFileName(fname);
  writer->SetInput(input);
  //writer->AllLightOn();
  writer->Write();
  writer->Delete();

  vtkXdmfReader *reader = vtkXdmfReader::New();
  reader->SetFileName(fname);
  reader->UpdateInformation();
  reader->EnableAllGrids();
  reader->Update();
  OutputDataType* dt = OutputDataType::SafeDownCast(reader->GetOutput());
  if ( dt )
    {
    dt->Register(0);
    }
  else
    {
    cerr << "Output of reader is not of specified type" << endl;
    }
  reader->Delete();
  return dt;
}

int CompareFieldData(vtkFieldData* da, vtkFieldData* db)
{
  TestCompareTwoItems(da->GetNumberOfArrays(),
    db->GetNumberOfArrays(),
    "Number of arrays is not the same");
  vtkIdType array, tuple, component;
  for ( array = 0; array < da->GetNumberOfArrays(); array ++ )
    {
    const char* name = da->GetArrayName(array);
    vtkDataArray* daArray = da->GetArray(name);
    vtkDataArray* dbArray = db->GetArray(name);
    if ( !daArray || !dbArray )
      {
      cerr << "Cannot find array named " << name << " (" << daArray  << " " << dbArray << ")" << endl;
      return 0;
      }
    TestCompareTwoItems(daArray->GetNumberOfTuples(),
      dbArray->GetNumberOfTuples(),
      "Number of tuples is not the same");
    TestCompareTwoItems(daArray->GetNumberOfComponents(),
      dbArray->GetNumberOfComponents(),
      "Number of components is not the same");
    for ( tuple = 0; tuple < daArray->GetNumberOfTuples(); tuple ++ )
      {
      for ( component = 0; component < daArray->GetNumberOfComponents(); component ++ )
        {
        double daVal = daArray->GetComponent(tuple, component);
        double dbVal = dbArray->GetComponent(tuple, component);
        TestCompareTwoItems(daVal, dbVal,
          "Value for tuple " << tuple << " component " << component << " are not the same");
        }
      }
    }
  return 1;
}

int CompareDataSets(vtkPolyData* pd, vtkUnstructuredGrid* ug)
{
  int pdNumCells = pd->GetNumberOfCells();
  int ugNumCells = ug->GetNumberOfCells();

  TestCompareTwoItems(pdNumCells, ugNumCells, "Number of cells not the same");

  int pdNumPts= pd->GetNumberOfPoints();
  int ugNumPts= ug->GetNumberOfPoints();

  TestCompareTwoItems(pdNumPts, ugNumPts, "Number of points not the same");

  vtkIdType cc;
  vtkIdType kk;
  for ( cc = 0; cc < pdNumPts; cc ++ )
    {
    double* pdPoint = pd->GetPoint(cc);
    double* ugPoint = ug->GetPoint(cc);
    double diff[3];
    diff[0] = pdPoint[0] - ugPoint[0];
    diff[1] = pdPoint[1] - ugPoint[1];
    diff[2] = pdPoint[2] - ugPoint[2];
    for ( kk = 0; kk < 3; kk ++ )
      {
      TestCompareTwoItems(pdPoint[kk], ugPoint[kk],
        "Points " << cc << " are not the same: " << OutVector3(pdPoint) << " <> " << OutVector3(ugPoint) << endl << "Difference: " << OutVector3(diff));
      }
    }
  vtkIdList* pdCellPts = vtkIdList::New();
  vtkIdList* ugCellPts = vtkIdList::New();
  for ( cc = 0; cc < pdNumCells; cc ++ )
    {
    TestCompareTwoItems(pd->GetCellType(cc), ug->GetCellType(cc), 
      "Cells are not the same type");
    pd->GetCellPoints(cc, pdCellPts);
    ug->GetCellPoints(cc, ugCellPts);
    TestCompareTwoItems(pdCellPts->GetNumberOfIds(),
      ugCellPts->GetNumberOfIds(),
      "Number of points for the cell " << cc << " are not the same");
    for ( kk = 0; kk < pdCellPts->GetNumberOfIds(); kk ++ )
      {
      TestCompareTwoItems(pdCellPts->GetId(kk), ugCellPts->GetId(kk),
        "Ids of point " << kk << " for cell " << cc << " are not the same");
      }
    }
  pdCellPts->Delete();
  ugCellPts->Delete();

  if ( !CompareFieldData(pd->GetPointData(), ug->GetPointData()) )
    {
    return 0;
    }
  if ( !CompareFieldData(pd->GetCellData(), ug->GetCellData()) )
    {
    return 0;
    }

  return 1;
}

int main(int argc, char* argv[])
{
  if ( argc < 2 )
    {
    cout << "Usage: " << argv[0] << " <xdmf file>" << endl;
    return 0;
    }

  vtkSphereSource *sphere = vtkSphereSource::New();
  vtkUnstructuredGrid* ug 
    = ::TestWriter(sphere->GetOutput(), argv[1], (vtkUnstructuredGrid*)0);
  if ( !ug )
    {
    sphere->Delete();
    return 1;
    }

  int res = CompareDataSets(sphere->GetOutput(), ug);

  ug->Delete();
  sphere->Delete();
  if ( !res )
    {
    cerr << "Errors while running test" << endl;
    return 1;
    }
  return 0;
}
