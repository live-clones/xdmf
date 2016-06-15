#include "XdmfArray.hpp"
#include "XdmfWriter.hpp"

#include "stdio.h"
#include "stdlib.h"

#include "string.h"
#include "assert.h"

int main()
{
  int status;
  XDMFWRITER * writer = XdmfWriterNew("testfile.xmf");
//TODO HeavyDataWriter interaction

  char * testFileName = XdmfWriterGetFilePath(writer, &status);
  int testLimit = XdmfWriterGetLightDataLimit(writer, &status);
  printf("light data limit = %d\n", testLimit);

  return 0;
}
