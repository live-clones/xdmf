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
#include "vtkXMLRectilinearGridReader.h"
#include "vtkRectilinearGrid.h"
#include "vtkDataArraySelection.h"
#include "vtkStructuredGrid.h"
#include "vtkXMLStructuredGridReader.h"
#include "vtkExtractGrid.h"
#include "vtkExtractRectilinearGrid.h"
#include "vtkExtractVOI.h"

#include "vtkCharArray.h"
#include "vtkShortArray.h"
#include "vtkIntArray.h"
#include "vtkFloatArray.h"
#include "vtkDoubleArray.h"
#include "vtkUnsignedCharArray.h"
#include "vtkUnsignedShortArray.h"
#include "vtkUnsignedIntArray.h"

#include "TestingConstants.h"  

#define OutVector3(x) (x)[0] << "," << (x)[1] << "," << (x)[2]
#define XdmfABS(x) (((x)<0)?-(x):(x))
#define XdmfMAX(x,y) (((x)>(y))?(x):(y))

template<class D1, class D2, class DiffType>
int CompareTwoSimilarItems(D1 i1, D2 i2, DiffType diff)
{
  if ( diff == 0 )
    {
    return i1 == i2;
    }
  DiffType rdiff = XdmfABS(i1 - i2);
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
    cerr << __LINE__ << ": " << str << " " << (x) << " <> " << (y) << endl; \
    return 0; \
    }
#define TestCompareTwoSimilarItems(x, y, str) \
  if ( !CompareTwoSimilarItems((x), (y), XdmfMAX(XdmfABS(x),XdmfABS(y))) ) \
    { \
    cerr << __LINE__ << ": " << str << " " << (x) << " <> " << (y) << endl; \
    return 0; \
    }

template<class InputDataType, class OutputDataType>
OutputDataType* TestWriter(InputDataType* input, const char* fname, 
  OutputDataType*, int alllight, const char* gridname,
  int strides[3])
{
  vtkXdmfWriter *writer = vtkXdmfWriter::New();
  writer->SetFileName(fname);
  writer->SetInput(input);
  writer->SetAllLight(alllight);
  writer->SetGridName(gridname);
  writer->SetDomainName("TestDomain");
  writer->Write();
  writer->Delete();

  vtkXdmfReader *reader = vtkXdmfReader::New();
  reader->SetFileName(fname);
  reader->UpdateInformation();
  reader->EnableAllGrids();
  reader->SetStride(strides);
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
  
  TestCompareTwoItems(daArray->GetDataType(),
    dbArray->GetDataType(),
    "Arrays "  << daArray->GetName() << ", " << dbArray->GetName() << " are not the same type");
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

template<class DArray>
void AddDataArray(vtkDataSetAttributes *pd, vtkIdType size,
  DArray *)
{
  DArray* b = DArray::New();
  const int cnt = 2;
  b->SetNumberOfComponents(cnt);
  b->SetNumberOfTuples(size);
  int cc;
  for ( cc = 0; cc < size * cnt; cc ++ )
    {
    b->SetValue(cc, cc);
    }
  pd->AddArray(b);
  b->Delete();
}

void AddDataArrays(vtkDataSet* ds)
{
  vtkIdType da;
  vtkPointData* dp = ds->GetPointData();
  vtkCellData* dc = ds->GetCellData();
  da = ds->GetNumberOfPoints();
  AddDataArray(dp, da, (vtkCharArray*)0);
  AddDataArray(dp, da, (vtkShortArray*)0);
  AddDataArray(dp, da, (vtkIntArray*)0);
  AddDataArray(dp, da, (vtkUnsignedCharArray*)0);
  AddDataArray(dp, da, (vtkUnsignedShortArray*)0);
  AddDataArray(dp, da, (vtkUnsignedIntArray*)0);
  AddDataArray(dp, da, (vtkFloatArray*)0);
  AddDataArray(dp, da, (vtkDoubleArray*)0);
  da = ds->GetNumberOfCells();
  AddDataArray(dc, da, (vtkCharArray*)0);
  AddDataArray(dc, da, (vtkShortArray*)0);
  AddDataArray(dc, da, (vtkIntArray*)0);
  AddDataArray(dc, da, (vtkUnsignedCharArray*)0);
  AddDataArray(dc, da, (vtkUnsignedShortArray*)0);
  AddDataArray(dc, da, (vtkUnsignedIntArray*)0);
  AddDataArray(dc, da, (vtkFloatArray*)0);
  AddDataArray(dc, da, (vtkDoubleArray*)0);
}

int TestPolyData(vtkPolyData* pd, const char* filename, int alllight, 
  const char* gridname, int strides[3])
{
  AddDataArrays(pd);
  cout << "** Preform PolyData test" << endl;
  vtkUnstructuredGrid* ug 
    = ::TestWriter(pd, filename, (vtkUnstructuredGrid*)0,
      alllight, gridname, strides);
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

vtkImageData* ApplyStrides(vtkImageData* input, int strides[3])
{
  vtkExtractVOI* ext = vtkExtractVOI::New();
  ext->SetInput(input);
  ext->SetSampleRate(strides);
  ext->Update();
  vtkImageData* sg = ext->GetOutput();
  sg->Register(0);
  ext->Delete();
  return sg;
  return 0;
}

vtkStructuredGrid* ApplyStrides(vtkStructuredGrid* input, int strides[3])
{
  vtkExtractGrid* ext = vtkExtractGrid::New();
  ext->SetInput(input);
  ext->SetSampleRate(strides);
  ext->Update();
  vtkStructuredGrid* sg = ext->GetOutput();
  sg->Register(0);
  ext->Delete();
  return sg;
}

vtkRectilinearGrid* ApplyStrides(vtkRectilinearGrid* input, int strides[3])
{
  vtkExtractRectilinearGrid* ext = vtkExtractRectilinearGrid::New();
  ext->SetInput(input);
  ext->SetSampleRate(strides);
  ext->Update();
  vtkRectilinearGrid* sg = ext->GetOutput();
  sg->Register(0);
  ext->Delete();
  return sg;
}

template<class DataType>
int TestSimpleData(DataType* orData, const char* filename, int alllight, 
  const char* gridname, int strides[3])
{
  cout << "** Preform simple data test" << endl;
  AddDataArrays(orData);
  DataType* rdData = ::TestWriter(orData, filename, (DataType*)0,
      alllight, gridname, strides);
  if ( !rdData )
    {
    return 1;
    }

  int dummystrides[3] = { 1, 1, 1 };
  DataType* dt1 =ApplyStrides(orData, strides);
  DataType* dt2 =ApplyStrides(rdData, dummystrides);
  rdData->Delete();

  int res = CompareDataSets(dt1, dt2);
  if ( !res )
    {
    cerr << "Difference:" << endl;
    dt1->Print(cout);
    cerr << "------------------" << endl;
    dt2->Print(cout);
    }

  dt1->UnRegister(0);
  dt2->UnRegister(0);
  if ( !res )
    {
    cerr << "Errors while running simple data test" << endl;
    return 1;
    }
  return 0;
}

int main(int argc, char* argv[])
{
  if ( argc < 2 )
    {
    cout << "Usage: " << argv[0] << " <xdmf file>" << endl;
    return 1;
    }
  int res = 0;

  char gridname[1024];
  int strides[3] = { 1, 1, 1 };

  vtkImageNoiseSource* noise;
  vtkXMLRectilinearGridReader* rect;
  vtkXMLStructuredGridReader* sgrid;

  sprintf(gridname, "TestUnstructuredGrid");
  vtkSphereSource *sphere = vtkSphereSource::New();
  sphere->Update();
  res += ::TestPolyData(sphere->GetOutput(), argv[1], 0, gridname, strides);
  res += ::TestPolyData(sphere->GetOutput(), argv[1], 1, gridname, strides);
  sphere->Delete();

  sprintf(gridname, "TestImageData");
  noise= vtkImageNoiseSource::New();
  noise->SetWholeExtent(0, 5, 0, 12, 0, 17);
  noise->Update();
  res += ::TestSimpleData(noise->GetOutput(), argv[1], 1, gridname, strides);
  res += ::TestSimpleData(noise->GetOutput(), argv[1], 0, gridname, strides);
  noise->Delete();

  sprintf(gridname, "TestRectilinearGrid");
  rect = vtkXMLRectilinearGridReader::New();
  rect->SetFileName(XDMF_DATA_DIR "/Data/Testing/rectilinear_grid.vtr");
  rect->Update();
  res += ::TestSimpleData(rect->GetOutput(), argv[1], 1, gridname, strides);
  res += ::TestSimpleData(rect->GetOutput(), argv[1], 0, gridname, strides);
  rect->Delete();

  sprintf(gridname, "TestStructuredGrid");
  sgrid = vtkXMLStructuredGridReader::New();
  sgrid->SetFileName(XDMF_DATA_DIR "/Data/Testing/structured_grid.vts");
  sgrid->Update();
  res += ::TestSimpleData(sgrid->GetOutput(), argv[1], 1, gridname, strides);
  res += ::TestSimpleData(sgrid->GetOutput(), argv[1], 0, gridname, strides);
  sgrid->Delete();

  /*
  strides[0] = 1;
  strides[0] = 2;
  strides[0] = 3;

  sprintf(gridname, "TestImageData");
  noise= vtkImageNoiseSource::New();
  noise->SetWholeExtent(0, 5, 0, 12, 0, 17);
  noise->Update();
  res += ::TestSimpleData(noise->GetOutput(), argv[1], 1, gridname, strides);
  res += ::TestSimpleData(noise->GetOutput(), argv[1], 0, gridname, strides);
  noise->Delete();
  
  sprintf(gridname, "TestRectilinearGrid");
  rect = vtkXMLRectilinearGridReader::New();
  rect->SetFileName(XDMF_DATA_DIR "/Data/Testing/rectilinear_grid.vtr");
  rect->Update();
  res += ::TestSimpleData(rect->GetOutput(), argv[1], 1, gridname, strides);
  res += ::TestSimpleData(rect->GetOutput(), argv[1], 0, gridname, strides);
  rect->Delete();

  sprintf(gridname, "TestStructuredGrid");
  sgrid = vtkXMLStructuredGridReader::New();
  sgrid->SetFileName(XDMF_DATA_DIR "/Data/Testing/structured_grid.vts");
  sgrid->Update();
  res += ::TestSimpleData(sgrid->GetOutput(), argv[1], 1, gridname, strides);
  res += ::TestSimpleData(sgrid->GetOutput(), argv[1], 0, gridname, strides);
  sgrid->Delete();
  */

  if ( res )
    {
    cerr << "Errors while running test" << endl;
    return 1;
    }
  return 0;
}
