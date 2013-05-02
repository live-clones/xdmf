/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfArrayType.cpp                                                   */
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

#include <sstream>
#include <utility>
#include "XdmfArrayType.hpp"
#include "XdmfError.hpp"

// Supported XdmfArrayTypes
shared_ptr<const XdmfArrayType>
XdmfArrayType::Uninitialized()
{
  static shared_ptr<const XdmfArrayType> p(new XdmfArrayType("None", 0));
  return p;
}

shared_ptr<const XdmfArrayType>
XdmfArrayType::Int8()
{
  static shared_ptr<const XdmfArrayType> p(new XdmfArrayType("Char", 1));
  return p;
}

shared_ptr<const XdmfArrayType>
XdmfArrayType::Int16()
{
  static shared_ptr<const XdmfArrayType> p(new XdmfArrayType("Short", 2));
  return p;
}

shared_ptr<const XdmfArrayType>
XdmfArrayType::Int32()
{
  static shared_ptr<const XdmfArrayType> p(new XdmfArrayType("Int", 4));
  return p;
}

shared_ptr<const XdmfArrayType>
XdmfArrayType::Int64()
{
  static shared_ptr<const XdmfArrayType> p(new XdmfArrayType("Int", 8));
  return p;
}

shared_ptr<const XdmfArrayType>
XdmfArrayType::Float32()
{
  static shared_ptr<const XdmfArrayType> p(new XdmfArrayType("Float", 4));
  return p;
}

shared_ptr<const XdmfArrayType>
XdmfArrayType::Float64()
{
  static shared_ptr<const XdmfArrayType> p(new XdmfArrayType("Float", 8));
  return p;
}

shared_ptr<const XdmfArrayType>
XdmfArrayType::UInt8()
{
  static shared_ptr<const XdmfArrayType> p(new XdmfArrayType("UChar", 1));
  return p;
}

shared_ptr<const XdmfArrayType>
XdmfArrayType::UInt16()
{
  static shared_ptr<const XdmfArrayType> p(new XdmfArrayType("UShort", 2));
  return p;
}

shared_ptr<const XdmfArrayType>
XdmfArrayType::UInt32()
{
  static shared_ptr<const XdmfArrayType> p(new XdmfArrayType("UInt", 4));
  return p;
}

shared_ptr<const XdmfArrayType>
XdmfArrayType::String()
{
  static shared_ptr<const XdmfArrayType> p(new XdmfArrayType("String", 0));
  return p;
}

XdmfArrayType::XdmfArrayType(const std::string & name,
                             const unsigned int precision) :
  mName(name),
  mPrecision(precision)
{
}

XdmfArrayType::~XdmfArrayType()
{
}

shared_ptr<const XdmfArrayType>
XdmfArrayType::New(const std::map<std::string, std::string> & itemProperties)
{
  std::map<std::string, std::string>::const_iterator type =
    itemProperties.find("DataType");
  if(type == itemProperties.end()) {
    type = itemProperties.find("NumberType");
  }
  if(type == itemProperties.end()) {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Type unset because neither 'DataType' nor "
                         "'NumberType' found in itemProperties in "
                         "XdmfArrayType::New");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
  const std::string & typeVal = type->second;

  std::map<std::string, std::string>::const_iterator precision =
    itemProperties.find("Precision");
  const unsigned int precisionVal = 
    (precision == itemProperties.end()) ? 0 : atoi(precision->second.c_str()); 

  if(typeVal.compare("Float") == 0) {
    if(precisionVal == 8) {
      return Float64();
    }
    return Float32();
  }
  else if(typeVal.compare("Int") == 0) {
    if(precisionVal == 8) {
      return Int64();
    }
    return Int32();
  }
  else if(typeVal.compare("String") == 0) {
    return String();
  }
  else if(typeVal.compare("Char") == 0) {
    return Int8();
  }
  else if(typeVal.compare("Short") == 0) {
    return Int16();
  }
  else if(typeVal.compare("UChar") == 0) {
    return UInt8();
  }
  else if(typeVal.compare("UShort") == 0) {
    return UInt16();
  }
  else if(typeVal.compare("UInt") == 0) {
    return UInt32();
  }
  else if(typeVal.compare("None") == 0) {
    return Uninitialized();
  }

  try {
    XdmfError::message(XdmfError::FATAL,
                       "Type not one of accepted values: " + typeVal +
                       " in XdmfArrayType::New");
  }
  catch (XdmfError e) {
    throw e;
  }

  return shared_ptr<const XdmfArrayType>();
}

unsigned int
XdmfArrayType::getElementSize() const
{
  return mPrecision;
}

std::string
XdmfArrayType::getName() const
{
  return mName;
}

void
XdmfArrayType::getProperties(std::map<std::string, std::string> & collectedProperties) const
{
  collectedProperties.insert(std::make_pair("DataType", mName));
  std::stringstream precision;
  precision << mPrecision;
  collectedProperties.insert(std::make_pair("Precision", precision.str()));
}
