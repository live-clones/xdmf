#include "XdmfAttribute.hpp"
#include "XdmfGridCollection.hpp"
#include "XdmfGridCollectionType.hpp"
#include "XdmfHDF5Controller.hpp"
#include "XdmfMap.hpp"

/**
 * PIMPL
 */
class XdmfMap::XdmfMapImpl {

public:

	XdmfMapImpl() :
		mLocalNodeIdsHDF5Controller(boost::shared_ptr<XdmfHDF5Controller>()),
		mRemoteLocalNodeIdsHDF5Controller(boost::shared_ptr<XdmfHDF5Controller>()),
		mRemoteTaskIdsHDF5Controller(boost::shared_ptr<XdmfHDF5Controller>())
	{
	};

	~XdmfMapImpl()
	{
	};

	boost::shared_ptr<XdmfHDF5Controller> mLocalNodeIdsHDF5Controller;
	std::map<unsigned int, std::map<unsigned int, unsigned int> > mMap;	// localNodeId | remoteTaskId | remoteLocalNodeId
	boost::shared_ptr<XdmfHDF5Controller> mRemoteLocalNodeIdsHDF5Controller;
	boost::shared_ptr<XdmfHDF5Controller> mRemoteTaskIdsHDF5Controller;

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
		for(unsigned int j=0; j<currGlobalNodeIds->size(); ++j)
		{
			unsigned int currGlobalNodeId = currGlobalNodeIds->getValueCopy<unsigned int>(j);
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
		for(unsigned int j=0; j<currGlobalNodeIds->size(); ++j)
		{
			unsigned int currGlobalNodeId = currGlobalNodeIds->getValueCopy<unsigned int>(j);
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
		for(unsigned int j=0; j<currGlobalNodeIds->size(); ++j)
		{
			unsigned int currGlobalNodeId = currGlobalNodeIds->getValueCopy<unsigned int>(j);
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

void XdmfMap::populateItem(const std::map<std::string, std::string> & itemProperties, std::vector<boost::shared_ptr<XdmfItem> > & childItems)
{
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
	assert(arrayVector[0]->size() == arrayVector[1]->size() &&  arrayVector[0]->size() == arrayVector[2]->size());
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
		for(unsigned int i=0; i<arrayVector[0]->size(); ++i)
		{
			this->insert(arrayVector[0]->getValueCopy<unsigned int>(i), arrayVector[1]->getValueCopy<unsigned int>(i), arrayVector[2]->getValueCopy<unsigned int>(i));
		}
	}
	else
	{
		mImpl->mLocalNodeIdsHDF5Controller = arrayVector[0]->getHDF5Controller();
		mImpl->mRemoteTaskIdsHDF5Controller = arrayVector[1]->getHDF5Controller();
		mImpl->mRemoteLocalNodeIdsHDF5Controller = arrayVector[2]->getHDF5Controller();
	}
}

void XdmfMap::read()
{
	if(mImpl->mLocalNodeIdsHDF5Controller != NULL && mImpl->mRemoteTaskIdsHDF5Controller != NULL && mImpl->mRemoteLocalNodeIdsHDF5Controller != NULL)
	{
		assert(mImpl->mLocalNodeIdsHDF5Controller->size() == mImpl->mRemoteTaskIdsHDF5Controller->size() && mImpl->mLocalNodeIdsHDF5Controller->size() == mImpl->mRemoteLocalNodeIdsHDF5Controller->size());
		boost::shared_ptr<XdmfArray> globalNodeIds = XdmfArray::New();
		boost::shared_ptr<XdmfArray> taskIds = XdmfArray::New();
		boost::shared_ptr<XdmfArray> localNodeIds = XdmfArray::New();
		mImpl->mLocalNodeIdsHDF5Controller->read(globalNodeIds.get());
		mImpl->mRemoteTaskIdsHDF5Controller->read(taskIds.get());
		mImpl->mRemoteLocalNodeIdsHDF5Controller->read(localNodeIds.get());

		for(unsigned int i=0; i<globalNodeIds->size(); ++i)
		{
			std::map<unsigned int, std::map<unsigned int, unsigned int> >::iterator iter = mImpl->mMap.find(globalNodeIds->getValueCopy<unsigned int>(i));
			if(iter != mImpl->mMap.end())
			{
				iter->second[taskIds->getValueCopy<unsigned int>(i)] = localNodeIds->getValueCopy<unsigned int>(i);
			}
			else
			{
				std::map<unsigned int, unsigned int> newMap;
				newMap[taskIds->getValueCopy<unsigned int>(i)] = localNodeIds->getValueCopy<unsigned int>(i);
				mImpl->mMap[globalNodeIds->getValueCopy<unsigned int>(i)] = newMap;
			}
		}
	}
}

void XdmfMap::release()
{
	mImpl->mMap.clear();
}

void XdmfMap::setHDF5Controllers(boost::shared_ptr<XdmfHDF5Controller> localNodeIdsHDF5Controller, boost::shared_ptr<XdmfHDF5Controller> remoteTaskIdsHDF5Controller, boost::shared_ptr<XdmfHDF5Controller> remoteLocalNodeIdsHDF5Controller)
{
	assert(localNodeIdsHDF5Controller->size() == remoteTaskIdsHDF5Controller->size() && localNodeIdsHDF5Controller->size() == remoteLocalNodeIdsHDF5Controller->size());
	mImpl->mLocalNodeIdsHDF5Controller = localNodeIdsHDF5Controller;
	mImpl->mRemoteTaskIdsHDF5Controller = remoteTaskIdsHDF5Controller;
	mImpl->mRemoteLocalNodeIdsHDF5Controller = remoteLocalNodeIdsHDF5Controller;
}


void XdmfMap::traverse(const boost::shared_ptr<XdmfBaseVisitor> visitor)
{
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
	localNodeIds->setHDF5Controller(mImpl->mLocalNodeIdsHDF5Controller);
	remoteTaskIds->setHDF5Controller(mImpl->mRemoteTaskIdsHDF5Controller);
	remoteLocalNodeIds->setHDF5Controller(mImpl->mRemoteLocalNodeIdsHDF5Controller);
	localNodeIds->accept(visitor);
	remoteTaskIds->accept(visitor);
	remoteLocalNodeIds->accept(visitor);
	mImpl->mLocalNodeIdsHDF5Controller = localNodeIds->getHDF5Controller();
	mImpl->mRemoteTaskIdsHDF5Controller = remoteTaskIds->getHDF5Controller();
	mImpl->mRemoteLocalNodeIdsHDF5Controller = remoteLocalNodeIds->getHDF5Controller();
}
