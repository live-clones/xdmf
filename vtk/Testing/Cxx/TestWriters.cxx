#include "vtkXdmfWriter.h"
#include "vtkSphereSource.h"
#include "vtkPolyData.h"
#include "vtkXdmfReader.h"
#include "vtkUnstructuredGrid.h"
#include "vtkIdList.h"
#include "vtkCellData.h"
#include "vtkPointData.h"
#include "vtkImageNoiseSource.h"
#include "vtkImageData.h"

#define OutVector3(x) (x)[0] << "," << (x)[1] << "," << (x)[2]
#define ABS(x) (((x)<0)?-(x):(x))
#define MAX(x,y) (((x)>(y))?(x):(y))

template<class D1, class D2, class DiffType>
int CompareTwoSimilarItems(D1 i1, D2 i2, DiffType diff)
{
  if ( diff == 0 )
    {
    return i1 == i2;
    }
  DiffType rdiff = ABS(i1 - i2);
  if ( rdiff < (diff * .00001) )
    {
    return 1;
    }
  return 0;
}

template<class D1, class D2>
int CompareTwoItems(D1 i1, D2 i2)
{
  return i1 == i2;
}

#define TestCompareTwoItems(x, y, str) \
  if ( !CompareTwoItems((x), (y)) ) \
    { \
    cerr << __LINE__ << " " << str << " " << (x) << " <> " << (y) << endl; \
    return 0; \
    }
#define TestCompareTwoSimilarItems(x, y, str) \
  if ( !CompareTwoSimilarItems((x), (y), MAX(ABS(x),ABS(y))) ) \
    { \
    cerr << __LINE__ << " " << str << " " << (x) << " <> " << (y) << endl; \
    return 0; \
    }

template<class InputDataType, class OutputDataType>
OutputDataType* TestWriter(InputDataType* input, const char* fname, 
  OutputDataType*, int alllight)
{
  vtkXdmfWriter *writer = vtkXdmfWriter::New();
  writer->SetFileName(fname);
  writer->SetInput(input);
  writer->SetAllLight(alllight);
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

int CompareArrays(vtkDataArray* daArray, vtkDataArray *dbArray)
{
  vtkIdType tuple, component;
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
      TestCompareTwoSimilarItems(daVal, dbVal,
        "Value for tuple " << tuple << " component " << component << " are not the same");
      }
    }
  return 1;
}

int CompareFieldData(vtkFieldData* da, vtkFieldData* db)
{
  TestCompareTwoItems(da->GetNumberOfArrays(),
    db->GetNumberOfArrays(),
    "Number of arrays is not the same");
  vtkIdType array;
  cout << "Found " << da->GetNumberOfArrays() << " arrays" << endl;
  for ( array = 0; array < da->GetNumberOfArrays(); array ++ )
    {
    const char* name = da->GetArrayName(array);
    vtkDataArray* daArray = da->GetArray(name);
    vtkDataArray* dbArray = db->GetArray(name);
    if ( !name )
      {
      daArray = da->GetArray(array);
      dbArray = db->GetArray(array);
      }
    if ( !daArray || !dbArray )
      {
      cerr << "Cannot find array named " << name << " (" << daArray  << " " << dbArray << ")" << endl;
      return 0;
      }
    int res = ::CompareArrays(daArray, dbArray);
    if ( !res )
      {
      return 0;
      }
    }
  return 1;
}

int CompareDataSets(vtkDataSet* pd, vtkDataSet* ug)
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
      TestCompareTwoSimilarItems(pdPoint[kk], ugPoint[kk],
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

  cout << "Point arrays" << endl;
  if ( !CompareFieldData(pd->GetPointData(), ug->GetPointData()) )
    {
    return 0;
    }
  cout << "Cell arrays" << endl;
  if ( !CompareFieldData(pd->GetCellData(), ug->GetCellData()) )
    {
    return 0;
    }

  return 1;
}

int TestPolyData(vtkPolyData* pd, const char* filename, int alllight)
{
  cout << "** Preform PolyData test" << endl;
  vtkUnstructuredGrid* ug 
    = ::TestWriter(pd, filename, (vtkUnstructuredGrid*)0,
      alllight);
  if ( !ug )
    {
    return 1;
    }

  int res = CompareDataSets(pd, ug);

  ug->Delete();
  if ( !res )
    {
    cerr << "Errors while running poly data test" << endl;
    return 1;
    }
  return 0;
}

template<class DataType>
int TestSimpleData(DataType* orData, const char* filename, int alllight)
{
  cout << "** Preform simple data test" << endl;
  DataType* rdData = ::TestWriter(orData, filename, (DataType*)0,
      alllight);
  if ( !rdData )
    {
    return 1;
    }

  int res = CompareDataSets(orData, rdData);

  rdData->Delete();
  if ( !res )
    {
    cerr << "Errors while running poly data test" << endl;
    return 1;
    }
  return 0;
}

int main(int argc, char* argv[])
{
  if ( argc < 2 )
    {
    cout << "Usage: " << argv[0] << " <xdmf file>" << endl;
    return 0;
    }
  int res = 0;

  vtkSphereSource *sphere = vtkSphereSource::New();
  res += ::TestPolyData(sphere->GetOutput(), argv[1], 0);
  res += ::TestPolyData(sphere->GetOutput(), argv[1], 1);
  sphere->Delete();

  vtkImageNoiseSource* noise= vtkImageNoiseSource::New();
  noise->SetWholeExtent(0, 5, 0, 12, 0, 17);
  noise->Update();
  res += ::TestSimpleData(noise->GetOutput(), argv[1], 1);
  res += ::TestSimpleData(noise->GetOutput(), argv[1], 0);
  noise->Delete();


  if ( res )
    {
    cerr << "Errors while running test" << endl;
    return 1;
    }
  return 0;
}
