#ifndef XDMFCOREITEMFACTORY_HPP_
#define XDMFCOREITEMFACTORY_HPP_

// Forward Declarations
class XdmfItem;

// Includes
#include <map>
#include "XdmfObject.hpp"

/**
 * @brief Factory that constructs XdmfItems using tags and properties.
 *
 * XdmfCoreItemFactory is an abstract base class.
 */
class XdmfCoreItemFactory : public XdmfObject {

public:

	virtual ~XdmfCoreItemFactory();

	/**
	 * Create a new XdmfItem.
	 *
	 * @param itemTag a string containing the tag of the XdmfItem to create.
	 * @param itemProperties a map of key/value properties for the the XdmfItem.
	 * @return constructed XdmfItem.  If no XdmfItem can be constructed, return NULL.
	 */
	virtual boost::shared_ptr<XdmfItem> createItem(const std::string & itemTag, const std::map<std::string, std::string> & itemProperties) const;

protected:

	XdmfCoreItemFactory();

private:

	XdmfCoreItemFactory(const XdmfCoreItemFactory & itemFactory);  // Not implemented.
	void operator=(const XdmfCoreItemFactory & itemFactory);  // Not implemented.

};

#endif /* XDMFCOREITEMFACTORY_HPP_ */
