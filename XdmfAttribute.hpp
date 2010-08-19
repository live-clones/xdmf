#ifndef XDMFATTRIBUTE_HPP_
#define XDMFATTRIBUTE_HPP_

// Forward Declarations
class XdmfAttributeCenter;
class XdmfAttributeType;

// Includes
#include "XdmfArray.hpp"

/**
 * @brief Handles computed values attached to an XdmfGrid.
 *
 * XdmfAttribute contains two properties that should be set, XdmfAttributeCenter and XdmfAttributeType.
 * XdmfAttribute is a subclass of XdmfDataItem, meaning it contains an XdmfArray to store values.
 */
class XdmfAttribute : public XdmfArray {

public:

	/**
	 * Create a new XdmfAttribute.
	 *
	 * @return constructed XdmfAttribute.
	 */
	static boost::shared_ptr<XdmfAttribute> New();

	virtual ~XdmfAttribute();

	LOKI_DEFINE_VISITABLE(XdmfAttribute, XdmfArray)
	static const std::string ItemTag;

	/**
	 * Get the XdmfAttributeCenter associated with this attribute.
	 *
	 * @return XdmfAttributeCenter of the attribute.
	 */
	boost::shared_ptr<const XdmfAttributeCenter> getCenter() const;


	std::map<std::string, std::string> getItemProperties() const;

	std::string getItemTag() const;

	/**
	 * Get the name of the attribute.
	 *
	 * @return a string containing the name of the attribute.
	 */
	std::string getName() const;

	/**
	 * Get the XdmfAttributeType associated with this attribute.
	 *
	 * @return XdmfAttributeType of the attribute.
	 */
	boost::shared_ptr<const XdmfAttributeType> getType() const;

	/**
	 * Set the XdmfAttributeCenter associated with this attribute.
	 *
	 * @param attributeCenter the XdmfAttributeCenter to set.
	 */
	void setCenter(const boost::shared_ptr<const XdmfAttributeCenter> attributeCenter);

	/**
	 * Set the name of the attribute.
	 *
	 * @param name a string containing the name to set.
	 */
	void setName(const std::string & name);

	/**
	 * Set the XdmfAttributeType associated with this attribute.
	 *
	 * @param attributeType XdmfAttributeType to set.
	 */
	void setType(const boost::shared_ptr<const XdmfAttributeType> attributeType);

protected:

	XdmfAttribute();
	virtual void populateItem(const std::map<std::string, std::string> & itemProperties, std::vector<boost::shared_ptr<XdmfItem> > & childItems, const XdmfCoreReader * const reader);

private:

	XdmfAttribute(const XdmfAttribute & attribute);  // Not implemented.
	void operator=(const XdmfAttribute & attribute);  // Not implemented.

	std::string mName;
	boost::shared_ptr<const XdmfAttributeCenter> mAttributeCenter;
	boost::shared_ptr<const XdmfAttributeType> mAttributeType;
};

#endif /* XDMFATTRIBUTE_HPP_ */
