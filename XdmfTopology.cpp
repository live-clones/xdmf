/*
 * XdmfTopology.cpp
 *
 *  Created on: Jan 29, 2010
 *      Author: kleiter
 */

#include <sstream>
#include "XdmfArray.hpp"
#include "XdmfTopology.hpp"
#include "XdmfTopologyType.hpp"

XdmfTopology::XdmfTopology() :
	mTopologyType(XdmfTopologyType::NoTopologyType())
{
	std::cout << "Created Topology " << this << std::endl;
}

XdmfTopology::~XdmfTopology()
{
	std::cout << "Deleted Topology " << this << std::endl;
}

const std::string XdmfTopology::ItemTag = "Topology";

std::string XdmfTopology::getItemTag() const
{
	return ItemTag;
}

std::map<std::string, std::string> XdmfTopology::getItemProperties() const
{
	std::map<std::string, std::string> topologyProperties;
	mTopologyType->getProperties(topologyProperties);
	if(mTopologyType != XdmfTopologyType::Polyvertex())
	{
		std::stringstream numElements;
		numElements << this->getNumberElements();
		topologyProperties["Dimensions"] = numElements.str();
	}
	return topologyProperties;
}

unsigned int XdmfTopology::getNumberElements() const
{
	if(mTopologyType->getNodesPerElement() == 0)
	{
		return 0;
	}
	return this->getArray()->getSize() / mTopologyType->getNodesPerElement();
}

boost::shared_ptr<const XdmfTopologyType> XdmfTopology::getType() const
{
	return mTopologyType;
}

void XdmfTopology::populateItem(const std::map<std::string, std::string> & itemProperties, std::vector<boost::shared_ptr<XdmfItem> > & childItems)
{
	mTopologyType = XdmfTopologyType::New(itemProperties);
	XdmfDataItem::populateItem(itemProperties, childItems);
}

void XdmfTopology::setType(const boost::shared_ptr<const XdmfTopologyType> topologyType)
{
	mTopologyType = topologyType;
}
