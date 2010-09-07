#include "XdmfTopology.hpp"
#include "XdmfTopologyType.hpp"

void setPolyTopology(boost::shared_ptr<XdmfTopology> polyTop)
{
	polyTop->setType(XdmfTopologyType::Polygon(6));
}

int main(int, char *)
{
	// Test != and == operators
	assert(XdmfTopologyType::Hexahedron() == XdmfTopologyType::Hexahedron());
	assert(XdmfTopologyType::Hexahedron() == XdmfTopologyType::Tetrahedron() == false);
	assert(XdmfTopologyType::Hexahedron() != XdmfTopologyType::Hexahedron() == false);
	assert(XdmfTopologyType::Hexahedron() != XdmfTopologyType::Tetrahedron());

	boost::shared_ptr<XdmfTopology> top = XdmfTopology::New();
	assert(top->getType() == XdmfTopologyType::NoTopologyType());
	assert(top->getType()->getNodesPerElement() == 0);

	top->setType(XdmfTopologyType::Hexahedron());
	assert(top->getType() == XdmfTopologyType::Hexahedron());
	assert(top->getType()->getNodesPerElement() == 8);

	boost::shared_ptr<const XdmfTopologyType> polygon = XdmfTopologyType::Polygon(6);
	boost::shared_ptr<const XdmfTopologyType> polygon6 = XdmfTopologyType::Polygon(6);
	boost::shared_ptr<const XdmfTopologyType> polygon12 = XdmfTopologyType::Polygon(12);

	assert(polygon == polygon6);
	assert(polygon == polygon12 == false);

	boost::shared_ptr<XdmfTopology> polyTop = XdmfTopology::New();
	setPolyTopology(polyTop);
	assert(polyTop->getType()->getNodesPerElement() == 6);

	return 0;
}
