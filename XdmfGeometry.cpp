/*
 * XdmfGrid.cpp
 *
 *  Created on: Jan 29, 2010
 *      Author: kleiter
 */

#include "XdmfGeometry.hpp"

// Supported XdmfGeometryTypes
const XdmfGeometryType XdmfGeometryType::NoGeometry("NONE", 0);
const XdmfGeometryType XdmfGeometryType::XYZ("XYZ", 3);
const XdmfGeometryType XdmfGeometryType::XY("XY", 2);
const XdmfGeometryType XdmfGeometryType::X_Y_Z("X_Y_Z", 3);
const XdmfGeometryType XdmfGeometryType::X_Y("X_Y", 2);
const XdmfGeometryType XdmfGeometryType::VXVYVZ("VXVYVZ", 3);
const XdmfGeometryType XdmfGeometryType::Origin_DXDYDZ("ORIGIN_DXDYDZ", 3);
const XdmfGeometryType XdmfGeometryType::VXVY("VXVY", 2);
const XdmfGeometryType XdmfGeometryType::Origin_DXDY("ORIGIN_DXDY", 2);

XdmfGeometry::XdmfGeometry() :
mGeometryType(&XdmfGeometryType::NoGeometry)
{
	std::cout << "Created Geometry " << this << std::endl;
}

XdmfGeometry::~XdmfGeometry()
{
	std::cout << "Deleted Geometry " << this << std::endl;
}

const XdmfGeometryType& XdmfGeometry::getGeometryType() const
{
	return *mGeometryType;
}

void XdmfGeometry::setGeometryType(const XdmfGeometryType& geomType)
{
	mGeometryType = &geomType;
	return;
}

const std::string XdmfGeometry::getGeometryTypeAsString() const
{
	return mGeometryType->name;
}

const int XdmfGeometry::getDimensions() const
{
	return mGeometryType->dimensions;
}

const std::string XdmfGeometry::printSelf() const
{
	return "XdmfGeometry";
}
