#ifndef XDMFCURVILINEARGRID_HPP_
#define XDMFCURVILINEARGRID_HPP_

// Forward Declarations
class XdmfArray;

// Includes
#include "XdmfGrid.hpp"

/**
 * @brief A curvilinear (or structured) grid consisting of cells and points arranged on a regular lattice in space.
 *
 * XdmfCurvilinearGrid represents a mesh of cells and points arranged with regular topology in space.  A curvilinear grid
 * contains regular topology but irregular geometry.
 *
 * In order to define a curvilinear grid, the dimensions of the grid must be supplied and the coordinates of each point
 * must be specified.
 *
 */
class XdmfCurvilinearGrid : public XdmfGrid {

public:

	/**
	 * Create a new curvilinear grid (Two dimensional).
	 *
	 * @param xNumPoints the number of points in the x direction.
	 * @param yNumPoints the number of points in the y direction.
	 *
	 * @return constructed curvilinear grid.
	 */
	static boost::shared_ptr<XdmfCurvilinearGrid> New(const unsigned int xNumPoints, const unsigned int yNumPoints);

	/**
	 * Create a new curvilinear grid (Three dimensional).
	 *
	 * @param xNumPoints the number of points in the x direction.
	 * @param yNumPoints the number of points in the y direction.
	 * @param yNumPoints the number of points in the z direction.
	 *
	 * @return constructed curvilinear grid.
	 */
	static boost::shared_ptr<XdmfCurvilinearGrid> New(const unsigned int xNumPoints, const unsigned int yNumPoints,
		const unsigned int zNumPounts);

	/**
	 * Create a new curvilinear grid (N dimensional).
	 *
	 * @param numPoints the number of points in each direction.
	 *
	 * @return constructed curvilinear grid.
	 */
	static boost::shared_ptr<XdmfCurvilinearGrid> New(const boost::shared_ptr<XdmfArray> numPoints);

	virtual ~XdmfCurvilinearGrid();

	LOKI_DEFINE_VISITABLE(XdmfCurvilinearGrid, XdmfGrid)
	static const std::string ItemTag;

	/**
	 * Get the dimensions of the grid, the number of points in each direction.
	 *
	 * @return XdmfArray containing dimensions of this grid.
	 */
	boost::shared_ptr<XdmfArray> getDimensions();

	/**
	 * Get the dimensions of the grid, the number of points in each direction (const version).
	 *
	 * @return XdmfArray containing the dimensions of this grid.
	 */
	boost::shared_ptr<const XdmfArray> getDimensions() const;

	/**
	 * Get the geometry associated with this grid.
	 *
	 * @return the geometry associated with this grid.
	 */
	boost::shared_ptr<XdmfGeometry> getGeometry();

	/**
	 * Set the dimensions of the grid, the number of points in each direction.
	 *
	 * @param dimensions the dimension of the grid.
	 */
	void setDimensions(const boost::shared_ptr<XdmfArray> dimensions);

	/**
	 * Set the geometry associated with this grid.
	 *
	 * @param geometry an XdmfGeometry to associate with this grid.
	 */
	void setGeometry(const boost::shared_ptr<XdmfGeometry> geometry);

protected:

	XdmfCurvilinearGrid(const boost::shared_ptr<XdmfArray> numPoints);

	void populateItem(const std::map<std::string, std::string> & itemProperties, std::vector<boost::shared_ptr<XdmfItem> > & childItems, const XdmfCoreReader * const reader);

private:

	/**
	 * PIMPL
	 */
	class XdmfCurvilinearGridImpl;

	XdmfCurvilinearGrid(const XdmfCurvilinearGrid & grid);  // Not implemented.
	void operator=(const XdmfCurvilinearGrid & grid);  // Not implemented.

	XdmfCurvilinearGridImpl * mImpl;

};

#endif /* XDMFCURVILINEARGRID_HPP_ */
