#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfHDF5Controller.hpp"
#include "XdmfHDF5Writer.hpp"
#include "XdmfWriter.hpp"

#include "assert.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"

int main()
{
  void * array = XdmfArrayNew();

  int status = 0;

  int i = 0;

  for (i = 0; i < 5; i++) {
    XdmfArrayPushBack(array, &i, XDMF_ARRAY_TYPE_INT32, &status);
  }

  char * valueString = XdmfArrayGetValuesString(array);

  printf("%s ?= %s\n", "0 1 2 3 4", valueString),

  assert(strcmp("0 1 2 3 4", valueString) == 0);

  free(valueString);

  unsigned int starts[1] = {0};
  unsigned int strides[1] = {2};
  unsigned int dimensions[1] = {5};
  unsigned int dataspace[1] = {10};

  unsigned int numDims = 1;

  XDMFHDF5CONTROLLER * controller = XdmfHDF5ControllerNew("hdf5test.h5", "Data", XDMF_ARRAY_TYPE_INT32, starts, strides, dimensions, dataspace, numDims, &status);

  valueString = XdmfHDF5ControllerGetFilePath(controller);

  printf("%s ?= %s\n", "hdf5test.h5", valueString);

  assert(strcmp("hdf5test.h5", valueString) == 0);

  free(valueString);

  valueString = XdmfHDF5ControllerGetDataSetPath(controller);

  printf("dataset = %s\n", valueString);

  assert(strcmp("Data", valueString) == 0);

  free(valueString);

  valueString = XdmfHDF5ControllerGetName(controller);

  printf("%s ?= %s\n", "HDF", valueString);

  assert(strcmp("HDF", valueString) == 0);

  free(valueString);

  unsigned int controllerSize = XdmfHDF5ControllerGetSize(controller);

  printf("%d ?= %d\n", controllerSize, 5);

  assert(controllerSize == 5);

  unsigned int heldNumDims = XdmfHDF5ControllerGetNumberDimensions(controller);

  printf("%d ?= %d\n", heldNumDims, 1);

  assert(heldNumDims == 1);

  int heldType = XdmfHDF5ControllerGetType(controller, &status);

  printf("%d ?= %d\n", heldType, XDMF_ARRAY_TYPE_INT32);

  assert(heldType == XDMF_ARRAY_TYPE_INT32);

  unsigned int * internalStart = XdmfHDF5ControllerGetStart(controller);

  printf("%d ?= %d\n", internalStart[0], 0);

  assert(internalStart[0] == 0);

  free(internalStart);

  unsigned int * internalStride = XdmfHDF5ControllerGetStride(controller);

  printf("%d ?= %d\n", internalStride[0], 2);

  assert(internalStride[0] == 2);

  free(internalStride);

  unsigned int * internalDim = XdmfHDF5ControllerGetDimensions(controller);

  printf("%d ?= %d\n", internalDim[0], 5);

  assert(internalDim[0] == 5);

  free(internalDim);

  unsigned int * internalDataspace = XdmfHDF5ControllerGetDataspaceDimensions(controller);

  printf("%d ?= %d\n", internalDataspace[0], 10);

  assert(internalDataspace[0] == 10);

  free(internalDataspace);

  XdmfArrayInsertHeavyDataController(array, (XDMFHEAVYDATACONTROLLER *)controller, 0);

  XDMFHDF5WRITER * heavyWriter = XdmfHDF5WriterNew("testhdf5.h5", 0);

  XdmfHDF5WriterSetMode(heavyWriter, XDMF_HEAVY_WRITER_MODE_HYPERSLAB, &status);

  XdmfArrayAccept(array, (XDMFVISITOR *)heavyWriter, &status);

  void * secondarray = XdmfArrayNew();

  for (i = 10; i < 15; i++) {
    XdmfArrayPushBack(secondarray, &i, XDMF_ARRAY_TYPE_INT32, &status);
  }

  valueString = XdmfArrayGetValuesString(secondarray);

  printf("%s ?= %s\n", "10 11 12 13 14", valueString);

  assert(strcmp("10 11 12 13 14", valueString) == 0);

  free(valueString);

  unsigned int secondstarts[1] = {1};
  unsigned int secondstrides[1] = {2};
  unsigned int seconddimensions[1] = {5};
  unsigned int seconddataspace[1] = {10};

  XDMFHDF5CONTROLLER * secondcontroller = XdmfHDF5ControllerNew("hdf5test.h5", "Data", XDMF_ARRAY_TYPE_INT32, secondstarts, secondstrides, seconddimensions, seconddataspace, numDims, &status);

  XdmfArrayInsertHeavyDataController(secondarray, (XDMFHEAVYDATACONTROLLER *)secondcontroller, 0);

  XdmfArrayAccept(secondarray, (XDMFVISITOR *)heavyWriter, &status);

  void * readarray = XdmfArrayNew();

  unsigned int readstarts[1] = {0};
  unsigned int readstrides[1] = {1};
  unsigned int readdimensions[1] = {10};
  unsigned int readdataspace[1] = {10};

  XDMFHDF5CONTROLLER * readcontroller = XdmfHDF5ControllerNew("hdf5test.h5", "Data", XDMF_ARRAY_TYPE_INT32, readstarts, readstrides, readdimensions, readdataspace, numDims, &status);

  int testType = XdmfHDF5ControllerGetType(readcontroller, &status);

  printf("%d ?= %d\n", testType, XDMF_ARRAY_TYPE_INT32);

  assert(testType == XDMF_ARRAY_TYPE_INT32);

  XdmfHDF5ControllerRead(readcontroller, readarray, &status);

  valueString = XdmfArrayGetValuesString(readarray);

  printf("Read Values = %s\n", valueString);

  assert(strcmp("0 10 1 11 2 12 3 13 4 14", valueString) == 0);

  free(valueString);

  XdmfHDF5ControllerSetArrayOffset(readcontroller, 5);

  unsigned int internalOffset = XdmfHDF5ControllerGetArrayOffset(readcontroller);

  printf("%d ?= %d\n", internalOffset, 5);

  assert(internalOffset == 5);

  XdmfArrayInsertHeavyDataController(readarray, (XDMFHEAVYDATACONTROLLER *)readcontroller, 0);

  XdmfArrayRead(readarray, &status);

  valueString = XdmfArrayGetValuesString(readarray);

  printf("%s ?= %s\n", "0 0 0 0 0 0 10 1 11 2 12 3 13 4 14", valueString);

  assert(strcmp("0 0 0 0 0 0 10 1 11 2 12 3 13 4 14", valueString) == 0);

  free(valueString);

  return 0;
}
