#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"

#include "assert.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int main()
{
  int status = 0;

  printf("%d ?= %d\n", XDMF_ARRAY_TYPE_INT8, XdmfArrayTypeInt8());
  printf("%d ?= %d\n", XDMF_ARRAY_TYPE_INT16, XdmfArrayTypeInt16());
  printf("%d ?= %d\n", XDMF_ARRAY_TYPE_INT32, XdmfArrayTypeInt32());
  printf("%d ?= %d\n", XDMF_ARRAY_TYPE_INT64, XdmfArrayTypeInt64());
  printf("%d ?= %d\n", XDMF_ARRAY_TYPE_UINT8, XdmfArrayTypeUInt8());
  printf("%d ?= %d\n", XDMF_ARRAY_TYPE_UINT16, XdmfArrayTypeUInt16());
  printf("%d ?= %d\n", XDMF_ARRAY_TYPE_UINT32, XdmfArrayTypeUInt32());
  printf("%d ?= %d\n", XDMF_ARRAY_TYPE_FLOAT32, XdmfArrayTypeFloat32());
  printf("%d ?= %d\n", XDMF_ARRAY_TYPE_FLOAT64, XdmfArrayTypeFloat64());


  assert(XDMF_ARRAY_TYPE_INT8 == XdmfArrayTypeInt8());
  assert(XDMF_ARRAY_TYPE_INT16 == XdmfArrayTypeInt16());
  assert(XDMF_ARRAY_TYPE_INT32 == XdmfArrayTypeInt32());
  assert(XDMF_ARRAY_TYPE_INT64 == XdmfArrayTypeInt64());
  assert(XDMF_ARRAY_TYPE_UINT8 == XdmfArrayTypeUInt8());
  assert(XDMF_ARRAY_TYPE_UINT16 == XdmfArrayTypeUInt16());
  assert(XDMF_ARRAY_TYPE_UINT32 == XdmfArrayTypeUInt32());
  assert(XDMF_ARRAY_TYPE_FLOAT32 == XdmfArrayTypeFloat32());
  assert(XDMF_ARRAY_TYPE_FLOAT64 == XdmfArrayTypeFloat64());

  printf("%s ?= %s\n", "Char", XdmfArrayTypeGetName(XDMF_ARRAY_TYPE_INT8, &status));
  printf("%s ?= %s\n", "Short", XdmfArrayTypeGetName(XDMF_ARRAY_TYPE_INT16, &status));
  printf("%s ?= %s\n", "Int", XdmfArrayTypeGetName(XDMF_ARRAY_TYPE_INT32, &status));
  printf("%s ?= %s\n", "Int", XdmfArrayTypeGetName(XDMF_ARRAY_TYPE_INT64, &status));
  printf("%s ?= %s\n", "UChar", XdmfArrayTypeGetName(XDMF_ARRAY_TYPE_UINT8, &status));
  printf("%s ?= %s\n", "UShort", XdmfArrayTypeGetName(XDMF_ARRAY_TYPE_UINT16, &status));
  printf("%s ?= %s\n", "UInt", XdmfArrayTypeGetName(XDMF_ARRAY_TYPE_UINT32, &status));
  printf("%s ?= %s\n", "Float", XdmfArrayTypeGetName(XDMF_ARRAY_TYPE_FLOAT32, &status));
  printf("%s ?= %s\n", "Float", XdmfArrayTypeGetName(XDMF_ARRAY_TYPE_FLOAT64, &status));

  assert(strcmp("Char", XdmfArrayTypeGetName(XDMF_ARRAY_TYPE_INT8, &status)) == 0);
  assert(strcmp("Short", XdmfArrayTypeGetName(XDMF_ARRAY_TYPE_INT16, &status)) == 0);
  assert(strcmp("Int", XdmfArrayTypeGetName(XDMF_ARRAY_TYPE_INT32, &status)) == 0);
  assert(strcmp("Int", XdmfArrayTypeGetName(XDMF_ARRAY_TYPE_INT64, &status)) == 0);
  assert(strcmp("UChar", XdmfArrayTypeGetName(XDMF_ARRAY_TYPE_UINT8, &status)) == 0);
  assert(strcmp("UShort", XdmfArrayTypeGetName(XDMF_ARRAY_TYPE_UINT16, &status)) == 0);
  assert(strcmp("UInt", XdmfArrayTypeGetName(XDMF_ARRAY_TYPE_UINT32, &status)) == 0);
  assert(strcmp("Float", XdmfArrayTypeGetName(XDMF_ARRAY_TYPE_FLOAT32, &status)) == 0);
  assert(strcmp("Float", XdmfArrayTypeGetName(XDMF_ARRAY_TYPE_FLOAT64, &status)) == 0);

  printf("%d ?= %d\n", 1, XdmfArrayTypeGetElementSize(XDMF_ARRAY_TYPE_INT8, &status));
  printf("%d ?= %d\n", 2, XdmfArrayTypeGetElementSize(XDMF_ARRAY_TYPE_INT16, &status));
  printf("%d ?= %d\n", 4, XdmfArrayTypeGetElementSize(XDMF_ARRAY_TYPE_INT32, &status));
  printf("%d ?= %d\n", 8, XdmfArrayTypeGetElementSize(XDMF_ARRAY_TYPE_INT64, &status));
  printf("%d ?= %d\n", 1, XdmfArrayTypeGetElementSize(XDMF_ARRAY_TYPE_UINT8, &status));
  printf("%d ?= %d\n", 2, XdmfArrayTypeGetElementSize(XDMF_ARRAY_TYPE_UINT16, &status));
  printf("%d ?= %d\n", 4, XdmfArrayTypeGetElementSize(XDMF_ARRAY_TYPE_UINT32, &status));
  printf("%d ?= %d\n", 4, XdmfArrayTypeGetElementSize(XDMF_ARRAY_TYPE_FLOAT32, &status));
  printf("%d ?= %d\n", 8, XdmfArrayTypeGetElementSize(XDMF_ARRAY_TYPE_FLOAT64, &status));

  assert(1 == XdmfArrayTypeGetElementSize(XDMF_ARRAY_TYPE_INT8, &status));
  assert(2 == XdmfArrayTypeGetElementSize(XDMF_ARRAY_TYPE_INT16, &status));
  assert(4 == XdmfArrayTypeGetElementSize(XDMF_ARRAY_TYPE_INT32, &status));
  assert(8 == XdmfArrayTypeGetElementSize(XDMF_ARRAY_TYPE_INT64, &status));
  assert(1 == XdmfArrayTypeGetElementSize(XDMF_ARRAY_TYPE_UINT8, &status));
  assert(2 == XdmfArrayTypeGetElementSize(XDMF_ARRAY_TYPE_UINT16, &status));
  assert(4 == XdmfArrayTypeGetElementSize(XDMF_ARRAY_TYPE_UINT32, &status));
  assert(4 == XdmfArrayTypeGetElementSize(XDMF_ARRAY_TYPE_FLOAT32, &status));
  assert(8 == XdmfArrayTypeGetElementSize(XDMF_ARRAY_TYPE_FLOAT64, &status));

  int typeCollection[9];

  typeCollection[0] = XDMF_ARRAY_TYPE_INT8;
  typeCollection[1] = XDMF_ARRAY_TYPE_INT16;
  typeCollection[2] = XDMF_ARRAY_TYPE_INT32;
  typeCollection[3] = XDMF_ARRAY_TYPE_INT64;
  typeCollection[4] = XDMF_ARRAY_TYPE_UINT8;
  typeCollection[5] = XDMF_ARRAY_TYPE_UINT16;
  typeCollection[6] = XDMF_ARRAY_TYPE_UINT32;
  typeCollection[7] = XDMF_ARRAY_TYPE_FLOAT32;
  typeCollection[8] = XDMF_ARRAY_TYPE_FLOAT64;

  int dims[1];
  dims[0] = 0;
  double testValue = -1.25;

  int mergedims[1];
  mergedims[0] = 1;

  int mergeStarts[1];
  mergeStarts[0] = 0;
  int mergeCounts[1];
  mergeCounts[0] = 1;
  int mergeStrides[1];
  mergeStrides[0] = 1;

  int secondMergeStarts[1];
  secondMergeStarts[0] = 1;

  int valType1;
  int valType2;

  int valIntersectType;

  unsigned int i = 0;
  unsigned int j = 0;
  char mergedoriginal[50];

  void * valTypeArray1;
  void * valTypeArray2;
  void * valIntersectArray;

  for (i = 0; i < 9; ++i) {
    for (j = 0; j < 9; ++j) {
      valTypeArray1 = XdmfArrayNew();
      valTypeArray2 = XdmfArrayNew();

      valType1 = typeCollection[j];
      valType2 = typeCollection[i];

      XdmfArrayInitialize(valTypeArray1, dims, 1, valType1, &status);
      XdmfArrayInitialize(valTypeArray2, dims, 1, valType2, &status);

      XdmfArrayPushBack(valTypeArray1, &testValue, XDMF_ARRAY_TYPE_FLOAT64, &status);
      XdmfArrayPushBack(valTypeArray2, &testValue, XDMF_ARRAY_TYPE_FLOAT64, &status);

      valIntersectArray = XdmfArrayNew();
      valIntersectType = XdmfArrayTypeComparePrecision(valType1, valType2, &status);
      XdmfArrayInitialize(valIntersectArray, mergedims, 1, valIntersectType, &status);

      XdmfArrayInsertDataFromXdmfArray(valIntersectArray, valTypeArray1, mergeStarts, mergeStarts, mergeCounts, mergeCounts, mergeStrides, mergeStrides, &status);
      XdmfArrayInsertDataFromXdmfArray(valIntersectArray, valTypeArray2, secondMergeStarts, mergeStarts, mergeCounts, mergeCounts, mergeStrides, mergeStrides, &status);

      char * array1output = XdmfArrayGetValuesString(valTypeArray1);
      char * array2output = XdmfArrayGetValuesString(valTypeArray2);
      char * resultarrayoutput = XdmfArrayGetValuesString(valIntersectArray);

      sprintf(mergedoriginal, "%s %s", array1output, array2output);
      printf("%s\n%s\n", mergedoriginal, resultarrayoutput);

      assert(strcmp(mergedoriginal, resultarrayoutput) == 0);

      XdmfArrayRelease(valTypeArray1);
      XdmfArrayRelease(valTypeArray2);
      XdmfArrayRelease(valIntersectArray);

      XdmfArrayFree(valTypeArray1);
      XdmfArrayFree(valTypeArray2);
      XdmfArrayFree(valIntersectArray);

      free(array1output);
      free(array2output);
      free(resultarrayoutput);
    }
  }

  return 0;
}
