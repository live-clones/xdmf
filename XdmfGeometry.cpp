/*
 * XdmfGeometry.cpp
 *
 *  Created on: Jan 29, 2010
 *      Author: kleiter
 */

#include "XdmfArray.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfGeometryType.hpp"

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

std::string XdmfGeometry::ItemTag = "Geometry";

boost::shared_ptr<const XdmfGeometryType> XdmfGeometry::getGeometryType() const
{
	return mGeometryType;
}

std::map<std::string, std::string> XdmfGeometry::getItemProperties() const
{
	std::map<std::string, std::string> geometryProperties;
	mGeometryType->getProperties(geometryProperties);
	return geometryProperties;
}

std::string XdmfGeometry::getItemTag() const
{
	return ItemTag;
}

unsigned int XdmfGeometry::getNumberPoints() const
{
	if(mGeometryType->getDimensions() == 0)
	{
		return 0;
	}
	return this->getArray()->getSize() / mGeometryType->getDimensions();
}

void XdmfGeometry::populateItem(const std::map<std::string, std::string> & itemProperties, std::vector<boost::shared_ptr<XdmfItem> > & childItems)
{
	mGeometryType = XdmfGeometryType::New(itemProperties);
	XdmfDataItem::populateItem(itemProperties, childItems);
}

void XdmfGeometry::setGeometryType(const boost::shared_ptr<const XdmfGeometryType> geometryType)
{
	mGeometryType = geometryType;
}
