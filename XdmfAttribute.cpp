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

XdmfAttributeCenter XdmfAttribute::getAttributeCenter() const
{
	return mAttributeCenter;
}

XdmfAttributeType XdmfAttribute::getAttributeType() const
{
	return mAttributeType;
}

std::string XdmfAttribute::getName() const
{
	return mName;
}

std::string XdmfAttribute::printSelf() const
{
	return "<XdmfAttribute>";
}

void XdmfAttribute::setAttributeCenter(const XdmfAttributeCenter & attributeCenter)
{
	mAttributeCenter = attributeCenter;
}

void XdmfAttribute::setAttributeType(const XdmfAttributeType & attributeType)
{
	mAttributeType = attributeType;
}

void XdmfAttribute::setName(const std::string & name)
{
	mName= name;
}

void XdmfAttribute::write(boost::shared_ptr<XdmfVisitor> visitor) const
{
	visitor->visit(this, visitor);
}
