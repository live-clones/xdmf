/*
 * XdmfAttributeCenter.cpp
 *
 *  Created on: Jan 29, 2010
 *      Author: kleiter
 */

#include "XdmfAttributeCenter.hpp"

// Supported XdmfAttributeCenters
boost::shared_ptr<const XdmfAttributeCenter> XdmfAttributeCenter::Grid()
{
	static boost::shared_ptr<const XdmfAttributeCenter> p(new XdmfAttributeCenter("Grid"));
	return p;
}

boost::shared_ptr<const XdmfAttributeCenter> XdmfAttributeCenter::Cell()
{
	static boost::shared_ptr<const XdmfAttributeCenter> p(new XdmfAttributeCenter("Cell"));
	return p;
}

boost::shared_ptr<const XdmfAttributeCenter> XdmfAttributeCenter::Face()
{
	static boost::shared_ptr<const XdmfAttributeCenter> p(new XdmfAttributeCenter("Face"));
	return p;
}

boost::shared_ptr<const XdmfAttributeCenter> XdmfAttributeCenter::Edge()
{
	static boost::shared_ptr<const XdmfAttributeCenter> p(new XdmfAttributeCenter("Edge"));
	return p;
}

boost::shared_ptr<const XdmfAttributeCenter> XdmfAttributeCenter::Node()
{
	static boost::shared_ptr<const XdmfAttributeCenter> p(new XdmfAttributeCenter("Node"));
	return p;
}

XdmfAttributeCenter::XdmfAttributeCenter(const std::string & name) :
	mName(name)
{
}

XdmfAttributeCenter::~XdmfAttributeCenter()
{
}

boost::shared_ptr<const XdmfAttributeCenter> XdmfAttributeCenter::New(const std::map<std::string, std::string> & itemProperties)
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

bool XdmfAttributeCenter::operator==(const XdmfAttributeCenter & attributeCenter) const
{
  return mName.compare(attributeCenter.mName) == 0;
}

bool XdmfAttributeCenter::operator!=(const XdmfAttributeCenter & attributeCenter) const
{
  return !this->operator==(attributeCenter);
}

bool XdmfAttributeCenter::IsEqual(boost::shared_ptr<XdmfAttributeCenter> attributeCenter)
{
        if(attributeCenter == NULL) return false;
        if(this == attributeCenter.get()      
           &&   mName == attributeCenter->mName    
        ) return true; 
        return false;
}


void XdmfAttributeCenter::getProperties(std::map<std::string, std::string> & collectedProperties) const
{
	collectedProperties["Center"] = mName;
}
