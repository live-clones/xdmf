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

#include <utility>
#include "XdmfAttributeItemType.hpp"
#include "XdmfError.hpp"

//-----------------------------------------------------------------------------
std::map<std::string, shared_ptr<const XdmfAttributeItemType>(*)()>
  XdmfAttributeItemType::mAttributeDefinitions;
//-----------------------------------------------------------------------------
// Supported XdmfAttributeItemType
shared_ptr<const XdmfAttributeItemType>
XdmfAttributeItemType::NoAttributeItemType()
{
  static shared_ptr<const XdmfAttributeItemType>
    p(new XdmfAttributeItemType("None"));
  return p;
}
//-----------------------------------------------------------------------------
shared_ptr<const XdmfAttributeItemType>
XdmfAttributeItemType::FiniteElementFunction()
{
  static shared_ptr<const XdmfAttributeItemType>
    p(new XdmfAttributeItemType("FiniteElementFunction"));
  return p;
}
//-----------------------------------------------------------------------------
void XdmfAttributeItemType::InitTypes()
{
  mAttributeDefinitions["NONE"] = NoAttributeItemType;
  mAttributeDefinitions["FINITE_ELEMENT_FUNCTION"] = FiniteElementFunction;
}
//-----------------------------------------------------------------------------
XdmfAttributeItemType::XdmfAttributeItemType(const std::string & name) :
  mName(name)
{
}
//-----------------------------------------------------------------------------
XdmfAttributeItemType::~XdmfAttributeItemType()
{
}
//-----------------------------------------------------------------------------
shared_ptr<const XdmfAttributeItemType> XdmfAttributeItemType::New(
  const std::map<std::string, std::string> &itemProperties)
{
  InitTypes();

  std::map<std::string, std::string>::const_iterator item_type =
    itemProperties.find("ItemType");

  const std::string & typeVal = ConvertToUpper(type->second);

  std::map<std::string,
           shared_ptr<const XdmfAttributeItemType>(*)()>::const_iterator
    returnType = mAttributeDefinitions.find(typeVal);

  if (returnType == mAttributeDefinitions.end()) {
    XdmfError::message(XdmfError::FATAL,
      "ItemType not of 'None','FiniteElementFunction' in "
      "XdmfAttributeItemType::New");
  }
  else {
    return (*(returnType->second))();
  }

  // unreachable
  return shared_ptr<const XdmfAttributeItemType>();
}
//-----------------------------------------------------------------------------
void XdmfAttributeItemType::getProperties(
  std::map<std::string, std::string>& collectedProperties) const
{
  collectedProperties.insert(std::make_pair("ItemType", mName));
}
//-----------------------------------------------------------------------------
// C Wrappers
//-----------------------------------------------------------------------------
int XdmfAttributeItemTypeFiniteElementFunction()
{
  return XDMF_ATTRIBUTE_ITEM_TYPE_FINITE_ELEMENT_FUNCTION;
}