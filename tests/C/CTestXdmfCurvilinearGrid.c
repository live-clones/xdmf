#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfWriter.hpp"
#include "XdmfHDF5Writer.hpp"
#include "XdmfReader.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfGeometryType.hpp"
#include "XdmfCurvilinearGrid.hpp"

#include "stdio.h"
#include "stdlib.h"

#include "string.h"
#include "assert.h"

int main()
{

  XDMFGEOMETRY * geometry = XdmfGeometryNew();

  int i = 0;

  int status = 0;

  for (i = 0; i < 10; i++) {
    XdmfGeometryPushBack(geometry, &i, XDMF_ARRAY_TYPE_INT32, &status);
  }

  XdmfGeometrySetType(geometry, XDMF_GEOMETRY_TYPE_XY, &status);

  XDMFWRITER * writer = XdmfWriterNew("curvfile.xmf");

  int testype = XdmfGeometryGetType(geometry);

  printf("%d ?= %d\n", testype, XDMF_GEOMETRY_TYPE_XY);

  assert(testype == XDMF_GEOMETRY_TYPE_XY);

  // Write to File

  XdmfGeometryAccept(geometry, (XDMFVISITOR *)writer, &status);

  // Read from File

  void * reader = XdmfReaderNew();

  void * readArray = XdmfReaderRead(reader, "curvfile.xmf", &status);

  char * valueString = XdmfGeometryGetItemTag(readArray);

  printf("%s ?= %s\n", valueString, "Geometry");

  assert(strcmp(valueString, "Geometry") == 0);

  free(valueString);

  XdmfGeometryRead(readArray, &status);

  unsigned int numPoints = XdmfGeometryGetNumberPoints(readArray);

  printf("%d ?= %d\n", numPoints, 5);

  assert(numPoints == 5);

  int geotype = XdmfGeometryGetType(readArray);

  printf("Geometry type code = %d\n", geotype);

  assert(geotype == XDMF_GEOMETRY_TYPE_XY);

  unsigned int numDims = XdmfGeometryTypeGetDimensions(geotype, &status);

  assert(numDims == 2);

  valueString = XdmfGeometryTypeGetName(geotype);

  printf("Geometry type name: %s\n", valueString);

  free(valueString);

  valueString = XdmfGeometryGetValuesString(readArray);

  printf("array contains: %s\n", valueString);

  assert(strcmp("0 1 2 3 4 5 6 7 8 9", valueString) == 0);

  XdmfItemFree(readArray);

  free(valueString);

  void * curvGrid = XdmfCurvilinearGridNew2D(2, 5);

  XdmfCurvilinearGridSetGeometry(curvGrid, geometry, 1);

  void * dimensions = XdmfArrayNew();

  int dimArray[2];
  dimArray[0] = 5;
  dimArray[1] = 2;

  XdmfArrayPushBack(dimensions, &dimArray[0], XDMF_ARRAY_TYPE_INT32, &status);
  XdmfArrayPushBack(dimensions, &dimArray[1], XDMF_ARRAY_TYPE_INT32, &status);

  XdmfCurvilinearGridSetDimensions(curvGrid, dimensions, 1, &status);

  printf("writing to file\n");

  XdmfCurvilinearGridAccept(curvGrid, (XDMFVISITOR *)writer, &status);

//  XdmfItemFree(curvGrid);

  void * readGrid = XdmfReaderRead(reader, "curvfile.xmf", &status);

  XDMFGEOMETRY * childGeometry = XdmfCurvilinearGridGetGeometry(readGrid);

  valueString = XdmfGeometryGetValuesString(childGeometry);

  printf("%s ?= %s\n", valueString, "0 1 2 3 4 5 6 7 8 9");

  assert(strcmp(valueString, "0 1 2 3 4 5 6 7 8 9") == 0);

  free(valueString);

  void * childDimensions = XdmfCurvilinearGridGetDimensions(readGrid, &status);

  valueString = XdmfArrayGetValuesString(childDimensions);

  printf("%s ?= %s\n", valueString, "5 2");

  assert(strcmp(valueString, "5 2") == 0);

  return 0;
}
