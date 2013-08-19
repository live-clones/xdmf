/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfCoreItemFactory.cpp                                             */
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

#include "XdmfArray.hpp"
#include "XdmfCoreItemFactory.hpp"
#include "XdmfError.hpp"

XdmfCoreItemFactory::XdmfCoreItemFactory()
{
}

XdmfCoreItemFactory::~XdmfCoreItemFactory()
{
}

shared_ptr<XdmfItem>
XdmfCoreItemFactory::createItem(const std::string & itemTag,
                                const std::map<std::string, std::string> & itemProperties,
                                const std::vector<shared_ptr<XdmfItem> > & childItems) const
{
  if(itemTag.compare(XdmfArray::ItemTag) == 0) {
    return XdmfArray::New();
  }
  else if (itemTag.compare(XdmfFunction::ItemTag) == 0) {
    std::map<std::string, std::string>::const_iterator type =
      itemProperties.find("ConstructedType");
    std::string arraySubType;
    if(type == itemProperties.end()) {
      // If no type is specified an array is generated
      arraySubType = XdmfArray::ItemTag;
    }
    else {
      arraySubType = type->second;
    }
    std::map<std::string, std::string>::const_iterator expression =
      itemProperties.find("Expression");
    std::string expressionToParse;
    if(expression == itemProperties.end()) {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Function found no expression");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
    else {
      expressionToParse = expression->second;
    }
    std::map<std::string, shared_ptr<XdmfArray> > variableCollection;
    for (unsigned int i = 0; i < childItems.size(); ++i) {
      try {
        shared_ptr<XdmfArray> tempArray =
          shared_dynamic_cast<XdmfArray>(childItems[i]);
        variableCollection[tempArray->getName()] = tempArray;
        tempArray->read();
      }
      catch (...) {
        try {
          XdmfError::message(XdmfError::FATAL,
                             "Error: Function passed non-Array item");
        }
        catch (XdmfError e) {
          throw e;
        }
      }
    }
    shared_ptr<XdmfArray> parsedArray = shared_ptr<XdmfArray>();
    try {
      parsedArray = XdmfFunction::evaluateExpression(expressionToParse,
                                                     variableCollection);
    }
    catch (XdmfError e) {
      throw e;
    }
    if (arraySubType != XdmfArray::ItemTag) {
      // The properties and children aren't really needed
      // to generate the object, but the factory still requires them.
      std::vector<shared_ptr<XdmfItem> > newArrayChildren;
      shared_ptr<XdmfArray> returnArray = XdmfArray::New();

      // This should generate an item that corresponds to the tag provided
      // the casting ensures that it is a subtype of array
      // Using a factory to be able to build things outside of core
      returnArray = shared_dynamic_cast<XdmfArray>(createItem(
                                                     arraySubType,
                                                     itemProperties,
                                                     newArrayChildren));

      returnArray->insert(0, parsedArray, 0, parsedArray->getSize());
      returnArray->setFunction(XdmfFunction::New(expressionToParse,
                                                 variableCollection));
      return returnArray;
    }
    else {
      parsedArray->setFunction(XdmfFunction::New(expressionToParse,
                                                 variableCollection));
      return parsedArray;
    }
  }
  return shared_ptr<XdmfItem>();
}
