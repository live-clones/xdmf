/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfRegularGrid.hpp                                                 */
/*                                                                           */
/*  Author:                                                                  */
/*     Kenneth Leiter                                                        */
/*     kenneth.leiter@arl.army.mil                                           */
/*     US Army Research Laboratory                                           */
/*     Aberdeen Proving Ground, MD                                           */
/*                                                                           */
/*     Copyright @ 2011 US Army Research Laboratory                          */
/*     All Rights Reserved                                                   */
/*     See Copyright.txt for details                                         */
/*                                                                           */
/*     This software is distributed WITHOUT ANY WARRANTY; without            */
/*     even the implied warranty of MERCHANTABILITY or FITNESS               */
/*     FOR A PARTICULAR PURPOSE.  See the above copyright notice             */
/*     for more information.                                                 */
/*                                                                           */
/*****************************************************************************/

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
   * Example of use:
   *
   * C++
   *
   * double newBrickX = 20.0;
   * double newBrickY = 20.0;
   * unsigned int newPointsX = 5;
   * unsigned int newPointsY = 5;
   * double newOriginX = 0;
   * double newOriginY = 0;
   * shared_ptr<XdmfRegularGrid> exampleGrid = XdmfRegularGrid::New(
   *   newBrickX, newBrickY, newPointsX, newPointsY, newOriginX, newOriginY);
   *
   * Python
   *
   * newBrickX = 20.0
   * newBrickY = 20.0
   * newPointsX = 5
   * newPointsY = 5
   * newOriginX = 0.0
   * newOriginY = 0.0
   * exampleGrid = XdmfRegularGrid.New(
   *   newBrickX, newBrickY, newPointsX, newPointsY, newOriginX, newOriginY)
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
  static shared_ptr<XdmfRegularGrid> New(const double xBrickSize,
                                         const double yBrickSize,
                                         const unsigned int xNumPoints,
                                         const unsigned int yNumPoints,
                                         const double xOrigin,
                                         const double yOrigin);

  /**
   * Create a new structured grid (Three dimensional).
   *
   * Example of use:
   *
   * C++
   *
   * double newBrickX = 20.0;
   * double newBrickY = 20.0;
   * double newBrickZ = 20.0;
   * unsigned int newPointsX = 5;
   * unsigned int newPointsY = 5;
   * unsigned int newPointsZ = 5;
   * double newOriginX = 0;
   * double newOriginY = 0;
   * double newOriginZ = 0;
   * shared_ptr<XdmfRegularGrid> exampleGrid = XdmfRegularGrid::New(
   *   newBrickX, newBrickY, newBrickZ, newPointsX, newPointsY, newPointsZ, newOriginX, newOriginY, newOriginZ);
   *
   * Python
   *
   * newBrickX = 20.0
   * newBrickY = 20.0
   * newBrickZ = 20.0
   * newPointsX = 5
   * newPointsY = 5
   * newPointsZ = 5
   * newOriginX = 0.0
   * newOriginY = 0.0
   * newOriginZ = 0.0
   * exampleGrid = XdmfRegularGrid::New(
   *   newBrickX, newBrickY, newBrickZ, newPointsX, newPointsY, newPointsZ, newOriginX, newOriginY, newOriginZ)
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
  static shared_ptr<XdmfRegularGrid> New(const double xBrickSize,
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
   * Example of use:
   *
   * C++
   *
   * shared_ptr<XdmfArray> newBrickSize = XdmfArray::New();
   * newBrickSize->pushBack(20.0);
   * newBrickSize->pushBack(20.0);
   * newBrickSize->pushBack(20.0);
   * shared_ptr<XdmfArray> newNumPoints = XdmfArray::New();
   * newNumPoints->pushBack(5);
   * newNumPoints->pushBack(5);
   * newNumPoints->pushBack(5);
   * shared_ptr<XdmfArray> newOrigin = XdmfArray::New();
   * newOrigin->pushBack(0.0);
   * newOrigin->pushBack(0.0);
   * newOrigin->pushBack(0.0);
   * shared_ptr<XdmfRegularGrid> exampleGrid = XdmfRegularGrid::New(newBrickSize, newNumPoints, newOrigin);
   *
   * Python
   *
   * newBrickSize = XdmfArray.New()
   * newBrickSize.pushBackAsFloat64(20.0)
   * newBrickSize.pushBackAsFloat64(20.0)
   * newBrickSize.pushBackAsFloat64(20.0)
   * newNumPoints = XdmfArray.New()
   * newNumPoints.pushBackAsUInt32(5)
   * newNumPoints.pushBackAsUInt32(5)
   * newNumPoints.pushBackAsUInt32(5)
   * newOrigin = XdmfArray.New()
   * newOrigin.pushBackAsFloat64(0.0)
   * newOrigin.pushBackAsFloat64(0.0)
   * newOrigin.pushBackAsFloat64(0.0)
   * exampleGrid = XdmfRegularGrid.New(newBrickSize, newNumPoints, newOrigin)
   *
   * @param brickSize the size of the brick in each direction.
   * @param numPoints the number of points in each direction.
   * @param origin the coordinates of the origin.
   *
   * @return constructed structured grid.
   */
  static shared_ptr<XdmfRegularGrid>
  New(const shared_ptr<XdmfArray> brickSize,
      const shared_ptr<XdmfArray> numPoints,
      const shared_ptr<XdmfArray> origin);

  virtual ~XdmfRegularGrid();

  LOKI_DEFINE_VISITABLE(XdmfRegularGrid, XdmfGrid);
  static const std::string ItemTag;

  /**
   * Get the size of the bricks composing the grid.
   *
   * Example of use:
   *
   * C++
   *
   * //Assuming exampleGrid is a shared pointer to an XdmfRegularGrid object
   * shared_ptr<XdmfArray> exampleBrick = exampleGrid->getBrickSize();
   *
   * Python
   *
   * '''
   * Assuming exampleGrid is a shared pointer to an XdmfRegularGrid object
   * '''
   * exampleBrick = exampleGrid.getBrickSize()
   *
   * @return XdmfArray containing brick sizes for this grid.
   */
  shared_ptr<XdmfArray> getBrickSize();

  /**
   * Get the size of the bricks composing the grid (const version).
   *
   * Example of use:
   *
   * C++
   *
   * //Assuming exampleGrid is a shared pointer to an XdmfRegularGrid object
   * shared_ptr<const XdmfArray> exampleBrick = exampleGrid->getBrickSize();
   *
   * Python: Does not support a constant version of this function
   *
   * @return XdmfArray containing brick sizes for this grid.
   */
  shared_ptr<const XdmfArray> getBrickSize() const;

  /**
   * Get the dimensions of the grid, the number of points in each
   * direction.
   *
   * Example of use:
   *
   * C++
   *
   * //Assuming exampleGrid is a shared pointer to an XdmfRegularGrid object
   * shared_ptr<XdmfArray> exampleDimensions = exampleGrid->getDimensions();
   *
   * Python
   *
   * '''
   * Assuming exampleGrid is a shared pointer to an XdmfRegularGrid object
   * '''
   * exampleDimensions = exampleGrid.getDimensions()
   *
   * @return XdmfArray containing dimensions of this grid.
   */
  shared_ptr<XdmfArray> getDimensions();

  /**
   * Get the dimensions of the grid, the number of points in each
   * direction (const version).
   *
   * Example of use:
   *
   * C++
   *
   * //Assuming exampleGrid is a shared pointer to an XdmfRegularGrid object
   * shared_ptr<const XdmfArray> exampleDimensions = exampleGrid->getDimensions();
   *
   * Python: Does not support a constant version of this function
   *
   * @return XdmfArray containing the dimensions of this grid.
   */
  shared_ptr<const XdmfArray> getDimensions() const;

  /**
   * Get the location of the origin of the grid.
   *
   * Example of use:
   *
   * C++
   *
   * //Assuming exampleGrid is a shared pointer to an XdmfRegularGrid object
   * shared_ptr<XdmfArray> exampleOrigin = exampleGrid->getOrigin();
   *
   * Python
   *
   * '''
   * Assuming exampleGrid is a shared pointer to an XdmfRegularGrid object
   * '''
   * exampleOrigin = exampleGrid.getOrigin()
   *
   * @return XdmfArray containing the location of the origin of the
   * grid.
   */
  shared_ptr<XdmfArray> getOrigin();

  /**
   * Get the location of the origin of the grid.
   *
   * Example of use:
   *
   * C++
   *
   * //Assuming exampleGrid is a shared pointer to an XdmfRegularGrid object
   * shared_ptr<const XdmfArray> exampleOrigin = exampleGrid->getOrigin();
   *
   * Python: Does not support a constant version of this function
   *
   * @return XdmfArray containing the location of the origin of the
   * grid (const version).
   */
  shared_ptr<const XdmfArray> getOrigin() const;

  /**
   * Set the size of the points composing the grid.
   *
   * Example of use:
   *
   * C++
   *
   * //Assuming exampleGrid is a shared pointer to an XdmfRegularGrid object
   * shared_ptr<XdmfArray> newBrickSize = XdmfArray::New();
   * newBrickSize->pushBack(20.0);
   * newBrickSize->pushBack(20.0);
   * newBrickSize->pushBack(20.0);
   * exampleGrid->setBrickSize(newBrickSize);
   *
   * Python
   *
   * '''
   * Assuming exampleGrid is a shared pointer to an XdmfRegularGrid object
   * '''
   * newBrickSize = XdmfArray.New()
   * newBrickSize.pushBackAsFloat64(20.0)
   * newBrickSize.pushBackAsFloat64(20.0)
   * newBrickSize.pushBackAsFloat64(20.0)
   * exampleGrid.setBrickSize(newBrickSize)
   *
   * @param brickSize the sizes of the points composing the mesh. This
   * should have the same number of terms as the dimensionality of the
   * mesh.
   */
  void setBrickSize(const shared_ptr<XdmfArray> brickSize);

  /**
   * Set the dimensions of the grid, the number of points in each
   * direction.
   *
   * Example of use:
   *
   * C++
   *
   * //Assuming exampleGrid is a shared pointer to an XdmfRegularGrid object
   * shared_ptr<XdmfArray> newNumPoints = XdmfArray::New();
   * newNumPoints->pushBack(5);
   * newNumPoints->pushBack(5);
   * newNumPoints->pushBack(5);
   * exampleGrid->setDimensions(newNumPoints);
   *
   * Python
   *
   * '''
   * Assuming exampleGrid is a shared pointer to an XdmfRegularGrid object
   * '''
   * newNumPoints = XdmfArray.New()
   * newNumPoints.pushBackAsInt32(5)
   * newNumPoints.pushBackAsInt32(5)
   * newNumPoints.pushBackAsInt32(5)
   * exampleGrid.setDimensions(newNumPoints)
   *
   * @param dimensions the dimension of the grid.
   */
  void setDimensions(const shared_ptr<XdmfArray> dimensions);

  /**
   * Set the origin of the grid.
   *
   * Example of use:
   *
   * C++
   *
   * //Assuming exampleGrid is a shared pointer to an XdmfRegularGrid object
   * shared_ptr<XdmfArray> newOrigin = XdmfArray::New();
   * newOrigin->pushBack(0.0);
   * newOrigin->pushBack(0.0);
   * newOrigin->pushBack(0.0);
   * exampleGrid->setDimensions(newOrigin);
   *
   * Python
   *
   * '''
   * Assuming exampleGrid is a shared pointer to an XdmfRegularGrid object
   * '''
   * newOrigin = XdmfArray.New()
   * newOrigin.pushBackAsFloat64(0.0)
   * newOrigin.pushBackAsFloat64(0.0)
   * newOrigin.pushBackAsFloat64(0.0)
   * exampleGrid.setDimensions(newOrigin)
   *
   * @param origin location of the origin of the grid.  This should
   * have the same number of terms as the dimensionality of the mesh.
   */
  void setOrigin(const shared_ptr<XdmfArray> origin);

protected:

  XdmfRegularGrid(const shared_ptr<XdmfArray> brickSize,
                  const shared_ptr<XdmfArray> numPoints,
                  const shared_ptr<XdmfArray> origin);

  void populateItem(const std::map<std::string, std::string> & itemProperties,
                    const std::vector<shared_ptr<XdmfItem> > & childItems,
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
shared_ptr<XdmfArray>;
XDMF_TEMPLATE template class XDMF_EXPORT
shared_ptr<const XdmfArray>;
#endif

#endif /* XDMFREGULARGRID_HPP_ */
