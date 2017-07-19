#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfWriter.hpp"
#include "XdmfHDF5Writer.hpp"
#include "XdmfReader.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfGeometryType.hpp"

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

  XDMFWRITER * writer = XdmfWriterNew("geofile.xmf");

  int testype = XdmfGeometryGetType(geometry);
  printf("%d ?= %d\n", testype, XDMF_GEOMETRY_TYPE_XY);
  assert(testype == XDMF_GEOMETRY_TYPE_XY);

  // Write to File
  XdmfGeometryAccept(geometry, (XDMFVISITOR *)writer, &status);
  XdmfGeometryFree(geometry);
  XdmfWriterFree(writer);

  // Read from File
  XDMFREADER * reader = XdmfReaderNew();
  XDMFITEM * readItem = XdmfReaderRead(reader, "geofile.xmf", &status);
  XDMFGEOMETRY * readGeometry = XdmfGeometryCast(readItem);
  XdmfItemFree(readItem);
  XdmfReaderFree(reader);

  char * valueString = XdmfGeometryGetItemTag(readGeometry);
  printf("%s ?= %s\n", valueString, "Geometry");
  assert(strcmp(valueString, "Geometry") == 0);
  free(valueString);

  XdmfGeometryRead(readGeometry, &status);
  unsigned int numPoints = XdmfGeometryGetNumberPoints(readGeometry);
  printf("%d ?= %d\n", numPoints, 5);
  assert(numPoints == 5);

  int geotype = XdmfGeometryGetType(readGeometry);
  printf("Geometry type code = %d\n", geotype);
  assert(geotype == XDMF_GEOMETRY_TYPE_XY);

  unsigned int numDims = XdmfGeometryTypeGetDimensions(geotype, &status);
  printf("%d ?= %d\n", numDims, 2);
  assert(numDims == 2);

  valueString = XdmfGeometryTypeGetName(geotype);
  printf("Geometry type name: %s\n", valueString);
  free(valueString);

  valueString = XdmfGeometryGetValuesString(readGeometry);
  printf("array contains: %s\n", valueString);
  assert(strcmp("0 1 2 3 4 5 6 7 8 9", valueString) == 0);
  free(valueString);

  XdmfGeometryFree(readGeometry);

  return 0;
}
