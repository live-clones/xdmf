#include "XdmfGeometry.hpp"

int main(int argc, char* argv[])
{
	// Test != and == operators
	assert(XdmfGeometryType::XYZ() == XdmfGeometryType::XYZ());
	assert(XdmfGeometryType::XYZ() == XdmfGeometryType::XY() == false);
	assert(XdmfGeometryType::XYZ() != XdmfGeometryType::XYZ() == false);
	assert(XdmfGeometryType::XYZ() != XdmfGeometryType::XY());

	boost::shared_ptr<XdmfGeometry> geom = XdmfGeometry::New();
	assert(geom->getGeometryType() == XdmfGeometryType::NoGeometryType());
	assert(geom->getGeometryType().getDimensions() == 0);
	geom->setGeometryType(XdmfGeometryType::XYZ());
	assert(geom->getGeometryType() == XdmfGeometryType::XYZ());
	assert(geom->getGeometryType().getDimensions() == 3);

	return 0;
}
