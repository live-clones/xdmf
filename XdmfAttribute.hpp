/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfAttribute.hpp                                                   */
/*                                                                           */
/*  Author:                                                                  */
/*     Kenneth Leiter                                                        */
/*     kenneth.leiter@arl.army.mil                                           */
/*     US Army Research Laboratory                                           */
/*     Aberdeen Proving Ground, MD                                           */
/*                                                                           */
/*     Copyright @ 2011 US Army Research Laboratory                          */
/*     All Rights Reserved                                                   */
/*     See Copyright.txt for details                                         */
/*                                                                           */
/*     This software is distributed WITHOUT ANY WARRANTY; without            */
/*     even the implied warranty of MERCHANTABILITY or FITNESS               */
/*     FOR A PARTICULAR PURPOSE.  See the above copyright notice             */
/*     for more information.                                                 */
/*                                                                           */
/*****************************************************************************/

#ifndef XDMFATTRIBUTE_HPP_
#define XDMFATTRIBUTE_HPP_

// Forward Declarations
class XdmfAttributeCenter;
class XdmfAttributeType;

// Includes
#include "Xdmf.hpp"
#include "XdmfArray.hpp"

/**
 * @brief Holds values located at specific parts of an XdmfGrid.
 *
 * XdmfAttribute holds values centered at specific locations of an
 * XdmfGrid. An attribute contains two properties that should be set,
 * XdmfAttributeCenter, which describes where the values are centered,
 * and XdmfAttributeType, which describes what types of values the
 * attribute contains.
 */
class XDMF_EXPORT XdmfAttribute : public XdmfArray {

public:

  /**
   * Create a new XdmfAttribute.
   *
   * @return constructed XdmfAttribute.
   */
  static shared_ptr<XdmfAttribute> New();

  virtual ~XdmfAttribute();

  LOKI_DEFINE_VISITABLE(XdmfAttribute, XdmfArray);
  static const std::string ItemTag;

  /**
   * Get the XdmfAttributeCenter associated with this attribute.
   *
   * @return XdmfAttributeCenter of the attribute.
   */
  shared_ptr<const XdmfAttributeCenter> getCenter() const;

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
  shared_ptr<const XdmfAttributeType> getType() const;

  /**
   * Set the XdmfAttributeCenter associated with this attribute.
   *
   * @param center the XdmfAttributeCenter to set.
   */
  void setCenter(const shared_ptr<const XdmfAttributeCenter> center);

  /**
   * Set the name of the attribute.
   *
   * @param name a string containing the name to set.
   */
  void setName(const std::string & name);

  /**
   * Set the XdmfAttributeType associated with this attribute.
   *
   * @param type XdmfAttributeType to set.
   */
  void setType(const shared_ptr<const XdmfAttributeType> type);

protected:

  XdmfAttribute();

  virtual void
  populateItem(const std::map<std::string, std::string> & itemProperties,
               std::vector<shared_ptr<XdmfItem> > & childItems,
               const XdmfCoreReader * const reader);

private:

  XdmfAttribute(const XdmfAttribute &);  // Not implemented.
  void operator=(const XdmfAttribute &);  // Not implemented.

  shared_ptr<const XdmfAttributeCenter> mCenter;
  std::string mName;
  shared_ptr<const XdmfAttributeType> mType;
};

#ifdef _WIN32
XDMF_TEMPLATE template class XDMF_EXPORT
shared_ptr<const XdmfAttributeType>;
XDMF_TEMPLATE template class XDMF_EXPORT
shared_ptr<const XdmfAttributeCenter>;
#endif

#endif /* XDMFATTRIBUTE_HPP_ */
