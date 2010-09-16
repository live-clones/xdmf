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
	XdmfDomain(),
	XdmfGrid("Collection"),
	mType(XdmfGridCollectionType::NoCollectionType())
{
}

XdmfGridCollection::~XdmfGridCollection()
{
}

const std::string XdmfGridCollection::ItemTag = "Grid";

std::map<std::string, std::string> XdmfGridCollection::getItemProperties() const
{
	std::map<std::string, std::string> collectionProperties = XdmfGrid::getItemProperties();
	collectionProperties["GridType"] = "Collection";
	mType->getProperties(collectionProperties);
	return collectionProperties;
}

std::string XdmfGridCollection::getItemTag() const
{
	return ItemTag;
}

boost::shared_ptr<const XdmfGridCollectionType> XdmfGridCollection::getType() const
{
	return mType;
}

void XdmfGridCollection::insert(const boost::shared_ptr<XdmfInformation> information)
{
	XdmfItem::insert(information);
}

void XdmfGridCollection::populateItem(const std::map<std::string, std::string> & itemProperties, std::vector<boost::shared_ptr<XdmfItem> > & childItems, const XdmfCoreReader * const reader)
{
	mType = XdmfGridCollectionType::New(itemProperties);
	XdmfDomain::populateItem(itemProperties, childItems, reader);
	mInformations.clear();
	XdmfGrid::populateItem(itemProperties, childItems, reader);
}

void XdmfGridCollection::setType(const boost::shared_ptr<const XdmfGridCollectionType> type)
{
	mType = type;
}

void XdmfGridCollection::traverse(const boost::shared_ptr<XdmfBaseVisitor> visitor)
{
	XdmfGrid::traverse(visitor);

	// Only write XdmfInformations once (deal with diamond inheritance)
	std::vector<boost::shared_ptr<XdmfInformation> > informations;
	informations.swap(mInformations);
	XdmfDomain::traverse(visitor);
	informations.swap(mInformations);
}
