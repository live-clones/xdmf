#include "XdmfGeometry.hpp"
#include "XdmfGeometryType.hpp"
#include "XdmfSharedPtr.hpp"

int main(int, char **)
{
  // Test != and == operators
  assert(XdmfGeometryType::XYZ() == XdmfGeometryType::XYZ());
  assert((XdmfGeometryType::XYZ() == XdmfGeometryType::XY()) == false);
  assert((XdmfGeometryType::XYZ() != XdmfGeometryType::XYZ()) == false);
  assert(XdmfGeometryType::XYZ() != XdmfGeometryType::XY());

  shared_ptr<XdmfGeometry> geometry = XdmfGeometry::New();
  assert(geometry->getType() == XdmfGeometryType::NoGeometryType());
  assert(geometry->getType()->getDimensions() == 0);
  assert(geometry->getType()->getName().compare("None") == 0);
  geometry->setType(XdmfGeometryType::XYZ());
  assert(geometry->getType() == XdmfGeometryType::XYZ());
  assert(geometry->getType()->getDimensions() == 3);
  assert(geometry->getType()->getName().compare("XYZ") == 0);

  return 0;
}
