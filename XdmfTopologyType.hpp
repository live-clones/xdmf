/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfTopologyType.hpp                                                */
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

#ifndef XDMFTOPOLOGYTYPE_HPP_
#define XDMFTOPOLOGYTYPE_HPP_

// Includes
#include "Xdmf.hpp"
#include "XdmfItemProperty.hpp"

/**
 * @brief Property describing the types of elements stored in an
 * XdmfTopology.
 *
 * XdmfTopologyType is a property used by XdmfTopology to specify the
 * element types stored. A specific XdmfTopologyType can be created by
 * calling one of the static methods in the class,
 * i.e. XdmfTopologyType::Tetrahedron().
 *
 * Xdmf supports the following topology types:
 *   NoTopologyType
 *   Polyvertex - Unconnected Points
 *   Polyline - Line Segments
 *   Polygon - N Edge Polygon
 *   Triangle - 3 Edge Polygon
 *   Quadrilateral - 4 Edge Polygon
 *   Tetrahedron - 4 Triangular Faces
 *   Wedge - 4 Triangular Faces, Quadrilateral Base
 *   Hexahedron - 6 Quadrilateral Faces
 *   Edge_3 - 3 Node Quadratic Line
 *   Triangle_6 - 6 Node Quadratic Triangle
 *   Quadrilateral_8 - 8 Node Quadratic Quadrilateral
 *   Quadrilateral_9 - 9 Node Bi-Quadratic Quadrilateral
 *   Tetrahedron_10 - 10 Node Quadratic Tetrahedron
 *   Pyramid_13 - 13 Node Quadratic Pyramid
 *   Wedge_15 - 15 Node Quadratic Wedge
 *   Wedge_18 - 18 Node Bi-Quadratic Wedge
 *   Hexahedron_20 - 20 Node Quadratic Hexahedron
 *   Hexahedron_24 - 24 Node Bi-Quadratic Hexahedron
 *   Hexahedron_27 - 27 Node Tri-Quadratic Hexahedron
 *   Hexahedron_64 - 64 Node Tri-Cubic Hexahedron
 *   Hexahedron_64_GLL - 64 Node Spectral Tri-Cubic Hexahedron with Gauss-Lobatto-Legendre points.
 *   Hexahedron_125 - 125 Node Tri-Quartic Hexahedron
 *   Hexahedron_125_GLL - 125 Node Spectral Tri-Quartic Hexahedron with Gauss-Lobatto-Legendre points.
 *   Mixed - Mixture of Unstructured Topologies
 */
class XDMF_EXPORT XdmfTopologyType : public XdmfItemProperty {

 public:

  virtual ~XdmfTopologyType();

  friend class XdmfTopology;

  enum CellType {
    NoCellType,
    Linear,
    Quadratic,
    Cubic,
    Quartic,
    Arbitrary,
    Structured
  };

  // Supported Xdmf Topology Types
  static boost::shared_ptr<const XdmfTopologyType> NoTopologyType();
  static boost::shared_ptr<const XdmfTopologyType> Polyvertex();
  static boost::shared_ptr<const XdmfTopologyType> Polyline();
  static boost::shared_ptr<const XdmfTopologyType>
    Polygon(const unsigned int nodesPerElement);
  static boost::shared_ptr<const XdmfTopologyType> Triangle();
  static boost::shared_ptr<const XdmfTopologyType> Quadrilateral();
  static boost::shared_ptr<const XdmfTopologyType> Tetrahedron();
  static boost::shared_ptr<const XdmfTopologyType> Pyramid();
  static boost::shared_ptr<const XdmfTopologyType> Wedge();
  static boost::shared_ptr<const XdmfTopologyType> Hexahedron();
  static boost::shared_ptr<const XdmfTopologyType> Edge_3();
  static boost::shared_ptr<const XdmfTopologyType> Triangle_6();
  static boost::shared_ptr<const XdmfTopologyType> Quadrilateral_8();
  static boost::shared_ptr<const XdmfTopologyType> Quadrilateral_9();
  static boost::shared_ptr<const XdmfTopologyType> Tetrahedron_10();
  static boost::shared_ptr<const XdmfTopologyType> Pyramid_13();
  static boost::shared_ptr<const XdmfTopologyType> Wedge_15();
  static boost::shared_ptr<const XdmfTopologyType> Wedge_18();
  static boost::shared_ptr<const XdmfTopologyType> Hexahedron_20();
  static boost::shared_ptr<const XdmfTopologyType> Hexahedron_24();
  static boost::shared_ptr<const XdmfTopologyType> Hexahedron_27();
  static boost::shared_ptr<const XdmfTopologyType> Hexahedron_64();
  static boost::shared_ptr<const XdmfTopologyType> Hexahedron_64_GLL();
  static boost::shared_ptr<const XdmfTopologyType> Hexahedron_125();
  static boost::shared_ptr<const XdmfTopologyType> Hexahedron_125_GLL();
  static boost::shared_ptr<const XdmfTopologyType> Mixed();

  /**
   * Get the cell type associated with this topology type.
   *
   * @return a CellType containing the cell type.
   */
  CellType getCellType() const;

  /**
   * Get the id of this cell type, necessary in order to create grids
   * containing mixed cells.
   *
   * @return the ID of the topology type.
   */
  virtual unsigned int getID() const;

  /**
   * Get the name of this topology type.
   *
   * @return the name of this topology type.
   */
  virtual std::string getName() const;

  /**
   * Get the number of nodes per element associated with this topology
   * type.
   *
   * @return an unsigned int containing number of nodes per element.
   */
  virtual unsigned int getNodesPerElement() const;

  void
  getProperties(std::map<std::string, std::string> & collectedProperties) const;

 protected:

  /**
   * Protected constructor for XdmfTopologyType. The constructor is
   * protected because all topology types supported by Xdmf should be
   * accessed through more specific static methods that construct
   * XdmfTopologyType - i.e. XdmfTopologyType::Tetrahedron()
   */
  XdmfTopologyType(const unsigned int nodesPerElement,
                   const std::string & name,
                   const CellType cellType,
                   const unsigned int id);

 private:

  XdmfTopologyType(const XdmfTopologyType &); // Not implemented.
  void operator=(const XdmfTopologyType &); // Not implemented.

  static boost::shared_ptr<const XdmfTopologyType>
  New(const std::map<std::string, std::string> & itemProperties);

  const CellType mCellType;
  const unsigned int mID;
  const std::string mName;
  const unsigned int mNodesPerElement;

};

#endif /* XDMFTOPOLOGYTYPE_HPP_ */
