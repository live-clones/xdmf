/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfArrayType.hpp                                                   */
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

#ifndef XDMFARRAYTYPE_HPP_
#define XDMFARRAYTYPE_HPP_

// Includes
#include "XdmfCore.hpp"
#include "XdmfItemProperty.hpp"

/**
 * @brief Property describing what types of values an XdmfArray
 * contains.
 *
 * XdmfArrayType specifies the types of values stored in an XdmfArray.
 * A specific XdmfArrayType can be created by calling one of the
 * static methods in the class, i.e. XdmfArrayType::Int32().
 *
 * Example of use:
 * //Assuming that exampleArray is a shared pointer to an XdmfArray object that has been filled with data
 * if (XdmfArrayType::Int8() == exampleArray->getArrayType())
 * {
 *   //do whatever is to be done with in the case that the array type is Int8
 * }
 *
 * Xdmf supports the following attribute types:
 *   Uninitialized
 *   Int8
 *   Int16
 *   Int32
 *   Int64
 *   Float32
 *   Float64
 *   UInt8
 *   UInt16
 *   UInt32
 *   String
 */
class XDMFCORE_EXPORT XdmfArrayType : public XdmfItemProperty {

public:

  virtual ~XdmfArrayType();

  friend class XdmfArray;

  // Supported XdmfArrayTypes
  static shared_ptr<const XdmfArrayType> Uninitialized();
  static shared_ptr<const XdmfArrayType> Int8();
  static shared_ptr<const XdmfArrayType> Int16();
  static shared_ptr<const XdmfArrayType> Int32();
  static shared_ptr<const XdmfArrayType> Int64();
  static shared_ptr<const XdmfArrayType> Float32();
  static shared_ptr<const XdmfArrayType> Float64();
  static shared_ptr<const XdmfArrayType> UInt8();
  static shared_ptr<const XdmfArrayType> UInt16();
  static shared_ptr<const XdmfArrayType> UInt32();
  static shared_ptr<const XdmfArrayType> String();

  /**
   * Get the data size, in bytes, of the value associated with this
   * array type.
   *
   * Example of use:
   *
   * C++
   *
   * @code {.cpp}
   * unsigned int dataSize = XdmfArrayType::Int8()->getElementSize();
   * //The number of bytes in an Int8 will be stored in the dataSize variable
   * @endcode
   *
   * Python
   *
   * @code {.py}
   * dataSize = XdmfArrayType.Int8().getElementSize()
   * '''
   * The number of bytes in an Int8 will be stored in the dataSize variable
   * '''
   * @endcode
   *
   * @return the data size, in bytes.
   */
  unsigned int getElementSize() const;

  /**
   * Get the name of the data type.
   *
   * Example of use:
   *
   * C++
   *
   * @code {.cpp}
   * std::string dataName = XdmfArrayType::Int8()->getName();
   * //The name of the Int8 data type will be stored in the dataName variable
   * @endcode
   *
   * Python
   *
   * @code {.py}
   * dataName = XdmfArrayType.Int8().getName()
   * '''
   * The name of the Int8 data type will be stored in the dataName variable
   * '''
   * @endcode
   *
   * @return the name of the data type.
   */
  std::string getName() const;

  void
  getProperties(std::map<std::string, std::string> & collectedProperties) const;

protected:

  /**
   * Protected constructor for XdmfArrayType. The constructor is
   * protected because all array types supported by Xdmf should be
   * accessed through more specific static methods that construct
   * XdmfArrayTypes - i.e. XdmfArrayType::Float64().
   *
   * @param name the name of the XdmfArrayType to construct.
   * @param precision the precision, in bytes, of the XdmfArrayType to
   * construct.
   */
  XdmfArrayType(const std::string & name,
                const unsigned int precision);

private:

  XdmfArrayType(const XdmfArrayType &); // Not implemented.
  void operator=(const XdmfArrayType &); // Not implemented.

  static shared_ptr<const XdmfArrayType>
  New(const std::map<std::string, std::string> & itemProperties);

  std::string mName;
  unsigned int mPrecision;
};

#endif /* XDMFARRAYTYPE_HPP_ */
