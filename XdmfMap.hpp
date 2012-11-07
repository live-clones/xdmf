/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfMap.hpp                                                         */
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

#ifndef XDMFMAP_HPP_
#define XDMFMAP_HPP_

// Forward Declarations
class XdmfArray;
class XdmfAttribute;
class XdmfHeavyDataController;

// Includes
#include "Xdmf.hpp"
#include "XdmfItem.hpp"

#include <set>

/**
 * @brief Boundary communicator map for partitioned spatial
 * collections.
 *
 * Provides mechanism for mapping nodes across partition
 * boundaries. Each partitioned grid contains its own map, mapping its
 * own nodes to all other nodes in the global system.
 *
 * There are two methods for constructing XdmfMaps:
 *
 * Calling New() with no parameters will construct an empty map. The
 * map can be filled manually with subsequent insert commands.
 *
 * Calling New(const std::vector<shared_ptr<XdmfAttribute> > &
 * globalNodeIds) will construct XdmfMaps for each grid in an entire
 * global system. Each entry in the vector contains the globalNodeIds
 * for that partition. The constructor accepts global node ids for
 * each partition to construct the proper XdmfMaps.
 */
class XDMF_EXPORT XdmfMap : public XdmfItem {

public:

  typedef int node_id;
  typedef int task_id;
  typedef std::map<node_id, std::set<node_id> > node_id_map;

  /**
   * Create a new XdmfMap.
   *
   * Example of use:
   *
   * C++
   *
   * @code {.cpp}
   * shared_ptr<XdmfMap> exampleMap = XdmfMap::New();
   * @endcode
   *
   * Python
   *
   * @code {.py}
   * exampleMap = XdmfMap.New()
   * @endcode
   *
   * @return constructed XdmfMap.
   */
  static shared_ptr<XdmfMap> New();

  /**
   * Create XdmfMaps for each grid in a domain decomposed mesh. Each
   * entry in the globalNodeIds vector contains the global node ids
   * for that partition.
   *
   * Example of use:
   *
   * C++
   *
   * @code {.cpp}
   * std::vector<shared_ptr<XdmfAttribute> > holdGlobalNodes;
   * shared_ptr<XdmfAttribute> nodeAttribute = XdmfAttribute::New();
   * //The globalNodeIDs are placed into the attribute
   * //The index they are at in the attribute corresponds to their localNodeID
   * nodeAttribute->insert(0, 1);
   * nodeAttribute->insert(1, 5);
   * nodeAttribute->insert(2, 8);
   * nodeAttribute->insert(3, 9);
   * nodeAttribute->insert(4, 4);
   * //The Attribute is then added to the vector
   * //The index that the Attribute has in the vector corresponds to a task id
   * holdGlobalNodes.push_back(nodeAttribute);
   * //using this method add all the required nodes
   * std::vector<shared_ptr<XdmfMap> > exampleMaps = XdmfMap::New(holdGlobalNodes);
   * //returns a vector of maps that holds the equivalencies for the nodes provided
   * //for example if Attribute 1 had globalNodeID 3 at localNodeID 2
   * //and Attribute 3 had globalNodeID 3 at localNodeID 5
   * //then map 1 would have an entry of (3, 5, 2)
   * //and map 3 would have an entry of (1, 2, 5)
   * //The entries are formatted (remoteTaskID, remoteLocalNodeID, localNodeID)
   * @endcode
   *
   * Python
   *
   * @code {.py}
   * grid = XdmfUnstructuredGrid.New()
   *
   * '''
   * create attributes for each task id
   * the index of the node id in the attribute is the local node id
   * '''
   * map1Attribute = XdmfAttribute.New()
   * map1Attribute.setName("Test Attribute")
   * map1Attribute.setType(XdmfAttributeType.Scalar())
   * map1Attribute.setCenter(XdmfAttributeCenter.Node())
   * map1Vals = [1,2,3,4,5,7,9]
   * map1Attribute.insertAsInt32(0, map1Vals)
   *
   * map2Attribute = XdmfAttribute.New()
   * map2Attribute.setName("Test Attribute")
   * map2Attribute.setType(XdmfAttributeType.Scalar())
   * map2Attribute.setCenter(XdmfAttributeCenter.Node())
   * map2Vals = [9,8,7,4,3]
   * map2Attribute.insertAsInt32(0, map2Vals)
   *
   * '''
   * insert the attributes into a vector
   * the id of the attribute in the vector is equal to the task id
   * '''
   * testVector = AttributeVector()
   * testVector.push_back(map1Attribute)
   * testVector.push_back(map2Attribute)
   *
   * testMap = XdmfMap.New(testVector)
   *
   * '''
   * returns a vector of maps that holds the equivalencies for the nodes provided
   * for example if Attribute 1 had globalNodeID 3 at localNodeID 2
   * and Attribute 3 had globalNodeID 3 at localNodeID 5
   * then map 1 would have an entry of (3, 5, 2)
   * and map 3 would have an entry of (1, 2, 5)
   * The entries are formatted (remoteTaskID, remoteLocalNodeID, localNodeID)
   * '''
   *
   * grid.insert(testMap[0])
   * grid.insert(testMap[1])
   * @endcode
   *
   * @param globalNodeIds a vector of attributes containing globalNodeId
   * values for each partition to be mapped.
   *
   * @return constructed XdmfMaps for each partition. The size of the vector
   * will be the same as the globalNodeIds vector.
   */
  static std::vector<shared_ptr<XdmfMap> >
  New(const std::vector<shared_ptr<XdmfAttribute> > & globalNodeIds);

  virtual ~XdmfMap();

  LOKI_DEFINE_VISITABLE(XdmfMap, XdmfItem);
  static const std::string ItemTag;

  std::map<std::string, std::string> getItemProperties() const;

  /**
   * Get stored boundary communicator map.
   *
   * Example of use:
   *
   * C++
   *
   * @code {.cpp}
   * //Assuming that exampleMap is a shared pointer to an XdmfMap object filled with the following tuples
   * //(1, 1, 9)
   * //(1, 2, 8)
   * //(2, 3, 7)
   * //(2, 4, 6)
   * //(3, 5, 5)
   * //(3, 6, 4)
   * std::map<int, std::map<int, int> > taskIDMap = exampleMap->getMap();
   * //taskIDMap now contains the same tuples as exampleMap
   * std::map<int, std::map<int, int> >::iterator taskWalker = taskIDMap.begin();
   * int taskIDValue = (*taskWalker).first;
   * //taskIDValue is now equal to 1, because that is the first taskID listed
   * std::map<int, int> nodeIDMap = taskIDMap[1];
   * //nodeIDMap now contains the following tuples because it retrieved the tuples associated with taskID 1
   * //(1, 9)
   * //(2, 8)
   * std::map<int, int>::iterator mapWalker = nodeIDMap.begin();
   * int localNodeValue = (*mapWalker).first;
   * //localNodeValue is now equal to 1, because it is the first entry in the first tuple in the set
   * std::set<int> remoteNodeSet = exampleMap[1];
   * //remoteNodeSet now contains all remoteLocalNodeIDs for taskID 1 and LocalNode 1
   * //in this case remoteNodeSet contains (9)
   * std::set<int>::iterator setWalker = remoteNodeSet.begin();
   * int remoteNodeValue = (*setWalker);
   * //remoteNodeValue now equals 9
   * @endcode
   *
   * Python
   *
   * @code {.py}
   * '''
   * Assuming that exampleMap is a shared pointer to an XdmfMap object filled with the following tuples
   * (1, 1, 9)
   * (1, 2, 8)
   * (2, 3, 7)
   * (2, 4, 6)
   * (3, 5, 5)
   * (3, 6, 4)
   * '''
   * taskIDMap = exampleMap.getMap()
   * i = 0
   * for val in taskIDMap:
   *   print val
   *   i = i + 1
   *   if i == taskIDMap.size():
   *     break
   * '''
   * This prints out all the task IDs
   * unless the break is called on the last iteration the program will fail because of an issue with SWIG
   * '''
   * nodeIDMap = taskIDMap[1]
   * '''
   * nodeIDMap now contains the following tuples because it retrieved the tuples associated with taskID 1
   * (1, 9)
   * (2, 8)
   * '''
   * i = 0
   * for val in nodeIDMap:
   *   print val
   *   i = i + 1
   *   if i == nodeIDMap.size():
   *     break
   * '''
   * This prints out all the local node IDs
   * unless the break is called on the last iteration the program will fail because of an issue with SWIG
   * '''
   * for val in nodeIDMap[1]:
   *   print val
   * '''
   * prints out all the remote node values associated with taskID 1 and localNode 1
   * '''
   * @endcode
   *
   * @return stored boundary communicator map.
   */
  std::map<task_id, node_id_map> getMap() const;

  /**
   * Get name of boundary communicator map.
   *
   * Example of use:
   *
   * C++
   *
   * @code {.cpp}
   * //Assumming that exampleMap is a shared pointer to a XdmfMap object
   * std::string exampleName = exampleMap->getName();
   * @endcode
   *
   * Python
   *
   * @code {.py}
   * '''
   * Assumming that exampleMap is a shared pointer to a XdmfMap object
   * '''
   * exampleName = exampleMap.getName()
   * @endcode
   *
   * @return name of boundary communicator map.
   */
  std::string getName() const;

  /**
   * Given a remote task id return a map of local node ids to remote
   * node ids
   *
   * Example of use:
   *
   * C++
   *
   * @code {.cpp}
   * //Assuming that exampleMap is a shared pointer to an XdmfMap object filled with the following tuples
   * //(1, 1, 9)
   * //(1, 2, 8)
   * //(2, 3, 7)
   * //(2, 4, 6)
   * //(3, 5, 5)
   * //(3, 6, 4)
   * std::map<int, int> nodeIDMap = exampleMap->getRemoteNodeIds(1);
   * //nodeIDMap now contains the following tuples because it retrieved the tuples associated with taskID 1
   * //(1, 9)
   * //(2, 8)
   * std::map<int, int>::iterator mapWalker = nodeIDMap.begin();
   * int localNodeValue = (*mapWalker).first;
   * //localNodeValue is now equal to 1, because it is the first entry in the first tuple in the set
   * std::set<int> remoteNodeSet = exampleMap[1];
   * //remoteNodeSet now contains all remoteLocalNodeIDs for taskID 1 and LocalNode 1
   * //in this case remoteNodeSet contains (9)
   * std::set<int>::iterator setWalker = remoteNodeSet.begin();
   * int remoteNodeValue = (*setWalker);
   * //remoteNodeValue now equals 9
   * @endcode
   *
   * Python
   *
   * @code {.py}
   * '''
   * Assuming that exampleMap is a shared pointer to an XdmfMap object filled with the following tuples
   * (1, 1, 9)
   * (1, 2, 8)
   * (2, 3, 7)
   * (2, 4, 6)
   * (3, 5, 5)
   * (3, 6, 4)
   * '''
   * nodeIDMap = exampleMap.getRemoteNodeIds(1)
   * '''
   * nodeIDMap now contains the following tuples because it retrieved the tuples associated with taskID 1
   * (1, 9)
   * (2, 8)
   * '''
   * i = 0
   * for val in nodeIDMap:
   *   print val
   *   i = i + 1
   *   if i == nodeIDMap.size():
   *     break
   * '''
   * This prints out all the local node IDs
   * unless the break is called on the last iteration the program will fail because of an issue with SWIG
   * '''
   * for val in nodeIDMap[1]:
   *   print val
   * '''
   * prints out all the remote node values associated with taskID 1 and localNode 1
   * '''
   * @endcode
   *
   * @param remoteTaskId a task id to retrieve mapping for.
   *
   * @return a map of local node ids to a vector of remote node ids on
   * remoteTaskId.
   */
  node_id_map getRemoteNodeIds(const task_id remoteTaskId);

  std::string getItemTag() const;

  using XdmfItem::insert;

  /**
   * Insert a new entry in map.
   *
   * Example of use:
   *
   * C++
   *
   * @code {.cpp}
   * shared_ptr<XdmfMap> exampleMap = XdmfMap::New();
   * unsigned int newRemoteTaskID = 4;
   * unsigned int newLocalNodeID = 7;
   * unsigned int newRemoteLocalNodeID = 3;
   * exampleMap->insert(newRemoteTaskID, newLocalNodeID, newRemoteLocalNodeID);
   * //This inserts an entry of (4, 7, 3) into the map
   * @endcode
   *
   * Python
   *
   * @code {.py}
   * exampleMap = XdmfMap.New()
   * newRemoteTaskID = 4
   * newLocalNodeID = 7
   * newRemoteLocalNodeID = 3
   * exampleMap.insert(newRemoteTaskID, newLocalNodeID, newRemoteLocalNodeID)
   * '''
   * This inserts an entry of (4, 7, 3) into the map
   * '''
   * @endcode
   *
   * @param remoteTaskId task id where the remoteLoalNodeId is located.
   * @param localNodeId the node id of the node being mapped.
   * @param remoteLocalNodeId a node id on the remoteTaskId that the
   * localNodeId is mapped to.
   */
  void insert(const task_id  remoteTaskId,
              const node_id  localNodeId,
              const node_id  remoteLocalNodeId);

  /**
   * Returns whether the map is initialized (contains values in
   * memory).
   *
   * Example of use:
   *
   * C++
   *
   * @code {.cpp}
   * //Assumming that exampleMap is a shared pointer to a XdmfMap object
   * if (exampleMap->isInitialized())
   * {
   *   //Do what is to be done if the map contains values
   * }
   * @endcode
   *
   * Python
   *
   * @code {.py}
   * '''
   * Assumming that exampleMap is a shared pointer to a XdmfMap object
   * '''
   * if exampleMap.isInitialized():
   *   '''
   *   Do what is to be done if the map contains values
   *   '''
   * @endcode
   *
   * @return bool true if map contains values in memory.
   */
  bool isInitialized() const;

  /**
   * Read data from disk into memory.
   *
   * Example of use:
   *
   * C++
   *
   * @code {.cpp}
   * //Assumming that exampleMap is a shared pointer to a XdmfMap object
   * if (!exampleMap->isInitialized())
   * {
   *   exampleMap->read();
   * }
   * @endcode
   *
   * Python
   *
   * @code {.py}
   * '''
   * Assumming that exampleMap is a shared pointer to a XdmfMap object
   * '''
   * if not(exampleMap.isInitialized()):
   *   exampleMap.read()
   * @endcode
   */
  void read();

  /**
   * Release all data held in memory. The heavy data remain attached.
   *
   * Example of use:
   *
   * C++
   *
   * @code {.cpp}
   * //Assumming that exampleMap is a shared pointer to a XdmfMap object
   * exampleMap->release();
   * @endcode
   *
   * Python
   *
   * @code {.py}
   * '''
   * Assumming that exampleMap is a shared pointer to a XdmfMap object
   * '''
   * exampleMap.release()
   * @endcode
   */
  void release();

  /**
   * Set the heavy data controllers for this map.
   *
   * Example of use:
   *
   * C++
   *
   * @code {.cpp}
   * std::string hdf5FilePath = "The HDF5 file path goes here";
   * std::string hdf5SetPath = "The HDF5 set path goes here";
   * int startIndex = 0; //start at the beginning
   * int readStride = 1; //read all values
   * int readNumber = 10; //read 10 values
   * shared_ptr<XdmfHDF5Controller> newRemoteTaskController = XdmfHDF5Controller::New(
   *   hdf5FilePath, hdf5SetPath, XdmfArrayType::Int32(),
   *   startIndex, readStride, readNumber);
   * hdf5FilePath = "The HDF5 file path for the local nodes goes here";
   * hdf5SetPath = "The HDF5 set path for the local nodes goes here";
   * shared_ptr<XdmfHDF5Controller> newLocalNodeController = XdmfHDF5Controller::New(
   *   hdf5FilePath, hdf5SetPath, XdmfArrayType::Int32(),
   *   startIndex, readStride, readNumber);
   * hdf5FilePath = "The HDF5 file path for the remote local nodes goes here";
   * hdf5SetPath = "The HDF5 set path for the remote local nodes goes here";
   * shared_ptr<XdmfHDF5Controller> newRemoteLocalNodeController = XdmfHDF5Controller::New(
   *   hdf5FilePath, hdf5SetPath, XdmfArrayType::Int32(),
   *   startIndex, readStride, readNumber);
   * shared_ptr<XdmfMap> exampleMap = XdmfMap::New();
   * exampleMap->setHeavyDataControllers(newRemoteTaskController, newLocalNodeController, newRemoteLocalNodeController);
   * @endcode
   *
   * Python
   *
   * @code {.py}
   * hdf5FilePath = "The HDF5 file path goes here"
   * hdf5SetPath = "The HDF5 set path goes here"
   * startIndex = 0
   * '''
   * start at the beginning
   * '''
   * readStride = 1
   * '''
   * read all values
   * '''
   * readNumber = 10
   * '''
   * read 10 values
   * '''
   * newRemoteTaskController = XdmfHDF5Controller.New(
   *   hdf5FilePath, hdf5SetPath, XdmfArrayType.Int32(),
   *   startIndex, readStride, readNumber)
   * hdf5FilePath = "The HDF5 file path for the local nodes goes here"
   * hdf5SetPath = "The HDF5 set path for the local nodes goes here"
   * newLocalNodeController = XdmfHDF5Controller.New(
   *   hdf5FilePath, hdf5SetPath, XdmfArrayType.Int32(),
   *   startIndex, readStride, readNumber)
   * hdf5FilePath = "The HDF5 file path for the remote local nodes goes here"
   * hdf5SetPath = "The HDF5 set path for the remote local nodes goes here"
   * newRemoteLocalNodeController = XdmfHDF5Controller.New(
   *   hdf5FilePath, hdf5SetPath, XdmfArrayType.Int32(),
   *   startIndex, readStride, readNumber)
   * exampleMap = XdmfMap.New()
   * exampleMap.setHeavyDataControllers(newRemoteTaskController, newLocalNodeController, newRemoteLocalNodeController)
   * @endcode
   *
   * @param remoteTaskIdsController an XdmfHeavyDataController to the remote
   * task ids dataset.
   * @param localNodeIdsController an XdmfHeavyDataController to the local
   * node ids dataset.
   * @param remoteLocalNodeIdsController an XdmfHeavyDataController to the
   * remote local node ids dataset.
   */
  void
  setHeavyDataControllers(shared_ptr<XdmfHeavyDataController> remoteTaskIdsController,
                          shared_ptr<XdmfHeavyDataController> localNodeIdsController,
                          shared_ptr<XdmfHeavyDataController> remoteLocalNodeIdsController);

  /**
   * Set the boundary communicator map.
   *
   * Example of use:
   *
   * C++
   *
   * @code {.cpp}
   * shared_ptr<XdmfMap> exampleMap = XdmfMap::New();
   * //First create several std::map<int, int>
   * std::map<int, int> nodeMap;
   * nodeMap.insert(pair<int, int>(2,3));
   * nodeMap.insert(pair<int, int>(2,6));
   * nodeMap.insert(pair<int, int>(2,8));
   * nodeMap.insert(pair<int, int>(3,3));
   * nodeMap.insert(pair<int, int>(4,7));
   * nodeMap.insert(pair<int, int>(4,9));
   * std::map<int, int> nodeMap2;
   * nodeMap.insert(pair<int, int>(5,3));
   * nodeMap.insert(pair<int, int>(5,6));
   * nodeMap.insert(pair<int, int>(5,8));
   * nodeMap.insert(pair<int, int>(7,3));
   * nodeMap.insert(pair<int, int>(9,7));
   * nodeMap.insert(pair<int, int>(9,9));
   * //Then create a std::map<int, std::map<int, int> >
   * std::map<int, std::map<int, int> > taskMap;
   * taskMap.insert(pair<int, std::map<int, int> >(1, nodeMap);
   * taskMap.insert(pair<int, std::map<int, int> >(2, nodeMap2);
   * //then the result is set to the XdmfMap
   * exampleMap->setMap(taskMap);
   * //This fills the XdmfMap with the following tuples
   * //(1, 2, 3)
   * //(1, 2, 6)
   * //(1, 2, 8)
   * //(1, 3, 3)
   * //(1, 4, 7)
   * //(1, 4, 9)
   * //(2, 5, 3)
   * //(2, 5, 6)
   * //(2, 5, 8)
   * //(2, 7, 3)
   * //(2, 9, 7)
   * //(2, 9, 9)
   * @endcode
   *
   * Python
   *
   * @code {.py}
   * newTaskMap = XdmfMapMap()
   * newNodeIdMap = XdmfMapNodeIdMap()
   * newNodeIdMap[2] = (3, 6, 8)
   * newNodeIdMap[3] = (3,)
   * newNodeIdMap[4] = (7,9)
   * '''
   * newNodeIdMap now contains the following
   * (2, 3)
   * (2, 6)
   * (2, 8)
   * (3, 3)
   * (4, 7)
   * (4, 9)
   * '''
   * secondNodeIdMap = XdmfMapNodeIdMap()
   * secondNodeIdMap[5] = (3, 6, 8)
   * secondNodeIdMap[7] = (3,)
   * secondNodeIdMap[9] = (7,9)
   * '''
   * secondNodeIdMap now contains the following
   * (5, 3)
   * (5, 6)
   * (5, 8)
   * (7, 3)
   * (9, 7)
   * (9, 9)
   * '''
   * newTaskMap[1] = newNodeIdMap
   * newTaskMap[2] = secondNodeIdMap
   * exampleMap = XdmfMap.New()
   * exampleMap.setMap(newTaskMap)
   * '''
   * This fills the XdmfMap with the following tuples
   * (1, 2, 3)
   * (1, 2, 6)
   * (1, 2, 8)
   * (1, 3, 3)
   * (1, 4, 7)
   * (1, 4, 9)
   * (2, 5, 3)
   * (2, 5, 6)
   * (2, 5, 8)
   * (2, 7, 3)
   * (2, 9, 7)
   * (2, 9, 9)
   * '''
   * @endcode
   *
   * @param map the boundary communicator map to store.
   */
  void setMap(std::map<task_id, node_id_map> map);

  /**
   * Set the name of the boundary communicator map.
   *
   * Example of use:
   *
   * C++
   *
   * @code {.cpp}
   * //Assumming that exampleMap is a shared pointer to a XdmfMap object
   * std::string newName = "New Name";
   * exampleMap->setName(newName);
   * @endcode
   *
   * Python
   *
   * @code {.py}
   * '''
   * Assumming that exampleMap is a shared pointer to a XdmfMap object
   * '''
   * newName = "New Name"
   * exampleMap.setName(newName)
   * @endcode
   *
   * @param name the name of the boundary communicator map to set.
   */
  void setName(const std::string & name);

  void traverse(const shared_ptr<XdmfBaseVisitor> visitor);

protected:

  XdmfMap();

  virtual void
  populateItem(const std::map<std::string, std::string> & itemProperties,
               const std::vector<shared_ptr<XdmfItem> > & childItems,
               const XdmfCoreReader * const reader);

private:

  XdmfMap(const XdmfMap & map);  // Not implemented.
  void operator=(const XdmfMap & map);  // Not implemented.

  shared_ptr<XdmfHeavyDataController> mLocalNodeIdsController;
  // remoteTaskId | localNodeId | remoteLocalNodeId
  std::map<task_id, node_id_map > mMap;
  std::string mName;
  shared_ptr<XdmfHeavyDataController> mRemoteLocalNodeIdsController;
  shared_ptr<XdmfHeavyDataController> mRemoteTaskIdsController;

};

#endif /* XDMFMAP_HPP_ */
