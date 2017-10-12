#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfWriter.hpp"
#include "XdmfHDF5Writer.hpp"
#include "XdmfReader.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfGeometryType.hpp"
#include "XdmfRectilinearGrid.hpp"

#include "stdio.h"
#include "stdlib.h"

#include "string.h"
#include "assert.h"

int main()
{

  int i = 0;

  int status = 0;

  XDMFWRITER * writer = XdmfWriterNew("rectfile.xmf");

  // Read from File

  void * reader = XdmfReaderNew();

  void * newXDim = XdmfArrayNew();

  for (i = 5; i < 9; ++i) {
    XdmfArrayPushBack(newXDim, &i, XDMF_ARRAY_TYPE_INT32, &status);
  }

  void * newYDim = XdmfArrayNew();

  for (i = 6; i < 10; ++i) {
    XdmfArrayPushBack(newYDim, &i, XDMF_ARRAY_TYPE_INT32, &status);
  }

  void * rectGrid = XdmfRectilinearGridNew2D(newXDim, newYDim, 0);

  XdmfRectilinearGridAccept(rectGrid, (XDMFVISITOR *)writer, &status);

  void * readGrid = XdmfReaderRead(reader, "rectfile.xmf", &status);

  int numCoordinates = XdmfRectilinearGridGetNumberCoordinates(readGrid, &status);

  printf("%d ?= %d\n", numCoordinates, 2);

  assert(numCoordinates == 2);

  XDMFARRAY ** readCoordinateArray = XdmfRectilinearGridGetCoordinates(readGrid, &status);

  char * valueString = XdmfArrayGetValuesString(readCoordinateArray[0]);

  printf("first dimension contains %s\n", valueString);

  assert(strcmp(valueString, "5 6 7 8") == 0);

  free(valueString);

  valueString = XdmfArrayGetValuesString(readCoordinateArray[1]);

  printf("second dimension contains %s\n", valueString);

  assert(strcmp(valueString, "6 7 8 9") == 0);

  free(valueString);

  void * readDimensions = XdmfRectilinearGridGetDimensions(readGrid, &status);

  valueString = XdmfArrayGetValuesString(readDimensions);

  printf("dimensions contain %s\n", valueString);

  assert(strcmp(valueString, "4 4") == 0);

  free(valueString);

  void * childGrid = XdmfRectilinearGridNew(readCoordinateArray, 2, 0);

  void * checkCoordinate = XdmfRectilinearGridGetCoordinatesByIndex(childGrid, 1, &status);

  valueString = XdmfArrayGetValuesString(checkCoordinate);

  printf("second dimension contains %s\n", valueString);

  assert(strcmp(valueString, "6 7 8 9") == 0);

  free(checkCoordinate);

  free(valueString);

  XDMFARRAY * newCoordinates[2];

  newCoordinates[0] = XdmfArrayNew();

  for (i = 0; i < 10; i++) {
    XdmfArrayPushBack(newCoordinates[0], &i, XDMF_ARRAY_TYPE_INT32, &status);
  }

  newCoordinates[1] = XdmfArrayNew();

  for (i = 0; i < 10; i++) {
    XdmfArrayPushBack(newCoordinates[1], &i, XDMF_ARRAY_TYPE_INT32, &status);
  }

  XdmfRectilinearGridSetCoordinates(childGrid, newCoordinates, 2, 0, &status);

  checkCoordinate = XdmfRectilinearGridGetCoordinatesByIndex(childGrid, 0, &status);

  valueString = XdmfArrayGetValuesString(checkCoordinate);

  printf("replaced dimension contains %s\n", valueString);

  assert(strcmp(valueString, "0 1 2 3 4 5 6 7 8 9") == 0);

  free(checkCoordinate);

  free(valueString);

  void * replacementCoordinate = XdmfArrayNew();

  for (i = 10; i < 20; i++) {
    XdmfArrayPushBack(replacementCoordinate, &i, XDMF_ARRAY_TYPE_INT32, &status);
  }

  XdmfRectilinearGridSetCoordinatesByIndex(childGrid, 0, replacementCoordinate, 0, &status);

  checkCoordinate = XdmfRectilinearGridGetCoordinatesByIndex(childGrid, 0, &status);

  valueString = XdmfArrayGetValuesString(checkCoordinate);

  printf("replaced dimension contains %s\n", valueString);

  assert(strcmp(valueString, "10 11 12 13 14 15 16 17 18 19") == 0);

  free(checkCoordinate);

  free(valueString);

  return 0;
}
