#ifndef XDMFMAP_HPP_
#define XDMFMAP_HPP_

// Forward Declarations
class XdmfAttribute;
class XdmfGridCollection;

// Includes
#include "XdmfItem.hpp"

/**
 * @brief Boundary communicator map for partitioned spatial collections.
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

	static std::vector<boost::shared_ptr<XdmfMap> > New(const std::vector<boost::shared_ptr<XdmfAttribute> > & globalNodeIds);

	virtual ~XdmfMap();

	LOKI_DEFINE_VISITABLE(XdmfMap, XdmfItem)
	static const std::string ItemTag;

	std::map<std::string, std::string> getItemProperties() const;

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
	 * Release all data held in memory.  The HDF5Controllers remains attached.
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
