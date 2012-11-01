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
   * shared_ptr<XdmfArray> pointsXArray = XdmfArray::New();
   * pointsXArray->pushBack(5);
   * pointsXArray->pushBack(6);
   * pointsXArray->pushBack(7);
   * pointsXArray->pushBack(8);
   * pointsXArray->pushBack(9);
   * pointsXArray->pushBack(10);
   * shared_ptr<XdmfArray> pointsYArray = XdmfArray::New();
   * pointsYArray->pushBack(3);
   * pointsYArray->pushBack(6);
   * pointsYArray->pushBack(4);
   * pointsYArray->pushBack(8);
   * pointsYArray->pushBack(7);
   * pointsYArray->pushBack(10);
   * shared_ptr<XdmfRectilinearGrid> exampleGrid = XdmfRectilinearGrid::New(pointsXArray, pointsYArray);
   *
   * Python
   *
   * pointsXArray = XdmfArray.New()
   * pointsXArray.pushBackAsInt32(5)
   * pointsXArray.pushBackAsInt32(6)
   * pointsXArray.pushBackAsInt32(7)
   * pointsXArray.pushBackAsInt32(8)
   * pointsXArray.pushBackAsInt32(9)
   * pointsXArray.pushBackAsInt32(10)
   * pointsYArray = XdmfArray.New()
   * pointsYArray.pushBackAsInt32(3)
   * pointsYArray.pushBackAsInt32(6)
   * pointsYArray.pushBackAsInt32(4)
   * pointsYArray.pushBackAsInt32(8)
   * pointsYArray.pushBackAsInt32(7)
   * pointsYArray.pushBackAsInt32(10)
   * exampleGrid = XdmfRectilinearGrid.New(pointsXArray, pointsYArray)
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
   * shared_ptr<XdmfArray> pointsXArray = XdmfArray::New();
   * pointsXArray->pushBack(5);
   * pointsXArray->pushBack(6);
   * pointsXArray->pushBack(7);
   * pointsXArray->pushBack(8);
   * pointsXArray->pushBack(9);
   * pointsXArray->pushBack(10);
   * shared_ptr<XdmfArray> pointsYArray = XdmfArray::New();
   * pointsYArray->pushBack(3);
   * pointsYArray->pushBack(6);
   * pointsYArray->pushBack(4);
   * pointsYArray->pushBack(8);
   * pointsYArray->pushBack(7);
   * pointsYArray->pushBack(10);
   * shared_ptr<XdmfArray> pointsZArray = XdmfArray::New();
   * pointsZArray->pushBack(3);
   * pointsZArray->pushBack(9);
   * pointsZArray->pushBack(4);
   * pointsZArray->pushBack(5);
   * pointsZArray->pushBack(7);
   * pointsZArray->pushBack(2);
   * shared_ptr<XdmfRectilinearGrid> exampleGrid = XdmfRectilinearGrid::New(pointsXArray, pointsYArray, pointsZArray);
   *
   * Python
   *
   * pointsXArray = XdmfArray.New()
   * pointsXArray.pushBackAsInt32(5)
   * pointsXArray.pushBackAsInt32(6)
   * pointsXArray.pushBackAsInt32(7)
   * pointsXArray.pushBackAsInt32(8)
   * pointsXArray.pushBackAsInt32(9)
   * pointsXArray.pushBackAsInt32(10)
   * pointsYArray = XdmfArray.New()
   * pointsYArray.pushBackAsInt32(3)
   * pointsYArray.pushBackAsInt32(6)
   * pointsYArray.pushBackAsInt32(4)
   * pointsYArray.pushBackAsInt32(8)
   * pointsYArray.pushBackAsInt32(7)
   * pointsYArray.pushBackAsInt32(10)
   * pointsZArray = XdmfArray.New()
   * pointsZArray.pushBackAsInt32(3)
   * pointsZArray.pushBackAsInt32(9)
   * pointsZArray.pushBackAsInt32(4)
   * pointsZArray.pushBackAsInt32(5)
   * pointsZArray.pushBackAsInt32(7)
   * pointsZArray.pushBackAsInt32(2)
   * exampleGrid = XdmfRectilinearGrid.New(pointsXArray, pointsYArray, pointsZArray)
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
   * shared_ptr<XdmfArray> pointsXArray = XdmfArray::New();
   * pointsXArray->pushBack(5);
   * pointsXArray->pushBack(6);
   * pointsXArray->pushBack(7);
   * pointsXArray->pushBack(8);
   * pointsXArray->pushBack(9);
   * pointsXArray->pushBack(10);
   * shared_ptr<XdmfArray> pointsYArray = XdmfArray::New();
   * pointsYArray->pushBack(3);
   * pointsYArray->pushBack(6);
   * pointsYArray->pushBack(4);
   * pointsYArray->pushBack(8);
   * pointsYArray->pushBack(7);
   * pointsYArray->pushBack(10);
   * shared_ptr<XdmfArray> pointsZArray = XdmfArray::New();
   * pointsZArray->pushBack(3);
   * pointsZArray->pushBack(9);
   * pointsZArray->pushBack(4);
   * pointsZArray->pushBack(5);
   * pointsZArray->pushBack(7);
   * pointsZArray->pushBack(2);
   * std::vector<shared_ptr<XdmfArray> > pointsCollector;
   * pointsCollector.push_back(pointsXArray);
   * pointsCollector.push_back(pointsYArray);
   * pointsCollector.push_back(pointsZArray);
   * shared_ptr<XdmfRectilinearGrid> exampleGrid = XdmfRectilinearGrid::New(pointsCollector);
   *
   * Python
   *
   * pointsXArray = XdmfArray.New()
   * pointsXArray.pushBackAsInt32(5)
   * pointsXArray.pushBackAsInt32(6)
   * pointsXArray.pushBackAsInt32(7)
   * pointsXArray.pushBackAsInt32(8)
   * pointsXArray.pushBackAsInt32(9)
   * pointsXArray.pushBackAsInt32(10)
   * pointsYArray = XdmfArray.New()
   * pointsYArray.pushBackAsInt32(3)
   * pointsYArray.pushBackAsInt32(6)
   * pointsYArray.pushBackAsInt32(4)
   * pointsYArray.pushBackAsInt32(8)
   * pointsYArray.pushBackAsInt32(7)
   * pointsYArray.pushBackAsInt32(10)
   * pointsZArray = XdmfArray.New()
   * pointsZArray.pushBackAsInt32(3)
   * pointsZArray.pushBackAsInt32(9)
   * pointsZArray.pushBackAsInt32(4)
   * pointsZArray.pushBackAsInt32(5)
   * pointsZArray.pushBackAsInt32(7)
   * pointsZArray.pushBackAsInt32(2)
   * pointsCollector = ArrayVector()
   * pointsCollector.push_back(pointsXArray)
   * pointsCollector.push_back(pointsYArray)
   * pointsCollector.push_back(pointsZArray)
   * exampleGrid = XdmfRectilinearGrid.New(pointsCollector)
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
   * //Assuming that exampleGrid is a shared pointer to an XdmfRectilinearGrid with two axes
   * shared_ptr<XdmfArray> readPointsX = exampleGrid->getCoordinates(0);
   * shared_ptr<XdmfArray> readPointsY = exampleGrid->getCoordinates(1);
   *
   * Python
   *
   * '''
   * Assuming that exampleGrid is a shared pointer to an XdmfRectilinearGrid with two axes
   * '''
   * readPointsX = exampleGrid.getCoordinates(0)
   * readPointsY = exampleGrid.getCoordinates(1)
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
   * //Assuming that exampleGrid is a shared pointer to an XdmfRectilinearGrid with two axes
   * shared_ptr<const XdmfArray> readPointsX = exampleGrid->getCoordinates(0);
   * shared_ptr<const XdmfArray> readPointsY = exampleGrid->getCoordinates(1);
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
   * //Assuming that exampleGrid is a shared pointer to an XdmfRectilinearGrid
   * std::vector<shared_ptr<XdmfArray> > exampleCoordinates = exampleGrid->getCoordinates();
   *
   * Python
   *
   * '''
   * Assuming that exampleGrid is a shared pointer to an XdmfRectilinearGrid
   * '''
   * exampleCoordinates = exampleGrid.getCoordinates()
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
   * //Assuming that exampleGrid is a shared pointer to an XdmfRectilinearGrid
   * const std::vector<shared_ptr<XdmfArray> > exampleCoordinates = exampleGrid->getCoordinates();
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
   * //Assuming that exampleGrid is a shared pointer to an XdmfRectilinearGrid
   * shared_ptr<XdmfArray> exampleDimensions = exampleGrid->getDimensions();
   *
   * Python
   *
   * '''
   * Assuming that exampleGrid is a shared pointer to an XdmfRectilinearGrid
   * '''
   * exampleDimensions = exampleGrid->getDimensions();
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
   * //Assuming that exampleGrid is a shared pointer to an XdmfRectilinearGrid
   * shared_ptr<const XdmfArray> = exampleGrid->getDimensions();
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
   * shared_ptr<XdmfArray> pointsXArray = XdmfArray::New();
   * pointsXArray->pushBack(5);
   * pointsXArray->pushBack(6);
   * pointsXArray->pushBack(7);
   * pointsXArray->pushBack(8);
   * pointsXArray->pushBack(9);
   * pointsXArray->pushBack(10);
   * //Assuming that exampleGrid is a shared pointer to an XdmfRectilinearGrid
   * exampleGrid->setCoordinates(0, pointsXArray);
   *
   * Python
   *
   * pointsXArray = XdmfArray.New()
   * pointsXArray.pushBackAsInt32(5)
   * pointsXArray.pushBackAsInt32(6)
   * pointsXArray.pushBackAsInt32(7)
   * pointsXArray.pushBackAsInt32(8)
   * pointsXArray.pushBackAsInt32(9)
   * pointsXArray.pushBackAsInt32(10)
   * '''
   * Assuming that exampleGrid is a shared pointer to an XdmfRectilinearGrid
   * '''
   * exampleGrid.setCoordinates(0, pointsXArray)
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
   * shared_ptr<XdmfArray> pointsXArray = XdmfArray::New();
   * pointsXArray->pushBack(5);
   * pointsXArray->pushBack(6);
   * pointsXArray->pushBack(7);
   * pointsXArray->pushBack(8);
   * pointsXArray->pushBack(9);
   * pointsXArray->pushBack(10);
   * shared_ptr<XdmfArray> pointsYArray = XdmfArray::New();
   * pointsYArray->pushBack(3);
   * pointsYArray->pushBack(6);
   * pointsYArray->pushBack(4);
   * pointsYArray->pushBack(8);
   * pointsYArray->pushBack(7);
   * pointsYArray->pushBack(10);
   * shared_ptr<XdmfArray> pointsZArray = XdmfArray::New();
   * pointsZArray->pushBack(3);
   * pointsZArray->pushBack(9);
   * pointsZArray->pushBack(4);
   * pointsZArray->pushBack(5);
   * pointsZArray->pushBack(7);
   * pointsZArray->pushBack(2);
   * std::vector<shared_ptr<XdmfArray> > pointsCollector;
   * pointsCollector.push_back(pointsXArray);
   * pointsCollector.push_back(pointsYArray);
   * pointsCollector.push_back(pointsZArray);
   * //Assuming that exampleGrid is a shared pointer to an XdmfRectilinearGrid
   * exampleGrid->setCoordinates(pointsCollector);
   *
   * Python
   *
   * pointsXArray = XdmfArray.New()
   * pointsXArray.pushBackAsInt32(5)
   * pointsXArray.pushBackAsInt32(6)
   * pointsXArray.pushBackAsInt32(7)
   * pointsXArray.pushBackAsInt32(8)
   * pointsXArray.pushBackAsInt32(9)
   * pointsXArray.pushBackAsInt32(10)
   * pointsYArray = XdmfArray.New()
   * pointsYArray.pushBackAsInt32(5)
   * pointsYArray.pushBackAsInt32(6)
   * pointsYArray.pushBackAsInt32(7)
   * pointsYArray.pushBackAsInt32(8)
   * pointsYArray.pushBackAsInt32(9)
   * pointsYArray.pushBackAsInt32(10)
   * pointsZArray = XdmfArray.New()
   * pointsZArray.pushBackAsInt32(5)
   * pointsZArray.pushBackAsInt32(6)
   * pointsZArray.pushBackAsInt32(7)
   * pointsZArray.pushBackAsInt32(8)
   * pointsZArray.pushBackAsInt32(9)
   * pointsZArray.pushBackAsInt32(10)
   * pointsCollector = ArrayVector();
   * pointsCollector.push_back(pointsXArray);
   * pointsCollector.push_back(pointsYArray);
   * pointsCollector.push_back(pointsZArray);
   * '''
   * Assuming that exampleGrid is a shared pointer to an XdmfRectilinearGrid
   * '''
   * exampleGrid.setCoordinates(pointsCollector);
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
