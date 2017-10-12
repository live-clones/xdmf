#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfAttribute.hpp"
#include "XdmfAttributeType.hpp"
#include "XdmfAttributeCenter.hpp"
#include "XdmfWriter.hpp"
#include "XdmfHDF5Writer.hpp"
#include "XdmfReader.hpp"

#include "stdio.h"
#include "stdlib.h"

#include "string.h"
#include "assert.h"

int main()
{

  XDMFATTRIBUTE * attrib = XdmfAttributeNew();

  int i = 0;

  int status = 0;

  for (i = 0; i < 10; i++) {
    XdmfAttributePushBack(attrib, &i, XDMF_ARRAY_TYPE_INT32, &status);
  }

  XdmfAttributeSetType(attrib, XDMF_ATTRIBUTE_TYPE_SCALAR, &status);

  XdmfAttributeSetCenter(attrib, XDMF_ATTRIBUTE_CENTER_NODE, &status);

  XDMFWRITER * writer = XdmfWriterNew("attributefile.xmf");

  int testype = XdmfAttributeGetType(attrib);

  printf("%d ?= %d\n", testype, XDMF_ATTRIBUTE_TYPE_SCALAR);

  assert(testype == XDMF_ATTRIBUTE_TYPE_SCALAR);

  int testcenter = XdmfAttributeGetCenter(attrib);

  printf("%d ?= %d\n", testcenter, XDMF_ATTRIBUTE_CENTER_NODE);

  assert(testcenter == XDMF_ATTRIBUTE_CENTER_NODE);

  // Write to File

  XdmfAttributeAccept(attrib, (XDMFVISITOR *)writer, &status);

  // Read from File

  void * reader = XdmfReaderNew();

  void * readArray = XdmfReaderRead(reader, "attributefile.xmf", &status);

  char * valueString = XdmfAttributeGetItemTag(readArray);

  printf("%s ?= %s\n", valueString, "Attribute");

  assert(strcmp(valueString, "Attribute") == 0);

  XdmfAttributeRead(readArray, &status);

  int attributetype = XdmfAttributeGetType(readArray);

  printf("Attribute type code = %d\n", attributetype);

  assert(attributetype == XDMF_ATTRIBUTE_TYPE_SCALAR);

  int attributecenter = XdmfAttributeGetCenter(readArray);

  printf("Attribute center code = %d\n", attributecenter);

  assert(attributecenter == XDMF_ATTRIBUTE_CENTER_NODE);

  valueString = XdmfAttributeGetValuesString(readArray);

  printf("array contains: %s\n", valueString);

  assert(strcmp("0 1 2 3 4 5 6 7 8 9", valueString) == 0);

  free(readArray);

  free(valueString);

  return 0;
}
