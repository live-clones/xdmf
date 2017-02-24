#include "XdmfGeometry.hpp"
#include "XdmfGeometryType.hpp"
#include "XdmfSharedPtr.hpp"

#include <iostream>
#include <assert.h>

int main(int, char **)
{
  // Test != and == operators
  std::cout << XdmfGeometryType::XYZ() << " ?= " << XdmfGeometryType::XYZ() << std::endl;
  std::cout << XdmfGeometryType::XYZ() << " ?!= " << XdmfGeometryType::XY() << std::endl;
  assert(XdmfGeometryType::XYZ() == XdmfGeometryType::XYZ());
  assert((XdmfGeometryType::XYZ() == XdmfGeometryType::XY()) == false);
  assert((XdmfGeometryType::XYZ() != XdmfGeometryType::XYZ()) == false);
  assert(XdmfGeometryType::XYZ() != XdmfGeometryType::XY());

  shared_ptr<XdmfGeometry> geometry = XdmfGeometry::New();
  std::cout << geometry->getType() << " ?= " << XdmfGeometryType::NoGeometryType() << std::endl;
  std::cout << geometry->getType()->getDimensions() << " ?= " << 0 << std::endl;
  std::cout << geometry->getType()->getName() << " ?= None" << std::endl;
  assert(geometry->getType() == XdmfGeometryType::NoGeometryType());
  assert(geometry->getType()->getDimensions() == 0);
  assert(geometry->getType()->getName().compare("None") == 0);
  geometry->setType(XdmfGeometryType::XYZ());
  std::cout << geometry->getType() << " ?= " << XdmfGeometryType::XYZ() << std::endl;
  std::cout << geometry->getType()->getDimensions() << " ?= " << 3 << std::endl;
  std::cout << geometry->getType()->getName() << " ?= XYZ" << std::endl;
  assert(geometry->getType() == XdmfGeometryType::XYZ());
  assert(geometry->getType()->getDimensions() == 3);
  assert(geometry->getType()->getName().compare("XYZ") == 0);

  return 0;
}
