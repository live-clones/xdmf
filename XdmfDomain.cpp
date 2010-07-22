/*
 * XdmfGrid.cpp
 *
 *  Created on: Jan 25, 2010
 *      Author: kleiter
 */

#include "XdmfDomain.hpp"
#include "XdmfGrid.hpp"
#include "XdmfGridCollection.hpp"

XdmfDomain::XdmfDomain()
{
	std::cout << "Created Domain " << this << std::endl;
}

XdmfDomain::~XdmfDomain()
{
  std::cout << "Deleted Domain " << this << std::endl;
}

const std::string XdmfDomain::ItemTag = "Domain";

boost::shared_ptr<XdmfGrid> XdmfDomain::getGrid(const unsigned int index)
{
	return boost::const_pointer_cast<XdmfGrid>(static_cast<const XdmfDomain &>(*this).getGrid(index));
}

boost::shared_ptr<const XdmfGrid> XdmfDomain::getGrid(const unsigned int index) const
{
	if(index >= mGrids.size())
	{
		assert(false);
	}
	return mGrids[index];
}

boost::shared_ptr<XdmfGridCollection> XdmfDomain::getGridCollection(const unsigned int index)
{
	return boost::const_pointer_cast<XdmfGridCollection>(static_cast<const XdmfDomain &>(*this).getGridCollection(index));
}

boost::shared_ptr<const XdmfGridCollection> XdmfDomain::getGridCollection(const unsigned int index) const
{
	if(index >= mGridCollections.size())
	{
		assert(false);
	}
	return mGridCollections[index];
}

std::map<std::string, std::string> XdmfDomain::getItemProperties() const
{
	std::map<std::string, std::string> domainProperties;
	return domainProperties;
}

std::string XdmfDomain::getItemTag() const
{
	return ItemTag;
}

unsigned int XdmfDomain::getNumberGrids() const
{
	return mGrids.size();
}

unsigned int XdmfDomain::getNumberGridCollections() const
{
	return mGridCollections.size();
}

void XdmfDomain::insert(const boost::shared_ptr<XdmfGrid> grid)
{
	mGrids.push_back(grid);
}

void XdmfDomain::insert(const boost::shared_ptr<XdmfGridCollection> gridCollection)
{
	mGridCollections.push_back(gridCollection);
}

void XdmfDomain::populateItem(const std::map<std::string, std::string> & itemProperties, std::vector<boost::shared_ptr<XdmfItem> > & childItems)
{
	for(std::vector<boost::shared_ptr<XdmfItem> >::const_iterator iter = childItems.begin(); iter != childItems.end(); ++iter)
	{
		if(boost::shared_ptr<XdmfGridCollection> gridCollection = boost::shared_dynamic_cast<XdmfGridCollection>(*iter))
		{
			this->insert(gridCollection);
		}
		else if(boost::shared_ptr<XdmfGrid> grid = boost::shared_dynamic_cast<XdmfGrid>(*iter))
		{
			this->insert(grid);
		}
	}
}

void XdmfDomain::removeGrid(const unsigned int index)
{
	if(index >= mGrids.size())
	{
		assert(false);
	}
	mGrids.erase(mGrids.begin() + index);
}

void XdmfDomain::removeGridCollection(const unsigned int index)
{
	if(index >= mGridCollections.size())
	{
		assert(false);
	}
	mGridCollections.erase(mGridCollections.begin() + index);
}

void XdmfDomain::traverse(const boost::shared_ptr<XdmfBaseVisitor> visitor) const
{
	for(std::vector<boost::shared_ptr<XdmfGrid> >::const_iterator iter = mGrids.begin(); iter != mGrids.end(); ++iter)
	{
		(*iter)->accept(visitor);
	}
	for(std::vector<boost::shared_ptr<XdmfGridCollection> >::const_iterator iter = mGridCollections.begin(); iter != mGridCollections.end(); ++iter)
	{
		(*iter)->accept(visitor);
	}
}
