#ifndef XDMFINFORMATION_HPP_
#define XDMFINFORMATION_HPP_

// Includes
#include "XdmfItem.hpp"

/**
 * @brief Holds a key/value pair that can be attached to an Xdmf structure.
 *
 * This can useful for storing input parameters to a code or for general information like runtime.
 */
class XdmfInformation : public XdmfItem {

public:

	XdmfNewMacro(XdmfInformation);
	virtual ~XdmfInformation();

	LOKI_DEFINE_VISITABLE(XdmfInformation, XdmfItem)
	static const std::string ItemTag;

	std::map<std::string, std::string> getItemProperties() const;

	std::string getItemTag() const;

	/**
	 * Get the key for this information item.
	 *
	 * @return std::string containing the key.
	 */
	std::string getKey() const;

	/**
	 * Get the value for this information item.
	 *
	 * @return std::string containing the value.
	 */
	std::string getValue() const;

	/**
	 * Set the key for this information item.
	 *
	 * @param key a string containing the key to set.
	 */
	void setKey(const std::string & key);

	/**
	 * Set the value for this information item.
	 *
	 * @param value a string containing the value to set.
	 */
	void setValue(const std::string & value);

protected:

	XdmfInformation();
	virtual void populateItem(const std::map<std::string, std::string> & itemProperties, std::vector<boost::shared_ptr<XdmfItem> > & childItems);

private:

	XdmfInformation(const XdmfInformation & information);  // Not implemented.
	void operator=(const XdmfInformation & information);  // Not implemented.

	std::string mKey;
	std::string mValue;
};

#endif /* XDMFINFORMATION_HPP_ */
