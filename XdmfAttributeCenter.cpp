/*
 * XdmfAttributeCenter.cpp
 *
 *  Created on: Jan 29, 2010
 *      Author: kleiter
 */

#include "XdmfAttributeCenter.hpp"

// Supported XdmfAttributeCenters
XdmfAttributeCenter XdmfAttributeCenter::Grid()
{
	return XdmfAttributeCenter("Grid");
}

XdmfAttributeCenter XdmfAttributeCenter::Cell()
{
	return XdmfAttributeCenter("Cell");
}

XdmfAttributeCenter XdmfAttributeCenter::Face()
{
	return XdmfAttributeCenter("Face");
}

XdmfAttributeCenter XdmfAttributeCenter::Edge()
{
	return XdmfAttributeCenter("Edge");
}

XdmfAttributeCenter XdmfAttributeCenter::Node()
{
	return XdmfAttributeCenter("Node");
}

XdmfAttributeCenter::XdmfAttributeCenter(const std::string & name) :
	mName(name)
{
}

XdmfAttributeCenter::XdmfAttributeCenter(const XdmfAttributeCenter & attributeCenter):
  mName(attributeCenter.mName)
{
}

XdmfAttributeCenter XdmfAttributeCenter::New(const std::map<std::string, std::string> & itemProperties)
{
	std::map<std::string, std::string>::const_iterator center = itemProperties.find("Center");
	if(center != itemProperties.end())
	{
		const std::string centerVal = center->second;
		if(centerVal.compare("Grid") == 0)
		{
			return Grid();
		}
		else if(centerVal.compare("Cell") == 0)
		{
			return Cell();
		}
		else if(centerVal.compare("Face") == 0)
		{
			return Face();
		}
		else if(centerVal.compare("Edge") == 0)
		{
			return Edge();
		}
		else if(centerVal.compare("Node") == 0)
		{
			return Node();
		}
		else
		{
			assert(false);
		}
	}
	assert(false);
}

XdmfAttributeCenter & XdmfAttributeCenter::operator=(const XdmfAttributeCenter & attributeCenter)
{
  if(this != &attributeCenter)
  {
    mName = attributeCenter.mName;
  }
  return *this;
}

bool XdmfAttributeCenter::operator==(const XdmfAttributeCenter & attributeCenter) const
{
  return mName.compare(attributeCenter.mName) == 0;
}

bool XdmfAttributeCenter::operator!=(const XdmfAttributeCenter & attributeCenter) const
{
  return !this->operator==(attributeCenter);
}

void XdmfAttributeCenter::getProperties(std::map<std::string, std::string> & collectedProperties) const
{
	collectedProperties["Center"] = mName;
}
