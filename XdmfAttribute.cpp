/*
 * XdmfAttribute.cpp
 *
 *  Created on: Jan 29, 2010
 *      Author: kleiter
 */

#include "XdmfAttribute.hpp"
#include "XdmfAttributeCenter.hpp"
#include "XdmfAttributeType.hpp"

XdmfAttribute::XdmfAttribute() :
	mName(""),
	mAttributeType(XdmfAttributeType::NoAttributeType()),
	mAttributeCenter(XdmfAttributeCenter::Grid())
{
}

XdmfAttribute::~XdmfAttribute()
{
}

const std::string XdmfAttribute::ItemTag = "Attribute";

boost::shared_ptr<const XdmfAttributeCenter> XdmfAttribute::getCenter() const
{
	return mAttributeCenter;
}

std::map<std::string, std::string> XdmfAttribute::getItemProperties() const
{
	std::map<std::string, std::string> attributeProperties;
	attributeProperties["Name"] = mName;
	mAttributeType->getProperties(attributeProperties);
	mAttributeCenter->getProperties(attributeProperties);
	return attributeProperties;
}

std::string XdmfAttribute::getItemTag() const
{
	return ItemTag;
}

std::string XdmfAttribute::getName() const
{
	return mName;
}

boost::shared_ptr<const XdmfAttributeType> XdmfAttribute::getType() const
{
	return mAttributeType;
}

void XdmfAttribute::populateItem(const std::map<std::string, std::string> & itemProperties, std::vector<boost::shared_ptr<XdmfItem> > & childItems)
{
	std::map<std::string, std::string>::const_iterator name = itemProperties.find("Name");
	if(name != itemProperties.end())
	{
		mName = name->second;
	}
	else
	{
		assert(false);
	}
	mAttributeCenter = XdmfAttributeCenter::New(itemProperties);
	mAttributeType = XdmfAttributeType::New(itemProperties);

	XdmfDataItem::populateItem(itemProperties, childItems);
}

void XdmfAttribute::setCenter(const boost::shared_ptr<const XdmfAttributeCenter> attributeCenter)
{
	mAttributeCenter = attributeCenter;
}

void XdmfAttribute::setName(const std::string & name)
{
	mName= name;
}

void XdmfAttribute::setType(const boost::shared_ptr<const XdmfAttributeType> attributeType)
{
	mAttributeType = attributeType;
}
