/*
 * XdmfGeometry.cpp
 *
 *  Created on: Jan 29, 2010
 *      Author: kleiter
 */

#include "XdmfGeometry.hpp"
#include "XdmfVisitor.hpp"

XdmfGeometry::XdmfGeometry() :
	mGeometryType(XdmfGeometryType::NoGeometryType()),
	mNumberPoints(0)
{
	std::cout << "Created Geometry " << this << std::endl;
}

XdmfGeometry::~XdmfGeometry()
{
	std::cout << "Deleted Geometry " << this << std::endl;
}

void XdmfGeometry::accept(boost::shared_ptr<XdmfVisitor> visitor) const
{
	visitor->visit(this, visitor);
}

XdmfGeometryType XdmfGeometry::getGeometryType() const
{
	return mGeometryType;
}

std::string XdmfGeometry::printSelf() const
{
	return "XdmfGeometry";
}

void XdmfGeometry::setGeometryType(const XdmfGeometryType & geometryType)
{
	mGeometryType = geometryType;
}
