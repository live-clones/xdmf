#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfSubset.hpp"
#include "XdmfAttribute.hpp"
#include "XdmfWriter.hpp"
#include "XdmfReader.hpp"
#include <map>
#include <iostream>

int main(int, char **)
{
	shared_ptr<XdmfArray> referenceArray = XdmfArray::New();

	for (unsigned int i = 0; i < 27; ++i)
	{
		referenceArray->pushBack(i);
	}

	std::vector<unsigned int> initDimensions;
	initDimensions.push_back(3);
	initDimensions.push_back(3);
	initDimensions.push_back(3);

	referenceArray->resize(initDimensions, 0);

	std::vector<unsigned int> newStarts;
	newStarts.push_back(0);
	newStarts.push_back(0);
	newStarts.push_back(0);
	std::vector<unsigned int> newStrides;
	newStrides.push_back(2);
	newStrides.push_back(2);
	newStrides.push_back(2);
	std::vector<unsigned int> newDimensions;
	newDimensions.push_back(2);
	newDimensions.push_back(2);
	newDimensions.push_back(2);

	shared_ptr<XdmfSubset> testSubset = XdmfSubset::New(referenceArray,
                                                            newStarts,
                                                            newStrides,
                                                            newDimensions);

	shared_ptr<XdmfAttribute> subsetHolder = XdmfAttribute::New();

	subsetHolder->setSubset(testSubset);

	subsetHolder->setReadMode(XdmfArray::Subset);



	shared_ptr<XdmfWriter> subsetWriter = XdmfWriter::New("subset.xmf");

	subsetHolder->accept(subsetWriter);




	subsetHolder->readSubset();

	std::cout << subsetHolder->getValuesString() << std::endl;

	shared_ptr<XdmfReader> subsetReader = XdmfReader::New();

	shared_ptr<XdmfItem> readItem = subsetReader->read("subset.xmf");

	assert(readItem->getItemTag().compare("Attribute") == 0);

	shared_ptr<XdmfAttribute> readSubsetHolder = shared_dynamic_cast<XdmfAttribute>(readItem);

	readSubsetHolder->readSubset();

	std::cout << readSubsetHolder->getSubset()->getReferenceArray()->getValuesString() << std::endl;

	std::cout << readSubsetHolder->getValuesString() << std::endl;

	assert(readSubsetHolder->getValuesString().compare(subsetHolder->getValuesString()) == 0);

	return 0;
}
