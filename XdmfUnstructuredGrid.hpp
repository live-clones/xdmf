/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfUnstructuredGrid.hpp                                            */
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

#ifndef XDMFUNSTRUCTUREDGRID_HPP_
#define XDMFUNSTRUCTUREDGRID_HPP_

// Forward Declarations
class XdmfRegularGrid;

// Includes
#include "Xdmf.hpp"
#include "XdmfGrid.hpp"

/**
 * @brief An unstructured grid that consists of elements, points, and
 * fields attached to the mesh.
 *
 * After creating an unstructured grid, the XdmfGeometry and
 * XdmfTopology must be set. The XdmfTopology describes the element
 * types contained in the grid and their connectivity. The
 * XdmfGeometry describes the positions of nodes.
 */
class XDMF_EXPORT XdmfUnstructuredGrid : public XdmfGrid {

public:

  /**
   * Create a new XdmfUnstructuredGrid.
   *
   * Example of use:
   *
   * C++
   *
   * shared_ptr<XdmfUnstructuredGrid> exampleGrid = XdmfUnstructuredGrid::New();
   *
   * Python
   *
   * exampleGrid = XdmfUnstructuredGrid.New()
   *
   * @return constructed XdmfUnstructuredGrid.
   */
  static shared_ptr<XdmfUnstructuredGrid> New();

  /**
   * Create a new XdmfUnstructuredGrid from a XdmfRegularGrid.
   *
   * Example of use:
   *
   * C++
   *
   * double newBrickX = 0.0;
   * double newBrickY = 0.0;
   * unsigned int newPointsX = 5;
   * unsigned int newPointsY = 5;
   * double newOriginX = 20.0;
   * double newOriginY = 20.0;
   * shared_ptr<XdmfRegularGrid> baseGrid = XdmfRegularGrid::New(newBrickX, newBrickY, newPointsX, newPointsY, newOriginX, newOriginY);
   * shared_ptr<XdmfUnstructuredGrid> exampleGrid = XdmfUnstructuredGrid::New(baseGrid);
   *
   * Python
   *
   * newBrickX = 0.0
   * newBrickY = 0.0
   * newPointsX = 5
   * newPointsY = 5
   * newOriginX = 20.0
   * newOriginY = 20.0
   * baseGrid = XdmfRegularGrid.New(newBrickX, newBrickY, newPointsX, newPointsY, newOriginX, newOriginY)
   * exampleGrid = XdmfUnstructuredGrid.New(baseGrid)
   *
   * @param regularGrid	The grid that the unstructured grid will be created from
   *
   * @return constructed XdmfUnstructuredGrid.
   */
  static shared_ptr<XdmfUnstructuredGrid> 
  New(const shared_ptr<XdmfRegularGrid> regularGrid);

  virtual ~XdmfUnstructuredGrid();

  static const std::string ItemTag;

  /**
   * Get the geometry associated with this grid.
   *
   * Example of use:
   *
   * C++
   *
   * //Assuming that exampleGrid is a shared pointer to an XdmfUnstructuredGrid object
   * shared_ptr<XdmfGeometry> exampleGeometry = exampleGrid->getGeometry();
   *
   * Python
   *
   * '''
   * Assuming that exampleGrid is a shared pointer to an XdmfUnstructuredGrid object
   * '''
   * exampleGeometry = exampleGrid.getGeometry()
   *
   * @return the geometry associated with this grid.
   */
  shared_ptr<XdmfGeometry> getGeometry();

  virtual std::string getItemTag() const;

  /**
   * Get the topology associated with this grid.
   *
   * Example of use:
   *
   * C++
   *
   * //Assuming that exampleGrid is a shared pointer to an XdmfUnstructuredGrid object
   * shared_ptr<XdmfTopology> exampleTopology = exampleGrid->getTopology();
   *
   * Python
   *
   * '''
   * Assuming that exampleGrid is a shared pointer to an XdmfUnstructuredGrid object
   * '''
   * exampleTopology = exampleGrid.getTopology()
   *
   * @return the topology associated with this grid.
   */
  shared_ptr<XdmfTopology> getTopology();

  /**
   * Set the geometry associated with this grid.
   *
   * Example of use:
   *
   * C++
   *
   * shared_ptr<XdmfUnstructuredGrid> exampleGrid = XdmfUnstructuredGrid::New();
   * shared_ptr<XdmfGeometry> newGeometry = XdmfGeometry::New();
   * newGeometry->setType(XdmfGeometryType::XYZ());
   * newGeometry->pushBack(1);
   * newGeometry->pushBack(2);
   * newGeometry->pushBack(3);
   * newGeometry->pushBack(4);
   * newGeometry->pushBack(5);
   * newGeometry->pushBack(6);
   * newGeometry->pushBack(7);
   * newGeometry->pushBack(8);
   * newGeometry->pushBack(9);
   * exampleGrid->setGeometry(newGeometry);
   *
   * Python
   *
   * exampleGrid = XdmfUnstructuredGrid.New()
   * newGeometry = XdmfGeometry.New()
   * newGeometry.setType(XdmfGeometryType.XYZ())
   * newGeometry.pushBackAsInt32(1)
   * newGeometry.pushBackAsInt32(2)
   * newGeometry.pushBackAsInt32(3)
   * newGeometry.pushBackAsInt32(4)
   * newGeometry.pushBackAsInt32(5)
   * newGeometry.pushBackAsInt32(6)
   * newGeometry.pushBackAsInt32(7)
   * newGeometry.pushBackAsInt32(8)
   * newGeometry.pushBackAsInt32(9)
   * exampleGrid.setGeometry(newGeometry)
   *
   * @param geometry an XdmfGeometry to associate with this grid.
   */
  void setGeometry(const shared_ptr<XdmfGeometry> geometry);

  /**
   * Set the topology associated with this grid.
   *
   * Example of use:
   *
   * C++
   *
   * shared_ptr<XdmfUnstructuredGrid> exampleGrid = XdmfUnstructuredGrid::New();
   * shared_ptr<XdmfTopology> newTopology = XdmfTopology::New();
   * newTopology->setType(XdmfTopologyType::Triangle());
   * newTopology->pushBack(1);
   * newTopology->pushBack(2);
   * newTopology->pushBack(3);
   * newTopology->pushBack(4);
   * newTopology->pushBack(5);
   * newTopology->pushBack(6);
   * newTopology->pushBack(7);
   * newTopology->pushBack(8);
   * newTopology->pushBack(9);
   * exampleGrid->setTopology(newTopology);
   *
   * Python
   *
   * exampleGrid = XdmfUnstructuredGrid.New()
   * newTopology = XdmfTopology.New()
   * newTopology.setType(XdmfTopologyType.Triangle())
   * newTopology.pushBackAsInt32(1)
   * newTopology.pushBackAsInt32(2)
   * newTopology.pushBackAsInt32(3)
   * newTopology.pushBackAsInt32(4)
   * newTopology.pushBackAsInt32(5)
   * newTopology.pushBackAsInt32(6)
   * newTopology.pushBackAsInt32(7)
   * newTopology.pushBackAsInt32(8)
   * newTopology.pushBackAsInt32(9)
   * exampleGrid.setTopology(newTopology)
   *
   * @param topology an XdmfTopology to associate with this grid.
   */
  void setTopology(const shared_ptr<XdmfTopology> topology);

protected:

  XdmfUnstructuredGrid();
  XdmfUnstructuredGrid(const shared_ptr<XdmfRegularGrid> regularGrid);

private:

  XdmfUnstructuredGrid(const XdmfUnstructuredGrid &);  // Not implemented.
  void operator=(const XdmfUnstructuredGrid &);  // Not implemented.

};

#ifdef _WIN32
XDMF_TEMPLATE template class XDMF_EXPORT
shared_ptr<XdmfTopology>;
XDMF_TEMPLATE template class XDMF_EXPORT
shared_ptr<XdmfGeometry>;
#endif

#endif /* XDMFUNSTRUCTUREDGRID_HPP_ */
