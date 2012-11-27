#include "XdmfMap.hpp"
#include "XdmfAttribute.h[["
#include "XdmfHDF5Controller.hpp"

int main(int, char **)
{
	shared_ptr<XdmfMap> exampleMap = XdmfMap::New();

	std::vector<shared_ptr<XdmfAttribute> > holdGlobalNodes;
	shared_ptr<XdmfAttribute> nodeAttribute = XdmfAttribute::New();
	nodeAttribute->insert(0, 1);
	nodeAttribute->insert(1, 5);
	nodeAttribute->insert(2, 8);
	nodeAttribute->insert(3, 9);
	nodeAttribute->insert(4, 4);
	holdGlobalNodes.push_back(nodeAttribute);
	nodeAttribute = XdmfAttribute::New();
        nodeAttribute->insert(0, 2);
        nodeAttribute->insert(1, 6);
        nodeAttribute->insert(2, 9);
        nodeAttribute->insert(3, 0);
        nodeAttribute->insert(4, 5);
	holdGlobalNodes.push_back(nodeAttribute);
	std::vector<shared_ptr<XdmfMap> > exampleMapVector = XdmfMap::New(holdGlobalNodes);

	std::map<int, std::map<int, std::set<int> > > taskMap;
	std::map<int, std::set<int> > nodeMap;
	std::set<int> remoteIDset;
	remoteIDset.insert(3);
	remoteIDset.insert(6);
	remoteIDset.insert(8);
	nodeMap.insert(pair<int, std::set<int> >(2, remoteIDset));
	remoteIDset.clear();
	remoteIDset.insert(3);
	nodeMap.insert(pair<int, std::set<int> >(3, remoteIDset));
	remoteIDset.clear();
	remoteIDset.insert(7);
	remoteIDset.insert(9);
	nodeMap.insert(pair<int, std::set<int> >(4, remoteIDset));
	taskMap.insert(pair<int, std::map<int, std::set<int> > >(1, nodeMap));
	nodeMap.clear();
	remoteIDset.clear();
	remoteIDset.insert(3);
	remoteIDset.insert(6);
	remoteIDset.insert(8);
	nodeMap.insert(pair<int, std::set<int> >(5, remoteIDset));
	remoteIDset.clear();
	remoteIDset.insert(3);
	nodeMap.insert(pair<int, std::set<int> >(7, remoteIDset));
	remoteIDset.clear();
	remoteIDset.insert(7);
	remoteIDset.insert(9);
	nodeMap.insert(pair<int, std::set<int> >(9, remoteIDset));
	taskMap.insert(pair<int, std::map<int, std::set<int> > >(2, nodeMap));
	exampleMap->setMap(taskMap);

        unsigned int newRemoteTaskID = 4;
        unsigned int newLocalNodeID = 7;
        unsigned int newRemoteLocalNodeID = 3;
        exampleMap->insert(newRemoteTaskID, newLocalNodeID, newRemoteLocalNodeID);


	std::string newName = "New Name";
	exampleMap->setName(newName);


	std::map<int, std::map<int, std::set<int> > > taskIDMap = exampleMap->getMap();
	std::map<int, std::map<int, std::set<int> > >::iterator taskWalker = taskIDMap.begin();
	int taskIDValue = (*taskWalker).first;
	std::map<int, std::set<int> > nodeIDMap = taskIDMap[1];
	std::map<int, std::set<int> >::iterator mapWalker = nodeIDMap.begin();
	int localNodeValue = (*mapWalker).first;
	std::set<int> remoteNodeSet = exampleMap[1];
	std::set<int>::iterator setWalker = remoteNodeSet.begin();
	int remoteNodeValue = (*setWalker);

	std::map<int, std::set<int> > nodeIDMap = exampleMap->getRemoteNodeIds(1);
	std::map<int, std::set<int> >::iterator mapWalker = nodeIDMap.begin();
	int localNodeValue = (*mapWalker).first;
	std::set<int> remoteNodeSet = exampleMap[1];
	std::set<int>::iterator setWalker = remoteNodeSet.begin();
	int remoteNodeValue = (*setWalker);



	std::string exampleName = exampleMap->getName();

	if (!exampleMap->isInitialized())
	{
		exampleMap->read();
	}

	std::string hdf5FilePath = "The HDF5 file path goes here";
	std::string hdf5SetPath = "The HDF5 set path goes here";
	int startIndex = 0; //start at the beginning
	int readStride = 1; //read all values
	int readNumber = 10; //read 10 values
	shared_ptr<XdmfHDF5Controller> newRemoteTaskController = XdmfHDF5Controller::New(
		hdf5FilePath, hdf5SetPath, XdmfArrayType::Int32(),
		startIndex, readStride, readNumber);
	hdf5FilePath = "The HDF5 file path for the local nodes goes here";
	hdf5SetPath = "The HDF5 set path for the local nodes goes here";
	shared_ptr<XdmfHDF5Controller> newLocalNodeController = XdmfHDF5Controller::New(
		hdf5FilePath, hdf5SetPath, XdmfArrayType::Int32(),
		startIndex, readStride, readNumber);
	hdf5FilePath = "The HDF5 file path for the remote local nodes goes here";
	hdf5SetPath = "The HDF5 set path for the remote local nodes goes here";
	shared_ptr<XdmfHDF5Controller> newRemoteLocalNodeController = XdmfHDF5Controller::New(
		hdf5FilePath, hdf5SetPath, XdmfArrayType::Int32(),
		startIndex, readStride, readNumber);
	shared_ptr<XdmfMap> exampleMap = XdmfMap::New();
	exampleMap->setHeavyDataControllers(newRemoteTaskController, newLocalNodeController, newRemoteLocalNodeController);

	exampleMap->release();

	return 0;
}
