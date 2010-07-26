/*
 * XdmfInformation.cpp
 *
 *  Created on: Jan 29, 2010
 *      Author: kleiter
 */

#include "XdmfInformation.hpp"

boost::shared_ptr<XdmfInformation> XdmfInformation::New()
{
	boost::shared_ptr<XdmfInformation> p(new XdmfInformation());
	return p;
};

XdmfInformation::XdmfInformation() :
	mKey(""),
	mValue("")
{
}

XdmfInformation::~XdmfInformation()
{
}

const std::string XdmfInformation::ItemTag = "Information";

std::map<std::string, std::string> XdmfInformation::getItemProperties() const
{
	std::map<std::string, std::string> informationProperties;
	informationProperties["Name"] = mKey;
	informationProperties["Value"] = mValue;
	return informationProperties;
}

std::string XdmfInformation::getItemTag() const
{
	return ItemTag;
}

std::string XdmfInformation::getKey() const
{
	return mKey;
}

std::string XdmfInformation::getValue() const
{
	return mValue;
}

void XdmfInformation::populateItem(const std::map<std::string, std::string> & itemProperties, std::vector<boost::shared_ptr<XdmfItem> > & childItems)
{
	std::map<std::string, std::string>::const_iterator key = itemProperties.find("Name");
	if(key != itemProperties.end())
	{
		mKey = key->second;
	}
	else
	{
		assert(false);
	}
	std::map<std::string, std::string>::const_iterator value = itemProperties.find("Value");
	if(value != itemProperties.end())
	{
		mValue = value->second;
	}
	else
	{
		assert(false);
	}
}

void XdmfInformation::setKey(const std::string & key)
{
	mKey= key;
}

void XdmfInformation::setValue(const std::string & value)
{
	mValue= value;
}
