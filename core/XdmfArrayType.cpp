/*
 * XdmfArrayType.cpp
 *
 *  Created on: Jan 29, 2010
 *      Author: kleiter
 */

#include <sstream>
#include "XdmfArrayType.hpp"

// Supported XdmfArrayTypes
boost::shared_ptr<const XdmfArrayType> XdmfArrayType::Uninitialized()
{
	static boost::shared_ptr<const XdmfArrayType> p(new XdmfArrayType("None", 0));
	return p;
}

boost::shared_ptr<const XdmfArrayType> XdmfArrayType::Int8()
{
	static boost::shared_ptr<const XdmfArrayType> p(new XdmfArrayType("Char", 1));
	return p;
}

boost::shared_ptr<const XdmfArrayType> XdmfArrayType::Int16()
{
	static boost::shared_ptr<const XdmfArrayType> p(new XdmfArrayType("Short", 2));
	return p;
}

boost::shared_ptr<const XdmfArrayType> XdmfArrayType::Int32()
{
	static boost::shared_ptr<const XdmfArrayType> p(new XdmfArrayType("Int", 4));
	return p;
}

boost::shared_ptr<const XdmfArrayType> XdmfArrayType::Int64()
{
	static boost::shared_ptr<const XdmfArrayType> p(new XdmfArrayType("Int", 8));
	return p;
}

boost::shared_ptr<const XdmfArrayType> XdmfArrayType::Float32()
{
	static boost::shared_ptr<const XdmfArrayType> p(new XdmfArrayType("Float", 4));
	return p;
}

boost::shared_ptr<const XdmfArrayType> XdmfArrayType::Float64()
{
	static boost::shared_ptr<const XdmfArrayType> p(new XdmfArrayType("Float", 8));
	return p;
}

boost::shared_ptr<const XdmfArrayType> XdmfArrayType::UInt8()
{
	static boost::shared_ptr<const XdmfArrayType> p(new XdmfArrayType("UChar", 1));
	return p;
}

boost::shared_ptr<const XdmfArrayType> XdmfArrayType::UInt16()
{
	static boost::shared_ptr<const XdmfArrayType> p(new XdmfArrayType("UShort", 2));
	return p;
}

boost::shared_ptr<const XdmfArrayType> XdmfArrayType::UInt32()
{
	static boost::shared_ptr<const XdmfArrayType> p(new XdmfArrayType("UInt", 4));
	return p;
}

XdmfArrayType::XdmfArrayType(const std::string & name, const unsigned int precision) :
	mName(name),
	mPrecision(precision)
{
}

XdmfArrayType::~XdmfArrayType()
{
}

boost::shared_ptr<const XdmfArrayType> XdmfArrayType::New(const std::map<std::string, std::string> & itemProperties)
{
	std::map<std::string, std::string>::const_iterator type = itemProperties.find("DataType");
	if(type == itemProperties.end())
	{
		type = itemProperties.find("NumberType");
	}
	std::map<std::string, std::string>::const_iterator precision = itemProperties.find("Precision");
	if(type != itemProperties.end())
	{
		const std::string typeVal = type->second;
		unsigned int precisionVal = 0;
		if(precision != itemProperties.end())
		{
			precisionVal = atoi(precision->second.c_str());
		}
		if(typeVal.compare("None") == 0)
		{
			return Uninitialized();
		}
		else if(typeVal.compare("Char") == 0)
		{
			return Int8();
		}
		else if(typeVal.compare("Short") == 0)
		{
			return Int16();
		}
		else if(typeVal.compare("Int") == 0 && precisionVal == 8)
		{
			return Int64();
		}
		else if(typeVal.compare("Int") == 0)
		{
			return Int32();
		}
		else if(typeVal.compare("Float") == 0 && precisionVal == 8)
		{
			return Float64();
		}
		else if(typeVal.compare("Float") == 0)
		{
			return Float32();
		}
		else if(typeVal.compare("UChar") == 0)
		{
			return UInt8();
		}
		else if(typeVal.compare("UShort") == 0)
		{
			return UInt16();
		}
		else if(typeVal.compare("UInt") == 0)
		{
			return UInt32();
		}
		else
		{
			assert(false);
		}
	}
	assert(false);
}

void XdmfArrayType::getProperties(std::map<std::string, std::string> & collectedProperties) const
{
	collectedProperties["DataType"] = mName;
	std::stringstream precision;
	precision << mPrecision;
	collectedProperties["Precision"] = precision.str();
}

int XdmfArrayType::getElementSize() const
{
	return (mPrecision);
}
