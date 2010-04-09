/*
 * XdmfTopology.cpp
 *
 *  Created on: Jan 29, 2010
 *      Author: kleiter
 */

#include "XdmfArray.hpp"
#include "XdmfTopology.hpp"
#include "XdmfVisitor.hpp"

XdmfTopology::XdmfTopology() :
	mTopologyType(XdmfTopologyType::NoTopologyType()),
	mNumberElements(0)
{
	std::cout << "Created Topology " << this << std::endl;
}

XdmfTopology::~XdmfTopology()
{
	std::cout << "Deleted Topology " << this << std::endl;
}

XdmfTopologyType XdmfTopology::getTopologyType() const
{
	return mTopologyType;
}

void XdmfTopology::setTopologyType(const XdmfTopologyType& topType)
{
	mTopologyType = topType;
}

int XdmfTopology::getNodesPerElement() const
{
	return mTopologyType.getNodesPerElement();
}

int XdmfTopology::getNumberElements() const
{
	return this->getArray()->getSize() / this->getNodesPerElement();
}

std::string XdmfTopology::getTopologyTypeAsString() const
{
	return mTopologyType.getName();
}

std::string XdmfTopology::printSelf() const
{
	return "XdmfTopology";
}

void XdmfTopology::write(boost::shared_ptr<XdmfVisitor> visitor) const
{
	visitor->visit(this);
}
