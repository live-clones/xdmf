#ifndef XDMFGRIDCOLLECTION_HPP_
#define XDMFGRIDCOLLECTION_HPP_

// Forward Declarations
class XdmfGridCollectionType;

// Includes
#include "XdmfDomain.hpp"

/**
 * @brief A spatial or temporal collection of XdmfGrids.
 *
 * A temporal collection is timestep data.  Each child grid represents the state at a single timestep.
 * A spatial collection consists of XdmfGrids that are arranged together in space.  E.g. a partitioned mesh.
 *
 * It is valid to nest collections.  A spatial collection within a temporal collection is commonly used.
 */
class XdmfGridCollection : public XdmfDomain {

public:

	/**
	 * Create a new XdmfGridCollection.
	 *
	 * @return constructed XdmfGridCollection.
	 */
	static boost::shared_ptr<XdmfGridCollection> New();

	virtual ~XdmfGridCollection();

	LOKI_DEFINE_VISITABLE(XdmfGridCollection, XdmfDomain)
	static const std::string ItemTag;

	std::map<std::string, std::string> getItemProperties() const;

	std::string getItemTag() const;

	/**
	 * Get the name of the grid collection.
	 *
	 * @return the name of the grid collection.
	 */
	std::string getName() const;

	/**
	 * Get the XdmfGridCollectionType associated with this grid collection.
	 *
	 * @return XdmfGridCollectionType of this collection.
	 */
	boost::shared_ptr<const XdmfGridCollectionType> getType() const;

	/**
	 * Set the name of the grid collection.
	 *
	 * @param name of the grid collection to set.
	 */
	void setName(const std::string & name);

	/**
	 * Set the XdmfGridCollectionType associated with this grid collection.
	 *
	 * @param collectionType the XdmfGridCollectionType to set.
	 */
	void setType(const boost::shared_ptr<const XdmfGridCollectionType> collectionType);

protected:

	XdmfGridCollection();
	virtual void populateItem(const std::map<std::string, std::string> & itemProperties, std::vector<boost::shared_ptr<XdmfItem> > & childItems, const XdmfCoreReader * const reader);

private:

	XdmfGridCollection(const XdmfGridCollection & collection);  // Not implemented.
	void operator=(const XdmfGridCollection & collection);  // Not implemented.

	boost::shared_ptr<const XdmfGridCollectionType> mCollectionType;
	std::string mName;
};

#endif /* XDMFGRID_HPP_ */
