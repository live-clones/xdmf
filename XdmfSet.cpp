/*
 * XdmfSet.cpp
 *
 *  Created on: Jan 29, 2010
 *      Author: kleiter
 */

#include "XdmfSet.hpp"

XdmfSet::XdmfSet() :
	mSetType(XdmfSetType::NoSetType()),
	mName("")
{
	std::cout << "Created Set " << this << std::endl;
}

XdmfSet::~XdmfSet()
{
	std::cout << "Deleted Set " << this << std::endl;
}

std::string XdmfSet::ItemTag = "Set";

std::string XdmfSet::getName() const
{
	return mName;
}

XdmfSetType XdmfSet::getSetType() const
{
	return mSetType;
}

std::map<std::string, std::string> XdmfSet::getItemProperties() const
{
	std::map<std::string, std::string> setProperties;
	setProperties["Name"] = mName;
	mSetType.getProperties(setProperties);
	return setProperties;
}

std::string XdmfSet::getItemTag() const
{
	return ItemTag;
}

void XdmfSet::populateItem(const std::map<std::string, std::string> & itemProperties, std::vector<boost::shared_ptr<XdmfItem> > & childItems)
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
	mSetType = XdmfSetType::New(itemProperties);
	XdmfDataItem::populateItem(itemProperties, childItems);
}

void XdmfSet::setName(const std::string & name)
{
	mName= name;
}

void XdmfSet::setSetType(const XdmfSetType & setType)
{
	mSetType = setType;
}
