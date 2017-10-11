#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfInformation.hpp"

#include "assert.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"

int main()
{
  int status = 0;

  void * array = XdmfArrayNew();

  XdmfArraySetName(array, "Test Array", &status);

  char * arrayName = XdmfArrayGetName(array);

  printf("%s ?= %s\n", "Test Array", arrayName);

  assert(strcmp("Test Array", arrayName) == 0);

  free(arrayName);

  if (!XdmfArrayIsInitialized(array))
  {
    printf("Array is not initialized\n");
  }
  else
  {
    printf("Array is initialized\n");
  }

  assert(!XdmfArrayIsInitialized(array));

  // Information coupling

  void * testInformation = XdmfInformationNew("Test Information","Testing");

  XdmfArrayInsertInformation(array, testInformation, 0);

  int numInfo = XdmfArrayGetNumberInformations(array);

  printf("%d ?= %d\n", numInfo, 1);

  assert(numInfo == 1);

  void * returnInformation = XdmfArrayGetInformation(array, 0);

  char * returnInfoName = XdmfInformationGetKey(returnInformation);

  char * returnInfoValue = XdmfInformationGetValue(returnInformation);

  printf("%s ?= %s\n", returnInfoName, "Test Information");
  printf("%s ?= %s\n", returnInfoValue, "Testing");

  assert(strcmp(returnInfoName, "Test Information") == 0);

  assert(strcmp(returnInfoValue, "Testing") == 0);

  XdmfArrayInsertInformation(array, XdmfInformationNew("Secondary Information", "Also Testing"), 0);

  numInfo = XdmfArrayGetNumberInformations(array);

  printf("%d ?= %d\n", numInfo, 2);

  assert(numInfo == 2);

  XdmfArrayRemoveInformation(array, 0);

  numInfo = XdmfArrayGetNumberInformations(array);

  printf("%d ?= %d\n", numInfo, 1);

  assert(numInfo == 1);

  void * secondReturnInformation = XdmfArrayGetInformation(array, 0);

  char * secondReturnInfoName = XdmfInformationGetKey(secondReturnInformation);

  char * secondReturnInfoValue = XdmfInformationGetValue(secondReturnInformation);

  printf("%s ?= %s\n", secondReturnInfoName, "Secondary Information");
  printf("%s ?= %s\n", secondReturnInfoValue, "AlsoTesting");

  assert(strcmp(secondReturnInfoName, "Secondary Information") == 0);
  assert(strcmp(secondReturnInfoValue, "Also Testing") == 0);

  XdmfArrayRemoveInformationByKey(array, "Secondary Information");

  numInfo = XdmfArrayGetNumberInformations(array);

  printf("%d ?= %d\n", numInfo, 0);

  assert(numInfo == 0);

  free(testInformation);
  free(returnInfoName);
  free(returnInfoValue);
  free(secondReturnInformation);
  free(secondReturnInfoName);
  free(secondReturnInfoValue);

  printf("%d ?= %d\n", XDMF_ARRAY_READ_MODE_CONTROLLER, XdmfArrayGetReadMode(array, &status));

  assert(XDMF_ARRAY_READ_MODE_CONTROLLER == XdmfArrayGetReadMode(array, &status));

  XdmfArraySetReadMode(array, XDMF_ARRAY_READ_MODE_REFERENCE, &status);

  printf("%d ?= %d\n", XDMF_ARRAY_READ_MODE_REFERENCE, XdmfArrayGetReadMode(array, &status));

  assert(XDMF_ARRAY_READ_MODE_REFERENCE == XdmfArrayGetReadMode(array, &status));

  XdmfArraySetReadMode(array, XDMF_ARRAY_READ_MODE_CONTROLLER, &status);

  int dims[2];
  dims[0] = 5;
  dims[1] = 4;

  XdmfArrayResize(array, dims, 2, XDMF_ARRAY_TYPE_INT32, &status);

  printf("%d ?= %d\n", XdmfArrayGetNumberDimensions(array), 2);
  printf("%d ?= %d\n", XdmfArrayGetSize(array), 20);

  assert(XdmfArrayGetNumberDimensions(array) == 2);
  assert(XdmfArrayGetSize(array) == 20);

  XdmfArrayReserve(array, 50);

  int capacity = XdmfArrayGetCapacity(array);

  printf("%d ?>= %d\n", capacity, 50);

  assert(capacity >= 50);

  unsigned int * returnDims = XdmfArrayGetDimensions(array);

  printf("%d ?= %d\n", returnDims[0], 5);
  printf("%d ?= %d\n", returnDims[1], 4);

  assert(returnDims[0] == 5);
  assert(returnDims[1] == 4);

  char * dimString = XdmfArrayGetDimensionsString(array);

  printf("%s ?= %s\n", "5 4", dimString);

  assert(strcmp("5 4", dimString) == 0);

  free(dimString);
  free(returnDims);

  printf("%d ?= %d\n", XDMF_ARRAY_TYPE_INT32, XdmfArrayGetArrayType(array, &status));

  assert(XDMF_ARRAY_TYPE_INT32 == XdmfArrayGetArrayType(array, &status));

  XdmfArrayClear(array);

  XdmfArrayRelease(array);

  int i = 0;

  for (i = 0; i < 10; i++) {
    XdmfArrayPushBack(array, &i, XDMF_ARRAY_TYPE_INT32, &status);
  }

  char * valueString = XdmfArrayGetValuesString(array);

  printf("%s ?= %s\n", "0 1 2 3 4 5 6 7 8 9", valueString);

  assert(strcmp("0 1 2 3 4 5 6 7 8 9", valueString) == 0);

  free(valueString);

  int * testVal = (int *)XdmfArrayGetValue(array, 3, XDMF_ARRAY_TYPE_INT32, &status);

  printf("%d ?= %d\n", *testVal, 3);

  assert(*testVal == 3);

  free(testVal);

  XdmfArrayErase(array, 3);

  valueString = XdmfArrayGetValuesString(array);

  printf("%s ?= %s\n", "0 1 2 4 5 6 7 8 9", valueString);

  assert(strcmp("0 1 2 4 5 6 7 8 9", valueString) == 0);

  free(valueString);

  int insertVal = 90;

  XdmfArrayInsertValue(array, 9, &insertVal, XDMF_ARRAY_TYPE_INT32, &status);

  valueString = XdmfArrayGetValuesString(array);

  printf("%s ?= %s\n", "0 1 2 3 4 5 6 7 8 9 90", valueString);

  assert(strcmp("0 1 2 4 5 6 7 8 9 90", valueString) == 0);

  free(valueString);

  void * secondaryArray = XdmfArrayNew();

  XdmfArraySetValuesInternal(secondaryArray, XdmfArrayGetValuesInternal(array), XdmfArrayGetSize(array), XdmfArrayGetArrayType(array, &status), 0, &status);

  valueString = XdmfArrayGetValuesString(secondaryArray);

  printf("%s ?= %s\n", "0 1 2 4 5 6 7 8 9 90", valueString);

  assert(strcmp("0 1 2 4 5 6 7 8 9 90", valueString) == 0);

  XdmfArrayRelease(secondaryArray);

  free(secondaryArray);
  free(valueString);

  valueString = XdmfArrayGetValuesString(array);

  printf("%s ?= %s\n", "0 1 2 4 5 6 7 8 9 90", valueString);

  assert(strcmp("0 1 2 4 5 6 7 8 9 90", valueString) == 0);

  free(valueString);

  secondaryArray = XdmfArrayNew();

  int valArray[3];
  valArray[0] = 5;
  valArray[1] = 6;
  valArray[2] = 7;

  int starts[1];
  starts[0] = 0;

  int counts[1];
  counts[0] = 10;

  int strides[1];
  strides[0] = 1;

  XdmfArrayInsertDataFromXdmfArray(secondaryArray, array, starts, starts, counts, counts, strides, strides, &status);

  valueString = XdmfArrayGetValuesString(secondaryArray);

  printf("%s ?= %s\n", "0 1 2 4 5 6 7 8 9 90", valueString);

  assert(strcmp("0 1 2 4 5 6 7 8 9 90", valueString) == 0);

  free(valueString);

  XdmfArrayInsertDataFromPointer(secondaryArray, valArray, XDMF_ARRAY_TYPE_INT32, 0, 3, 1, 1, &status);

  valueString = XdmfArrayGetValuesString(secondaryArray);

  printf("%s ?= %s\n", "5 6 7 4 5 6 7 8 9 90", valueString);

  assert(strcmp("5 6 7 4 5 6 7 8 9 90", valueString) == 0);

  free(valueString);

  XdmfArrayClear(secondaryArray);

  XdmfArrayInsertDataFromPointer(secondaryArray, valArray, XDMF_ARRAY_TYPE_INT32, 0, 3, 1, 1, &status);

  valueString = XdmfArrayGetValuesString(secondaryArray);

  printf("%s ?= %s\n", "5 6 7", valueString);

  assert(strcmp("5 6 7", valueString) == 0);

  free(valueString);

  valueString = XdmfArrayGetValuesString(array);

  printf("%s ?= %s\n", "0 1 2 4 5 6 7 8 9 90", valueString);

  assert(strcmp("0 1 2 4 5 6 7 8 9 90", valueString) == 0);

  free(valueString);

  XdmfArraySwapWithXdmfArray(array, secondaryArray);

  valueString = XdmfArrayGetValuesString(array);

  printf("%s ?= %s\n", "5 6 7", valueString);

  assert(strcmp("5 6 7", valueString) == 0);

  free(valueString);

  valueString = XdmfArrayGetValuesString(secondaryArray);

  printf("%s ?= %s\n", "0 1 2 4 5 6 7 8 9 90", valueString);

  assert(strcmp("0 1 2 4 5 6 7 8 9 90", valueString) == 0);

  free(valueString);

  int * swapArray = valArray;

  XdmfArraySwapWithArray(secondaryArray, ((void **)&(swapArray)), 3, XDMF_ARRAY_TYPE_INT32, &status);

  valueString = XdmfArrayGetValuesString(secondaryArray);

  printf("%s ?= %s\n", "5 6 7", valueString);

  assert(strcmp("5 6 7", valueString) == 0);

  for (i = 0; i < 10; ++i)
  {
    printf("swapArray[%d] = %d\n", i, swapArray[i]);
  }

  assert(swapArray[0] == 0);
  assert(swapArray[1] == 1);
  assert(swapArray[2] == 2);
  assert(swapArray[3] == 4);
  assert(swapArray[4] == 5);
  assert(swapArray[5] == 6);
  assert(swapArray[6] == 7);
  assert(swapArray[7] == 8);
  assert(swapArray[8] == 9);
  assert(swapArray[9] == 90);

  free(valueString);

  int *returnArray = (int *)XdmfArrayGetValues(secondaryArray, 0, XDMF_ARRAY_TYPE_INT32, 3, 1, 1, &status);

  for (i = 0; i < 3; ++i)
  {
    printf("returnArray[%d] = %d\n", i, returnArray[i]);
  }

  assert(returnArray[0] == 5);
  assert(returnArray[1] == 6);
  assert(returnArray[2] == 7);

  return 0;
}
