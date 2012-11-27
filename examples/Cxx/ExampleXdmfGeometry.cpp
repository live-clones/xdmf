#include "XdmfGeometry.hpp"
#include "XdmfGeometryType.hpp"

int main(int, char **)
{
	shared_ptr<XdmfGeometry> exampleGeometry = XdmfGeometry::New();

	exampleGeometry->setType(XdmfGeometryType::XYZ());

	shared_ptr<const XdmfGeometryType> exampleType = exampleGeometry->getType();

	unsigned int numPoints = exampleGeometry->getNumberPoints();

	return 0;
}
