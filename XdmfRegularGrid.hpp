#ifndef XDMFREGULARGRID_HPP_
#define XDMFREGULARGRID_HPP_

// Forward Declarations
class XdmfArray;

// Includes
#include "Xdmf.hpp"
#include "XdmfGrid.hpp"

/**
 * @brief A regular grid consists of congruent points arranged
 * regularly in space.
 *
 * XdmfRegularGrid represents a regular mesh of congruent points
 * arranged in space. In order to define a regular grid, three sets of
 * terms need to be supplied:
 *
 * Brick Size (Dx, Dy, (Dz)) - Size of an individual brick.
 * Dimensions (X, Y, (Z)) - Number of points in X, Y, and Z directions
 * Origin Location (X, Y, (Z)) - Location of the origin of the mesh in space.
 */
class XDMF_EXPORT XdmfRegularGrid : public XdmfGrid {

 public:

  /**
   * Create a new structured grid (Two dimensional).
   *
   * @param xBrickSize the size of the brick in the x direction.
   * @param yBrickSize the size of the brick in the y direction.
   * @param xNumPoints the number of points in the x direction.
   * @param yNumPoints the number of points in the y direction.
   * @param xOrigin the x coordinate of the origin.
   * @param yOrigin the y coordinate of the origin.
   *
   * @return constructed structured grid.
   */
  static boost::shared_ptr<XdmfRegularGrid> New(const double xBrickSize,
                                                const double yBrickSize,
                                                const unsigned int xNumPoints,
                                                const unsigned int yNumPoints,
                                                const double xOrigin,
                                                const double yOrigin);

  /**
   * Create a new structured grid (Three dimensional).
   *
   * @param xBrickSize the size of the brick in the x direction.
   * @param yBrickSize the size of the brick in the y direction.
   * @param zBrickSize the size of the brick in the z direction.
   * @param xNumPoints the number of points in the x direction.
   * @param yNumPoints the number of points in the y direction.
   * @param zNumPoints the number of points in the z direction.
   * @param xOrigin the x coordinate of the origin.
   * @param yOrigin the y coordinate of the origin.
   * @param zOrigin the z coordinate of the origin.
   *
   * @return constructed structured grid.
   */
  static boost::shared_ptr<XdmfRegularGrid> New(const double xBrickSize,
                                                const double yBrickSize,
                                                const double zBrickSize,
                                                const unsigned int xNumPoints,
                                                const unsigned int yNumPoints,
                                                const unsigned int zNumPoints,
                                                const double xOrigin,
                                                const double yOrigin,
                                                const double zOrigin);

  /**
   * Create a new structured grid (N dimensional).
   *
   * @param brickSize the size of the brick in each direction.
   * @param numPoints the number of points in each direction.
   * @param origin the coordinates of the origin.
   *
   * @return constructed structured grid.
   */
  static boost::shared_ptr<XdmfRegularGrid>
  New(const boost::shared_ptr<XdmfArray> brickSize,
      const boost::shared_ptr<XdmfArray> numPoints,
      const boost::shared_ptr<XdmfArray> origin);

  virtual ~XdmfRegularGrid();

  LOKI_DEFINE_VISITABLE(XdmfRegularGrid, XdmfGrid);
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
   * Get the dimensions of the grid, the number of points in each
   * direction.
   *
   * @return XdmfArray containing dimensions of this grid.
   */
  boost::shared_ptr<XdmfArray> getDimensions();

  /**
   * Get the dimensions of the grid, the number of points in each
   * direction (const version).
   *
   * @return XdmfArray containing the dimensions of this grid.
   */
  boost::shared_ptr<const XdmfArray> getDimensions() const;

  /**
   * Get the location of the origin of the grid.
   *
   * @return XdmfArray containing the location of the origin of the
   * grid.
   */
  boost::shared_ptr<XdmfArray> getOrigin();

  /**
   * Get the location of the origin of the grid.
   *
   * @return XdmfArray containing the location of the origin of the
   * grid (const version).
   */
  boost::shared_ptr<const XdmfArray> getOrigin() const;

  /**
   * Set the size of the points composing the grid.
   *
   * @param brickSize the sizes of the points composing the mesh. This
   * should have the same number of terms as the dimensionality of the
   * mesh.
   */
  void setBrickSize(const boost::shared_ptr<XdmfArray> brickSize);

  /**
   * Set the dimensions of the grid, the number of points in each
   * direction.
   *
   * @param dimensions the dimension of the grid.
   */
  void setDimensions(const boost::shared_ptr<XdmfArray> dimensions);

  /**
   * Set the origin of the grid.
   *
   * @param origin location of the origin of the grid.  This should
   * have the same number of terms as the dimensionality of the mesh.
   */
  void setOrigin(const boost::shared_ptr<XdmfArray> origin);

 protected:

  XdmfRegularGrid(const boost::shared_ptr<XdmfArray> brickSize,
                  const boost::shared_ptr<XdmfArray> numPoints,
                  const boost::shared_ptr<XdmfArray> origin);

  void populateItem(const std::map<std::string, std::string> & itemProperties,
                    std::vector<boost::shared_ptr<XdmfItem> > & childItems,
                    const XdmfCoreReader * const reader);

 private:

  /**
   * PIMPL
   */
  class XdmfRegularGridImpl;

  XdmfRegularGrid(const XdmfRegularGrid &);  // Not implemented.
  void operator=(const XdmfRegularGrid &);  // Not implemented.

  XdmfRegularGridImpl * mImpl;

};

#ifdef _WIN32
XDMF_TEMPLATE template class XDMF_EXPORT
boost::shared_ptr<XdmfArray>;
XDMF_TEMPLATE template class XDMF_EXPORT
boost::shared_ptr<const XdmfArray>;
#endif

#endif /* XDMFREGULARGRID_HPP_ */
