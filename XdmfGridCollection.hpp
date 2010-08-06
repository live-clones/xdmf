#ifndef XDMFGRIDCOLLECTION_HPP_
#define XDMFGRIDCOLLECTION_HPP_

// Forward Declarations
class XdmfGridCollectionType;

// Includes
#include "XdmfDomain.hpp"
#include "XdmfGrid.hpp"

/**
 * @brief A spatial or temporal collection of XdmfGrids.
 *
 * A temporal collection is timestep data.  Each child grid represents the state at a single timestep.
 * A spatial collection consists of XdmfGrids that are arranged together in space.  E.g. a partitioned mesh.
 *
 * It is valid to nest collections.  A spatial collection within a temporal collection is commonly used.
 */
class XdmfGridCollection : public XdmfGrid,
	public XdmfDomain {

public:

	/**
	 * Create a new XdmfGridCollection.
	 *
	 * @return constructed XdmfGridCollection.
	 */
	static boost::shared_ptr<XdmfGridCollection> New();

	virtual ~XdmfGridCollection();

	LOKI_DEFINE_VISITABLE(XdmfGridCollection, XdmfGrid)
	static const std::string ItemTag;

	std::map<std::string, std::string> getItemProperties() const;

	std::string getItemTag() const;

	/**
	 * Get the XdmfGridCollectionType associated with this grid collection.
	 *
	 * @return XdmfGridCollectionType of this collection.
	 */
	boost::shared_ptr<const XdmfGridCollectionType> getType() const;

	using XdmfDomain::insert;
	using XdmfGrid::insert;

	/**
	 * Set the XdmfGridCollectionType associated with this grid collection.
	 *
	 * @param collectionType the XdmfGridCollectionType to set.
	 */
	void setType(const boost::shared_ptr<const XdmfGridCollectionType> collectionType);

	void traverse(const boost::shared_ptr<XdmfBaseVisitor> visitor);

protected:

	XdmfGridCollection();
	virtual void populateItem(const std::map<std::string, std::string> & itemProperties, std::vector<boost::shared_ptr<XdmfItem> > & childItems);

private:

	XdmfGridCollection(const XdmfGridCollection & collection);  // Not implemented.
	void operator=(const XdmfGridCollection & collection);  // Not implemented.

	boost::shared_ptr<const XdmfGridCollectionType> mCollectionType;
};

#endif /* XDMFGRID_HPP_ */
