/*
 * XdmfGrid.cpp
 *
 *  Created on: Jan 29, 2010
 *      Author: kleiter
 */

#include "XdmfTopology.hpp"

XdmfTopology::XdmfTopology()
{
	std::cout << "Created Topology " << this << std::endl;
}

XdmfTopology::~XdmfTopology()
{
	std::cout << "Deleted Topology " << this << std::endl;
}

std::string XdmfTopology::printSelf() const
{
	return "XdmfTopology";
}
