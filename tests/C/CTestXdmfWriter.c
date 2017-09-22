#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfWriter.hpp"
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

  XDMFARRAY * array = XdmfArrayNew();

  int i = 0;
  int status = 0;
  for (i = 0; i < 10; i++) {
    XdmfArrayPushBack(array, &i, XDMF_ARRAY_TYPE_INT32, &status);
  }

  XDMFWRITER * writer = XdmfWriterNew("testfile.xmf");

  char * testFileName = XdmfWriterGetFilePath(writer, &status);
  free(testFileName);

  int testLimit = XdmfWriterGetLightDataLimit(writer, &status);
  printf("light data limit = %d\n", testLimit);
  assert(testLimit == 100);
  XdmfWriterSetLightDataLimit(writer, 100, &status);

  int writerMode = XdmfWriterGetMode(writer, &status);
  printf("%d ?= %d\n", writerMode, XDMF_WRITER_MODE_DEFAULT);
  assert(writerMode == XDMF_WRITER_MODE_DEFAULT);
  XdmfWriterSetMode(writer, XDMF_WRITER_MODE_DEFAULT, &status);

  int writerXPaths = XdmfWriterGetWriteXPaths(writer, &status);
  if (writerXPaths) {
    printf("will write XPaths\n");
  }
  else {
    printf("will not write XPaths\n");
  }
  assert(writerXPaths);
  XdmfWriterSetWriteXPaths(writer, 1, &status);

  int writerXPathParse = XdmfWriterGetXPathParse(writer, &status);
  if (writerXPathParse) {
    printf("will parse XPaths\n");
  }
  else  {
    printf("will not parse XPaths\n");
  }
  assert(writerXPathParse);
  XdmfWriterSetXPathParse(writer, 1, &status);

  XDMFHDF5WRITER * heavyWriter = XdmfHDF5WriterNew("secondtestfile.h5", 0);
  unsigned int chunkSize = XdmfHDF5WriterGetChunkSize(heavyWriter, &status);
  XdmfHDF5WriterSetChunkSize(heavyWriter, chunkSize, &status);
  int testmode = XdmfHDF5WriterGetMode(heavyWriter);
  printf("%d ?= %d\n", testmode, XDMF_HEAVY_WRITER_MODE_DEFAULT);
  assert(testmode == XDMF_HEAVY_WRITER_MODE_DEFAULT);
  XdmfHDF5WriterSetMode(heavyWriter, XDMF_HEAVY_WRITER_MODE_DEFAULT, &status);

  int currentsplitting = XdmfHDF5WriterGetAllowSetSplitting(heavyWriter);
  if (currentsplitting) {
    printf("will allow dataset splitting\n");
  }
  else {
    printf("will not allow dataset splitting\n");
  }
  assert(!currentsplitting);

  XdmfHDF5WriterSetAllowSetSplitting(heavyWriter, currentsplitting);

  int currentindex = XdmfHDF5WriterGetFileIndex(heavyWriter);
  XdmfHDF5WriterSetFileIndex(heavyWriter, currentindex);
  unsigned int hdf5overhead = XdmfHDF5WriterGetFileOverhead(heavyWriter);
  printf("%d ?= %d\n", hdf5overhead, 800);
  assert(hdf5overhead == 800);

  char * testHeavyName =  XdmfHDF5WriterGetFilePath(heavyWriter);
  free(testHeavyName);

  int currentLimit = XdmfHDF5WriterGetFileSizeLimit(heavyWriter);
  XdmfHDF5WriterSetFileSizeLimit(heavyWriter, currentLimit);

  int currentReleaseStatus = XdmfHDF5WriterGetReleaseData(heavyWriter);
  XdmfHDF5WriterSetReleaseData(heavyWriter, currentReleaseStatus);

  XdmfHDF5WriterOpenFile(heavyWriter, &status);

  // Write HDF5 to file

  XdmfArrayAccept(array, (XDMFVISITOR *)heavyWriter, &status);

  XdmfHDF5WriterCloseFile(heavyWriter, &status);

  XDMFWRITER * writerWithHeavy = 
    XdmfWriterNewSpecifyHeavyDataWriter("secondtestfile.xmf", 
					(XDMFHEAVYDATAWRITER *) heavyWriter);

  XdmfHDF5WriterFree(heavyWriter);

  XDMFHEAVYDATAWRITER * heldWriter = 
    XdmfWriterGetHeavyDataWriter(writer, &status);
  XdmfWriterSetHeavyDataWriter(writer, heldWriter, 0, &status);
  XdmfHeavyDataWriterFree(heldWriter);

  // Write to File
  XdmfArrayAccept(array, (XDMFVISITOR *)writerWithHeavy, &status);

  XdmfWriterFree(writerWithHeavy);

  // Read from File

  XDMFREADER * reader = XdmfReaderNew();
  XDMFITEM * readItem = XdmfReaderRead(reader, "secondtestfile.xmf", &status);
  XDMFARRAY * readArray = XdmfArrayCast(readItem);
  XdmfItemFree(readItem);

  XdmfArrayRead(readArray, &status);

  char * valueString = XdmfArrayGetValuesString(readArray);
  printf("array contains: %s\n", valueString);
  assert(strcmp("0 1 2 3 4 5 6 7 8 9", valueString) == 0);
  free(valueString);

  XdmfArrayFree(readArray);

  unsigned int starts[1] = {0};
  unsigned int strides[1] = {2};
  unsigned int dimensions[1] = {5};

  unsigned int numDims = 1;

  XDMFSUBSET * subset = 
    XdmfSubsetNew(array, starts, strides, dimensions, numDims, 0, &status);

  readArray = XdmfSubsetRead(subset, &status);

  valueString = XdmfArrayGetValuesString(readArray);
  printf("%s ?= %s\n", "0 2 4 6 8", valueString);
  assert(strcmp("0 2 4 6 8", valueString) == 0);
  free(valueString);

  XdmfSubsetSetConstructedType(subset, "DataItem");
  XdmfSubsetSetConstructedProperties(subset, readArray);

  XdmfArrayFree(readArray);

  XdmfWriterFree(writer);
  writer = XdmfWriterNew("subsetfile.xmf");

  XdmfSubsetAccept(subset, (XDMFVISITOR *)writer, &status);

  readItem = XdmfReaderRead(reader, "subsetfile.xmf", &status);
  readArray = XdmfArrayCast(readItem);
  XdmfItemFree(readItem);

  XDMFARRAYREFERENCE * readSubset = XdmfArrayGetReference(readArray);
  XDMFARRAY * internalArray = 
    XdmfSubsetGetReferenceArray((XDMFSUBSET *) readSubset);

  XdmfSubsetFree(subset);
  XdmfArrayReferenceFree(readSubset);

  XdmfArrayRead(internalArray, &status);

  valueString = XdmfArrayGetValuesString(internalArray);
  free(valueString);
  XdmfArrayFree(internalArray);

  XdmfArrayRead(readArray, &status);

  valueString = XdmfArrayGetValuesString(readArray);
  printf("%s ?= %s\n", "0 2 4 6 8", valueString);
  assert(strcmp("0 2 4 6 8", valueString) == 0);
  free(valueString);

  XdmfArrayFree(readArray);

  XDMFARRAY * firstarray = XdmfArrayNew();
  for (i = 0; i < 10; i++) {
    XdmfArrayPushBack(firstarray, &i, XDMF_ARRAY_TYPE_INT32, &status);
  }
  valueString = XdmfArrayGetValuesString(firstarray);
  printf("%s ?= %s\n", "0 1 2 3 4 5 6 7 8 9", valueString);
  assert(strcmp("0 1 2 3 4 5 6 7 8 9", valueString) == 0);
  free(valueString);

  XDMFARRAY * secondarray = XdmfArrayNew();
  int val;
  for (i = 1; i < 11; i++) {
    val = i * 10;
    XdmfArrayPushBack(secondarray, &val, XDMF_ARRAY_TYPE_INT32, &status);
  }
  valueString = XdmfArrayGetValuesString(secondarray);
  printf("%s ?= %s\n", "10 20 30 40 50 60 70 80 90 100", valueString);
  assert(strcmp("10 20 30 40 50 60 70 80 90 100", valueString) == 0);
  free(valueString);

  XDMFFUNCTION * function = XdmfFunctionNew();
  XdmfFunctionFree(function);

  char * keys[2];
  keys[0] = "A";
  keys[1] = "B";
  char * expression = "A#B";
  XDMFARRAY * values[2];
  values[0] = firstarray;
  values[1] = secondarray;

  function = XdmfFunctionNewInit(expression, keys, values, 2);
  readArray = XdmfFunctionRead(function, &status);

  XdmfArrayFree(firstarray);
  XdmfArrayFree(secondarray);

  valueString = XdmfArrayGetValuesString(readArray);
  printf("%s ?= %s\n", 
	 "0 10 1 20 2 30 3 40 4 50 5 60 6 70 7 80 8 90 9 100", 
	 valueString);
  assert(strcmp("0 10 1 20 2 30 3 40 4 50 5 60 6 70 7 80 8 90 9 100", 
		valueString) == 0);
  free(valueString);

  XdmfFunctionSetConstructedType(function, "DataItem");
  XdmfFunctionSetConstructedProperties(function, readArray);

  XdmfWriterFree(writer);
  writer = XdmfWriterNew("functionfile.xmf");

  XdmfFunctionAccept(function, (XDMFVISITOR *)writer, &status);

  XdmfFunctionFree(function);
  XdmfWriterFree(writer);
  XdmfArrayFree(readArray);

  readItem = XdmfReaderRead(reader, "functionfile.xmf", &status);
  readArray = XdmfArrayCast(readItem);
  XdmfItemFree(readItem);

  XdmfArrayRead(readArray, &status);
  valueString = XdmfArrayGetValuesString(readArray);
  printf("%s ?= %s\n", 
	 "0 10 1 20 2 30 3 40 4 50 5 60 6 70 7 80 8 90 9 100", 
	 valueString);
  assert(strcmp("0 10 1 20 2 30 3 40 4 50 5 60 6 70 7 80 8 90 9 100", 
		valueString) == 0);
  free(valueString);

  XdmfReaderFree(reader);

  XdmfArrayFree(readArray);
  XdmfArrayFree(array);

  return 0;
}
