#ifndef XDMFATTRIBUTETYPE_HPP_
#define XDMFATTRIBUTETYPE_HPP_

// Includes
#include <string>
#include "XdmfItemProperty.hpp"

/**
 * @brief Property describing what types of values an XdmfAttribute contains.
 *
 * XdmfAttributeType is a property used by XdmfAttribute to specify what types of values the XdmfAttribute
 * contains.  A specific XdmfAttributeType can be created by calling one of the static methods
 * in the class, i.e. XdmfAttributeType::Scalar().
 *
 * Xdmf supports the following attribute types:
 * 	NoAttributeType
 * 	Scalar
 * 	Vector
 * 	Tensor
 * 	Matrix
 * 	Tensor6
 * 	GlobalId
 */
class XdmfAttributeType : public XdmfItemProperty {

public:

	friend class XdmfAttribute;

	// Supported Xdmf Attribute Types
	static XdmfAttributeType NoAttributeType();
	static XdmfAttributeType Scalar();
	static XdmfAttributeType Vector();
	static XdmfAttributeType Tensor();
	static XdmfAttributeType Matrix();
	static XdmfAttributeType Tensor6();
	static XdmfAttributeType GlobalId();

	void getProperties(std::map<std::string, std::string> & collectedProperties) const;

	/*
	 * Compare two XdmfAttributeTypes for equality.
	 *
	 * @param attributeType an XdmfAttributeType to compare equality to.
	 * @return true if the XdmfAttributeTypes are equal.
	 */
	bool operator==(const XdmfAttributeType & attributeType) const;

	/**
	 * Compare two XdmfAttributeTypes for inequality.
	 *
	 * @param attributeType an XdmfAttributeType to compare inequality to.
	 * @return true if the XdmfAttributeTypes are not equal.
	 */
	bool operator!=(const XdmfAttributeType & attributeType) const;

	XdmfAttributeType(const XdmfAttributeType & attributeType);
	XdmfAttributeType & operator=(const XdmfAttributeType & attributeType);

protected:

	/**
	 * Protected constructor for XdmfAttributeType.  The constructor is protected because all attribute types supported
	 * by Xdmf should be accessed through more specific static methods that construct XdmfAttributeTypes - i.e.
	 * XdmfAttributeType::Scalar().
	 *
	 * @param name the name of the XdmfAttributeType to construct.
	 */
	XdmfAttributeType(const std::string & name);

private:

	static XdmfAttributeType New(const std::map<std::string, std::string> & itemProperties);

	std::string mName;
};

#endif /* XDMFATTRIBUTETYPE_HPP_ */
