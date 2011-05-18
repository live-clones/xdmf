#include "XdmfAttribute.hpp"
#include "XdmfAttributeType.hpp"

int main(int, char **)
{
  // Test != and == operators
  assert(XdmfAttributeType::Scalar() == XdmfAttributeType::Scalar());
  assert(XdmfAttributeType::Scalar() == XdmfAttributeType::Vector() == false);
  assert(XdmfAttributeType::Scalar() != XdmfAttributeType::Scalar() == false);
  assert(XdmfAttributeType::Scalar() != XdmfAttributeType::Vector());

  boost::shared_ptr<XdmfAttribute> attribute = XdmfAttribute::New();
  assert(attribute->getType() == XdmfAttributeType::NoAttributeType());
  attribute->setType(XdmfAttributeType::Scalar());
  assert(attribute->getType() == XdmfAttributeType::Scalar());

  return 0;
}
