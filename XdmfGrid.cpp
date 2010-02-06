/*
 * XdmfGrid.cpp
 *
 *  Created on: Jan 25, 2010
 *      Author: kleiter
 */

#include "XdmfGrid.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfTopology.hpp"

XdmfGrid::XdmfGrid() :
mGeometry(XdmfGeometry::New()),
mTopology(XdmfTopology::New())
{
	std::cout << "Created Grid " << this << std::endl;
}

XdmfGrid::~XdmfGrid()
{
  std::cout << "Deleted Grid " << this << std::endl;
}

const std::string XdmfGrid::printSelf() const
{
	return "XdmfGrid containing a " + mGeometry->printSelf() + " and a " + mTopology->printSelf();
}
