#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfBinaryController.hpp"
#include "XdmfReader.hpp"
#include "XdmfWriter.hpp"

#include "assert.h"
#include "stdio.h"
#include "stdlib.h"


int main()
{
  FILE * binFile = fopen("binary.bin", "w+");

  int status = 0;

  //
  // write binary file
  //

  int outputData[4];
  outputData[0] = 1;
  outputData[1] = 0;
  outputData[2] = -1;
  outputData[3] = 100;

  // Dummy data to test seek

  int dummyData[3];
  dummyData[0] = 5;
  dummyData[1] = -5;
  dummyData[2] = 12;

  fwrite(dummyData, sizeof(int), 3, binFile);
  fwrite(outputData, sizeof(int), 4, binFile);

//  fprintf(binFile, "%s\n", (char *)dummyData);
//  fprintf(binFile, "%s\n", (char *)outputData);

  fclose(binFile);

  //
  // read binary file using XdmfBinaryController
  // Checking use of seek
  //
  unsigned int dims[1] = {4};
  XDMFBINARYCONTROLLER * binaryController =
    XdmfBinaryControllerNew("binary.bin",
                            XDMF_ARRAY_TYPE_INT32,
                            XDMF_BINARY_CONTROLLER_ENDIAN_NATIVE,
                            sizeof(int) * 3,
                            dims,
                            1,
                            &status);


  XDMFARRAY * testArray = XdmfArrayNew();
  XdmfArrayInsertHeavyDataController(testArray, (XDMFHEAVYDATACONTROLLER *)binaryController, 1);
  XdmfArrayRead(testArray, &status);

  XdmfBinaryControllerFree(binaryController);

  int * testArrayVal0 = 
    (int *)XdmfArrayGetValue(testArray, 0, XDMF_ARRAY_TYPE_INT32, &status);
  int * testArrayVal1 = 
    (int *)XdmfArrayGetValue(testArray, 1, XDMF_ARRAY_TYPE_INT32, &status);
  int * testArrayVal2 = 
    (int *)XdmfArrayGetValue(testArray, 2, XDMF_ARRAY_TYPE_INT32, &status);
  int * testArrayVal3 = 
    (int *)XdmfArrayGetValue(testArray, 3, XDMF_ARRAY_TYPE_INT32, &status);

  printf("%u ?= %u\n", XdmfArrayGetSize(testArray), 4);
  printf("%d ?= %d\n", *testArrayVal0, outputData[0]);
  printf("%d ?= %d\n", *testArrayVal1, outputData[1]);
  printf("%d ?= %d\n", *testArrayVal2, outputData[2]);
  printf("%d ?= %d\n", *testArrayVal3, outputData[3]);

  assert(XdmfArrayGetSize(testArray)== 4);
  assert(*testArrayVal0 == outputData[0]);
  assert(*testArrayVal1 == outputData[1]);
  assert(*testArrayVal2 == outputData[2]);
  assert(*testArrayVal3 == outputData[3]);

  free(testArrayVal0);
  free(testArrayVal1);
  free(testArrayVal2);
  free(testArrayVal3);

  //
  // output array to disk
  //
  XDMFWRITER * writer = XdmfWriterNew("TestXdmfBinary.xmf");
  XdmfWriterSetMode(writer, XDMF_WRITER_MODE_DISTRIBUTED_HEAVY_DATA, &status);
  XdmfArrayAccept(testArray, (XDMFVISITOR *)writer, &status);
  XdmfWriterFree(writer);

  XdmfArrayFree(testArray);

  //
  // read array in
  //
  XDMFREADER * reader = XdmfReaderNew();
  XDMFITEM * item = XdmfReaderRead(reader, "TestXdmfBinary.xmf", &status);
  XdmfItemFree(item);
  XdmfReaderFree(reader);

  return 0;
}
