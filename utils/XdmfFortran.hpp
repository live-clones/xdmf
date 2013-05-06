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
class XdmfArray;
class XdmfMap;
class XdmfSet;
class XdmfCurvilinearGrid;
class XdmfRectilinearGrid;
class XdmfRegularGrid;
class XdmfUnstructuredGrid;
class XdmfHeavyDataWriter;

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

/**
 * Set Type
 */
#define XDMF_SET_TYPE_NODE 601
#define XDMF_SET_TYPE_CELL 602
#define XDMF_SET_TYPE_FACE 603
#define XDMF_SET_TYPE_EDGE 604

/**
 * Grid Type
 */
#define XDMF_GRID_TYPE_CURVILINEAR  701
#define XDMF_GRID_TYPE_RECTILINEAR  702
#define XDMF_GRID_TYPE_REGULAR      703
#define XDMF_GRID_TYPE_UNSTRUCTURED 704


// This works with g77 and gfortran. Different compilers require different
// name mangling.
#if !defined(WIN32)
#define XdmfInit xdmfinit_
#define XdmfClose xdmfclose_
#define XdmfAddAttribute xdmfaddattribute_
#define XdmfAddGrid xdmfaddgrid_

#define XdmfAddGridCurvilinear xdmfaddgridcurvilinear_
#define XdmfAddGridRectilinear xdmfaddgridrectilinear_
#define XdmfAddGridRegular xdmfaddgridregular_

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
#define XdmfSetAllowSetSplitting xdmfsetallowsetsplitting_
#define XdmfSetMaxFileSize xdmfsetmaxfilesize_
#define XdmfWrite xdmfwrite_
#define XdmfRead xdmfread_
#define XdmfWriteHDF5 xdmfwritehdf5_
#define XdmfInitHDF5 xdmfinithdf5_

#define XdmfSetTopologyPolyline xdmfsettopologypolyline_


#define XdmfRetrieveNumDomainGridCollections xdmfretrievenumdomaingridcollections_
#define XdmfRetrieveNumGridCollectionGridCollections xdmfretrievenumgridcollectiongridcollections_
#define XdmfRetrieveDomainTag xdmfretrievedomaintag_
#define XdmfRetrieveDomainNumProperties xdmfretrievedomainnumproperties_
#define XdmfRetrieveDomainProperty xdmfretrievedomainproperty_
#define XdmfRetrieveDomainPropertyByKey xdmfretrievedomainpropertybykey_
#define XdmfOpenDomainGridCollection xdmfopendomaingridcollection_
#define XdmfRemoveDomainGridCollection xdmfremovedomaingridcollection_
#define XdmfOpenGridCollectionGridCollection xdmfopengridcollectiongridcollection_
#define XdmfRemoveGridCollectionGridCollection xdmfremovegricollectiongridcollection_
#define XdmfRetrieveGridCollectionTag xdmfretrievegridcollectiontag_
#define XdmfRetrieveGridCollectionNumProperties xdmfretrievegridcollectionnumproperties_
#define XdmfRetrieveGridCollectionProperty xdmfretrievegridcollectionproperty_
#define XdmfRetrieveGridCollectionPropertyByKey xdmfretrievegridcollectionpropertybykey_
#define XdmfRetrieveGridCollectionNumGrids xdmfretrievegridcollectionnumgrids_
#define XdmfRetrieveDomainNumGrids xdmfretrievedomainnumgrids_
#define XdmfOpenDomainGrid xdmfopendomaingrid_
#define XdmfRemoveDomainGrid xdmfremovedomaingrid_
#define XdmfReplaceDomainGrid xdmfreplacedomaingrid_
#define XdmfRetrieveDomainGridTag xdmfretrievedomaingridtag_
#define XdmfRetrieveDomainGridName xdmfretrievedomaingridname_
#define XdmfRetrieveDomainGridNumProperties xdmfretrievedomaingridnumproperties_
#define XdmfRetrieveDomainGridProperty xdmfretrievedomaingridproperty_
#define XdmfRetrieveDomainGridPropertyByKey xdmfretrievedomaingridpropertybykey_
#define XdmfOpenGridCollectionGrid xdmfopengridcollectiongrid_
#define XdmfRemoveGridCollectionGrid xdmfremovegridcollectiongrid_
#define XdmfReplaceGridCollectionGrid xdmfreplacegridcollectiongrid_
#define XdmfRetrieveGridCollectionGridTag xdmfretrievegridcollectiongridtag_
#define XdmfRetrieveGridCollectionGridName xdmfretrievegridcollectiongridname_
#define XdmfRetrieveGridCollectionGridNumProperties xdmfretrievegridcollectiongridnumproperties_
#define XdmfRetrieveGridCollectionGridProperty xdmfretrievegridcollectiongridproperty_
#define XdmfRetrieveGridCollectionGridPropertyByKey xdmfretrievegridcollectiongridpropertybykey_
#define XdmfRetrieveGridCollectionType xdmfretrievegridcollectiontype_
#define XdmfRetrieveTime xdmfretrievetime_
#define XdmfRetrieveTopologyTag xdmfretrievetopologytag_
#define XdmfRetrieveTopologyType xdmfretrievetopologytype_
#define XdmfRetrieveTopologyValueType xdmfretrievetopologyvaluetype_
#define XdmfRetrieveTopologyValues xdmfretrievetopologyvalues_
#define XdmfRetrieveTopologyNumElements xdmfretrievetopologynumelements_
#define XdmfRetrieveTopologySize xdmfretrievetopologysize_
#define XdmfClearPreviousTopologies xdmfclearprevioustopologies_
#define XdmfRetrieveTopologyNumProperties xdmfretrievetopologynumproperties_
#define XdmfRetrieveTopologyProperty xdmfretrievetopologyproperty_
#define XdmfRetrieveTopologyPropertyByKey xdmfretrievetopologypropertybykey_
#define XdmfRetrieveGeometryTag xdmfretrievegeometrytag_
#define XdmfRetrieveGeometryType xdmfretrievegeometrytype_
#define XdmfRetrieveGeometryValues xdmfretrievegeometryvalues_
#define XdmfRetrieveGeometryValueType xdmfretrievegeometryvaluetype_
#define XdmfRetrieveGeometryNumPoints xdmfretrievegeometrynumpoints_
#define XdmfRetrieveGeometrySize xdmfretrievegeometrysize_
#define XdmfClearPreviousGeometries xdmfclearpreviousgeometries_
#define XdmfRetrieveGeometryNumProperties xdmfretrievegeometrynumproperties_
#define XdmfRetrieveGeometryProperty xdmfretrievegeometryproperty_
#define XdmfRetrieveGeometryPropertyByKey xdmfretrievegeometrypropertybykey_
#define XdmfSetDimensions xdmfsetdimensions_
#define XdmfOpenPreviousDimensions xdmfopenpreviousdimensions_
#define XdmfClearPreviousDimensions xdmfclearpreviousdimensions_
#define XdmfRetrieveDimensionsTag xdmfretrievedimensionstag_
#define XdmfRetrieveDimensionsType xdmfretrievedimensionstype_
#define XdmfRetrieveDimensionsValueType xdmfretrievedimensionsvaluetype_
#define XdmfRetrieveDimensionsValues xdmfretrievedimensionsvalues_
#define XdmfRetrieveDimensionsNumElements xdmfretrievedimensionsnumelements_
#define XdmfRetrieveDimensionsSize xdmfretrievedimensionssize_
#define XdmfRetrieveDimensionsNumProperties xdmfretrievedimensionsnumproperties_
#define XdmfRetrieveDimensionsProperty xdmfretrievedimensionsproperty_
#define XdmfRetrieveDimensionsPropertyByKey xdmfretrievedimensionspropertybykey_
#define XdmfSetOrigin xdmfsetorigin_
#define XdmfSetPreviousOrigin xdmfsetpreviousorigin_
#define XdmfClearPreviousOrigins xdmfclearpreviousorigins_
#define XdmfRetrieveOriginTag xdmfretrieveorigintag_
#define XdmfRetrieveOriginType xdmfretrieveorigintype_
#define XdmfRetrieveOriginValueType xdmfretrieveoriginvaluetype_
#define XdmfRetrieveOriginValues xdmfretrieveoriginvalues_
#define XdmfRetrieveOriginNumElements xdmfretrieveoriginnumelements_
#define XdmfRetrieveOriginSize xdmfretrieveoriginsize_
#define XdmfRetrieveOriginNumProperties xdmfretrieveoriginnumproperties_
#define XdmfRetrieveOriginProperty xdmfretrieveoriginproperty_
#define XdmfRetrieveOriginPropertyByKey xdmfretrieveoriginpropertybykey_
#define XdmfSetBrick xdmfsetbrick_
#define XdmfSetPreviousBrick xdmfsetpreviousbrick_
#define XdmfClearPreviousBricks xdmfclearpreviousbricks_
#define XdmfRetrieveBrickTag xdmfretrievebricktag_
#define XdmfRetrieveBrickType xdmfretrievebricktype_
#define XdmfRetrieveBrickValueType xdmfretrievebrickvaluetype_
#define XdmfRetrieveBrickValues xdmfretrievebrickvalues_
#define XdmfRetrieveBrickNumElements xdmfretrievebricknumelements_
#define XdmfRetrieveBrickSize xdmfretrievebricksize_
#define XdmfRetrieveBrickNumProperties xdmfretrievebricknumproperties_
#define XdmfRetrieveBrickProperty xdmfretrievebrickproperty_
#define XdmfRetrieveBrickPropertyByKey xdmfretrievebrickpropertybykey_
#define XdmfAddMap xdmfaddmap_
#define XdmfRetrieveNumMaps xdmfretrievenummaps_
#define XdmfAddRemoteNodeID xdmfaddremotenodeid_
#define XdmfRetrieveRemoteNodeIDs xdmfretrieveremotenodeids_
#define XdmfRetrieveNumRemoteNodeIDs xdmfretrievenumremotenodeids_
#define XdmfRemoveMap xdmfremovemap_
#define XdmfClearMaps xdmfclearmaps_
#define XdmfStoreMap xdmfstoremap_
#define XdmfAddPreviousMap xdmfaddpreviousmap_
#define XdmfClearPreviousMaps xdmfclearpreviousmaps_
#define XdmfRetrieveMapNumProperties xdmfretrievemapnumproperties_
#define XdmfRetrieveMapProperty xdmfretrievemapproperty_
#define XdmfRetrieveMapPropertyByKey xdmfretrievemappropertybykey_
#define XdmfRetrieveNumAttributes xdmfretrievenumattributes_
#define XdmfClearAttributes xdmfclearattributes_
#define XdmfRemoveAttribute xdmfremoveattribute_
#define XdmfReplaceAttribute xdmfreplaceattribute_
#define XdmfOpenAttribute xdmfopenattribute_
#define XdmfRetrieveAttributeTag xdmfretrieveattributetag_
#define XdmfRetrieveAttributeName xdmfretrieveattributename_
#define XdmfRetrieveAttributeType xdmfretrieveattributetype_
#define XdmfRetrieveAttributeCenter xdmfretrieveattributecenter_
#define XdmfRetrieveAttributeValues xdmfretrieveattributevalues_
#define XdmfRetrieveAttributeValueType xdmfretrieveattributevaluetype_
#define XdmfRetrieveAttributeSize xdmfretrieveattributesize_
#define XdmfClearPreviousAttributes xdmfclearpreviousattributes_
#define XdmfRetrieveAttributeNumProperties xdmfretrieveattributenumproperties_
#define XdmfRetrieveAttributeProperty xdmfretrieveattributeproperty_
#define XdmfRetrieveAttributePropertyByKey xdmfretrieveattributepropertybykey_
#define XdmfAddCoordinate xdmfaddcoordinate_
#define XdmfAddPreviousCoordinate xdmfaddpreviouscoordinate_
#define XdmfClearPreviousCoordinates xdmfclearpreviouscoordinates_
#define XdmfRetrieveNumCoordinates xdmfretrievenumcoordinates_
#define XdmfRemoveCoordinate xdmfremovecoordinate_
#define XdmfReplaceCoordinate xdmfreplacecoordinate_
#define XdmfRetrieveCoordinateTag xdmfretrievecoordinatetag_
#define XdmfRetrieveCoordinateName xdmfretrievecoordinatename_
#define XdmfRetrieveCoordinateValues xdmfretrievecoordinatevalues_
#define XdmfRetrieveCoordinateValueType xdmfretrievecoordinatevaluetype_
#define XdmfRetrieveCoordinateSize xdmfretrievecoordinatesize_
#define XdmfClearCoordinates xdmfclearcoordinates_
#define XdmfRetrieveCoordinateNumProperties xdmfretrievecoordinatenumproperties_
#define XdmfRetrieveCoordinateProperty xdmfretrievecoordinateproperty_
#define XdmfRetrieveCoordinatePropertyByKey xdmfretrievecoordinatepropertybykey_
#define XdmfRetrieveSetTag xdmfretrievesettag_
#define XdmfRetrieveSetName xdmfretrievesetname_
#define XdmfRetrieveSetType xdmfretrievesettype_
#define XdmfAddSet xdmfaddset_
#define XdmfAddPreviousSet xdmfaddpreviousset_
#define XdmfClearPreviousSets xdmfclearprevioussets_
#define XdmfClearSets xdmfclearsets_
#define XdmfRetrieveNumSets xdmfretrievenumsets_
#define XdmfRetrieveSetSize xdmfretrievesetsize_
#define XdmfRetrieveSetValues xdmfretrievesetvalues_
#define XdmfRetrieveSetValueType xdmfretrievesetvaluetype_
#define XdmfOpenSet xdmfopenet_
#define XdmfRemoveSet xdmfremoveset_
#define XdmfReplaceSet xdmfreplaceset_
#define XdmfRetrieveSetNumProperties xdmfretrievesetnumproperties_
#define XdmfRetrieveSetProperty xdmfretrievesetproperty_
#define XdmfRetrieveSetPropertyByKey xdmfretrievesetpropertybykey_
#define XdmfRetrieveNumInformation xdmfretrievenuminformation_
#define XdmfRetrieveInformationTag xdmfretrieveinformationtag_
#define XdmfClearInformations xdmfclearinformations_
#define XdmfRetrieveInformation xdmfretrieveinformation_
#define XdmfRemoveInformation xdmfremoveinformation_
#define XdmfReplaceInformation xdmfreplaceinformation_
#define XdmfOpenInformation xdmfopeninformation_
#define XdmfRetrieveInformationByKey xdmfretrieveinformationbykey_
#define XdmfRemoveInformationByKey xdmfremoveinformationbykey_
#define XdmfReplaceInformationByKey xdmfreplaceinformationbykey_
#define XdmfClearPreviousInformation xdmfclearpreviousinformation_
#define XdmfRetrieveInformationNumProperties xdmfretrieveinformationnumproperties_
#define XdmfRetrieveInformationProperty xdmfretrieveinformationproperty_
#define XdmfRetrieveInformationPropertyByKey xdmfretrieveinformationpropertybykey_
#define XdmfClearPrevious xdmfclearprevious_
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
   * Add unstructured grid to domain or collection. Inserts geometry, topology,
   * attributes, and informations into grid. If no geometry or
   * topology is set, an error is generated.
   * The top of parentAttributes, parentInformations, and parentSets
   * are placed in mAttributes, mInformations, and mSets
   *
   * @param 	name		name of the grid.
   * @param 	writeToHDF5	whether to write the grid to hdf5 after adding it
   */
  void addGrid(const char * const name, const bool writeToHDF5);

  /**
   * Add curvilinear grid to domain or collection. Inserts geometry, dimensions,
   * attributes, and informations into grid. If no geometry or
   * dimensions are set, an error is generated.
   * The top of parentAttributes, parentInformations, and parentSets
   * are placed in mAttributes, mInformations, and mSets
   *
   * @param 	name 	name of the grid.
   * @param     writeToHDF5     whether to write the grid to hdf5 after adding it
   */
  void addGridCurvilinear(const char * const name, const bool writeToHDF5);

  /**
   * Add rectilinear grid to domain or collection. Inserts coordinate,
   * attributes, and informations into grid. If no geometry or
   * topology is set, an error is generated.
   * The top of parentAttributes, parentInformations, and parentSets
   * are placed in mAttributes, mInformations, and mSets
   *
   * @param 	name 	name of the grid.
   * @param     writeToHDF5     whether to write the grid to hdf5 after adding it
   */
  void addGridRectilinear(const char * const name, const bool writeToHDF5);

  /**
   * Add rectilinear grid to domain or collection. Inserts origin, brick, dimensions,
   * attributes, and informations into grid. If no geometry or
   * topology is set, an error is generated.
   * The top of parentAttributes, parentInformations, and parentSets
   * are placed in mAttributes, mInformations, and mSets
   *
   * @param 	name 	name of the grid.
   * @param     writeToHDF5     whether to write the grid to hdf5 after adding it
   */
  void addGridRegular(const char * const name, const bool writeToHDF5);

  /**
   * Add grid collection to domain or collection. Inserts attributes
   * and informations into collection.
   * The top of parentAttributes, parentInformations, and parentSets
   * are placed in mAttributes, mInformations, and mSets
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
   *
   * @param     writeToHDF5     whether to write the grid to hdf5 after adding it
   */
  void closeGridCollection(const bool writeToHDF5);

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
                  const void * const connectivityValues,
                  const int numNodes);





  /**
   * Returns the number of grid collections currently
   * contained in the domain
   *
   * @return	The number of grid collections
   */
  int retrieveNumDomainGridCollections();

  /**
   * Returns the number of grid collections currently
   * contained in the grid collection on top of mGridCollections
   *
   * @return	The number of grid collections
   */
  int numGridCollectionGridCollections();

  /**
   * Fills the provided character pointer with the value of the Domain's tag
   *
   * @param	tag		the pointer to the point where the string will be written
   * @param	tagLength	the size of the array assigned at the tag pointer
   */
  void retrieveDomainTag(char * tag, int tagLength);

  /**
   * Returns the number of properties contained within the domain.
   *
   * @return	the number of properties that the domain has.
   */
  int retrieveDomainNumProperties();

  /**
   * Fills the key and length from the property at the index specified.
   * Since this refers to a map it is not completely reliable that the values will 
   * stay at the same indexes when new values are added.
   * Using retrieve by key is more reliable.
   *
   * Throws an error if the index is out of bounds
   *
   * @param 	index		The index of the property to fill from
   * @param	key		The pointer to the location where the key will be written
   * @param	keyLength	The size of the array assigned to the key pointer
   * @param	value		The pointer to the location where the value will be written
   * @param	valueLength	The size of the array assigned to the value pointer
   */
  void retrieveDomainProperty(int index, char * key, int keyLength, char * value, int valueLength);

  /**
   * Searches the properties of the domain for one with the specified key.
   * Stores the value found at a provided pointer.
   * 
   * Throws an error if the key does not match any values
   * 
   * @param	key		A pointer to the key being searched for
   * @param	value		A pointer to the location where the value will be written
   * @param	valueLength	The size of the Array assigned to the value pointer
   */
  void retrieveDomainPropertyByKey(char * key, char * value, int valueLength);

  /**
   * Makes the specified grid collection owned by the domain accessable
   * by adding it to mGridCollections.
   * Pushes the previous contents of mAttributes, mInformations, and mSets
   * onto parentAttributes, parentInformations, and parentSets
   *
   * Returns an error if the index is out of bounds
   *
   * @param	index		the index of the grid collection in the domain that is to be opened
   * @param	openMaps	set to 1 to open maps
   * @param	openAttributes	set to 1 to open attributes
   * @param	openInformation	set to 1 to open information
   * @param	openSets	set to 1 to open sets
   */
  void openDomainGridCollection(int index, int openMaps, int openAttributes, int openInformation, int openSets);

  /**
   * Removes the specifed grid collection from mDomain's grid collections
   *
   * Throws an error if the index is out of bounds
   *
   * @param	index	The index of the grid collection to be removed
   */
  void removeDomainGridCollection(int index);

  /**
   * Makes the specified grid collection owned by the grid collection on top of mGridCollections
   * accessable by adding it to mGridCollections.
   * Pushes the previous contents of mAttributes, mInformations, and mSets
   * onto parentAttributes, parentInformations, and parentSets
   *
   * Returns an error if the index is out of bounds
   *
   * @param	index	the index of the grid collection in the domain that is to be opened
   * @param	openMaps	set to 1 to open maps
   * @param	openAttributes	set to 1 to open attributes
   * @param	openInformation	set to 1 to open information
   * @param	openSets	set to 1 to open sets
   */
  void openGridCollectionGridCollection(int index, int openMaps, int openAttributes, int openInformation, int openSets);

  /**
   * Removes the specifed grid collection from the
   * grid collections owned by the grid collection on top of mGridCollections
   *
   * Throws an error if the index is out of bounds
   *
   * @param	index	The index of the grid collection to be removed
   */
  void removeGridCollectionGridCollection(int index);

  /**
   * Fills the provided character pointer with the value of the name of the grid collection on top of mGridCollections
   *
   * @param	name		the pointer to the point where the string will be written
   * @param	nameLength	the size of the array assigned at the name pointer
   */
  void retrieveGridCollectionName(char * name, int nameLength);

  /**
   * Fills the provided character pointer with the value of the tag of the grid collection on top of mGridCollections
   *
   * @param	tag		the pointer to the point where the string will be written
   * @param	tagLength	the size of the array assigned at the tag pointer
   */
  void retrieveGridCollectionTag(char * tag, int tagLength);

  /**
   * Returns the number of properties contained within the grid collection on top of mGridCollections.
   *
   * @return	the number of properties that the grid collection has.
   */
  int retrieveGridCollectionNumProperties();

  /**
   * Fills the key and length from the property at the index specified.
   * Since this refers to a map it is not completely reliable that the values will 
   * stay at the same indexes when new values are added.
   * Using retrieve by key is more reliable.
   *
   * Throws an error if the index is out of bounds
   *
   * @param 	index		The index of the property to fill from
   * @param	key		The pointer to the location where the key will be written
   * @param	keyLength	The size of the array assigned to the key pointer
   * @param	value		The pointer to the location where the value will be written
   * @param	valueLength	The size of the array assigned to the value pointer
   */
  void retrieveGridCollectionProperty(int index, char * key, int keyLength, char * value, int valueLength);

  /**
   * Searches the properties of the Grid Collection on top of mGridCollections for one with the specified key.
   * Stores the value found at a provided pointer.
   * 
   * Throws an error if the key does not match any values
   * 
   * @param	key		A pointer to the key being searched for
   * @param	value		A pointer to the location where the value will be written
   * @param	valueLength	The size of the Array assigned to the value pointer
   */
  void retrieveGridCollectionPropertyByKey(char * key, char * value, int valueLength);

  /**
   * Opens a grid of the specified type from the domain by
   * placing its topography, geometry, time, etc..
   * into the appropriate containers
   *
   * Returns an error if the index is out of bounds
   *
   * @param 	gridType	the type of the grid expressed as an integer, Ex: XDMF_GRID_TYPE_UNSTRUCTURED
   * @param 	index		the index of the grid to be opened
   * @param	openMaps	set to 1 to open maps
   * @param	openAttributes	set to 1 to open attributes
   * @param	openInformation	set to 1 to open information
   * @param	openSets	set to 1 to open sets
   */
  void openDomainGrid(int gridType, int index, int openMaps, int openAttributes, int openInformation, int openSets);

  /**
   * Removes a grid of the specified type from the domain
   *
   * Returns an error if the index is out of bounds
   *
   * @param 	gridType	the type of the grid expressed as an integer, Ex: XDMF_GRID_TYPE_UNSTRUCTURED
   * @param 	index		the index of the grid to be opened
   */
  void removeDomainGrid(int gridType, int index);

  /**
   * Replaces a grid of the specified type from the domain by
   * placing the appropriate data into the specified grid
   *
   * Returns an error if the index is out of bounds
   *
   * @param 	gridType	the type of the grid expressed as an integer, Ex: XDMF_GRID_TYPE_UNSTRUCTURED
   * @param 	index		the index of the grid to be opened
   * @param 	name		a pointer to the name of the grid replacing the indicated grid
   */
  void replaceDomainGrid(int gridType, int index, char * name);

  /**
   * Retrieves the name of the specified grid and places it at the locaiton provided.
   *
   * Returns an error if the index is out of bounds
   *
   * @param	gridType	The of the specified grid represented as an integer, Ex: XDMF_GRID_TYPE_UNSTRUCTURED
   * @param	index		The index of the specified grid
   * @param	name		The pointer to where the string will be written
   * @param	nameLength	The size of the array assigned to the name pointer
   */
  void retrieveDomainGridName(int gridType, int index, char * name, int nameLength);

  /**
   * Retrieves the tag of the specified grid and places it at the location provided.
   *
   * Returns an error if the index is out of bounds
   *
   * @param	gridType	The of the specified grid represented as an integer, Ex: XDMF_GRID_TYPE_UNSTRUCTURED
   * @param	index		The index of the specified grid
   * @param	tag		The pointer to the point where the string will be written
   * @param	tagLength	The size of the array assigned at the tag pointer
   */
  void retrieveDomainGridTag(int gridType, int index, char * tag, int tagLength);

  /**
   * Retrieves the number of properties that the specified grid has.
   *
   * Returns an error if the index is out of bounds
   *
   * @param	gridType	The of the specified grid represented as an integer, Ex: XDMF_GRID_TYPE_UNSTRUCTURED
   * @param	index		The index of the specified grid
   *
   * @return			The number of properties that thte grid has.
   */
  int retrieveDomainGridNumProperties(int gridType, int index);

  /**
   * Retrieves the key and value of the property at the specified index
   * in the properties of the grid at the specified index.
   *
   * Since the properties are stored in a map the location of individual pairs are mutable.
   * Use retrieve by key for more oonsistent results.
   *
   * Returns an error if the index is out of bounds
   *
   * @param	gridType	The of the specified grid represented as an integer, Ex: XDMF_GRID_TYPE_UNSTRUCTURED
   * @param 	gridIndex	The index of the specified grid
   * @param 	index		The index of the property to fill from
   * @param	key		The pointer to the location where the key will be written
   * @param	keyLength	The size of the array assigned to the key pointer
   * @param	value		The pointer to the location where the value will be written
   * @param	valueLength	The size of the array assigned to the value pointer
   */
  void retrieveDomainGridProperty(int gridType, int gridIndex, int index, char * key, int keyLength, char * value, int valueLength);

  /**
   * Searches for the property that corresponds with the supplied key
   * in the properties of the grid at the specified index.
   *
   * Returns an error if the index is out of bounds
   * Returns an error if the key has no matching value
   *
   * @param	gridType	The of the specified grid represented as an integer, Ex: XDMF_GRID_TYPE_UNSTRUCTURED
   * @param	index		The index of the specified grid
   * @param	key		A pointer to the key being searched for
   * @param	value		A pointer to the location where the value will be written
   * @param	valueLength	The size of the Array assigned to the value pointer
   */
  void retrieveDomainGridPropertyByKey(int gridType, int index, char * key, char * value, int valueLength);

  /**
   * Opens a grid of the specified type from the grid collection
   * on top of mGridCollections by placing its
   * topography, geometry, time, etc.. into the appropriate 
   * containers
   *
   * Returns an error if the index is out of bounds
   *
   * @param 	gridType	the type of the grid expressed as an integer, Ex: XDMF_GRID_TYPE_UNSTRUCTURED
   * @param 	index		the index of the grid to be opened
   * @param	openMaps	set to 1 to open maps
   * @param	openAttributes	set to 1 to open attributes
   * @param	openInformation	set to 1 to open information
   * @param	openSets	set to 1 to open sets
   */
  void openGridCollectionGrid(int gridType, int index, int openMaps, int openAttributes, int openInformation, int openSets);

  /**
   * Removes a grid of the specified type from the grid collection
   * on top of mGridCollections
   *
   * Returns an error if the index is out of bounds
   *
   * @param 	gridType	the type of the grid expressed as an integer, Ex: XDMF_GRID_TYPE_UNSTRUCTURED
   * @param 	index		the index of the grid to be opened
   */
  void removeGridCollectionGrid(int gridType, int index);

  /**
   * Replaces a grid of the specified type from the grid collection
   * on top of mGridCollections by placing the appropriate data into it
   *
   * Returns an error if the index is out of bounds
   *
   * @param 	gridType	the type of the grid expressed as an integer, Ex: XDMF_GRID_TYPE_UNSTRUCTURED
   * @param 	index		the index of the grid to be opened
   * @param	name		a pointer to the name of the grid to be replacing the specified one
   */
  void replaceGridCollectionGrid(int gridType, int index, char * name);

  /**
   * Retrieves the name of the specified grid and places it at the locaiton provided.
   *
   * Returns an error if the index is out of bounds
   *
   * @param 	gridType	the type of the grid expressed as an integer, Ex: XDMF_GRID_TYPE_UNSTRUCTURED
   * @param	index		The index of the specified grid
   * @param	name		The pointer to where the string will be written
   * @param	nameLength	The size of the array assigned to the name pointer
   */
  void retrieveGridCollectionGridName(int gridType, int index, char * name, int nameLength);

  /**
   * Retrieves the tag of the specified grid and places it at the location provided.
   *
   * Returns an error if the index is out of bounds
   *
   * @param 	gridType	the type of the grid expressed as an integer, Ex: XDMF_GRID_TYPE_UNSTRUCTURED
   * @param	index		The index of the specified grid
   * @param	tag		The pointer to the point where the string will be written
   * @param	tagLength	The size of the array assigned at the tag pointer
   */
  void retrieveGridCollectionGridTag(int gridType, int index, char * tag, int tagLength);

  /**
   * Retrieves the number of properties that the specified grid has.
   *
   * Returns an error if the index is out of bounds
   *
   * @param 	gridType	the type of the grid expressed as an integer, Ex: XDMF_GRID_TYPE_UNSTRUCTURED
   * @param	index	The index of the specified grid
   *
   * @return		The number of properties that thte grid has.
   */
  int retrieveGridCollectionGridNumProperties(int gridType, int index);

  /**
   * Retrieves the key and value of the property at the specified index
   * in the properties of the grid at the specified index.
   *
   * Since the properties are stored in a map the location of individual pairs are mutable.
   * Use retrieve by key for more oonsistent results.
   *
   * Returns an error if the index is out of bounds
   *
   * @param 	gridType	the type of the grid expressed as an integer, Ex: XDMF_GRID_TYPE_UNSTRUCTURED
   * @param 	GridIndex	The index of the specified grid
   * @param 	index		The index of the property to fill from
   * @param	key		The pointer to the location where the key will be written
   * @param	keyLength	The size of the array assigned to the key pointer
   * @param	value		The pointer to the location where the value will be written
   * @param	valueLength	The size of the array assigned to the value pointer
   */
  void retrieveGridCollectionGridProperty(int gridType, int gridIndex, int index,
	char * key, int keyLength, char * value, int valueLength);

  /**
   * Searches for the property that corresponds with the supplied key
   * in the properties of the grid at the specified index.
   *
   * Returns an error if the index is out of bounds
   * Returns an error if the key has no matching value
   *
   * @param 	gridType	the type of the grid expressed as an integer, Ex: XDMF_GRID_TYPE_UNSTRUCTURED
   * @param	index		The index of the specified grid
   * @param	key		A pointer to the key being searched for
   * @param	value		A pointer to the location where the value will be written
   * @param	valueLength	The size of the Array assigned to the value pointer
   */
  void retrieveGridCollectionGridPropertyByKey(int gridType, int index, char * key, char * value, int valueLength);

  /**
   * Returns the number of grids of a specified type that the domain contains
   *
   * @param	gridType	The specified type of grid as an integer. Ex: XDMF_GRID_TYPE_UNSTRUCTURED
   *
   * @return			The number of grids
   */
  int numDomainGrids(int gridType);

  /**
   * Returns the number of grids of a specified type that
   * the top grid collection in mGridCollections contains
   *
   * Returns an error if there are no grid collections in mGridCollections
   *
   * @param	gridType	The specified type of grid as an integer. Ex: XDMF_GRID_TYPE_UNSTRUCTURED
   *
   * @return			The number of grids
   */
  int numGridCollectionGrids(int gridType);

  /**
   * Returns the type of the grid collection on top of the mGridCollections stack
   *
   * @return	the grid collection's type, EX: XDMF_GRID_COLLECTION_TYPE_TEMPORAL
   */
  int retrieveGridCollectionType();

  /**
   * Returns the time stored in mTime
   *
   * @return	the value of mTime as a float
   */
  float retrieveTime();

  /**
   * Retrieves the Geometry's tag and stores it into the provided pointer
   *
   * @param	tag		The location where the tag will be stored
   * @param	tagLength	The size fo the array at the provided pointer
   */
  void retrieveGeometryTag(char * tag, int tagLength);

  /**
   * Returns the geometry's type as an integer.
   *
   * Returns an error if it doesn't recognize the type or if the geometry isn't set
   *
   * @return	the geometry's type, EX: XDMF_GEOMETRY_TYPE_XY
   */
  int retrieveGeometryType();

  /**
   * Returns the geometry's datatype as an integer.
   *
   * Returns an error if it doesn't recognize the type or if the geometry isn't set
   *
   * @return	the geometry's type, EX: XDMF_ARRAY_TYPE_INT8
   */
  int retrieveGeometryValueType();

  /**
   * Returns a pointer to the array of values contained in the geometry
   *
   * Returns an error if the geometry is not set
   *
   * @param	values		a void pointer to the array to recieve the geometry's values
   * @param	dataType	an integer corresponding to the datatype that the data will be stored in
   * @param 	numberRead	the number of values read into the values array
   * @param 	startIndex	the place to start reading from the attribute's array
   * @param 	arrayStride	the distance between values read (1 reads all values, 2 reads every other, ect..)
   * @param	valueStride	the distance between the places that the read values are placed in the supplied array
   */
  void retrieveGeometryValues(void * values, int dataType,
	int numberRead, int startIndex, int arrayStride, int valueStride);

  /**
   * returns the number of points contained within the geometry
   *
   * @return	the number of points
   */
  int retrieveGeometryNumPoints();

  /**
   * returns the size of the geometry
   *
   * @return	the size
   */
  int retrieveGeometrySize();

  /**
   * Clears mPreviousGeometries. Use to reduce memory load.
   */
  void clearPreviousGeometries();

  /**
   * Returns the number of properties currently stored in the geometry.
   *
   * @return		the number of properties associated with the geometry
   */
  int retrieveGeometryNumProperties();

  /**
   * Retrieves the key and value of the property at the specifed index of the Geometry
   * and modifies the values at the provided pointers
   *
   * Returns an error if the index is out of bounds
   *
   * @param 	index			The index of the property that is being retrieved
   * @param	key			A pointer to the location where the key value will be stored
   * @param	keyLength		the size of the key variable
   * @param	value			A pointer to the location where the value of the property will be stored
   * @param	valueLength		the size of the value variable
   */
  void retrieveGeometryProperty(int index, char * key, int keyLength, char * value, int valueLength);

  /**
   * Retrieves the value of a property of the geometry when given its key.
   *
   * Returns an error if the key has no matching value
   *
   * @param 	key		A pointer to the value of the key that is being searched for
   * @param	value		A pointer to the location where the value of the property will be stored
   * @param	valueLength	the size of the value variable
   */
  void retrieveGeometryPropertyByKey(char * key, char * value, int valueLength);

  /**
   * Retrieves the topology's tag and stores it into the provided pointer
   *
   * @param	tag		The location where the tag will be stored
   * @param	tagLength	The size fo the array at the provided pointer
   */
  void retrieveTopologyTag(char * tag, int tagLength);

  /**
   * Returns the topology's type as an integer.
   *
   * Returns an error if it doesn't recognize the type or if the topology isn't set
   *
   * @return	the topology's type, EX: XDMF_TOPOLOGY_TYPE_POLYVERTEX
   */
  int retrieveTopologyType();

  /**
   * Returns the topology's datatype as an integer.
   *
   * Returns an error if it doesn't recognize the type or if the topology isn't set
   *
   * @return	the topology's type, EX: XDMF_ARRAY_TYPE_INT8
   */
  int retrieveTopologyValueType();

  /**
   * Returns a pointer to the array of values contained in the topology
   *
   * Returns an error if the topology is not set
   *
   * @param	values		a void pointer to the array to recieve the topology's values
   * @param	dataType	an integer corresponding to the datatype that the data will be stored in
   * @param 	numberRead	the number of values read into the values array
   * @param 	startIndex	the place to start reading from the attribute's array
   * @param 	arrayStride	the distance between values read (1 reads all values, 2 reads every other, ect..)
   * @param	valueStride	the distance between the places that the read values are placed in the supplied array
   */
  void retrieveTopologyValues(void * values, int dataType,
	int numberRead, int startIndex, int arrayStride, int valueStride);

  /**
   * returns the number of elements contained within the topology
   *
   * @return	the number of elements
   */
  int retrieveTopologyNumElements();

  /**
   * returns the size of the topology
   *
   * @return	the size
   */
  int retrieveTopologySize();

  /**
   * Clears mPreviousTopologies. Use to reduce memory load.
   */
  void clearPreviousTopologies();

  /**
   * Returns the number of properties currently stored in the topology.
   *
   * @return		the number of properties associated with the topology
   */
  int retrieveTopologyNumProperties();

  /**
   * Retrieves the key and value of the property at the specifed index of the Topology
   * and modifies the values at the provided pointers
   *
   * Returns an error if the index is out of bounds
   *
   * @param 	index			The index of the property that is being retrieved
   * @param	key			A pointer to the location where the key value will be stored
   * @param	keyLength		the size of the key variable
   * @param	value			A pointer to the location where the value of the property will be stored
   * @param	valueLength		the size of the value variable
   */
  void retrieveTopologyProperty(int index, char * key, int keyLength, char * value, int valueLength);

  /**
   * Retrieves the value of a property of the topology when given its key.
   *
   * Returns an error if the key has no matching value
   *
   * @param 	key		A pointer to the value of the key that is being searched for
   * @param	value		A pointer to the location where the value of the property will be stored
   * @param	valueLength	the size of the value variable
   */
  void retrieveTopologyPropertyByKey(char * key, char * value, int valueLength);

  /**
   * Set the dimensions that will be added to the next grid.
   *
   * @param numValues number of point values to copy.
   * @param arrayType type of point values.
   * @param pointValues array of point values.
   *
   * @return int providing id to fortran if reusing.
   */
  int setDimensions(int numValues, const int arrayType, void * pointValues);

  /**
   * Replaces mDimensions with the specified dimensions from mPreviousDimensions
   *
   * Returns an error if the index is out of range
   *
   * @param	index	The index of the specified dimension
   */
  void openPreviousDimensions(int index);

  /**
   * Clears mPreviousDimensions. Use to reduce memory load.
   */
  void clearPreviousDimensions();

  /**
   * Retrieves the dimensions' tag and stores it into the provided pointer
   *
   * @param	tag		The location where the tag will be stored
   * @param	tagLength	The size fo the array at the provided pointer
   */
  void retrieveDimensionsTag(char * tag, int tagLength);

  /**
   * Returns the dimensions' datatype as an integer.
   *
   * Returns an error if it doesn't recognize the type or if the dimension isn't set
   *
   * @return	the dimensions' type, EX: XDMF_ARRAY_TYPE_INT8
   */
  int retrieveDimensionsValueType();

  /**
   * Returns a pointer to the array of values contained in the dimensions
   *
   * Returns an error if the dimensions are not set
   *
   * @param	values		a void pointer to the array to recieve the geometry's values
   * @param	dataType	an integer corresponding to the datatype that the data will be stored in
   * @param 	numberRead	the number of values read into the values array
   * @param 	startIndex	the place to start reading from the attribute's array
   * @param 	arrayStride	the distance between values read (1 reads all values, 2 reads every other, ect..)
   * @param	valueStride	the distance between the places that the read values are placed in the supplied array
   */
  void retrieveDimensionsValues(void * values, int dataType,
	int numberRead, int startIndex, int arrayStride, int valueStride);

  /**
   * returns the size of the dimensions
   *
   * @return	the size
   */
  int retrieveDimensionsSize();

  /**
   * Returns the number of properties currently stored in the dimensions.
   *
   * @return		the number of properties associated with the dimensions
   */
  int retrieveDimensionsNumProperties();

  /**
   * Retrieves the key and value of the property at the specifed index of the dimensions
   * and modifies the values at the provided pointers
   *
   * Returns an error if the index is out of bounds
   *
   * @param 	index			The index of the property that is being retrieved
   * @param	key			A pointer to the location where the key value will be stored
   * @param	keyLength		the size of the key variable
   * @param	value			A pointer to the location where the value of the property will be stored
   * @param	valueLength		the size of the value variable
   */
  void retrieveDimensionsProperty(int index, char * key, int keyLength, char * value, int valueLength);

  /**
   * Retrieves the value of a property of the dimensions when given its key.
   *
   * Returns an error if the key has no matching value
   *
   * @param 	key		A pointer to the value of the key that is being searched for
   * @param	value		A pointer to the location where the value of the property will be stored
   * @param	valueLength	the size of the value variable
   */
  void retrieveDimensionsPropertyByKey(char * key, char * value, int valueLength);

  /**
   * Set the origin that will be added to the next grid.
   *
   * @param numValues number of point values to copy.
   * @param arrayType type of point values.
   * @param pointValues array of point values.
   *
   * @return int providing id to fortran if reusing.
   */
  int setOrigin(int numValues, const int arrayType, void * pointValues);

  /**
   * Sets mOrigin to a specified origin in mPreviousOrigins
   *
   * @param	index	The index of the specified origin
   */
  void setPreviousOrigin(int index);
  
  /**
   * Clears mPreviousOrigins. Use to reduce memory load.
   */
  void clearPreviousOrigins();

  /**
   * Retrieves the origin's tag and stores it into the provided pointer
   *
   * @param	tag		The location where the tag will be stored
   * @param	tagLength	The size fo the array at the provided pointer
   */
  void retrieveOriginTag(char * tag, int tagLength);

  /**
   * Returns the origin's datatype as an integer.
   *
   * Returns an error if it doesn't recognize the type or if the origin isn't set
   *
   * @return	the origin's type, EX: XDMF_ARRAY_TYPE_INT8
   */
  int retrieveOriginValueType();

  /**
   * Returns a pointer to the array of values contained in the origin
   *
   * Returns an error if the origin is not set
   *
   * @param	values		a void pointer to the array to recieve the geometry's values
   * @param	dataType	an integer corresponding to the datatype that the data will be stored in
   * @param 	numberRead	the number of values read into the values array
   * @param 	startIndex	the place to start reading from the attribute's array
   * @param 	arrayStride	the distance between values read (1 reads all values, 2 reads every other, ect..)
   * @param	valueStride	the distance between the places that the read values are placed in the supplied array
   */
  void retrieveOriginValues(void * values, int dataType,
	int numberRead, int startIndex, int arrayStride, int valueStride);

  /**
   * returns the size of the origin
   *
   * @return	the size
   */
  int retrieveOriginSize();

  /**
   * Returns the number of properties currently stored in the origin.
   *
   * @return		the number of properties associated with the origin
   */
  int retrieveOriginNumProperties();

  /**
   * Retrieves the key and value of the property at the specifed index of the origin
   * and modifies the values at the provided pointers
   *
   * Returns an error if the index is out of bounds
   *
   * @param 	index			The index of the property that is being retrieved
   * @param	key			A pointer to the location where the key value will be stored
   * @param	keyLength		the size of the key variable
   * @param	value			A pointer to the location where the value of the property will be stored
   * @param	valueLength		the size of the value variable
   */
  void retrieveOriginProperty(int index, char * key, int keyLength, char * value, int valueLength);

  /**
   * Retrieves the value of a property of the origin when given its key.
   *
   * Returns an error if the key has no matching value
   *
   * @param 	key		A pointer to the value of the key that is being searched for
   * @param	value		A pointer to the location where the value of the property will be stored
   * @param	valueLength	the size of the value variable
   */
  void retrieveOriginPropertyByKey(char * key, char * value, int valueLength);

  /**
   * Set the brick size that will be added to the next grid.
   *
   * @param numValues number of point values to copy.
   * @param arrayType type of point values.
   * @param pointValues array of point values.
   *
   * @return int providing id to fortran if reusing.
   */
  int setBrick(int numValues, const int arrayType, void * pointValues);

  /**
   * Sets the brick size to a specified brick size stored in mPreviousBricks
   *
   * @param	index	The index of the specifed brick size
   */
  void setPreviousBrick(int index);

  /**
   * Clears mPreviousBricks. Use to reduce memory load.
   */
  void clearPreviousBricks();

  /**
   * Retrieves the Geometry's tag and stores it into the provided pointer
   *
   * @param	tag		The location where the tag will be stored
   * @param	tagLength	The size fo the array at the provided pointer
   */
  void retrieveBrickTag(char * tag, int tagLength);

  /**
   * Returns the brick's datatype as an integer.
   *
   * Returns an error if it doesn't recognize the type or if the brick isn't set
   *
   * @return	the brick's type, EX: XDMF_ARRAY_TYPE_INT8
   */
  int retrieveBrickValueType();

  /**
   * Returns a pointer to the array of values contained in the brick
   *
   * Returns an error if the brick is not set
   *
   * @param	values		a void pointer to the array to recieve the brick's values
   * @param	dataType	an integer corresponding to the datatype that the data will be stored in
   * @param 	numberRead	the number of values read into the values array
   * @param 	startIndex	the place to start reading from the attribute's array
   * @param 	arrayStride	the distance between values read (1 reads all values, 2 reads every other, ect..)
   * @param	valueStride	the distance between the places that the read values are placed in the supplied array
   */
  void retrieveBrickValues(void * values, int dataType,
	int numberRead, int startIndex, int arrayStride, int valueStride);

  /**
   * returns the size of the brick size
   *
   * @return	the size
   */
  int retrieveBrickSize();

  /**
   * Returns the number of properties currently stored in the brick size.
   *
   * @return		the number of properties associated with the brick
   */
  int retrieveBrickNumProperties();

  /**
   * Retrieves the key and value of the property at the specifed index of the brick size
   * and modifies the values at the provided pointers
   *
   * Returns an error if the index is out of bounds
   *
   * @param 	index			The index of the property that is being retrieved
   * @param	key			A pointer to the location where the key value will be stored
   * @param	keyLength		the size of the key variable
   * @param	value			A pointer to the location where the value of the property will be stored
   * @param	valueLength		the size of the value variable
   */
  void retrieveBrickProperty(int index, char * key, int keyLength, char * value, int valueLength);

  /**
   * Retrieves the value of a property of the brick size when given its key.
   *
   * Returns an error if the key has no matching value
   *
   * @param 	key		A pointer to the value of the key that is being searched for
   * @param	value		A pointer to the location where the value of the property will be stored
   * @param	valueLength	the size of the value variable
   */
  void retrieveBrickPropertyByKey(char * key, char * value, int valueLength);

  /**
   * Pushes a blank map onto mMaps.
   *
   * @param	name	A pointer to the name of the map to be added
   */
  void addMap(char * name);

  /**
   * Returns the number of maps stored in mMaps
   *
   * @return	The number of maps
   */
  int retrieveNumMaps();

  /**
   * Retrieves the specified map's tag and stores it into the provided pointer
   *
   * Returns an error if the index is out of bounds
   *
   * @param	index		The index of the map whose tag is being retrieved
   * @param	tag		The location where the tag will be stored
   * @param	tagLength	The size fo the array at the provided pointer
   */
  void retrieveMapTag(int index, char * tag, int tagLength);

  /**
   * Adds id dependancies to the specified map in mMaps
   *
   * @param	index			The index of the map for the data to be added to
   * @param	localNodeID		The ID of the local node
   * @param	remoteTaskID		The task ID to be associated with the local node
   * @param	remoteLocalNodeID	The remote local node to be associated with the local node and task id
   */
  void addRemoteNodeID(int index, int localNodeID, int remoteTaskID, int remoteLocalNodeID);

  /**
   * Gets the remote local ids associated with the provided local node and task from a specified map in mMaps
   *
   * @param	index		The index of the map which the data is to be retrieved from
   * @param	localNodeID	The ID of the local node whose remote local node is to be found
   * @param	remoteTaskID	The ID of the task whose remote local node is to be found
   * @param	remoteNodeID	A pointer to the location where the remote local IDs will be returned to
   */
  void retrieveRemoteNodeIDs(int index, int localNodeID, int remoteTaskID, int * remoteNodeIDs);

  /**
   * Gets the number of remote local ids associated with the provided local node and task from a specified map in mMaps
   *
   * @param	index		The index of the map which the data is to be retrieved from
   * @param	localNodeID	The ID of the local node whose number of associated remote local nodes is to be found
   * @param	remoteTaskID	The ID of the task whose number of associated remote local nodes is to be found
   *
   * @return			The number of remote node ids associated with the supplied IDs
   */
  int retrieveNumRemoteNodeIDs(int index, int localNodeID, int remoteTaskID);

  /**
   * Clears mMaps of all added maps.
   */
  void clearMaps();

  /**
   * Removes the specified map from mMaps.
   *
   * @param	index	The index of the map to be removed.
   */
  void removeMap(int index);

  /**
   * Stores specified mMap in mPreviousMaps.
   *
   * Gives an error if the index is out of bounds
   *
   * @param	index	The index of the specified map in mMaps
   *
   * @return		The id of the corresponding map in mPreviousMaps
   */
  int storeMap(int index);

  /**
   * Replaces the current mMap with one specified from mPreviousMaps.
   *
   * Gives an error if the index is out of range.
   *
   * @param	index	The index of the specified map
   */
  void addPreviousMap(int index);

  /**
   * Clears mPreviousMaps. Use to reduce memory load.
   */
  void clearPreviousMaps();

  /**
   * Returns the number of properties currently stored in the specified map in mMaps.
   *
   * @param	index	The index of the specified map
   *
   * @return		the number of properties associated with the map
   */
  int retrieveMapNumProperties(int index);

  /**
   * Retrieves the key and value of the property at the specifed index of the specified map in mMaps
   * and modifies the values at the provided pointers
   *
   * Returns an error if the index is out of bounds
   *
   * @param 	mapIndex		The index of the specified map
   * @param 	index			The index of the property that is being retrieved
   * @param	key			A pointer to the location where the key value will be stored
   * @param	keyLength		the size of the key variable
   * @param	value			A pointer to the location where the value of the property will be stored
   * @param	valueLength		the size of the value variable
   */
  void retrieveMapProperty(int mapIndex, int index, char * key, int keyLength, char * value, int valueLength);

  /**
   * Retrieves the value of a property of the specified map when given its key.
   *
   * Returns an error if the key has no matching value
   *
   * @param	index		The index of the specified map
   * @param 	key		A pointer to the value of the key that is being searched for
   * @param	value		A pointer to the location where the value of the property will be stored
   * @param	valueLength	the size of the value variable
   */
  void retrieveMapPropertyByKey(int index, char * key, char * value, int valueLength);

  /**
   * Returns the number of attributes currently loaded.
   *
   * @return	the number of attributes in mAttributes
   */
  int retrieveNumAttributes();

  /**
   * Removes the attribute at the supplied index from mAttributes
   *
   * Returns an error if the index is out of bounds
   *
   * @param	index	The index of the Attribute to be removed
   */
  void removeAttribute(int index);

  /**
   * Replaces the specified attribute in mAttributes with one made using the supplied components
   *
   * Returns an error if the index is out of bounds
   *
   * @param	index			The index of the Attribute to be replaced
   * @param	name			A pointer to the name of the new attribute
   * @param	attributeCenter		An integer representation of the center of the new attribute Ex:XDMF_ATTRIBUTE_CENTER_NODE
   * @param	attributeType		An integer representation of the type of the new attribute Ex:XDMF_ATTRIBUTE_TYPE_SCALAR
   * @param	numValues		The number of values contained at the array located at the supplied void pointer
   * @param	arrayType		An integer representation of the type of data contained within the array Ex:XDMF_ARRAY_TYPE_INT32
   * @param	values			A pointer to the location of the values that will be used to fill the new attribute
   */
  void replaceAttribute(int index, char * name, int attributeCenter, int attributeType, int numValues, int arrayType, void * values);

  /**
   * Opens the specified attribute by placing the information it contains in mInformations
   *
   * @param	index	The index of the specified information
   */
  void openAttribute(int index);

  /**
   * Retrieves the specified Attribute's tag and stores it into the provided pointer
   *
   * Returns an error if the index is out of bounds
   *
   * @param	index		The index of the attribute whose tag is being retrieved
   * @param	tag		The location where the tag will be stored
   * @param	tagLength	The size fo the array at the provided pointer
   */
  void retrieveAttributeTag(int index, char * tag, int tagLength);

  /**
   * Retrieves the specified Attribute's name and stores it into the provided pointer
   *
   * Returns an error if the index is out of bounds
   *
   * @param	index		The index of the attribute whose name is being retrieved
   * @param	name		The location where the name will be stored
   * @param	nameLength	The size fo the array at the provided pointer
   */
  void retrieveAttributeName(int index, char * name, int nameLength);

  /**
   * Clears mAttributes of all added attributes
   */
  void clearAttributes();

  /**
   * Returns a pointer the values of a specified attribute
   *
   * Returns an error if the index is out of bounds
   *
   * @param 	index		the index that the specified Attribute exists at
   * @param	values		a void pointer to the array to recieve the attribute's values
   * @param	dataType	an integer corresponding to the datatype that the data will be stored in
   * @param 	numberRead	the number of values read into the values array
   * @param 	startIndex	the place to start reading from the attribute's array
   * @param 	arrayStride	the distance between values read (1 reads all values, 2 reads every other, ect..)
   * @param	valueStride	the distance between the places that the read values are placed in the supplied array
   */
  void retrieveAttributeValues(int index, void * values, int dataType,
	int numberRead, int startIndex, int arrayStride, int valueStride);

  /**
   * Returns the datatype of the specified attribute as an integer.
   * 
   * Returns an error if the index is out of bounds
   *
   * @param	index	the index to the attribute
   *
   * @return	the type of the specified attribute as an integer, EX: XDMF_ARRAY_TYPE_INT8
   */
  int retrieveAttributeValueType(int index);

  /**
   * Returns the number of values in the specified Attribute
   *
   * Returns an error if the index is out of bounds
   *
   * @param	index	the index to the attribute
   *
   * @return	the number of values contained in the specified attribute
   */
  int retrieveAttributeSize(int index);

  /**
   * Returns the type of the specified attribute as an integer.
   * 
   * Returns an error if the index is out of bounds
   *
   * @param	index	the index to the attribute
   *
   * @return	the type of the specified attribute as an integer, EX: XDMF_ATTRIBUTE_TYPE_SCALAR
   */
  int retrieveAttributeType(int index);

  /**
   * Returns the center of the specified attribute as an integer.
   * 
   * Returns an error if the index is out of bounds
   *
   * @param	index	the index to the attribute
   *
   * @return	the center of the specified attribute as an integer, EX: XDMF_ATTRIBUTE_CENTER_NODE
   */
  int retrieveAttributeCenter(int index);

  /**
   * Clears mPreviousAttributes. Use to reduce memory load.
   */
  void clearPreviousAttributes();

  /**
   * Returns the number of properties currently stored in the specified attribute.
   *
   * Returns an error if the index is out of bounds
   *
   * @param	index	The index of the attribute whose number of properties is being retrieved
   *
   * @return		the number of properties associated with the specified attribute
   */
  int retrieveAttributeNumProperties(int index);

  /**
   * Retrieves the key and value of the property at the specifed index of the specified attribute
   * and modifies the values at the provided pointers
   *
   * Returns an error if the index is out of bounds
   *
   * @param 	attributeIndex		The index of the attribute that is being retrieved
   * @param 	index			The index of the property that is being retrieved
   * @param	key			A pointer to the location where the key value will be stored
   * @param	keyLength		the size of the key variable
   * @param	value			A pointer to the location where the value of the property will be stored
   * @param	valueLength		the size of the value variable
   */
  void retrieveAttributeProperty(int attributeIndex, int index, char * key, int keyLength, char * value, int valueLength);

  /**
   * Retrieves the value of a property of a specified attribute when given its key.
   *
   * Returns an error if the index is out of bounds
   * Returns an error if the key has no matching value
   *
   * @param	index		The index of the attribute whose property is being retrieved
   * @param 	key		A pointer to the value of the key that is being searched for
   * @param	value		A pointer to the location where the value of the property will be stored
   * @param	valueLength	The size of the value variable
   */
  void retrieveAttributePropertyByKey(int index, char * key, char * value, int valueLength);

  /**
   * Returns the number of coordinates currently loaded.
   *
   * @return	the number of coordinates in mCoordinates
   */
  int retrieveNumCoordinates();

  /**
   * Removes the coordinate at the supplied index from mCoordinates
   *
   * Returns an error if the index is out of bounds
   *
   * @param	index	The index of the Coordinate to be removed
   */
  void removeCoordinate(int index);

  /**
   * Add an coordinate that will be inserted into the next grid or grid
   * collection.
   *
   * @param name of the coordinate.
   * @param numValues number of coordinate values to copy.
   * @param arrayType type of coordinate values.
   * @param values array of coordinate values.
   *
   * @return int providing id to fortran if reusing.
   */
  int addCoordinate(char * name, int numValues, int arrayType, void * values);

  /**
   * Adds a specified coordinate from mPreviousCoordinates to mCoordinates
   *
   * @param	index	The index of the specified coordinate
   */
  void addPreviousCoordinate(int index);

  /**
   * Clears mPreviousCoordinates. Use to reduce memory load.
   */
  void clearPreviousCoordinates();

  /**
   * Replaces the specified coordinate in mCoordinates with one made using the supplied components
   *
   * Returns an error if the index is out of bounds
   *
   * @param	index			The index of the coordinate to be replaced
   * @param	name			A pointer to the name of the new coordinate
   * @param	numValues		The number of values contained at the array located at the supplied void pointer
   * @param	arrayType		An integer representation of the type of data contained within the array Ex:XDMF_ARRAY_TYPE_INT32
   * @param	values			A pointer to the location of the values that will be used to fill the new coordinate
   */
  void replaceCoordinate(int index, char * name, int numValues, int arrayType, void * values);

  /**
   * Retrieves the specified Coordinate's tag and stores it into the provided pointer
   *
   * Returns an error if the index is out of bounds
   *
   * @param	index		The index of the coordinate whose tag is being retrieved
   * @param	tag		The location where the tag will be stored
   * @param	tagLength	The size fo the array at the provided pointer
   */
  void retrieveCoordinateTag(int index, char * tag, int tagLength);

  /**
   * Retrieves the specified Coordinate's name and stores it into the provided pointer
   *
   * Returns an error if the index is out of bounds
   *
   * @param	index		The index of the coordinate whose name is being retrieved
   * @param	name		The location where the name will be stored
   * @param	nameLength	The size fo the array at the provided pointer
   */
  void retrieveCoordinateName(int index, char * name, int nameLength);

  /**
   * Returns a pointer the values of a specified coordinate
   *
   * Returns an error if the index is out of bounds
   *
   * @param 	index		the index that the specified coordinate exists at
   * @param	values		a void pointer to the array to recieve the coordinate's values
   * @param	dataType	an integer corresponding to the datatype that the data will be stored in
   * @param 	numberRead	the number of values read into the values array
   * @param 	startIndex	the place to start reading from the coordinate's array
   * @param 	arrayStride	the distance between values read (1 reads all values, 2 reads every other, ect..)
   * @param	valueStride	the distance between the places that the read values are placed in the supplied array
   */
  void retrieveCoordinateValues(int index, void * values, int dataType,
	int numberRead, int startIndex, int arrayStride, int valueStride);

  /**
   * Returns the datatype of the specified coordinate as an integer.
   * 
   * Returns an error if the index is out of bounds
   *
   * @param	index	the index to the coordinate
   *
   * @return	the type of the specified coordinate as an integer, EX: XDMF_ARRAY_TYPE_INT8
   */
  int retrieveCoordinateValueType(int index);

  /**
   * Returns the number of values in the specified coordinate
   *
   * Returns an error if the index is out of bounds
   *
   * @param	index	the index to the coordinate
   *
   * @return	the number of values contained in the specified coordinate
   */
  int retrieveCoordinateSize(int index);

  /**
   * Clears mCoordinates of all added coordiantes.
   */
  void clearCoordinates();

  /**
   * Returns the number of properties currently stored in the specified coordinate.
   *
   * Returns an error if the index is out of bounds
   *
   * @param	index	The index of the coordinate whose number of properties is being retrieved
   *
   * @return		the number of properties associated with the specified coordinate
   */
  int retrieveCoordinateNumProperties(int index);

  /**
   * Retrieves the key and value of the property at the specifed index of the specified coordinate
   * and modifies the values at the provided pointers
   *
   * Returns an error if the index is out of bounds
   *
   * @param 	coordinateIndex		The index of the coordinate that is being retrieved
   * @param 	index			The index of the property that is being retrieved
   * @param	key			A pointer to the location where the key value will be stored
   * @param	keyLength		the size of the key variable
   * @param	value			A pointer to the location where the value of the property will be stored
   * @param	valueLength		the size of the value variable
   */
  void retrieveCoordinateProperty(int coordinateIndex, int index, char * key, int keyLength, char * value, int valueLength);

  /**
   * Retrieves the value of a property of a specified coordinate when given its key.
   *
   * Returns an error if the index is out of bounds
   * Returns an error if the key has no matching value
   *
   * @param	index		The index of the coordinate whose property is being retrieved
   * @param 	key		A pointer to the value of the key that is being searched for
   * @param	value		A pointer to the location where the value of the property will be stored
   * @param	valueLength	The size of the value variable
   */
  void retrieveCoordinatePropertyByKey(int index, char * key, char * value, int valueLength);

  /**
   * Retrieves the specified set's tag and stores it into the provided pointer
   *
   * Returns an error if the index is out of bounds
   *
   * @param	index		The index of the set whose tag is being retrieved
   * @param	tag		The location where the tag will be stored
   * @param	tagLength	The size fo the array at the provided pointer
   */
  void retrieveSetTag(int index, char * tag, int tagLength);

  /**
   * Retrieves the specified set's name and stores it into the provided pointer
   *
   * Returns an error if the index is out of bounds
   *
   * @param	index		The index of the set whose name is being retrieved
   * @param	name		The location where the name will be stored
   * @param	nameLength	The size fo the array at the provided pointer
   */
  void retrieveSetName(int index, char * name, int nameLength);

  /**
   * Retrievest the type of the specified set
   *
   * Returns an error if the index is out of bounds
   *
   * @param	index	The index of the specified set
   *
   * @return		The set's type as an integer Ex. XDMF_SET_TYPE_NODE
   */
  int retrieveSetType(int index);

  /**
   * Adds a set to mSets that contains the current contents to mInformations and mAttributes.
   * Then the top of parentAttributes and parentInformations
   * are placed in mAttributes and mInformations
   *
   * @param	name		A pointer to the location of the name for the set
   * @param	newSetType	The integer equivalent to the set type of the added set Ex: XDMF_SET_TYPE_NODE
   * @param	values		A pointer to the location where the values to be placed in the set will are stored
   * @param	numValues	The amount of values in the array at the provided pointer
   * @param	arrayType	The integer representation of the datatype of the values Ex: XDMF_ARRAY_TYPE_INT32
   *
   * @return			An ID that can be used to recall the set from mPreviousSets
   */
  int addSet(char * name, int newSetType, void * values, int numValues, int arrayType);

  /**
   * Adds the specifed set from mPreviousSets into mSets.
   *
   * @param	index	The index of the set in mPreviousSets to be added
   */
  void addPreviousSet(int index);

  /**
   * Clears mPreviousSets. Use to reduce memory load.
   */
  void clearPreviousSets();

  /**
   * Clears mSets of all added Sets
   */
  void clearSets();

  /**
   * Returns the number of sets in mSets
   *
   * @return	The number of sets
   */
  int retrieveNumSets();

  /**
   * Returns the number of values in the specified set
   *
   * Returns an error if the index is out of bounds
   *
   * @param	index	the index to the set
   *
   * @return		the number of values contained in the specified set
   */
  int retrieveSetSize(int index);

  /**
   * Returns a pointer the values of a specified set
   *
   * Returns an error if the index is out of bounds
   *
   * @param 	index		the index that the specified set exists at
   * @param	values		a void pointer to the array to recieve the set's values
   * @param	dataType	an integer corresponding to the datatype that the data will be stored in
   * @param 	numberRead	the number of values read into the values array
   * @param 	startIndex	the place to start reading from the set's array
   * @param 	arrayStride	the distance between values read (1 reads all values, 2 reads every other, ect..)
   * @param	valueStride	the distance between the places that the read values are placed in the supplied array
   */
  void retrieveSetValues(int index, void * values, int dataType,
	int numberRead, int startIndex, int arrayStride, int valueStride);

  /**
   * Returns the datatype of the specified set as an integer.
   * 
   * Returns an error if the index is out of bounds
   *
   * @param	index	the index of the Set
   *
   * @return		the type of the specified Set's values as an integer, EX: XDMF_ARRAY_TYPE_INT8
   */
  int retrieveSetValueType(int index);

  /**
   * Opens the set at the specified index by placing its attributes and information
   * in mAttributes and mInformations. Pushes the previous contents of those two containers
   * into parentAttributes and parentInformations.
   *
   * Returns an error if the index is out of bounds
   *
   * @param	index	The index of the specified set
   * @param	openAttributes	set to 1 to open attributes
   * @param	openInformation	set to 1 to open information
   */
  void openSet(int index, int openAttributes, int openInformation);

  /**
   * Removes the specified set from mSets.
   *
   * Returns an error if the index is out of bounds
   *
   * @param	index	The index of the set to be removed
   */
  void removeSet(int index);

  /**
   * Replaces a specified set in mSets with a set that contains
   * the current contents to mInformations and mAttributes.
   * Then the top of parentAttributes and parentInformations
   * are placed in mAttributes and mInformations
   *
   * Returns an error if the index is out of bounds
   *
   * @param	index		The index of the set to be replaced
   * @param	name		A pointer to the location of the name for the set
   * @param	newSetType	The integer equivalent to the set type of the added set Ex: XDMF_SET_TYPE_NODE
   * @param	values		A pointer to the location where the values to be placed in the set will are stored
   * @param	numValues	The amount of values in the array at the provided pointer
   * @param	arrayType	The integer representation of the datatype of the values Ex: XDMF_ARRAY_TYPE_INT32
   */
  void replaceSet(int index, char * name, int newSetType, void * values, int numValues, int arrayType);

  /**
   * Returns the number of properties currently stored in the specified set.
   *
   * Returns an error if the index is out of bounds
   *
   * @param	index	The index of the set whose number of properties is being retrieved
   *
   * @return		the number of properties associated with the specified set
   */
  int retrieveSetNumProperties(int index);

  /**
   * Retrieves the key and value of the property at the specifed index of the specified set
   * and modifies the values at the provided pointers
   *
   * Returns an error if the index is out of bounds
   *
   * @param 	setIndex	The index of the set that is being retrieved
   * @param 	index		The index of the property that is being retrieved
   * @param	key		A pointer to the location where the key value will be stored
   * @param	keyLength	the size of the key variable
   * @param	value		A pointer to the location where the value of the property will be stored
   * @param	valueLength	the size of the value variable
   */
  void retrieveSetProperty(int setIndex, int index, char * key, int keyLength, char * value, int valueLength);

  /**
   * Retrieves the value of a property of a specified set when given its key.
   *
   * Returns an error if the index is out of bounds
   * Returns an error if the key has no matching value
   *
   * @param	index		The index of the set whose property is being retrieved
   * @param 	key		A pointer to the value of the key that is being searched for
   * @param	value		A pointer to the location where the value of the property will be stored
   * @param	valueLength	the size of the value variable
   */
  void retrieveSetPropertyByKey(int index, char * key, char * value, int valueLength);

  /**
   * Returns the number of information currently stored in mInformations.
   *
   * @return	the number of information contained within mInformations
   */
  int retrieveNumInformation();

  /**
   * Retrieves the specified information's tag and stores it into the provided pointer
   *
   * Returns an error if the index is out of bounds
   *
   * @param	index		The index of the information whose tag is being retrieved
   * @param	tag		The location where the tag will be stored
   * @param	tagLength	The size fo the array at the provided pointer
   */
  void retrieveInformationTag(int index, char * tag, int tagLength);

  /**
   * Retrieves the key and value of the Information at the specifed index
   * and modifies the values at the provided pointers
   *
   * Returns an error if the index is out of bounds
   *
   * @param 	index			The index of the property that is being retrieved
   * @param	key			A pointer to the location where the key value will be stored
   * @param	keyLength		the size of the key variable
   * @param	value			A pointer to the location where the value of the information will be stored
   * @param	valueLength		the size of the value variable
   */
  void retrieveInformation(int index, char * key, int keyLength, char * value, int valueLength);

  /**
   * Removes the information at the specified index.
   *
   * Returns an error if the index is out of bounds
   *
   * @param	index	The index of the information to be removed
   */
  void removeInformation(int index);

  /**
   * Replaces the key and value of the information at the specified index
   *
   * Returns an error if the index is out of bounds
   *
   * @param	index	The index of the information to be changed
   * @param	key	A pointer to the new key
   * @param	value	A pointer to the new value
   */
  void replaceInformation(int index, char * key, char * value);

  /**
   * Opens the specified information by placing the information it contains in mInformations
   *
   * @param	index	The index of the information to be opened
   */
  void openInformation(int index);

  /**
   * Retrieves the value of a specified information when given its key.
   *
   * Returns an error if the key has no matching value
   *
   * @param 	key		A pointer to the value of the key that is being searched for
   * @param	value		A pointer to the location where the value of the information will be stored
   * @param	valueLength	the size of the value variable
   */
  void retrieveInformationByKey(char * key, char * value, int valueLength);

  /**
   * Removes the information with the specified key
   *
   * Returns an error if the key does not match an information
   *
   * @param	key	A pointer to the value of the key being searched for
   */
  void removeInformationByKey(char * key);

  /**
   * Replaces the value of the information with the specified key
   *
   * Returns an error if the key does not match a value
   *
   * @param	key	A pointer to the value of the key being searched for
   * @param	value	A pointer to the value that will be placed into the information
   */
  void replaceInformationByKey(char * key, char * value);

  /**
   * Clears mPreviousInformations. Use to reduce memory load.
   */
  void clearPreviousInformation();

  /**
   * Clears mInformations of all added information.
   */
  void clearInformations();

  /**
   * Returns the number of properties currently stored in the specified information.
   *
   * Returns an error if the index is out of bounds
   *
   * @param	index	The index of the information whose number of properties is being retrieved
   *
   * @return		the number of properties associated with the specified information
   */
  int retrieveInformationNumProperties(int index);

  /**
   * Retrieves the key and value of the property at the specifed index of the specified information
   * and modifies the values at the provided pointers
   *
   * Returns an error if the index is out of bounds
   *
   * @param 	informationIndex	The index of the information that is being retrieved
   * @param 	index			The index of the property that is being retrieved
   * @param	key			A pointer to the location where the key value will be stored
   * @param	keyLength		the size of the key variable
   * @param	value			A pointer to the location where the value of the property will be stored
   * @param	valueLength		the size of the value variable
   */
  void retrieveInformationProperty(int informationIndex, int index, char * key, int keyLength, char * value, int valueLength);

  /**
   * Retrieves the value of a property of a specified information when given its key.
   *
   * Returns an error if the index is out of bounds
   * Returns an error if the key has no matching value
   *
   * @param	index		The index of the information whose property is being retrieved
   * @param 	key		A pointer to the value of the key that is being searched for
   * @param	value		A pointer to the location where the value of the property will be stored
   * @param	valueLength	the size of the value variable
   */
  void retrieveInformationPropertyByKey(int index, char * key, char * value, int valueLength);

  /**
   * clears all of the mPrevious vectors. Used to reduce memory load.
   */
  void clearPrevious();




  /**
   * Sets whether to allow the hdf5 writer to split large data sets across multiple files.
   * Setting to true removes compatibiliity with previous versions.
   * When off it will write sets that will not fit into a file to the current file
   * before moving to the next should file splitting be enabled.
   * Default is off (false).
   *
   * @param	newAllow	whether to allow splitting or not
   */
  void setAllowSetSplitting(bool newAllow);

  /**
   * Sets the file size at which the hdf5 writer will move to a new file.
   * Default is no splitting (value=0)
   * 
   * @param	newSize		new maximum file size before moving
   */
  void setMaxFileSize(int newSize);

  /**
   * Write constructed file to disk.
   *
   * @param xmlFilePath the path to the xml file to write to.
   * @param release	whether the HDF5 writer will release data after finishing
   */
  void write(const char * const xmlFilePath, const int dataLimit, const bool release);

  /** 
   * Write HDF5 heavy data to disk and release
   *
   * @param xmlFilePath the path to the xml file to write to.
   * @param release     whether or not to release data after a write
   */
  void writeHDF5(const char * const xmlFilePath, const bool release);

  /**
   * Generate the persistant hdf5 writer so it doesn't need to be generated later
   *
   * @param xmlFilePath the path to the xml file to write to.
   * @param release	whether or not to release data after a write
   */
  void initHDF5(const char * const xmlFilePath, const bool release);

  /** 
   * Read xml file and make it the domain. Replaces current domain.
   *
   * @param xmlFilePath the path to the xml file to read.
   */
  void read(const char * const xmlFilePath);


  /** 
   * Set the topology (connectivity data) for a polyline that will be
   * added to the next grid.
   *
   * @param nodesPerElement number of nodes in the polyline.
   * @param numValues number of connectivity values to copy.
   * @param arrayType type of connectivity values.
   * @param connectivityValues array of connectivity values.
   *
   * @return int providing id to fortran if reusing.
   */
  int setTopologyPolyline(const unsigned int nodesPerElement, 
                          const unsigned int numValues,
                          const int arrayType,
                          const void * const connectivityValues);


private:
  
  shared_ptr<XdmfDomain>   mDomain;
  shared_ptr<XdmfGeometry> mGeometry;
  shared_ptr<XdmfTime>     mTime;
  shared_ptr<XdmfTopology> mTopology;
  shared_ptr<XdmfArray>    mBrick;
  shared_ptr<XdmfArray>    mOrigin;
  shared_ptr<XdmfArray>	   mDimensions;
  shared_ptr<XdmfHeavyDataWriter> mHeavyDataWriter;

  std::vector<shared_ptr<XdmfAttribute> >     mAttributes;
  std::vector<shared_ptr<XdmfArray> >         mCoordinates;
  std::stack<shared_ptr<XdmfGridCollection> > mGridCollections;
  std::vector<shared_ptr<XdmfInformation> >   mInformations;
  std::vector<shared_ptr<XdmfSet> >           mSets;
  std::vector<shared_ptr<XdmfMap> >           mMaps;

  std::vector<shared_ptr<XdmfAttribute> >   mPreviousAttributes;
  std::vector<shared_ptr<XdmfGeometry> >    mPreviousGeometries;
  std::vector<shared_ptr<XdmfInformation> > mPreviousInformations;
  std::vector<shared_ptr<XdmfSet> >         mPreviousSets;
  std::vector<shared_ptr<XdmfTopology> >    mPreviousTopologies;
  std::vector<shared_ptr<XdmfArray> >       mPreviousDimensions;
  std::vector<shared_ptr<XdmfArray> >       mPreviousOrigins;
  std::vector<shared_ptr<XdmfArray> >       mPreviousBricks;
  std::vector<shared_ptr<XdmfArray> >       mPreviousCoordinates;
  std::vector<shared_ptr<XdmfMap> >         mPreviousMaps;

  unsigned int mMaxFileSize;
  bool mAllowSetSplitting;


};

#endif /* XDMFFORTRAN_HPP_ */
