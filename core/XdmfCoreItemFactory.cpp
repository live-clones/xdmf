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

XdmfCoreItemFactory::XdmfCoreItemFactory()
{
}

XdmfCoreItemFactory::~XdmfCoreItemFactory()
{
}

shared_ptr<XdmfItem>
XdmfCoreItemFactory::createItem(const std::string & itemTag,
                                const std::map<std::string, std::string> &,
                                const std::vector<shared_ptr<XdmfItem> > &) const
{
  if(itemTag.compare(XdmfArray::ItemTag) == 0) {
    return XdmfArray::New();
  }
  return shared_ptr<XdmfItem>();
}
