/*
 * XdmfTime.cpp
 *
 *  Created on: Jan 29, 2010
 *      Author: kleiter
 */

#include <sstream>
#include "XdmfTime.hpp"

XdmfTime::XdmfTime(const double & value) :
	mValue(value)
{
	std::cout << "Created Time " << this << std::endl;
}

XdmfTime::~XdmfTime()
{
	std::cout << "Deleted Time " << this << std::endl;
}

const std::string XdmfTime::ItemTag = "Time";

std::map<std::string, std::string> XdmfTime::getItemProperties() const
{
	std::map<std::string, std::string> timeProperties;
	std::stringstream value;
	value << mValue;
	timeProperties["Value"] = value.str();
	return timeProperties;
}

std::string XdmfTime::getItemTag() const
{
	return ItemTag;
}

double XdmfTime::getValue() const
{
	return mValue;
}

void XdmfTime::populateItem(const std::map<std::string, std::string> & itemProperties, std::vector<boost::shared_ptr<XdmfItem> > & childItems)
{
	std::map<std::string, std::string>::const_iterator value = itemProperties.find("Value");
	if(value != itemProperties.end())
	{
		mValue = atof(value->second.c_str());
	}
	else
	{
		assert(false);
	}
}

void XdmfTime::setValue(const double & value)
{
	mValue = value;
}
