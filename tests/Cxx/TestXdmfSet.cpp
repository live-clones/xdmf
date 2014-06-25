#include "XdmfAttribute.hpp"
#include "XdmfSet.hpp"
#include "XdmfSetType.hpp"
#include <iostream>

int main(int, char **)
{
  // Test != and == operators
  std::cout << XdmfSetType::Node() << " ?= " << XdmfSetType::Node() << std::endl;
  std::cout << XdmfSetType::Node() << " ?!= " << XdmfSetType::Cell() << std::endl;
  assert(XdmfSetType::Node() == XdmfSetType::Node());
  assert((XdmfSetType::Node() == XdmfSetType::Cell()) == false);
  assert((XdmfSetType::Node() != XdmfSetType::Node()) == false);
  assert(XdmfSetType::Node() != XdmfSetType::Cell());

  shared_ptr<XdmfSet> set = XdmfSet::New();
  std::cout << set->getType() << " ?= " << XdmfSetType::NoSetType() << std::endl;
  assert(set->getType() == XdmfSetType::NoSetType());
  set->setType(XdmfSetType::Node());
  std::cout << set->getType() << " ?= " << XdmfSetType::Node() << std::endl;
  assert(set->getType() == XdmfSetType::Node());
  set->setName("TEST");
  std::cout << set->getName() << " ?= TEST" << std::endl;
  assert(set->getName().compare("TEST") == 0);

  std::cout << set->getNumberAttributes() << " ?= " << 0 << std::endl;
  std::cout << set->getAttribute("") << " ?= " << NULL << std::endl;
  std::cout << set->getAttribute(0) << " ?= " << NULL << std::endl;
  assert(set->getNumberAttributes() == 0);
  assert(set->getAttribute("") == NULL);
  assert(set->getAttribute(0) == NULL);
  shared_ptr<XdmfAttribute> attribute = XdmfAttribute::New();
  attribute->setName("foo");
  set->insert(attribute);
  std::cout << set->getNumberAttributes() << " ?= " << 1 << std::endl;
  std::cout << set->getAttribute("foo") << " ?= " << attribute << std::endl;
  std::cout << set->getAttribute(0) << " ?= " << attribute << std::endl;
  assert(set->getNumberAttributes() == 1);
  assert(set->getAttribute(0) == attribute);
  assert(set->getAttribute("foo") == attribute);
  set->removeAttribute(0);
  std::cout << set->getNumberAttributes() << " ?= " << 0 << std::endl;
  assert(set->getNumberAttributes() == 0);
  set->insert(attribute);
  set->removeAttribute("no");
  std::cout << set->getNumberAttributes() << " ?= " << 1 << std::endl;
  assert(set->getNumberAttributes() == 1);
  set->removeAttribute("foo");
  std::cout << set->getNumberAttributes() << " ?= " << 0 << std::endl;
  assert(set->getNumberAttributes() == 0);
  return 0;
}
