/*
 * XdmfAttributeCenter.cpp
 *
 *  Created on: Jan 29, 2010
 *      Author: kleiter
 */

#include "XdmfAttributeCenter.hpp"

// Supported XdmfAttributeCenters
XdmfAttributeCenter XdmfAttributeCenter::Grid()
{
	return XdmfAttributeCenter("Grid");
}

XdmfAttributeCenter XdmfAttributeCenter::Cell()
{
	return XdmfAttributeCenter("Cell");
}

XdmfAttributeCenter XdmfAttributeCenter::Face()
{
	return XdmfAttributeCenter("Face");
}

XdmfAttributeCenter XdmfAttributeCenter::Edge()
{
	return XdmfAttributeCenter("Edge");
}

XdmfAttributeCenter XdmfAttributeCenter::Node()
{
	return XdmfAttributeCenter("Node");
}

XdmfAttributeCenter::XdmfAttributeCenter(const std::string& name) :
	mName(name)
{};

XdmfAttributeCenter::XdmfAttributeCenter(const XdmfAttributeCenter& attributeCenter):
	mName(attributeCenter.mName)
{
}

XdmfAttributeCenter& XdmfAttributeCenter::operator=(const XdmfAttributeCenter& attributeCenter)
{
	if(this != &attributeCenter)
	{
		mName = attributeCenter.mName;
	}
	return *this;
}

bool XdmfAttributeCenter::operator==(const XdmfAttributeCenter& attributeCenter) const
{
	return mName.compare(attributeCenter.mName) == 0;
}

bool XdmfAttributeCenter::operator!=(const XdmfAttributeCenter& attributeCenter) const
{
	return !this->operator==(attributeCenter);
}

std::string XdmfAttributeCenter::getName() const
{
	return mName;
}
