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
	return XdmfDataItemType("Unknown", 0, 0);
}

XdmfDataItemType XdmfDataItemType::Int8()
{
	return XdmfDataItemType("Char", H5T_NATIVE_CHAR, 1);
}

XdmfDataItemType XdmfDataItemType::Int16()
{
	return XdmfDataItemType("Short", H5T_NATIVE_SHORT, 2);
}

XdmfDataItemType XdmfDataItemType::Int32()
{
	return XdmfDataItemType("Int", H5T_NATIVE_INT, 4);
}

XdmfDataItemType XdmfDataItemType::Int64()
{
	return XdmfDataItemType("Int", H5T_NATIVE_LONG, 8);
}

XdmfDataItemType XdmfDataItemType::Float32()
{
	return XdmfDataItemType("Float", H5T_NATIVE_FLOAT, 4);
}

XdmfDataItemType XdmfDataItemType::Float64()
{
	return XdmfDataItemType("Float", H5T_NATIVE_DOUBLE, 8);
}

XdmfDataItemType XdmfDataItemType::UInt8()
{
	return XdmfDataItemType("UChar", H5T_NATIVE_UCHAR, 1);
}

XdmfDataItemType XdmfDataItemType::UInt16()
{
	return XdmfDataItemType("UShort", H5T_NATIVE_USHORT, 2);
}

XdmfDataItemType XdmfDataItemType::UInt32()
{
	return XdmfDataItemType("UInt", H5T_NATIVE_UINT, 4);
}

XdmfDataItemType::XdmfDataItemType(const std::string& name, hid_t hdfDataType, int precision) :
	mName(name),
	mHDF5DataType(hdfDataType),
	mPrecision(precision)
{};

XdmfDataItemType::XdmfDataItemType(const XdmfDataItemType& dataItemType):
	mName(dataItemType.mName),
	mHDF5DataType(dataItemType.mHDF5DataType),
	mPrecision(dataItemType.mPrecision)
{
}

XdmfDataItemType& XdmfDataItemType::operator=(const XdmfDataItemType& dataItemType)
{
	if(this != &dataItemType)
	{
		mName = dataItemType.mName;
		mHDF5DataType = dataItemType.mHDF5DataType;
		mPrecision = dataItemType.mPrecision;
	}
	return *this;
}

bool XdmfDataItemType::operator==(const XdmfDataItemType& dataItemType) const
{
	return mName.compare(dataItemType.mName) == 0 && mHDF5DataType == dataItemType.mHDF5DataType && mPrecision == dataItemType.mPrecision;
}

bool XdmfDataItemType::operator!=(const XdmfDataItemType& dataItemType) const
{
	return !this->operator==(dataItemType);
}

std::string XdmfDataItemType::getName() const
{
	return mName;
}

hid_t XdmfDataItemType::getHDF5DataType() const
{
	return mHDF5DataType;
}

int XdmfDataItemType::getPrecision() const
{
	return mPrecision;
}
