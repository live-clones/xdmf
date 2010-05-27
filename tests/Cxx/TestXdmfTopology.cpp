#include "XdmfTopology.hpp"
#include "XdmfTopologyType.hpp"

void setPolyTopology(boost::shared_ptr<XdmfTopology> polyTop)
{
	polyTop->setTopologyType(XdmfTopologyType::Polygon(6));
}

int main(int argc, char* argv[])
{
	// Test != and == operators
	assert(XdmfTopologyType::Hexahedron() == XdmfTopologyType::Hexahedron());
	assert(XdmfTopologyType::Hexahedron() == XdmfTopologyType::Tetrahedron() == false);
	assert(XdmfTopologyType::Hexahedron() != XdmfTopologyType::Hexahedron() == false);
	assert(XdmfTopologyType::Hexahedron() != XdmfTopologyType::Tetrahedron());

	boost::shared_ptr<XdmfTopology> top = XdmfTopology::New();
	assert(top->getTopologyType() == XdmfTopologyType::NoTopologyType());
	assert(top->getTopologyType()->getNodesPerElement() == 0);

	top->setTopologyType(XdmfTopologyType::Hexahedron());
	assert(top->getTopologyType() == XdmfTopologyType::Hexahedron());
	assert(top->getTopologyType()->getNodesPerElement() == 8);

	boost::shared_ptr<const XdmfTopologyType> polygon = XdmfTopologyType::Polygon(6);
	boost::shared_ptr<const XdmfTopologyType> polygon6 = XdmfTopologyType::Polygon(6);
	boost::shared_ptr<const XdmfTopologyType> polygon12 = XdmfTopologyType::Polygon(12);

	assert(polygon == polygon6);
	assert(polygon == polygon12 == false);

	boost::shared_ptr<XdmfTopology> polyTop = XdmfTopology::New();
	setPolyTopology(polyTop);
	assert(polyTop->getTopologyType()->getNodesPerElement() == 6);

	return 0;
}
