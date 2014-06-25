#include "XdmfTopology.hpp"
#include "XdmfTopologyType.hpp"

void setPolyTopology(shared_ptr<XdmfTopology> polyTop)
{
  polyTop->setType(XdmfTopologyType::Polygon(6));
}

int main(int, char **)
{
  // Test != and == operators
  std::cout << XdmfTopologyType::Hexahedron() << " ?= " << XdmfTopologyType::Hexahedron() << std::endl;
  std::cout << XdmfTopologyType::Hexahedron() << " ?!= " << XdmfTopologyType::Tetrahedron() << std::endl;
  assert(XdmfTopologyType::Hexahedron() == XdmfTopologyType::Hexahedron());
  assert((XdmfTopologyType::Hexahedron() ==
         XdmfTopologyType::Tetrahedron()) == false);
  assert((XdmfTopologyType::Hexahedron() !=
         XdmfTopologyType::Hexahedron()) == false);
  assert(XdmfTopologyType::Hexahedron() != XdmfTopologyType::Tetrahedron());

  shared_ptr<XdmfTopology> top = XdmfTopology::New();

  std::cout << top->getType() << " ?= " << XdmfTopologyType::NoTopologyType() << std::endl;
  std::cout << top->getType()->getNodesPerElement() << " ?= " << 0 << std::endl;
  std::cout << top->getType()->getName() << " ?= NoTopology" << std::endl;

  assert(top->getType() == XdmfTopologyType::NoTopologyType());
  assert(top->getType()->getNodesPerElement() == 0);
  assert(top->getType()->getName().compare("NoTopology") == 0);

  top->setType(XdmfTopologyType::Hexahedron());

  std::cout << top->getType() << " ?= " << XdmfTopologyType::Hexahedron() << std::endl;
  std::cout << top->getType()->getNodesPerElement() << " ?= " << 9 << std::endl;
  std::cout << top->getType()->getName() << " ?= Hexahedron" << std::endl;

  assert(top->getType() == XdmfTopologyType::Hexahedron());
  assert(top->getType()->getNodesPerElement() == 8);
  assert(top->getType()->getName().compare("Hexahedron") == 0);

  shared_ptr<const XdmfTopologyType> polygon = XdmfTopologyType::Polygon(6);
  shared_ptr<const XdmfTopologyType> polygon6 = XdmfTopologyType::Polygon(6);
  shared_ptr<const XdmfTopologyType> polygon12 = XdmfTopologyType::Polygon(12);

  std::cout << polygon << " ?= " << polygon6 << std::endl;
  std::cout << polygon << " ?!= " << polygon12 << std::endl;

  assert(polygon == polygon6);
  assert((polygon == polygon12) == false);

  shared_ptr<XdmfTopology> polyTop = XdmfTopology::New();
  setPolyTopology(polyTop);
  std::cout << polyTop->getType()->getNodesPerElement() << " ?= " << 6 << std::endl;
  assert(polyTop->getType()->getNodesPerElement() == 6);

  return 0;
}
