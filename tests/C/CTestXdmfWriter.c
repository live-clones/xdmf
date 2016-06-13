#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfWriter.hpp"
#include "XdmfHDF5ControllerDSM.hpp"
#include "XdmfHDF5Writer.hpp"
#include "XdmfReader.hpp"
#include "XdmfSubset.hpp"
#include "XdmfFunction.hpp"

#include "stdio.h"
#include "stdlib.h"

#include "string.h"
#include "assert.h"

int main()
{

  void * array = XdmfArrayNew();

  int i = 0;

  int status = 0;

  for (i = 0; i < 10; i++) {
    XdmfArrayPushBack(array, &i, XDMF_ARRAY_TYPE_INT32, &status);
  }

  XDMFWRITER * writer = XdmfWriterNew("testfile.xmf");

  char * testFileName = XdmfWriterGetFilePath(writer, &status);
  int testLimit = XdmfWriterGetLightDataLimit(writer, &status);

  printf("light data limit = %d\n", testLimit);

  assert(testLimit == 100);

  XdmfWriterSetLightDataLimit(writer, 100, &status);

  int writerMode = XdmfWriterGetMode(writer, &status);

  printf("%d ?= %d\n", writerMode, XDMF_WRITER_MODE_DEFAULT);

  assert(writerMode == XDMF_WRITER_MODE_DEFAULT);

  XdmfWriterSetMode(writer, XDMF_WRITER_MODE_DEFAULT, &status);

  int writerXPaths = XdmfWriterGetWriteXPaths(writer, &status);

  if (writerXPaths)
  {
    printf("will write XPaths\n");
  }
  else
  {
    printf("will not write XPaths\n");
  }

  assert(writerXPaths);

  XdmfWriterSetWriteXPaths(writer, 1, &status);

  int writerXPathParse = XdmfWriterGetXPathParse(writer, &status);

  if (writerXPathParse)
  {
    printf("will parse XPaths\n");
  }
  else
  {
    printf("will not parse XPaths\n");
  }

  assert(writerXPathParse);

  XdmfWriterSetXPathParse(writer, 1, &status);

  void * heavyWriter = XdmfHDF5WriterNew("secondtestfile.h5", 0);

  unsigned int chunkSize = XdmfHDF5WriterGetChunkSize(heavyWriter, &status);

  XdmfHDF5WriterSetChunkSize(heavyWriter, chunkSize, &status);

  int testmode = XdmfHeavyDataWriterGetMode(heavyWriter);

  printf("%d ?= %d\n", testmode, XDMF_HEAVY_WRITER_MODE_DEFAULT);

  assert(testmode == XDMF_HEAVY_WRITER_MODE_DEFAULT);

  XdmfHeavyDataWriterSetMode(heavyWriter, XDMF_HEAVY_WRITER_MODE_DEFAULT, &status);

  int currentsplitting = XdmfHeavyDataWriterGetAllowSetSplitting(heavyWriter);

  if (writerXPathParse)
  {
    printf("will allow dataset splitting\n");
  }
  else
  {
    printf("will not allow dataset splitting\n");
  }

  assert(!currentsplitting);

  XdmfHeavyDataWriterSetAllowSetSplitting(heavyWriter, currentsplitting);

  int currentindex = XdmfHeavyDataWriterGetFileIndex(heavyWriter);

  XdmfHeavyDataWriterSetFileIndex(heavyWriter, currentindex);

  unsigned int hdf5overhead = XdmfHeavyDataWriterGetFileOverhead(heavyWriter);

  printf("%d ?= %d\n", hdf5overhead, 800);

  assert(hdf5overhead == 800);

  char * testHeavyName =  XdmfHeavyDataWriterGetFilePath(heavyWriter);

  int currentLimit = XdmfHeavyDataWriterGetFileSizeLimit(heavyWriter);

  XdmfHeavyDataWriterSetFileSizeLimit(heavyWriter, currentLimit);

  int currentReleaseStatus = XdmfHeavyDataWriterGetReleaseData(heavyWriter);

  XdmfHeavyDataWriterSetReleaseData(heavyWriter, currentReleaseStatus);

  XdmfHDF5WriterOpenFile(heavyWriter, &status);

  // Write HDF5 to file

  XdmfArrayAccept(array, (XDMFVISITOR *)heavyWriter, &status);

  XdmfHDF5WriterCloseFile(heavyWriter, &status);

  XDMFWRITER * writerWithHeavy = XdmfWriterNewSpecifyHeavyDataWriter("secondtestfile.xmf", heavyWriter);

  void * heldWriter = XdmfWriterGetHeavyDataWriter(writer, &status);

  XdmfWriterSetHeavyDataWriter(writer, heldWriter, 0, &status);

  // Write to File

  XdmfArrayAccept(array, (XDMFVISITOR *)writerWithHeavy, &status);

  // Read from File

  void * reader = XdmfReaderNew();

  void * readArray = XdmfReaderRead(reader, "secondtestfile.xmf", &status);

  XdmfArrayRead(readArray, &status);

  char * valueString = XdmfArrayGetValuesString(readArray);

  printf("array contains: %s\n", valueString);

  assert(strcmp("0 1 2 3 4 5 6 7 8 9", valueString) == 0);

  XdmfArrayFree(readArray);

/*
XDMFCORE_EXPORT void * XdmfWriterGetHeavyDataWriter(void * writer);

XDMFCORE_EXPORT void XdmfWriterSetHeavyDataWriter(void * writer, void * heavyDataWriter, int transferOwnership);
*/

  unsigned int starts[1] = {0};
  unsigned int strides[1] = {2};
  unsigned int dimensions[1] = {5};

  unsigned int numDims = 1;

  void * subset = XdmfSubsetNew(array, starts, strides, dimensions, numDims, 0, &status);

  readArray = XdmfSubsetRead(subset, &status);

  valueString = XdmfArrayGetValuesString(readArray);

  printf("%s ?= %s\n", "0 2 4 6 8", valueString);

  assert(strcmp("0 2 4 6 8", valueString) == 0);

  free(valueString);

  free(writer);

  XdmfSubsetSetConstructedType(subset, "DataItem");

  XdmfSubsetSetConstructedProperties(subset, readArray);

  free(readArray);

  writer = XdmfWriterNew("subsetfile.xmf");

  XdmfSubsetAccept(subset, (XDMFVISITOR *)writer, &status);

  readArray = XdmfReaderRead(reader, "subsetfile.xmf", &status);

  void * readSubset = XdmfArrayGetReference(readArray);

  void * internalArray = XdmfSubsetGetReferenceArray(readSubset);

  XdmfArrayRead(internalArray, &status);

  valueString = XdmfArrayGetValuesString(internalArray);

  free(valueString);

  XdmfArrayRead(readArray, &status);

  valueString = XdmfArrayGetValuesString(readArray);

  printf("%s ?= %s\n", "0 2 4 6 8", valueString);

  assert(strcmp("0 2 4 6 8", valueString) == 0);

  free(valueString);

  free(readArray);

  void * firstarray = XdmfArrayNew();

  for (i = 0; i < 10; i++) {
    XdmfArrayPushBack(firstarray, &i, XDMF_ARRAY_TYPE_INT32, &status);
  }

  valueString = XdmfArrayGetValuesString(firstarray);

  printf("%s ?= %s\n", "0 1 2 3 4 5 6 7 8 9", valueString);

  assert(strcmp("0 1 2 3 4 5 6 7 8 9", valueString) == 0);

  free(valueString);

  void * secondarray = XdmfArrayNew();

  int val;

  for (i = 1; i < 11; i++) {
    val = i * 10;
    XdmfArrayPushBack(secondarray, &val, XDMF_ARRAY_TYPE_INT32, &status);
  }

  valueString = XdmfArrayGetValuesString(secondarray);

  printf("%s ?= %s\n", "10 20 30 40 50 60 70 80 90 100", valueString);

  assert(strcmp("10 20 30 40 50 60 70 80 90 100", valueString) == 0);

  free(valueString);

  void * function = XdmfFunctionNew();

  free(function);

  char * keys[2];

  keys[0] = "A";
  keys[1] = "B";

  char * expression = "A#B";

  XDMFARRAY * values[2];

  values[0] = firstarray;
  values[1] = secondarray;

  function = XdmfFunctionNewInit(expression, keys, values, 2);

  readArray = XdmfFunctionRead(function, &status);

  valueString = XdmfArrayGetValuesString(readArray);

  printf("%s ?= %s\n", "0 10 1 20 2 30 3 40 4 50 5 60 6 70 7 80 8 90 9 100", valueString);

  assert(strcmp("0 10 1 20 2 30 3 40 4 50 5 60 6 70 7 80 8 90 9 100", valueString) == 0);

  free(valueString);

  free(writer);

  XdmfFunctionSetConstructedType(function, "DataItem");

  XdmfFunctionSetConstructedProperties(function, readArray);

  writer = XdmfWriterNew("functionfile.xmf");

  XdmfFunctionAccept(function, (XDMFVISITOR *)writer, &status);

  readArray = XdmfReaderRead(reader, "functionfile.xmf", &status);

  XdmfArrayRead(readArray, &status);

  valueString = XdmfArrayGetValuesString(readArray);

  printf("%s ?= %s\n", "0 10 1 20 2 30 3 40 4 50 5 60 6 70 7 80 8 90 9 100", valueString);

  assert(strcmp("0 10 1 20 2 30 3 40 4 50 5 60 6 70 7 80 8 90 9 100", valueString) == 0);

  free(valueString);

  return 0;
}
