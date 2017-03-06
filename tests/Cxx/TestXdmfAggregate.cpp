#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfAggregate.hpp"
#include "XdmfAttribute.hpp"
#include "XdmfWriter.hpp"
#include "XdmfReader.hpp"
#include <map>
#include <iostream>
#include <assert.h>

int main(int, char **)
{
	shared_ptr<XdmfAggregate> testAggregate = XdmfAggregate::New();

	for (unsigned int iteration = 0; iteration < 5; ++iteration)
	{
		shared_ptr<XdmfArray> referenceArray = XdmfArray::New();

		for (unsigned int i = 0; i < 27; ++i)
		{
			referenceArray->pushBack(i*(iteration + 1));
		}
		testAggregate->insert(referenceArray);
	}

	shared_ptr<XdmfAttribute> aggregateHolder = XdmfAttribute::New();

	aggregateHolder->setReference(testAggregate);

	aggregateHolder->setReadMode(XdmfArray::Reference);



	shared_ptr<XdmfWriter> aggregateWriter = XdmfWriter::New("aggregate.xmf");

	aggregateHolder->accept(aggregateWriter);

	aggregateHolder->readReference();

	std::cout << aggregateHolder->getValuesString() << std::endl;

	shared_ptr<XdmfReader> aggregateReader = XdmfReader::New();

	shared_ptr<XdmfItem> readItem = aggregateReader->read("aggregate.xmf");

        std::cout << readItem->getItemTag() << " ?= Attribute" << std::endl;

	assert(readItem->getItemTag().compare("Attribute") == 0);

	shared_ptr<XdmfAttribute> readAggregateHolder = shared_dynamic_cast<XdmfAttribute>(readItem);

	readAggregateHolder->readReference();

	std::cout << readAggregateHolder->getValuesString() << "\n?=\n" << aggregateHolder->getValuesString() << std::endl;

	assert(readAggregateHolder->getValuesString().compare(aggregateHolder->getValuesString()) == 0);

        shared_ptr<XdmfArray> aggregateHolder2 = XdmfArray::New();

        aggregateHolder2->setReference(testAggregate);

        aggregateHolder2->setReadMode(XdmfArray::Reference);

        aggregateHolder2->accept(aggregateWriter);

        aggregateHolder2->readReference();

        std::cout << aggregateHolder2->getValuesString() << std::endl;

        std::cout << "reading" << std::endl;

        readItem = aggregateReader->read("aggregate.xmf");

        std::cout << readItem->getItemTag() << " ?= DataItem" << std::endl;

        assert(readItem->getItemTag().compare("DataItem") == 0);

        shared_ptr<XdmfArray> readAggregateHolder2 = shared_dynamic_cast<XdmfArray>(readItem);

        readAggregateHolder2->readReference();

        std::cout << readAggregateHolder2->getValuesString() 
                  << "\n?=\n" << aggregateHolder2->getValuesString() << std::endl;

        assert(readAggregateHolder2->getValuesString().compare(aggregateHolder2->getValuesString()) == 0);


	return 0;
}
