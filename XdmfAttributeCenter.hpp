/*
 * XdmfAttributeCenter.hpp
 *
 *  Created on: Jan 25, 2010
 *      Author: kleiter
 */

#ifndef XDMFATTRIBUTECENTER_HPP_
#define XDMFATTRIBUTECENTER_HPP_

#include <string>

class XdmfAttributeCenter {

public:

	// Supported Xdmf Attribute Centers
	static XdmfAttributeCenter Grid();
	static XdmfAttributeCenter Cell();
	static XdmfAttributeCenter Face();
	static XdmfAttributeCenter Edge();
	static XdmfAttributeCenter Node();

	/**
	 * Get the name of this center type
	 *
	 * @return a string containing the name.
	 */
	std::string getName() const;

	/*
	 * Compare two XdmfAttributeCenters for equality.
	 *
	 * @param an XdmfAttributeCenter to compare equality to.
	 * @return true iff the XdmfAttributeCenters are equal.
	 */
	bool operator==(const XdmfAttributeCenter& attributeCenter) const;

	/**
	 * Compare two XdmfAttributeCenters for inequality.
	 *
	 * @param XdmfAttributeCenter to compare inequality to.
	 * @return true iff the XdmfAttributeCenters are not equal.
	 */
	bool operator!=(const XdmfAttributeCenter& attributeCenter) const;

	XdmfAttributeCenter(const XdmfAttributeCenter& attributeCenter);
	XdmfAttributeCenter& operator=(const XdmfAttributeCenter& attributeCenter);

protected:

	/**
	 * Protected constructor for XdmfAttributeCenter.  The constructor is protected because all attribute centers supported
	 * by Xdmf should be accessed through more specific static methods that construct XdmfAttributeCenters - i.e. XdmfAttributeCenter::Node().
	 */
	XdmfAttributeCenter(const std::string& name);

private:

	std::string mName;
};

#endif /* XDMFATTRIBUTECENTER_HPP_ */
