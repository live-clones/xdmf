/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfGrid.hpp                                                        */
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

#ifndef XDMFGRID_HPP_
#define XDMFGRID_HPP_

// Forward Declarations
class XdmfAttribute;
class XdmfGeometry;
class XdmfMap;
class XdmfSet;
class XdmfTime;
class XdmfTopology;

// Includes
#include "Xdmf.hpp"
#include "XdmfItem.hpp"

/**
 * @brief A mesh containing elements, points, and fields attached to
 * the mesh.
 *
 * XdmfGrid represents a mesh. It is required to contain two other
 * Xdmf data structures, an XdmfGeometry that stores point locations
 * and an XdmfTopology that store connectivity
 * information. XdmfAttributes can be inserted into the XdmfGrid to
 * specify fields centered on various parts of the mesh.  XdmfSets can
 * be inserted into XdmfGrids to specify collections of mesh elements.
 *
 * XdmfGrid is an abstract base class. There are several
 * implementations for representing both structured and unstructured
 * grids.
 */
class XDMF_EXPORT XdmfGrid : public virtual XdmfItem {

public:

  virtual ~XdmfGrid() = 0;

  LOKI_DEFINE_VISITABLE(XdmfGrid, XdmfItem);
  XDMF_CHILDREN(XdmfAttribute, Attribute, Name);
  XDMF_CHILDREN(XdmfSet, Set, Name);
  XDMF_CHILDREN(XdmfMap, Map, Name);
  static const std::string ItemTag;

  /**
   * Get the geometry associated with this grid.
   *
   * @return the geometry associated with this grid.
   */
  shared_ptr<const XdmfGeometry> getGeometry() const;

  std::map<std::string, std::string> getItemProperties() const;

  virtual std::string getItemTag() const;

  /**
   * Get the name of the grid.
   *
   * @return the name of the grid.
   */
  std::string getName() const;

  /**
   * Get the time associated with this grid.
   *
   * @return pointer to the XdmfTime attached to this grid. If no
   * XdmfTime is attached, return a NULL pointer.
   */
  shared_ptr<XdmfTime> getTime();

  /**
   * Get the time associated with this grid (const version).
   *
   * @return pointer to the XdmfTime attached to this grid. If no
   * XdmfTime is attached, return a NULL pointer.
   */
  shared_ptr<const XdmfTime> getTime() const;

  /**
   * Get the topology associated with this grid.
   *
   * @return the topology associated with this grid.
   */
  shared_ptr<const XdmfTopology> getTopology() const;

  using XdmfItem::insert;

  /**
   * Set the name of the grid.
   *
   * @param name of the grid to set.
   */
  void setName(const std::string & name);

  /**
   * Set the time associated with this grid.
   *
   * @param time an XdmfTime to associate with this grid.
   */
  void setTime(const shared_ptr<XdmfTime> time);

  virtual void traverse(const shared_ptr<XdmfBaseVisitor> visitor);

protected:

  XdmfGrid(const shared_ptr<XdmfGeometry> geometry,
           const shared_ptr<XdmfTopology> topology,
           const std::string & name = "Grid");

  virtual void
  populateItem(const std::map<std::string, std::string> & itemProperties,
               std::vector<shared_ptr<XdmfItem> > & childItems,
               const XdmfCoreReader * const reader);

  shared_ptr<XdmfGeometry> mGeometry;
  shared_ptr<XdmfTopology> mTopology;

private:

  XdmfGrid(const XdmfGrid &);  // Not implemented.
  void operator=(const XdmfGrid &);  // Not implemented.

  std::string mName;
  shared_ptr<XdmfTime> mTime;

};

#ifdef _WIN32
XDMF_TEMPLATE template class XDMF_EXPORT
std::allocator<shared_ptr<XdmfAttribute> >;
XDMF_TEMPLATE template class XDMF_EXPORT
std::vector<shared_ptr<XdmfAttribute>,
            std::allocator<shared_ptr<XdmfAttribute> > >;
XDMF_TEMPLATE template class XDMF_EXPORT
std::allocator<shared_ptr<XdmfSet> >;
XDMF_TEMPLATE template class XDMF_EXPORT
std::vector<shared_ptr<XdmfSet>,
            std::allocator<shared_ptr<XdmfSet> > >;
XDMF_TEMPLATE template class XDMF_EXPORT
shared_ptr<XdmfGeometry>;
XDMF_TEMPLATE template class XDMF_EXPORT
shared_ptr<const XdmfGeometry>;
XDMF_TEMPLATE template class XDMF_EXPORT
shared_ptr<XdmfTopology>;
XDMF_TEMPLATE template class XDMF_EXPORT
shared_ptr<const XdmfTopology>;
XDMF_TEMPLATE template class XDMF_EXPORT
shared_ptr<XdmfMap>;
XDMF_TEMPLATE template class XDMF_EXPORT
shared_ptr<const XdmfMap>;
XDMF_TEMPLATE template class XDMF_EXPORT
shared_ptr<XdmfTime>;
XDMF_TEMPLATE template class XDMF_EXPORT
shared_ptr<const XdmfTime>;
#endif

#endif /* XDMFGRID_HPP_ */
