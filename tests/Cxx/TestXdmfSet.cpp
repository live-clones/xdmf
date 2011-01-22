#include "XdmfAttribute.hpp"
#include "XdmfSet.hpp"
#include "XdmfSetType.hpp"

int main(int, char *)
{
  // Test != and == operators
  assert(XdmfSetType::Node() == XdmfSetType::Node());
  assert(XdmfSetType::Node() == XdmfSetType::Cell() == false);
  assert(XdmfSetType::Node() != XdmfSetType::Node() == false);
  assert(XdmfSetType::Node() != XdmfSetType::Cell());

  boost::shared_ptr<XdmfSet> set = XdmfSet::New();
  assert(set->getType() == XdmfSetType::NoSetType());
  set->setType(XdmfSetType::Node());
  assert(set->getType() == XdmfSetType::Node());
  set->setName("TEST");
  assert(set->getName().compare("TEST") == 0);

  assert(set->getNumberAttributes() == 0);
  assert(set->getAttribute("") == NULL);
  assert(set->getAttribute(0) == NULL);
  boost::shared_ptr<XdmfAttribute> attribute = XdmfAttribute::New();
  attribute->setName("foo");
  set->insert(attribute);
  assert(set->getNumberAttributes() == 1);
  assert(set->getAttribute(0) == attribute);
  assert(set->getAttribute("foo") == attribute);
  set->removeAttribute(0);
  assert(set->getNumberAttributes() == 0);
  set->insert(attribute);
  set->removeAttribute("no");
  assert(set->getNumberAttributes() == 1);
  set->removeAttribute("foo");
  assert(set->getNumberAttributes() == 0);
  return 0;
}
