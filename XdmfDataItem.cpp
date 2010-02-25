/*
 * XdmfDataItem.cpp
 *
 *  Created on: Jan 29, 2010
 *      Author: kleiter
 */

#include "XdmfDataItem.hpp"
#include "XdmfVisitor.hpp"

XdmfDataItem::XdmfDataItem() :
	mDataItemType(XdmfDataItemType::Unknown()),
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
	mDataItemType = XdmfDataItemType::Int8();
}

template <>
void XdmfDataItem::setValues<unsigned char>(unsigned char* dataPointer)
{
	mDataPointer = dataPointer;
	mDataItemType = XdmfDataItemType::UInt8();
}

template <>
void XdmfDataItem::setValues<short>(short* dataPointer)
{
	mDataPointer = dataPointer;
	mDataItemType = XdmfDataItemType::Int16();
}

template <>
void XdmfDataItem::setValues<unsigned short>(unsigned short* dataPointer)
{
	mDataPointer = dataPointer;
	mDataItemType = XdmfDataItemType::UInt16();
}

template <>
void XdmfDataItem::setValues<int>(int* dataPointer)
{
	mDataPointer = dataPointer;
	mDataItemType = XdmfDataItemType::Int32();
}

template <>
void XdmfDataItem::setValues<unsigned int>(unsigned int* dataPointer)
{
	mDataPointer = dataPointer;
	mDataItemType = XdmfDataItemType::UInt32();
}

template <>
void XdmfDataItem::setValues<long>(long* dataPointer)
{
	mDataPointer = dataPointer;
	mDataItemType = XdmfDataItemType::Int64();
}

template <>
void XdmfDataItem::setValues<float>(float* dataPointer)
{
	mDataPointer = dataPointer;
	mDataItemType = XdmfDataItemType::Float32();
}

template <>
void XdmfDataItem::setValues<double>(double* dataPointer)
{
	mDataPointer = dataPointer;
	mDataItemType = XdmfDataItemType::Float64();
}

template <typename T>
void XdmfDataItem::setValues(T* dataPointer)
{
	mDataPointer = dataPointer;
	mDataItemType = XdmfDataItemType::Unknown();
}

void* XdmfDataItem::getValues()
{
	return mDataPointer;
}

const void* const XdmfDataItem::getValues() const
{
	return mDataPointer;
}

int XdmfDataItem::getNumberValues() const
{
	return mNumberValues;
}

hid_t XdmfDataItem::getHDF5DataType() const
{
	return mDataItemType.getHDF5DataType();
}

std::string XdmfDataItem::getName() const
{
	return mDataItemType.getName();
}

int XdmfDataItem::getPrecision() const
{
	return mDataItemType.getPrecision();
}

//std::string getValuesAsString() const
//{

//}

/*
template <typename T>
T* XdmfDataItem::getValues()
{
	return (T*)mDataPointer;
}

template <typename T>
const T* const XdmfDataItem::getValues() const
{
	return (T*)mDataPointer;
}
*/

//void XdmfDataItem::write(boost::shared_ptr<XdmfVisitor> visitor) const
//{
//	visitor->visit(this);
//}
