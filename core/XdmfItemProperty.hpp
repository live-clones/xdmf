#ifndef XDMFITEMPROPERTY_HPP_
#define XDMFITEMPROPERTY_HPP_

// Includes
#include <boost/shared_ptr.hpp>
#include <map>
#include <string>
#include "XdmfCore.hpp"

/**
 * @brief A property attached to an XdmfItem.
 *
 * XdmfItems can have zero or more properties attached to them that
 * describe a specific characteristic of that item. For instance,
 * XdmfAttributes have both center and type properties.
 *
 * This is an abstract base class to facilitate reading and writing of
 * properties in a generic way.
 */

class XDMFCORE_EXPORT XdmfItemProperty {

 public:

  virtual ~XdmfItemProperty() = 0;

  /**
   * Retrieve the key/value pairs that this XdmfItemProperty contains by
   * inserting into the passed map.
   *
   * @param collectedProperties a map to insert name / value pairs into.
   */
  virtual void
  getProperties(std::map<std::string, std::string> & collectedProperties) const = 0;

 protected:

  XdmfItemProperty();

 private:

  XdmfItemProperty(const XdmfItemProperty &);  // Not implemented.
  void operator=(const XdmfItemProperty &);  // Not implemented.

};

#endif /* XDMFITEMPROPERTY_HPP_ */
