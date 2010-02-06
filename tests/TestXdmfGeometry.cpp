#include "XdmfGeometry.hpp"

int main(int argc, char* argv[])
{
	// Test != and == operators
	assert(XdmfGeometryType::XYZ == XdmfGeometryType::XYZ);
	assert(XdmfGeometryType::XYZ == XdmfGeometryType::XY == false);
	assert(XdmfGeometryType::XYZ != XdmfGeometryType::XYZ == false);
	assert(XdmfGeometryType::XYZ != XdmfGeometryType::XY);

	boost::shared_ptr<XdmfGeometry> geom = XdmfGeometry::New();
	assert(geom->getGeometryType() == XdmfGeometryType::NoGeometry);
	assert(geom->getGeometryType().name.compare("NONE") == 0);
	assert(geom->getGeometryType().dimensions == 0);
	geom->setGeometryType(XdmfGeometryType::XYZ);
	assert(geom->getGeometryType() == XdmfGeometryType::XYZ);
	assert(geom->getGeometryTypeAsString().compare("XYZ") == 0);
	assert(geom->getDimensions() == 3);

	return 0;
}
