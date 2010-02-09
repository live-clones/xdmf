/*
 * XdmfAttribute.cpp
 *
 *  Created on: Jan 29, 2010
 *      Author: kleiter
 */

#include "XdmfAttribute.hpp"
#include "XdmfVisitor.hpp"

// Supported XdmfAttribut
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

std::string XdmfAttributeCenter::getName() const
{
	return mName;
}

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

XdmfAttributeType::XdmfAttributeType(const XdmfAttributeType& attributeType):
	mName(attributeType.mName)
{
}

XdmfAttributeType& XdmfAttributeType::operator=(const XdmfAttributeType& attributeType)
{
	if(this != &attributeType)
	{
		mName = attributeType.mName;
	}
	return *this;
}

std::string XdmfAttributeType::getName() const
{
	return mName;
}

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

const std::string XdmfAttribute::printSelf() const
{
	return "<XdmfAttribute>";
}
