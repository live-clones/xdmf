#include "XdmfError.hpp"
#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfFunction.hpp"

#include "assert.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"

int main()
{
  int status = 0;

  int currentLimit = XdmfErrorGetLevelLimit();

  printf("%d ?= %d\n", currentLimit, XDMF_ERROR_FATAL);

  assert(currentLimit == XDMF_ERROR_FATAL);

  int currentSuppression = XdmfErrorGetSuppressionLevel();

  printf("%d ?= %d\n", currentSuppression, XDMF_ERROR_WARNING);

  assert(currentSuppression == XDMF_ERROR_WARNING);

  XdmfErrorSetLevelLimit(XDMF_ERROR_DEBUG, &status);

  XdmfErrorSetSuppressionLevel(XDMF_ERROR_DEBUG, &status);

  currentLimit = XdmfErrorGetLevelLimit();

  printf("%d ?= %d\n", currentLimit, XDMF_ERROR_DEBUG);

  assert(currentLimit == XDMF_ERROR_DEBUG);

  currentSuppression = XdmfErrorGetSuppressionLevel();

  printf("%d ?= %d\n", currentSuppression, XDMF_ERROR_DEBUG);

  assert(currentSuppression == XDMF_ERROR_DEBUG);

  int currentCErrorsAreFatal = XdmfErrorGetCErrorsAreFatal();

  printf("? %d\n", !currentCErrorsAreFatal);

  assert(!currentCErrorsAreFatal);

  XdmfErrorSetCErrorsAreFatal(1);

  currentCErrorsAreFatal = XdmfErrorGetCErrorsAreFatal();

  printf("current error status = %d\n", currentCErrorsAreFatal);

  assert(currentCErrorsAreFatal);

  XdmfErrorSetCErrorsAreFatal(0);

  void * array1 = XdmfArrayNew();

  int i = 0;

  for (i = 0; i < 10; i++) {
    XdmfArrayPushBack(array1, &i, XDMF_ARRAY_TYPE_INT32, &status);
  } 

  void * array2 = XdmfArrayNew();

  for (i = 10; i < 20; i++) {
    XdmfArrayPushBack(array2, &i, XDMF_ARRAY_TYPE_INT32, &status);
  }

  char * keys[2];

  keys[0] = "A";
  keys[1] = "B";

  char * expression = "A^B";

  XDMFARRAY * values[2];

  values[0] = array1;
  values[1] = array2;

  void * function = XdmfFunctionNewInit(expression, keys, values, 2);

  void * functionResult = NULL;

  functionResult = XdmfFunctionRead(function, &status);

  printf("%d ?= %d\n", status, XDMF_FAIL);

  assert(status == XDMF_FAIL);

  printf("%p ?= %p\n", functionResult, NULL);

  assert(functionResult == NULL);

  return 0;
}
