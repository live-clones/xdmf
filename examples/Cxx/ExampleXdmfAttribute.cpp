#include "XdmfAttribute.hpp"
#include "XdmfAttributeType.hpp"
#include "XdmfAttributeCenter.hpp"

int main(int, char **)
{
	//#initialization begin
	shared_ptr<XdmfAttribute> exampleAttribute = XdmfAttribute::New();
	//#initialization end

	//Assuming that exampleAttribute is a shared pointer to an XdmfAttribute object

	//#setCenter begin

	exampleAttribute->setCenter(XdmfAttributeCenter::Node());

	//#setCenter end

	//#setName begin

	std::string newName = "New Name";
	exampleAttribute->setName(newName);

	//#setName end

	//#setType begin

	exampleAttribute->setType(XdmfAttributeType::Scalar());

	//#setType end

	//#getCenter begin

	shared_ptr<const XdmfAttributeCenter> exampleCenter = exampleAttribute->getCenter();

	if (exampleCenter == XdmfAttributeCenter::Grid())
	{
		//do whatever is to be done if the center is grid
	}

	//#getCenter end

	//#getName begin

	std::string exampleName = exampleAttribute->getName();

	//#getName end

        //#getType begin

	shared_ptr<const XdmfAttributeType> exampleType = exampleAttribute->getType();

	if (exampleType == XdmfAttributeType:Scalar())
	{
		//do whatever is to be done if the attribute is a scalar
	}

	//#getType end

	return 0;
}
