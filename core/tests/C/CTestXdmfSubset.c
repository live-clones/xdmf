#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfSubset.hpp"
#include "XdmfWriter.hpp"

#include "assert.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"

int main()
{
  void * array = XdmfArrayNew();

  int i = 0;

  int status = 0;

  for (i = 0; i < 10; i++) {
    XdmfArrayPushBack(array, &i, XDMF_ARRAY_TYPE_INT32, &status);
  }

  char * valueString = XdmfArrayGetValuesString(array);

  printf("%s ?= %s\n", "0 1 2 3 4 5 6 7 8 9", valueString);

  assert(strcmp("0 1 2 3 4 5 6 7 8 9", valueString) == 0);

  free(valueString);

  unsigned int starts[1] = {0};
  unsigned int strides[1] = {2};
  unsigned int dimensions[1] = {5};

  unsigned int numDims = 1;

  XDMFSUBSET * subset = XdmfSubsetNew(array, starts, strides, dimensions, numDims, 0, &status);

  void * internalArray = XdmfSubsetGetReferenceArray(subset);

  valueString = XdmfArrayGetValuesString(internalArray);

  printf("%s ?= %s\n", "0 1 2 3 4 5 6 7 8 9", valueString);

  assert(strcmp("0 1 2 3 4 5 6 7 8 9", valueString) == 0);

  free(valueString);

  unsigned int internalNumDims = XdmfSubsetGetNumberDimensions(subset);

  printf("%d ?= %d\n", internalNumDims, 1);

  assert(internalNumDims == 1);

  unsigned int * internalDims = XdmfSubsetGetDimensions(subset);

  printf("%d ?= %d\n", internalDims[0], 5);

  assert(internalDims[0] == 5);

  free(internalDims);

  unsigned int * internalStarts = XdmfSubsetGetStart(subset);

  printf("%d ?= %d\n", internalStarts[0], 0);

  assert(internalStarts[0] == 0);

  free(internalStarts);

  unsigned int * internalStrides = XdmfSubsetGetStride(subset);

  printf("%d ?= %d\n", internalStrides[0], 2);

  assert(internalStrides[0] == 2);

  free(internalStrides);

  void * readArray = XdmfSubsetRead(subset, &status);

  valueString = XdmfArrayGetValuesString(readArray);

  printf("%s ?= %s\n", "0 2 4 6 8", valueString);

  assert(strcmp("0 2 4 6 8", valueString) == 0);

  free(valueString);

  free(readArray);

  unsigned int internalSize = XdmfSubsetGetSize(subset);

  printf("%d ?= %d\n", internalSize, 5);

  assert(internalSize == 5);

  strides[0] = 1;

  starts[0] = 3;

  dimensions[0] = 4;

  XdmfSubsetSetDimensions(subset, dimensions, 1, &status);

  XdmfSubsetSetStart(subset, starts, 1, &status);

  XdmfSubsetSetStride(subset, strides, 1, &status);

  internalNumDims = XdmfSubsetGetNumberDimensions(subset);

  printf("%d ?= %d\n", internalNumDims, 1);

  assert(internalNumDims == 1);

  internalDims = XdmfSubsetGetDimensions(subset);

  printf("%d ?= %d\n", internalDims[0], 4);

  assert(internalDims[0] == 4);

  internalStarts = XdmfSubsetGetStart(subset);

  printf("%d ?= %d\n", internalStarts[0], 3);

  assert(internalStarts[0] == 3);

  internalStrides = XdmfSubsetGetStride(subset);

  printf("%d ?= %d\n", internalStrides[0], 1);

  assert(internalStrides[0] == 1);

  readArray = XdmfSubsetRead(subset, &status);

  valueString = XdmfArrayGetValuesString(readArray);

  printf("%s ?= %s\n", "3 4 5 6", valueString);

  assert(strcmp("3 4 5 6", valueString) == 0);

  free(valueString);

  valueString = XdmfSubsetGetConstructedType(subset);

  printf("%s ?= %s\n", "", valueString);

  assert(strcmp("", valueString) == 0);

  free(valueString);

  valueString = XdmfSubsetGetItemTag(subset);

  printf("%s ?= %s\n", "Subset", valueString);

  assert(strcmp("Subset", valueString) == 0);

  free(valueString);

  XdmfSubsetSetConstructedType(subset, "DataItem");

  XdmfSubsetSetConstructedProperties(subset, readArray);

  XDMFWRITER * writer = XdmfWriterNew("subsetfile.xmf");

  XdmfSubsetAccept(subset, (XDMFVISITOR *)writer, &status);

  unsigned int numInfo = XdmfSubsetGetNumberInformations(subset);

  printf("%d ?= %d\n", numInfo, 0);

  assert(numInfo == 0);

  return 0;
}
