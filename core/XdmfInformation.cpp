/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfInformation.cpp                                                 */
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

#include "XdmfInformation.hpp"

boost::shared_ptr<XdmfInformation>
XdmfInformation::New()
{
  boost::shared_ptr<XdmfInformation> p(new XdmfInformation());
  return p;
};

boost::shared_ptr<XdmfInformation>
XdmfInformation::New(const std::string & key,
                     const std::string & value)
{
  boost::shared_ptr<XdmfInformation> p(new XdmfInformation(key, value));
  return p;
};

XdmfInformation::XdmfInformation(const std::string & key,
                                 const std::string & value) :
  mKey(key),
  mValue(value)
{
}

XdmfInformation::~XdmfInformation()
{
}

const std::string XdmfInformation::ItemTag = "Information";

std::map<std::string, std::string>
XdmfInformation::getItemProperties() const
{
  std::map<std::string, std::string> informationProperties;
  informationProperties["Name"] = mKey;
  informationProperties["Value"] = mValue;
  return informationProperties;
}

std::string
XdmfInformation::getItemTag() const
{
  return ItemTag;
}

std::string
XdmfInformation::getKey() const
{
  return mKey;
}

std::string
XdmfInformation::getValue() const
{
  return mValue;
}

void
XdmfInformation::populateItem(const std::map<std::string, std::string> & itemProperties,
                              std::vector<boost::shared_ptr<XdmfItem> > & childItems,
                              const XdmfCoreReader * const reader)
{
  XdmfItem::populateItem(itemProperties, childItems, reader);

  std::map<std::string, std::string>::const_iterator key =
    itemProperties.find("Name");
  if(key != itemProperties.end()) {
    mKey = key->second;
  }
  else {
    assert(false);
  }

  std::map<std::string, std::string>::const_iterator value =
    itemProperties.find("Value");
  if(value != itemProperties.end()) {
    mValue = value->second;
  }
  else {
    assert(false);
  }
}

void
XdmfInformation::setKey(const std::string & key)
{
  mKey= key;
}

void
XdmfInformation::setValue(const std::string & value)
{
  mValue= value;
}
