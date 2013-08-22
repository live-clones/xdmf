#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfAttribute.hpp"
#include "XdmfSubset.hpp"
#include <vector>
#include <map>

int main(int, char **)
{
	//#initialization begin

	shared_ptr<XdmfArray> baseArray = XdmfArray::New();

	for (unsigned int i = 0; i < 10; ++i)
	{
		baseArray->pushBack(i);
	}

	std::vector<unsigned int> initStart;
	initStart.push_back(0);
	std::vector<unsigned int> initStride;
	initStride.push_back(1);
	std::vector<unsigned int> initDimension;
	initDimension.push_back(10);

	shared_ptr<XdmfSubset> exampleSubset = XdmfSubset::New(baseArray,
                                                               initStart,
                                                               initStride,
                                                               initDimension);

	//#initialization end

	//#getStart begin

	std::vector<unsigned int> exampleStart = exampleSubset->getStart();

	//#getStart end

	//#setStart begin

	exampleSubset->setStart(exampleStart);

	//#setStart end

	//#getStride begin

	std::vector<unsigned int> exampleStride = exampleSubset->getStride();

	//#getStride end

	//#setStride begin

	exampleSubset->setStride(exampleStride);

	//#setStride end

	//#getDimensions begin

	std::vector<unsigned int> exampleDimensions = exampleSubset->getDimensions();

	//#getDimensions end

	//#setDimensions begin

	exampleSubset->setDimensions(exampleDimensions);

	//#setDimensions end

	//#getReferenceArray begin

	shared_ptr<XdmfArray> exampleInternalArray = exampleSubset->getReferenceArray();

	//#getReferenceArray end

	//#setReferenceArray begin

	exampleSubset->setReferenceArray(exampleInternalArray);

	//#setReferenceArray end

	//#getSize begin

	int exampleSize = exampleSubset->getSize();

	//#getSize end

	//#setConstructedType begin

	shared_ptr<XdmfAttribute> typeAttribute = XdmfAttribute::New();
	exampleSubset->setConstructedType(typeAttribute->getItemTag());

	//#setConstructedType end

	//#getConstructedType begin

	std::string exampleType = exampleSubset->getConstructedType();

	//#getConstructedType end

	//#setConstructedProperties begin

	shared_ptr<XdmfAttribute> propertyAttribute = XdmfAttribute::New();
	exampleSubset->setConstructedProperties(propertyAttribute->getItemProperties());

	//#setConstructedProperties end

	//#getConstructedProperties begin

	std::map<std::string, std::string> exampleProperties = exampleSubset->getConstructedProperties();

	//#getConstructedProperties end

	//#read begin

	shared_ptr<XdmfArray> subsetResult = exampleSubset->read();

	//#read end

	return 0;
}
