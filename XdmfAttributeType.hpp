/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfAttributeType.hpp                                               */
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

#ifndef XDMFATTRIBUTETYPE_HPP_
#define XDMFATTRIBUTETYPE_HPP_

// Includes
#include "Xdmf.hpp"
#include "XdmfItemProperty.hpp"

/**
 * @brief Property describing the type of values an XdmfAttribute
 * contains.
 *
 * XdmfAttributeType is a property used by XdmfAttribute to specify
 * what type of values the XdmfAttribute contains. A specific
 * XdmfAttributeType can be created by calling one of the static
 * methods in the class, i.e. XdmfAttributeType::Scalar().
 *
 * Example of use:
 *
 * //Assuming that exampleAttribute is a shared pointer to an XdmfAttribute that has had its type set
 * if (exampleAttribute->getType() == XdmfAttributeType:Scalar())
 * {
 *   //do whatever is to be done if the attribute is a scalar
 * }
 *
 * Xdmf supports the following attribute types:
 *   NoAttributeType
 *   Scalar
 *   Vector
 *   Tensor
 *   Matrix
 *   Tensor6
 *   GlobalId
 */
class XDMF_EXPORT XdmfAttributeType : public XdmfItemProperty {

public:

  virtual ~XdmfAttributeType();

  friend class XdmfAttribute;

  // Supported Xdmf Attribute Types
  static shared_ptr<const XdmfAttributeType> NoAttributeType();
  static shared_ptr<const XdmfAttributeType> Scalar();
  static shared_ptr<const XdmfAttributeType> Vector();
  static shared_ptr<const XdmfAttributeType> Tensor();
  static shared_ptr<const XdmfAttributeType> Matrix();
  static shared_ptr<const XdmfAttributeType> Tensor6();
  static shared_ptr<const XdmfAttributeType> GlobalId();

  void
  getProperties(std::map<std::string, std::string> & collectedProperties) const;

protected:

  /**
   * Protected constructor for XdmfAttributeType.  The constructor is
   * protected because all attribute types supported by Xdmf should be
   * accessed through more specific static methods that construct
   * XdmfAttributeTypes - i.e. XdmfAttributeType::Scalar().
   *
   * @param name the name of the XdmfAttributeType to construct.
   */
  XdmfAttributeType(const std::string & name);

private:

  XdmfAttributeType(const XdmfAttributeType &); // Not implemented.
  void operator=(const XdmfAttributeType &); // Not implemented.

  static shared_ptr<const XdmfAttributeType>
  New(const std::map<std::string, std::string> & itemProperties);

  std::string mName;
};

#endif /* XDMFATTRIBUTETYPE_HPP_ */
