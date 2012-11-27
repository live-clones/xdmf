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
   * @dontinclude ExampleXdmfMap.cpp
   * @skipline New
   *
   * Python
   *
   * @dontinclude XdmfExampleMap.py
   * @skipline New
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
   * @dontinclude ExampleXdmfMap.cpp
   * @skipline holdGlobalNodes
   * @until New
   * The globalNodeIDs are placed into the attribute
   * The index they are at in the attribute corresponds to their localNodeID
   * @skipline nodeAttribute
   * @until push_back
   * The Attribute is then added to the vector
   * The index that the Attribute has in the vector corresponds to a task id
   * Using this method add all the required nodes
   * @skipline nodeAttribute
   * until vector
   * returns a vector of maps that holds the equivalencies for the nodes provided
   * for example if Attribute 1 had globalNodeID 3 at localNodeID 2
   * and Attribute 3 had globalNodeID 3 at localNodeID 5
   * then map 1 would have an entry of (3, 5, 2)
   * and map 3 would have an entry of (1, 2, 5)
   * The entries are formatted (remoteTaskID, remoteLocalNodeID, localNodeID)
   *
   * Python
   *
   * @dontinclude XdmfExampleMap.py
   * create attributes for each task id
   * the index of the node id in the attribute is the local node id
   * @skipline map1Attribute
   * @until map2Attribute.insertAsInt32
   * insert the attributes into a vector
   * the id of the attribute in the vector is equal to the task id
   * @skipline testVector
   * @until New
   * returns a vector of maps that holds the equivalencies for the nodes provided
   * for example if Attribute 1 had globalNodeID 3 at localNodeID 2
   * and Attribute 3 had globalNodeID 3 at localNodeID 5
   * then map 1 would have an entry of (3, 5, 2)
   * and map 3 would have an entry of (1, 2, 5)
   * The entries are formatted (remoteTaskID, remoteLocalNodeID, localNodeID)
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
   * @dontinclude ExampleXdmfMap.cpp
   * Assuming that exampleMap is a shared pointer to an XdmfMap object filled with the following tuples
   * (1, 1, 9)
   * (1, 2, 8)
   * (2, 3, 7)
   * (2, 4, 6)
   * (3, 5, 5)
   * (3, 6, 4)
   * @skipline getMap
   * taskIDMap now contains the same tuples as exampleMap
   * @skipline begin
   * @until first
   * taskIDValue is now equal to 1, because that is the first taskID listed
   * @skipline nodeIDMap
   * nodeIDMap now contains the following tuples because it retrieved the tuples associated with taskID 1
   * (1, 9)
   * (2, 8)
   * @skipline begin
   * @until first
   * localNodeValue is now equal to 1, because it is the first entry in the first tuple in the set
   * @skipline remoteNodeSet
   * remoteNodeSet now contains all remoteLocalNodeIDs for taskID 1 and LocalNode 1
   * in this case remoteNodeSet contains (9)
   * @skipline begin
   * @until remoteNodeValue
   * remoteNodeValue now equals 9
   *
   * Python
   *
   * @dontinclude XdmfExampleMap.py
   * Assuming that exampleMap is a shared pointer to an XdmfMap object filled with the following tuples
   * (1, 1, 9)
   * (1, 2, 8)
   * (2, 3, 7)
   * (2, 4, 6)
   * (3, 5, 5)
   * (3, 6, 4)
   * @skipline getMap
   * @until break
   * This prints out all the task IDs
   * unless the break is called on the last iteration the program will fail because of an issue with SWIG
   * @skipline nodeIDMap
   * nodeIDMap now contains the following tuples because it retrieved the tuples associated with taskID 1
   * (1, 9)
   * (2, 8)
   * @skipline 0
   * @until break
   * This prints out all the local node IDs
   * unless the break is called on the last iteration the program will fail because of an issue with SWIG
   * @skipline val
   * @until print
   * prints out all the remote node values associated with taskID 1 and localNode 1
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
   * @dontinclude ExampleXdmfMap.cpp
   * Assumming that exampleMap is a shared pointer to a XdmfMap object
   * @skipline getName
   *
   * Python
   *
   * @dontinclude XdmfExampleMap.py
   * Assumming that exampleMap is a shared pointer to a XdmfMap object
   * @skipline getName
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
   * @dontinclude ExampleXdmfMap.cpp
   * Assuming that exampleMap is a shared pointer to an XdmfMap object filled with the following tuples
   * (1, 1, 9)
   * (1, 2, 8)
   * (2, 3, 7)
   * (2, 4, 6)
   * (3, 5, 5)
   * (3, 6, 4)
   * @skipline getRemoteNodeIds
   * nodeIDMap now contains the following tuples because it retrieved the tuples associated with taskID 1
   * (1, 9)
   * (2, 8)
   * @skipline begin
   * @until first
   * localNodeValue is now equal to 1, because it is the first entry in the first tuple in the set
   * @skipline remoteNodeSet
   * remoteNodeSet now contains all remoteLocalNodeIDs for taskID 1 and LocalNode 1
   * in this case remoteNodeSet contains (9)
   * @skipline begin
   * @until remoteNodeValue
   * remoteNodeValue now equals 9
   *
   * Python
   *
   * @dontinclude XdmfExampleMap.py
   * Assuming that exampleMap is a shared pointer to an XdmfMap object filled with the following tuples
   * (1, 1, 9)
   * (1, 2, 8)
   * (2, 3, 7)
   * (2, 4, 6)
   * (3, 5, 5)
   * (3, 6, 4)
   * @skipline getRemoteNodeIds
   * nodeIDMap now contains the following tuples because it retrieved the tuples associated with taskID 1
   * (1, 9)
   * (2, 8)
   * @skipline 0
   * @until break
   * This prints out all the local node IDs
   * unless the break is called on the last iteration the program will fail because of an issue with SWIG
   * @skipline val
   * @until print
   * prints out all the remote node values associated with taskID 1 and localNode 1
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
   * @dontinclude ExampleXdmfMap.cpp
   * @skipline New
   * @skipline newRemoteTaskID
   * @until insert
   * This inserts an entry of (4, 7, 3) into the map
   *
   * Python
   *
   * @dontinclude XdmfExampleMap.py
   * @skipline New
   * @skipline newRemoteTaskID
   * @until insert
   * This inserts an entry of (4, 7, 3) into the map
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
   * @dontinclude ExampleXdmfMap.cpp
   * Assumming that exampleMap is a shared pointer to a XdmfMap object
   * @skipline isInitialized
   * @until }
   *
   * Python
   *
   * @dontinclude XdmfExampleMap.py
   * Assumming that exampleMap is a shared pointer to a XdmfMap object
   * @skipline isInitialized
   * @until read
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
   * @dontinclude ExampleXdmfMap.cpp
   * Assumming that exampleMap is a shared pointer to a XdmfMap object
   * @skipline isInitialized
   * @until }
   *
   * Python
   *
   * @dontinclude XdmfExampleMap.py
   * Assumming that exampleMap is a shared pointer to a XdmfMap object
   * @skipline isInitialized
   * @until read
   */
  void read();

  /**
   * Release all data held in memory. The heavy data remain attached.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfMap.cpp
   * Assumming that exampleMap is a shared pointer to a XdmfMap object
   * @skipline release
   *
   * Python
   *
   * @dontinclude XdmfExampleMap.py
   * Assumming that exampleMap is a shared pointer to a XdmfMap object
   * @skipline release
   */
  void release();

  /**
   * Set the heavy data controllers for this map.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfMap.cpp
   * @skipline hdf5FilePath
   * @until setHeavyDataController
   *
   * Python
   *
   * @dontinclude XdmfExampleMap.py
   * @skipline hdf5FilePath
   * @until setHeavyDataController
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
   * @dontinclude ExampleXdmfMap.cpp
   * @skipline New
   * First create a std::map<int, std::set<int> >
   * @skipline taskMap
   * @until taskMap.insert
   * Then insert it into a std::map<int, std::map<int, std::set<int> > >
   * Repeat as many times as necessary.
   * @skipline nodeMap
   * @until taskMap.insert
   * then the result is set to the XdmfMap
   * @skipline setMap
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
   *
   * Python
   *
   * @dontinclude XdmfExampleMap.py
   * @skipline newTaskMap
   * @until newNodeIdMap[4]
   * newNodeIdMap now contains the following
   * (2, 3)
   * (2, 6)
   * (2, 8)
   * (3, 3)
   * (4, 7)
   * (4, 9)
   * @skipline secondNodeIdMap
   * @until secondNodeIdMap[9]
   * secondNodeIdMap now contains the following
   * (5, 3)
   * (5, 6)
   * (5, 8)
   * (7, 3)
   * (9, 7)
   * (9, 9)
   * @skipline newTaskMap
   * @until setMap
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
   * @dontinclude ExampleXdmfMap.cpp
   * Assumming that exampleMap is a shared pointer to a XdmfMap object
   * @skipline newName
   * @until setName
   *
   * Python
   *
   * @dontinclude XdmfExampleMap.py
   * Assumming that exampleMap is a shared pointer to a XdmfMap object
   * @skipline newName
   * @until setName
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
