#include "XdmfAttribute.hpp"
#include "XdmfUnstructuredGrid.hpp"
#include "XdmfInformation.hpp"
#include "XdmfSet.hpp"
#include "XdmfTime.hpp"

#include <iostream>
#include <assert.h>

int main(int, char **)
{
  shared_ptr<XdmfUnstructuredGrid> grid = XdmfUnstructuredGrid::New();
  std::string gridName = "Grid1";

  grid->setName(gridName);
  std::cout << grid->getName() << " ?= " << gridName << std::endl;
  assert(grid->getName().compare(gridName) == 0);

  // Insert Attributes
  shared_ptr<XdmfAttribute> attr = XdmfAttribute::New();
  attr->setName("foo");
  std::cout << grid->getNumberAttributes() << " ?= " << 0 << std::endl;
  assert(grid->getNumberAttributes() == 0);
  grid->insert(attr);
  std::cout << grid->getNumberAttributes() << " ?= " << 1 << std::endl;
  std::cout << grid->getAttribute(0) << " ?= " << attr << std::endl;
  std::cout << grid->getAttribute("foo") << " ?= " << attr << std::endl;
  std::cout << grid->getAttribute(1) << " ?= " << "NULL" << std::endl;
  std::cout << grid->getAttribute("foo1") << " ?= " << "NULL" << std::endl;
  assert(grid->getNumberAttributes() == 1);
  assert(grid->getAttribute(0) == attr);
  assert(grid->getAttribute("foo") == attr);
  assert(grid->getAttribute(1) == NULL);
  assert(grid->getAttribute("foo1") == NULL);

  // Insert Sets
  shared_ptr<XdmfSet> set = XdmfSet::New();
  set->setName("foo");
  std::cout << grid->getNumberSets() << " ?= " << 0 << std::endl;
  assert(grid->getNumberSets() == 0);
  grid->insert(set);
  std::cout << grid->getNumberSets() << " ?= " << 1 << std::endl;
  std::cout << grid->getSet(0) << " ?= " << set << std::endl;
  std::cout << grid->getSet("foo") << " ?= " << set << std::endl;
  std::cout << grid->getSet(1) << " ?= " << "NULL" << std::endl;
  std::cout << grid->getSet("foo1") << " ?= " << "NULL" << std::endl;
  assert(grid->getNumberSets() == 1);
  assert(grid->getSet(0) == set);
  assert(grid->getSet("foo") == set);
  assert(grid->getSet("foo1") == NULL);
  assert(grid->getSet(1) == NULL);

  // Insert Time
  std::cout << grid->getTime() << " ?= " << "NULL" << std::endl;
  assert(grid->getTime() == NULL);
  shared_ptr<XdmfTime> time = XdmfTime::New(0);
  grid->setTime(time);
  std::cout << grid->getTime() << " ?= " << time << std::endl;
  assert(grid->getTime() == time);

  // Remove Attributes and Sets
  grid->insert(attr);
  grid->insert(set);
  std::cout << grid->getNumberAttributes() << " ?= " << 2 << std::endl;
  std::cout << grid->getNumberSets() << " ?= " << 2 << std::endl;
  assert(grid->getNumberAttributes() == 2);
  assert(grid->getNumberSets() == 2);
  grid->removeAttribute(2);
  grid->removeSet(2);
  std::cout << grid->getNumberAttributes() << " ?= " << 2 << std::endl;
  std::cout << grid->getNumberSets() << " ?= " << 2 << std::endl;
  assert(grid->getNumberAttributes() == 2);
  assert(grid->getNumberSets() == 2);
  grid->removeAttribute(0);
  grid->removeSet(0);
  std::cout << grid->getNumberAttributes() << " ?= " << 1 << std::endl;
  std::cout << grid->getNumberSets() << " ?= " << 1 << std::endl;
  assert(grid->getNumberAttributes() == 1);
  assert(grid->getNumberSets() == 1);
  grid->removeAttribute("foo1");
  grid->removeSet("foo1");
  std::cout << grid->getNumberAttributes() << " ?= " << 1 << std::endl;
  std::cout << grid->getNumberSets() << " ?= " << 1 << std::endl;
  assert(grid->getNumberAttributes() == 1);
  assert(grid->getNumberSets() == 1);
  grid->removeAttribute("foo");
  grid->removeSet("foo");
  std::cout << grid->getNumberAttributes() << " ?= " << 0 << std::endl;
  std::cout << grid->getNumberSets() << " ?= " << 0 << std::endl;
  assert(grid->getNumberAttributes() == 0);
  assert(grid->getNumberSets() == 0);

  // Insert Information
  shared_ptr<XdmfInformation> information =
    XdmfInformation::New("key", "value");
  grid->insert(information);

  return 0;
}
