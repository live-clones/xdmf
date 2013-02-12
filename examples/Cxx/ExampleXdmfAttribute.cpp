#include "XdmfAttribute.hpp"
#include "XdmfAttributeType.hpp"
#include "XdmfAttributeCenter.hpp"

int main(int, char **)
{
	shared_ptr<XdmfAttribute> exampleAttribute = XdmfAttribute::New();

	//Assuming that exampleAttribute is a shared pointer to an XdmfAttribute object

	exampleAttribute->setCenter(XdmfAttributeCenter::Node());

	std::string newName = "New Name";
	exampleAttribute->setName(newName);

	exampleAttribute->setType(XdmfAttributeType::Node());

	shared_ptr<const XdmfAttributeCenter> exampleCenter = exampleAttribute->getCenter();

	if (exampleCenter == XdmfAttributeCenter::Grid())
	{
		//do whatever is to be done if the center is grid
	}

	std::string exampleName = exampleAttribute->getName();

	shared_ptr<const XdmfAttributeType> exampleType = exampleAttribute->getType();

	if (exampleType == XdmfAttributeType:Scalar())
	{
		//do whatever is to be done if the attribute is a scalar
	}

	return 0;
}
