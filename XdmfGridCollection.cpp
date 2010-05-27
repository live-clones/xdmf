/*
 * XdmfGridCollection.cpp
 *
 *  Created on: Jan 25, 2010
 *      Author: kleiter
 */

#include "XdmfGridCollection.hpp"
#include "XdmfGridCollectionType.hpp"

XdmfGridCollection::XdmfGridCollection() :
	mCollectionType(XdmfGridCollectionType::NoCollectionType())
{
	mName = "Collection";
	std::cout << "Created Collection " << this << std::endl;
}

XdmfGridCollection::~XdmfGridCollection()
{
  std::cout << "Deleted Collection " << this << std::endl;
}

std::string XdmfGridCollection::ItemTag = "Grid";

boost::shared_ptr<XdmfGrid> XdmfGridCollection::getGrid(unsigned int index)
{
	if(index >= mGrids.size())
	{
		assert(false);
	}
	return mGrids[index];
}

boost::shared_ptr<const XdmfGrid> XdmfGridCollection::getGrid(unsigned int index) const
{
	if(index >= mGrids.size())
	{
		assert(false);
	}
	return mGrids[index];
}

boost::shared_ptr<const XdmfGridCollectionType> XdmfGridCollection::getGridCollectionType() const
{
	return mCollectionType;
}

std::map<std::string, std::string> XdmfGridCollection::getItemProperties() const
{
	std::map<std::string, std::string> collectionProperties;
	collectionProperties["Name"] = mName;
	collectionProperties["GridType"] = "Collection";
	mCollectionType->getProperties(collectionProperties);
	return collectionProperties;
}

unsigned int XdmfGridCollection::getNumberOfGrids() const
{
	return mGrids.size();
}

void XdmfGridCollection::insert(boost::shared_ptr<XdmfGrid> grid)
{
	mGrids.push_back(grid);
}

void XdmfGridCollection::populateItem(const std::map<std::string, std::string> & itemProperties, std::vector<boost::shared_ptr<XdmfItem> > & childItems)
{
	mCollectionType = XdmfGridCollectionType::New(itemProperties);
	for(std::vector<boost::shared_ptr<XdmfItem> >::const_iterator iter = childItems.begin(); iter != childItems.end(); ++iter)
	{
		if(boost::shared_ptr<XdmfGrid> grid = boost::shared_dynamic_cast<XdmfGrid>(*iter))
		{
			this->insert(grid);
		}
		else
		{
			assert(false);
		}
	}
	XdmfGrid::populateItem(itemProperties, childItems);
}

void XdmfGridCollection::removeGrid(const unsigned int index)
{
	if(index >= mGrids.size())
	{
		assert(false);
	}
	mGrids.erase(mGrids.begin() + index);
}

void XdmfGridCollection::setGridCollectionType(const boost::shared_ptr<const XdmfGridCollectionType> collectionType)
{
	mCollectionType = collectionType;
}

void XdmfGridCollection::traverse(boost::shared_ptr<XdmfBaseVisitor> visitor)
{
	for(std::vector<boost::shared_ptr<XdmfGrid> >::const_iterator iter = mGrids.begin(); iter != mGrids.end(); ++iter)
	{
		(*iter)->accept(visitor);
	}
}
