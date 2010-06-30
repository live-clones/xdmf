#ifndef XDMFATTRIBUTE_HPP_
#define XDMFATTRIBUTE_HPP_

// Forward Declarations
class XdmfAttributeCenter;
class XdmfAttributeType;

// Includes
#include "XdmfDataItem.hpp"

/**
 * @brief Handles computed values attached to an XdmfGrid.
 *
 * XdmfAttribute contains two properties that should be set, XdmfAttributeCenter and XdmfAttributeType.
 * XdmfAttribute is a subclass of XdmfDataItem, meaning it contains an XdmfArray to store values.
 */
class XdmfAttribute : public XdmfDataItem {

public:

	XdmfNewMacro(XdmfAttribute);
	virtual ~XdmfAttribute();

	LOKI_DEFINE_VISITABLE(XdmfAttribute, XdmfDataItem)
	static const std::string ItemTag;

	/**
	 * Get the XdmfAttributeCenter associated with this attribute.
	 *
	 * @return XdmfAttributeCenter of the attribute.
	 */
	boost::shared_ptr<const XdmfAttributeCenter> getAttributeCenter() const;

	/**
	 * Get the XdmfAttributeType associated with this attribute.
	 *
	 * @return XdmfAttributeType of the attribute.
	 */
	boost::shared_ptr<const XdmfAttributeType> getAttributeType() const;

	std::map<std::string, std::string> getItemProperties() const;

	std::string getItemTag() const;

	/**
	 * Get the name of the attribute.
	 *
	 * @return a string containing the name of the attribute.
	 */
	std::string getName() const;

	/**
	 * Set the XdmfAttributeCenter associated with this attribute.
	 *
	 * @param attributeCenter the XdmfAttributeCenter to set.
	 */
	void setAttributeCenter(const boost::shared_ptr<const XdmfAttributeCenter> attributeCenter);

	/**
	 * Set the XdmfAttributeType associated with this attribute.
	 *
	 * @param attributeType XdmfAttributeType to set.
	 */
	void setAttributeType(const boost::shared_ptr<const XdmfAttributeType> attributeType);

	/**
	 * Set the name of the attribute.
	 *
	 * @param name a string containing the name to set.
	 */
	void setName(const std::string & name);

protected:

	XdmfAttribute();
	virtual void populateItem(const std::map<std::string, std::string> & itemProperties, std::vector<boost::shared_ptr<XdmfItem> > & childItems);

private:

	XdmfAttribute(const XdmfAttribute & attribute);  // Not implemented.
	void operator=(const XdmfAttribute & attribute);  // Not implemented.

	std::string mName;
	boost::shared_ptr<const XdmfAttributeType> mAttributeType;
	boost::shared_ptr<const XdmfAttributeCenter> mAttributeCenter;
};

#endif /* XDMFATTRIBUTE_HPP_ */
