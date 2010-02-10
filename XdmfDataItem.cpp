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
void XdmfDataItem::setData<int>(int* dataPointer)
{
	mDataPointer = dataPointer;
	mDataType = XdmfDataItemType::Int32();
}

template <>
int* XdmfDataItem::getData<int>()
{
	return mDataPointer;
}

template <>
const int* const XdmfDataItem::getData<int>() const
{
	return mDataPointer;
}

int XdmfDataItem::getNumberValues() const
{
	return mNumberValues;
}

void XdmfDataItem::write(boost::shared_ptr<XdmfVisitor> visitor) const
{
	visitor->visit(this);
}
