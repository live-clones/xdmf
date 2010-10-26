/*
 * XdmfGridUnstructured.cpp
 *
 *  Created on: Jan 25, 2010
 *      Author: kleiter
 */

#include "XdmfGridUnstructured.hpp"

boost::shared_ptr<XdmfGridUnstructured> XdmfGridUnstructured::New()
{
	boost::shared_ptr<XdmfGridUnstructured> p(new XdmfGridUnstructured());
	return p;
}

XdmfGridUnstructured::XdmfGridUnstructured() :
	XdmfGrid()
{
}

XdmfGridUnstructured::~XdmfGridUnstructured()
{
}

const std::string XdmfGridUnstructured::ItemTag = "Grid";

boost::shared_ptr<XdmfGeometry> XdmfGridUnstructured::getGeometry()
{
	return boost::const_pointer_cast<XdmfGeometry>(static_cast<const XdmfGrid &>(*this).getGeometry());
}

std::string XdmfGridUnstructured::getItemTag() const
{
	return ItemTag;
}

boost::shared_ptr<XdmfTopology> XdmfGridUnstructured::getTopology()
{
	return boost::const_pointer_cast<XdmfTopology>(static_cast<const XdmfGrid &>(*this).getTopology());
}

void XdmfGridUnstructured::setGeometry(const boost::shared_ptr<XdmfGeometry> geometry)
{
	mGeometry = geometry;
}

void XdmfGridUnstructured::setTopology(const boost::shared_ptr<XdmfTopology> topology)
{
	mTopology = topology;
}
