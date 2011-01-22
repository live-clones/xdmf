#ifndef XDMFCOREITEMFACTORY_HPP_
#define XDMFCOREITEMFACTORY_HPP_

// Forward Declarations
class XdmfItem;

// Includes
#include "XdmfCore.hpp"
#include <boost/shared_ptr.hpp>
#include <map>
#include <vector>

/**
 * @brief Factory that constructs XdmfItems using tags and properties.
 *
 * XdmfCoreItemFactory is an abstract base class.
 */
class XDMFCORE_EXPORT XdmfCoreItemFactory {

 public:

  virtual ~XdmfCoreItemFactory() = 0;

  /**
   * Create a new XdmfItem.
   *
   * @param itemTag a string containing the tag of the XdmfItem to create.
   * @param itemProperties a map of key/value properties for the the XdmfItem.
   * @param childItems the children of the XdmfItem to create.
   *
   * @return constructed XdmfItem. If no XdmfItem can be constructed,
   * return NULL.
   */
  virtual boost::shared_ptr<XdmfItem>
  createItem(const std::string & itemTag,
             const std::map<std::string, std::string> & itemProperties,
             const std::vector<boost::shared_ptr<XdmfItem> > & childItems) const;

 protected:

  XdmfCoreItemFactory();

 private:

  XdmfCoreItemFactory(const XdmfCoreItemFactory &);  // Not implemented.
  void operator=(const XdmfCoreItemFactory &);  // Not implemented.

};

#endif /* XDMFCOREITEMFACTORY_HPP_ */
