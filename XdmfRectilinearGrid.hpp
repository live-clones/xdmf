/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfRectilinearGrid.hpp                                             */
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

#ifndef XDMFRECTILINEARGRID_HPP_
#define XDMFRECTILINEARGRID_HPP_

// Forward Declarations
class XdmfArray;

// Includes
#include "Xdmf.hpp"
#include "XdmfGrid.hpp"

/**
 * @brief A rectilinear grid consists of cells and points arranged on
 * a regular lattice in space.
 *
 * XdmfRectilinearGrid represents a mesh of cells and points arranged
 * on a regular lattice in space. Points are arranged along coordinate
 * axes, but the spacing between points may vary.
 *
 * In order to define a rectilinear grid, the coordinates along each
 * axis direction must be specified.
 *
 */
class XDMF_EXPORT XdmfRectilinearGrid : public XdmfGrid {

public:

  /**
   * Create a new rectilinear grid (Two dimensional).
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfRectilinearGrid.cpp
   * @skipline pointsXArray
   * @until pointsYArray->pushBack(10)
   * @skipline XdmfRectilinearGrid
   *
   * Python
   *
   * @dontinclude XdmfExampleRectilinearGrid.py
   * @skipline pointsXArray
   * @until XdmfRectilinearGrid
   *
   * @param xCoordinates the coordinates of points along the x axis
   * @param yCoordinates the coordinates of points along the y axis.
   *
   * @return constructed rectilinear grid.
   */
  static shared_ptr<XdmfRectilinearGrid>
  New(const shared_ptr<XdmfArray> xCoordinates,
      const shared_ptr<XdmfArray> yCoordinates);

  /**
   * Create a new rectilinear grid (Three dimensional).
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfRectilinearGrid.cpp
   * @skipline pointsXArray
   * @until pointsZArray->pushBack(2)
   * @skip size
   * @skipline pointsZArray
   *
   * Python
   *
   * @dontinclude XdmfExampleRectilinearGrid.py
   * @skipline pointsXArray
   * @until pointsYArray.pushBackAsInt32(10)
   * @skipline pointsZArray
   * @until XdmfRectilinearGrid
   *
   * @param xCoordinates the coordinates of points along the x axis
   * @param yCoordinates the coordinates of points along the y axis.
   * @param zCoordinates the coordinates of points along the z axis.
   *
   * @return constructed rectilinear grid.
   */
  static shared_ptr<XdmfRectilinearGrid>
  New(const shared_ptr<XdmfArray> xCoordinates,
      const shared_ptr<XdmfArray> yCoordinates,
      const shared_ptr<XdmfArray> zCoordinates);

  /**
   * Create a new rectilinear grid (N dimensional).
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfRectilinearGrid.cpp
   * @skipline pointsXArray
   * @until pointsZArray->pushBack(2)
   * @skipline pointsCollector
   * @until XdmfRectilinearGrid
   *
   * Python
   *
   * @dontinclude XdmfExampleRectilinearGrid.py
   * @skipline pointsXArray
   * @until pointsYArray.pushBackAsInt32(10)
   * @skipline pointsZArray
   * @until pointsZArray.pushBackAsInt32(2)
   * @skipline pointsCollector
   * @until XdmfRectilinearGrid
   *
   * @param axesCoordinates the coordinates of points along each axis.
   *
   * @return constructed rectilinear grid.
   */
  static shared_ptr<XdmfRectilinearGrid>
  New(const std::vector<shared_ptr<XdmfArray> > & axesCoordinates);

  virtual ~XdmfRectilinearGrid();

  LOKI_DEFINE_VISITABLE(XdmfRectilinearGrid, XdmfGrid);
  static const std::string ItemTag;

  /**
   * Get the coordinates of the grid along a single axis.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfRectilinearGrid.cpp
   * @skipline Assuming
   * @skipline readPointsX
   * @until readPointsY
   *
   * Python
   *
   * @dontinclude XdmfExampleRectilinearGrid.py
   * @skipline Assuming
   * @skipline readPointsX
   * @until readPointsY
   *
   * @param axisIndex the index of the axis to retrieve, (i.e. 0 for
   * x-axis). If no array exists at the index, return NULL.
   *
   * @return array of coordinates along
   */
  shared_ptr<XdmfArray> getCoordinates(const unsigned int axisIndex);

  /**
   * Get the coordinates of the grid along a single axis (const
   * version).
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfRectilinearGrid.cpp
   * @skipline Assuming
   * @skipline readPointsXConst
   * @until readPointsYConst
   *
   * Python: does not support a constant version of this function
   *
   * @param axisIndex the index of the axis to retrieve (i.e. 0 for
   * x-axis). If no array exists at the index, return NULL.
   *
   * @return array of coordinates along
   */
  shared_ptr<const XdmfArray>
  getCoordinates(const unsigned int axisIndex) const;

  /**
   * Get the coordinates of the grid along all axes.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfRectilinearGrid.cpp
   * @skipline Assuming
   * @skipline exampleCoordinates
   *
   * Python
   *
   * @dontinclude XdmfExampleRectilinearGrid.py
   * @skipline Assuming
   * @skipline exampleCoordinates
   *
   * @return vector containing an array of coordinates along each
   * direction.
   */
  std::vector<shared_ptr<XdmfArray> > getCoordinates();

  /**
   * Get the coordinates of the grid along all axes (const version).
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfRectilinearGrid.cpp
   * @skipline Assuming
   * @skipline exampleCoordinatesConst
   *
   * Python: does not support a constant version of this function
   *
   * @return vector containing an array of coordinates along each
   * direction.
   */
  const std::vector<shared_ptr<XdmfArray> > getCoordinates() const;

  /**
   * Get the dimensions of the grid, the number of points in each
   * direction.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfRectilinearGrid.cpp
   * @skipline Assuming
   * @skipline exampleDimensions
   *
   * Python
   *
   * @dontinclude XdmfExampleRectilinearGrid.py
   * @skipline Assuming
   * @skipline exampleDimensions
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
   * @dontinclude ExampleXdmfRectilinearGrid.cpp
   * @skipline Assuming
   * @skipline exampleDimensionsConst
   *
   * Python: Doesn't support a constant version of this function
   *
   * @return XdmfArray containing the dimensions of this grid.
   */
  shared_ptr<const XdmfArray> getDimensions() const;

  /**
   * Set the coordinates of the grid along a single axis.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfRectilinearGrid.cpp
   * @skipline Assuming
   * @skipline pointsXArray
   * @until pointsXArray->pushBack(10)
   * @skipline setCoordinates
   *
   * Python
   *
   * @dontinclude XdmfExampleRectilinearGrid.py
   * @skipline Assuming
   * @skipline pointsXArray
   * @until pointsXArray.pushBackAsInt32(10)
   * @skipline setCoordinates
   *
   * @param axisIndex the index of the axis to set (i.e. 0 for x-axis).
   * @param axisCoordinates the coordinates of points along a single axis to
   * set.
   */
  void setCoordinates(const unsigned int axisIndex,
                      const shared_ptr<XdmfArray> axisCoordinates);

  /**
   * Set the coordinates of the grid along all axes.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfRectilinearGrid.cpp
   * @skipline Assuming
   * @skipline pointsXArray
   * @until pointsZArray->pushBack(2);
   * @skipline pointsCollector
   * @until pointsZArray
   * @skipline setCoordinates
   *
   * Python
   *
   * @dontinclude XdmfExampleRectilinearGrid.py
   * @skipline Assuming
   * @skipline pointsXArray
   * @until pointsYArray.pushBackAsInt32(10)
   * @skipline pointsZArray
   * @until pointsZArray.pushBackAsInt32(2)
   * @skipline ArrayVector
   * @until pointsZArray
   * @skipline setCoordinates
   *
   * @param axesCoordinates the coordinates of points along each axis.
   */
  void
  setCoordinates(const std::vector<shared_ptr<XdmfArray> > axesCoordinates);

protected:

  XdmfRectilinearGrid(const std::vector<shared_ptr<XdmfArray> > & axesCoordinates);

  void populateItem(const std::map<std::string, std::string> & itemProperties,
                    const std::vector<shared_ptr<XdmfItem> > & childItems,
                    const XdmfCoreReader * const reader);

private:

  /**
   * PIMPL
   */
  class XdmfRectilinearGridImpl;

  XdmfRectilinearGrid(const XdmfRectilinearGrid &);  // Not implemented.
  void operator=(const XdmfRectilinearGrid &);  // Not implemented.

  XdmfRectilinearGridImpl * mImpl;

};

#ifdef _WIN32
XDMF_TEMPLATE template class XDMF_EXPORT
shared_ptr<XdmfArray>;
XDMF_TEMPLATE template class XDMF_EXPORT
shared_ptr<const XdmfArray>;
#endif

#endif /* XDMFRECTILINEARGRID_HPP_ */
