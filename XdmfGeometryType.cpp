/*
 * XdmfGeometryType.cpp
 *
 *  Created on: Jan 29, 2010
 *      Author: kleiter
 */

#include "XdmfGeometryType.hpp"

// Supported XdmfGeometryTypes
XdmfGeometryType XdmfGeometryType::NoGeometryType()
{
	return XdmfGeometryType("NONE", 0);
}

XdmfGeometryType XdmfGeometryType::XYZ()
{
	return XdmfGeometryType("XYZ", 3);
}

XdmfGeometryType XdmfGeometryType::XY()
{
	return XdmfGeometryType("XY", 2);
}

XdmfGeometryType XdmfGeometryType::X_Y_Z()
{
	return XdmfGeometryType("X_Y_Z", 3);
}

XdmfGeometryType XdmfGeometryType::X_Y()
{
	return XdmfGeometryType("X_Y", 2);
}

XdmfGeometryType XdmfGeometryType::VXVYVZ()
{
	return XdmfGeometryType("VXVYVZ", 3);
}

XdmfGeometryType XdmfGeometryType::Origin_DXDYDZ()
{
	return XdmfGeometryType("ORIGIN_DXDYDZ", 3);
}

XdmfGeometryType XdmfGeometryType::VXVY()
{
	return XdmfGeometryType("VXVY", 2);
}

XdmfGeometryType XdmfGeometryType::Origin_DXDY()
{
	return XdmfGeometryType("ORIGIN_DXDY", 2);
}

XdmfGeometryType::XdmfGeometryType(const XdmfGeometryType& geometryType):
	mDimensions(geometryType.mDimensions),
	mName(geometryType.mName)
{
}

XdmfGeometryType::XdmfGeometryType(const std::string& name, const int& dimensions) :
	mName(name),
	mDimensions(dimensions)
{
}

XdmfGeometryType XdmfGeometryType::New(const std::map<std::string, std::string> & itemProperties)
{
	std::map<std::string, std::string>::const_iterator type = itemProperties.find("GeometryType");
	if(type != itemProperties.end())
	{
		const std::string typeVal = type->second;
		if(typeVal.compare("NONE") == 0)
		{
			return NoGeometryType();
		}
		else if(typeVal.compare("XYZ") == 0)
		{
			return XYZ();
		}
		else if(typeVal.compare("XY") == 0)
		{
			return XY();
		}
		else if(typeVal.compare("X_Y_Z") == 0)
		{
			return X_Y_Z();
		}
		else if(typeVal.compare("X_Y") == 0)
		{
			return X_Y();
		}
		else if(typeVal.compare("VXVYVZ") == 0)
		{
			return VXVYVZ();
		}
		else if(typeVal.compare("ORIGIN_DXDYDZ") == 0)
		{
			return Origin_DXDYDZ();
		}
		else if(typeVal.compare("VXVY") == 0)
		{
			return VXVY();
		}
		else if(typeVal.compare("ORIGIN_DXDY") == 0)
		{
			return Origin_DXDY();
		}
		else
		{
			assert(false);
		}
	}
	assert(false);
}

XdmfGeometryType & XdmfGeometryType::operator=(const XdmfGeometryType& geometryType)
{
	if(this != &geometryType)
	{
		mDimensions = geometryType.mDimensions;
		mName = geometryType.mName;
	}
	return *this;
}

bool XdmfGeometryType::operator==(const XdmfGeometryType& geometryType) const
{
	return mDimensions == geometryType.mDimensions && mName.compare(geometryType.mName) == 0;
}

bool XdmfGeometryType::operator!=(const XdmfGeometryType& geometryType) const
{
	return !this->operator==(geometryType);
}

unsigned int XdmfGeometryType::getDimensions() const
{
	return mDimensions;
}

void XdmfGeometryType::getProperties(std::map<std::string, std::string> & collectedProperties) const
{
	collectedProperties["GeometryType"] = this->mName;
}
