/*
 * XdmfAttributeType.hpp
 *
 *  Created on: Jan 25, 2010
 *      Author: kleiter
 */

#ifndef XDMFATTRIBUTETYPE_HPP_
#define XDMFATTRIBUTETYPE_HPP_

#include <string>

class XdmfAttributeType {

public:

	// Supported Xdmf Attribute Types
	static XdmfAttributeType NoAttributeType();
	static XdmfAttributeType Scalar();
	static XdmfAttributeType Vector();
	static XdmfAttributeType Tensor();
	static XdmfAttributeType Matrix();
	static XdmfAttributeType Tensor6();
	static XdmfAttributeType GlobalId();

	/**
	 * Get the name of this attribute type
	 *
	 * @return a string containing the name.
	 */
	std::string getName() const;

	/*
	 * Compare two XdmfAttributeTypes for equality.
	 *
	 * @param an XdmfAttributeType to compare equality to.
	 * @return true iff the XdmfAttributeTypes are equal.
	 */
	bool operator==(const XdmfAttributeType& attributeType) const;

	/**
	 * Compare two XdmfAttributeTypes for inequality.
	 *
	 * @param XdmfAttributeType to compare inequality to.
	 * @return true iff the XdmfAttributeTypes are not equal.
	 */
	bool operator!=(const XdmfAttributeType& top) const;

	XdmfAttributeType(const XdmfAttributeType&);
	XdmfAttributeType& operator=(const XdmfAttributeType&);

protected:

	/**
	 * Protected constructor for XdmfAttributeType.  The constructor is protected because all attribute types supported
	 * by Xdmf should be accessed through more specific static methods that construct XdmfAttributeTypes - i.e. XdmfAttributeType::Scalar().
	 */
	XdmfAttributeType(const std::string& name);

private:

	std::string mName;
};

#endif /* XDMFATTRIBUTETYPE_HPP_ */
