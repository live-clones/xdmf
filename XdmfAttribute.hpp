/*
 * XdmfAttribute.hpp
 *
 *  Created on: Jan 25, 2010
 *      Author: kleiter
 */

#ifndef XDMFATTRIBUTE_HPP_
#define XDMFATTRIBUTE_HPP_

#include "XdmfAttributeCenter.hpp"
#include "XdmfAttributeType.hpp"
#include "XdmfDataItem.hpp"

class XdmfAttribute : public XdmfDataItem {

public:

	XdmfNewMacro(XdmfAttribute);

	/**
	 * Get the name of the attribute.
	 *
	 * @return a string containing the name of the attribute.
	 */
	std::string getName() const;

	/**
	 * Set the name of the attribute.
	 *
	 * @param a string containing the name to set.
	 */
	void setName(const std::string&);

	/**
	 * Get the XdmfAttributeType associated with this Attribute.
	 *
	 * @return XdmfAttributeType.
	 */
	XdmfAttributeType getAttributeType() const;

	/**
	 * Set the XdmfAttributeType associated with this Attribute.
	 *
	 * @param XdmfAttributeType to set.
	 */
	void setAttributeType(const XdmfAttributeType& attributeType);

	/**
	 * Get the name of the AttributeType associated with this Attribute.
	 *
	 * @return std::string containing name of the AttributeType.
	 */
	std::string getAttributeTypeAsString() const;

	/**
	 * Get the XdmfAttributeCenter associated with this Attribute.
	 *
	 * @return XdmfAttributeCenter.
	 */
	XdmfAttributeCenter getAttributeCenter() const;

	/**
	 * Set the XdmfAttributeCenter associated with this Attribute.
	 *
	 * @param XdmfAttributeCenter to set.
	 */
	void setAttributeCenter(const XdmfAttributeCenter& attributeCenter);

	/**
	 * Get the name of the AttributeCenter associated with this Attribute.
	 *
	 * @return std::string containing name of the AttributeCenter.
	 */
	std::string getAttributeCenterAsString() const;

	void write(boost::shared_ptr<XdmfVisitor> visitor) const;

	virtual std::string printSelf() const;

	void setNumberValues(int numValues);

protected:

	XdmfAttribute();
	virtual ~XdmfAttribute();

private:

	XdmfAttribute(const XdmfAttribute&);  // Not implemented.
	void operator=(const XdmfAttribute&);  // Not implemented.

	std::string mName;
	XdmfAttributeType mAttributeType;
	XdmfAttributeCenter mAttributeCenter;

};


#endif /* XDMFATTRIBUTE_HPP_ */
