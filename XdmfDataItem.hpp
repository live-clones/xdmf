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

	//XdmfNewMacro(XdmfDataItem);

	template <typename T> void setValues(T * dataPointer);

	template <typename T> T* getValues();

	template <typename T> const T* const getValues() const;

	int getNumberValues() const;

	std::string printSelf() const;

	void write(boost::shared_ptr<XdmfVisitor> visitor) const = 0;

protected:

	int mNumberValues;

	XdmfDataItem();
	virtual ~XdmfDataItem();

private:

	XdmfDataItem(const XdmfDataItem&);  // Not implemented.
	void operator=(const XdmfDataItem&);  // Not implemented.

	void* mDataPointer;
	XdmfDataItemType mDataType;
};

#endif /* XDMFDATAITEM_HPP_ */
