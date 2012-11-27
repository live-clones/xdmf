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
   * @dontinclude ExampleXdmfRegularGrid.cpp
   * @skipline newBrickX
   * @until newOriginY
   * @skipline XdmfRegularGrid
   * @until ;
   *
   * Python
   *
   * @dontinclude XdmfExampleRegularGrid.py
   * @skipline newBrickX
   * @until newOriginY
   * @skipline XdmfRegularGrid
   * @until )
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
   * @dontinclude ExampleXdmfRegularGrid.cpp
   * @skipline newBrickX
   * @until newOriginY
   * @skipline XdmfRegularGrid
   * @skipline newBrickZ
   *
   * Python
   *
   * @dontinclude XdmfExampleRegularGrid.py
   * @skipline newBrickX
   * @until newOriginY
   * @skipline XdmfRegularGrid
   * @skipline newBrickZ
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
   * @dontinclude ExampleXdmfRegularGrid.cpp
   * @skipline newBrickSize
   * @until XdmfRegularGrid
   *
   * Python
   *
   * @dontinclude XdmfExampleRegularGrid.py
   * @skipline newBrickSize
   * @until XdmfRegularGrid
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
   * @dontinclude ExampleXdmfRegularGrid.cpp
   * Assuming exampleGrid is a shared pointer to an XdmfRegularGrid object
   * @skipline exampleBrick
   *
   * Python
   *
   * @dontinclude XdmfExampleRegularGrid.py
   * Assuming exampleGrid is a shared pointer to an XdmfRegularGrid object
   * @skipline getBrickSize
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
   * @dontinclude ExampleXdmfRegularGrid.cpp
   * Assuming exampleGrid is a shared pointer to an XdmfRegularGrid object
   * @skipline exampleBrickConst
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
   * @dontinclude ExampleXdmfRegularGrid.cpp
   * Assuming exampleGrid is a shared pointer to an XdmfRegularGrid object
   * @skipline exampleDimensions
   *
   * Python
   *
   * @dontinclude XdmfExampleRegularGrid.py
   * Assuming exampleGrid is a shared pointer to an XdmfRegularGrid object
   * @skipline getDimensions
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
   * @dontinclude ExampleXdmfRegularGrid.cpp
   * Assuming exampleGrid is a shared pointer to an XdmfRegularGrid object
   * @skipline exampleDimensions
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
   * @dontinclude ExampleXdmfRegularGrid.cpp
   * Assuming exampleGrid is a shared pointer to an XdmfRegularGrid object
   * @skipline exampleOrigin
   *
   * Python
   *
   * @dontinclude XdmfExampleRegularGrid.py
   * Assuming exampleGrid is a shared pointer to an XdmfRegularGrid object
   * @skipline getOrigin
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
   * @dontinclude ExampleXdmfRegularGrid.cpp
   * Assuming exampleGrid is a shared pointer to an XdmfRegularGrid object
   * @skipline exampleOriginConst
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
   * @dontinclude ExampleXdmfRegularGrid.cpp
   * Assuming exampleGrid is a shared pointer to an XdmfRegularGrid object
   * @skipline newBrickSize
   * @until newBrickSize->pushBack(22.0)
   * @skipline setBrickSize
   *
   * Python
   *
   * @dontinclude XdmfExampleRegularGrid.py
   * Assuming exampleGrid is a shared pointer to an XdmfRegularGrid object
   * @skipline newBrickSize
   * @until newBrickSize.pushBackAsFloat64(22.0)
   * @skipline setBrickSize
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
   * @dontinclude ExampleXdmfRegularGrid.cpp
   * Assuming exampleGrid is a shared pointer to an XdmfRegularGrid object
   * @skipline newNumPoints
   * @until newNumPoints->pushBack(7)
   * @skipline setDimensions
   *
   * Python
   *
   * @dontinclude XdmfExampleRegularGrid.py
   * Assuming exampleGrid is a shared pointer to an XdmfRegularGrid object
   * @skipline newNumPoints
   * @until newNumPoints.pushBackAsUInt32(7)
   * @skipline setDimensions
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
   * @dontinclude ExampleXdmfRegularGrid.cpp
   * Assuming exampleGrid is a shared pointer to an XdmfRegularGrid object
   * @skipline newGridOrigin
   * @until newGridOrigin->pushBack(2.0)
   * @skipline setOrigin
   *
   * Python
   *
   * @dontinclude XdmfExampleRegularGrid.py
   * Assuming exampleGrid is a shared pointer to an XdmfRegularGrid object
   * @skipline newGridOrigin
   * @until newGridOrigin.pushBackAsFloat64(2.0)
   * @skipline setOrigin
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
