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

const std::string XdmfGridCollection::ItemTag = "Grid";

boost::shared_ptr<XdmfGrid> XdmfGridCollection::getGrid(const unsigned int index)
{
	return boost::const_pointer_cast<XdmfGrid>(static_cast<const XdmfGridCollection &>(*this).getGrid(index));
}

boost::shared_ptr<const XdmfGrid> XdmfGridCollection::getGrid(const unsigned int index) const
{
	if(index >= mGrids.size())
	{
		assert(false);
	}
	return mGrids[index];
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

boost::shared_ptr<const XdmfGridCollectionType> XdmfGridCollection::getType() const
{
	return mCollectionType;
}

void XdmfGridCollection::insert(const boost::shared_ptr<XdmfGrid> grid)
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

void XdmfGridCollection::setType(const boost::shared_ptr<const XdmfGridCollectionType> collectionType)
{
	mCollectionType = collectionType;
}

void XdmfGridCollection::traverse(const boost::shared_ptr<XdmfBaseVisitor> visitor) const
{
	for(std::vector<boost::shared_ptr<XdmfGrid> >::const_iterator iter = mGrids.begin(); iter != mGrids.end(); ++iter)
	{
		(*iter)->accept(visitor);
	}
}
