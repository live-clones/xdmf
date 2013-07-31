#include "XdmfDomain.hpp"

int main(int, char **)
{
	//#initialization begin

	shared_ptr<XdmfGeometry> exampleGeometry = XdmfGeometry::New();

	//#initialization end

	//#getType begin

	if (exampleGeometry->getType() == XdmfGeometry::XYZ())
	{
		//do whatever is to be done if the geometry is xyz
	}

	//#getType end

	//#getDimensions begin

	unsigned int exampleDimensions = XdmfGeometryType::XYZ()->getDimensions();

	//#getDimensions end

	//#getName begin

	std::string exampleName = XdmfGeometryType::XYZ()->getName();

	//#getName end

	return 0;
}
