/*
 * XdmfGrid.cpp
 *
 *  Created on: Jan 25, 2010
 *      Author: kleiter
 */

#include "XdmfDomain.hpp"
#include "XdmfVisitor.hpp"

XdmfDomain::XdmfDomain()
{
	std::cout << "Created Domain " << this << std::endl;
}

XdmfDomain::~XdmfDomain()
{
  std::cout << "Deleted Domain " << this << std::endl;
}

void XdmfDomain::insert(boost::shared_ptr<XdmfGrid> grid)
{
	mGrids.push_back(grid);
}

boost::shared_ptr<XdmfGrid> XdmfDomain::getGrid(unsigned int index)
{
	if(index >= mGrids.size())
	{
		assert(false);
		// Out of range --- should we throw exceptions?
	}
	return mGrids[index];
}

boost::shared_ptr<const XdmfGrid> XdmfDomain::getGrid(unsigned int index) const
{
	if(index >= mGrids.size())
	{
		assert(false);
		// Out of range --- should we throw exceptions?
	}
	return mGrids[index];
}

unsigned int XdmfDomain::getNumberOfGrids() const
{
	return mGrids.size();
}

std::string XdmfDomain::printSelf() const
{
	return "XdmfDomain";
}

void XdmfDomain::write(boost::shared_ptr<XdmfVisitor> visitor) const
{
	visitor->visit(this);
}
