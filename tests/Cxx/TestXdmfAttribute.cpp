#include "XdmfAttribute.hpp"

int main(int argc, char* argv[])
{
	// Test != and == operators
	assert(XdmfAttributeType::Scalar == XdmfAttributeType::Scalar);
	assert(XdmfAttributeType::Scalar == XdmfAttributeType::Vector == false);
	assert(XdmfAttributeType::Scalar != XdmfAttributeType::Scalar == false);
	assert(XdmfAttributeType::Scalar != XdmfAttributeType::Vector);

	boost::shared_ptr<XdmfAttribute> attribute = XdmfAttribute::New();
	assert(attribute->getAttributeType() == XdmfAttributeType::NoAttributeType());
	attribute->setAttributeType(XdmfAttributeType::Scalar());
	assert(attribute->getAttributeType() == XdmfAttributeType::Scalar());

	return 0;
}
