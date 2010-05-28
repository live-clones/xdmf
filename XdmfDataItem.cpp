/*
 * XdmfDataItem.cpp
 *
 *  Created on: Jan 29, 2010
 *      Author: kleiter
 */

#include "XdmfArray.hpp"
#include "XdmfDataItem.hpp"

XdmfDataItem::XdmfDataItem() :
	mArray(XdmfArray::New())
{
	std::cout << "Created DataItem " << this << std::endl;
}

XdmfDataItem::~XdmfDataItem()
{
	std::cout << "Deleted DataItem " << this << std::endl;
}

boost::shared_ptr<XdmfArray> XdmfDataItem::getArray()
{
	return mArray;
}

boost::shared_ptr<const XdmfArray> XdmfDataItem::getArray() const
{
	return mArray;
}

void XdmfDataItem::populateItem(const std::map<std::string, std::string> & itemProperties, std::vector<boost::shared_ptr<XdmfItem> > & childItems)
{
	for(std::vector<boost::shared_ptr<XdmfItem> >::const_iterator iter = childItems.begin(); iter != childItems.end(); ++iter)
	{
		if(boost::shared_ptr<XdmfArray> array = boost::shared_dynamic_cast<XdmfArray>(*iter))
		{
			this->setArray(array);
		}
	}
}

void XdmfDataItem::setArray(const boost::shared_ptr<XdmfArray> array)
{
	mArray = array;
}

void XdmfDataItem::traverse(const boost::shared_ptr<XdmfBaseVisitor> visitor) const
{
	mArray->accept(visitor);
}
