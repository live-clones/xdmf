/*******************************************************************/
/*                               XDMF                              */
/*                   eXtensible Data Model and Format              */
/*                                                                 */
/*  Id : Id  */
/*  Date : $Date$ */
/*  Version : $Revision$ */
/*                                                                 */
/*  Author:                                                        */
/*     John Vines                                                  */
/*     john.m.vines@us.army.mil                                    */
/*     US Army Research Laboratory                                 */
/*     Aberdeen Proving Ground, MD                                 */
/*                                                                 */
/*     Copyright @ 2012 US Army Research Laboratory                */
/*     All Rights Reserved                                         */
/*     See Copyright.txt or http://www.arl.hpc.mil/ice for details */
/*                                                                 */
/*     This software is distributed WITHOUT ANY WARRANTY; without  */
/*     even the implied warranty of MERCHANTABILITY or FITNESS     */
/*     FOR A PARTICULAR PURPOSE.  See the above copyright notice   */
/*     for more information.                                       */
/*                                                                 */
/*******************************************************************/
#ifndef XDMFFORTRAN_HPP_
#define XDMFFORTRAN_HPP_

// Forward Declarations
class XdmfAttribute;
class XdmfDomain;
class XdmfGeometry;
class XdmfGridCollection;
class XdmfInformation;
class XdmfTime;
class XdmfTopology;
class XdmfUnstructuredGrid;

//Includes
#include <stack>
#include <vector>
#include "XdmfUtils.hpp"
#include "XdmfSharedPtr.hpp"

/**
 * Array Type
 */
#define XDMF_ARRAY_TYPE_INT8    0
#define XDMF_ARRAY_TYPE_INT16   1
#define XDMF_ARRAY_TYPE_INT32   2
#define XDMF_ARRAY_TYPE_INT64   3
#define XDMF_ARRAY_TYPE_UINT8   4
#define XDMF_ARRAY_TYPE_UINT16  5
#define XDMF_ARRAY_TYPE_UINT32  6
#define XDMF_ARRAY_TYPE_FLOAT32 7
#define XDMF_ARRAY_TYPE_FLOAT64 8

/**
 * Attribute Center
 */
#define XDMF_ATTRIBUTE_CENTER_GRID 100
#define XDMF_ATTRIBUTE_CENTER_CELL 101
#define XDMF_ATTRIBUTE_CENTER_FACE 102
#define XDMF_ATTRIBUTE_CENTER_EDGE 103
#define XDMF_ATTRIBUTE_CENTER_NODE 104

/**
 * Attribute Type
 */
#define XDMF_ATTRIBUTE_TYPE_SCALAR   200
#define XDMF_ATTRIBUTE_TYPE_VECTOR   201
#define XDMF_ATTRIBUTE_TYPE_TENSOR   202
#define XDMF_ATTRIBUTE_TYPE_MATRIX   203
#define XDMF_ATTRIBUTE_TYPE_TENSOR6  204
#define XDMF_ATTRIBUTE_TYPE_GLOBALID 205
#define XDMF_ATTRIBUTE_TYPE_NOTYPE   206

/**
 * Geometry Type
 */
#define XDMF_GEOMETRY_TYPE_XYZ  301
#define XDMF_GEOMETRY_TYPE_XY   302

/**
 * Grid Collection Type
 */
#define XDMF_GRID_COLLECTION_TYPE_SPATIAL  400
#define XDMF_GRID_COLLECTION_TYPE_TEMPORAL 401 

/**
 * Topology Type
 */
#define XDMF_TOPOLOGY_TYPE_POLYVERTEX       500
#define XDMF_TOPOLOGY_TYPE_POLYLINE         501
#define XDMF_TOPOLOGY_TYPE_POLYGON          502
#define XDMF_TOPOLOGY_TYPE_TRIANGLE         503
#define XDMF_TOPOLOGY_TYPE_QUADRILATERAL    504
#define XDMF_TOPOLOGY_TYPE_TETRAHEDRON      505
#define XDMF_TOPOLOGY_TYPE_PYRAMID          506
#define XDMF_TOPOLOGY_TYPE_WEDGE            507
#define XDMF_TOPOLOGY_TYPE_HEXAHEDRON       508
#define XDMF_TOPOLOGY_TYPE_EDGE_3           509
#define XDMF_TOPOLOGY_TYPE_TRIANGLE_6       510
#define XDMF_TOPOLOGY_TYPE_QUADRILATERAL_8  511
#define XDMF_TOPOLOGY_TYPE_QUADRILATERAL_9  512
#define XDMF_TOPOLOGY_TYPE_TETRAHEDRON_10   513
#define XDMF_TOPOLOGY_TYPE_PYRAMID_13       514
#define XDMF_TOPOLOGY_TYPE_WEDGE_15         515
#define XDMF_TOPOLOGY_TYPE_WEDGE_18         516
#define XDMF_TOPOLOGY_TYPE_HEXAHEDRON_20    517
#define XDMF_TOPOLOGY_TYPE_HEXAHEDRON_24    518
#define XDMF_TOPOLOGY_TYPE_HEXAHEDRON_27    519
#define XDMF_TOPOLOGY_TYPE_HEXAHEDRON_64    520
#define XDMF_TOPOLOGY_TYPE_HEXAHEDRON_125   521
#define XDMF_TOPOLOGY_TYPE_HEXAHEDRON_216   522
#define XDMF_TOPOLOGY_TYPE_HEXAHEDRON_343   523
#define XDMF_TOPOLOGY_TYPE_HEXAHEDRON_512   524
#define XDMF_TOPOLOGY_TYPE_HEXAHEDRON_729   525
#define XDMF_TOPOLOGY_TYPE_HEXAHEDRON_1000  526
#define XDMF_TOPOLOGY_TYPE_HEXAHEDRON_1331  527
#define XDMF_TOPOLOGY_TYPE_MIXED            528

// This works with g77. Different compilers require different
// name mangling.
#if !defined(WIN32)
#define XdmfInit xdmfinit_
#define XdmfClose xdmfclose_
#define XdmfAddAttribute xdmfaddattribute_
#define XdmfAddGrid xdmfaddgrid_
#define XdmfAddGridCollection xdmfaddgridcollection_
#define XdmfAddInformation xdmfaddinformation_
#define XdmfAddPreviousAttribute xdmfaddpreviousattribute_
#define XdmfAddPreviousInformation xdmfaddpreviousinformation_
#define XdmfCloseGridCollection xdmfclosegridcollection_
#define XdmfSetGeometry xdmfsetgeometry_
#define XdmfSetPreviousGeometry xdmfsetpreviousgeometry_
#define XdmfSetPreviousTopology xdmfsetprevioustopology_
#define XdmfSetTime xdmfsettime_
#define XdmfSetTopology xdmfsettopology_
#define XdmfWrite xdmfwrite_
#endif

/**
 * @brief XdmfFortran provides an interface to outputting xdmf files
 * from Fortran.
 *
 * Fortran programs will include Xdmf.f and link against libXdmfUtils.
 */
class XDMFUTILS_EXPORT XdmfFortran {
  
public:

  /**
   * Create a new xdmf file from fortran.
   *
   * @return constructed XdmfFortran.
   */
  XdmfFortran();

  virtual ~XdmfFortran();

  /**
   * Add an attribute that will be inserted into the next grid or grid
   * collection.
   *
   * @param name of the attribute.
   * @param attributeCenter the attribute center.
   * @param attributeType the attribute type.
   * @param numValues number of attribute values to copy.
   * @param arrayType type of attribute values.
   * @param values array of attribute values.
   *
   * @return int providing id to fortran if reusing.
   */
  int addAttribute(const char * const name,
                   const int attributeCenter,
                   const int attributeType,
                   const unsigned int numValues,
                   const int arrayType,
                   const void * const values);

  /**
   * Add grid to domain or collection. Inserts geometry, topology,
   * attributes, and informations into grid. If no geometry or
   * topology is set, an error is generated.
   *
   * @param name of the grid.
   */
  void addGrid(const char * const name);

  /**
   * Add grid collection to domain or collection. Inserts attributes
   * and informations into collection.
   *
   * @param name of the collection.
   * @param gridCollectionType the grid collection type.
   */
  void addGridCollection(const char * const name,
                         const int gridCollectionType);

  /**
   * Add an information that will be inserted into the next added
   * grid or grid collection.
   *
   * @param key string containing the key of the information to
   * create.
   * @param value string containing the value of the information to
   * create.
   *
   * @return int providing id to fortran if reusing.
   */
  int addInformation(const char * const key,
                     const char * const value);


  /**
   * Add an attribute that will be inserted into the next grid or grid
   * collection. This will reuse a previously added attribute so that
   * xpointers can be used when writing to file (reducing file size).
   *
   * @param attributeId, returned from a previous addAttribute().
   */
  void addPreviousAttribute(const int attributeId);

  /**
   * Add an information that will be inserted into the next grid or grid
   * collection. This will reuse a previously added information so that
   * xpointers can be used when writing to file (reducing file size).
   *
   * @param informationId, returned from a previous addInformation().
   */
  void addPreviousInformation(const int informationId);

  /**
   * Closes grid collection. No additional grids or collections can be
   * added to the current collection.
   */
  void closeGridCollection();

  /**
   * Set the geometry (point data) that will be added to the next grid.
   *
   * @param geometryType the geometry type.
   * @param numValues number of point values to copy.
   * @param arrayType type of point values.
   * @param pointValues array of point values.
   *
   * @return int providing id to fortran if reusing.
   */
  int setGeometry(const int geometryType, 
                  const unsigned int numValues,
                  const int arrayType, 
                  const void * const pointValues);
  
  /**
   * Set the geometry (point data) that will be added to the next grid.
   * This will reuse a previously set geometry so that xpointers can be
   * used when writing to file (reducing file size);
   *
   * @param geometryId, returned from previous setGeometry()
   */
  void setPreviousGeometry(const int geometryId);

  /**
   * Set the topology (connectivity data) that will be added to the
   * next grid.  This will reuse a previously set topology so that
   * xpointers can be used when writing to file (reducing file size);
   *
   * @param topologyId, returned from previous setTopology()
   */
  void setPreviousTopology(const int topologyId);

  /**
   * Set the time that will be added to the next grid or grid
   * collection.
   *
   * @param time time value to insert.
   */
  void setTime(const double time);
  
  /**
   * Set the topology (connectivity data) that will be added to the
   * next grid.
   *
   * @param topologyType the topology type.
   * @param numValues number of connectivity values to copy.
   * @param arrayType type of connectivity values.
   * @param connectivityValues array of connectivity values.
   *
   * @return int providing id to fortran if reusing.
   */
  int setTopology(const int topologyType, 
                  const unsigned int numValues,
                  const int arrayType,
                  const void * const connectivityValues);

  /**
   * Write constructed file to disk.
   *
   * @param xmlFilePath the path to the xml file to write to.
   */
  void write(const char * const xmlFilePath);

private:
  
  shared_ptr<XdmfDomain>   mDomain;
  shared_ptr<XdmfGeometry> mGeometry;
  shared_ptr<XdmfTime>     mTime;
  shared_ptr<XdmfTopology> mTopology;

  std::vector<shared_ptr<XdmfAttribute> >     mAttributes;
  std::stack<shared_ptr<XdmfGridCollection> > mGridCollections;
  std::vector<shared_ptr<XdmfInformation> >   mInformations;

  std::vector<shared_ptr<XdmfAttribute> > mPreviousAttributes;
  std::vector<shared_ptr<XdmfGeometry> > mPreviousGeometries;
  std::vector<shared_ptr<XdmfInformation> > mPreviousInformations;
  std::vector<shared_ptr<XdmfTopology> > mPreviousTopologies;



};

#endif /* XDMFFORTRAN_HPP_ */
