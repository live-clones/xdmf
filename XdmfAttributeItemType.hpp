/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfAttributeItemType.hpp                                           */
/*                                                                           */
/*                                                                           */
/*  Author:                                                                  */
/*     Michal Habera                                                         */
/*     habera<at>karlin.mff.cuni.cz                                          */
/*                                                                           */
/*     This software is distributed WITHOUT ANY WARRANTY; without            */
/*     even the implied warranty of MERCHANTABILITY or FITNESS               */
/*     FOR A PARTICULAR PURPOSE.  See the above copyright notice             */
/*     for more information.                                                 */
/*                                                                           */
/*****************************************************************************/

#ifndef XDMFATTRIBUTEITEMTYPE_HPP_
#define XDMFATTRIBUTEITEMTYPE_HPP_

// C Compatible Includes
#include "Xdmf.hpp"

#ifdef __cplusplus

#include "XdmfItemProperty.hpp"

/**
 * @brief Property describing the type of values an XdmfAttribute
 * contains.
 *
 * XdmfAttributeItemType is a specific property of XdmfAttribute
 * used to specify what type of values XdmfAttribute contains.
 * Unlike XdmfAttributeType, this property does not specify rank
 * of values (scalar, vector, tensor, etc) but more general
 * framework is allowed. For example, FiniteElementFunction
 * defines how to interpret content of Attribute as a finite
 * element function.
 *
 * Xdmf supports the following attribute item types:
 *   FiniteElementFunction
 */

class XDMF_EXPORT XdmfAttributeItemType : public XdmfItemProperty {
{

public:

  virtual ~XdmfAttributeItemType();

  friend class XdmfAttribute;

  // Supported Xdmf Attribute Types
  static shared_ptr<const XdmfAttributeItemType> FiniteElementFunction();

  void
  getProperties(std::map<std::string, std::string> & collectedProperties) const;

protected:

  /**
   * Protected constructor for XdmfAttributeItemType.  The constructor is
   * protected because all attribute types supported by Xdmf should be
   * accessed through more specific static methods that construct
   * XdmfAttributeItemTypes - i.e.
   * XdmfAttributeItemType::FiniteElementFunction().
   *
   * @param     name    The name of the XdmfAttributeItemType to construct.
   */
  XdmfAttributeItemType(const std::string & name);

  static std::map<std::string, shared_ptr<const XdmfAttributeItemType>(*)()>
    mAttributeDefinitions;

  static void InitTypes();

private:

  XdmfAttributeItemType(const XdmfAttributeItemType &); // Not implemented.
  void operator=(const XdmfAttributeItemType &); // Not implemented.

  static shared_ptr<const XdmfAttributeItemType>
  New(const std::map<std::string, std::string> & itemProperties);

  std::string mName;
};

#endif

#ifdef __cplusplus
extern "C" {
#endif

// C wrappers go here

#define XDMF_ATTRIBUTE_ITEM_TYPE_FINITE_ELEMENT_FUNCTION                    300
#define XDMF_ATTRIBUTE_ITEM_TYPE_NOTYPE                                     301

XDMF_EXPORT int XdmfAttributeItemTypeFiniteElementFunction();
XDMF_EXPORT int XdmfAttributeItemTypeNoAttributeItemType();

#ifdef __cplusplus
}
#endif

#endif // XDMFATTRIBUTEITEMTYPE_HPP_
