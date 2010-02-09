/*
 * XdmfGeometry.cpp
 *
 *  Created on: Jan 29, 2010
 *      Author: kleiter
 */

#include "XdmfGeometry.hpp"
#include "XdmfVisitor.hpp"

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

std::string XdmfGeometry::printSelf() const
{
	return "XdmfGeometry";
}

void XdmfGeometry::write(boost::shared_ptr<XdmfVisitor> visitor) const
{
	visitor->visit(this);
}
