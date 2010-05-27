/*
 * XdmfAttributeType.cpp
 *
 *  Created on: Jan 29, 2010
 *      Author: kleiter
 */

#include "XdmfAttributeType.hpp"

// Supported XdmfAttributeTypes
boost::shared_ptr<const XdmfAttributeType> XdmfAttributeType::NoAttributeType()
{
	static boost::shared_ptr<const XdmfAttributeType> p(new XdmfAttributeType("None"));
	return p;
}

boost::shared_ptr<const XdmfAttributeType> XdmfAttributeType::Scalar()
{
	static boost::shared_ptr<const XdmfAttributeType> p(new XdmfAttributeType("Scalar"));
	return p;
}

boost::shared_ptr<const XdmfAttributeType> XdmfAttributeType::Vector()
{
	static boost::shared_ptr<const XdmfAttributeType> p(new XdmfAttributeType("Vector"));
	return p;
}

boost::shared_ptr<const XdmfAttributeType> XdmfAttributeType::Tensor()
{
	static boost::shared_ptr<const XdmfAttributeType> p(new XdmfAttributeType("Tensor"));
	return p;
}

boost::shared_ptr<const XdmfAttributeType> XdmfAttributeType::Matrix()
{
	static boost::shared_ptr<const XdmfAttributeType> p(new XdmfAttributeType("Matrix"));
	return p;
}

boost::shared_ptr<const XdmfAttributeType> XdmfAttributeType::Tensor6()
{
	static boost::shared_ptr<const XdmfAttributeType> p(new XdmfAttributeType("Tensor6"));
	return p;
}

boost::shared_ptr<const XdmfAttributeType> XdmfAttributeType::GlobalId()
{
	static boost::shared_ptr<const XdmfAttributeType> p(new XdmfAttributeType("GlobalId"));
	return p;
}

XdmfAttributeType::XdmfAttributeType(const std::string & name) :
	mName(name)
{
	std::cout << "CRRRRREATED" << std::endl;
}

XdmfAttributeType::~XdmfAttributeType()
{
	std::cout << "DEEEELETED" << std::endl;
}

boost::shared_ptr<const XdmfAttributeType> XdmfAttributeType::New(const std::map<std::string, std::string> & itemProperties)
{
	std::map<std::string, std::string>::const_iterator type = itemProperties.find("AttributeType");
	if(type != itemProperties.end())
	{
		const std::string typeVal = type->second;
		if(typeVal.compare("None") == 0)
		{
			return NoAttributeType();
		}
		else if(typeVal.compare("Scalar") == 0)
		{
			return Scalar();
		}
		else if(typeVal.compare("Vector") == 0)
		{
			return Vector();
		}
		else if(typeVal.compare("Tensor") == 0)
		{
			return Tensor();
		}
		else if(typeVal.compare("Matrix") == 0)
		{
			return Matrix();
		}
		else if(typeVal.compare("Tensor6") == 0)
		{
			return Tensor6();
		}
		else if(typeVal.compare("GlobalId") == 0)
		{
			return GlobalId();
		}
		else
		{
			assert(false);
		}
	}
	assert(false);
}

bool XdmfAttributeType::operator==(const XdmfAttributeType & attributeType) const
{
	return mName.compare(attributeType.mName) == 0;
}

bool XdmfAttributeType::operator!=(const XdmfAttributeType & attributeType) const
{
	return !this->operator==(attributeType);
}

void XdmfAttributeType::getProperties(std::map<std::string, std::string> & collectedProperties) const
{
	collectedProperties["AttributeType"] = mName;
}
