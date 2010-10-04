#ifndef XDMFGRIDRECTILINEAR_HPP_
#define XDMFGRIDRECTILINEAR_HPP_

// Includes
#include "XdmfGrid.hpp"

/**
 * @brief A mesh consisting of cells and points arranged on a regular lattice in space.
 *
 * XdmfGridRectilinear represents a mesh of cells and point arranged on a regular lattice in space.
 * Points are arranged along coordinate axes, but the spacing between points may vary.
 *
 * In order to define a rectilinear grid, the coordinates along each axis direction
 * must be specified.
 *
 */
class XdmfGridRectilinear : public XdmfGrid {

public:

	/**
	 * Create a new rectilinear grid (Two dimensional).
	 *
	 * @param xCoordinates the coordinates of points along the x axis
	 * @param yCoordinates the coordinates of points along the y axis.
	 *
	 * @return constructed rectilinear grid.
	 */
	static boost::shared_ptr<XdmfGridRectilinear> New(const boost::shared_ptr<XdmfArray> xCoordinates, const boost::shared_ptr<XdmfArray> yCoordinates);

	/**
	 * Create a new rectilinear grid (Three dimensional).
	 *
	 * @param xCoordinates the coordinates of points along the x axis
	 * @param yCoordinates the coordinates of points along the y axis.
	 * @param zCoordinates the coordinates of points along the z axis.
	 *
	 * @return constructed rectilinear grid.
	 */
	static boost::shared_ptr<XdmfGridRectilinear> New(const boost::shared_ptr<XdmfArray> xCoordinates, const boost::shared_ptr<XdmfArray> yCoordinates,
			const boost::shared_ptr<XdmfArray> zCoordinates);

	/**
	 * Create a new rectilinear grid (N dimensional).
	 *
	 * @param axesCoordinates the coordinates of points along each axis.
	 *
	 * @return constructed rectilinear grid.
	 */
	static boost::shared_ptr<XdmfGridRectilinear> New(const std::vector<boost::shared_ptr<XdmfArray> > & axesCoordinates);

	virtual ~XdmfGridRectilinear();

	LOKI_DEFINE_VISITABLE(XdmfGridRectilinear, XdmfGrid)
	static const std::string ItemTag;

	/**
	 * Get the coordinates of the grid along a single axis.
	 *
	 * @param axisIndex the index of the axis to retrieve, (i.e. 0 for x-axis). If no array exists at the index, return NULL.
	 *
	 * @return array of coordinates along
	 */
	boost::shared_ptr<XdmfArray> getCoordinates(const unsigned int axisIndex);

	/**
	 * Get the coordinates of the grid along a single axis (const version).
	 *
	 * @param axisIndex the index of the axis to retrieve (i.e. 0 for x-axis). If no array exists at the index, return NULL.
	 *
	 * @return array of coordinates along
	 */
	boost::shared_ptr<const XdmfArray> getCoordinates(const unsigned int axisIndex) const;

	/**
	 * Get the coordinates of the grid along all axes.
	 *
	 * @return vector containing an array of coordinates along each direction.
	 */
	std::vector<boost::shared_ptr<XdmfArray> > getCoordinates();

	/**
	 * Get the coordinates of the grid along all axes (const version).
	 *
	 * @return vector containing an array of coordinates along each direction.
	 */
	const std::vector<boost::shared_ptr<XdmfArray> > getCoordinates() const;

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
	 * Set the coordinates of the grid along a single axis.
	 *
	 * @param axisIndex the index of the axis to set (i.e. 0 for x-axis).
	 * @param axisCoordinates the coordinates of points along a single axis to set.
	 */
	void setCoordinates(const unsigned int axisIndex, const boost::shared_ptr<XdmfArray> axisCoordinates);

	/**
	 * Set the coordinates of the grid along all axes.
	 *
	 * @param axesCoordinates the coordinates of points along each axis.
	 */
	void setCoordinates(const std::vector<boost::shared_ptr<XdmfArray> > axesCoordinates);

protected:

	XdmfGridRectilinear(const std::vector<boost::shared_ptr<XdmfArray> > & axesCoordinates);

	void populateItem(const std::map<std::string, std::string> & itemProperties, std::vector<boost::shared_ptr<XdmfItem> > & childItems, const XdmfCoreReader * const reader);

private:

	/**
	 * PIMPL
	 */
	class XdmfGridRectilinearImpl;

	XdmfGridRectilinear(const XdmfGridRectilinear & grid);  // Not implemented.
	void operator=(const XdmfGridRectilinear & grid);  // Not implemented.

	XdmfGridRectilinearImpl * mImpl;

};

#endif /* XDMFGRIDRECTILINEAR_HPP_ */
