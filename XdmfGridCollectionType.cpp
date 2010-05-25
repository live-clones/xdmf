/*
 * XdmfGridCollectionType.cpp
 *
 *  Created on: Jan 29, 2010
 *      Author: kleiter
 */

#include "XdmfGridCollectionType.hpp"

// Supported XdmfGridCollectionTypes
XdmfGridCollectionType XdmfGridCollectionType::NoCollectionType()
{
	return XdmfGridCollectionType("None");
}

XdmfGridCollectionType XdmfGridCollectionType::Spatial()
{
	return XdmfGridCollectionType("Spatial");
}

XdmfGridCollectionType XdmfGridCollectionType::Temporal()
{
	return XdmfGridCollectionType("Temporal");
}

XdmfGridCollectionType::XdmfGridCollectionType(const std::string & name) :
	mName(name)
{
}

XdmfGridCollectionType::XdmfGridCollectionType(const XdmfGridCollectionType & collectionType):
	mName(collectionType.mName)
{
}

XdmfGridCollectionType XdmfGridCollectionType::New(const std::map<std::string, std::string> & itemProperties)
{
	std::map<std::string, std::string>::const_iterator type = itemProperties.find("CollectionType");
	if(type != itemProperties.end())
	{
		const std::string typeVal = type->second;
		if(typeVal.compare("None") == 0)
		{
			return NoCollectionType();
		}
		else if(typeVal.compare("Spatial") == 0)
		{
			return Spatial();
		}
		else if(typeVal.compare("Temporal") == 0)
		{
			return Temporal();
		}
		else
		{
			assert(false);
		}
	}
	assert(false);
}

XdmfGridCollectionType & XdmfGridCollectionType::operator=(const XdmfGridCollectionType & collectionType)
{
	if(this != &collectionType)
	{
		mName = collectionType.mName;
	}
	return *this;
}

bool XdmfGridCollectionType::operator==(const XdmfGridCollectionType & collectionType) const
{
	return mName.compare(collectionType.mName) == 0;
}

bool XdmfGridCollectionType::operator!=(const XdmfGridCollectionType & collectionType) const
{
	return !this->operator==(collectionType);
}

void XdmfGridCollectionType::getProperties(std::map<std::string, std::string> & collectedProperties) const
{
	collectedProperties["CollectionType"] = mName;
}
