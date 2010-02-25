/*
 * XdmfDataItemType.hpp
 *
 *  Created on: Jan 25, 2010
 *      Author: kleiter
 */

#ifndef XDMFDATAITEMTYPE_HPP_
#define XDMFDATAITEMTYPE_HPP_

#include <string>
#include <hdf5.h>

class XdmfDataItemType {

public:

	// Supported Xdmf Data Item Types
	static XdmfDataItemType Unknown();
	static XdmfDataItemType Int8();
	static XdmfDataItemType Int16();
	static XdmfDataItemType Int32();
	static XdmfDataItemType Int64();
	static XdmfDataItemType Float32();
	static XdmfDataItemType Float64();
	static XdmfDataItemType UInt8();
	static XdmfDataItemType UInt16();
	static XdmfDataItemType UInt32();

	/**
	 * Get the name of this data item type.
	 *
	 * @return a string containing the name.
	 */
	std::string getName() const;

	/**
	 * Return the equivalent HDF5 data type for this data item type.
	 *
	 * @return a hid_t giving the equivalent HDF5 data type.
	 */
	hid_t getHDF5DataType() const;

	/**
	 * Return the precision(number of bytes) of this data item type.
	 *
	 * @return an int containing the precision.
	 */
	int getPrecision() const;

	/*
	 * Compare two XdmfDataItemTypes for equality.
	 *
	 * @param an XdmfDataItemType to compare equality to.
	 * @return true iff the XdmfDataItemTypes are equal.
	 */
	bool operator==(const XdmfDataItemType& attributeType) const;

	/**
	 * Compare two XdmfDataItemTypes for inequality.
	 *
	 * @param XdmfDataItemType to compare inequality to.
	 * @return true iff the XdmfDataItemTypes are not equal.
	 */
	bool operator!=(const XdmfDataItemType& top) const;

	XdmfDataItemType(const XdmfDataItemType& dataItemType);
	XdmfDataItemType& operator=(const XdmfDataItemType& dataItemType);

protected:

	/**
	 * Protected constructor for XdmfDataItemType.  The constructor is protected because all data item types supported
	 * by Xdmf should be accessed through more specific static methods that construct XdmfDataItemTypes - i.e. XdmfDataItemType::Int8().
	 */
	XdmfDataItemType(const std::string& name, hid_t HDFDataType, int precision);

private:

	static XdmfDataItemType* UnknownPtr;
	std::string mName;
	hid_t mHDF5DataType;
	int mPrecision;
};

#endif /* XDMFDATAITEMTYPE_HPP_ */
