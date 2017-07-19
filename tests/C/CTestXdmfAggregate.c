#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfAggregate.hpp"
#include "XdmfAttribute.hpp"
#include "XdmfWriter.hpp"
#include "XdmfReader.hpp"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
  int status = 0;

  unsigned int iteration = 0;
  unsigned int i = 0;

  XDMFAGGREGATE * testAggregate = XdmfAggregateNew();

  for (iteration = 0; iteration < 5; ++iteration)
  {
    XDMFARRAY * referenceArray = XdmfArrayNew();

    for (i = 0; i < 27; ++i)
    {
      int pushedVal = i*(iteration + 1);
      XdmfArrayPushBack(referenceArray, &pushedVal, XDMF_ARRAY_TYPE_INT32, &status);
    }
    XdmfAggregateInsertArray(testAggregate, referenceArray, 1);

    XdmfArrayFree(referenceArray);

  }

  XDMFATTRIBUTE * aggregateHolder = XdmfAttributeNew();

  XdmfAttributeSetReference(aggregateHolder, (XDMFARRAYREFERENCE *)testAggregate, 0);

  XdmfAttributeSetReadMode(aggregateHolder, XDMF_ARRAY_READ_MODE_REFERENCE, &status);

  XDMFWRITER * aggregateWriter = XdmfWriterNew("aggregate.xmf");

  XdmfAttributeAccept(aggregateHolder, (XDMFVISITOR *)aggregateWriter, &status);

  XdmfAttributeReadReference(aggregateHolder, &status);

  char * aggregateHolderString = XdmfAttributeGetValuesString(aggregateHolder);
  
  printf("%s\n", aggregateHolderString);

  free(aggregateHolderString);

  XDMFREADER * aggregateReader = XdmfReaderNew();

  XDMFITEM * readItem = XdmfReaderRead(aggregateReader,"aggregate.xmf", &status);

  char * itemTag = XdmfItemGetItemTag(readItem);

  printf("%s ?= Attribute\n", itemTag);

  assert(strcmp(itemTag, "Attribute") == 0);

  free(itemTag);

  XDMFATTRIBUTE * readAggregateHolder = (XDMFATTRIBUTE *)readItem;

  XdmfAttributeReadReference(readAggregateHolder, &status);

  char * readAggregateHolderString = XdmfAttributeGetValuesString(readAggregateHolder);

  aggregateHolderString = XdmfAttributeGetValuesString(aggregateHolder);

  printf("%s\n?=\n%s\n", readAggregateHolderString, aggregateHolderString);

  assert(strcmp(readAggregateHolderString, aggregateHolderString) == 0);

  free(readAggregateHolderString);
  
  free(aggregateHolderString);

  XdmfAttributeFree(aggregateHolder);

  XDMFARRAY * aggregateHolder2 = XdmfArrayNew();

  XdmfArraySetReference(aggregateHolder2, (XDMFARRAYREFERENCE *)testAggregate, 1);

  XdmfArraySetReadMode(aggregateHolder2, XDMF_ARRAY_READ_MODE_REFERENCE, &status);

  XdmfArrayAccept(aggregateHolder2, (XDMFVISITOR *)aggregateWriter, &status);

  XdmfArrayReadReference(aggregateHolder2, &status);

  char * aggregateHolder2String = XdmfArrayGetValuesString(aggregateHolder2);

  printf("%s\n", aggregateHolder2String);

  free(aggregateHolder2String);

  printf("reading");

  XdmfItemFree(readItem);

  readItem = XdmfReaderRead(aggregateReader, "aggregate.xmf", &status);

  XdmfReaderFree(aggregateReader);

  itemTag = XdmfItemGetItemTag(readItem);

  printf("%s ?= DataItem\n", itemTag);

  assert(strcmp(itemTag, "DataItem") == 0);

  free(itemTag);

  XDMFARRAY * readAggregateHolder2 = XdmfArrayCast(readItem);

  XdmfItemFree(readItem);

  XdmfArrayReadReference(readAggregateHolder2, &status);

  char * readAggregate2String = XdmfArrayGetValuesString(readAggregateHolder2);

  char * aggregate2String = XdmfArrayGetValuesString(aggregateHolder2); 
    
  printf("%s\n?=\n%s\n", readAggregate2String, aggregate2String);

  assert(strcmp(readAggregate2String, aggregate2String) == 0);

  free(readAggregate2String);
  
  free(aggregate2String);

  XdmfArrayFree(aggregateHolder2);

  XdmfWriterFree(aggregateWriter);

  XdmfAggregateFree(testAggregate);

  XdmfArrayFree(readAggregateHolder2);

  return 0;
}
