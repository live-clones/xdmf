/*
 * XdmfDataItem.hpp
 *
 *
 *  Created on: Jan 25, 2010
 *      Author: kleiter
 */

#ifndef XDMFDATAITEM_HPP_
#define XDMFDATAITEM_HPP_

#include "XdmfDataItemType.hpp"
#include "XdmfItem.hpp"

class XdmfDataItem : public XdmfItem {

public:

	XdmfNewMacro(XdmfDataItem);

	template <typename T> void setData(T * dataPointer);

	template <typename T> T* getData();

	template <typename T> const T* const getData() const;

	int getNumberValues() const;

	void setNumberValues(int numValues);

	std::string printSelf() const;

	void write(boost::shared_ptr<XdmfVisitor> visitor) const;

protected:

	XdmfDataItem();
	virtual ~XdmfDataItem();

private:

	XdmfDataItem(const XdmfDataItem&);  // Not implemented.
	void operator=(const XdmfDataItem&);  // Not implemented.

	int* mDataPointer;
	XdmfDataItemType mDataType;
	int mNumberValues;
};

#endif /* XDMFDATAITEM_HPP_ */
