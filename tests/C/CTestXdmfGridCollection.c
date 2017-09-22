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
#include "XdmfMap.hpp"
#include "XdmfReader.hpp"
#include "XdmfRectilinearGrid.hpp"
#include "XdmfRegularGrid.hpp"
#include "XdmfSparseMatrix.hpp"
#include "XdmfSet.hpp"
#include "XdmfSetType.hpp"
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
  int j = 0;
  int k = 0;

  int status = 0;

  XDMFGRIDCOLLECTION * mainCollection = XdmfGridCollectionNew();
  printf("Generating interal objects\n");

  XDMFATTRIBUTE * attrib = XdmfAttributeNew();
  for (i = 0; i < 10; i++) {
    XdmfAttributePushBack(attrib, &i, XDMF_ARRAY_TYPE_INT32, &status);
  }
  XdmfAttributeSetType(attrib, XDMF_ATTRIBUTE_TYPE_SCALAR, &status);
  XdmfAttributeSetCenter(attrib, XDMF_ATTRIBUTE_CENTER_NODE, &status);
  XdmfAttributeSetName(attrib, "Test Attribute", &status);
  XdmfGridCollectionInsertAttribute(mainCollection, attrib, 1);
  XdmfAttributeFree(attrib);

  XDMFATTRIBUTE * attrib2 = XdmfAttributeNew();
  for (i = 10; i < 20; i++) {
    XdmfAttributePushBack(attrib2, &i, XDMF_ARRAY_TYPE_INT32, &status);
  }
  XdmfAttributeSetType(attrib2, XDMF_ATTRIBUTE_TYPE_SCALAR, &status);
  XdmfAttributeSetCenter(attrib2, XDMF_ATTRIBUTE_CENTER_NODE, &status);
  XdmfAttributeSetName(attrib2, "Second Test Attribute", &status);
  XdmfGridCollectionInsertAttribute(mainCollection, attrib2, 1);
  XdmfAttributeFree(attrib2);

  XDMFSET * set = XdmfSetNew();
  for (i = 0; i < 10; i++) {
    XdmfSetPushBack(set, &i, XDMF_ARRAY_TYPE_INT32, &status);
  }
  XdmfSetSetType(set, XDMF_SET_TYPE_NODE, &status);
  XdmfSetSetName(set, "Test Set", &status);
  XdmfGridCollectionInsertSet(mainCollection, set, 1);
  XdmfSetFree(set);

  XDMFSET * set2 = XdmfSetNew();
  for (i = 10; i < 20; i++) {
    XdmfSetPushBack(set2, &i, XDMF_ARRAY_TYPE_INT32, &status);
  }
  XdmfSetSetType(set2, XDMF_SET_TYPE_NODE, &status);
  XdmfSetSetName(set2, "Second Test Set", &status);
  XdmfGridCollectionInsertSet(mainCollection, set2, 1);
  XdmfSetFree(set2);

  XDMFMAP * map = XdmfMapNew();
  XdmfMapInsert(map, 1, 0, 8);
  XdmfMapInsert(map, 1, 3, 7);
  XdmfMapInsert(map, 1, 5, 9);
  XdmfMapInsert(map, 2, 4, 6);
  XdmfMapInsert(map, 2, 7, 5);
  XdmfMapInsert(map, 3, 8, 3);
  XdmfMapInsert(map, 3, 2, 4);
  XdmfMapInsert(map, 3, 6, 2);
  XdmfMapInsert(map, 3, 1, 1);
  XdmfMapSetName(map, "Test Map");
  XdmfGridCollectionInsertMap(mainCollection, map, 1);
  XdmfMapFree(map);

  XDMFMAP * map2 = XdmfMapNew();
  XdmfMapInsert(map2, 1, 0, 8);
  XdmfMapInsert(map2, 1, 3, 7);
  XdmfMapInsert(map2, 1, 5, 9);
  XdmfMapInsert(map2, 2, 4, 6);
  XdmfMapInsert(map2, 2, 7, 5);
  XdmfMapInsert(map2, 3, 8, 3);
  XdmfMapInsert(map2, 3, 2, 4);
  XdmfMapInsert(map2, 3, 6, 2);
  XdmfMapInsert(map2, 3, 1, 1);
  XdmfMapSetName(map2, "Second Test Map");
  XdmfGridCollectionInsertMap(mainCollection, map2, 1);
  XdmfMapFree(map2);
  printf("generated maps, sets, and attributes\n");

  // Grid Collection

  XDMFGRIDCOLLECTION * collection = XdmfGridCollectionNew();
  XdmfGridCollectionSetType(collection, 
			    XDMF_GRID_COLLECTION_TYPE_SPATIAL, 
			    &status);
  char * testName = XdmfGridCollectionGetName(collection);
  printf("%s ?= %s\n", testName, "Collection");
  assert(strcmp(testName, "Collection") == 0);
  free(testName);
  XdmfGridCollectionSetName(collection, "Test Grid Collection", &status);

  XDMFTIME * collection1Time = XdmfTimeNew(21.0);
  XdmfGridCollectionSetTime(collection, collection1Time, 1);
  XdmfTimeFree(collection1Time);

  printf("checking time\n");

  XDMFTIME * checkTime = XdmfGridCollectionGetTime(collection);
  double checkTimeVal = XdmfTimeGetValue(checkTime);
  printf("%lf ?= %lf\n", checkTimeVal, 21.0);
  assert(checkTimeVal == 21.0);
  XdmfTimeFree(checkTime);

  XdmfGridCollectionInsertGridCollection(mainCollection, collection, 1);
  XdmfGridCollectionFree(collection);

  XDMFGRIDCOLLECTION * collection2 = XdmfGridCollectionNew();
  XdmfGridCollectionSetType(collection2, 
			    XDMF_GRID_COLLECTION_TYPE_TEMPORAL, 
			    &status);
  XdmfGridCollectionSetName(collection2, 
			    "Second Test Grid Collection", 
			    &status);

  XDMFTIME * collection2Time = XdmfTimeNew(12.0);
  XdmfGridCollectionSetTime(collection2, collection2Time, 1);
  XdmfGridCollectionInsertGridCollection(mainCollection, collection2, 1);
  XdmfTimeFree(collection2Time);
  XdmfGridCollectionFree(collection2);
  printf("Generated Grid Collections\n");

  // Curvilinear Grid

  XDMFGEOMETRY * geometry = XdmfGeometryNew();
  for (i = 0; i < 10; i++) {
    XdmfGeometryPushBack(geometry, &i, XDMF_ARRAY_TYPE_INT32, &status);
  }
  XdmfGeometrySetType(geometry, XDMF_GEOMETRY_TYPE_XY, &status);

  XDMFCURVILINEARGRID * curvGrid = XdmfCurvilinearGridNew2D(2, 5);
  XdmfCurvilinearGridSetGeometry(curvGrid, geometry, 1);
  XdmfCurvilinearGridSetName(curvGrid, "Test Curvilinear Grid", &status);
  XdmfGridCollectionInsertCurvilinearGrid(mainCollection, curvGrid, 1);
  XdmfCurvilinearGridFree(curvGrid);

  XDMFCURVILINEARGRID * curvGrid2 = XdmfCurvilinearGridNew2D(5, 2);
  XdmfCurvilinearGridSetGeometry(curvGrid2, geometry, 1);
  XdmfCurvilinearGridSetName(curvGrid2, 
			     "Second Test Curvilinear Grid", 
			     &status);
  XdmfGridCollectionInsertCurvilinearGrid(mainCollection, curvGrid2, 1);
  XdmfGeometryFree(geometry);
  XdmfCurvilinearGridFree(curvGrid2);

  printf("Curvilinear Grids Generated\n");

  // Rectilinear Grid

  XDMFARRAY * newXDim = XdmfArrayNew();
   for (i = 5; i < 9; ++i) {
    XdmfArrayPushBack(newXDim, &i, XDMF_ARRAY_TYPE_INT32, &status);
  }
  XDMFARRAY * newYDim = XdmfArrayNew();
  for (i = 6; i < 10; ++i) {
    XdmfArrayPushBack(newYDim, &i, XDMF_ARRAY_TYPE_INT32, &status);
  }
  
  XDMFRECTILINEARGRID * rectGrid = 
    XdmfRectilinearGridNew2D(newXDim, newYDim, 0);
  XdmfArrayFree(newXDim);
  XdmfArrayFree(newYDim);

  XdmfRectilinearGridSetName(rectGrid, "Test Rectilinear Grid", &status);
  XdmfGridCollectionInsertRectilinearGrid(mainCollection, rectGrid, 1);
  XdmfRectilinearGridFree(rectGrid);

  XDMFARRAY * newXDim2 = XdmfArrayNew();
  for (i = 15; i < 19; ++i) {
    XdmfArrayPushBack(newXDim2, &i, XDMF_ARRAY_TYPE_INT32, &status);
  }
  XDMFARRAY * newYDim2 = XdmfArrayNew();
  for (i = 16; i < 20; ++i) {
    XdmfArrayPushBack(newYDim2, &i, XDMF_ARRAY_TYPE_INT32, &status);
  }

  XDMFRECTILINEARGRID * rectGrid2 = 
    XdmfRectilinearGridNew2D(newXDim2, newYDim2, 0);
  XdmfArrayFree(newXDim2);
  XdmfArrayFree(newYDim2);
  XdmfRectilinearGridSetName(rectGrid2, 
			     "Second Test Rectilinear Grid", 
			     &status);
  XdmfGridCollectionInsertRectilinearGrid(mainCollection, rectGrid2, 1);
  XdmfRectilinearGridFree(rectGrid2);
  
  printf("Rectilinear Grids Generated\n");

  // Regular Grid
  
  XDMFREGULARGRID * regGrid = XdmfRegularGridNew2D(5, 5, 5, 5, 0, 0);
  XdmfRegularGridSetName(regGrid, "Test Regular Grid", &status);
  XdmfGridCollectionInsertRegularGrid(mainCollection, regGrid, 1);

  XDMFREGULARGRID * regGrid2 = XdmfRegularGridNew2D(3, 3, 3, 3, 1, 1);
  XdmfRegularGridSetName(regGrid2, "Second Test Regular Grid", &status);
  XdmfGridCollectionInsertRegularGrid(mainCollection, regGrid2, 1);

  printf("Regular Grids Generated\n");

  // Unstructured Grid

  XDMFUNSTRUCTUREDGRID * unGrid = 
    XdmfUnstructuredGridNewFromRegularGrid(regGrid, &status);
  XdmfUnstructuredGridSetName(unGrid, "Test Unstructred Grid", &status);
  XdmfGridCollectionInsertUnstructuredGrid(mainCollection, unGrid, 1);
  XdmfUnstructuredGridFree(unGrid);

  XDMFUNSTRUCTUREDGRID * unGrid2 = 
    XdmfUnstructuredGridNewFromRegularGrid(regGrid2, &status);
  XdmfUnstructuredGridSetName(unGrid2, "Second Test Unstructred Grid", &status);
  XdmfGridCollectionInsertUnstructuredGrid(mainCollection, unGrid2, 1);
  XdmfUnstructuredGridFree(unGrid2);

  XdmfRegularGridFree(regGrid);
  XdmfRegularGridFree(regGrid2);

  printf("Unstructured Grids Generated\n");
  
  // Graph

  XDMFGRAPH * matrix = XdmfGraphNew(3);
  XdmfGraphSetName(matrix, "Test Matrix", &status);
  XDMFARRAY * rowPointer = XdmfGraphGetRowPointer(matrix, &status);
  XDMFARRAY * columnIndex = XdmfGraphGetColumnIndex(matrix, &status);
  XDMFARRAY * values = XdmfGraphGetValues(matrix, &status);

  unsigned int insertedVal = 0;
  XdmfArrayInsertValue(rowPointer, 
		       0, 
		       &insertedVal, 
		       XDMF_ARRAY_TYPE_UINT32, 
		       &status);

  insertedVal = 1;
  XdmfArrayInsertValue(rowPointer, 
		       1, 
		       &insertedVal, 
		       XDMF_ARRAY_TYPE_UINT32, 
		       &status);

  insertedVal = 2;
  XdmfArrayInsertValue(rowPointer, 
		       2, 
		       &insertedVal, 
		       XDMF_ARRAY_TYPE_UINT32, 
		       &status);

  insertedVal = 3;
  XdmfArrayInsertValue(rowPointer, 
		       3, 
		       &insertedVal, 
		       XDMF_ARRAY_TYPE_UINT32, 
		       &status);

  insertedVal = 1;
  XdmfArrayInsertValue(columnIndex, 
		       0, 
		       &insertedVal, 
		       XDMF_ARRAY_TYPE_UINT32, 
		       &status);

  insertedVal = 2;
  XdmfArrayInsertValue(columnIndex, 
		       1, 
		       &insertedVal, 
		       XDMF_ARRAY_TYPE_UINT32, 
		       &status);

  insertedVal = 0;
  XdmfArrayInsertValue(columnIndex, 
		       2, 
		       &insertedVal, 
		       XDMF_ARRAY_TYPE_UINT32, 
		       &status);

  double insertedDouble = 5.0;
  XdmfArrayInsertValue(values, 
		       0, 
		       &insertedDouble, 
		       XDMF_ARRAY_TYPE_FLOAT64, 
		       &status);

  insertedDouble = 6.0;
  XdmfArrayInsertValue(values, 
		       1, 
		       &insertedDouble, 
		       XDMF_ARRAY_TYPE_FLOAT64, 
		       &status);

  insertedDouble = -1.0;
  XdmfArrayInsertValue(values, 
		       2, 
		       &insertedDouble, 
		       XDMF_ARRAY_TYPE_FLOAT64, 
		       &status);

  XdmfGridCollectionInsertGraph(mainCollection, matrix, 1);
  XdmfGraphFree(matrix);
  XdmfArrayFree(rowPointer);
  XdmfArrayFree(columnIndex);
  XdmfArrayFree(values);

  XDMFGRAPH * matrix2 = XdmfGraphNew(3);
  XdmfGraphSetName(matrix2, "Second Test Matrix", &status);
  XDMFARRAY * rowPointer2 = XdmfGraphGetRowPointer(matrix2, &status);
  XDMFARRAY * columnIndex2 = XdmfGraphGetColumnIndex(matrix2, &status);
  XDMFARRAY * values2 = XdmfGraphGetValues(matrix2, &status);

  insertedVal = 0;
  XdmfArrayInsertValue(rowPointer2, 
		       0, 
		       &insertedVal, 
		       XDMF_ARRAY_TYPE_UINT32, 
		       &status);

  insertedVal = 1;
  XdmfArrayInsertValue(rowPointer2, 
		       1, 
		       &insertedVal, 
		       XDMF_ARRAY_TYPE_UINT32, 
		       &status);

  insertedVal = 2;
  XdmfArrayInsertValue(rowPointer2, 
		       2, 
		       &insertedVal, 
		       XDMF_ARRAY_TYPE_UINT32, 
		       &status);

  insertedVal = 3;
  XdmfArrayInsertValue(rowPointer2, 
		       3, 
		       &insertedVal, 
		       XDMF_ARRAY_TYPE_UINT32, 
		       &status);

  insertedVal = 1;
  XdmfArrayInsertValue(columnIndex2, 
		       0, 
		       &insertedVal, 
		       XDMF_ARRAY_TYPE_UINT32, 
		       &status);

  insertedVal = 2;
  XdmfArrayInsertValue(columnIndex2, 
		       1, 
		       &insertedVal, 
		       XDMF_ARRAY_TYPE_UINT32, 
		       &status);

  insertedVal = 0;
  XdmfArrayInsertValue(columnIndex2, 
		       2, 
		       &insertedVal, 
		       XDMF_ARRAY_TYPE_UINT32, 
		       &status);

  insertedDouble = 5.0;
  XdmfArrayInsertValue(values2, 
		       0, 
		       &insertedDouble, 
		       XDMF_ARRAY_TYPE_FLOAT64, 
		       &status);

  insertedDouble = 6.0;
  XdmfArrayInsertValue(values2, 
		       1, 
		       &insertedDouble, 
		       XDMF_ARRAY_TYPE_FLOAT64, 
		       &status);

  insertedDouble = -2.20;
  XdmfArrayInsertValue(values2, 
		       2, 
		       &insertedDouble, 
		       XDMF_ARRAY_TYPE_FLOAT64, 
		       &status);

  XdmfGridCollectionInsertGraph(mainCollection, matrix2, 1);
  XdmfGraphFree(matrix2);
  XdmfArrayFree(rowPointer2);
  XdmfArrayFree(columnIndex2);
  XdmfArrayFree(values2);

  printf("Graphs Generated\n");

  // Write to file

  XDMFWRITER * writer = XdmfWriterNew("colfile.xmf");
  XdmfGridCollectionAccept(mainCollection, (XDMFVISITOR *)writer, &status);
  printf("After Writing\n");
  XdmfWriterFree(writer);
  XdmfGridCollectionFree(mainCollection);

  XDMFREADER * reader = XdmfReaderNew();

  XDMFITEM * readMainItem = XdmfReaderRead(reader, "colfile.xmf", &status);
  XDMFGRIDCOLLECTION * readMainCollection = 
    XdmfGridCollectionCast(readMainItem);
  XdmfItemFree(readMainItem);
  XdmfReaderFree(reader);

  printf("After Reading\n");

  char * valueString = XdmfGridCollectionGetItemTag(readMainCollection);
  printf("%s ?= %s\n", valueString, "Grid");
  assert(strcmp(valueString, "Grid") == 0);
  free(valueString);

  // Curvilinear

  unsigned int numContained = 
    XdmfGridCollectionGetNumberCurvilinearGrids(readMainCollection);
  printf("%d ?= %d\n", numContained, 2);
  assert(numContained == 2);

  XDMFCURVILINEARGRID * readCurvGrid = 
    XdmfGridCollectionGetCurvilinearGrid(readMainCollection, 0);
  XDMFGEOMETRY * childGeometry = XdmfCurvilinearGridGetGeometry(readCurvGrid);
  valueString = XdmfGeometryGetValuesString(childGeometry);
  printf("%s ?= %s\n", valueString, "0 1 2 3 4 5 6 7 8 9");
  assert(strcmp(valueString, "0 1 2 3 4 5 6 7 8 9") == 0);
  free(valueString);
  XdmfGeometryFree(childGeometry);

  XDMFARRAY * childDimensions = 
    XdmfCurvilinearGridGetDimensions(readCurvGrid, &status);
  valueString = XdmfArrayGetValuesString(childDimensions);
  printf("grid 1's dimension = %s\n", valueString);
  assert(strcmp(valueString, "2 5") == 0);
  free(valueString);
  XdmfArrayFree(childDimensions);
  XdmfCurvilinearGridFree(readCurvGrid);

  XDMFCURVILINEARGRID * readCurvGrid2 = 
    XdmfGridCollectionGetCurvilinearGridByName(readMainCollection, 
					       "Second Test Curvilinear Grid");
  XDMFGEOMETRY * childGeometry2 = XdmfCurvilinearGridGetGeometry(readCurvGrid2);
  valueString = XdmfGeometryGetValuesString(childGeometry2);
  printf("%s ?= %s\n", valueString, "0 1 2 3 4 5 6 7 8 9");
  assert(strcmp(valueString, "0 1 2 3 4 5 6 7 8 9") == 0);
  free(valueString);
  XdmfGeometryFree(childGeometry2);

  XDMFARRAY * childDimensions2 = 
    XdmfCurvilinearGridGetDimensions(readCurvGrid2, &status);
  valueString = XdmfArrayGetValuesString(childDimensions2);
  printf("grid 2's dimension = %s\n", valueString);
  assert(strcmp(valueString, "5 2") == 0);
  free(valueString);
  XdmfCurvilinearGridFree(readCurvGrid2);
  XdmfArrayFree(childDimensions2);

  XdmfGridCollectionRemoveCurvilinearGridByName(readMainCollection, 
						"Test Curvilinear Grid");
  numContained = 
    XdmfGridCollectionGetNumberCurvilinearGrids(readMainCollection);
  printf("%d ?= %d\n", numContained, 1);
  assert(numContained == 1);

  XDMFCURVILINEARGRID * readCurvGrid3 = 
    XdmfGridCollectionGetCurvilinearGrid(readMainCollection, 0);
  valueString = XdmfCurvilinearGridGetName(readCurvGrid3);
  printf("%s ?= %s\n", valueString, "Second Test Curvilinear Grid");
  assert(strcmp(valueString, "Second Test Curvilinear Grid") == 0);
  free(valueString);
  XdmfCurvilinearGridFree(readCurvGrid3);

  XdmfGridCollectionRemoveCurvilinearGrid(readMainCollection, 0);
  numContained = 
    XdmfGridCollectionGetNumberCurvilinearGrids(readMainCollection);
  printf("%d ?= %d\n", numContained, 0);
  assert(numContained == 0);
  printf("done with curvilinear grids\n");

  // Rectilinear

  numContained = 
    XdmfGridCollectionGetNumberRectilinearGrids(readMainCollection);
  printf("%d ?= %d\n", numContained, 2);
  assert(numContained == 2);

  XDMFRECTILINEARGRID * readRectGrid = 
    XdmfGridCollectionGetRectilinearGrid(readMainCollection, 0);
  int numCoordinates = 
    XdmfRectilinearGridGetNumberCoordinates(readRectGrid, &status);
  printf("%d ?= %d\n", numCoordinates, 2);
  assert(numCoordinates == 2);

  XDMFARRAY ** readCoordinateArray = 
    XdmfRectilinearGridGetCoordinates(readRectGrid, &status);

  valueString = XdmfArrayGetValuesString(readCoordinateArray[0]);
  printf("first dimension contains %s\n", valueString);
  assert(strcmp(valueString, "5 6 7 8") == 0);
  free(valueString);

  valueString = XdmfArrayGetValuesString(readCoordinateArray[1]);
  printf("second dimension contains %s\n", valueString);
  assert(strcmp(valueString, "6 7 8 9") == 0);
  free(valueString);

  XdmfArrayFree(readCoordinateArray[0]);
  XdmfArrayFree(readCoordinateArray[1]);
  free(readCoordinateArray);

  XDMFARRAY * readDimensions = 
    XdmfRectilinearGridGetDimensions(readRectGrid, &status);
  valueString = XdmfArrayGetValuesString(readDimensions);
  printf("dimensions contain %s\n", valueString);
  assert(strcmp(valueString, "4 4") == 0);
  free(valueString);
  XdmfArrayFree(readDimensions);
  XdmfRectilinearGridFree(readRectGrid);
  
  XDMFRECTILINEARGRID * readRectGrid2 = 
    XdmfGridCollectionGetRectilinearGridByName(readMainCollection, 
					       "Second Test Rectilinear Grid");
  numCoordinates = 
    XdmfRectilinearGridGetNumberCoordinates(readRectGrid2, &status);
  printf("%d ?= %d\n", numCoordinates, 2);
  assert(numCoordinates == 2);

  XDMFARRAY ** readCoordinateArray2 = 
    XdmfRectilinearGridGetCoordinates(readRectGrid2, &status);

  valueString = XdmfArrayGetValuesString(readCoordinateArray2[0]);
  printf("first dimension contains %s\n", valueString);
  assert(strcmp(valueString, "15 16 17 18") == 0);
  free(valueString);

  valueString = XdmfArrayGetValuesString(readCoordinateArray2[1]);
  printf("second dimension contains %s\n", valueString);
  assert(strcmp(valueString, "16 17 18 19") == 0);
  free(valueString);

  XdmfArrayFree(readCoordinateArray2[0]);
  XdmfArrayFree(readCoordinateArray2[1]);
  free(readCoordinateArray2);
  
  readDimensions = XdmfRectilinearGridGetDimensions(readRectGrid2, &status);
  valueString = XdmfArrayGetValuesString(readDimensions);
  printf("dimensions contain %s\n", valueString);
  assert(strcmp(valueString, "4 4") == 0);
  free(valueString);
  XdmfArrayFree(readDimensions);
  XdmfRectilinearGridFree(readRectGrid2);

  XdmfGridCollectionRemoveRectilinearGridByName(readMainCollection, 
						"Test Rectilinear Grid");
  numContained = 
    XdmfGridCollectionGetNumberRectilinearGrids(readMainCollection);
  printf("%d ?= %d\n", numContained, 1);
  assert(numContained == 1);

  XDMFRECTILINEARGRID * readRectGrid3 = 
    XdmfGridCollectionGetRectilinearGrid(readMainCollection, 0);
  valueString = XdmfRectilinearGridGetName(readRectGrid3);
  printf("%s ?= %s\n", valueString, "Second Test Rectilinear Grid");
  assert(strcmp(valueString, "Second Test Rectilinear Grid") == 0);
  free(valueString);
  XdmfRectilinearGridFree(readRectGrid3);

  XdmfGridCollectionRemoveRectilinearGrid(readMainCollection, 0);
  numContained = 
    XdmfGridCollectionGetNumberRectilinearGrids(readMainCollection);
  printf("%d ?= %d\n", numContained, 0);
  assert(numContained == 0);

  // Regular

  numContained = XdmfGridCollectionGetNumberRegularGrids(readMainCollection);
  printf("%d ?= %d\n", numContained, 2);
  assert(numContained == 2);

  XDMFREGULARGRID * readRegGrid = 
    XdmfGridCollectionGetRegularGrid(readMainCollection, 0);
  
  XDMFARRAY * readBrick = XdmfRegularGridGetBrickSize(readRegGrid, &status);
  valueString = XdmfArrayGetValuesString(readBrick);
  printf("brick contains %s\n", valueString);
  assert(strcmp(valueString, "5 5") == 0);
  free(valueString);
  XdmfArrayFree(readBrick);

  readDimensions = XdmfRegularGridGetDimensions(readRegGrid, &status);
  valueString = XdmfArrayGetValuesString(readDimensions);
  printf("dimensions contains %s\n", valueString);
  assert(strcmp(valueString, "5 5") == 0);
  free(valueString);
  XdmfArrayFree(readDimensions);

  XDMFARRAY * readOrigin = XdmfRegularGridGetOrigin(readRegGrid, &status);
  valueString = XdmfArrayGetValuesString(readOrigin);
  printf("origin contains %s\n", valueString);
  assert(strcmp(valueString, "0 0") == 0);
  free(valueString);
  XdmfArrayFree(readOrigin);
  XdmfRegularGridFree(readRegGrid);

  XDMFREGULARGRID * readRegGrid2 = 
    XdmfGridCollectionGetRegularGridByName(readMainCollection, 
					   "Second Test Regular Grid");
  
  XDMFARRAY * readBrick2 = XdmfRegularGridGetBrickSize(readRegGrid2, &status);
  valueString = XdmfArrayGetValuesString(readBrick2);
  printf("brick contains %s\n", valueString);
  assert(strcmp(valueString, "3 3") == 0);
  free(valueString);
  XdmfArrayFree(readBrick2);

  readDimensions = XdmfRegularGridGetDimensions(readRegGrid2, &status);
  valueString = XdmfArrayGetValuesString(readDimensions);
  printf("dimensions contains %s\n", valueString);
  assert(strcmp(valueString, "3 3") == 0);
  free(valueString);
  XdmfArrayFree(readDimensions);

  XDMFARRAY * readOrigin2 = XdmfRegularGridGetOrigin(readRegGrid2, &status);
  valueString = XdmfArrayGetValuesString(readOrigin2);
  printf("origin contains %s\n", valueString);
  assert(strcmp(valueString, "1 1") == 0);
  free(valueString);
  XdmfArrayFree(readOrigin2);
  
  XdmfRegularGridFree(readRegGrid2);

  XdmfGridCollectionRemoveRegularGridByName(readMainCollection, 
					    "Test Regular Grid");
  numContained = XdmfGridCollectionGetNumberRegularGrids(readMainCollection);
  printf("%d ?= %d\n", numContained, 1);
  assert(numContained == 1);

  XDMFREGULARGRID * readRegGrid3 = 
    XdmfGridCollectionGetRegularGrid(readMainCollection, 0);
  valueString = XdmfRegularGridGetName(readRegGrid3);
  printf("%s ?= %s\n", valueString, "Second Test Regular Grid");
  assert(strcmp(valueString, "Second Test Regular Grid") == 0);
  free(valueString);
  XdmfRegularGridFree(readRegGrid3);

  XdmfGridCollectionRemoveRegularGrid(readMainCollection, 0);
  numContained = XdmfGridCollectionGetNumberRegularGrids(readMainCollection);
  printf("%d ?= %d\n", numContained, 0);
  assert(numContained == 0);

  // Unstructured

  numContained = 
    XdmfGridCollectionGetNumberUnstructuredGrids(readMainCollection);
  printf("%d ?= %d\n", numContained, 2);
  assert(numContained == 2);

  XDMFUNSTRUCTUREDGRID * readUnGrid = 
    XdmfGridCollectionGetUnstructuredGrid(readMainCollection, 0);

  XDMFTOPOLOGY * readTopology = XdmfUnstructuredGridGetTopology(readUnGrid);
  valueString = XdmfTopologyGetValuesString(readTopology);
  printf("unstructured topology contains: %s\n", valueString);
  assert(strcmp(valueString, "0 1 6 5 1 2 7 6 2 3 8 7 3 4 9 8 5 6 11 10 6 7 12 11 7 8 13 12 8 9 14 13 10 11 16 15 11 12 17 16 12 13 18 17 13 14 19 18 15 16 21 20 16 17 22 21 17 18 23 22 18 19 24 23") == 0);
  free(valueString);
  XdmfTopologyFree(readTopology);

  XDMFGEOMETRY * readGeometry = XdmfUnstructuredGridGetGeometry(readUnGrid);
  valueString = XdmfGeometryGetValuesString(readGeometry);
  printf("unstructured geometry contains: %s\n", valueString);
  assert(strcmp(valueString, "0 0 5 0 10 0 15 0 20 0 0 5 5 5 10 5 15 5 20 5 0 10 5 10 10 10 15 10 20 10 0 15 5 15 10 15 15 15 20 15 0 20 5 20 10 20 15 20 20 20") == 0);
  free(valueString);
  XdmfGeometryFree(readGeometry);
  XdmfUnstructuredGridFree(readUnGrid);

  XDMFUNSTRUCTUREDGRID * readUnGrid2 = 
    XdmfGridCollectionGetUnstructuredGridByName(readMainCollection, 
						"Second Test Unstructred Grid");

  XDMFTOPOLOGY * readTopology2 = XdmfUnstructuredGridGetTopology(readUnGrid2);
  valueString = XdmfTopologyGetValuesString(readTopology2);
  printf("unstructured topology contains: %s\n", valueString);
  assert(strcmp(valueString, "0 1 4 3 1 2 5 4 3 4 7 6 4 5 8 7") == 0);
  free(valueString);
  XdmfTopologyFree(readTopology2);

  XDMFGEOMETRY * readGeometry2 = XdmfUnstructuredGridGetGeometry(readUnGrid2);
  valueString = XdmfGeometryGetValuesString(readGeometry2);
  printf("unstructured geometry contains: %s\n", valueString);
  assert(strcmp(valueString, "1 1 4 1 7 1 1 4 4 4 7 4 1 7 4 7 7 7") == 0);
  free(valueString);
  XdmfGeometryFree(readGeometry2);
  XdmfUnstructuredGridFree(readUnGrid2);

  XdmfGridCollectionRemoveUnstructuredGridByName(readMainCollection, 
						 "Test Unstructred Grid");
  numContained = XdmfGridCollectionGetNumberUnstructuredGrids(readMainCollection);
  printf("%d ?= %d\n", numContained, 1);
  assert(numContained == 1);

  XDMFUNSTRUCTUREDGRID * readUnGrid3 = 
    XdmfGridCollectionGetUnstructuredGrid(readMainCollection, 0);
  valueString = XdmfUnstructuredGridGetName(readUnGrid3);
  printf("%s ?= %s\n", valueString, "Second Test Unstructred Grid");
  assert(strcmp(valueString, "Second Test Unstructred Grid") == 0);
  free(valueString);
  XdmfUnstructuredGridFree(readUnGrid3);

  XdmfGridCollectionRemoveUnstructuredGrid(readMainCollection, 0);
  numContained = 
    XdmfGridCollectionGetNumberUnstructuredGrids(readMainCollection);
  printf("%d ?= %d\n", numContained, 0);
  assert(numContained == 0);

  // Graph

  numContained = XdmfGridCollectionGetNumberGraphs(readMainCollection);
  printf("%d ?= %d\n", numContained, 2);
  assert(numContained == 2);

  XDMFGRAPH * readGraph = XdmfGridCollectionGetGraph(readMainCollection, 0);

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
  XdmfGraphFree(readGraph);

  XDMFGRAPH * readGraph2 = 
    XdmfGridCollectionGetGraphByName(readMainCollection, "Test Matrix");

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
  XdmfGraphFree(readGraph2);

  XdmfGridCollectionRemoveGraphByName(readMainCollection, "Test Matrix");
  numContained = XdmfGridCollectionGetNumberGraphs(readMainCollection);
  printf("%d ?= %d\n", numContained, 1);
  assert(numContained == 1);

  XDMFGRAPH * readGraph3 = XdmfGridCollectionGetGraph(readMainCollection, 0);

  valueString = XdmfGraphGetName(readGraph3);
  printf("%s\n?=\n%s\n", valueString, "Second Test Matrix");
  assert(strcmp(valueString, "Second Test Matrix") == 0);
  free(valueString);
  XdmfGraphFree(readGraph3);

  XdmfGridCollectionRemoveGraph(readMainCollection, 0);
  numContained = XdmfGridCollectionGetNumberGraphs(readMainCollection);
  printf("%d ?= %d\n", numContained, 0);
  assert(numContained == 0);

  // Grid Collection

  numContained = XdmfGridCollectionGetNumberGridCollections(readMainCollection);
  printf("%d ?= %d\n", numContained, 2);
  assert(numContained == 2);

  XDMFGRIDCOLLECTION * readCollection = 
    XdmfGridCollectionGetGridCollection(readMainCollection, 0);

  valueString = XdmfGridCollectionGetName(readCollection);
  printf("First Collection name = %s\n", valueString);
  assert(strcmp(valueString, "Test Grid Collection") == 0);
  free(valueString);

  XDMFTIME * readTime = XdmfGridCollectionGetTime(readCollection);
  double timeVal = XdmfTimeGetValue(readTime);
  printf("%lf ?= %lf\n", timeVal, 21.0);
  assert(timeVal == 21.0);
  XdmfTimeFree(readTime);
  XdmfGridCollectionFree(readCollection);

  XDMFGRIDCOLLECTION * readCollection2 = 
    XdmfGridCollectionGetGridCollectionByName(readMainCollection, 
					      "Second Test Grid Collection");

  valueString = XdmfGridCollectionGetName(readCollection2);
  printf("%s ?= %s\n", valueString, "Second Test Grid Collection");
  assert(strcmp(valueString, "Second Test Grid Collection") == 0);
  free(valueString);
  
  XDMFTIME * readTime2 = XdmfGridCollectionGetTime(readCollection2);
  double timeVal2 = XdmfTimeGetValue(readTime2);
  printf("%lf ?= %lf\n", timeVal2, 12.0);
  assert(timeVal2 == 12.0);
  XdmfTimeFree(readTime2);
  XdmfGridCollectionFree(readCollection2);

  XdmfGridCollectionRemoveGridCollectionByName(readMainCollection, 
					       "Test Grid Collection");
  numContained = XdmfGridCollectionGetNumberGridCollections(readMainCollection);
  printf("%d ?= %d\n", numContained, 1);
  assert(numContained == 1);

  XDMFGRIDCOLLECTION * readCollection3 = 
    XdmfGridCollectionGetGridCollection(readMainCollection, 0);
  valueString = XdmfGridCollectionGetName(readCollection3);
  printf("%s ?= %s\n", valueString, "Second Test Grid Collection");
  assert(strcmp(valueString, "Second Test Grid Collection") == 0);
  free(valueString);
  XdmfGridCollectionFree(readCollection3);

  XdmfGridCollectionRemoveGridCollection(readMainCollection, 0);
  numContained = XdmfGridCollectionGetNumberGridCollections(readMainCollection);
  printf("%d ?= %d\n", numContained, 0);
  assert(numContained == 0);
  printf("checking map, set, and attribute\n");

  // Map

  numContained = XdmfGridCollectionGetNumberMaps(readMainCollection);
  printf("contains %d maps\n", numContained);
  assert(numContained == 2);

  XDMFMAP * readMap = XdmfGridCollectionGetMap(readMainCollection, 0);

  XdmfMapRead(readMap, &status);
  valueString = XdmfMapGetName(readMap);
  printf("%s ?= %s\n", valueString, "Test Map");
  assert(strcmp(valueString, "Test Map") == 0);
  free(valueString);

  int testNumRemoteTaskIds;
  int * remoteTaskIds;
  int testNumLocalNodeIds;
  int * localNodeIds;
  int testNumRemoteNodeIds;
  int * remoteNodeIds;

  int expectedRemoteTasks[3];
  expectedRemoteTasks[0] = 1;
  expectedRemoteTasks[1] = 2;
  expectedRemoteTasks[2] = 3;


  int expectedLocalNodeIds[3][4];
  expectedLocalNodeIds[0][0] = 0;
  expectedLocalNodeIds[0][1] = 3;
  expectedLocalNodeIds[0][2] = 5;
  expectedLocalNodeIds[1][0] = 4;
  expectedLocalNodeIds[1][1] = 7;
  expectedLocalNodeIds[2][0] = 1;
  expectedLocalNodeIds[2][1] = 2;
  expectedLocalNodeIds[2][2] = 6;
  expectedLocalNodeIds[2][3] = 8;


  int expectedRemoteNodeIds[3][4][1];
  expectedRemoteNodeIds[0][0][0] = 8;
  expectedRemoteNodeIds[0][1][0] = 7;
  expectedRemoteNodeIds[0][2][0] = 9;
  expectedRemoteNodeIds[1][0][0] = 6;
  expectedRemoteNodeIds[1][1][0] = 5;
  expectedRemoteNodeIds[2][0][0] = 1;
  expectedRemoteNodeIds[2][1][0] = 4;
  expectedRemoteNodeIds[2][2][0] = 2;
  expectedRemoteNodeIds[2][3][0] = 3;

  testNumRemoteTaskIds = XdmfMapRetrieveNumberRemoteTaskIds(readMap);

  remoteTaskIds = XdmfMapRetrieveRemoteTaskIds(readMap);

  for (i = 0; i < testNumRemoteTaskIds; ++i) {
    printf("remote task id %d\n", remoteTaskIds[i]);
    assert(expectedRemoteTasks[i] == remoteTaskIds[i]);
    testNumLocalNodeIds = XdmfMapRetrieveNumberLocalNodeIds(readMap, remoteTaskIds[i]);
    localNodeIds = XdmfMapRetrieveLocalNodeIds(readMap, remoteTaskIds[i]);
    for (j = 0; j < testNumLocalNodeIds; ++j) {
      printf("\tlocal node id %d == %d\n", localNodeIds[j], expectedLocalNodeIds[i][j]);
      assert(localNodeIds[j] == expectedLocalNodeIds[i][j]);
      testNumRemoteNodeIds = XdmfMapRetrieveNumberRemoteNodeIds(readMap, remoteTaskIds[i], localNodeIds[j]);
      remoteNodeIds = XdmfMapRetrieveRemoteNodeIds(readMap, remoteTaskIds[i], localNodeIds[j]);
      for (k = 0; k < testNumRemoteNodeIds; ++k) {
        printf("\t\tremote node id %d == %d\n", remoteNodeIds[k], expectedRemoteNodeIds[i][j][k]);
        assert(remoteNodeIds[k] == expectedRemoteNodeIds[i][j][k]);
      }
      free(remoteNodeIds);
    }
    free(localNodeIds);
  }
  free(remoteTaskIds);

  XdmfMapFree(readMap);

  XDMFMAP * readMap2 = 
    XdmfGridCollectionGetMapByName(readMainCollection, "Second Test Map");

  valueString = XdmfMapGetName(readMap2);
  printf("%s ?= %s\n", valueString, "Second Test Map");
  assert(strcmp(valueString, "Second Test Map") == 0);
  free(valueString);

  testNumRemoteTaskIds = XdmfMapRetrieveNumberRemoteTaskIds(readMap2);
  remoteTaskIds = XdmfMapRetrieveRemoteTaskIds(readMap2);

  for (i = 0; i < testNumRemoteTaskIds; ++i) {
    printf("remote task id %d\n", remoteTaskIds[i]);
    assert(expectedRemoteTasks[i] == remoteTaskIds[i]);
    testNumLocalNodeIds = XdmfMapRetrieveNumberLocalNodeIds(readMap2, remoteTaskIds[i]);
    localNodeIds = XdmfMapRetrieveLocalNodeIds(readMap2, remoteTaskIds[i]);
    for (j = 0; j < testNumLocalNodeIds; ++j) {
      printf("\tlocal node id %d == %d\n", localNodeIds[j], expectedLocalNodeIds[i][j]);
      assert(localNodeIds[j] == expectedLocalNodeIds[i][j]);
      testNumRemoteNodeIds = XdmfMapRetrieveNumberRemoteNodeIds(readMap2, remoteTaskIds[i], localNodeIds[j]);
      remoteNodeIds = XdmfMapRetrieveRemoteNodeIds(readMap2, remoteTaskIds[i], localNodeIds[j]);
      for (k = 0; k < testNumRemoteNodeIds; ++k) {
        printf("\t\tremote node id %d == %d\n", remoteNodeIds[k], expectedRemoteNodeIds[i][j][k]);
        assert(remoteNodeIds[k] == expectedRemoteNodeIds[i][j][k]);
      }
      free(remoteNodeIds);
    }
    free(localNodeIds);
  }
  free(remoteTaskIds);

  XdmfMapFree(readMap2);

  XdmfGridCollectionRemoveMapByName(readMainCollection, "Test Map");
  numContained = XdmfGridCollectionGetNumberMaps(readMainCollection);
  printf("%d ?= %d\n", numContained, 1);
  assert(numContained == 1);

  XDMFMAP * readMap3 = XdmfGridCollectionGetMap(readMainCollection, 0);
  valueString = XdmfMapGetName(readMap3);
  printf("%s ?= %s\n", valueString, "Second Test Map");
  assert(strcmp(valueString, "Second Test Map") == 0);
  free(valueString);
  XdmfMapFree(readMap3);

  XdmfGridCollectionRemoveMap(readMainCollection, 0);
  numContained = XdmfGridCollectionGetNumberMaps(readMainCollection);
  printf("%d ?= %d\n", numContained, 0);
  assert(numContained == 0);

  // Set

  numContained = XdmfGridCollectionGetNumberSets(readMainCollection);
  printf("%d ?= %d\n", numContained, 2);
  assert(numContained == 2);

  XDMFSET * readSet = XdmfGridCollectionGetSet(readMainCollection, 0);
  
  valueString = XdmfSetGetName(readSet);
  printf("%s ?= %s\n", valueString, "Test Set");
  assert(strcmp(valueString, "Test Set") == 0);
  free(valueString);

  valueString = XdmfSetGetValuesString(readSet);
  printf("%s ?= %s\n", valueString, "0 1 2 3 4 5 6 7 8 9");
  assert(strcmp(valueString, "0 1 2 3 4 5 6 7 8 9") == 0);
  free(valueString);
  XdmfSetFree(readSet);


  XDMFSET * readSet2 = 
    XdmfGridCollectionGetSetByName(readMainCollection, "Second Test Set");

  valueString = XdmfSetGetName(readSet2);
  printf("%s ?= %s\n", valueString, "Second Test Set");
  assert(strcmp(valueString, "Second Test Set") == 0);
  free(valueString);
  
  valueString = XdmfSetGetValuesString(readSet2);
  printf("%s ?= %s\n", valueString, "10 11 12 13 14 15 16 17 18 19");
  assert(strcmp(valueString, "10 11 12 13 14 15 16 17 18 19") == 0);
  free(valueString);
  XdmfSetFree(readSet2);

  XdmfGridCollectionRemoveSetByName(readMainCollection, "Test Set");
  numContained = XdmfGridCollectionGetNumberSets(readMainCollection);
  printf("%d ?= %d\n", numContained, 1);
  assert(numContained == 1);

  XDMFSET * readSet3 = XdmfGridCollectionGetSet(readMainCollection, 0);

  valueString = XdmfSetGetName(readSet3);
  printf("%s ?= %s\n", valueString, "Second Test Set");
  assert(strcmp(valueString, "Second Test Set") == 0);
  free(valueString);
  XdmfSetFree(readSet3);

  XdmfGridCollectionRemoveSet(readMainCollection, 0);
  numContained = XdmfGridCollectionGetNumberSets(readMainCollection);
  printf("%d ?= %d\n", numContained, 0);
  assert(numContained == 0);

  // Attribute

  numContained = XdmfGridCollectionGetNumberAttributes(readMainCollection);
  printf("%d ?= %d\n", numContained, 2);
  assert(numContained == 2);

  XDMFATTRIBUTE * readAttribute = 
    XdmfGridCollectionGetAttribute(readMainCollection, 0);

  valueString = XdmfAttributeGetName(readAttribute);
  printf("%s ?= %s\n", valueString, "Test Attribute");
  assert(strcmp(valueString, "Test Attribute") == 0);
  free(valueString);

  valueString = XdmfAttributeGetValuesString(readAttribute);
  printf("%s ?= %s\n", valueString, "0 1 2 3 4 5 6 7 8 9");
  assert(strcmp(valueString, "0 1 2 3 4 5 6 7 8 9") == 0);
  free(valueString);
  XdmfAttributeFree(readAttribute);


  XDMFATTRIBUTE * readAttribute2 = 
    XdmfGridCollectionGetAttributeByName(readMainCollection, 
					 "Second Test Attribute");

  valueString = XdmfAttributeGetName(readAttribute2);
  printf("%s ?= %s\n", valueString, "Second Test Attribute");
  assert(strcmp(valueString, "Second Test Attribute") == 0);
  free(valueString);

  valueString = XdmfAttributeGetValuesString(readAttribute2);
  printf("%s ?= %s\n", valueString, "10 11 12 13 14 15 16 17 18 19");
  assert(strcmp(valueString, "10 11 12 13 14 15 16 17 18 19") == 0);
  free(valueString);
  XdmfAttributeFree(readAttribute2);

  XdmfGridCollectionRemoveAttributeByName(readMainCollection, "Test Attribute");
  numContained = XdmfGridCollectionGetNumberAttributes(readMainCollection);
  printf("%d ?= %d\n", numContained, 1);
  assert(numContained == 1);

  XDMFATTRIBUTE * readAttribute3 = 
    XdmfGridCollectionGetAttribute(readMainCollection, 0);

  valueString = XdmfAttributeGetName(readAttribute3);
  printf("%s ?= %s\n", valueString, "Second Test Attribute");
  assert(strcmp(valueString, "Second Test Attribute") == 0);
  free(valueString);
  XdmfAttributeFree(readAttribute3);

  XdmfGridCollectionRemoveAttribute(readMainCollection, 0);
  numContained = XdmfGridCollectionGetNumberAttributes(readMainCollection);
  printf("%d ?= %d\n", numContained, 0);
  assert(numContained == 0);

  XdmfGridCollectionFree(readMainCollection);

  return 0;
}
