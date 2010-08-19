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
	mCollectionType(XdmfGridCollectionType::NoCollectionType()),
	mName("Collection")
{
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

std::string XdmfGridCollection::getName() const
{
	return mName;
}

boost::shared_ptr<const XdmfGridCollectionType> XdmfGridCollection::getType() const
{
	return mCollectionType;
}

void XdmfGridCollection::populateItem(const std::map<std::string, std::string> & itemProperties, std::vector<boost::shared_ptr<XdmfItem> > & childItems, const XdmfCoreReader * const reader)
{
	mCollectionType = XdmfGridCollectionType::New(itemProperties);
	std::map<std::string, std::string>::const_iterator name = itemProperties.find("Name");
	if(name != itemProperties.end())
	{
		mName = name->second;
	}
	else
	{
		mName = "";
	}
	XdmfDomain::populateItem(itemProperties, childItems, reader);
}

void XdmfGridCollection::setName(const std::string & name)
{
	mName = name;
}

void XdmfGridCollection::setType(const boost::shared_ptr<const XdmfGridCollectionType> collectionType)
{
	mCollectionType = collectionType;
}
