/*
 * XdmfDataItem.cpp
 *
 *  Created on: Jan 29, 2010
 *      Author: kleiter
 */

#include "XdmfDataItem.hpp"
#include "XdmfVisitor.hpp"

XdmfDataItem::XdmfDataItem() :
	mDataType(XdmfDataItemType::Unknown()),
	mNumberValues(0)
{
	std::cout << "Created DataItem " << this << std::endl;
}

XdmfDataItem::~XdmfDataItem()
{
	std::cout << "Deleted DataItem " << this << std::endl;
}

std::string XdmfDataItem::printSelf() const
{
	return "XdmfDataItem";
}

template <>
void XdmfDataItem::setValues<char>(char* dataPointer)
{
	mDataPointer = dataPointer;
	mDataType = XdmfDataItemType::Int8();
}

template <>
void XdmfDataItem::setValues<unsigned char>(unsigned char* dataPointer)
{
	mDataPointer = dataPointer;
	mDataType = XdmfDataItemType::UInt8();
}

template <>
void XdmfDataItem::setValues<short>(short* dataPointer)
{
	mDataPointer = dataPointer;
	mDataType = XdmfDataItemType::Int16();
}

template <>
void XdmfDataItem::setValues<unsigned short>(unsigned short* dataPointer)
{
	mDataPointer = dataPointer;
	mDataType = XdmfDataItemType::UInt16();
}

template <>
void XdmfDataItem::setValues<int>(int* dataPointer)
{
	mDataPointer = dataPointer;
	mDataType = XdmfDataItemType::Int32();
}

template <>
void XdmfDataItem::setValues<unsigned int>(unsigned int* dataPointer)
{
	mDataPointer = dataPointer;
	mDataType = XdmfDataItemType::UInt32();
}

template <>
void XdmfDataItem::setValues<long>(long* dataPointer)
{
	mDataPointer = dataPointer;
	mDataType = XdmfDataItemType::Int64();
}

template <>
void XdmfDataItem::setValues<float>(float* dataPointer)
{
	mDataPointer = dataPointer;
	mDataType = XdmfDataItemType::Float32();
}

template <>
void XdmfDataItem::setValues<double>(double* dataPointer)
{
	mDataPointer = dataPointer;
	mDataType = XdmfDataItemType::Float64();
}

template <typename T>
void XdmfDataItem::setValues(T* dataPointer)
{
	mDataPointer = dataPointer;
	mDataType = XdmfDataItemType::Unknown();
}

template <>
int* XdmfDataItem::getValues<int>()
{
	return (int*)mDataPointer;
}

template <>
const int* const XdmfDataItem::getValues<int>() const
{
	return (int*)mDataPointer;
}

int XdmfDataItem::getNumberValues() const
{
	return mNumberValues;
}

//void XdmfDataItem::write(boost::shared_ptr<XdmfVisitor> visitor) const
//{
//	visitor->visit(this);
//}
