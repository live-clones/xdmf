#include "XdmfDomain.hpp"

int main(int, char **)
{
	shared_ptr<XdmfGeometry> exampleGeometry = XdmfGeometry::New();

	if (exampleGeometry->getType() == XdmfGeometry::XYZ())
	{
		//do whatever is to be done if the geometry is xyz
	}

	unsigned int exampleDimensions = XdmfGeometryType::XYZ()->getDimensions();
	std::string exampleName = XdmfGeometryType::XYZ()->getName();

	return 0;
}
