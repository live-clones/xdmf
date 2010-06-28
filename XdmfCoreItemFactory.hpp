#ifndef XDMFCOREITEMFACTORY_HPP_
#define XDMFCOREITEMFACTORY_HPP_

// Forward Declarations
class XdmfItem;

// Includes
#include <map>
#include "XdmfObject.hpp"

/**
 * @brief Factory for constructing XdmfItems from their ItemTag and ItemProperties
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
	 */
	virtual boost::shared_ptr<XdmfItem> createItem(const std::string & itemTag, const std::map<std::string, std::string> & itemProperties) const = 0;

protected:

	XdmfCoreItemFactory();

private:

	XdmfCoreItemFactory(const XdmfCoreItemFactory & itemFactory);  // Not implemented.
	void operator=(const XdmfCoreItemFactory & itemFactory);  // Not implemented.

};

#endif /* XDMFCOREITEMFACTORY_HPP_ */
