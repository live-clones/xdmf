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
{};

XdmfGeometryType& XdmfGeometryType::operator=(const XdmfGeometryType& geometryType)
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

int XdmfGeometryType::getDimensions() const
{
	return mDimensions;
}

std::string XdmfGeometryType::getName() const
{
	return mName;
}
