#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfAttribute.hpp"
#include "XdmfAttributeType.hpp"
#include "XdmfAttributeCenter.hpp"
#include "XdmfGraph.hpp"
#include "XdmfSparseMatrix.hpp"
#include "XdmfReader.hpp"
#include "XdmfWriter.hpp"

#include "assert.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"

int main()
{
  int status = 0;

  XDMFGRAPH * matrix = XdmfGraphNew(3);

  XdmfGraphSetName(matrix, "testMatrix", &status);

  char * valueString = XdmfSparseMatrixGetName((XDMFSPARSEMATRIX *)((void *)matrix));

  printf("%s ?= %s\n", valueString, "testMatrix");

  assert(strcmp(valueString, "testMatrix") == 0);

  free(valueString);

  valueString = XdmfGraphGetItemTag(matrix);

  printf("%s ?= %s\n", valueString, "Graph");

  assert(strcmp(valueString, "Graph") == 0);

  void * rowPointer = XdmfGraphGetRowPointer(matrix, &status);

  void * columnIndex = XdmfGraphGetColumnIndex(matrix, &status);

  void * values = XdmfGraphGetValues(matrix, &status);

  unsigned int insertedVal = 0;

  XdmfArrayInsertValue(rowPointer, 0, &insertedVal, XDMF_ARRAY_TYPE_UINT32, &status);

  insertedVal = 1;

  XdmfArrayInsertValue(rowPointer, 1, &insertedVal, XDMF_ARRAY_TYPE_UINT32, &status);

  insertedVal = 2;

  XdmfArrayInsertValue(rowPointer, 2, &insertedVal, XDMF_ARRAY_TYPE_UINT32, &status);

  insertedVal = 3;

  XdmfArrayInsertValue(rowPointer, 3, &insertedVal, XDMF_ARRAY_TYPE_UINT32, &status);

  insertedVal = 1;

  XdmfArrayInsertValue(columnIndex, 0, &insertedVal, XDMF_ARRAY_TYPE_UINT32, &status);

  insertedVal = 2;

  XdmfArrayInsertValue(columnIndex, 1, &insertedVal, XDMF_ARRAY_TYPE_UINT32, &status);

  insertedVal = 0;

  XdmfArrayInsertValue(columnIndex, 2, &insertedVal, XDMF_ARRAY_TYPE_UINT32, &status);

  double insertedDouble = 5.0;

  XdmfArrayInsertValue(values, 0, &insertedDouble, XDMF_ARRAY_TYPE_FLOAT64, &status);

  insertedDouble = 6.0;

  XdmfArrayInsertValue(values, 1, &insertedDouble, XDMF_ARRAY_TYPE_FLOAT64, &status);

  insertedDouble = -1.0;

  XdmfArrayInsertValue(values, 2, &insertedDouble, XDMF_ARRAY_TYPE_FLOAT64, &status);

  valueString = XdmfSparseMatrixGetValuesString((XDMFSPARSEMATRIX *)((void *)matrix), &status);

  printf("%s\n?=\n%s\n", valueString, "0.0, 5, 0.0, \n0.0, 0.0, 6, \n-1, 0.0, 0.0, \n");

  assert(strcmp(valueString, "0.0, 5, 0.0, \n0.0, 0.0, 6, \n-1, 0.0, 0.0, \n") == 0);

  free(valueString);

  rowPointer = XdmfArrayNew();

  columnIndex = XdmfArrayNew();

  values = XdmfArrayNew();

  insertedVal = 0;

  XdmfArrayInsertValue(rowPointer, 0, &insertedVal, XDMF_ARRAY_TYPE_UINT32, &status);

  insertedVal = 1;

  XdmfArrayInsertValue(rowPointer, 1, &insertedVal, XDMF_ARRAY_TYPE_UINT32, &status);

  insertedVal = 2;

  XdmfArrayInsertValue(rowPointer, 2, &insertedVal, XDMF_ARRAY_TYPE_UINT32, &status);

  insertedVal = 3;

  XdmfArrayInsertValue(rowPointer, 3, &insertedVal, XDMF_ARRAY_TYPE_UINT32, &status);

  insertedVal = 1;

  XdmfArrayInsertValue(columnIndex, 0, &insertedVal, XDMF_ARRAY_TYPE_UINT32, &status);

  insertedVal = 2;

  XdmfArrayInsertValue(columnIndex, 1, &insertedVal, XDMF_ARRAY_TYPE_UINT32, &status);

  insertedVal = 0;

  XdmfArrayInsertValue(columnIndex, 2, &insertedVal, XDMF_ARRAY_TYPE_UINT32, &status);

  insertedDouble = 5.0;

  XdmfArrayInsertValue(values, 0, &insertedDouble, XDMF_ARRAY_TYPE_FLOAT64, &status);

  insertedDouble = 6.0;

  XdmfArrayInsertValue(values, 1, &insertedDouble, XDMF_ARRAY_TYPE_FLOAT64, &status);

  insertedDouble = -1.0;

  XdmfArrayInsertValue(values, 2, &insertedDouble, XDMF_ARRAY_TYPE_FLOAT64, &status);

  XdmfGraphSetColumnIndex(matrix, columnIndex, 0, &status);

  XdmfGraphSetRowPointer(matrix, rowPointer, 0, &status);

  XdmfGraphSetValues(matrix, values, 0, &status);

  XDMFWRITER * writer = XdmfWriterNew("matrixfile.xmf");

  XDMFATTRIBUTE * attrib1 = XdmfAttributeNew();

  int i = 0;

  for (i = 0; i < 10; i++) {
    XdmfAttributePushBack(attrib1, &i, XDMF_ARRAY_TYPE_INT32, &status);
  }

  XdmfAttributeSetType(attrib1, XDMF_ATTRIBUTE_TYPE_SCALAR, &status);

  XdmfAttributeSetCenter(attrib1, XDMF_ATTRIBUTE_CENTER_NODE, &status);

  XdmfAttributeSetName(attrib1, "Attribute 1", &status);

  XdmfGraphInsertAttribute(matrix, attrib1, 0);

  XDMFATTRIBUTE * attrib2 = XdmfAttributeNew();

  for (i = 10; i < 20; i++) {
    XdmfAttributePushBack(attrib2, &i, XDMF_ARRAY_TYPE_INT32, &status);
  }

  XdmfAttributeSetType(attrib2, XDMF_ATTRIBUTE_TYPE_SCALAR, &status);

  XdmfAttributeSetCenter(attrib2, XDMF_ATTRIBUTE_CENTER_NODE, &status);

  XdmfAttributeSetName(attrib2, "Attribute 2", &status);

  XdmfGraphInsertAttribute(matrix, attrib2, 0);

  XdmfGraphAccept(matrix, (XDMFVISITOR *)writer, &status);

  unsigned int numRows = XdmfGraphGetNumberRows(matrix);

  unsigned int numCols = XdmfGraphGetNumberColumns(matrix);

  printf("%d ?= %d\n%d ?= %d\n", numRows, 3, numCols, 3);

  assert(numRows == 3);

  assert(numCols == 3);

  valueString = XdmfGraphGetValuesString(matrix, &status);

  printf("%s\n?=\n%s\n", valueString, "0.0, 5, 0.0, \n0.0, 0.0, 6, \n-1, 0.0, 0.0, \n");

  assert(strcmp(valueString, "0.0, 5, 0.0, \n0.0, 0.0, 6, \n-1, 0.0, 0.0, \n") == 0);

  free(valueString);

  unsigned int numInfo = XdmfGraphGetNumberInformations(matrix);

  printf("%d ?= %d\n", numInfo, 0);

  assert(numInfo == 0);

  void * reader = XdmfReaderNew();

  XDMFGRAPH * readGraph = (XDMFGRAPH *)XdmfReaderRead(reader, "matrixfile.xmf", &status);

  unsigned int readNumRows = XdmfGraphGetNumberRows(readGraph);

  unsigned int readNumCols = XdmfGraphGetNumberColumns(readGraph);

  printf("%d ?= %d\n%d ?= %d\n", readNumRows, 3, readNumCols, 3);

  assert(readNumRows == 3);

  assert(readNumCols == 3);

  unsigned int readNumNodes = XdmfGraphGetNumberNodes(readGraph);

  printf("%d ?= %d\n", readNumNodes, 3);

  assert(readNumNodes == 3);

  valueString = XdmfGraphGetValuesString(readGraph, &status);

  printf("%s\n?=\n%s\n", valueString, "0.0, 5, 0.0, \n0.0, 0.0, 6, \n-1, 0.0, 0.0, \n");

  assert(strcmp(valueString, "0.0, 5, 0.0, \n0.0, 0.0, 6, \n-1, 0.0, 0.0, \n") == 0);

  free(valueString);

  unsigned int readNumAttributes = XdmfGraphGetNumberAttributes(readGraph);

  printf("%d ?= %d\n", readNumAttributes, 2);

  assert(readNumAttributes == 2);

  XDMFATTRIBUTE * readAttribute = XdmfGraphGetAttribute(readGraph, 1);

  valueString = XdmfAttributeGetName(readAttribute);

  printf("%s ?= %s\n", valueString, "Attribute 2");

  assert(strcmp(valueString, "Attribute 2") == 0);

  free(valueString);

  XDMFATTRIBUTE * readAttribute2 = XdmfGraphGetAttributeByName(readGraph, "Attribute 1");

  valueString = XdmfAttributeGetValuesString(readAttribute2);

  printf("%s ?= %s\n", valueString, "0 1 2 3 4 5 6 7 8 9");

  assert(strcmp(valueString, "0 1 2 3 4 5 6 7 8 9") == 0);

  XdmfGraphRemoveAttribute(readGraph, 0);

  readNumAttributes = XdmfGraphGetNumberAttributes(readGraph);

  printf("%d ?= %d\n", readNumAttributes, 1);

  assert(readNumAttributes == 1);

  XDMFATTRIBUTE * readAttribute3 = XdmfGraphGetAttributeByName(readGraph, "Attribute 2");

  valueString = XdmfAttributeGetValuesString(readAttribute3);

  printf("%s ?= %s\n", valueString, "10 11 12 13 14 15 16 17 18 19");

  assert(strcmp(valueString, "10 11 12 13 14 15 16 17 18 19") == 0);

  XdmfGraphRemoveAttributeByName(readGraph, "Attribute 2");

  readNumAttributes = XdmfGraphGetNumberAttributes(readGraph);

  printf("%d ?= %d\n", readNumAttributes, 0);

  assert(readNumAttributes == 0);

  return 0;
}
