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

	std::string exampleName = exampleAttribute->getName();

	shared_ptr<const XdmfAttributeType> exampleType = exampleAttribute->getType();

	return 0;
}
