#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfFunction.hpp"
#include "XdmfWriter.hpp"

#include "assert.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"

XDMFARRAY * maximum(XDMFARRAY ** values, unsigned int numValues);

XDMFARRAY * prepend(XDMFARRAY * val1, XDMFARRAY * val2);

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

  XDMFFUNCTION * function = XdmfFunctionNew();

  free(function);

  char * keys[2];

  keys[0] = "A";
  keys[1] = "B";

  char * expression = "A#B";

  XDMFARRAY * values[2];

  values[0] = array;
  values[1] = secondarray;

  function = XdmfFunctionNewInit(expression, keys, values, 2);

  void * readArray = XdmfFunctionRead(function, &status);

  valueString = XdmfArrayGetValuesString(readArray);

  printf("%s ?= %s\n", "0 10 1 20 2 30 3 40 4 50 5 60 6 70 7 80 8 90 9 100", valueString);

  assert(strcmp("0 10 1 20 2 30 3 40 4 50 5 60 6 70 7 80 8 90 9 100", valueString) == 0);

  free(valueString);

  char * internalExpression = XdmfFunctionGetExpression(function);

  printf("%s ?= %s\n", "A#B", internalExpression);

  assert(strcmp("A#B", internalExpression) == 0);

  unsigned int numVars = XdmfFunctionGetNumberVariables(function);

  printf("%d ?= %d\n", numVars, 2);

  assert(numVars == 2);

  char ** internalVariableList = XdmfFunctionGetVariableList(function);

  printf("%s ?= %s\n", internalVariableList[0], "A");
  printf("%s ?= %s\n", internalVariableList[1], "B");

  assert(strcmp(internalVariableList[0], "A") == 0);
  assert(strcmp(internalVariableList[1], "B") == 0);

  void * variable1 = XdmfFunctionGetVariable(function, internalVariableList[0]);

  valueString = XdmfArrayGetValuesString(variable1);

  printf("%s ?= %s\n", "0 1 2 3 4 5 6 7 8 9", valueString);

  assert(strcmp("0 1 2 3 4 5 6 7 8 9", valueString) == 0);

  free(valueString);

  void * variable2 = XdmfFunctionGetVariable(function, internalVariableList[1]);

  valueString = XdmfArrayGetValuesString(variable2);

  printf("%s ?= %s\n", "10 20 30 40 50 60 70 80 90 100", valueString);

  assert(strcmp("10 20 30 40 50 60 70 80 90 100", valueString) == 0);

  free(valueString);

  void * thirdarray = XdmfArrayNew();

  for (i = 1; i < 11; i++) {
    val = i * 20;
    XdmfArrayPushBack(thirdarray, &val, XDMF_ARRAY_TYPE_INT32, &status);
  }

  valueString = XdmfArrayGetValuesString(thirdarray);

  printf("%s ?= %s\n", "20 40 60 80 100 120 140 160 180 200", valueString);

  assert(strcmp("20 40 60 80 100 120 140 160 180 200", valueString) == 0);

  free(valueString);

  XdmfFunctionRemoveVariable(function, "B");

  XdmfFunctionInsertVariable(function, "B", thirdarray, 0);

  free(readArray);

  readArray = XdmfFunctionRead(function, &status);

  valueString = XdmfArrayGetValuesString(readArray);

  printf("%s ?= %s\n", "0 20 1 40 2 60 3 80 4 100 5 120 6 140 7 160 8 180 9 200", valueString);

  assert(strcmp("0 20 1 40 2 60 3 80 4 100 5 120 6 140 7 160 8 180 9 200", valueString) == 0);

  free(valueString);

  XdmfFunctionSetExpression(function, "A|B", &status);

  free(readArray);

  readArray = XdmfFunctionRead(function, &status);

  valueString = XdmfArrayGetValuesString(readArray);

  printf("%s ?= %s\n", "0 1 2 3 4 5 6 7 8 9 20 40 60 80 100 120 140 160 180 200", valueString);

  assert(strcmp("0 1 2 3 4 5 6 7 8 9 20 40 60 80 100 120 140 160 180 200", valueString) == 0);

  free(valueString);

  valueString = XdmfFunctionGetItemTag(function);

  printf("%s ?= %s\n", "Function", valueString);

  assert(strcmp("Function", valueString) == 0);

  free(valueString);

  char * constructedTag = XdmfArrayGetItemTag(array);

  XdmfFunctionSetConstructedType(function, constructedTag);

  XdmfFunctionSetConstructedProperties(function, array);

  char * internalTag = XdmfFunctionGetConstructedType(function);

  printf("%s ?= %s\n", "DataItem", internalTag);

  assert(strcmp(internalTag, "DataItem") == 0);

  char * validoperations = XdmfFunctionGetSupportedOperations();

  printf("%s ?= %s\n", "-+/*|#()", validoperations);

  assert(strcmp("-+/*|#()", validoperations) == 0);

  char * validdigits = XdmfFunctionGetValidDigitChars();

  printf("%s ?= %s\n", "1234567890.", validdigits);

  assert(strcmp("1234567890.", validdigits) == 0);

  char * validvariablechars = XdmfFunctionGetValidVariableChars();

  printf("%s ?= %s\n", "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890_:.", validvariablechars);

  assert(strcmp("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890_:.", validvariablechars) == 0);

  unsigned int priority = XdmfFunctionGetOperationPriority('#');

  assert(priority == 1);

  XdmfFunctionAddFunction("MAX", maximum, &status);

  XdmfFunctionAddOperation('%', prepend, 2, &status);

  void * evaluatedarray = XdmfFunctionEvaluateOperation(array, secondarray, '%', &status);

  valueString = XdmfArrayGetValuesString(evaluatedarray);

  printf("%s ?= %s\n", "10 20 30 40 50 60 70 80 90 100 0 1 2 3 4 5 6 7 8 9", valueString);

  assert(strcmp("10 20 30 40 50 60 70 80 90 100 0 1 2 3 4 5 6 7 8 9", valueString) == 0);

  free(valueString);

  free(evaluatedarray);

  XDMFARRAY * evaluatedpointers[3] = {array, secondarray, thirdarray};

  evaluatedarray = XdmfFunctionEvaluateFunction(evaluatedpointers, 3, "MAX", &status);

  valueString = XdmfArrayGetValuesString(evaluatedarray);

  printf("%s ?= %s\n", "200", valueString);

  assert(strcmp("200", valueString) == 0);

  free(valueString);

  free(evaluatedarray);

  char * secondkeys[3];

  secondkeys[0] = "A";
  secondkeys[1] = "B";
  secondkeys[2] = "C";

  evaluatedarray = XdmfFunctionEvaluateExpression("MAX(A|B|C)", secondkeys, evaluatedpointers, 3, &status);

  valueString = XdmfArrayGetValuesString(evaluatedarray);

  printf("%s ?= %s\n", "200", valueString);

  assert(strcmp("200", valueString) == 0);

  free(valueString);

  free(evaluatedarray);

  unsigned int numSupportedFunctions = XdmfFunctionGetNumberSupportedFunctions();

  printf("%d functions supported\n", numSupportedFunctions);

  printf("%d ?= %d\n", numSupportedFunctions, 15);

  assert(numSupportedFunctions == 15);

  char ** functionsSupported = XdmfFunctionGetSupportedFunctions();

  for (i = 0; i < numSupportedFunctions; ++i)
  {
    printf("%s\n", functionsSupported[i]);
  }

  assert(strcmp(functionsSupported[0], "ABS") == 0);
  assert(strcmp(functionsSupported[1], "ABS_TOKEN") == 0);
  assert(strcmp(functionsSupported[2], "ACOS") == 0);
  assert(strcmp(functionsSupported[3], "ASIN") == 0);
  assert(strcmp(functionsSupported[4], "ATAN") == 0);
  assert(strcmp(functionsSupported[5], "AVE") == 0);
  assert(strcmp(functionsSupported[6], "COS") == 0);
  assert(strcmp(functionsSupported[7], "EXP") == 0);
  assert(strcmp(functionsSupported[8], "JOIN") == 0);
  assert(strcmp(functionsSupported[9], "LOG") == 0);
  assert(strcmp(functionsSupported[10], "MAX") == 0);
  assert(strcmp(functionsSupported[11], "SIN") == 0);
  assert(strcmp(functionsSupported[12], "SQRT") == 0);
  assert(strcmp(functionsSupported[13], "SUM") == 0);
  assert(strcmp(functionsSupported[14], "TAN") == 0);

  evaluatedarray = XdmfFunctionChunk(array, secondarray, &status);

  valueString = XdmfArrayGetValuesString(evaluatedarray);

  printf("%s ?= %s\n", "0 1 2 3 4 5 6 7 8 9 10 20 30 40 50 60 70 80 90 100", valueString);

  assert(strcmp("0 1 2 3 4 5 6 7 8 9 10 20 30 40 50 60 70 80 90 100", valueString) == 0);

  free(valueString);

  free(evaluatedarray);

  evaluatedarray = XdmfFunctionInterlace(array, secondarray, &status);

  valueString = XdmfArrayGetValuesString(evaluatedarray);

  printf("%s ?= %s\n", "0 10 1 20 2 30 3 40 4 50 5 60 6 70 7 80 8 90 9 100", valueString);

  assert(strcmp("0 10 1 20 2 30 3 40 4 50 5 60 6 70 7 80 8 90 9 100", valueString) == 0);

  free(valueString);

  free(evaluatedarray);

  evaluatedarray = XdmfFunctionAverage(evaluatedpointers, 3);

  valueString = XdmfArrayGetValuesString(evaluatedarray);

  printf("%s ?= %s\n", "56.5", valueString);

  assert(strcmp("56.5", valueString) == 0);

  free(valueString);

  free(evaluatedarray);

  evaluatedarray = XdmfFunctionSum(evaluatedpointers, 3);

  valueString = XdmfArrayGetValuesString(evaluatedarray);

  printf("%s ?= %s\n", "1695", valueString);

  assert(strcmp("1695", valueString) == 0);

  free(valueString);

  free(evaluatedarray);

  XDMFWRITER * writer = XdmfWriterNew("functionfile.xmf");

  XdmfFunctionAccept(function, (XDMFVISITOR *)writer, &status);

  unsigned int numInfo = XdmfFunctionGetNumberInformations(function);

  printf("%d ?= %d\n", numInfo, 0);

  assert(numInfo == 0);

  return 0;
}

XDMFARRAY * maximum(XDMFARRAY ** values, unsigned int numValues)
{
  int status = 0;
  int type = XdmfArrayGetArrayType(values[0], &status);
  if (type < XDMF_ARRAY_TYPE_INT8 || type > XDMF_ARRAY_TYPE_FLOAT64) {
    // If not a supported type
    return NULL;
  }
  else {
    // convert all to doubles and return the largest
    double maxVal = ((double *)XdmfArrayGetValue(values[0], 0, XDMF_ARRAY_TYPE_FLOAT64, &status))[0];
    int limit;
    double currentVal;
    int i;
    int j;
    for (i = 0; i < numValues; ++i) {
      limit = XdmfArrayGetSize(values[i]);
      for (j = 0; j < limit; ++j) {
        currentVal = ((double *)XdmfArrayGetValue(values[i], j, XDMF_ARRAY_TYPE_FLOAT64, &status))[0];
        if (maxVal < currentVal) {
          maxVal = currentVal;
        }
      }
    }
    XDMFARRAY * returnArray = XdmfArrayNew();
    XdmfArrayPushBack(returnArray, &maxVal, XDMF_ARRAY_TYPE_FLOAT64, &status);
    return returnArray;
  }
}

XDMFARRAY * prepend(XDMFARRAY * val1, XDMFARRAY * val2)
{
  //joins into new array and returns it
  XDMFARRAY * returnArray = XdmfArrayNew();
  unsigned int val1size = XdmfArrayGetSize(val1);
  unsigned int val2size = XdmfArrayGetSize(val2);
  int starts[1] = {0};
  int secondstarts[1] = {0};
  int strides[1] = {1};
  int dimensions[1] = {val2size};
  int status = 0;
  XdmfArrayInsertDataFromXdmfArray(returnArray, val2, secondstarts, starts, dimensions, dimensions, strides, strides, &status);
  secondstarts[0] = val2size;
  dimensions[0] = val1size;
  XdmfArrayInsertDataFromXdmfArray(returnArray, val1, secondstarts, starts, dimensions, dimensions, strides, strides, &status);
  return returnArray;
}
