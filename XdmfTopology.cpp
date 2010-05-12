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

unsigned int XdmfTopology::getNumberElements() const
{
	if(this->getTopologyType().getNodesPerElement() == 0)
	{
		return 0;
	}
	return this->getArray()->getSize() / this->getTopologyType().getNodesPerElement();
}

XdmfTopologyType XdmfTopology::getTopologyType() const
{
	return mTopologyType;
}

void XdmfTopology::setTopologyType(const XdmfTopologyType & topologyType)
{
	mTopologyType = topologyType;
}
