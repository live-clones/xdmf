/*
 * XdmfAttribute.hpp
 *
 *  Created on: Jan 25, 2010
 *      Author: kleiter
 */

#ifndef XDMFATTRIBUTE_HPP_
#define XDMFATTRIBUTE_HPP_

#include "XdmfItem.hpp"

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
	bool operator==(const XdmfAttributeCenter& attributeCenter) const
	{
		return mName.compare(attributeCenter.mName) == 0;
	}

	/**
	 * Compare two XdmfAttributeCenters for inequality.
	 *
	 * @param XdmfAttributeCenter to compare inequality to.
	 * @return true iff the XdmfAttributeCenters are not equal.
	 */
	bool operator!=(const XdmfAttributeCenter& attributeCenter) const
	{
		return !this->operator==(attributeCenter);
	}

	XdmfAttributeCenter(const XdmfAttributeCenter&);
	XdmfAttributeCenter& operator=(const XdmfAttributeCenter&);

protected:

	/**
	 * Protected constructor for XdmfAttributeCenter.  The constructor is protected because all attribute centers supported
	 * by Xdmf should be accessed through more specific static methods that construct XdmfAttributeCenters - i.e. XdmfAttributeCenter::Node().
	 */
	XdmfAttributeCenter(const std::string& name) :
		mName(name)
	{};

private:

	std::string mName;
};

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
	 * Compare two XdmfAttribute types for equality.
	 *
	 * @param an XdmfAttributeType to compare equality to.
	 * @return true iff the XdmfAttributeTypes are equal.
	 */
	bool operator==(const XdmfAttributeType& top) const
	{
		return mName.compare(top.mName) == 0;
	}

	/**
	 * Compare two XdmfAttribute types for inequality.
	 *
	 * @param XdmfAttributeType to compare inequality to.
	 * @return true iff the XdmfAttributeTypes are not equal.
	 */
	bool operator!=(const XdmfAttributeType& top) const
	{
		return !this->operator ==(top);
	}

	XdmfAttributeType(const XdmfAttributeType&);
	XdmfAttributeType& operator=(const XdmfAttributeType&);

protected:

	/**
	 * Protected constructor for XdmfAttributeType.  The constructor is protected because all attribute types supported
	 * by Xdmf should be accessed through more specific static methods that construct XdmfAttributeTypes - i.e. XdmfAttributeType::Scalar().
	 */
	XdmfAttributeType(const std::string& name) :
		mName(name)
	{};

private:

	std::string mName;
};

class XdmfAttribute : public XdmfItem {

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
	void setAttributeType(const XdmfAttributeType&);

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
	void setAttributeCenter(const XdmfAttributeCenter&);

	/**
	 * Get the name of the AttributeCenter associated with this Attribute.
	 *
	 * @return std::string containing name of the AttributeCenter.
	 */
	std::string getAttributeCenterAsString() const;

	void write(boost::shared_ptr<XdmfVisitor> visitor) const;

	virtual const std::string printSelf() const;

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
