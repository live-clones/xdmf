#include "XdmfAttribute.hpp"
#include "XdmfAttributeType.hpp"
#include <iostream>
#include <assert.h>

int main(int, char **)
{
  // Test != and == operators
  std::cout << XdmfAttributeType::Scalar() << " ?= " << XdmfAttributeType::Scalar() << std::endl;
  std::cout << XdmfAttributeType::Scalar() << " ?!= " << XdmfAttributeType::Vector() << std::endl;
  assert(XdmfAttributeType::Scalar() == XdmfAttributeType::Scalar());
  assert((XdmfAttributeType::Scalar() == XdmfAttributeType::Vector()) == false);
  assert((XdmfAttributeType::Scalar() != XdmfAttributeType::Scalar()) == false);
  assert(XdmfAttributeType::Scalar() != XdmfAttributeType::Vector());

  shared_ptr<XdmfAttribute> attribute = XdmfAttribute::New();
  std::cout << attribute->getType() << " ?= " << XdmfAttributeType::NoAttributeType() << std::endl;
  assert(attribute->getType() == XdmfAttributeType::NoAttributeType());
  attribute->setType(XdmfAttributeType::Scalar());
  std::cout << attribute->getType() << " ?= " << XdmfAttributeType::Scalar() << std::endl;
  assert(attribute->getType() == XdmfAttributeType::Scalar());

  return 0;
}
