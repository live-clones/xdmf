#ifndef XDMFITEMFACTORY_HPP_
#define XDMFITEMFACTORY_HPP_

// Forward Declarations
class XdmfItem;

// Includes
#include "XdmfCoreItemFactory.hpp"

/**
 * @brief Factory for constructing XdmfItems from their ItemTag and ItemProperties
 */
class XdmfItemFactory : public XdmfCoreItemFactory {

public:

	XdmfNewMacro(XdmfItemFactory);
	virtual ~XdmfItemFactory();

	/**
	 * Create a new XdmfItem.
	 *
	 * @param itemTag a string containing the tag of the XdmfItem to create.
	 * @param itemProperties a map of key/value properties for the the XdmfItem.
	 */
	virtual boost::shared_ptr<XdmfItem> createItem(const std::string & itemTag, const std::map<std::string, std::string> & itemProperties) const;

protected:

	XdmfItemFactory();

private:

	XdmfItemFactory(const XdmfItemFactory & itemFactory);  // Not implemented.
	void operator=(const XdmfItemFactory & itemFactory);  // Not implemented.

};

#endif /* XDMFITEMFACTORY_HPP_ */
