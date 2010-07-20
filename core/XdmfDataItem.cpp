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
	return boost::const_pointer_cast<XdmfArray>(static_cast<const XdmfDataItem &>(*this).getArray());
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
			if(mArray->getSize() == 0)
			{
				this->setArray(array);
			}
			else
			{
				// Support old Xdmf format that could contain multiple data items.  Just append the values to the first array.
				if(!mArray->isInitialized())
				{
					mArray->read();
				}
				const unsigned int oldSize = mArray->getSize();
				mArray->resize(oldSize + array->getSize(), 0);
				mArray->copyValues(oldSize, array, 0, array->getSize(), 1, 1);
			}
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
