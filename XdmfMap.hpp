#ifndef XDMFMAP_HPP_
#define XDMFMAP_HPP_

// Forward Declarations
class XdmfArray;
class XdmfGridCollection;
class XdmfHeavyDataController;

// Includes
#include "XdmfMap.hpp"
#include "XdmfItem.hpp"

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
 * Calling New(const std::vector<boost::shared_ptr<XdmfAttribute> > &
 * globalNodeIds) will construct XdmfMaps for each grid in an entire
 * global system. Each entry in the vector contains the globalNodeIds
 * for that partition. The constructor accepts global node ids for
 * each partition to construct the proper XdmfMaps.
 */
class XDMF_EXPORT XdmfMap : public XdmfItem {

 public:

  /**
   * Create a new XdmfMap.
   *
   * @return constructed XdmfMap.
   */
  static boost::shared_ptr<XdmfMap> New();

  /**
   * Create XdmfMaps for each grid in a domain decomposed mesh. Each
   * entry in the globalNodeIds vector contains the global node ids
   * for that partition.
   *
   * @param globalNodeIds a vector of attributes containing globalNodeId
   * values for each partition to be mapped.
   * @return constructed XdmfMaps for each partition. The size of the vector
   * will be the same as the globalNodeIds vector.
   */
  static std::vector<boost::shared_ptr<XdmfMap> >
  New(const std::vector<boost::shared_ptr<XdmfAttribute> > & globalNodeIds);

  virtual ~XdmfMap();

  LOKI_DEFINE_VISITABLE(XdmfMap, XdmfItem);
  static const std::string ItemTag;

  std::map<std::string, std::string> getItemProperties() const;

  /**
   * Given a local node id return a map containing equivalent remote
   * node ids
   */
  std::map<unsigned int, unsigned int>
  getRemoteNodeIds(const unsigned int localNodeId);

  std::string getItemTag() const;

  using XdmfItem::insert;

  void insert(const unsigned int localNodeId,
              const unsigned int remoteTaskId,
              const unsigned int remoteLocalNodeId);

  /**
   * Returns whether the map is initialized (contains values in
   * memory).
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
   * @param localNodeIdsController an XdmfHeavyDataController to the local
   * node ids dataset.
   * @param remoteTaskIdsController an XdmfHeavyDataController to the remote
   * task ids dataset.
   * @param remoteLocalNodeIdsController an XdmfHeavyDataController to the
   * remote local node ids dataset.
   */
  void
  setHeavyDataControllers(boost::shared_ptr<XdmfHeavyDataController> localNodeIdsController,
                          boost::shared_ptr<XdmfHeavyDataController> remoteTaskIdsController,
                          boost::shared_ptr<XdmfHeavyDataController> remoteLocalNodeIdsController);

  void traverse(const boost::shared_ptr<XdmfBaseVisitor> visitor);

 protected:

  XdmfMap();

  virtual void
  populateItem(const std::map<std::string, std::string> & itemProperties,
               std::vector<boost::shared_ptr<XdmfItem> > & childItems,
               const XdmfCoreReader * const reader);

 private:

  XdmfMap(const XdmfMap & map);  // Not implemented.
  void operator=(const XdmfMap & map);  // Not implemented.

  boost::shared_ptr<XdmfHeavyDataController> mLocalNodeIdsController;
  // localNodeId | remoteTaskId | remoteLocalNodeId
  std::map<unsigned int, std::map<unsigned int, unsigned int> > mMap;
  boost::shared_ptr<XdmfHeavyDataController> mRemoteLocalNodeIdsController;
  boost::shared_ptr<XdmfHeavyDataController> mRemoteTaskIdsController;

};

#endif /* XDMFMAP_HPP_ */
