#ifndef XDMFITEMFACTORY_HPP_
#define XDMFITEMFACTORY_HPP_

// Forward Declarations
class XdmfItem;

// Includes
#include "Xdmf.hpp"
#include "XdmfCoreItemFactory.hpp"

/**
 * @brief Factory for constructing XdmfItems from their ItemTag and
 * ItemProperties.
 */
class XDMF_EXPORT XdmfItemFactory : public XdmfCoreItemFactory {

 public:

  /**
   * Create a new XdmfItemFactory.
   *
   * @return constructed XdmfItemFactory.
   */
  static boost::shared_ptr<XdmfItemFactory> New();

  virtual ~XdmfItemFactory();

  virtual boost::shared_ptr<XdmfItem>
  createItem(const std::string & itemTag,
             const std::map<std::string, std::string> & itemProperties,
             const std::vector<boost::shared_ptr<XdmfItem> > & childItems) const;

 protected:

  XdmfItemFactory();

 private:

  XdmfItemFactory(const XdmfItemFactory &);  // Not implemented.
  void operator=(const XdmfItemFactory &);  // Not implemented.

};

#endif /* XDMFITEMFACTORY_HPP_ */
