/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfCurvilinearGrid.hpp                                             */
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

#ifndef XDMFCURVILINEARGRID_HPP_
#define XDMFCURVILINEARGRID_HPP_

// Forward Declarations
class XdmfArray;

// Includes
#include "Xdmf.hpp"
#include "XdmfGrid.hpp"

/**
 * @brief A curvilinear (or structured) grid consisting of cells and
 * points arranged on a regular lattice in space.
 *
 * XdmfCurvilinearGrid represents a mesh of cells and points arranged
 * with regular topology and irregular geometry.
 *
 * In order to define a curvilinear grid, the dimensions of the grid
 * must be supplied along with the coordinates of each point.
 *
 */
class XDMF_EXPORT XdmfCurvilinearGrid : public XdmfGrid {

public:

  /**
   * Create a new curvilinear grid (Two dimensional).
   *
   * Example of use:
   *
   * C++
   *
   * unsigned int newPointsX = 5;
   * unsigned int newPointsY = 5;
   * shared_ptr<XdmfCurvilinearGrid> exampleGrid = XdmfCurvilinearGrid::New(newPointsX, newPointsY);
   *
   * Python
   *
   * newPointsX = 5
   * newPointsY = 5
   * exampleGrid = XdmfCurvilinearGrid.New(newPointsX, newPointsY)
   *
   * @param xNumPoints the number of points in the x direction.
   * @param yNumPoints the number of points in the y direction.
   *
   * @return constructed curvilinear grid.
   */
  static shared_ptr<XdmfCurvilinearGrid>
  New(const unsigned int xNumPoints,
      const unsigned int yNumPoints);

  /**
   * Create a new curvilinear grid (Three dimensional).
   *
   * Example of use:
   *
   * C++
   *
   * unsigned int newPointsX = 5;
   * unsigned int newPointsY = 5;
   * unsigned int newPointsZ = 5;
   * shared_ptr<XdmfCurvilinearGrid> exampleGrid = XdmfCurvilinearGrid::New(newPointsX, newPointsY, newPointsZ);
   *
   * Python
   *
   * newPointsX = 5
   * newPointsY = 5
   * newPointsZ = 5
   * exampleGrid = XdmfCurvilinearGrid.New(newPointsX, newPointsY, newPointsZ)
   *
   * @param xNumPoints the number of points in the x direction.
   * @param yNumPoints the number of points in the y direction.
   * @param zNumPoints the number of points in the z direction.
   *
   * @return constructed curvilinear grid.
   */
  static shared_ptr<XdmfCurvilinearGrid>
  New(const unsigned int xNumPoints,
      const unsigned int yNumPoints,
      const unsigned int zNumPoints);

  /**
   * Create a new curvilinear grid (N dimensional).
   *
   * Example of use:
   *
   * C++
   *
   * shared_ptr<XdmfArray> newPoints = XdmfArray::New();
   * newPoints->pushBack(5);
   * newPoints->pushBack(5);
   * newPoints->pushBack(5);
   * shared_ptr<XdmfCurvilinearGrid> exampleGrid = XdmfCurvilinearGrid::New(newPoints);
   *
   * Python
   *
   * newPoints = XdmfArray.New()
   * newPoints.pushBackAsInt32(5)
   * newPoints.pushBackAsInt32(5)
   * newPoints.pushBackAsInt32(5)
   * exampleGrid = XdmfCurvilinearGrid.New(newPoints)
   *
   * @param numPoints the number of points in each direction.
   *
   * @return constructed curvilinear grid.
   */
  static shared_ptr<XdmfCurvilinearGrid>
  New(const shared_ptr<XdmfArray> numPoints);

  virtual ~XdmfCurvilinearGrid();

  LOKI_DEFINE_VISITABLE(XdmfCurvilinearGrid, XdmfGrid);
  static const std::string ItemTag;

  /**
   * Get the dimensions of the grid, the number of points in each
   * direction.
   *
   * Example of use:
   *
   * C++
   *
   * //Assuming that exampleGrid is a shared pointer to an XdmfCurvilinearGrid object
   * shared_ptr<XdmfArray> exampleDimensions = exampleGrid->getDimensions();
   *
   * Python
   *
   * '''
   * Assuming that exampleGrid is a shared pointer to an XdmfCurvilinearGrid object
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
   * //Assuming that exampleGrid is a shared pointer to an XdmfCurvilinearGrid object
   * shared_ptr<const XdmfArray> exampleDimensions = exampleGrid->getDimensions();
   *
   * Python: Python doesn't have a constant version
   *
   * @return XdmfArray containing the dimensions of this grid.
   */
  shared_ptr<const XdmfArray> getDimensions() const;

  /**
   * Get the geometry associated with this grid.
   *
   * Example of use:
   *
   * C++
   *
   * //Assuming that exampleGrid is a sharedPointer to an XdmfCurvilinearGrid object
   * shared_ptr<XdmfGeometry> exampleGeometry = exampleGrid->getGeometry();
   *
   * Python
   *
   * '''
   * Assuming that exampleGrid is a sharedPointer to an XdmfCurvilinearGrid object
   * '''
   * exampleGeometry = exampleGrid.getGeometry()
   *
   * @return the geometry associated with this grid.
   */
  shared_ptr<XdmfGeometry> getGeometry();

  /**
   * Set the dimensions of the grid, the number of points in each
   * direction.
   *
   * Example of use:
   *
   * C++
   *
   * shared_ptr<XdmfArray> newPoints = XdmfArray::New();
   * newPoints->pushBack(5);
   * newPoints->pushBack(5);
   * newPoints->pushBack(5);
   * //Assuming that exampleGrid is a sharedPointer to an XdmfCurvilinearGrid object
   * exampleGrid->setDimensions(newPoints);
   *
   * Python
   *
   * newPoints = XdmfArray.New()
   * newPoints.pushBackAsInt32(5)
   * newPoints.pushBackAsInt32(5)
   * newPoints.pushBackAsInt32(5)
   * '''
   * Assuming that exampleGrid is a sharedPointer to an XdmfCurvilinearGrid object
   * '''
   * exampleGrid.setDimensions(newPoints)
   *
   * @param dimensions the dimension of the grid.
   */
  void setDimensions(const shared_ptr<XdmfArray> dimensions);

  /**
   * Set the geometry associated with this grid.
   *
   * Example of use:
   *
   * C++
   *
   * shared_ptr<XdmfArray> newPoints = XdmfArray::New();
   * newPoints->pushBack(5);
   * newPoints->pushBack(5);
   * newPoints->pushBack(5);
   * shared_ptr<XdmfGeometry> newGeometry = XdmfGeometry::New();
   * newGeometry->setType(XdmfGeometryType::XYZ());
   * newGeometry->insert(0, newPoints, 0, 3, 1, 1);//Start index is 0, 3 values are passed, stride for both arrays is 1
   * //Assuming that exampleGrid is a sharedPointer to an XdmfCurvilinearGrid object
   * exampleGrid->setGeometry(newGeometry);
   *
   * Python
   *
   * newPoints = XdmfArray.New()
   * newPoints.pushBackAsInt32(5)
   * newPoints.pushBackAsInt32(5)
   * newPoints.pushBackAsInt32(5)
   * newGeometry = XdmfGeometry.New()
   * newGeometry.setType(XdmfGeometryType.XYZ())
   * newGeometry.insert(0, newPoints, 0, 3, 1, 1)//Start index is 0, 3 values are passed, stride for both arrays is 1
   * '''
   * Assuming that exampleGrid is a sharedPointer to an XdmfCurvilinearGrid object
   * '''
   * exampleGrid.setGeometry(newGeometry)
   *
   * @param geometry an XdmfGeometry to associate with this grid.
   */
  void setGeometry(const shared_ptr<XdmfGeometry> geometry);

protected:

  XdmfCurvilinearGrid(const shared_ptr<XdmfArray> numPoints);

  void
  populateItem(const std::map<std::string, std::string> & itemProperties,
               const std::vector<shared_ptr<XdmfItem> > & childItems,
               const XdmfCoreReader * const reader);

private:

  /**
   * PIMPL
   */
  class XdmfCurvilinearGridImpl;

  XdmfCurvilinearGrid(const XdmfCurvilinearGrid &);  // Not implemented.
  void operator=(const XdmfCurvilinearGrid &);  // Not implemented.

  XdmfCurvilinearGridImpl * mImpl;

};

#ifdef _WIN32
XDMF_TEMPLATE template class XDMF_EXPORT
shared_ptr<XdmfGeometry>;
XDMF_TEMPLATE template class XDMF_EXPORT
shared_ptr<XdmfArray>;
XDMF_TEMPLATE template class XDMF_EXPORT
shared_ptr<const XdmfArray>;
#endif

#endif /* XDMFCURVILINEARGRID_HPP_ */
