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

  int status = 0;

  XDMFWRITER * writer = XdmfWriterNew("regfile.xmf");

  // Read from File

  XDMFREADER * reader = XdmfReaderNew();

  XDMFREGULARGRID * regGrid = XdmfRegularGridNew2D(5, 5, 5, 5, 0, 0);

  XdmfRegularGridAccept(regGrid, (XDMFVISITOR *)writer, &status);
  XdmfWriterFree(writer);
  XdmfRegularGridFree(regGrid);

  XDMFITEM * readItem = XdmfReaderRead(reader, "regfile.xmf", &status);
  XdmfReaderFree(reader);
  XDMFREGULARGRID * readGrid = XdmfRegularGridCast(readItem);
  XdmfItemFree(readItem);

  XDMFARRAY * readBrick = XdmfRegularGridGetBrickSize(readGrid, &status);
  char * valueString = XdmfArrayGetValuesString(readBrick);
  printf("brick contains %s\n", valueString);
  assert(strcmp(valueString, "5 5") == 0);
  free(valueString);
  XdmfArrayFree(readBrick);

  XDMFARRAY * readDimensions = XdmfRegularGridGetDimensions(readGrid, &status);
  valueString = XdmfArrayGetValuesString(readDimensions);
  printf("dimensions contains %s\n", valueString);
  assert(strcmp(valueString, "5 5") == 0);
  free(valueString);
  XdmfArrayFree(readDimensions);

  XDMFARRAY * readOrigin = XdmfRegularGridGetOrigin(readGrid, &status);
  valueString = XdmfArrayGetValuesString(readOrigin);
  printf("origin contains %s\n", valueString);
  assert(strcmp(valueString, "0 0") == 0);
  free(valueString);
  XdmfArrayFree(readOrigin);

  int insertVal = 7;

  XDMFARRAY * newBrick = XdmfArrayNew();
  XdmfArrayPushBack(newBrick, &insertVal, XDMF_ARRAY_TYPE_INT32, &status);
  XdmfArrayPushBack(newBrick, &insertVal, XDMF_ARRAY_TYPE_INT32, &status);
  XdmfRegularGridSetBrickSize(readGrid, newBrick, 0, &status);
  XdmfArrayFree(newBrick);

  insertVal = 6;
  XDMFARRAY * newDimensions = XdmfArrayNew();
  XdmfArrayPushBack(newDimensions, &insertVal, XDMF_ARRAY_TYPE_INT32, &status);
  XdmfArrayPushBack(newDimensions, &insertVal, XDMF_ARRAY_TYPE_INT32, &status);
  XdmfRegularGridSetDimensions(readGrid, newDimensions, 0, &status);
  XdmfArrayFree(newDimensions);

  insertVal = 1;
  XDMFARRAY * newOrigin = XdmfArrayNew();
  XdmfArrayPushBack(newOrigin, &insertVal, XDMF_ARRAY_TYPE_INT32, &status);
  XdmfArrayPushBack(newOrigin, &insertVal, XDMF_ARRAY_TYPE_INT32, &status);
  XdmfRegularGridSetOrigin(readGrid, newOrigin, 0, &status);
  XdmfArrayFree(newOrigin);

  readBrick = XdmfRegularGridGetBrickSize(readGrid, &status);
  valueString = XdmfArrayGetValuesString(readBrick);
  printf("brick contains %s\n", valueString);
  assert(strcmp(valueString, "7 7") == 0);
  free(valueString);
  XdmfArrayFree(readBrick);

  readDimensions = XdmfRegularGridGetDimensions(readGrid, &status);
  valueString = XdmfArrayGetValuesString(readDimensions);
  printf("dimensions contains %s\n", valueString);
  assert(strcmp(valueString, "6 6") == 0);
  free(valueString);
  XdmfArrayFree(readDimensions);

  readOrigin = XdmfRegularGridGetOrigin(readGrid, &status);
  valueString = XdmfArrayGetValuesString(readOrigin);
  printf("origin contains %s\n", valueString);
  assert(strcmp(valueString, "1 1") == 0);
  free(valueString);
  XdmfArrayFree(readOrigin);

  XdmfRegularGridFree(readGrid);

  return 0;
}
