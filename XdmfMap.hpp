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
   * @return constructed XdmfMap.
   */
  static shared_ptr<XdmfMap> New();

  /**
   * Create XdmfMaps for each grid in a domain decomposed mesh. Each
   * entry in the globalNodeIds vector contains the global node ids
   * for that partition.
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
   * @return stored boundary communicator map.
   */
  std::map<task_id, node_id_map> getMap() const;

  /**
   * Get name of boundary communicator map.
   *
   * @return name of boundary communicator map.
   */
  std::string getName() const;

  /**
   * Given a remote task id return a map of local node ids to remote
   * node ids
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
   * @return bool true if map contains values in memory.
   */
  bool isInitialized() const;

  /**
   * Read data from disk into memory.
   */
  void read();

  /**
   * Release all data held in memory. The heavy data remain attached.
   */
  void release();

  /**
   * Set the heavy data controllers for this map.
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
   * @param map the boundary communicator map to store.
   */
  void setMap(std::map<task_id, node_id_map> map);

  /**
   * Set the name of the boundary communicator map.
   *
   * @param name the name of the boundary communicator map to set.
   */
  void setName(const std::string & name);

  void traverse(const shared_ptr<XdmfBaseVisitor> visitor);

protected:

  XdmfMap();

  virtual void
  populateItem(const std::map<std::string, std::string> & itemProperties,
               std::vector<shared_ptr<XdmfItem> > & childItems,
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
