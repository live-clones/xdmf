#ifndef XDMFGRIDREGULAR_HPP_
#define XDMFGRIDREGULAR_HPP_

// Includes
#include "XdmfGrid.hpp"

/**
 * @brief A mesh consisting of congruent points arranged regularly in space.
 *
 * XdmfGridRegular represents a regular mesh of congruent points arranged in space.
 * In order to define a regular grid, three sets of terms need to be supplied:
 *
 * Brick Size (Dx, Dy, (Dz)) - Size of an individual brick.
 * Dimensions (X, Y, (Z)) - Number of points in X, Y, and Z directions
 * Origin Location (X, Y, (Z)) - Location of the origin of the mesh in space.
 */
class XdmfGridRegular : public XdmfGrid {

public:

	/**
	 * Create a new structured grid (Two dimensional).
	 *
	 * @param brickSizeX the size of the brick in the x direction.
	 * @param brickSizeY the size of the brick in the y direction.
	 * @param numPointsX the number of points in the x direction.
	 * @param numPointsY the number of points in the y direction.
	 * @param originX the x coordinate of the origin.
	 * @param originY the y coordinate of the origin.
	 *
	 * @return constructed structured grid.
	 */
	static boost::shared_ptr<XdmfGridRegular> New(const double brickSizeX, const double brickSizeY, const unsigned int numPointsX,
		const unsigned int numPointsY, const double originX, const double originY);

	/**
	 * Create a new structured grid (Three dimensional).
	 *
	 * @param brickSizeX the size of the brick in the x direction.
	 * @param brickSizeY the size of the brick in the y direction.
	 * @param brickSizeZ the size of the brick in the z direction.
	 * @param numPointsX the number of points in the x direction.
	 * @param numPointsY the number of points in the y direction.
	 * @param numPointsZ the number of points in the z direction.
	 * @param originX the x coordinate of the origin.
	 * @param originY the y coordinate of the origin.
	 * @param originZ the z coordinate of the origin.
	 *
	 * @return constructed structured grid.
	 */
	static boost::shared_ptr<XdmfGridRegular> New(const double brickSizeX, const double brickSizeY, const double brickSizeZ,
		const unsigned int numPointsX, const unsigned int numPointsY, const unsigned int numPointsZ,
		const double originX, const double originY, const double originZ);

	/**
	 * Create a new structured grid (N dimensional).
	 *
	 * @param brickSize the size of the brick in each direction.
	 * @param numPoints the number of points in each direction.
	 * @param origin the coordinates of the origin.
	 *
	 * @return constructed structured grid.
	 */
	static boost::shared_ptr<XdmfGridRegular> New(const boost::shared_ptr<XdmfArray> brickSize, const boost::shared_ptr<XdmfArray> numPoints,
		const boost::shared_ptr<XdmfArray> origin);

	virtual ~XdmfGridRegular();

	LOKI_DEFINE_VISITABLE(XdmfGridRegular, XdmfGrid)
	static const std::string ItemTag;

	/**
	 * Get the size of the points composing the grid.
	 *
	 * @return XdmfArray containing brick sizes for this grid.
	 */
	boost::shared_ptr<XdmfArray> getBrickSize();

	/**
	 * Get the size of the points composing the grid (const version).
	 *
	 * @return XdmfArray containing brick sizes for this grid.
	 */
	boost::shared_ptr<const XdmfArray> getBrickSize() const;

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
	 * Get the location of the origin of the grid.
	 *
	 * @return XdmfArray containing the location of the origin of the grid.
	 */
	boost::shared_ptr<XdmfArray> getOrigin();

	/**
	 * Get the location of the origin of the grid.
	 *
	 * @return XdmfArray containing the location of the origin of the grid (const version).
	 */
	boost::shared_ptr<const XdmfArray> getOrigin() const;

	/**
	 * Set the size of the points composing the grid.
	 *
	 * @param brickSize the sizes of the points composing the mesh.  This should
	 * have the same number of terms as the dimensionality of the mesh.
	 */
	void setBrickSize(const boost::shared_ptr<XdmfArray> brickSize);

	/**
	 * Set the dimensions of the grid, the number of points in each direction.
	 *
	 * @param dimensions the dimension of the grid.
	 */
	void setDimensions(const boost::shared_ptr<XdmfArray> dimensions);

	/**
	 * Set the origin of the grid.
	 *
	 * @param origin location of the origin of the grid.  This should have
	 * the same number of terms as the dimensionality of the mesh.
	 */
	void setOrigin(const boost::shared_ptr<XdmfArray> origin);

protected:

	XdmfGridRegular(const boost::shared_ptr<XdmfArray> brickSize, const boost::shared_ptr<XdmfArray> numPoints,
		const boost::shared_ptr<XdmfArray> origin);

	void populateItem(const std::map<std::string, std::string> & itemProperties, std::vector<boost::shared_ptr<XdmfItem> > & childItems, const XdmfCoreReader * const reader);

private:

	/**
	 * PIMPL
	 */
	class XdmfGridRegularImpl;

	XdmfGridRegular(const XdmfGridRegular & grid);  // Not implemented.
	void operator=(const XdmfGridRegular & grid);  // Not implemented.

	XdmfGridRegularImpl * mImpl;

};

#endif /* XDMFGRIDREGULAR_HPP_ */
