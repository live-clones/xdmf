#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfSparseMatrix.hpp"
#include "XdmfWriter.hpp"

#include "assert.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"

int main()
{
  int status = 0;

  XDMFSPARSEMATRIX * matrix = XdmfSparseMatrixNew(3, 3);

  XdmfSparseMatrixSetName(matrix, "testMatrix", &status);

  char * valueString = XdmfSparseMatrixGetName(matrix);

  printf("%s ?= %s\n", valueString, "testMatrix");

  assert(strcmp(valueString, "testMatrix") == 0);

  free(valueString);

  valueString = XdmfSparseMatrixGetItemTag(matrix);

  printf("%s ?= %s\n", valueString, "SparseMatrix");

  assert(strcmp(valueString, "SparseMatrix") == 0);
  
  free(valueString);

  XDMFARRAY * rowPointer = XdmfSparseMatrixGetRowPointer(matrix, &status);

  XDMFARRAY * columnIndex = XdmfSparseMatrixGetColumnIndex(matrix, &status);

  XDMFARRAY * values = XdmfSparseMatrixGetValues(matrix, &status);

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

  valueString = XdmfSparseMatrixGetValuesString(matrix, &status);

  printf("%s\n?=\n%s\n", valueString, "0.0, 5, 0.0, \n0.0, 0.0, 6, \n-1, 0.0, 0.0, \n");

  assert(strcmp(valueString, "0.0, 5, 0.0, \n0.0, 0.0, 6, \n-1, 0.0, 0.0, \n") == 0);

  free(valueString);

  XdmfArrayFree(rowPointer);

  XdmfArrayFree(columnIndex);

  XdmfArrayFree(values);
  
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

  XdmfSparseMatrixSetColumnIndex(matrix, columnIndex, 0, &status);

  XdmfSparseMatrixSetRowPointer(matrix, rowPointer, 0, &status);

  XdmfSparseMatrixSetValues(matrix, values, 0, &status);

  XdmfArrayFree(rowPointer);
  
  XdmfArrayFree(columnIndex);
  
  XdmfArrayFree(values);

  XDMFWRITER * writer = XdmfWriterNew("matrixfile.xmf");

  XdmfSparseMatrixAccept(matrix, (XDMFVISITOR *)writer, &status);

  XdmfWriterFree(writer);

  unsigned int numRows = XdmfSparseMatrixGetNumberRows(matrix);

  unsigned int numCols = XdmfSparseMatrixGetNumberColumns(matrix);

  printf("%d ?= %d\n", numRows, 3);

  assert(numRows == 3);

  printf("%d ?= %d\n", numCols, 3);

  assert(numCols == 3);

  valueString = XdmfSparseMatrixGetValuesString(matrix, &status);

  printf("%s\n?=\n%s\n", valueString, "0.0, 5, 0.0, \n0.0, 0.0, 6, \n-1, 0.0, 0.0, \n");

  assert(strcmp(valueString, "0.0, 5, 0.0, \n0.0, 0.0, 6, \n-1, 0.0, 0.0, \n") == 0);

  free(valueString);

  unsigned int numInfo = XdmfSparseMatrixGetNumberInformations(matrix);

  printf("%d ?= %d\n", numInfo, 0);

  assert(numInfo == 0);

  XdmfSparseMatrixFree(matrix);

  return 0;
}
