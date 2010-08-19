/*
 * XdmfGridCollection.cpp
 *
 *  Created on: Jan 25, 2010
 *      Author: kleiter
 */

#include "XdmfGridCollection.hpp"
#include "XdmfGridCollectionType.hpp"

boost::shared_ptr<XdmfGridCollection> XdmfGridCollection::New()
{
	boost::shared_ptr<XdmfGridCollection> p(new XdmfGridCollection());
	return p;
};

XdmfGridCollection::XdmfGridCollection() :
	XdmfGrid(),
	mCollectionType(XdmfGridCollectionType::NoCollectionType())
{
	mGeometry = boost::shared_ptr<XdmfGeometry>();
	mName = "Collection";
	mTopology = boost::shared_ptr<XdmfTopology>();
}

XdmfGridCollection::~XdmfGridCollection()
{
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

void XdmfGridCollection::populateItem(const std::map<std::string, std::string> & itemProperties, std::vector<boost::shared_ptr<XdmfItem> > & childItems, const XdmfCoreReader * const reader)
{
	mCollectionType = XdmfGridCollectionType::New(itemProperties);
	XdmfDomain::populateItem(itemProperties, childItems, reader);
	XdmfGrid::populateItem(itemProperties, childItems, reader);
}

void XdmfGridCollection::setType(const boost::shared_ptr<const XdmfGridCollectionType> collectionType)
{
	mCollectionType = collectionType;
}

void XdmfGridCollection::traverse(const boost::shared_ptr<XdmfBaseVisitor> visitor)
{
	XdmfGrid::traverse(visitor);
	XdmfDomain::traverse(visitor);
}
