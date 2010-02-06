#include "XdmfTopology.hpp"

int main(int argc, char* argv[])
{
	// Test != and == operators
	assert(XdmfTopologyType::Hexahedron == XdmfTopologyType::Hexahedron);
	assert(XdmfTopologyType::Hexahedron == XdmfTopologyType::Tetrahedron == false);
	assert(XdmfTopologyType::Hexahedron != XdmfTopologyType::Hexahedron == false);
	assert(XdmfTopologyType::Hexahedron != XdmfTopologyType::Tetrahedron);

	boost::shared_ptr<XdmfTopology> top = XdmfTopology::New();
	assert(top->getTopologyType() == XdmfTopologyType::NoTopology);
	assert(top->getTopologyType().name.compare("NoTopology") == 0);
	assert(top->getTopologyType().nodesPerElement == 0);
	top->setTopologyType(XdmfTopologyType::Hexahedron);
	assert(top->getTopologyType() == XdmfTopologyType::Hexahedron);
	assert(top->getTopologyTypeAsString().compare("Hexahedron") == 0);
	assert(top->getNodesPerElement() == 8);

	return 0;
}
