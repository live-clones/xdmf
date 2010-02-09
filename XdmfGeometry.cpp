/*
 * XdmfGrid.cpp
 *
 *  Created on: Jan 29, 2010
 *      Author: kleiter
 */

#include "XdmfGeometry.hpp"
#include "XdmfVisitor.hpp"

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

XdmfGeometryType::XdmfGeometryType(const XdmfGeometryType& geomType):
	mDimensions(geomType.mDimensions),
	mName(geomType.mName)
{
}

XdmfGeometryType& XdmfGeometryType::operator=(const XdmfGeometryType& geomType)
{
	if(this != &geomType)
	{
		mDimensions = geomType.mDimensions;
		mName = geomType.mName;
	}
	return *this;
}

int XdmfGeometryType::getDimensions() const
{
	return mDimensions;
}

std::string XdmfGeometryType::getName() const
{
	return mName;
}

XdmfGeometry::XdmfGeometry() :
mGeometryType(XdmfGeometryType::NoGeometryType())
{
	std::cout << "Created Geometry " << this << std::endl;
}

XdmfGeometry::~XdmfGeometry()
{
	std::cout << "Deleted Geometry " << this << std::endl;
}

XdmfGeometryType XdmfGeometry::getGeometryType() const
{
	return mGeometryType;
}

void XdmfGeometry::setGeometryType(const XdmfGeometryType& geomType)
{
	mGeometryType = geomType;
}

std::string XdmfGeometry::getGeometryTypeAsString() const
{
	return mGeometryType.getName();
}

int XdmfGeometry::getDimensions() const
{
	return mGeometryType.getDimensions();
}

const std::string XdmfGeometry::printSelf() const
{
	return "XdmfGeometry";
}

void XdmfGeometry::write(boost::shared_ptr<XdmfVisitor> visitor) const
{
	visitor->visit(this);
}
