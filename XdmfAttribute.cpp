/*
 * XdmfAttribute.cpp
 *
 *  Created on: Jan 29, 2010
 *      Author: kleiter
 */

#include "XdmfAttribute.hpp"

XdmfAttribute::XdmfAttribute() :
	mName(""),
	mAttributeType(XdmfAttributeType::NoAttributeType()),
	mAttributeCenter(XdmfAttributeCenter::Grid())
{
	std::cout << "Created Attribute " << this << std::endl;
}

XdmfAttribute::~XdmfAttribute()
{
	std::cout << "Deleted Attribute " << this << std::endl;
}

std::string XdmfAttribute::ItemTag = "Attribute";

XdmfAttributeCenter XdmfAttribute::getAttributeCenter() const
{
	return mAttributeCenter;
}

XdmfAttributeType XdmfAttribute::getAttributeType() const
{
	return mAttributeType;
}

std::map<std::string, std::string> XdmfAttribute::getItemProperties() const
{
	std::map<std::string, std::string> attributeProperties;
	attributeProperties["Name"] = mName;
	mAttributeType.getProperties(attributeProperties);
	mAttributeCenter.getProperties(attributeProperties);
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

void XdmfAttribute::setAttributeCenter(const XdmfAttributeCenter & attributeCenter)
{
	mAttributeCenter = attributeCenter;
}

void XdmfAttribute::setAttributeType(const XdmfAttributeType & attributeType)
{
	mAttributeType = attributeType;
}

void XdmfAttribute::setName(const std::string & name)
{
	mName= name;
}
