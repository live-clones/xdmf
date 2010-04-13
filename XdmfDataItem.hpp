/*
 * XdmfDataItem.hpp
 *
 *
 *  Created on: Jan 25, 2010
 *      Author: kleiter
 */

#ifndef XDMFDATAITEM_HPP_
#define XDMFDATAITEM_HPP_

// Forward Declarations
class XdmfArray;

// Includes
#include "XdmfItem.hpp"

class XdmfDataItem : public XdmfItem {

public:

	/**
	 * Attach an array to this dataitem.
	 *
	 * @param a smart pointer to the array.
	 */
	void setArray(boost::shared_ptr<XdmfArray> array);

	/**
	 * Get the array attached to this dataitem.
	 *
	 * @return a smart pointer to the array.
	 */
	boost::shared_ptr<XdmfArray> getArray();

	/**
	 * Get the array attached to this dataitem (const version).
	 *
	 * @return a smart pointer to the array.
	 */
	boost::shared_ptr<const XdmfArray> getArray() const;

	std::string printSelf() const;

	virtual void traverse(boost::shared_ptr<XdmfVisitor> visitor) const;

protected:

	XdmfDataItem();
	virtual ~XdmfDataItem();

private:

	XdmfDataItem(const XdmfDataItem&);  // Not implemented.
	void operator=(const XdmfDataItem&);  // Not implemented.

	boost::shared_ptr<XdmfArray> mArray;
};

#endif /* XDMFDATAITEM_HPP_ */
