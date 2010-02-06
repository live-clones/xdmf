#include "XdmfTopology.hpp"

int main(int argc, char* argv[])
{
	// Test != and == operators
	assert(XdmfTopologyType::Hexahedron == XdmfTopologyType::Hexahedron);
	assert(XdmfTopologyType::Hexahedron == XdmfTopologyType::Tetrahedron == false);
	assert(XdmfTopologyType::Hexahedron != XdmfTopologyType::Hexahedron == false);
	assert(XdmfTopologyType::Hexahedron != XdmfTopologyType::Tetrahedron);

	return 0;
}
