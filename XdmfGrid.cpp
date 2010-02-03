/*
 * XdmfGrid.cpp
 *
 *  Created on: Jan 25, 2010
 *      Author: kleiter
 */

#include "XdmfGrid.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfTopology.hpp"

XdmfGrid::XdmfGrid()
{
	std::cout << "Created Grid " << this << std::endl;
	mGeometry = XdmfGeometry::New();
	mTopology = XdmfTopology::New();
}

XdmfGrid::~XdmfGrid()
{
  std::cout << "Deleted Grid " << this << std::endl;
}

std::string XdmfGrid::printSelf() const
{
	return "XdmfGrid containing a " + mGeometry->printSelf() + " and a " + mTopology->printSelf();
}
