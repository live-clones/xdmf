/*
 * XdmfAttribute.cpp
 *
 *  Created on: Jan 29, 2010
 *      Author: kleiter
 */

#include "XdmfAttribute.hpp"
#include "XdmfVisitor.hpp"

XdmfAttribute::XdmfAttribute() :
	mName(""),
	mAttributeType(XdmfAttributeType::NoAttributeType()),
	mAttributeCenter(XdmfAttributeCenter::Grid())
{
	std::cout << "Created Attribute " << this << std::endl;
}

XdmfAttribute::~XdmfAttribute()
{
	std::cout << "Deleted Attribute " << this << std::endl;
}

XdmfAttributeType XdmfAttribute::getAttributeType() const
{
	return mAttributeType;
}

void XdmfAttribute::setAttributeType(const XdmfAttributeType& attributeType)
{
	mAttributeType = attributeType;
}

std::string XdmfAttribute::getAttributeTypeAsString() const
{
	return mAttributeType.getName();
}

XdmfAttributeCenter XdmfAttribute::getAttributeCenter() const
{
	return mAttributeCenter;
}

void XdmfAttribute::setAttributeCenter(const XdmfAttributeCenter& attributeCenter)
{
	mAttributeCenter = attributeCenter;
}

std::string XdmfAttribute::getAttributeCenterAsString() const
{
	return mAttributeCenter.getName();
}

std::string XdmfAttribute::getName() const
{
	return mName;
}

void XdmfAttribute::setName(const std::string& name)
{
	mName= name;
}

void XdmfAttribute::write(boost::shared_ptr<XdmfVisitor> visitor) const
{
	visitor->visit(this);
}

std::string XdmfAttribute::printSelf() const
{
	return "<XdmfAttribute>";
}
