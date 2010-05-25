#ifndef XDMFGRIDCOLLECTION_HPP_
#define XDMFGRIDCOLLECTION_HPP_

// Includes
#include "XdmfGrid.hpp"
#include "XdmfGridCollectionType.hpp"

/**
 * @brief A spatial or temporal collection of XdmfGrids.
 *
 * A temporal collection is timestep data.  Each child grid represents the state at a single timestep.
 * A spatial collection consists of XdmfGrids that are arranged together in space.  E.g. a partitioned mesh.
 *
 * It is valid to nest collections.  A spatial collection within a temporal collection is commonly used.
 */
class XdmfGridCollection : public XdmfGrid {

public:

	XdmfNewMacro(XdmfGridCollection);
	LOKI_DEFINE_VISITABLE(XdmfGridCollection, XdmfGrid)
	static std::string ItemTag;

	/**
	 * Get a grid from this collection.
	 *
	 * @param index of grid to retrieve.
	 * @return the requested XdmfGrid.
	 */
	boost::shared_ptr<XdmfGrid> getGrid(unsigned int index);

	/**
	 * Get a grid from this collection (const version).
	 *
	 * @param index of the grid to retrieve.
	 * @return the requested XdmfGrid.
	 */
	boost::shared_ptr<const XdmfGrid> getGrid(unsigned int index) const;

	/**
	 * Get the XdmfGridCollectionType associated with this grid collection.
	 *
	 * @return XdmfGridCollectionType of this collection.
	 */
	XdmfGridCollectionType getGridCollectionType() const;

	std::map<std::string, std::string> getItemProperties() const;

	/**
	 * Get the number of grids in this collection.
	 *
	 * @return unsigned int containing the number of XdmfGrids in this collection.
	 */
	unsigned int getNumberOfGrids() const;

	/**
	 * Insert a grid into this collection.
	 *
	 * @param grid an XdmfGrid to insert into this collection.
	 */
	void insert(boost::shared_ptr<XdmfGrid> grid);

	/**
	 * Remove a grid from this collection.
	 *
	 * @param index of the XdmfGrid to remove.
	 */
	void removeGrid(const unsigned int index);

	/**
	 * Set the XdmfGridCollectionType associated with this grid collection.
	 *
	 * @param collectionType the XdmfGridCollectionType to set.
	 */
	void setGridCollectionType(const XdmfGridCollectionType & collectionType);

	virtual void traverse(boost::shared_ptr<Loki::BaseVisitor> visitor);

protected:

	XdmfGridCollection();
	virtual ~XdmfGridCollection();
	virtual void populateItem(const std::map<std::string, std::string> & itemProperties, std::vector<boost::shared_ptr<XdmfItem> > & childItems);

private:

	XdmfGridCollection(const XdmfGridCollection & collection);  // Not implemented.
	void operator=(const XdmfGridCollection & collection);  // Not implemented.

	XdmfGridCollectionType mCollectionType;
	std::vector<boost::shared_ptr<XdmfGrid> > mGrids;
};

#endif /* XDMFGRID_HPP_ */
