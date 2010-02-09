/*
 * XdmfDataItemType.cpp
 *
 *  Created on: Jan 29, 2010
 *      Author: kleiter
 */

#include "XdmfDataItemType.hpp"

// Supported XdmfDataItemTypes
XdmfDataItemType XdmfDataItemType::Unknown()
{
	return XdmfDataItemType("Unknown");
}

XdmfDataItemType XdmfDataItemType::Int8()
{
	return XdmfDataItemType("Int8");
}

XdmfDataItemType XdmfDataItemType::Int16()
{
	return XdmfDataItemType("Int16");
}

XdmfDataItemType XdmfDataItemType::Int32()
{
	return XdmfDataItemType("Int32");
}

XdmfDataItemType XdmfDataItemType::Int64()
{
	return XdmfDataItemType("Int64");
}

XdmfDataItemType XdmfDataItemType::Float32()
{
	return XdmfDataItemType("Float32");
}

XdmfDataItemType XdmfDataItemType::Float64()
{
	return XdmfDataItemType("Float64");
}

XdmfDataItemType XdmfDataItemType::UInt8()
{
	return XdmfDataItemType("UInt8");
}

XdmfDataItemType XdmfDataItemType::UInt16()
{
	return XdmfDataItemType("UInt16");
}

XdmfDataItemType XdmfDataItemType::UInt32()
{
	return XdmfDataItemType("UInt32");
}

XdmfDataItemType::XdmfDataItemType(const std::string& name) :
	mName(name)
{};

XdmfDataItemType::XdmfDataItemType(const XdmfDataItemType& dataItemType):
	mName(dataItemType.mName)
{
}

XdmfDataItemType& XdmfDataItemType::operator=(const XdmfDataItemType& dataItemType)
{
	if(this != &dataItemType)
	{
		mName = dataItemType.mName;
	}
	return *this;
}

bool XdmfDataItemType::operator==(const XdmfDataItemType& dataItemType) const
{
	return mName.compare(dataItemType.mName) == 0;
}

bool XdmfDataItemType::operator!=(const XdmfDataItemType& dataItemType) const
{
	return !this->operator==(dataItemType);
}

std::string XdmfDataItemType::getName() const
{
	return mName;
}
