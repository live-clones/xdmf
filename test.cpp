/*
 * test.cc
 *
 *  Created on: Jan 29, 2010
 *      Author: kleiter
 */

#include "XdmfGrid.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfTopology.hpp"

#include <iostream>

int main(int argc, char* argv[])
{
	boost::shared_ptr<XdmfGrid> grid = XdmfGrid::New();
	std::cout << *grid << std::endl;

	boost::shared_ptr<XdmfObject> obj = XdmfGrid::New();
	std::cout << *obj << std::endl;

	boost::shared_ptr<XdmfTopology> top = XdmfTopology::New();
	std::cout << *top << std::endl;
	std::cout << top->getTopologyTypeAsString() << std::endl;
	top->setTopologyType(XdmfTopologyType::Hexahedron());
	std::cout << top->getTopologyTypeAsString() << std::endl;

	std::cout << top->getTopologyType().getName() << std::endl;
}
