#ifndef XDMFMAP_HPP_
#define XDMFMAP_HPP_

// Forward Declarations
class XdmfArray;
class XdmfGridCollection;

// Includes
#include "XdmfItem.hpp"

/**
 * @brief Boundary communicator map for partitioned spatial collections.
 *
 * Provides mechanism for mapping nodes across partition boundaries.  Each partitioned grid contains its own map, mapping its own nodes
 * to all other nodes in the global system.
 *
 * There are two methods for constructing XdmfMaps:
 * 	Call New() providing no parameters will construct an empty map.  The map can be filled manually with subsequent insert commands.
 *	Call New(const std::vector<boost::shared_ptr<XdmfAttribute> > & globalNodeIds) will construct XdmfMaps for each grid in an entire
 *		global system.  Each entry in the vector contains the globalNodeIds for that partition.  The function takes these global node ids
 *		to construct the proper XdmfMaps.
 *
 */
class XdmfMap : public XdmfItem {

public:

	/**
	 * Create a new XdmfMap.
	 *
	 * @return constructed XdmfMap.
	 */
	static boost::shared_ptr<XdmfMap> New();

	/**
	 * Create XdmfMaps for each grid in a domain decomposed mesh.  Each entry in the globalNodeIds vector contains the global node ids
	 * for that partition.
	 *
	 * @param globalNodeIds a vector of attributes containing globalNodeId values for each partition to be mapped.
	 * @return constructed XdmfMaps for each partition.  The size of the vector will be the same as the globalNodeIds vector.
	 */
	static std::vector<boost::shared_ptr<XdmfMap> > New(const std::vector<boost::shared_ptr<XdmfAttribute> > & globalNodeIds);

	virtual ~XdmfMap();

	LOKI_DEFINE_VISITABLE(XdmfMap, XdmfItem)
	static const std::string ItemTag;

	std::map<std::string, std::string> getItemProperties() const;

	/**
	 * Given a local node id return a map containing equivalent remote node ids
	 */
	std::map<unsigned int, unsigned int> getRemoteNodeIds(const unsigned int localNodeId);

	std::string getItemTag() const;

	using XdmfItem::insert;

	void insert(const unsigned int localNodeId, const unsigned int remoteTaskId, const unsigned int remoteLocalNodeId);

	/**
	 * Returns whether the map is initialized (contains values in memory).
	 */
	bool isInitialized() const;

	/**
	 * Read data from disk into memory.
	 */
	void read();

	/**
	 * Release all data held in memory.  The HDF5Controllers remain attached.
	 */
	void release();

	void setHDF5Controllers(boost::shared_ptr<XdmfHDF5Controller> localNodeIdsHDF5Controller, boost::shared_ptr<XdmfHDF5Controller> remoteTaskIdsHDF5Controller, boost::shared_ptr<XdmfHDF5Controller> remoteLocalNodeIdsHDF5Controller);

	void traverse(const boost::shared_ptr<XdmfBaseVisitor> visitor);

protected:

	XdmfMap();
	virtual void populateItem(const std::map<std::string, std::string> & itemProperties, std::vector<boost::shared_ptr<XdmfItem> > & childItems, const XdmfCoreReader * const reader);

private:

	/**
	 * PIMPL
	 */
	class XdmfMapImpl;

	XdmfMap(const XdmfMap & map);  // Not implemented.
	void operator=(const XdmfMap & map);  // Not implemented.

	XdmfMapImpl * mImpl;

};

#endif /* XDMFMAP_HPP_ */
