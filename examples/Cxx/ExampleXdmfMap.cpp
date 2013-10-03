#include "XdmfMap.hpp"
#include "XdmfAttribute.h[["
#include "XdmfHDF5Controller.hpp"

int main(int, char **)
{
        //#initialization begin

        shared_ptr<XdmfMap> exampleMap = XdmfMap::New();

        //#initialization end

        //#initializationnode begin

        std::vector<shared_ptr<XdmfAttribute> > holdGlobalNodes;
        shared_ptr<XdmfAttribute> nodeAttribute = XdmfAttribute::New();
        //The globalNodeIDs are placed into the attribute
        //The index they are at in the attribute corresponds to their localNodeID
        nodeAttribute->insert(0, 1);
        nodeAttribute->insert(1, 5);
        nodeAttribute->insert(2, 8);
        nodeAttribute->insert(3, 9);
        nodeAttribute->insert(4, 4);
        holdGlobalNodes.push_back(nodeAttribute);
        //The Attribute is then added to the vector
        //The index that the Attribute has in the vector corresponds to a task id
        //Using this method add all the required nodes
        nodeAttribute = XdmfAttribute::New();
        nodeAttribute->insert(0, 2);
        nodeAttribute->insert(1, 6);
        nodeAttribute->insert(2, 9);
        nodeAttribute->insert(3, 0);
        nodeAttribute->insert(4, 5);
        holdGlobalNodes.push_back(nodeAttribute);
        std::vector<shared_ptr<XdmfMap> > exampleMapVector = XdmfMap::New(holdGlobalNodes);
        //returns a vector of maps that holds the equivalencies for the nodes provided
        //for example if Attribute 1 had globalNodeID 3 at localNodeID 2
        //and Attribute 3 had globalNodeID 3 at localNodeID 5
        //then map 1 would have an entry of (3, 5, 2)
        //and map 3 would have an entry of (1, 2, 5)
        //The entries are formatted (remoteTaskID, remoteLocalNodeID, localNodeID)

        //#initializationnode end

        //#setMap begin

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
        //First create a std::map<int, std::set<int> >
        taskMap.insert(pair<int, std::map<int, std::set<int> > >(1, nodeMap));
        //Then insert it into a std::map<int, std::map<int, std::set<int> > >
        nodeMap.clear();
        //Repeat as many times as necessary.
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
        //then the result is set to the XdmfMap
        exampleMap->setMap(taskMap);
        //(1, 2, 3)
        //(1, 2, 6)
        //(1, 2, 8)
        //(1, 3, 3)
        //(1, 4, 7)
        //(1, 4, 9)
        //(2, 5, 3)
        //(2, 5, 6)
        //(2, 5, 8)
        //(2, 7, 3)
        //(2, 9, 7)
        //(2, 9, 9)
        //Are now in the XdmfMap

        //#setMap end


        //#inserttuple begin

        unsigned int newRemoteTaskID = 4;
        unsigned int newLocalNodeID = 7;
        unsigned int newRemoteLocalNodeID = 3;
        exampleMap->insert(newRemoteTaskID, newLocalNodeID, newRemoteLocalNodeID);
        //This inserts an entry of (4, 7, 3) into the map

        //#inserttuple end


        //#setName begin

        std::string newName = "New Name";
        exampleMap->setName(newName);

        //#setName end

        //#getMap begin

        //Assuming that exampleMap is a shared pointer to an XdmfMap object filled with the following tuples
        //(1, 1, 9)
        //(1, 2, 8)
        //(2, 3, 7)
        //(2, 4, 6)
        //(3, 5, 5)
        //(3, 6, 4)
        std::map<int, std::map<int, std::set<int> > > taskIDMap = exampleMap->getMap();
        //taskIDMap now contains the same tuples as exampleMap
        std::map<int, std::map<int, std::set<int> > >::iterator taskWalker = taskIDMap.begin();
        int taskIDValue = (*taskWalker).first;
        //taskIDValue is now equal to 1, because that is the first taskID listed
        std::map<int, std::set<int> > nodeIDMap = taskIDMap[1];
        //nodeIDMap now contains the following tuples because it retrieved the tuples associated with taskID 1
        //(1, 9)
        //(2, 8)
        std::map<int, std::set<int> >::iterator mapWalker = nodeIDMap.begin();
        int localNodeValue = (*mapWalker).first;
        //localNodeValue is now equal to 1, because it is the first entry in the first tuple in the set
        std::set<int> remoteNodeSet = exampleMap[1];
        //remoteNodeSet now contains all remoteLocalNodeIDs for taskID 1 and LocalNode 1
        //in this case remoteNodeSet contains (9)
        std::set<int>::iterator setWalker = remoteNodeSet.begin();
        int remoteNodeValue = (*setWalker);
        //remoteNodeValue now equals 9

        //#getMap end


        //#getRemoteNodeIds begin

        //Assuming that exampleMap is a shared pointer to an XdmfMap object filled with the following tuples
        //(1, 1, 9)
        //(1, 2, 8)
        //(2, 3, 7)
        //(2, 4, 6)
        //(3, 5, 5)
        //(3, 6, 4)
        std::map<int, std::set<int> > nodeIDMap = exampleMap->getRemoteNodeIds(1);
        //nodeIDMap now contains the following tuples because it retrieved the tuples associated with taskID 1
        //(1, 9)
        //(2, 8)
        std::map<int, std::set<int> >::iterator mapWalker = nodeIDMap.begin();
        int localNodeValue = (*mapWalker).first;
        //localNodeValue is now equal to 1, because it is the first entry in the first tuple in the set
        std::set<int> remoteNodeSet = exampleMap[1];
        //remoteNodeSet now contains all remoteLocalNodeIDs for taskID 1 and LocalNode 1
        //in this case remoteNodeSet contains (9)
        std::set<int>::iterator setWalker = remoteNodeSet.begin();
        int remoteNodeValue = (*setWalker);
        //remoteNodeValue now equals 9

        //#getRemoteNodeIds end

        //#getName begin

        std::string exampleName = exampleMap->getName();

        //#getName end

        //#isInitialized begin

        if (!exampleMap->isInitialized())
        {
                exampleMap->read();
        }

        //#isInitialized end

        //#setHeavyDataControllers begin

        std::string hdf5FilePath = "The HDF5 file path goes here";
        std::string hdf5SetPath = "The HDF5 set path goes here";
        int startIndex = 0; //start at the beginning
        int readStride = 1; //read all values
        int readNumber = 10; //read 10 values
        int readDataSize = 10;//maximum data size
        std::vector<int> startVector;
        startVector.push_back(startIndex);
        std::vector<int> strideVector;
        strideVector.push_back(readStride);
        std::vector<int> countVector;
        countVector.push_back(readNumber);
        std::vector<int> dataSizeVector;
        dataSizeVector.push_back(readDataSize);
        shared_ptr<XdmfHDF5Controller> newRemoteTaskController = XdmfHDF5Controller::New(
                hdf5FilePath, hdf5SetPath, XdmfArrayType::Int32(),
                startVector, strideVector, countVector, dataSizeVector);
        hdf5FilePath = "The HDF5 file path for the local nodes goes here";
        hdf5SetPath = "The HDF5 set path for the local nodes goes here";
        shared_ptr<XdmfHDF5Controller> newLocalNodeController = XdmfHDF5Controller::New(
                hdf5FilePath, hdf5SetPath, XdmfArrayType::Int32(),
                startVector, strideVector, countVector, dataSizeVector);
        hdf5FilePath = "The HDF5 file path for the remote local nodes goes here";
        hdf5SetPath = "The HDF5 set path for the remote local nodes goes here";
        shared_ptr<XdmfHDF5Controller> newRemoteLocalNodeController = XdmfHDF5Controller::New(
                hdf5FilePath, hdf5SetPath, XdmfArrayType::Int32(),
                startVector, strideVector, countVector, dataSizeVector);
        shared_ptr<XdmfMap> exampleMap = XdmfMap::New();
        exampleMap->setHeavyDataControllers(newRemoteTaskController, newLocalNodeController, newRemoteLocalNodeController);

        //#setHeavyDataControllers end

        //#release begin

        exampleMap->release();

        //#release end

        return 0;
}
