/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfInformation.hpp                                                 */
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

#ifndef XDMFINFORMATION_HPP_
#define XDMFINFORMATION_HPP_

// Forward declarations
class XdmfArray;

// Includes
#include "XdmfCore.hpp"
#include "XdmfItem.hpp"

/**
 * @brief Holds a key/value pair that can be attached to an Xdmf
 * structure.
 *
 * XdmfInformation stores two strings as a key value pair. These can
 * be used to store input parameters to a code or for simple result
 * data like wall time.
 */
class XDMFCORE_EXPORT XdmfInformation : public XdmfItem {

public:

  /**
   * Create a new XdmfInformation.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfInformation.cpp
   * @skipline New
   * @until setValue
   *
   * Python
   *
   * @dontinclude XdmfExampleInformation.py
   * @skipline New
   * @until setValue
   *
   * @return constructed XdmfInformation.
   */
  static shared_ptr<XdmfInformation> New();

  /**
   * Create a new XdmfInformation.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfInformation.cpp
   * @skipline infoExample
   * @until //
   *
   * Python
   *
   * @dontinclude XdmfExampleInformation.py
   * @skipline infoExample
   * @until #
   *
   * @param key a string containing the key of the XdmfInformation to create.
   * @param value a string containing the value of the XdmfInformation to
   * create.
   *
   * @return constructed XdmfInformation
   */
  static shared_ptr<XdmfInformation> New(const std::string & key,
                                         const std::string & value);

  virtual ~XdmfInformation();

  LOKI_DEFINE_VISITABLE(XdmfInformation, XdmfItem);
  XDMF_CHILDREN(XdmfInformation, XdmfArray, Array, Name);
  static const std::string ItemTag;

  std::map<std::string, std::string> getItemProperties() const;

  virtual std::string getItemTag() const;

  /**
   * Get the key for this information item.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfInformation.cpp
   * @skipline infoExample
   * @until //
   * @skipline getKey
   * @until //
   *
   * Python
   *
   * @dontinclude XdmfExampleInformation.py
   * @skipline infoExample
   * @until #
   * @skipline getKey
   * @until #
   *
   * @return string containing the key.
   */
  std::string getKey() const;

  /**
   * Get the value for this information item.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfInformation.cpp
   * @skipline infoExample
   * @until //
   * @skipline getValue
   * @until //
   *
   * Python
   *
   * @dontinclude XdmfExampleInformation.py
   * @skipline infoExample
   * @until #
   * @skipline getValue
   * @until #
   *
   * @return string containing the value.
   */
  std::string getValue() const;

  using XdmfItem::insert;

  /**
   * Set the key for this information item.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfInformation.cpp
   * @skipline infoExample
   * @until //
   * @skipline setKey
   * @until //
   *
   * Python
   *
   * @dontinclude XdmfExampleInformation.py
   * @skipline infoExample
   * @until #
   * @skipline setKey
   * @until #
   *
   * @param key a string containing the key to set.
   */
  void setKey(const std::string & key);

  /**
   * Set the value for this information item.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfInformation.cpp
   * @skipline infoExample
   * @until //
   * @skipline setValue
   * @until //
   *
   * Python
   *
   * @dontinclude XdmfExampleInformation.py
   * @skipline infoExample
   * @until #
   * @skipline setValue
   * @until #
   *
   * @param value a string containing the value to set.
   */
  void setValue(const std::string & value);

  virtual void traverse(const shared_ptr<XdmfBaseVisitor> visitor);

protected:

  XdmfInformation(const std::string & key = "",
                  const std::string & value = "");

  virtual void
  populateItem(const std::map<std::string, std::string> & itemProperties,
               const std::vector<shared_ptr<XdmfItem> > & childItems,
               const XdmfCoreReader * const reader);

private:

  XdmfInformation(const XdmfInformation &);  // Not implemented.
  void operator=(const XdmfInformation &);  // Not implemented.

  std::string mKey;
  std::string mValue;
};

#ifdef _WIN32
XDMFCORE_TEMPLATE template class XDMFCORE_EXPORT
shared_ptr<Loki::BaseVisitor>;
XDMFCORE_TEMPLATE template class XDMFCORE_EXPORT
Loki::Visitor<shared_ptr<XdmfInformation>,
              shared_ptr<XdmfItem> >;
#endif

#endif /* XDMFINFORMATION_HPP_ */
