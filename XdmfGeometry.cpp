/*
 * XdmfGeometry.cpp
 *
 *  Created on: Jan 29, 2010
 *      Author: kleiter
 */

#include "XdmfGeometry.hpp"
#include "XdmfGeometryType.hpp"

boost::shared_ptr<XdmfGeometry> XdmfGeometry::New()
{
	boost::shared_ptr<XdmfGeometry> p(new XdmfGeometry());
	return p;
}

XdmfGeometry::XdmfGeometry() :
	mNumberPoints(0),
	mType(XdmfGeometryType::NoGeometryType())
{
}

XdmfGeometry::~XdmfGeometry()
{
}

const std::string XdmfGeometry::ItemTag = "Geometry";

std::map<std::string, std::string> XdmfGeometry::getItemProperties() const
{
	std::map<std::string, std::string> geometryProperties;
	mType->getProperties(geometryProperties);
	return geometryProperties;
}

std::string XdmfGeometry::getItemTag() const
{
	return ItemTag;
}

unsigned int XdmfGeometry::getNumberPoints() const
{
	if(mType->getDimensions() == 0)
	{
		return 0;
	}
	return this->getSize() / mType->getDimensions();
}

boost::shared_ptr<const XdmfGeometryType> XdmfGeometry::getType() const
{
	return mType;
}

void XdmfGeometry::populateItem(const std::map<std::string, std::string> & itemProperties, std::vector<boost::shared_ptr<XdmfItem> > & childItems, const XdmfCoreReader * const reader)
{
	XdmfItem::populateItem(itemProperties, childItems, reader);
	mType = XdmfGeometryType::New(itemProperties);
	for(std::vector<boost::shared_ptr<XdmfItem> >::const_iterator iter = childItems.begin(); iter != childItems.end(); ++iter)
	{
		if(boost::shared_ptr<XdmfArray> array = boost::shared_dynamic_cast<XdmfArray>(*iter))
		{
			this->swap(array);
		}
		// TODO: If multiple dataitems.
	}
}

void XdmfGeometry::setType(const boost::shared_ptr<const XdmfGeometryType> type)
{
	mType = type;
}
