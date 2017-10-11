#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfWriter.hpp"
#include "XdmfHDF5Writer.hpp"
#include "XdmfReader.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfTopology.hpp"
#include "XdmfTime.hpp"
#include "XdmfGeometryType.hpp"
#include "XdmfRegularGrid.hpp"
#include "XdmfUnstructuredGrid.hpp"

#include "stdio.h"
#include "stdlib.h"

#include "string.h"
#include "assert.h"

int main()
{

  printf("test start\n");

  int i = 0;

  int status = 0;

  XDMFWRITER * writer = XdmfWriterNew("unfile.xmf");

  // Read from File

  void * reader = XdmfReaderNew();

  printf("generating regular grid\n");

  void * regGrid = XdmfRegularGridNew2D(5, 5, 5, 5, 0, 0);

  printf("generating unstructured grid\n");

  void * unGrid = XdmfUnstructuredGridNewFromRegularGrid(regGrid, &status);

  void * unTime = XdmfTimeNew(0.0);

  double testTimeVal = XdmfTimeGetValue(unTime);

  printf("%lf ?= %lf\n", testTimeVal, 0.0);

  assert(testTimeVal == 0.0);

  XdmfTimeSetValue(unTime, 5.0);

  XdmfUnstructuredGridSetTime(unGrid, unTime, 0);

  XdmfUnstructuredGridAccept(unGrid, (XDMFVISITOR *)writer, &status);

  void * readGrid = XdmfReaderRead(reader, "unfile.xmf", &status);

  void * readTime = XdmfUnstructuredGridGetTime(readGrid);

  double readTimeVal = XdmfTimeGetValue(readTime);

  printf("%lf ?= %lf\n", readTimeVal, 5.0);

  assert(readTimeVal == 5.0);

  XDMFTOPOLOGY * readTopology = XdmfUnstructuredGridGetTopology(readGrid);

  char * valueString = XdmfTopologyGetValuesString(readTopology);

  printf("unstructured topology contains: %s\n", valueString);

  assert(strcmp(valueString, "0 1 6 5 1 2 7 6 2 3 8 7 3 4 9 8 5 6 11 10 6 7 12 11 7 8 13 12 8 9 14 13 10 11 16 15 11 12 17 16 12 13 18 17 13 14 19 18 15 16 21 20 16 17 22 21 17 18 23 22 18 19 24 23") == 0);

  free(valueString);

  XDMFGEOMETRY * readGeometry = XdmfUnstructuredGridGetGeometry(readGrid);

  valueString = XdmfGeometryGetValuesString(readGeometry);

  printf("unstructured geometry contains: %s\n", valueString);

  assert(strcmp(valueString, "0 0 5 0 10 0 15 0 20 0 0 5 5 5 10 5 15 5 20 5 0 10 5 10 10 10 15 10 20 10 0 15 5 15 10 15 15 15 20 15 0 20 5 20 10 20 15 20 20 20") == 0);

  free(valueString);

  XDMFGEOMETRY * newGeometry = XdmfGeometryNew();

  for (i = 0; i < 10; ++i) {
    XdmfGeometryPushBack(newGeometry, &i, XDMF_ARRAY_TYPE_INT32, &status);
  }

  XdmfUnstructuredGridSetGeometry(readGrid, newGeometry, 0);

  void * newTopology = XdmfTopologyNew();

  for (i = 10; i < 20; ++i) {
    XdmfTopologyPushBack(newTopology, &i, XDMF_ARRAY_TYPE_INT32, &status);
  }

  XdmfUnstructuredGridSetTopology(readGrid, newTopology, 0);

  readTopology = XdmfUnstructuredGridGetTopology(readGrid);

  valueString = XdmfTopologyGetValuesString(readTopology);

  printf("unstructured topology contains: %s\n", valueString);

  assert(strcmp(valueString, "10 11 12 13 14 15 16 17 18 19") == 0);

  free(valueString);

  readGeometry = XdmfUnstructuredGridGetGeometry(readGrid);

  valueString = XdmfGeometryGetValuesString(readGeometry);

  printf("unstructured geometry contains: %s\n", valueString);

  assert(strcmp(valueString, "0 1 2 3 4 5 6 7 8 9") == 0);

  free(valueString);


  return 0;
}
