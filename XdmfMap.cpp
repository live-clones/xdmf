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
#include "XdmfGridCollection.hpp"
#include "XdmfGridCollectionType.hpp"
#include "XdmfHeavyDataController.hpp"
#include "XdmfMap.hpp"

/**
 * PIMPL
 */
class XdmfMap::XdmfMapImpl {

public:

	XdmfMapImpl() :
		mLocalNodeIdsHeavyDataController(boost::shared_ptr<XdmfHeavyDataController>()),
		mRemoteLocalNodeIdsHeavyDataController(boost::shared_ptr<XdmfHeavyDataController>()),
		mRemoteTaskIdsHeavyDataController(boost::shared_ptr<XdmfHeavyDataController>())
	{
	};

	~XdmfMapImpl()
	{
	};

	boost::shared_ptr<XdmfHeavyDataController> mLocalNodeIdsHeavyDataController;
	std::map<unsigned int, std::map<unsigned int, unsigned int> > mMap;	// localNodeId | remoteTaskId | remoteLocalNodeId
	boost::shared_ptr<XdmfHeavyDataController> mRemoteLocalNodeIdsHeavyDataController;
	boost::shared_ptr<XdmfHeavyDataController> mRemoteTaskIdsHeavyDataController;

};

boost::shared_ptr<XdmfMap> XdmfMap::New()
{
	boost::shared_ptr<XdmfMap> p(new XdmfMap());
	return p;
}

std::vector<boost::shared_ptr<XdmfMap> > XdmfMap::New(const std::vector<boost::shared_ptr<XdmfAttribute> > & globalNodeIds)
{
	unsigned int maxGlobalNodeId = 0;
	for(unsigned int i=0; i<globalNodeIds.size(); ++i)
	{
		const boost::shared_ptr<XdmfAttribute> currGlobalNodeIds = globalNodeIds[i];
		for(unsigned int j=0; j<currGlobalNodeIds->getSize(); ++j)
		{
			unsigned int currGlobalNodeId = currGlobalNodeIds->getValue<unsigned int>(j);
			if(currGlobalNodeId > maxGlobalNodeId)
			{
				maxGlobalNodeId = currGlobalNodeId;
			}
		}
	}

	// globalNodeId | taskId | localNodeId at taskId
	std::vector<std::map<unsigned int, unsigned int> > globalNodeIdMap(maxGlobalNodeId + 1, std::map<unsigned int, unsigned int>());

	// Fill globalNodeIdMap
	for(unsigned int i=0; i<globalNodeIds.size(); ++i)
	{
		const boost::shared_ptr<XdmfAttribute> currGlobalNodeIds = globalNodeIds[i];
		for(unsigned int j=0; j<currGlobalNodeIds->getSize(); ++j)
		{
			unsigned int currGlobalNodeId = currGlobalNodeIds->getValue<unsigned int>(j);
			globalNodeIdMap[currGlobalNodeId][i] = j;
		}
	}

	std::vector<boost::shared_ptr<XdmfMap> > toReturn;
	toReturn.reserve(globalNodeIds.size());
	// Fill maps for each partition
	for(unsigned int i=0; i<globalNodeIds.size(); ++i)
	{
		boost::shared_ptr<XdmfMap> map = XdmfMap::New();
		toReturn.push_back(map);
		const boost::shared_ptr<XdmfAttribute> currGlobalNodeIds = globalNodeIds[i];
		for(unsigned int j=0; j<currGlobalNodeIds->getSize(); ++j)
		{
			unsigned int currGlobalNodeId = currGlobalNodeIds->getValue<unsigned int>(j);
			if(globalNodeIdMap[currGlobalNodeId].size() > 1)
			{
				for(std::map<unsigned int, unsigned int>::const_iterator iter = globalNodeIdMap[currGlobalNodeId].begin(); iter != globalNodeIdMap[currGlobalNodeId].end(); ++iter)
				{
					if(iter->first != i)
					{
						map->insert(j, iter->first, iter->second);
					}
				}
			}
		}
	}

	return toReturn;
}

XdmfMap::XdmfMap() :
	mImpl(new XdmfMapImpl())
{
}

XdmfMap::~XdmfMap()
{
	delete mImpl;
}

const std::string XdmfMap::ItemTag = "Map";

std::map<std::string, std::string> XdmfMap::getItemProperties() const
{
	std::map<std::string, std::string> mapProperties;
	return mapProperties;
}

std::string XdmfMap::getItemTag() const
{
	return ItemTag;
}

std::map<unsigned int, unsigned int> XdmfMap::getRemoteNodeIds(const unsigned int localNodeId)
{
	std::map<unsigned int, std::map<unsigned int, unsigned int> >::const_iterator iter = mImpl->mMap.find(localNodeId);
	if(iter != mImpl->mMap.end())
	{
		return iter->second;
	}
	// No entry, return empty map.
	return std::map<unsigned int, unsigned int>();
}

void XdmfMap::insert(const unsigned int localNodeId, const unsigned int remoteTaskId, const unsigned int remoteLocalNodeId)
{
	std::map<unsigned int, std::map<unsigned int, unsigned int> >::iterator iter = mImpl->mMap.find(localNodeId);
	if(iter != mImpl->mMap.end())
	{
		iter->second[remoteTaskId] = remoteLocalNodeId;
	}
	else
	{
		std::map<unsigned int, unsigned int> newMap;
		newMap[remoteTaskId] = remoteLocalNodeId;
		mImpl->mMap[localNodeId] = newMap;
	}
}

bool XdmfMap::isInitialized() const
{
	return mImpl->mMap.size() > 0;
}

void XdmfMap::populateItem(const std::map<std::string, std::string> & itemProperties, std::vector<boost::shared_ptr<XdmfItem> > & childItems, const XdmfCoreReader * const reader)
{
	XdmfItem::populateItem(itemProperties, childItems, reader);
	std::vector<boost::shared_ptr<XdmfArray> > arrayVector;
	arrayVector.reserve(3);
	for(std::vector<boost::shared_ptr<XdmfItem> >::const_iterator iter = childItems.begin(); iter != childItems.end(); ++iter)
	{
		if(boost::shared_ptr<XdmfArray> array = boost::shared_dynamic_cast<XdmfArray>(*iter))
		{
			arrayVector.push_back(array);
		}
	}
	assert(arrayVector.size() == 3);
	assert(arrayVector[0]->getSize() == arrayVector[1]->getSize() &&  arrayVector[0]->getSize() == arrayVector[2]->getSize());
	bool needToRead = false;
	for(std::vector<boost::shared_ptr<XdmfArray> >::const_iterator iter = arrayVector.begin(); iter != arrayVector.end(); ++iter)
	{
		if((*iter)->isInitialized())
		{
			needToRead = true;
			break;
		}
	}
	if(needToRead)
	{
		for(std::vector<boost::shared_ptr<XdmfArray> >::const_iterator iter = arrayVector.begin(); iter != arrayVector.end(); ++iter)
		{
			(*iter)->read();
		}
		for(unsigned int i=0; i<arrayVector[0]->getSize(); ++i)
		{
			this->insert(arrayVector[0]->getValue<unsigned int>(i), arrayVector[1]->getValue<unsigned int>(i), arrayVector[2]->getValue<unsigned int>(i));
		}
	}
	else
	{
		mImpl->mLocalNodeIdsHeavyDataController = arrayVector[0]->getHeavyDataController();
		mImpl->mRemoteTaskIdsHeavyDataController = arrayVector[1]->getHeavyDataController();
		mImpl->mRemoteLocalNodeIdsHeavyDataController = arrayVector[2]->getHeavyDataController();
	}
}

void XdmfMap::read()
{
	if(mImpl->mLocalNodeIdsHeavyDataController && mImpl->mRemoteTaskIdsHeavyDataController && mImpl->mRemoteLocalNodeIdsHeavyDataController)
	{
		assert(mImpl->mLocalNodeIdsHeavyDataController->getSize() == mImpl->mRemoteTaskIdsHeavyDataController->getSize() && mImpl->mLocalNodeIdsHeavyDataController->getSize() == mImpl->mRemoteLocalNodeIdsHeavyDataController->getSize());
		boost::shared_ptr<XdmfArray> globalNodeIds = XdmfArray::New();
		boost::shared_ptr<XdmfArray> taskIds = XdmfArray::New();
		boost::shared_ptr<XdmfArray> localNodeIds = XdmfArray::New();
		mImpl->mLocalNodeIdsHeavyDataController->read(globalNodeIds.get());
		mImpl->mRemoteTaskIdsHeavyDataController->read(taskIds.get());
		mImpl->mRemoteLocalNodeIdsHeavyDataController->read(localNodeIds.get());

		for(unsigned int i=0; i<globalNodeIds->getSize(); ++i)
		{
			std::map<unsigned int, std::map<unsigned int, unsigned int> >::iterator iter = mImpl->mMap.find(globalNodeIds->getValue<unsigned int>(i));
			if(iter != mImpl->mMap.end())
			{
				iter->second[taskIds->getValue<unsigned int>(i)] = localNodeIds->getValue<unsigned int>(i);
			}
			else
			{
				std::map<unsigned int, unsigned int> newMap;
				newMap[taskIds->getValue<unsigned int>(i)] = localNodeIds->getValue<unsigned int>(i);
				mImpl->mMap[globalNodeIds->getValue<unsigned int>(i)] = newMap;
			}
		}
	}
}

void XdmfMap::release()
{
	mImpl->mMap.clear();
}

void XdmfMap::setHeavyDataControllers(boost::shared_ptr<XdmfHeavyDataController> localNodeIdsHeavyDataController, boost::shared_ptr<XdmfHeavyDataController> remoteTaskIdsHeavyDataController, boost::shared_ptr<XdmfHeavyDataController> remoteLocalNodeIdsHeavyDataController)
{
	assert(localNodeIdsHeavyDataController->getSize() == remoteTaskIdsHeavyDataController->getSize() && localNodeIdsHeavyDataController->getSize() == remoteLocalNodeIdsHeavyDataController->getSize());
	mImpl->mLocalNodeIdsHeavyDataController = localNodeIdsHeavyDataController;
	mImpl->mRemoteTaskIdsHeavyDataController = remoteTaskIdsHeavyDataController;
	mImpl->mRemoteLocalNodeIdsHeavyDataController = remoteLocalNodeIdsHeavyDataController;
}


void XdmfMap::traverse(const boost::shared_ptr<XdmfBaseVisitor> visitor)
{
	XdmfItem::traverse(visitor);
	boost::shared_ptr<XdmfArray> localNodeIds = XdmfArray::New();
	boost::shared_ptr<XdmfArray> remoteTaskIds = XdmfArray::New();
	boost::shared_ptr<XdmfArray> remoteLocalNodeIds = XdmfArray::New();
	for(std::map<unsigned int, std::map<unsigned int, unsigned int> >::const_iterator iter = mImpl->mMap.begin(); iter != mImpl->mMap.end(); ++iter)
	{
		for(std::map<unsigned int, unsigned int>::const_iterator iter2 = iter->second.begin(); iter2 != iter->second.end(); ++iter2)
		{
			localNodeIds->pushBack(iter->first);
			remoteTaskIds->pushBack(iter2->first);
			remoteLocalNodeIds->pushBack(iter2->second);
		}
	}
	localNodeIds->setHeavyDataController(mImpl->mLocalNodeIdsHeavyDataController);
	remoteTaskIds->setHeavyDataController(mImpl->mRemoteTaskIdsHeavyDataController);
	remoteLocalNodeIds->setHeavyDataController(mImpl->mRemoteLocalNodeIdsHeavyDataController);
	localNodeIds->accept(visitor);
	remoteTaskIds->accept(visitor);
	remoteLocalNodeIds->accept(visitor);
	mImpl->mLocalNodeIdsHeavyDataController = localNodeIds->getHeavyDataController();
	mImpl->mRemoteTaskIdsHeavyDataController = remoteTaskIds->getHeavyDataController();
	mImpl->mRemoteLocalNodeIdsHeavyDataController = remoteLocalNodeIds->getHeavyDataController();
}
