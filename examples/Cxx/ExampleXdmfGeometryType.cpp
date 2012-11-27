#include "XdmfDomain.hpp"

int main(int, char **)
{
	unsigned int exampleDimensions = XdmfGeometryType::XYZ()->getDimensions();
	std::string exampleName = XdmfGeometryType::XYZ()->getName();

	return 0;
}
