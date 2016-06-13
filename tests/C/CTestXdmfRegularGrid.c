#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfWriter.hpp"
#include "XdmfHDF5Writer.hpp"
#include "XdmfReader.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfGeometryType.hpp"
#include "XdmfRegularGrid.hpp"

#include "stdio.h"
#include "stdlib.h"

#include "string.h"
#include "assert.h"

int main()
{

  int i = 0;

  int status = 0;

  XDMFWRITER * writer = XdmfWriterNew("regfile.xmf");

  // Read from File

  void * reader = XdmfReaderNew();

  void * regGrid = XdmfRegularGridNew2D(5, 5, 5, 5, 0, 0);

  XdmfRegularGridAccept(regGrid, (XDMFVISITOR *)writer, &status);

  void * readGrid = XdmfReaderRead(reader, "regfile.xmf", &status);

  void * readBrick = XdmfRegularGridGetBrickSize(readGrid, &status);

  char * valueString = XdmfArrayGetValuesString(readBrick);

  printf("brick contains %s\n", valueString);

  assert(strcmp(valueString, "5 5") == 0);

  free(valueString);

  free(readBrick);

  void * readDimensions = XdmfRegularGridGetDimensions(readGrid, &status);

  valueString = XdmfArrayGetValuesString(readDimensions);

  printf("dimensions contains %s\n", valueString);

  assert(strcmp(valueString, "5 5") == 0);

  free(valueString);

  free(readDimensions);

  void * readOrigin = XdmfRegularGridGetOrigin(readGrid, &status);

  valueString = XdmfArrayGetValuesString(readOrigin);

  printf("origin contains %s\n", valueString);

  assert(strcmp(valueString, "0 0") == 0);

  free(valueString);

  free(readOrigin);

  int insertVal = 7;

  void * newBrick = XdmfArrayNew();

  XdmfArrayPushBack(newBrick, &insertVal, XDMF_ARRAY_TYPE_INT32, &status);
  XdmfArrayPushBack(newBrick, &insertVal, XDMF_ARRAY_TYPE_INT32, &status);

  XdmfRegularGridSetBrickSize(readGrid, newBrick, 0, &status);

  insertVal = 6;

  void * newDimensions = XdmfArrayNew();

  XdmfArrayPushBack(newDimensions, &insertVal, XDMF_ARRAY_TYPE_INT32, &status);
  XdmfArrayPushBack(newDimensions, &insertVal, XDMF_ARRAY_TYPE_INT32, &status);

  XdmfRegularGridSetDimensions(readGrid, newDimensions, 0, &status);

  insertVal = 1;

  void * newOrigin = XdmfArrayNew();

  XdmfArrayPushBack(newOrigin, &insertVal, XDMF_ARRAY_TYPE_INT32, &status);
  XdmfArrayPushBack(newOrigin, &insertVal, XDMF_ARRAY_TYPE_INT32, &status);

  XdmfRegularGridSetOrigin(readGrid, newOrigin, 0, &status);

  readBrick = XdmfRegularGridGetBrickSize(readGrid, &status);

  valueString = XdmfArrayGetValuesString(readBrick);

  printf("brick contains %s\n", valueString);

  assert(strcmp(valueString, "7 7") == 0);

  free(valueString);

  readDimensions = XdmfRegularGridGetDimensions(readGrid, &status);

  valueString = XdmfArrayGetValuesString(readDimensions);

  printf("dimensions contains %s\n", valueString);

  assert(strcmp(valueString, "6 6") == 0);

  free(valueString);

  readOrigin = XdmfRegularGridGetOrigin(readGrid, &status);

  valueString = XdmfArrayGetValuesString(readOrigin);

  printf("origin contains %s\n", valueString);

  assert(strcmp(valueString, "1 1") == 0);

  free(valueString);



/*
XDMF_EXPORT void * XdmfRegularGridNew(void * brickSize,
                                      void * numPoints,
                                      void * origin,
                                      int passControl);
*/

  return 0;
}
