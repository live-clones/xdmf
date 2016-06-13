#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfAggregate.hpp"
#include "XdmfAttribute.hpp"
#include "XdmfWriter.hpp"
#include "XdmfReader.hpp"

#include "assert.h"
#include "stdio.h"
#include "string.h"

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
  }

  XDMFATTRIBUTE * aggregateHolder = XdmfAttributeNew();

  XdmfAttributeSetReference(aggregateHolder, (XDMFARRAYREFERENCE *)testAggregate, 0);

  XdmfAttributeSetReadMode(aggregateHolder, XDMF_ARRAY_READ_MODE_REFERENCE, &status);



  XDMFWRITER * aggregateWriter = XdmfWriterNew("aggregate.xmf");

  XdmfAttributeAccept(aggregateHolder, (XDMFVISITOR *)aggregateWriter, &status);

  XdmfAttributeReadReference(aggregateHolder, &status);

  printf("%s\n", XdmfAttributeGetValuesString(aggregateHolder));

  XDMFREADER * aggregateReader = XdmfReaderNew();

  XDMFITEM * readItem = XdmfReaderRead(aggregateReader,"aggregate.xmf", &status);

  printf("%s ?= Attribute\n", XdmfItemGetItemTag(readItem));

  assert(strcmp(XdmfItemGetItemTag(readItem), "Attribute") == 0);

  XDMFATTRIBUTE * readAggregateHolder = (XDMFATTRIBUTE *)readItem;

  XdmfAttributeReadReference(readAggregateHolder, &status);

  printf("%s\n?=\n%s\n", XdmfAttributeGetValuesString(readAggregateHolder), XdmfAttributeGetValuesString(aggregateHolder));

  assert(strcmp(XdmfAttributeGetValuesString(readAggregateHolder), XdmfAttributeGetValuesString(aggregateHolder)) == 0);



  XDMFARRAY * aggregateHolder2 = XdmfArrayNew();

  XdmfArraySetReference(aggregateHolder2, (XDMFARRAYREFERENCE *)testAggregate, 1);

  XdmfArraySetReadMode(aggregateHolder2, XDMF_ARRAY_READ_MODE_REFERENCE, &status);

  XdmfArrayAccept(aggregateHolder2, (XDMFVISITOR *)aggregateWriter, &status);

  XdmfArrayReadReference(aggregateHolder2, &status);

  printf("%s\n", XdmfArrayGetValuesString(aggregateHolder2));

  printf("reading");

  XdmfItemFree(readItem);

  readItem = XdmfReaderRead(aggregateReader, "aggregate.xmf", &status);

  printf("%s ?= DataItem\n", XdmfItemGetItemTag(readItem));

  assert(strcmp(XdmfItemGetItemTag(readItem), "DataItem") == 0);

  XDMFARRAY * readAggregateHolder2 = (XDMFARRAY *)readItem;

  XdmfArrayReadReference(readAggregateHolder2, &status);

  printf("%s\n?=\n%s\n", XdmfArrayGetValuesString(readAggregateHolder2), XdmfArrayGetValuesString(aggregateHolder2));

  assert(strcmp(XdmfArrayGetValuesString(readAggregateHolder2), XdmfArrayGetValuesString(aggregateHolder2)) == 0);

  return 0;
}
