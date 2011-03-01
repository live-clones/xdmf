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
   * Create a new XdmfGridUnstructured.
   *
   * @return constructed XdmfGridUnstructured.
   */
  static boost::shared_ptr<XdmfUnstructuredGrid> New();

  virtual ~XdmfUnstructuredGrid();

  static const std::string ItemTag;

  /**
   * Get the geometry associated with this grid.
   *
   * @return the geometry associated with this grid.
   */
  boost::shared_ptr<XdmfGeometry> getGeometry();

  virtual std::string getItemTag() const;

  /**
   * Get the topology associated with this grid.
   *
   * @return the topology associated with this grid.
   */
  boost::shared_ptr<XdmfTopology> getTopology();

  /**
   * Set the geometry associated with this grid.
   *
   * @param geometry an XdmfGeometry to associate with this grid.
   */
  void setGeometry(const boost::shared_ptr<XdmfGeometry> geometry);

  /**
   * Set the topology associated with this grid.
   *
   * @param topology an XdmfTopology to associate with this grid.
   */
  void setTopology(const boost::shared_ptr<XdmfTopology> topology);

 protected:

  XdmfUnstructuredGrid();

 private:

  XdmfUnstructuredGrid(const XdmfUnstructuredGrid &);  // Not implemented.
  void operator=(const XdmfUnstructuredGrid &);  // Not implemented.

};

#ifdef _WIN32
XDMF_TEMPLATE template class XDMF_EXPORT
boost::shared_ptr<XdmfTopology>;
XDMF_TEMPLATE template class XDMF_EXPORT
boost::shared_ptr<XdmfGeometry>;
#endif

#endif /* XDMFUNSTRUCTUREDGRID_HPP_ */
