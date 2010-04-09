#include "XdmfAttribute.hpp"

int main(int argc, char* argv[])
{
	// Test != and == operators
	assert(XdmfAttributeType::Scalar == XdmfAttributeType::Scalar);
	assert(XdmfAttributeType::Scalar == XdmfAttributeType::Vector == false);
	assert(XdmfAttributeType::Scalar != XdmfAttributeType::Scalar == false);
	assert(XdmfAttributeType::Scalar != XdmfAttributeType::Vector);

	// Test copy constructor and assigment operators
	XdmfAttributeType type = XdmfAttributeType::Scalar();
	assert(type.getName().compare("Scalar") == 0);
	XdmfAttributeType type2(type); // Copy Constructor
	assert(type2.getName().compare("Scalar") == 0);
	XdmfAttributeType type3 = XdmfAttributeType::Vector();
	assert(type3.getName().compare("Vector") == 0);
	type2 = type3;
	assert(type2.getName().compare("Vector") == 0);

	boost::shared_ptr<XdmfAttribute> attribute = XdmfAttribute::New();
	assert(attribute->getAttributeType() == XdmfAttributeType::NoAttributeType());
	assert(attribute->getAttributeType().getName().compare("None") == 0);
	attribute->setAttributeType(XdmfAttributeType::Scalar());
	assert(attribute->getAttributeType() == XdmfAttributeType::Scalar());
	assert(attribute->getAttributeTypeAsString().compare("Scalar") == 0);

	return 0;
}
