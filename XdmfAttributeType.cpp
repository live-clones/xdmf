/*
 * XdmfAttributeType.cpp
 *
 *  Created on: Jan 29, 2010
 *      Author: kleiter
 */

#include "XdmfAttributeType.hpp"

// Supported XdmfAttributeTypes
XdmfAttributeType XdmfAttributeType::NoAttributeType()
{
	return XdmfAttributeType("None");
}

XdmfAttributeType XdmfAttributeType::Scalar()
{
	return XdmfAttributeType("Scalar");
}

XdmfAttributeType XdmfAttributeType::Vector()
{
	return XdmfAttributeType("Vector");
}

XdmfAttributeType XdmfAttributeType::Tensor()
{
	return XdmfAttributeType("Tensor");
}

XdmfAttributeType XdmfAttributeType::Matrix()
{
	return XdmfAttributeType("Matrix");
}

XdmfAttributeType XdmfAttributeType::Tensor6()
{
	return XdmfAttributeType("Tensor6");
}

XdmfAttributeType XdmfAttributeType::GlobalId()
{
	return XdmfAttributeType("GlobalId");
}

XdmfAttributeType::XdmfAttributeType(const std::string & name) :
	mName(name)
{};

XdmfAttributeType::XdmfAttributeType(const XdmfAttributeType & attributeType):
	mName(attributeType.mName)
{
}

XdmfAttributeType& XdmfAttributeType::operator=(const XdmfAttributeType & attributeType)
{
	if(this != &attributeType)
	{
		mName = attributeType.mName;
	}
	return *this;
}

bool XdmfAttributeType::operator==(const XdmfAttributeType & attributeType) const
{
	return mName.compare(attributeType.mName) == 0;
}

bool XdmfAttributeType::operator!=(const XdmfAttributeType & attributeType) const
{
	return !this->operator==(attributeType);
}

std::string XdmfAttributeType::getName() const
{
	return mName;
}

void XdmfAttributeType::getProperties(std::map<std::string, std::string> & collectedProperties) const
{
	collectedProperties["AttributeType"] = this->mName;
}
