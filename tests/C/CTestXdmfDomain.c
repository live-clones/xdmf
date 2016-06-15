#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfAttribute.hpp"
#include "XdmfAttributeType.hpp"
#include "XdmfAttributeCenter.hpp"
#include "XdmfCurvilinearGrid.hpp"
#include "XdmfDomain.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfGeometryType.hpp"
#include "XdmfGraph.hpp"
#include "XdmfGridCollection.hpp"
#include "XdmfGridCollectionType.hpp"
#include "XdmfHDF5Writer.hpp"
#include "XdmfReader.hpp"
#include "XdmfRectilinearGrid.hpp"
#include "XdmfRegularGrid.hpp"
#include "XdmfSparseMatrix.hpp"
#include "XdmfTime.hpp"
#include "XdmfTopology.hpp"
#include "XdmfUnstructuredGrid.hpp"
#include "XdmfWriter.hpp"


#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "assert.h"

int main()
{
  int i = 0;

  int status = 0;

  void * domain = XdmfDomainNew();

  printf("Generating interal objects\n");

// Grid Collection

  void * collection = XdmfGridCollectionNew();

  XdmfGridCollectionSetType(collection, XDMF_GRID_COLLECTION_TYPE_SPATIAL, &status);

  char * testName = XdmfGridCollectionGetName(collection);

  printf("%s ?= %s\n", testName, "Collection");

  assert(strcmp(testName, "Collection") == 0);

  XdmfGridCollectionSetName(collection, "Test Grid Collection", &status);

  void * collection1Time = XdmfTimeNew(21.0);

  XdmfGridCollectionSetTime(collection, collection1Time, 1);

  printf("checking time\n");

  void * checkTime = XdmfGridCollectionGetTime(collection);

  double checkTimeVal = XdmfTimeGetValue(checkTime);

  printf("%lf ?= %lf\n", checkTimeVal, 21.0);

  assert(checkTimeVal == 21.0);

  XdmfDomainInsertGridCollection(domain, collection, 1);

  void * collection2 = XdmfGridCollectionNew();

  XdmfGridCollectionSetType(collection2, XDMF_GRID_COLLECTION_TYPE_TEMPORAL, &status);

  XdmfGridCollectionSetName(collection2, "Second Test Grid Collection", &status);

  void * collection2Time = XdmfTimeNew(12.0);

  XdmfGridCollectionSetTime(collection2, collection2Time, 1);

  XdmfDomainInsertGridCollection(domain, collection2, 1);

  printf("Generated Grid Collections\n");

// Curvilinear Grid

  XDMFGEOMETRY * geometry = XdmfGeometryNew();

  for (i = 0; i < 10; i++) {
    XdmfGeometryPushBack(geometry, &i, XDMF_ARRAY_TYPE_INT32, &status);
  }

  XdmfGeometrySetType(geometry, XDMF_GEOMETRY_TYPE_XY, &status);


  void * curvGrid = XdmfCurvilinearGridNew2D(2, 5);

  XdmfCurvilinearGridSetGeometry(curvGrid, geometry, 1);

  XdmfCurvilinearGridSetName(curvGrid, "Test Curvilinear Grid", &status);

  XdmfDomainInsertCurvilinearGrid(domain, curvGrid, 1);

  void * curvGrid2 = XdmfCurvilinearGridNew2D(5, 2);

  XdmfCurvilinearGridSetGeometry(curvGrid2, geometry, 1);

  XdmfCurvilinearGridSetName(curvGrid2, "Second Test Curvilinear Grid", &status);

  XdmfDomainInsertCurvilinearGrid(domain, curvGrid2, 1);

  printf("Curvilinear Grids Generated\n");

// Rectilinear Grid

  void * newXDim = XdmfArrayNew();

  for (i = 5; i < 9; ++i) {
    XdmfArrayPushBack(newXDim, &i, XDMF_ARRAY_TYPE_INT32, &status);
  }

  void * newYDim = XdmfArrayNew();

  for (i = 6; i < 10; ++i) {
    XdmfArrayPushBack(newYDim, &i, XDMF_ARRAY_TYPE_INT32, &status);
  }

  void * rectGrid = XdmfRectilinearGridNew2D(newXDim, newYDim, 0);

  XdmfRectilinearGridSetName(rectGrid, "Test Rectilinear Grid", &status);

  XdmfDomainInsertRectilinearGrid(domain, rectGrid, 1);

  void * newXDim2 = XdmfArrayNew();

  for (i = 15; i < 19; ++i) {
    XdmfArrayPushBack(newXDim2, &i, XDMF_ARRAY_TYPE_INT32, &status);
  }

  void * newYDim2 = XdmfArrayNew();

  for (i = 16; i < 20; ++i) {
    XdmfArrayPushBack(newYDim2, &i, XDMF_ARRAY_TYPE_INT32, &status);
  }

  void * rectGrid2 = XdmfRectilinearGridNew2D(newXDim2, newYDim2, 0);

  XdmfRectilinearGridSetName(rectGrid2, "Second Test Rectilinear Grid", &status);

  XdmfDomainInsertRectilinearGrid(domain, rectGrid2, 1);

  printf("Rectilinear Grids Generated\n");

// Regular Grid

  void * regGrid = XdmfRegularGridNew2D(5, 5, 5, 5, 0, 0);

  XdmfRegularGridSetName(regGrid, "Test Regular Grid", &status);

  XdmfDomainInsertRegularGrid(domain, regGrid, 1);

  void * regGrid2 = XdmfRegularGridNew2D(3, 3, 3, 3, 1, 1);

  XdmfRegularGridSetName(regGrid2, "Second Test Regular Grid", &status);

  XdmfDomainInsertRegularGrid(domain, regGrid2, 1);

  printf("Regular Grids Generated\n");

// Unstuctured Grid

  void * unGrid = XdmfUnstructuredGridNewFromRegularGrid(regGrid, &status);

  XdmfUnstructuredGridSetName(unGrid, "Test Unstructred Grid", &status);

  XdmfDomainInsertUnstructuredGrid(domain, unGrid, 1);

  void * unGrid2 = XdmfUnstructuredGridNewFromRegularGrid(regGrid2, &status);

  XdmfUnstructuredGridSetName(unGrid2, "Second Test Unstructred Grid", &status);

  XdmfDomainInsertUnstructuredGrid(domain, unGrid2, 1);

  printf("Unstructured Grids Generated\n");

// Graph

  XDMFGRAPH * matrix = XdmfGraphNew(3);

  XdmfGraphSetName(matrix, "Test Matrix", &status);

  void * rowPointer = XdmfGraphGetRowPointer(matrix, &status);

  void * columnIndex = XdmfGraphGetColumnIndex(matrix, &status);

  void * values = XdmfGraphGetValues(matrix, &status);

  unsigned int insertedVal = 0;

  XdmfArrayInsertValue(rowPointer, 0, &insertedVal, XDMF_ARRAY_TYPE_UINT32, &status);

  insertedVal = 1;

  XdmfArrayInsertValue(rowPointer, 1, &insertedVal, XDMF_ARRAY_TYPE_UINT32, &status);

  insertedVal = 2;

  XdmfArrayInsertValue(rowPointer, 2, &insertedVal, XDMF_ARRAY_TYPE_UINT32, &status);

  insertedVal = 3;

  XdmfArrayInsertValue(rowPointer, 3, &insertedVal, XDMF_ARRAY_TYPE_UINT32, &status);

  insertedVal = 1;

  XdmfArrayInsertValue(columnIndex, 0, &insertedVal, XDMF_ARRAY_TYPE_UINT32, &status);

  insertedVal = 2;

  XdmfArrayInsertValue(columnIndex, 1, &insertedVal, XDMF_ARRAY_TYPE_UINT32, &status);

  insertedVal = 0;

  XdmfArrayInsertValue(columnIndex, 2, &insertedVal, XDMF_ARRAY_TYPE_UINT32, &status);

  double insertedDouble = 5.0;

  XdmfArrayInsertValue(values, 0, &insertedDouble, XDMF_ARRAY_TYPE_FLOAT64, &status);

  insertedDouble = 6.0;

  XdmfArrayInsertValue(values, 1, &insertedDouble, XDMF_ARRAY_TYPE_FLOAT64, &status);

  insertedDouble = -1.0;

  XdmfArrayInsertValue(values, 2, &insertedDouble, XDMF_ARRAY_TYPE_FLOAT64, &status);

  XdmfDomainInsertGraph(domain, matrix, 1);

  XDMFGRAPH * matrix2 = XdmfGraphNew(3);

  XdmfGraphSetName(matrix2, "Second Test Matrix", &status);

  void * rowPointer2 = XdmfGraphGetRowPointer(matrix2, &status);

  void * columnIndex2 = XdmfGraphGetColumnIndex(matrix2, &status);

  void * values2 = XdmfGraphGetValues(matrix2, &status);

  insertedVal = 0;

  XdmfArrayInsertValue(rowPointer2, 0, &insertedVal, XDMF_ARRAY_TYPE_UINT32, &status);

  insertedVal = 1;

  XdmfArrayInsertValue(rowPointer2, 1, &insertedVal, XDMF_ARRAY_TYPE_UINT32, &status);

  insertedVal = 2;

  XdmfArrayInsertValue(rowPointer2, 2, &insertedVal, XDMF_ARRAY_TYPE_UINT32, &status);

  insertedVal = 3;

  XdmfArrayInsertValue(rowPointer2, 3, &insertedVal, XDMF_ARRAY_TYPE_UINT32, &status);

  insertedVal = 1;

  XdmfArrayInsertValue(columnIndex2, 0, &insertedVal, XDMF_ARRAY_TYPE_UINT32, &status);

  insertedVal = 2;

  XdmfArrayInsertValue(columnIndex2, 1, &insertedVal, XDMF_ARRAY_TYPE_UINT32, &status);

  insertedVal = 0;

  XdmfArrayInsertValue(columnIndex2, 2, &insertedVal, XDMF_ARRAY_TYPE_UINT32, &status);

  insertedDouble = 5.0;

  XdmfArrayInsertValue(values2, 0, &insertedDouble, XDMF_ARRAY_TYPE_FLOAT64, &status);

  insertedDouble = 6.0;

  XdmfArrayInsertValue(values2, 1, &insertedDouble, XDMF_ARRAY_TYPE_FLOAT64, &status);

  insertedDouble = -2.20;

  XdmfArrayInsertValue(values2, 2, &insertedDouble, XDMF_ARRAY_TYPE_FLOAT64, &status);

  XdmfDomainInsertGraph(domain, matrix2, 1);

  printf("Graphs Generated\n");

// Write to file

  XDMFWRITER * writer = XdmfWriterNew("domainfile.xmf");

  XdmfDomainAccept(domain, (XDMFVISITOR *)writer, &status);

  printf("After Writing\n");

  void * reader = XdmfReaderNew();

  void * readDomain = XdmfReaderRead(reader, "domainfile.xmf", &status);

  printf("After Reading\n");

  char * valueString = XdmfDomainGetItemTag(readDomain);

  printf("%s ?= %s\n", valueString, "Domain");

  assert(strcmp(valueString, "Domain") == 0);

  free(valueString);

// Curvilinear

  unsigned int numContained = XdmfDomainGetNumberCurvilinearGrids(readDomain);

  printf("%d ?= %d\n", numContained, 2);

  assert(numContained == 2);

  void * readCurvGrid = XdmfDomainGetCurvilinearGrid(readDomain, 0);

  XDMFGEOMETRY * childGeometry = XdmfCurvilinearGridGetGeometry(readCurvGrid);

  valueString = XdmfGeometryGetValuesString(childGeometry);

  printf("%s ?= %s\n", valueString, "0 1 2 3 4 5 6 7 8 9");

  assert(strcmp(valueString, "0 1 2 3 4 5 6 7 8 9") == 0);

  free(valueString);

  void * childDimensions = XdmfCurvilinearGridGetDimensions(readCurvGrid, &status);

  valueString = XdmfArrayGetValuesString(childDimensions);

  printf("grid 1's dimension = %s\n", valueString);

  assert(strcmp(valueString, "2 5") == 0);

  free(valueString);

  void * readCurvGrid2 = XdmfDomainGetCurvilinearGridByName(readDomain, "Second Test Curvilinear Grid");

  XDMFGEOMETRY * childGeometry2 = XdmfCurvilinearGridGetGeometry(readCurvGrid2);

  valueString = XdmfGeometryGetValuesString(childGeometry2);

  printf("%s ?= %s\n", valueString, "0 1 2 3 4 5 6 7 8 9");

  assert(strcmp(valueString, "0 1 2 3 4 5 6 7 8 9") == 0);

  free(valueString);

  void * childDimensions2 = XdmfCurvilinearGridGetDimensions(readCurvGrid2, &status);

  valueString = XdmfArrayGetValuesString(childDimensions2);

  printf("grid 2's dimension = %s\n", valueString);

  assert(strcmp(valueString, "5 2") == 0);

  free(valueString);

  XdmfDomainRemoveCurvilinearGridByName(readDomain, "Test Curvilinear Grid");

  numContained = XdmfDomainGetNumberCurvilinearGrids(readDomain);

  printf("%d ?= %d\n", numContained, 1);

  assert(numContained == 1);

  void * readCurvGrid3 = XdmfDomainGetCurvilinearGrid(readDomain, 0);

  valueString = XdmfCurvilinearGridGetName(readCurvGrid3);

  printf("%s ?= %s\n", valueString, "Second Test Curvilinear Grid");

  assert(strcmp(valueString, "Second Test Curvilinear Grid") == 0);

  free(valueString);

  XdmfDomainRemoveCurvilinearGrid(readDomain, 0);

  numContained = XdmfDomainGetNumberCurvilinearGrids(readDomain);

  printf("%d ?= %d\n", numContained, 0);

  assert(numContained == 0);

  printf("done with curvilinear grids\n");

// Rectilinear

  numContained = XdmfDomainGetNumberRectilinearGrids(readDomain);

  printf("%d ?= %d\n", numContained, 2);

  assert(numContained == 2);

  void * readRectGrid = XdmfDomainGetRectilinearGrid(readDomain, 0);

  int numCoordinates = XdmfRectilinearGridGetNumberCoordinates(readRectGrid, &status);

  printf("%d ?= %d\n", numCoordinates, 2);

  assert(numCoordinates == 2);

  XDMFARRAY ** readCoordinateArray = XdmfRectilinearGridGetCoordinates(readRectGrid, &status);

  valueString = XdmfArrayGetValuesString(readCoordinateArray[0]);

  printf("first dimension contains %s\n", valueString);

  assert(strcmp(valueString, "5 6 7 8") == 0);

  free(valueString);

  valueString = XdmfArrayGetValuesString(readCoordinateArray[1]);

  printf("second dimension contains %s\n", valueString);

  assert(strcmp(valueString, "6 7 8 9") == 0);

  free(valueString);

  void * readDimensions = XdmfRectilinearGridGetDimensions(readRectGrid, &status);

  valueString = XdmfArrayGetValuesString(readDimensions);

  printf("dimensions contain %s\n", valueString);

  assert(strcmp(valueString, "4 4") == 0);

  free(valueString);

  XdmfItemFree(readDimensions);

  void * readRectGrid2 = XdmfDomainGetRectilinearGridByName(readDomain, "Second Test Rectilinear Grid");

  numCoordinates = XdmfRectilinearGridGetNumberCoordinates(readRectGrid2, &status);

  printf("%d ?= %d\n", numCoordinates, 2);

  assert(numCoordinates == 2);

  XDMFARRAY ** readCoordinateArray2 = XdmfRectilinearGridGetCoordinates(readRectGrid2, &status);

  valueString = XdmfArrayGetValuesString(readCoordinateArray2[0]);

  printf("first dimension contains %s\n", valueString);

  assert(strcmp(valueString, "15 16 17 18") == 0);

  free(valueString);

  valueString = XdmfArrayGetValuesString(readCoordinateArray2[1]);

  printf("second dimension contains %s\n", valueString);

  assert(strcmp(valueString, "16 17 18 19") == 0);

  free(valueString);

  readDimensions = XdmfRectilinearGridGetDimensions(readRectGrid2, &status);

  valueString = XdmfArrayGetValuesString(readDimensions);

  printf("dimensions contain %s\n", valueString);

  assert(strcmp(valueString, "4 4") == 0);

  free(valueString);

  XdmfItemFree(readDimensions);

  XdmfDomainRemoveRectilinearGridByName(readDomain, "Test Rectilinear Grid");

  numContained = XdmfDomainGetNumberRectilinearGrids(readDomain);

  printf("%d ?= %d\n", numContained, 1);

  assert(numContained == 1);

  void * readRectGrid3 = XdmfDomainGetRectilinearGrid(readDomain, 0);

  valueString = XdmfRectilinearGridGetName(readRectGrid3);

  printf("%s ?= %s\n", valueString, "Second Test Rectilinear Grid");

  assert(strcmp(valueString, "Second Test Rectilinear Grid") == 0);

  free(valueString);

  XdmfDomainRemoveRectilinearGrid(readDomain, 0);

  numContained = XdmfDomainGetNumberRectilinearGrids(readDomain);

  printf("%d ?= %d\n", numContained, 0);

  assert(numContained == 0);

// Regular

  numContained = XdmfDomainGetNumberRegularGrids(readDomain);

  printf("%d ?= %d\n", numContained, 2);

  assert(numContained == 2);

  void * readRegGrid = XdmfDomainGetRegularGrid(readDomain, 0);

  void * readBrick = XdmfRegularGridGetBrickSize(readRegGrid, &status);

  valueString = XdmfArrayGetValuesString(readBrick);

  printf("brick contains %s\n", valueString);

  assert(strcmp(valueString, "5 5") == 0);

  free(valueString);

  free(readBrick);

  readDimensions = XdmfRegularGridGetDimensions(readRegGrid, &status);

  valueString = XdmfArrayGetValuesString(readDimensions);

  printf("dimensions contains %s\n", valueString);

  assert(strcmp(valueString, "5 5") == 0);

  free(valueString);

  free(readDimensions);

  void * readOrigin = XdmfRegularGridGetOrigin(readRegGrid, &status);

  valueString = XdmfArrayGetValuesString(readOrigin);

  printf("origin contains %s\n", valueString);

  assert(strcmp(valueString, "0 0") == 0);

  free(valueString);

  free(readOrigin);

  void * readRegGrid2 = XdmfDomainGetRegularGridByName(readDomain, "Second Test Regular Grid");

  void * readBrick2 = XdmfRegularGridGetBrickSize(readRegGrid2, &status);

  valueString = XdmfArrayGetValuesString(readBrick2);

  printf("brick contains %s\n", valueString);

  assert(strcmp(valueString, "3 3") == 0);

  free(valueString);

  free(readBrick2);

  readDimensions = XdmfRegularGridGetDimensions(readRegGrid2, &status);

  valueString = XdmfArrayGetValuesString(readDimensions);

  printf("dimensions contains %s\n", valueString);

  assert(strcmp(valueString, "3 3") == 0);

  free(valueString);

  free(readDimensions);

  void * readOrigin2 = XdmfRegularGridGetOrigin(readRegGrid2, &status);

  valueString = XdmfArrayGetValuesString(readOrigin2);

  printf("origin contains %s\n", valueString);

  assert(strcmp(valueString, "1 1") == 0);

  free(valueString);

  free(readOrigin2);

  XdmfDomainRemoveRegularGridByName(readDomain, "Test Regular Grid");

  numContained = XdmfDomainGetNumberRegularGrids(readDomain);

  printf("%d ?= %d\n", numContained, 1);

  assert(numContained == 1);

  void * readRegGrid3 = XdmfDomainGetRegularGrid(readDomain, 0);

  valueString = XdmfRegularGridGetName(readRegGrid3);

  printf("%s ?= %s\n", valueString, "Second Test Regular Grid");

  assert(strcmp(valueString, "Second Test Regular Grid") == 0);

  free(valueString);

  XdmfDomainRemoveRegularGrid(readDomain, 0);

  numContained = XdmfDomainGetNumberRegularGrids(readDomain);

  printf("%d ?= %d\n", numContained, 0);

  assert(numContained == 0);

// Unstructured

  numContained = XdmfDomainGetNumberUnstructuredGrids(readDomain);

  printf("%d ?= %d\n", numContained, 2);

  assert(numContained == 2);

  void * readUnGrid = XdmfDomainGetUnstructuredGrid(readDomain, 0);

  XDMFTOPOLOGY * readTopology = XdmfUnstructuredGridGetTopology(readUnGrid);

  valueString = XdmfTopologyGetValuesString(readTopology);

  printf("unstructured topology contains: %s\n", valueString);

  assert(strcmp(valueString, "0 1 6 5 1 2 7 6 2 3 8 7 3 4 9 8 5 6 11 10 6 7 12 11 7 8 13 12 8 9 14 13 10 11 16 15 11 12 17 16 12 13 18 17 13 14 19 18 15 16 21 20 16 17 22 21 17 18 23 22 18 19 24 23") == 0);

  free(valueString);

  XDMFGEOMETRY * readGeometry = XdmfUnstructuredGridGetGeometry(readUnGrid);

  valueString = XdmfGeometryGetValuesString(readGeometry);

  printf("unstructured geometry contains: %s\n", valueString);

  assert(strcmp(valueString, "0 0 5 0 10 0 15 0 20 0 0 5 5 5 10 5 15 5 20 5 0 10 5 10 10 10 15 10 20 10 0 15 5 15 10 15 15 15 20 15 0 20 5 20 10 20 15 20 20 20") == 0);

  free(valueString);

  void * readUnGrid2 = XdmfDomainGetUnstructuredGridByName(readDomain, "Second Test Unstructred Grid");

  XDMFTOPOLOGY * readTopology2 = XdmfUnstructuredGridGetTopology(readUnGrid2);

  valueString = XdmfTopologyGetValuesString(readTopology2);

  printf("unstructured topology contains: %s\n", valueString);

  assert(strcmp(valueString, "0 1 4 3 1 2 5 4 3 4 7 6 4 5 8 7") == 0);

  free(valueString);

  XDMFGEOMETRY * readGeometry2 = XdmfUnstructuredGridGetGeometry(readUnGrid2);

  valueString = XdmfGeometryGetValuesString(readGeometry2);

  printf("unstructured geometry contains: %s\n", valueString);

  assert(strcmp(valueString, "1 1 4 1 7 1 1 4 4 4 7 4 1 7 4 7 7 7") == 0);

  free(valueString);

  XdmfDomainRemoveUnstructuredGridByName(readDomain, "Test Unstructred Grid");

  numContained = XdmfDomainGetNumberUnstructuredGrids(readDomain);

  printf("%d ?= %d\n", numContained, 1);

  assert(numContained == 1);

  void * readUnGrid3 = XdmfDomainGetUnstructuredGrid(readDomain, 0);

  valueString = XdmfUnstructuredGridGetName(readUnGrid3);

  printf("%s ?= %s\n", valueString, "Second Test Unstructured Grid");

  assert(strcmp(valueString, "Second Test Unstructred Grid") == 0);

  free(valueString);

  XdmfDomainRemoveUnstructuredGrid(readDomain, 0);

  numContained = XdmfDomainGetNumberUnstructuredGrids(readDomain);

  printf("%d ?= %d\n", numContained, 1);

  assert(numContained == 0);

// Graph

  numContained = XdmfDomainGetNumberGraphs(readDomain);

  printf("%d ?= %d\n", numContained, 2);

  assert(numContained == 2);

  XDMFGRAPH * readGraph = XdmfDomainGetGraph(readDomain, 0);

  unsigned int readNumRows = XdmfGraphGetNumberRows(readGraph);

  unsigned int readNumCols = XdmfGraphGetNumberColumns(readGraph);

  printf("%d ?= %d\n%d ?= %d\n", readNumRows, 3, readNumCols, 3);

  assert(readNumRows == 3);

  assert(readNumCols == 3);

  unsigned int readNumNodes = XdmfGraphGetNumberNodes(readGraph);

  printf("%d ?= %d\n", readNumNodes, 3);

  assert(readNumNodes == 3);

  valueString = XdmfGraphGetValuesString(readGraph, &status);

  printf("%s\n?=\n%s\n", valueString, "0.0, 5, 0.0, \n0.0, 0.0, 6, \n-1, 0.0, 0.0, \n");

  assert(strcmp(valueString, "0.0, 5, 0.0, \n0.0, 0.0, 6, \n-1, 0.0, 0.0, \n") == 0);

  free(valueString);

  XDMFGRAPH * readGraph2 = XdmfDomainGetGraphByName(readDomain, "Test Matrix");

  readNumRows = XdmfGraphGetNumberRows(readGraph2);

  readNumCols = XdmfGraphGetNumberColumns(readGraph2);

  printf("%d ?= %d\n%d ?= %d\n", readNumRows, 3, readNumCols, 3);

  assert(readNumRows == 3);

  assert(readNumCols == 3);

  readNumNodes = XdmfGraphGetNumberNodes(readGraph2);

  printf("%d ?= %d\n", readNumNodes, 3);

  assert(readNumNodes == 3);

  valueString = XdmfGraphGetValuesString(readGraph2, &status);

  printf("%s\n?=\n%s\n", valueString, "0.0, 5, 0.0, \n0.0, 0.0, 6, \n-1, 0.0, 0.0, \n");

  assert(strcmp(valueString, "0.0, 5, 0.0, \n0.0, 0.0, 6, \n-1, 0.0, 0.0, \n") == 0);

  free(valueString);

  XdmfDomainRemoveGraphByName(readDomain, "Test Matrix");

  numContained = XdmfDomainGetNumberGraphs(readDomain);

  printf("%d ?= %d\n", numContained, 1);

  assert(numContained == 1);

  XDMFGRAPH * readGraph3 = XdmfDomainGetGraph(readDomain, 0);

  valueString = XdmfGraphGetName(readGraph3);

  assert(strcmp(valueString, "Second Test Matrix") == 0);

  XdmfDomainRemoveGraph(readDomain, 0);

  numContained = XdmfDomainGetNumberGraphs(readDomain);

  printf("%d ?= %d\n", numContained, 0);

  assert(numContained == 0);

// Grid Collection

  numContained = XdmfDomainGetNumberGridCollections(readDomain);

  printf("%d ?= %d\n", numContained, 2);

  assert(numContained == 2);

  void * readCollection = XdmfDomainGetGridCollection(readDomain, 0);

  valueString = XdmfGridCollectionGetName(readCollection);

  printf("First Collection name = %s\n", valueString);

  assert(strcmp(valueString, "Test Grid Collection") == 0);

  free(valueString);

  void * readTime = XdmfGridCollectionGetTime(readCollection);

  double timeVal = XdmfTimeGetValue(readTime);

  printf("%lf ?= %lf\n", timeVal, 21.0);

  assert(timeVal == 21.0);

  void * readCollection2 = XdmfDomainGetGridCollectionByName(readDomain, "Second Test Grid Collection");

  valueString = XdmfGridCollectionGetName(readCollection2);

  printf("%s ?= %s\n", valueString, "Second Test Grid Collection");

  assert(strcmp(valueString, "Second Test Grid Collection") == 0);

  void * readTime2 = XdmfGridCollectionGetTime(readCollection2);

  double timeVal2 = XdmfTimeGetValue(readTime2);

  printf("%lf ?= %lf\n", timeVal2, 12.0);

  assert(timeVal2 == 12.0);

  return 0;
}
