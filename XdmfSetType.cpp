/*
 * XdmfSetType.cpp
 *
 *  Created on: Jan 29, 2010
 *      Author: kleiter
 */

#include "XdmfSetType.hpp"

// Supported XdmfSetTypes
XdmfSetType XdmfSetType::NoSetType()
{
	return XdmfSetType("None");
}

XdmfSetType XdmfSetType::Node()
{
	return XdmfSetType("Node");
}

XdmfSetType XdmfSetType::Cell()
{
	return XdmfSetType("Cell");
}

XdmfSetType XdmfSetType::Face()
{
	return XdmfSetType("Face");
}

XdmfSetType XdmfSetType::Edge()
{
	return XdmfSetType("Edge");
}

XdmfSetType::XdmfSetType(const XdmfSetType & setType):
	mName(setType.mName)
{
}

XdmfSetType::XdmfSetType(const std::string & name) :
	mName(name)
{
}

XdmfSetType XdmfSetType::New(const std::map<std::string, std::string> & itemProperties)
{
	std::map<std::string, std::string>::const_iterator type = itemProperties.find("SetType");
	if(type != itemProperties.end())
	{
		const std::string typeVal = type->second;
		if(typeVal.compare("None") == 0)
		{
			return NoSetType();
		}
		else if(typeVal.compare("Node") == 0)
		{
			return Node();
		}
		else if(typeVal.compare("Cell") == 0)
		{
			return Cell();
		}
		else if(typeVal.compare("Face") == 0)
		{
			return Face();
		}
		else if(typeVal.compare("Edge") == 0)
		{
			return Edge();
		}
		else
		{
			assert(false);
		}
	}
	assert(false);
}

XdmfSetType & XdmfSetType::operator=(const XdmfSetType & setType)
{
	if(this != &setType)
	{
		mName = setType.mName;
	}
	return *this;
}

bool XdmfSetType::operator==(const XdmfSetType & setType) const
{
	return mName.compare(setType.mName) == 0;
}

bool XdmfSetType::operator!=(const XdmfSetType & setType) const
{
	return !this->operator==(setType);
}

void XdmfSetType::getProperties(std::map<std::string, std::string> & collectedProperties) const
{
	collectedProperties["SetType"] = this->mName;
}
