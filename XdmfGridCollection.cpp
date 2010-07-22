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

std::map<std::string, std::string> XdmfGridCollection::getItemProperties() const
{
	std::map<std::string, std::string> collectionProperties;
	collectionProperties["Name"] = mName;
	collectionProperties["GridType"] = "Collection";
	mCollectionType->getProperties(collectionProperties);
	return collectionProperties;
}

std::string XdmfGridCollection::getItemTag() const
{
	return ItemTag;
}

boost::shared_ptr<const XdmfGridCollectionType> XdmfGridCollection::getType() const
{
	return mCollectionType;
}

void XdmfGridCollection::populateItem(const std::map<std::string, std::string> & itemProperties, std::vector<boost::shared_ptr<XdmfItem> > & childItems)
{
	mCollectionType = XdmfGridCollectionType::New(itemProperties);
	XdmfDomain::populateItem(itemProperties, childItems);
	XdmfGrid::populateItem(itemProperties, childItems);
}

void XdmfGridCollection::setType(const boost::shared_ptr<const XdmfGridCollectionType> collectionType)
{
	mCollectionType = collectionType;
}

void XdmfGridCollection::traverse(const boost::shared_ptr<XdmfBaseVisitor> visitor) const
{
	XdmfGrid::traverse(visitor);
	XdmfDomain::traverse(visitor);
}
