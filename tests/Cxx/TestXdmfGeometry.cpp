#include "XdmfGeometry.hpp"
#include "XdmfGeometryType.hpp"

int main(int argc, char* argv[])
{
	// Test != and == operators
	assert(XdmfGeometryType::XYZ() == XdmfGeometryType::XYZ());
	assert(XdmfGeometryType::XYZ() == XdmfGeometryType::XY() == false);
	assert(XdmfGeometryType::XYZ() != XdmfGeometryType::XYZ() == false);
	assert(XdmfGeometryType::XYZ() != XdmfGeometryType::XY());

	boost::shared_ptr<XdmfGeometry> geometry = XdmfGeometry::New();
	assert(geometry->getType() == XdmfGeometryType::NoGeometryType());
	assert(geometry->getType()->getDimensions() == 0);
	geometry->setType(XdmfGeometryType::XYZ());
	assert(geometry->getType() == XdmfGeometryType::XYZ());
	assert(geometry->getType()->getDimensions() == 3);

	return 0;
}
