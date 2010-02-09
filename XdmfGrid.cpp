/*
 * XdmfGrid.cpp
 *
 *  Created on: Jan 25, 2010
 *      Author: kleiter
 */

#include "XdmfGrid.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfTopology.hpp"
#include "XdmfVisitor.hpp"
#include <sstream>

XdmfGrid::XdmfGrid() :
mGeometry(XdmfGeometry::New()),
mTopology(XdmfTopology::New()),
mName("Grid")
{
	std::cout << "Created Grid " << this << std::endl;
}

XdmfGrid::~XdmfGrid()
{
  std::cout << "Deleted Grid " << this << std::endl;
}


/**
 * Set the geometry associated with this grid.
 *
 * @param a smart pointer to the geometry to attach
 */
void setGeometry(boost::shared_ptr<const XdmfGeometry>);


boost::shared_ptr<XdmfGeometry> XdmfGrid::getGeometry()
{
	return mGeometry;
}

boost::shared_ptr<const XdmfGeometry> XdmfGrid::getGeometry() const
{
	return mGeometry;
}

void XdmfGrid::setGeometry(boost::shared_ptr<XdmfGeometry> geometry)
{
	mGeometry = geometry;
}

boost::shared_ptr<XdmfTopology> XdmfGrid::getTopology()
{
	return mTopology;
}

boost::shared_ptr<const XdmfTopology> XdmfGrid::getTopology() const
{
	return mTopology;
}

void XdmfGrid::setTopology(boost::shared_ptr<XdmfTopology> topology)
{
	mTopology = topology;
}

std::string XdmfGrid::getName() const
{
	return mName;
}

void XdmfGrid::setName(const std::string& name)
{
	mName= name;
}

const std::string XdmfGrid::printSelf() const
{
	return "XdmfGrid containing a " + mGeometry->printSelf() + " and a " + mTopology->printSelf();
}

void XdmfGrid::write(boost::shared_ptr<XdmfVisitor> visitor) const
{
	visitor->visit(this);
}
