#include "XdmfAttribute.hpp"
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
  unsigned int maxGlobalNodeId = 0;
  for(unsigned int i=0; i<globalNodeIds.size(); ++i) {
    const boost::shared_ptr<XdmfAttribute> currGlobalNodeIds =
      globalNodeIds[i];
    for(unsigned int j=0; j<currGlobalNodeIds->getSize(); ++j) {
      unsigned int currGlobalNodeId =
        currGlobalNodeIds->getValue<unsigned int>(j);
      if(currGlobalNodeId > maxGlobalNodeId) {
        maxGlobalNodeId = currGlobalNodeId;
      }
    }
  }

  // globalNodeId | taskId | localNodeId at taskId
  std::vector<std::map<unsigned int, unsigned int> >
    globalNodeIdMap(maxGlobalNodeId + 1,
                    std::map<unsigned int, unsigned int>());

  // Fill globalNodeIdMap
  for(unsigned int i=0; i<globalNodeIds.size(); ++i) {
    const boost::shared_ptr<XdmfAttribute> currGlobalNodeIds =
      globalNodeIds[i];
    for(unsigned int j=0; j<currGlobalNodeIds->getSize(); ++j) {
      unsigned int currGlobalNodeId =
        currGlobalNodeIds->getValue<unsigned int>(j);
      globalNodeIdMap[currGlobalNodeId][i] = j;
    }
  }

  std::vector<boost::shared_ptr<XdmfMap> > toReturn;
  toReturn.reserve(globalNodeIds.size());
  // Fill maps for each partition
  for(unsigned int i=0; i<globalNodeIds.size(); ++i)  {
    boost::shared_ptr<XdmfMap> map = XdmfMap::New();
    toReturn.push_back(map);
    const boost::shared_ptr<XdmfAttribute> currGlobalNodeIds =
      globalNodeIds[i];
    for(unsigned int j=0; j<currGlobalNodeIds->getSize(); ++j) {
      unsigned int currGlobalNodeId =
        currGlobalNodeIds->getValue<unsigned int>(j);
      if(globalNodeIdMap[currGlobalNodeId].size() > 1) {
        for(std::map<unsigned int, unsigned int>::const_iterator iter =
              globalNodeIdMap[currGlobalNodeId].begin();
            iter != globalNodeIdMap[currGlobalNodeId].end();
            ++iter) {
          if(iter->first != i) {
            map->insert(j, iter->first, iter->second);
          }
        }
      }
    }
  }

  return toReturn;
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

std::map<unsigned int, unsigned int>
XdmfMap::getRemoteNodeIds(const unsigned int localNodeId)
{
  std::map<unsigned int, std::map<unsigned int, unsigned int> >::const_iterator
    iter = mMap.find(localNodeId);
  if(iter != mMap.end()) {
    return iter->second;
  }
  // No entry, return empty map.
  return std::map<unsigned int, unsigned int>();
}

void
XdmfMap::insert(const unsigned int localNodeId,
                const unsigned int remoteTaskId,
                const unsigned int remoteLocalNodeId)
{
  std::map<unsigned int, std::map<unsigned int, unsigned int> >::iterator
    iter = mMap.find(localNodeId);
  if(iter != mMap.end()) {
    iter->second[remoteTaskId] = remoteLocalNodeId;
  }
  else {
    std::map<unsigned int, unsigned int> newMap;
    newMap[remoteTaskId] = remoteLocalNodeId;
    mMap[localNodeId] = newMap;
  }
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
  assert(arrayVector.size() == 3);
  assert(arrayVector[0]->getSize() == arrayVector[1]->getSize() &&
         arrayVector[0]->getSize() == arrayVector[2]->getSize());
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
      (*iter)->read();
    }
    for(unsigned int i=0; i<arrayVector[0]->getSize(); ++i) {
      this->insert(arrayVector[0]->getValue<unsigned int>(i),
                   arrayVector[1]->getValue<unsigned int>(i),
                   arrayVector[2]->getValue<unsigned int>(i));
    }
  }
  else {
    mLocalNodeIdsController = arrayVector[0]->getHeavyDataController();
    mRemoteTaskIdsController = arrayVector[1]->getHeavyDataController();
    mRemoteLocalNodeIdsController = arrayVector[2]->getHeavyDataController();
  }
}

void
XdmfMap::read()
{
  if(mLocalNodeIdsController &&
     mRemoteTaskIdsController &&
     mRemoteLocalNodeIdsController) {
    assert(mLocalNodeIdsController->getSize() ==
           mRemoteTaskIdsController->getSize() &&
           mLocalNodeIdsController->getSize() ==
           mRemoteLocalNodeIdsController->getSize());
    boost::shared_ptr<XdmfArray> globalNodeIds = XdmfArray::New();
    boost::shared_ptr<XdmfArray> taskIds = XdmfArray::New();
    boost::shared_ptr<XdmfArray> localNodeIds = XdmfArray::New();
    mLocalNodeIdsController->read(globalNodeIds.get());
    mRemoteTaskIdsController->read(taskIds.get());
    mRemoteLocalNodeIdsController->read(localNodeIds.get());

    for(unsigned int i=0; i<globalNodeIds->getSize(); ++i) {
      std::map<unsigned int, std::map<unsigned int, unsigned int> >::iterator
        iter = mMap.find(globalNodeIds->getValue<unsigned int>(i));
      if(iter != mMap.end()) {
        iter->second[taskIds->getValue<unsigned int>(i)] =
          localNodeIds->getValue<unsigned int>(i);
      }
      else {
        std::map<unsigned int, unsigned int> newMap;
        newMap[taskIds->getValue<unsigned int>(i)] =
          localNodeIds->getValue<unsigned int>(i);
        mMap[globalNodeIds->getValue<unsigned int>(i)] = newMap;
      }
    }
  }
}

void
XdmfMap::release()
{
  mMap.clear();
}

void
XdmfMap::setHeavyDataControllers(boost::shared_ptr<XdmfHeavyDataController> localNodeIdsController,
                                 boost::shared_ptr<XdmfHeavyDataController> remoteTaskIdsController,
                                 boost::shared_ptr<XdmfHeavyDataController> remoteLocalNodeIdsController)
{
  assert(localNodeIdsController->getSize() ==
         remoteTaskIdsController->getSize() &&
         localNodeIdsController->getSize() ==
         remoteLocalNodeIdsController->getSize());
  mLocalNodeIdsController = localNodeIdsController;
  mRemoteTaskIdsController = remoteTaskIdsController;
  mRemoteLocalNodeIdsController = remoteLocalNodeIdsController;
}


void
XdmfMap::traverse(const boost::shared_ptr<XdmfBaseVisitor> visitor)
{
  XdmfItem::traverse(visitor);
  boost::shared_ptr<XdmfArray> localNodeIds = XdmfArray::New();
  boost::shared_ptr<XdmfArray> remoteTaskIds = XdmfArray::New();
  boost::shared_ptr<XdmfArray> remoteLocalNodeIds = XdmfArray::New();
  for(std::map<unsigned int, std::map<unsigned int, unsigned int> >::const_iterator
        iter = mMap.begin();
      iter != mMap.end();
      ++iter) {
    for(std::map<unsigned int, unsigned int>::const_iterator iter2 =
          iter->second.begin();
        iter2 != iter->second.end();
        ++iter2) {
      localNodeIds->pushBack(iter->first);
      remoteTaskIds->pushBack(iter2->first);
      remoteLocalNodeIds->pushBack(iter2->second);
    }
  }
  localNodeIds->setHeavyDataController(mLocalNodeIdsController);
  remoteTaskIds->setHeavyDataController(mRemoteTaskIdsController);
  remoteLocalNodeIds->setHeavyDataController(mRemoteLocalNodeIdsController);
  localNodeIds->accept(visitor);
  remoteTaskIds->accept(visitor);
  remoteLocalNodeIds->accept(visitor);
  mLocalNodeIdsController = localNodeIds->getHeavyDataController();
  mRemoteTaskIdsController = remoteTaskIds->getHeavyDataController();
  mRemoteLocalNodeIdsController = remoteLocalNodeIds->getHeavyDataController();
}
