/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfMap.cpp                                                         */
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

#include "XdmfAttribute.hpp"
#include "XdmfError.hpp"
#include "XdmfGridCollection.hpp"
#include "XdmfGridCollectionType.hpp"
#include "XdmfHeavyDataController.hpp"
#include "XdmfMap.hpp"

boost::shared_ptr<XdmfMap>
XdmfMap::New()
{
  boost::shared_ptr<XdmfMap> p(new XdmfMap());
  return p;
}

std::vector<boost::shared_ptr<XdmfMap> >
XdmfMap::New(const std::vector<boost::shared_ptr<XdmfAttribute> > & globalNodeIds)
{
  // globalNodeId | taskId | localNodeId at taskId
  std::map<node_id, std::map<task_id, node_id> > globalNodeIdMap;

  // fill globalNodeIdMap using globalNodeIds
  for(unsigned int i=0; i<globalNodeIds.size(); ++i) {
    const boost::shared_ptr<XdmfAttribute> currGlobalNodeIds = 
      globalNodeIds[i];
    for(unsigned int j=0; j<currGlobalNodeIds->getSize(); ++j) {
      const node_id currGlobalNodeId = currGlobalNodeIds->getValue<node_id>(j);
      globalNodeIdMap[currGlobalNodeId][i] = j;
    }
  }

  std::vector<boost::shared_ptr<XdmfMap> > returnValue;
  returnValue.resize(globalNodeIds.size());

  // fill maps for each partition
  for(unsigned int i=0; i<globalNodeIds.size(); ++i)  {
    boost::shared_ptr<XdmfMap> map = XdmfMap::New();
    returnValue[i] = map;
    const boost::shared_ptr<XdmfAttribute> currGlobalNodeIds = 
      globalNodeIds[i];
    
    for(unsigned int j=0; j<currGlobalNodeIds->getSize(); ++j) {
      const node_id currGlobalNodeId = currGlobalNodeIds->getValue<node_id>(j);
      const std::map<task_id, node_id> & currMap = 
        globalNodeIdMap[currGlobalNodeId];
      if(currMap.size() > 1) {
        for(std::map<task_id, node_id>::const_iterator iter = currMap.begin(); 
            iter != currMap.end();
            ++iter) {
          if(iter->first != i) {
            map->insert(iter->first, j, iter->second);
          }
        }
      }
    }
  }
  
  return returnValue;
}

XdmfMap::XdmfMap()
{
}

XdmfMap::~XdmfMap()
{
}

const std::string XdmfMap::ItemTag = "Map";

std::map<std::string, std::string>
XdmfMap::getItemProperties() const
{
  std::map<std::string, std::string> mapProperties;
  return mapProperties;
}

std::string
XdmfMap::getItemTag() const
{
  return ItemTag;
}

std::map<XdmfMap::task_id, XdmfMap::node_id_map> 
XdmfMap::getMap() const
{
  return mMap;
}

XdmfMap::node_id_map
XdmfMap::getRemoteNodeIds(const task_id remoteTaskId)
{
  std::map<task_id, node_id_map>::const_iterator iter = 
    mMap.find(remoteTaskId);
  if(iter != mMap.end()) {
    return iter->second;
  }
  // No entry, return empty map.
  return node_id_map();
}

void
XdmfMap::insert(const task_id remoteTaskId,
                const node_id localNodeId,
                const node_id remoteLocalNodeId)
{
  mMap[remoteTaskId][localNodeId].insert(remoteLocalNodeId);
}

bool XdmfMap::isInitialized() const
{
  return mMap.size() > 0;
}

void
XdmfMap::populateItem(const std::map<std::string, std::string> & itemProperties,
                      std::vector<boost::shared_ptr<XdmfItem> > & childItems,
                      const XdmfCoreReader * const reader)
{
  XdmfItem::populateItem(itemProperties, childItems, reader);

  std::vector<boost::shared_ptr<XdmfArray> > arrayVector;
  arrayVector.reserve(3);
  for(std::vector<boost::shared_ptr<XdmfItem> >::const_iterator iter =
        childItems.begin();
      iter != childItems.end();
      ++iter) {
      if(boost::shared_ptr<XdmfArray> array =
         boost::shared_dynamic_cast<XdmfArray>(*iter)) {
        arrayVector.push_back(array);
      }
  }

  if(arrayVector.size() != 3)
    XdmfError::message(XdmfError::FATAL, 
                       "Expected 3 arrays attached to XdmfMap::populateItem");
  if(!(arrayVector[0]->getSize() == arrayVector[1]->getSize() &&
       arrayVector[0]->getSize() == arrayVector[2]->getSize())) {
    XdmfError::message(XdmfError::FATAL, 
                       "Arrays must be of equal size in "
                       "XdmfMap:: populateItem");
  }
  
  // check if any arrays have values in memory - if so, they need to be 
  // read into map
  bool needToRead = false;
  for(std::vector<boost::shared_ptr<XdmfArray> >::const_iterator iter = 
        arrayVector.begin();
      iter != arrayVector.end();
      ++iter) {
    if((*iter)->isInitialized()) {
      needToRead = true;
      break;
    }
  }
  
  if(needToRead) {
    for(std::vector<boost::shared_ptr<XdmfArray> >::const_iterator iter =
          arrayVector.begin();
        iter != arrayVector.end();
        ++iter) {
      if(!(*iter)->isInitialized()) {
        (*iter)->read();
      }
    }
    for(unsigned int i=0; i<arrayVector[0]->getSize(); ++i) {
      this->insert(arrayVector[0]->getValue<task_id>(i),
                   arrayVector[1]->getValue<node_id>(i),
                   arrayVector[2]->getValue<node_id>(i));
    }
  }
  else {
    mRemoteTaskIdsController = arrayVector[0]->getHeavyDataController();
    mLocalNodeIdsController = arrayVector[1]->getHeavyDataController();
    mRemoteLocalNodeIdsController = arrayVector[2]->getHeavyDataController();
  }
}

void
XdmfMap::read()
{
  if(mLocalNodeIdsController &&
     mRemoteTaskIdsController &&
     mRemoteLocalNodeIdsController) {
    
    if(!(mLocalNodeIdsController->getSize() == 
         mRemoteTaskIdsController->getSize() &&
         mLocalNodeIdsController->getSize() ==
         mRemoteLocalNodeIdsController->getSize())) {
      XdmfError::message(XdmfError::FATAL, 
                         "Arrays must be of equal size in XdmfMap::read");
    }

    boost::shared_ptr<XdmfArray> remoteTaskIds = XdmfArray::New();
    boost::shared_ptr<XdmfArray> localNodeIds = XdmfArray::New();
    boost::shared_ptr<XdmfArray> remoteLocalNodeIds = XdmfArray::New();

    mRemoteTaskIdsController->read(remoteTaskIds.get());
    mLocalNodeIdsController->read(localNodeIds.get());
    mRemoteLocalNodeIdsController->read(remoteLocalNodeIds.get());

    for(unsigned int i=0; i<remoteTaskIds->getSize(); ++i) {
      const unsigned int remoteTaskId = remoteTaskIds->getValue<task_id>(i);
      const unsigned int localNodeId = localNodeIds->getValue<node_id>(i);
      const unsigned int remoteLocalNodeId = 
        remoteLocalNodeIds->getValue<node_id>(i);
      mMap[remoteTaskId][localNodeId].insert(remoteLocalNodeId);
    }
  }
}

void
XdmfMap::release()
{
  mMap.clear();
}

void
XdmfMap::setHeavyDataControllers(boost::shared_ptr<XdmfHeavyDataController> remoteTaskIdsController,
                                 boost::shared_ptr<XdmfHeavyDataController> localNodeIdsController,
                                 boost::shared_ptr<XdmfHeavyDataController> remoteLocalNodeIdsController)
{
  if(!(localNodeIdsController->getSize() ==
       remoteTaskIdsController->getSize() &&
       localNodeIdsController->getSize() ==
       remoteLocalNodeIdsController->getSize()))
    XdmfError::message(XdmfError::FATAL, 
                       "Arrays must be of equal size in "
                       "XdmfMap::setHeavyDataControllers");
  mRemoteTaskIdsController = remoteTaskIdsController;
  mLocalNodeIdsController = localNodeIdsController;
  mRemoteLocalNodeIdsController = remoteLocalNodeIdsController;
}


void
XdmfMap::traverse(const boost::shared_ptr<XdmfBaseVisitor> visitor)
{
  XdmfItem::traverse(visitor);

  boost::shared_ptr<XdmfArray> remoteTaskIds = XdmfArray::New();
  boost::shared_ptr<XdmfArray> localNodeIds = XdmfArray::New();
  boost::shared_ptr<XdmfArray> remoteLocalNodeIds = XdmfArray::New();

  for(std::map<task_id, node_id_map>::const_iterator
        iter = mMap.begin();
      iter != mMap.end();
      ++iter) {
    for(node_id_map::const_iterator 
          iter2 = iter->second.begin();
        iter2 != iter->second.end();
        ++iter2) {
      for(node_id_map::mapped_type::const_iterator iter3 = 
            iter2->second.begin();
          iter3 != iter2->second.end();
          ++iter3) {
        remoteTaskIds->pushBack(iter->first);
        localNodeIds->pushBack(iter2->first);
        remoteLocalNodeIds->pushBack(*iter3);
      }
    }
  }

  remoteTaskIds->setHeavyDataController(mRemoteTaskIdsController);
  localNodeIds->setHeavyDataController(mLocalNodeIdsController);
  remoteLocalNodeIds->setHeavyDataController(mRemoteLocalNodeIdsController);

  remoteTaskIds->accept(visitor);  
  localNodeIds->accept(visitor);
  remoteLocalNodeIds->accept(visitor);

  mLocalNodeIdsController = localNodeIds->getHeavyDataController();
  mRemoteTaskIdsController = remoteTaskIds->getHeavyDataController();
  mRemoteLocalNodeIdsController = remoteLocalNodeIds->getHeavyDataController();
}
