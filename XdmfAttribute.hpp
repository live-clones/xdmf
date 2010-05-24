#ifndef XDMFATTRIBUTE_HPP_
#define XDMFATTRIBUTE_HPP_

// Includes
#include "XdmfAttributeCenter.hpp"
#include "XdmfAttributeType.hpp"
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
	LOKI_DEFINE_VISITABLE(XdmfAttribute, XdmfDataItem)
	static std::string ItemTag;

	/**
	 * Get the XdmfAttributeCenter associated with this attribute.
	 *
	 * @return XdmfAttributeCenter of the attribute.
	 */
	XdmfAttributeCenter getAttributeCenter() const;

	/**
	 * Get the XdmfAttributeType associated with this attribute.
	 *
	 * @return XdmfAttributeType of the attribute.
	 */
	XdmfAttributeType getAttributeType() const;

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
	void setAttributeCenter(const XdmfAttributeCenter & attributeCenter);

	/**
	 * Set the XdmfAttributeType associated with this attribute.
	 *
	 * @param attributeType XdmfAttributeType to set.
	 */
	void setAttributeType(const XdmfAttributeType & attributeType);

	/**
	 * Set the name of the attribute.
	 *
	 * @param name a string containing the name to set.
	 */
	void setName(const std::string & name);

protected:

	XdmfAttribute();
	virtual ~XdmfAttribute();
	virtual void populateItem(const std::map<std::string, std::string> & itemProperties, std::vector<boost::shared_ptr<XdmfItem> > & childItems);

private:

	XdmfAttribute(const XdmfAttribute & attribute);  // Not implemented.
	void operator=(const XdmfAttribute & attribute);  // Not implemented.

	std::string mName;
	XdmfAttributeType mAttributeType;
	XdmfAttributeCenter mAttributeCenter;
};

#endif /* XDMFATTRIBUTE_HPP_ */
