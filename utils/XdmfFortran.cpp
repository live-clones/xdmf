
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
/*     Copyright @ 2009 US Army Research Laboratory                */
/*     All Rights Reserved                                         */
/*     See Copyright.txt or http://www.arl.hpc.mil/ice for details */
/*                                                                 */
/*     This software is distributed WITHOUT ANY WARRANTY; without  */
/*     even the implied warranty of MERCHANTABILITY or FITNESS     */
/*     FOR A PARTICULAR PURPOSE.  See the above copyright notice   */
/*     for more information.                                       */
/*                                                                 */
/*******************************************************************/
#include <sys/stat.h>

#include "XdmfFortran.hpp"

#include "XdmfArrayType.hpp"
#include "XdmfAttribute.hpp"
#include "XdmfAttributeCenter.hpp"
#include "XdmfAttributeType.hpp"
#include "XdmfDomain.hpp"
#include "XdmfError.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfGeometryType.hpp"
#include "XdmfGrid.hpp"
#include "XdmfGridCollection.hpp"
#include "XdmfGridCollectionType.hpp"
#include "XdmfInformation.hpp"
#include "XdmfReader.hpp"
#include "XdmfTime.hpp"
#include "XdmfTopology.hpp"
#include "XdmfTopologyType.hpp"
#include "XdmfSet.hpp"
#include "XdmfSetType.hpp"
#include "XdmfMap.hpp"
#include "XdmfCurvilinearGrid.hpp"
#include "XdmfRectilinearGrid.hpp"
#include "XdmfRegularGrid.hpp"
#include "XdmfUnstructuredGrid.hpp"
#include "XdmfWriter.hpp"
#include "XdmfHDF5Writer.hpp"
#include "XdmfHDF5Controller.hpp"
#include "string.h"

#include <stdio.h>
#include <stdarg.h>

#ifdef XDMF_BUILD_DSM
  #include <mpi.h>
  #include "XdmfHDF5WriterDSM.hpp"
  #include "XdmfHDF5ControllerDSM.hpp"
#endif

template <typename T>
void
XdmfFortran::insertElements(const T grid,
                            std::vector<shared_ptr<XdmfAttribute> > & mAttributes,
                            std::vector<shared_ptr<XdmfInformation> > & mInformations,
                            std::vector<shared_ptr<XdmfSet> > & mSets,
                            std::vector<shared_ptr<XdmfMap> > & mMaps,
                            shared_ptr<XdmfTime> mTime,
                            shared_ptr<XdmfDomain> mDomain,
                            std::stack<shared_ptr<XdmfGridCollection> > & mGridCollections)
{
  for(std::vector<shared_ptr<XdmfAttribute> >::const_iterator iter =
        mAttributes.begin();
      iter != mAttributes.end();
      ++iter) {
    grid->insert(*iter);
  }

  mAttributes.clear();

  for(std::vector<shared_ptr<XdmfInformation> >::const_iterator iter =
        mInformations.begin();
      iter != mInformations.end();
      ++iter) {
    grid->insert(*iter);
  }

  mInformations.clear();

  for(std::vector<shared_ptr<XdmfSet> >::const_iterator iter =
        mSets.begin();
      iter != mSets.end();
      ++iter) {
    grid->insert(*iter);
  }

  mSets.clear();

  for(std::vector<shared_ptr<XdmfMap> >::const_iterator iter =
        mMaps.begin();
      iter != mMaps.end();
      ++iter) {
    grid->insert(*iter);
  }

  mMaps.clear();

  if(mTime) {
    grid->setTime(mTime);
  }

  if(mGridCollections.empty()) {
    mDomain->insert(grid);
  }
  else {
    mGridCollections.top()->insert(grid);
  }
}

// read values from an xdmf array for a number type
void
XdmfFortran::readFromArray(shared_ptr<XdmfArray> array,
                           const int arrayType,
                           void * const values,
                           const unsigned int numValues,
                           const unsigned int startIndex,
                           const unsigned int arrayStride,
                           const unsigned int valuesStride)
{
  if (!array->isInitialized()) {
    array->read();
  }
  switch(arrayType) {
  case XDMF_ARRAY_TYPE_INT8:
    array->getValues(startIndex,
                     static_cast<char *>(values),
                     numValues,
                     arrayStride,
                     valuesStride);
    break;
  case XDMF_ARRAY_TYPE_INT16:
    array->getValues(startIndex,
                     static_cast<short *>(values),
                     numValues,
                     arrayStride,
                     valuesStride);
    break;
  case XDMF_ARRAY_TYPE_INT32:
    array->getValues(startIndex,
                     static_cast<int *>(values),
                     numValues,
                     arrayStride,
                     valuesStride);
    break;
  case XDMF_ARRAY_TYPE_INT64:
    array->getValues(startIndex,
                     static_cast<long *>(values),
                     numValues,
                     arrayStride,
                     valuesStride);
    break;
  case XDMF_ARRAY_TYPE_UINT8:
    array->getValues(startIndex,
                     static_cast<unsigned char *>(values),
                     numValues,
                     arrayStride,
                     valuesStride);
    break;
  case XDMF_ARRAY_TYPE_UINT16:
    array->getValues(startIndex,
                     static_cast<unsigned short *>(values),
                     numValues,
                     arrayStride,
                     valuesStride);
    break;
  case XDMF_ARRAY_TYPE_UINT32:
    array->getValues(startIndex,
                     static_cast<unsigned int *>(values),
                     numValues,
                     arrayStride,
                     valuesStride);
    break;
  case XDMF_ARRAY_TYPE_FLOAT32:
    array->getValues(startIndex,
                     static_cast<float *>(values),
                     numValues,
                     arrayStride,
                     valuesStride);
    break;
  case XDMF_ARRAY_TYPE_FLOAT64:
    array->getValues(startIndex,
                     static_cast<double *>(values),
                     numValues,
                     arrayStride,
                     valuesStride);
    break;
  default:
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Invalid array number type");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

// write values to xdmf array for a number type
void
XdmfFortran::writeToArray(shared_ptr<XdmfArray> array,
                          const unsigned int numValues,
                          const int arrayType,
                          const void * const values,
                          const unsigned int offset,
                          const unsigned int arrayStride,
                          const unsigned int valueStride)
{
  switch(arrayType) {
  case XDMF_ARRAY_TYPE_INT8:
    array->insert(offset,
                  static_cast<const char *>(values),
                  numValues,
                  arrayStride,
                  valueStride);
    break;
  case XDMF_ARRAY_TYPE_INT16:
    array->insert(offset,
                  static_cast<const short *>(values),
                  numValues,
                  arrayStride,
                  valueStride);
    break;
  case XDMF_ARRAY_TYPE_INT32:
    array->insert(offset,
                  static_cast<const int *>(values),
                  numValues,
                  arrayStride,
                  valueStride);
    break;
  case XDMF_ARRAY_TYPE_INT64:
    array->insert(offset,
                  static_cast<const long *>(values),
                  numValues,
                  arrayStride,
                  valueStride);
    break;
  case XDMF_ARRAY_TYPE_UINT8:
    array->insert(offset,
                  static_cast<const unsigned char *>(values),
                  numValues,
                  arrayStride,
                  valueStride);
    break;
  case XDMF_ARRAY_TYPE_UINT16:
    array->insert(offset,
                  static_cast<const unsigned short *>(values),
                  numValues,
                  arrayStride,
                  valueStride);
    break;
  case XDMF_ARRAY_TYPE_UINT32:
    array->insert(offset,
                  static_cast<const unsigned int *>(values),
                  numValues,
                  arrayStride,
                  valueStride);
    break;
  case XDMF_ARRAY_TYPE_FLOAT32:
    array->insert(offset,
                  static_cast<const float *>(values),
                  numValues,
                  arrayStride,
                  valueStride);
    break;
  case XDMF_ARRAY_TYPE_FLOAT64:
    array->insert(offset,
                  static_cast<const double *>(values),
                  numValues,
                  arrayStride,
                  valueStride);
    break;
  default:
    try {
      XdmfError::message(XdmfError::FATAL, "Invalid array type");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}



XdmfFortran::XdmfFortran() :
  mDomain(XdmfDomain::New()),
  mGeometry(shared_ptr<XdmfGeometry>()),
  mTime(shared_ptr<XdmfTime>()),
  mTopology(shared_ptr<XdmfTopology>()),
  mBrick(shared_ptr<XdmfArray>()),
  mOrigin(shared_ptr<XdmfArray>()),
  mDimensions(shared_ptr<XdmfArray>()),
  mHeavyDataWriter(shared_ptr<XdmfHeavyDataWriter>()),
  mDSMWriter(shared_ptr<XdmfHDF5WriterDSM>()),
  mMaxFileSize(0),
  mAllowSetSplitting(false)
{
}

XdmfFortran::~XdmfFortran()
{
}

int
XdmfFortran::addAttribute(const char * const name,
                          const int attributeCenter,
                          const int attributeType,
                          const unsigned int numValues,
                          const int arrayType,
                          const void * const values)
{
  shared_ptr<XdmfAttribute> currAttribute = XdmfAttribute::New();
  currAttribute->setName(name);

  switch(attributeCenter) {
  case XDMF_ATTRIBUTE_CENTER_GRID:
    currAttribute->setCenter(XdmfAttributeCenter::Grid());
    break;
  case XDMF_ATTRIBUTE_CENTER_CELL:
    currAttribute->setCenter(XdmfAttributeCenter::Cell());
    break;
  case XDMF_ATTRIBUTE_CENTER_FACE:
    currAttribute->setCenter(XdmfAttributeCenter::Face());
    break;
  case XDMF_ATTRIBUTE_CENTER_EDGE:
    currAttribute->setCenter(XdmfAttributeCenter::Edge());
    break;
  case XDMF_ATTRIBUTE_CENTER_NODE:
    currAttribute->setCenter(XdmfAttributeCenter::Node());
    break;
  default:
    try {
      XdmfError::message(XdmfError::FATAL, "Invalid attribute center");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
   
  switch(attributeType) {
  case XDMF_ATTRIBUTE_TYPE_SCALAR:
    currAttribute->setType(XdmfAttributeType::Scalar());
    break;
  case XDMF_ATTRIBUTE_TYPE_VECTOR:
    currAttribute->setType(XdmfAttributeType::Vector());
    break;
  case XDMF_ATTRIBUTE_TYPE_TENSOR:
    currAttribute->setType(XdmfAttributeType::Tensor());
    break;
  case XDMF_ATTRIBUTE_TYPE_MATRIX:
    currAttribute->setType(XdmfAttributeType::Matrix());
    break;
  case XDMF_ATTRIBUTE_TYPE_TENSOR6:
    currAttribute->setType(XdmfAttributeType::Tensor6());
    break;
  case XDMF_ATTRIBUTE_TYPE_GLOBALID:
    currAttribute->setType(XdmfAttributeType::GlobalId());
    break;
  default:
    try {
      XdmfError::message(XdmfError::FATAL, "Invalid attribute type");
    }
    catch (XdmfError e) {
      throw e;
    }
  }

  for(std::vector<shared_ptr<XdmfInformation> >::const_iterator iter =
        mInformations.begin(); 
      iter != mInformations.end();
      ++iter) {
    currAttribute->insert(*iter);
  }
  mInformations.clear();


  // insert values into attribute
  writeToArray(currAttribute,
               numValues,
               arrayType,
               values);

  mAttributes.push_back(currAttribute);

  const int id = mPreviousAttributes.size();
  mPreviousAttributes.push_back(currAttribute);
  return id;
}

// unstructured version
void 
XdmfFortran::addGrid(const char * const name, const bool writeToHDF5)
{
  const shared_ptr<XdmfUnstructuredGrid> grid = XdmfUnstructuredGrid::New();
  grid->setName(name);

  if(mGeometry == NULL) {
    try {
      XdmfError::message(XdmfError::FATAL, 
                       "Must set geometry before adding grid.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }

  if(mTopology == NULL) {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "Must set topology before adding grid.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
  
  grid->setGeometry(mGeometry);
  grid->setTopology(mTopology);

  insertElements(grid,
                 mAttributes,
                 mInformations,
                 mSets,
                 mMaps,
                 mTime,
                 mDomain,
                 mGridCollections);
  if (writeToHDF5 && mHeavyDataWriter != NULL) {
    mHeavyDataWriter->openFile();
    grid->accept(mHeavyDataWriter);
    mHeavyDataWriter->closeFile();
  }
}

void
XdmfFortran::addGridCurvilinear(const char * const name,
                                const bool writeToHDF5)
{
  if(mDimensions == NULL) {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Must set dimensions before adding grid.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }

  const shared_ptr<XdmfCurvilinearGrid> grid =
    XdmfCurvilinearGrid::New(mDimensions);
  grid->setName(name);

  if(mGeometry == NULL) {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Must set geometry before adding grid.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }

  grid->setGeometry(mGeometry);

  insertElements(grid,
                 mAttributes,
                 mInformations,
                 mSets,
                 mMaps,
                 mTime,
                 mDomain,
                 mGridCollections);
  if (writeToHDF5 && mHeavyDataWriter != NULL) {
    mHeavyDataWriter->openFile();
    grid->accept(mHeavyDataWriter);
    mHeavyDataWriter->closeFile();
  }
}

void
XdmfFortran::addGridRectilinear(const char * const name,
                                const bool writeToHDF5)
{
  if(mCoordinates.empty()) {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Must set Coordinates before adding grid.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }

  const shared_ptr<XdmfRectilinearGrid> grid =
    XdmfRectilinearGrid::New(mCoordinates);
  mCoordinates.clear();
  grid->setName(name);

  insertElements(grid,
                 mAttributes,
                 mInformations,
                 mSets,
                 mMaps,
                 mTime,
                 mDomain,
                 mGridCollections);
  if (writeToHDF5 && mHeavyDataWriter != NULL) {
    mHeavyDataWriter->openFile();
    grid->accept(mHeavyDataWriter);
    mHeavyDataWriter->closeFile();
  }
}

void
XdmfFortran::addGridRegular(const char * const name,
                            const bool writeToHDF5)
{
  if(mBrick == NULL) {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Must set brick size before adding grid.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }

  if(mDimensions == NULL) {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Must set dimensions before adding grid.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }

  if(mOrigin == NULL) {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Must set origin before adding grid.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }

  const shared_ptr<XdmfRegularGrid> grid =
    XdmfRegularGrid::New(mBrick, mDimensions, mOrigin);
  grid->setName(name);

  insertElements(grid,
                 mAttributes,
                 mInformations,
                 mSets,
                 mMaps,
                 mTime,
                 mDomain,
                 mGridCollections);
  if (writeToHDF5 && mHeavyDataWriter != NULL) {
    mHeavyDataWriter->openFile();
    grid->accept(mHeavyDataWriter);
    mHeavyDataWriter->closeFile();
  }
}



void 
XdmfFortran::addGridCollection(const char * const name,
                               const int gridCollectionType)
{
  const shared_ptr<XdmfGridCollection> gridCollection = 
    XdmfGridCollection::New();
  gridCollection->setName(name);

  switch(gridCollectionType) {
  case XDMF_GRID_COLLECTION_TYPE_SPATIAL:
    gridCollection->setType(XdmfGridCollectionType::Spatial());
    break;
  case XDMF_GRID_COLLECTION_TYPE_TEMPORAL:
    gridCollection->setType(XdmfGridCollectionType::Temporal());
    break;
  default:
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Invalid grid collection type");
    }
    catch (XdmfError e) {
      throw e;
    }
  }

  insertElements(gridCollection,
                 mAttributes,
                 mInformations,
                 mSets,
                 mMaps,
                 mTime,
                 mDomain,
                 mGridCollections);

  mGridCollections.push(gridCollection);
}

int
XdmfFortran::addInformation(const char * const key,
                            const char * const value)
{
  shared_ptr<XdmfInformation> information = XdmfInformation::New();
  information->setKey(key);
  information->setValue(value);

  mInformations.push_back(information);

  const int id = mPreviousInformations.size();
  mPreviousInformations.push_back(information);
  return id;
}

void 
XdmfFortran::addPreviousAttribute(const int attributeId)
{
  if(attributeId >= (int)mPreviousAttributes.size()) {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Invalid attribute id");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
  mAttributes.push_back(mPreviousAttributes[attributeId]);
}

void 
XdmfFortran::addPreviousInformation(const int informationId)
{
  if(informationId >= (int)mPreviousInformations.size()) {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Invalid information id");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
  mInformations.push_back(mPreviousInformations[informationId]);
}

void 
XdmfFortran::closeGridCollection(const bool writeToHDF5)
{
  if(!mGridCollections.empty()) {
    if (writeToHDF5 && mHeavyDataWriter != NULL) {
      mHeavyDataWriter->openFile();
      mGridCollections.top()->accept(mHeavyDataWriter);
      mHeavyDataWriter->closeFile();
    }
    mGridCollections.pop();
  }
}

int
XdmfFortran::setGeometry(const int geometryType, 
                         const unsigned int numValues,
                         const int arrayType, 
                         const void * const pointValues)
{
  mGeometry = XdmfGeometry::New();

  switch(geometryType) {
  case XDMF_GEOMETRY_TYPE_XYZ:
    mGeometry->setType(XdmfGeometryType::XYZ());
    break;
  case XDMF_GEOMETRY_TYPE_XY:
    mGeometry->setType(XdmfGeometryType::XY());
    break;
  default:
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Invalid geometry type.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }

  // insert geometry values into array
  writeToArray(mGeometry,
               numValues,
               arrayType,
               pointValues);

  const int id = mPreviousGeometries.size();
  mPreviousGeometries.push_back(mGeometry);
  return id;
}

void 
XdmfFortran::setPreviousGeometry(const int geometryId)
{
  if(geometryId >= (int)mPreviousGeometries.size()) {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Invalid geometry id");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
  mGeometry = mPreviousGeometries[geometryId];
}

void 
XdmfFortran::setPreviousTopology(const int topologyId)
{
  if(topologyId >= (int)mPreviousTopologies.size()) {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Invalid topology id");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
  mTopology = mPreviousTopologies[topologyId];
}

void 
XdmfFortran::setTime(const double time)
{
  mTime = XdmfTime::New();
  mTime->setValue(time);
}

int
XdmfFortran::setTopology(const int topologyType, 
                         const unsigned int numValues,
                         const int arrayType,
                         const void * const connectivityValues,
                         const int numNodes)
{
  mTopology = XdmfTopology::New();

  switch(topologyType) {
  case XDMF_TOPOLOGY_TYPE_POLYVERTEX:
    mTopology->setType(XdmfTopologyType::Polyvertex());
    break;
  case XDMF_TOPOLOGY_TYPE_POLYLINE:
    mTopology->setType(XdmfTopologyType::Polyline(numNodes));
    break;
  case XDMF_TOPOLOGY_TYPE_POLYGON:
    mTopology->setType(XdmfTopologyType::Polygon(numNodes));
    break;
  case XDMF_TOPOLOGY_TYPE_TRIANGLE:
    mTopology->setType(XdmfTopologyType::Triangle());
    break;
  case XDMF_TOPOLOGY_TYPE_QUADRILATERAL:
    mTopology->setType(XdmfTopologyType::Quadrilateral());
    break;
  case XDMF_TOPOLOGY_TYPE_TETRAHEDRON:
    mTopology->setType(XdmfTopologyType::Tetrahedron());
    break;
  case XDMF_TOPOLOGY_TYPE_PYRAMID:
    mTopology->setType(XdmfTopologyType::Pyramid());
    break;
  case XDMF_TOPOLOGY_TYPE_WEDGE:
    mTopology->setType(XdmfTopologyType::Wedge());
    break;
  case XDMF_TOPOLOGY_TYPE_HEXAHEDRON:
    mTopology->setType(XdmfTopologyType::Hexahedron());
    break;
  case XDMF_TOPOLOGY_TYPE_EDGE_3:
    mTopology->setType(XdmfTopologyType::Edge_3());
    break;
  case XDMF_TOPOLOGY_TYPE_TRIANGLE_6:
    mTopology->setType(XdmfTopologyType::Triangle_6());
    break;
  case XDMF_TOPOLOGY_TYPE_QUADRILATERAL_8:
    mTopology->setType(XdmfTopologyType::Quadrilateral_8());
    break;
  case XDMF_TOPOLOGY_TYPE_QUADRILATERAL_9:
    mTopology->setType(XdmfTopologyType::Quadrilateral_9());
    break;
  case XDMF_TOPOLOGY_TYPE_TETRAHEDRON_10:
    mTopology->setType(XdmfTopologyType::Tetrahedron_10());
    break;
  case XDMF_TOPOLOGY_TYPE_PYRAMID_13:
    mTopology->setType(XdmfTopologyType::Pyramid_13());
    break;
  case XDMF_TOPOLOGY_TYPE_WEDGE_15:
    mTopology->setType(XdmfTopologyType::Wedge_15());
    break;
  case XDMF_TOPOLOGY_TYPE_WEDGE_18:
    mTopology->setType(XdmfTopologyType::Wedge_18());
    break;
  case XDMF_TOPOLOGY_TYPE_HEXAHEDRON_20:
    mTopology->setType(XdmfTopologyType::Hexahedron_20());
    break;
  case XDMF_TOPOLOGY_TYPE_HEXAHEDRON_24:
    mTopology->setType(XdmfTopologyType::Hexahedron_24());
    break;
  case XDMF_TOPOLOGY_TYPE_HEXAHEDRON_27:
    mTopology->setType(XdmfTopologyType::Hexahedron_27());
    break;
  case XDMF_TOPOLOGY_TYPE_HEXAHEDRON_64:
    mTopology->setType(XdmfTopologyType::Hexahedron_64());
    break;
  case XDMF_TOPOLOGY_TYPE_HEXAHEDRON_125:
    mTopology->setType(XdmfTopologyType::Hexahedron_125());
    break;
  case XDMF_TOPOLOGY_TYPE_HEXAHEDRON_216:
    mTopology->setType(XdmfTopologyType::Hexahedron_216());
    break;
  case XDMF_TOPOLOGY_TYPE_HEXAHEDRON_343:
    mTopology->setType(XdmfTopologyType::Hexahedron_343());
    break;
  case XDMF_TOPOLOGY_TYPE_HEXAHEDRON_512:
    mTopology->setType(XdmfTopologyType::Hexahedron_512());
    break;
  case XDMF_TOPOLOGY_TYPE_HEXAHEDRON_729:
    mTopology->setType(XdmfTopologyType::Hexahedron_729());
    break;
  case XDMF_TOPOLOGY_TYPE_HEXAHEDRON_1000:
    mTopology->setType(XdmfTopologyType::Hexahedron_1000());
    break;
  case XDMF_TOPOLOGY_TYPE_HEXAHEDRON_1331:
    mTopology->setType(XdmfTopologyType::Hexahedron_1331());
    break;
  case XDMF_TOPOLOGY_TYPE_MIXED:
    mTopology->setType(XdmfTopologyType::Mixed());
    break;
  default:
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Invalid topology type.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }

  // insert connectivity values into array
  writeToArray(mTopology,
               numValues,
               arrayType,
               connectivityValues);

  const int id = mPreviousTopologies.size();
  mPreviousTopologies.push_back(mTopology);
  return id;
}







int
XdmfFortran::retrieveNumDomainGridCollections()
{
  return mDomain->getNumberGridCollections();
}

int
XdmfFortran::numGridCollectionGridCollections()
{
  if (!mGridCollections.empty()) {
    return mGridCollections.top()->getNumberGridCollections();
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: No grid collections are open."); 
    }
    catch (XdmfError e) {
      throw e;
    }
    return -1;
  }
}

void
XdmfFortran::retrieveDomainTag(char * returnTag, const int tagLength)
{
  char * tempTag = strdup(mDomain->getItemTag().c_str());
  memset(returnTag, 0, tagLength);
  memcpy(returnTag, tempTag, strlen(tempTag)+1);
  delete [] tempTag;
}

int
XdmfFortran::retrieveDomainNumProperties()
{
  return mDomain->getItemProperties().size();
}

void
XdmfFortran::retrieveDomainProperty(const int index,
                                    char * key,
                                    const int keyLength,
                                    char * value,
                                    const int valueLength)
{
  if (index < (int)mDomain->getItemProperties().size()) {
    std::map<std::string, std::string>::iterator walker = mDomain->getItemProperties().begin();
    for (int i = 0; i<index; i++) {
      walker++;
    }
    char * tempKey = strdup((*walker).first.c_str());
    memset(key, 0, keyLength);
    memcpy(key, tempKey, strlen(tempKey)+1);
    delete [] tempKey;
    char * tempValue = strdup((*walker).second.c_str());
    memset(value, 0, valueLength);
    memcpy(value, tempValue, strlen(tempValue)+1);
    delete [] tempValue;
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

void
XdmfFortran::retrieveDomainPropertyByKey(char * key,
                                         char * value,
                                         const int valueLength)
{
  std::string tempString = key;
  memset(value, 0, valueLength);
  if ((mDomain->getItemProperties().count(tempString))>0) {
    char * tempValue =
      strdup(mDomain->getItemProperties()[tempString].c_str());
    memcpy(value, tempValue, strlen(tempValue)+1);
    delete [] tempValue;
  }
}

void
XdmfFortran::removeDomainGridCollection(const int index)
{
  if ((int)mDomain->getNumberGridCollections() > index) {
    mDomain->removeGridCollection(index);
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

void
XdmfFortran::openDomainGridCollection(const int index,
                                      const int openMaps,
                                      const int openAttributes,
                                      const int openInformation,
                                      const int openSets)
{
  if ((int)mDomain->getNumberGridCollections() > index) {
    shared_ptr<XdmfGridCollection> openedGridCollection =
      mDomain->getGridCollection(index);
    int i;
    int n;
    if (openMaps) {
      n = openedGridCollection->getNumberMaps();
      shared_ptr<XdmfMap> openedMap;
      for (i = 0; i < n; i++) {
        openedMap = openedGridCollection->getMap(i);
        mMaps.push_back(openedMap);
        mPreviousMaps.push_back(openedMap);
      }
    }
    if (openAttributes) {
      n = openedGridCollection->getNumberAttributes();
      shared_ptr<XdmfAttribute> openedAttribute;
      for (i = 0; i < n; i++) {
        openedAttribute = openedGridCollection->getAttribute(i);
        mAttributes.push_back(openedAttribute);
        mPreviousAttributes.push_back(openedAttribute);
      }
    }
    if (openInformation) {
      n = openedGridCollection->getNumberInformations();
      shared_ptr<XdmfInformation> openedInformation;
      for (i = 0; i < n; i++) {
        openedInformation = openedGridCollection->getInformation(i);
        mInformations.push_back(openedInformation);
        mPreviousInformations.push_back(openedInformation);
      }
    }
    if (openSets) {
      n = openedGridCollection->getNumberSets();
      shared_ptr<XdmfSet> openedSet;
      for (i = 0; i < n; i++) {
        openedSet = openedGridCollection->getSet(i);
        mSets.push_back(openedSet);
      }
      mGridCollections.push(openedGridCollection);
    }
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

void
XdmfFortran::removeGridCollectionGridCollection(const int index)
{
  if (!mGridCollections.empty()) {
    if ((int)mGridCollections.top()->getNumberGridCollections() > index) {
      mGridCollections.top()->removeGridCollection(index);
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Index out of range.");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: No grid collections are open.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

void
XdmfFortran::openGridCollectionGridCollection(const int index,
                                              const int openMaps,
                                              const int openAttributes,
                                              const int openInformation,
                                              const int openSets)
{
  if (!mGridCollections.empty()) {
    if ((int)mGridCollections.top()->getNumberGridCollections() > index) {
      shared_ptr<XdmfGridCollection> openedGridCollection =
        mGridCollections.top()->getGridCollection(index);
      int i;
      int n;
      if (openMaps) {
        n = openedGridCollection->getNumberMaps();
        shared_ptr<XdmfMap> openedMap;
        for (i = 0; i < n; i++) {
          openedMap = openedGridCollection->getMap(i);
          mMaps.push_back(openedMap);
          mPreviousMaps.push_back(openedMap);
        }
      }
      if (openAttributes) {
        n = openedGridCollection->getNumberAttributes();
        shared_ptr<XdmfAttribute> openedAttribute;
        for (i = 0; i < n; i++) {
          openedAttribute = openedGridCollection->getAttribute(i);
          mAttributes.push_back(openedAttribute);
          mPreviousAttributes.push_back(openedAttribute);
        }
      }
      if (openInformation) {
        n = openedGridCollection->getNumberInformations();
        shared_ptr<XdmfInformation> openedInformation;
        for (i = 0; i < n; i++) {
          openedInformation = openedGridCollection->getInformation(i);
          mInformations.push_back(openedInformation);
          mPreviousInformations.push_back(openedInformation);
        }
      }
      if (openSets) {
        n = openedGridCollection->getNumberSets();
        shared_ptr<XdmfSet> openedSet;
        for (i = 0; i < n; i++) {
          openedSet = openedGridCollection->getSet(i);
          mSets.push_back(openedSet);
        }
        mGridCollections.push(openedGridCollection);
      }
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Index out of range.");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: No grid collections are open.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

void
XdmfFortran::retrieveGridCollectionTag(char * returnTag, const int tagLength)
{
  if (!mGridCollections.empty()) {
    char * tempTag = strdup(mGridCollections.top()->getItemTag().c_str());
    memset(returnTag, 0, tagLength);
    memcpy(returnTag, tempTag, strlen(tempTag)+1);
    delete [] tempTag;
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: No grid collections are open.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

void
XdmfFortran::retrieveGridCollectionName(char * returnName, const int nameLength)
{
  if (!mGridCollections.empty()) {
    char * tempName = strdup(mGridCollections.top()->getName().c_str());
    memset(returnName, 0, nameLength);
    memcpy(returnName, tempName, strlen(tempName)+1);
    delete [] tempName;
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: No grid collections are open.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

int
XdmfFortran::retrieveGridCollectionNumProperties()
{
  if (!mGridCollections.empty()) {
    return mGridCollections.top()->getItemProperties().size();
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: No grid collections are open.");
    }
    catch (XdmfError e) {
      throw e;
    }
    return -1;
  }
}

void
XdmfFortran::retrieveGridCollectionProperty(const int index, char * key, const int keyLength, char * value, const int valueLength)
{
  if (!mGridCollections.empty()) {
    if (index < (int)mGridCollections.top()->getItemProperties().size()) {
      std::map<std::string, std::string>::iterator walker =
        mGridCollections.top()->getItemProperties().begin();
      for (int i = 0; i<index; i++) {
        walker++;
      }
      char * tempKey = strdup((*walker).first.c_str());
      memset(key, 0, keyLength);
      memcpy(key, tempKey, strlen(tempKey)+1);
      delete [] tempKey;
      char * tempValue = strdup((*walker).second.c_str());
      memset(value, 0, valueLength);
      memcpy(value, tempValue, strlen(tempValue)+1);
      delete [] tempValue;
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Index out of range.");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: No grid collections are open.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

void
XdmfFortran::retrieveGridCollectionPropertyByKey(char * key,
                                                 char * value,
                                                 const int valueLength)
{
  if (!mGridCollections.empty()) {
    std::string tempString = key;
    memset(value, 0, valueLength);
    if ((mGridCollections.top()->getItemProperties().count(tempString))>0) {
      char * tempValue =
        strdup(mGridCollections.top()->getItemProperties()[tempString].c_str());
      memcpy(value, tempValue, strlen(tempValue)+1);
      delete [] tempValue;
    }
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: No grid collections are open.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

void
XdmfFortran::openDomainGrid(const int gridType,
                            const int index,
                            const int openMaps,
                            const int openAttributes,
                            const int openInformation,
                            const int openSets)
{
  if (gridType == XDMF_GRID_TYPE_CURVILINEAR) {
    if (index < (int)mDomain->getNumberCurvilinearGrids()) {
      shared_ptr<XdmfCurvilinearGrid> openedGrid =
        mDomain->getCurvilinearGrid(index);
      shared_ptr<const XdmfArrayType> dataType;
      mGeometry = openedGrid->getGeometry();
      mDimensions = openedGrid->getDimensions();
      mTime = openedGrid->getTime();
      int i;
      int n;
      if (openAttributes) {
        n = openedGrid->getNumberAttributes();
        shared_ptr<XdmfAttribute> openedAttribute;
        for (i = 0; i < n; i++) {
          openedAttribute = openedGrid->getAttribute(i);
          mAttributes.push_back(openedAttribute);
          mPreviousAttributes.push_back(openedAttribute);
        }
      }
      if (openMaps) {
        n = openedGrid->getNumberMaps();
        shared_ptr<XdmfMap> openedMap;
        for (i = 0; i < n; i++) {
          openedMap = openedGrid->getMap(i);
          mMaps.push_back(openedMap);
          mPreviousMaps.push_back(openedMap);
        }
      }
      if (openInformation) {
        n = openedGrid->getNumberInformations();
        shared_ptr<XdmfInformation> openedInformation;
        for (i = 0; i < n; i++) {
          openedInformation = openedGrid->getInformation(i);
          mInformations.push_back(openedInformation);
          mPreviousInformations.push_back(openedInformation);
        }
      }
      if (openSets) {
        n = openedGrid->getNumberSets();
        shared_ptr<XdmfSet> openedSet;
        for (i = 0; i < n; i++) {
          openedSet = openedGrid->getSet(i);
          mSets.push_back(openedSet);
        }
      }
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Index out of range.");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
  }
  else if (gridType == XDMF_GRID_TYPE_RECTILINEAR) {
    if (index < (int)mDomain->getNumberRectilinearGrids()) {
      shared_ptr<XdmfRectilinearGrid> openedGrid =
        mDomain->getRectilinearGrid(index);
      shared_ptr<const XdmfArrayType> dataType;
      mCoordinates = openedGrid->getCoordinates();
      mTime = openedGrid->getTime();
      int i;
      int n;
      if (openAttributes) {
        n = openedGrid->getNumberAttributes();
        shared_ptr<XdmfAttribute> openedAttribute;
        for (i = 0; i < n; i++) {
          openedAttribute = openedGrid->getAttribute(i);
          mAttributes.push_back(openedAttribute);
          mPreviousAttributes.push_back(openedAttribute);
        }
      }
      if (openMaps) {
        n = openedGrid->getNumberMaps();
        shared_ptr<XdmfMap> openedMap;
        for (i = 0; i < n; i++) {
          openedMap = openedGrid->getMap(i);
          mMaps.push_back(openedMap);
          mPreviousMaps.push_back(openedMap);
        }
      }
      if (openInformation) {
        n = openedGrid->getNumberInformations();
        shared_ptr<XdmfInformation> openedInformation;
        for (i = 0; i < n; i++) {
          openedInformation = openedGrid->getInformation(i);
          mInformations.push_back(openedInformation);
          mPreviousInformations.push_back(openedInformation);
        }
      }
      if (openSets) {
        n = openedGrid->getNumberSets();
        shared_ptr<XdmfSet> openedSet;
        for (i = 0; i < n; i++) {
          openedSet = openedGrid->getSet(i);
          mSets.push_back(openedSet);
        }
      }
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Index out of range.");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
  }
  else if (gridType == XDMF_GRID_TYPE_REGULAR) {
    if (index < (int)mDomain->getNumberRegularGrids()) {
      shared_ptr<XdmfRegularGrid> openedGrid =
        mDomain->getRegularGrid(index);
      shared_ptr<const XdmfArrayType> dataType;
      mTime = openedGrid->getTime();
      mBrick = openedGrid->getBrickSize();
      mOrigin = openedGrid->getOrigin();
      mDimensions = openedGrid->getDimensions();
      int i;
      int n;
      if (openAttributes) {
        n = openedGrid->getNumberAttributes();
        shared_ptr<XdmfAttribute> openedAttribute;
        for (i = 0; i < n; i++) {
          openedAttribute = openedGrid->getAttribute(i);
          mAttributes.push_back(openedAttribute);
          mPreviousAttributes.push_back(openedAttribute);
        }
      }
      if (openMaps) {
        n = openedGrid->getNumberMaps();
        shared_ptr<XdmfMap> openedMap;
        for (i = 0; i < n; i++) {
          openedMap = openedGrid->getMap(i);
          mMaps.push_back(openedMap);
          mPreviousMaps.push_back(openedMap);
        }
      }
      if (openInformation) {
        n = openedGrid->getNumberInformations();
        shared_ptr<XdmfInformation> openedInformation;
        for (i = 0; i < n; i++) {
          openedInformation = openedGrid->getInformation(i);
          mInformations.push_back(openedInformation);
          mPreviousInformations.push_back(openedInformation);
        }
      }
      if (openSets) {
        n = openedGrid->getNumberSets();
        shared_ptr<XdmfSet> openedSet;
        for (i = 0; i < n; i++) {
          openedSet = openedGrid->getSet(i);
          mSets.push_back(openedSet);
        }
      }
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Index out of range.");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
  }
  else if (gridType == XDMF_GRID_TYPE_UNSTRUCTURED) {
    if (index < (int)mDomain->getNumberUnstructuredGrids()) {
      shared_ptr<XdmfUnstructuredGrid> openedGrid =
        mDomain->getUnstructuredGrid(index);
      mTopology = openedGrid->getTopology();
      mGeometry = openedGrid->getGeometry();
      mTime = openedGrid->getTime();
      int i;
      int n;
      if (openAttributes) {
        n = openedGrid->getNumberAttributes();
        shared_ptr<XdmfAttribute> openedAttribute;
        for (i = 0; i < n; i++) {
          openedAttribute = openedGrid->getAttribute(i);
          mAttributes.push_back(openedAttribute);
          mPreviousAttributes.push_back(openedAttribute);
        }
      }
      if (openMaps) {
        n = openedGrid->getNumberMaps();
        shared_ptr<XdmfMap> openedMap;
        for (i = 0; i < n; i++) {
          openedMap = openedGrid->getMap(i);
          mMaps.push_back(openedMap);
          mPreviousMaps.push_back(openedMap);
        }
      }
      if (openInformation) {
        n = openedGrid->getNumberInformations();
        shared_ptr<XdmfInformation> openedInformation;
        for (i = 0; i < n; i++) {
          openedInformation = openedGrid->getInformation(i);
          mInformations.push_back(openedInformation);
          mPreviousInformations.push_back(openedInformation);
        }
      }
      if (openSets) {
        n = openedGrid->getNumberSets();
        shared_ptr<XdmfSet> openedSet;
        for (i = 0; i < n; i++) {
          openedSet = openedGrid->getSet(i);
          mSets.push_back(openedSet);
        }
      }
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Index out of range.");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Invalid Grid Type.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

void
XdmfFortran::removeDomainGrid(const int gridType, const int index)
{
  if (gridType == XDMF_GRID_TYPE_CURVILINEAR) {
    if (index < (int)mDomain->getNumberCurvilinearGrids()) {
      mDomain->removeCurvilinearGrid(index);
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Index out of range.");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
  }
  else if (gridType == XDMF_GRID_TYPE_RECTILINEAR) {
    if (index < (int)mDomain->getNumberRectilinearGrids()) {
      mDomain->removeRectilinearGrid(index);
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Index out of range.");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
  }
  else if (gridType == XDMF_GRID_TYPE_REGULAR) {
    if (index < (int)mDomain->getNumberRegularGrids()) {
      mDomain->removeRegularGrid(index);
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Index out of range.");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
  }
  else if (gridType == XDMF_GRID_TYPE_UNSTRUCTURED) {
    if (index < (int)mDomain->getNumberUnstructuredGrids()) {
      mDomain->removeUnstructuredGrid(index);
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Index out of range.");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Invalid Grid Type.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

void
XdmfFortran::replaceDomainGrid(const int gridType, const int index, char * name)
{
  int i;
  if (gridType == XDMF_GRID_TYPE_CURVILINEAR) {
    if (index < (int)mDomain->getNumberCurvilinearGrids()) {
      if(mDimensions == NULL) {
        try {
          XdmfError::message(XdmfError::FATAL,
                             "Must set dimensions before replacing grid.");
        }
        catch (XdmfError e) {
          throw e;
        }
      }
      if(mGeometry == NULL) {
        try {
          XdmfError::message(XdmfError::FATAL, 
            "Must set geometry before adding grid.");
        }
        catch (XdmfError e) {
          throw e;
        }
      }

      shared_ptr<XdmfCurvilinearGrid> grid = mDomain->getCurvilinearGrid(index);
      grid->setName(name);

      grid->setGeometry(mGeometry);
      grid->setDimensions(mDimensions);

      for (i=grid->getNumberAttributes()-1;i>=0;i--) {
        grid->removeAttribute(0);
      }

      for(std::vector<shared_ptr<XdmfAttribute> >::const_iterator iter =
          mAttributes.begin(); 
          iter != mAttributes.end();
          ++iter) {
        grid->insert(*iter);
      }

      mAttributes.clear();

      for( i=grid->getNumberInformations()-1;i>=0;i--) {
        grid->removeInformation(0);
      }

      for(std::vector<shared_ptr<XdmfInformation> >::const_iterator iter =
          mInformations.begin(); 
          iter != mInformations.end();
          ++iter) {
        grid->insert(*iter);
      }

      mInformations.clear();

      for (i=grid->getNumberSets()-1;i>=0;i--) {
        grid->removeSet(0);
      }

      for(std::vector<shared_ptr<XdmfSet> >::const_iterator iter =
          mSets.begin(); 
          iter != mSets.end();
          ++iter) {
        grid->insert(*iter);
      }

      mSets.clear();

      for (i=grid->getNumberMaps()-1;i>=0;i--) {
        grid->removeMap(0);
      }

      for(std::vector<shared_ptr<XdmfMap> >::const_iterator iter =
          mMaps.begin(); 
          iter != mMaps.end();
          ++iter) {
        grid->insert(*iter);
      }

      mMaps.clear();
    
      if(mTime) {
        grid->setTime(mTime);
      }
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Index out of range.");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
  }
  else if (gridType == XDMF_GRID_TYPE_RECTILINEAR) {
    if (index < (int)mDomain->getNumberRectilinearGrids()) {
      if(mCoordinates.empty()) {
        try {
          XdmfError::message(XdmfError::FATAL, 
            "Must set Coordinates before adding grid.");
        }
        catch (XdmfError e) {
          throw e;
        }
      }

      shared_ptr<XdmfRectilinearGrid> grid = mDomain->getRectilinearGrid(index);
      grid->setCoordinates(mCoordinates);
      mCoordinates.clear();
      for (i=grid->getNumberAttributes()-1;i>=0;i--) {
        grid->removeAttribute(0);
      }

      for(std::vector<shared_ptr<XdmfAttribute> >::const_iterator iter =
          mAttributes.begin(); 
          iter != mAttributes.end();
          ++iter) {
        grid->insert(*iter);
      }

      mAttributes.clear();

      for (i=grid->getNumberInformations()-1;i>=0;i--) {
        grid->removeInformation(0);
      }

      for(std::vector<shared_ptr<XdmfInformation> >::const_iterator iter =
          mInformations.begin(); 
          iter != mInformations.end();
          ++iter) {
        grid->insert(*iter);
      }

      mInformations.clear();

      for (i=grid->getNumberSets()-1;i>=0;i--) {
        grid->removeSet(0);
      }

      for(std::vector<shared_ptr<XdmfSet> >::const_iterator iter =
          mSets.begin(); 
          iter != mSets.end();
          ++iter) {
        grid->insert(*iter);
      }

      mSets.clear();

      for (i=grid->getNumberMaps()-1;i>=0;i--) {
        grid->removeMap(0);
      }

      for(std::vector<shared_ptr<XdmfMap> >::const_iterator iter =
          mMaps.begin(); 
          iter != mMaps.end();
          ++iter) {
        grid->insert(*iter);
      }

      mMaps.clear();
    
      if(mTime) {
        grid->setTime(mTime);
      }
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Index out of range.");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
  }
  else if (gridType == XDMF_GRID_TYPE_REGULAR) {
    if (index < (int)mDomain->getNumberRegularGrids()) {
      if(mBrick == NULL) {
        try {
          XdmfError::message(XdmfError::FATAL, 
            "Must set brick size before adding grid.");
        }
        catch (XdmfError e) {
          throw e;
        }
      }

      if(mDimensions == NULL) {
        try {
          XdmfError::message(XdmfError::FATAL, 
            "Must set dimensions before adding grid.");
        }
        catch (XdmfError e) {
          throw e;
        }
      }

      if(mOrigin == NULL) {
        try {
          XdmfError::message(XdmfError::FATAL, 
            "Must set origin before adding grid.");
        }
        catch (XdmfError e) {
          throw e;
        }
      }
  
      shared_ptr<XdmfRegularGrid> grid = mDomain->getRegularGrid(index);
      grid->setOrigin(mOrigin);
      grid->setDimensions(mDimensions);
      grid->setBrickSize(mBrick);

      for (i=grid->getNumberAttributes()-1;i>=0;i--) {
        grid->removeAttribute(0);
      }

      for(std::vector<shared_ptr<XdmfAttribute> >::const_iterator iter =
          mAttributes.begin(); 
          iter != mAttributes.end();
          ++iter) {
        grid->insert(*iter);
      }

      mAttributes.clear();

      for (i=grid->getNumberInformations()-1;i>=0;i--) {
        grid->removeInformation(0);
      }

      for(std::vector<shared_ptr<XdmfInformation> >::const_iterator iter =
          mInformations.begin(); 
          iter != mInformations.end();
          ++iter) {
        grid->insert(*iter);
      }

      mInformations.clear();

      for (i=grid->getNumberSets()-1;i>=0;i--) {
        grid->removeSet(0);
      }

      for(std::vector<shared_ptr<XdmfSet> >::const_iterator iter =
          mSets.begin(); 
          iter != mSets.end();
          ++iter) {
        grid->insert(*iter);
      }

      mSets.clear();

      for (i=grid->getNumberMaps()-1;i>=0;i--) {
        grid->removeMap(0);
      }

      for(std::vector<shared_ptr<XdmfMap> >::const_iterator iter =
          mMaps.begin(); 
          iter != mMaps.end();
          ++iter) {
        grid->insert(*iter);
      }

      mMaps.clear();
    
      if(mTime) {
        grid->setTime(mTime);
      }
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Index out of range.");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
  }
  else if (gridType == XDMF_GRID_TYPE_UNSTRUCTURED) {
    if (index < (int)mDomain->getNumberUnstructuredGrids()) {
      if(mGeometry == NULL) {
        try {
          XdmfError::message(XdmfError::FATAL, 
            "Must set geometry before adding grid.");
        }
        catch (XdmfError e) {
          throw e;
        }
      }
      if(mTopology == NULL) {
        try {
          XdmfError::message(XdmfError::FATAL, 
            "Must set topology before adding grid.");
        }
        catch (XdmfError e) {
          throw e;
        }
      }
      shared_ptr<XdmfUnstructuredGrid> grid = mDomain->getUnstructuredGrid(index);
      grid->setName(name);
      grid->setGeometry(mGeometry);
      grid->setTopology(mTopology);

      for (i=grid->getNumberAttributes()-1;i>=0;i--) {
        grid->removeAttribute(0);
      }

      for(std::vector<shared_ptr<XdmfAttribute> >::const_iterator iter =
          mAttributes.begin(); 
          iter != mAttributes.end();
          ++iter) {
        grid->insert(*iter);
      }

      mAttributes.clear();

      for (i=grid->getNumberInformations()-1;i>=0;i--) {
        grid->removeInformation(0);
      }

      for(std::vector<shared_ptr<XdmfInformation> >::const_iterator iter =
          mInformations.begin(); 
          iter != mInformations.end();
          ++iter) {
        grid->insert(*iter);
      }

      mInformations.clear();

      for (i=grid->getNumberSets()-1;i>=0;i--) {
        grid->removeSet(0);
      }

      for(std::vector<shared_ptr<XdmfSet> >::const_iterator iter =
          mSets.begin(); 
          iter != mSets.end();
          ++iter) {
        grid->insert(*iter);
      }

      mSets.clear();

      for (i=grid->getNumberMaps()-1;i>=0;i--) {
        grid->removeMap(0);
      }

      for(std::vector<shared_ptr<XdmfMap> >::const_iterator iter =
          mMaps.begin(); 
          iter != mMaps.end();
          ++iter) {
        grid->insert(*iter);
      }

      mMaps.clear();
    
      if(mTime) {
        grid->setTime(mTime);
      }
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Index out of range.");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
  }
}

void
XdmfFortran::retrieveDomainGridName(const int gridType, const int index, char * returnName, const int nameLength)
{
  shared_ptr<XdmfGrid> openedGrid;
  if (gridType == XDMF_GRID_TYPE_CURVILINEAR) {
    if (index < (int)mDomain->getNumberCurvilinearGrids()) {
      openedGrid = mDomain->getCurvilinearGrid(index);
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Index out of range.");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
  }
  else if (gridType == XDMF_GRID_TYPE_RECTILINEAR) {
    if (index < (int)mDomain->getNumberRectilinearGrids()) {
      openedGrid = mDomain->getRectilinearGrid(index);
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Index out of range.");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
  }
  else if (gridType == XDMF_GRID_TYPE_REGULAR) {
    if (index < (int)mDomain->getNumberRegularGrids()) {
      openedGrid = mDomain->getRegularGrid(index);
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Index out of range.");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
  }
  else if (gridType == XDMF_GRID_TYPE_UNSTRUCTURED) {
    if (index < (int)mDomain->getNumberUnstructuredGrids()) {
      openedGrid = mDomain->getUnstructuredGrid(index);
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Index out of range.");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Invalid Grid Type.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
  char * tempName = strdup(openedGrid->getName().c_str());
  memset(returnName, 0, nameLength);
  memcpy(returnName, tempName, strlen(tempName)+1);
  delete [] tempName;
}

void
XdmfFortran::retrieveDomainGridTag(const int gridType,
                                   const int index,
                                   char * returnTag,
                                   const int tagLength)
{
  shared_ptr<XdmfGrid> openedGrid;
  if (gridType == XDMF_GRID_TYPE_CURVILINEAR) {
    if (index < (int)mDomain->getNumberCurvilinearGrids()) {
      openedGrid = mDomain->getCurvilinearGrid(index);
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Index out of range.");
      }
      catch (XdmfError e)
      {
        throw e;
      }
    }
  }
  else if (gridType == XDMF_GRID_TYPE_RECTILINEAR) {
    if (index < (int)mDomain->getNumberRectilinearGrids()) {
      openedGrid = mDomain->getRectilinearGrid(index);
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Index out of range.");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
  }
  else if (gridType == XDMF_GRID_TYPE_REGULAR) {
    if (index < (int)mDomain->getNumberRegularGrids()) {
      openedGrid = mDomain->getRegularGrid(index);
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Index out of range.");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
  }
  else if (gridType == XDMF_GRID_TYPE_UNSTRUCTURED) {
    if (index < (int)mDomain->getNumberUnstructuredGrids()) {
      openedGrid = mDomain->getUnstructuredGrid(index);
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Index out of range.");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Invalid Grid Type.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
  char * tempTag = strdup(openedGrid->getItemTag().c_str());
  memset(returnTag, 0, tagLength);
  memcpy(returnTag, tempTag, strlen(tempTag)+1);
  delete [] tempTag;
}

int
XdmfFortran::retrieveDomainGridNumProperties(const int gridType, const int index)
{
  if (gridType == XDMF_GRID_TYPE_CURVILINEAR) {
    if (index < (int)mDomain->getNumberCurvilinearGrids()) {
      return mDomain->getCurvilinearGrid(index)->getItemProperties().size();
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Index out of range.");
      }
      catch (XdmfError e) {
        throw e;
      }
      return -1;
    }
  }
  else if (gridType == XDMF_GRID_TYPE_RECTILINEAR) {
    if (index < (int)mDomain->getNumberRectilinearGrids()) {
      return mDomain->getRectilinearGrid(index)->getItemProperties().size();
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Index out of range.");
      }
      catch (XdmfError e) {
        throw e;
      }
      return -1;
    }
  }
  else if (gridType == XDMF_GRID_TYPE_REGULAR) {
    if (index < (int)mDomain->getNumberRegularGrids()) {
      return mDomain->getRegularGrid(index)->getItemProperties().size();
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Index out of range.");
      }
      catch (XdmfError e) {
        throw e;
      }
      return -1;
    }
  }
  else if (gridType == XDMF_GRID_TYPE_UNSTRUCTURED) {
    if (index < (int)mDomain->getNumberUnstructuredGrids()) {
      return mDomain->getUnstructuredGrid(index)->getItemProperties().size();
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Index out of range.");
      }
      catch (XdmfError e) {
        throw e;
      }
      return -1;
    }
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Invalid Grid Type.");
    }
    catch (XdmfError e) {
      throw e;
    }
    return -1;
  }
}

void
XdmfFortran::retrieveDomainGridProperty(const int gridType,
                                        const int gridIndex,
                                        const int index,
                                        char * key,
                                        const int keyLength,
                                        char * value,
                                        const int valueLength)
{
  shared_ptr<XdmfGrid> openedGrid;
  if (gridType == XDMF_GRID_TYPE_CURVILINEAR) {
    if (index < (int)mDomain->getNumberCurvilinearGrids()) {
      openedGrid = mDomain->getCurvilinearGrid(index);
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Index out of range.");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
  }
  else if (gridType == XDMF_GRID_TYPE_RECTILINEAR) {
    if (index < (int)mDomain->getNumberRectilinearGrids()) {
      openedGrid = mDomain->getRectilinearGrid(index);
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Index out of range.");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
  }
  else if (gridType == XDMF_GRID_TYPE_REGULAR) {
    if (index < (int)mDomain->getNumberRegularGrids()) {
      openedGrid = mDomain->getRegularGrid(index);
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Index out of range.");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
  }
  else if (gridType == XDMF_GRID_TYPE_UNSTRUCTURED) {
    if (index < (int)mDomain->getNumberUnstructuredGrids()) {
      openedGrid = mDomain->getUnstructuredGrid(index);
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Index out of range.");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Invalid Grid Type.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
  if (index < (int)openedGrid->getItemProperties().size()) {
    std::map<std::string, std::string>::iterator walker =
      openedGrid->getItemProperties().begin();
    for (int i = 0; i<index; i++) {
      walker++;
    }
    char * tempKey = strdup((*walker).first.c_str());
    memset(key, 0, keyLength);
    memcpy(key, tempKey, strlen(tempKey)+1);
    delete [] tempKey;
    char * tempValue = strdup((*walker).second.c_str());
    memset(value, 0, valueLength);
    memcpy(value, tempValue, strlen(tempValue)+1);
    delete [] tempValue;
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

void
XdmfFortran::retrieveDomainGridPropertyByKey(int gridType,
                                             int index,
                                             char * key,
                                             char * value,
                                             int valueLength)
{
  shared_ptr<XdmfGrid> openedGrid;
  if (gridType == XDMF_GRID_TYPE_CURVILINEAR) {
    if (index < (int)mDomain->getNumberCurvilinearGrids()) {
      openedGrid = mDomain->getCurvilinearGrid(index);
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Index out of range.");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
  }
  else if (gridType == XDMF_GRID_TYPE_RECTILINEAR) {
    if (index < (int)mDomain->getNumberRectilinearGrids()) {
      openedGrid = mDomain->getRectilinearGrid(index);
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Index out of range.");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
  }
  else if (gridType == XDMF_GRID_TYPE_REGULAR) {
    if (index < (int)mDomain->getNumberRegularGrids()) {
      openedGrid = mDomain->getRegularGrid(index);
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Index out of range.");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
  }
  else if (gridType == XDMF_GRID_TYPE_UNSTRUCTURED) {
    if (index < (int)mDomain->getNumberUnstructuredGrids()) {
      openedGrid = mDomain->getUnstructuredGrid(index);
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Index out of range.");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Invalid Grid Type.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
  std::string tempString = key;
  memset(value, 0, valueLength);
  if ((openedGrid->getItemProperties().count(tempString))>0) {
    char * tempValue = strdup(openedGrid->getItemProperties()[tempString].c_str());
    memcpy(value, tempValue, strlen(tempValue)+1);
    delete [] tempValue;
  }
}

void
XdmfFortran::openGridCollectionGrid(const int gridType,
                                    const int index,
                                    const int openMaps,
                                    const int openAttributes,
                                    const int openInformation,
                                    const int openSets)
{
  if (!mGridCollections.empty()) {
    if (gridType == XDMF_GRID_TYPE_CURVILINEAR) {
      if (index < (int)mGridCollections.top()->getNumberCurvilinearGrids()) {
        shared_ptr<XdmfCurvilinearGrid> openedGrid =
          mGridCollections.top()->getCurvilinearGrid(index);
        shared_ptr<const XdmfArrayType> dataType;
        mGeometry = openedGrid->getGeometry();
        mDimensions = openedGrid->getDimensions();
        mTime = openedGrid->getTime();
        int i;
        int n;
        if (openAttributes) {
          n = openedGrid->getNumberAttributes();
          shared_ptr<XdmfAttribute> openedAttribute;
          for (i = 0; i < n; i++) {
            openedAttribute = openedGrid->getAttribute(i);
            mAttributes.push_back(openedAttribute);
            mPreviousAttributes.push_back(openedAttribute);
          }
        }
        if (openMaps) {
          n = openedGrid->getNumberMaps();
          shared_ptr<XdmfMap> openedMap;
          for (i = 0; i < n; i++) {
            openedMap = openedGrid->getMap(i);
            mMaps.push_back(openedMap);
            mPreviousMaps.push_back(openedMap);
          }
        }
        if (openInformation) {
          n = openedGrid->getNumberInformations();
          shared_ptr<XdmfInformation> openedInformation;
          for (i = 0; i < n; i++) {
            openedInformation = openedGrid->getInformation(i);
            mInformations.push_back(openedInformation);
            mPreviousInformations.push_back(openedInformation);
          }
        }
        if (openSets) {
          n = openedGrid->getNumberSets();
          shared_ptr<XdmfSet> openedSet;
          for (i = 0; i < n; i++) {
            openedSet = openedGrid->getSet(i);
            mSets.push_back(openedSet);
          }
        }
      }
      else {
        try {
          XdmfError::message(XdmfError::FATAL,
                             "Error: Index out of range.");
        }
        catch (XdmfError e) {
          throw e;
        }
      }
    }
    else if (gridType == XDMF_GRID_TYPE_RECTILINEAR) {
      if (index < (int)mGridCollections.top()->getNumberRectilinearGrids()) {
        shared_ptr<XdmfRectilinearGrid> openedGrid =
          mGridCollections.top()->getRectilinearGrid(index);
        shared_ptr<const XdmfArrayType> dataType;
        mCoordinates = openedGrid->getCoordinates();
        mTime = openedGrid->getTime();
        int i;
        int n;
        if (openAttributes) {
          n = openedGrid->getNumberAttributes();
          shared_ptr<XdmfAttribute> openedAttribute;
          for (i = 0; i < n; i++) {
            openedAttribute = openedGrid->getAttribute(i);
            mAttributes.push_back(openedAttribute);
            mPreviousAttributes.push_back(openedAttribute);
          }
        }
        if (openMaps) {
          n = openedGrid->getNumberMaps();
          shared_ptr<XdmfMap> openedMap;
          for (i = 0; i < n; i++) {
            openedMap = openedGrid->getMap(i);
            mMaps.push_back(openedMap);
            mPreviousMaps.push_back(openedMap);
          }
        }
        if (openInformation) {
          n = openedGrid->getNumberInformations();
          shared_ptr<XdmfInformation> openedInformation;
          for (i = 0; i < n; i++) {
            openedInformation = openedGrid->getInformation(i);
            mInformations.push_back(openedInformation);
            mPreviousInformations.push_back(openedInformation);
          }
        }
        if (openSets) {
          n = openedGrid->getNumberSets();
          shared_ptr<XdmfSet> openedSet;
          for (i = 0; i < n; i++) {
            openedSet = openedGrid->getSet(i);
            mSets.push_back(openedSet);
          }
        }
      }
      else {
        try {
          XdmfError::message(XdmfError::FATAL,
                             "Error: Index out of range.");
        }
        catch (XdmfError e) {
          throw e;
        }
      }
    }
    else if (gridType == XDMF_GRID_TYPE_REGULAR) {
      if (index < (int)mGridCollections.top()->getNumberRegularGrids()) {
        shared_ptr<XdmfRegularGrid> openedGrid =
          mGridCollections.top()->getRegularGrid(index);
        shared_ptr<const XdmfArrayType> dataType;
        mTime = openedGrid->getTime();
        mBrick = openedGrid->getBrickSize();
        mOrigin = openedGrid->getOrigin();
        mDimensions = openedGrid->getDimensions();
        int i;
        int n;
        if (openAttributes) {
          n = openedGrid->getNumberAttributes();
          shared_ptr<XdmfAttribute> openedAttribute;
          for (i = 0; i < n; i++) {
            openedAttribute = openedGrid->getAttribute(i);
            mAttributes.push_back(openedAttribute);
            mPreviousAttributes.push_back(openedAttribute);
          }
        }
        if (openMaps) {
          n = openedGrid->getNumberMaps();
          shared_ptr<XdmfMap> openedMap;
          for (i = 0; i < n; i++) {
            openedMap = openedGrid->getMap(i);
            mMaps.push_back(openedMap);
            mPreviousMaps.push_back(openedMap);
          }
        }
        if (openInformation) {
          n = openedGrid->getNumberInformations();
          shared_ptr<XdmfInformation> openedInformation;
          for (i = 0; i < n; i++) {
            openedInformation = openedGrid->getInformation(i);
            mInformations.push_back(openedInformation);
            mPreviousInformations.push_back(openedInformation);
          }
        }
        if (openSets) {
          n = openedGrid->getNumberSets();
          shared_ptr<XdmfSet> openedSet;
          for (i = 0; i < n; i++) {
            openedSet = openedGrid->getSet(i);
            mSets.push_back(openedSet);
          }
        }
      }
      else {
        try {
          XdmfError::message(XdmfError::FATAL,
                             "Error: Index out of range.");
        }
        catch (XdmfError e) {
          throw e;
        }
      }
    }
    else if (gridType == XDMF_GRID_TYPE_UNSTRUCTURED) {
      if (index < (int)mGridCollections.top()->getNumberUnstructuredGrids()) {
        shared_ptr<XdmfUnstructuredGrid> openedGrid =
          mGridCollections.top()->getUnstructuredGrid(index);
        mTopology = openedGrid->getTopology();
        mGeometry = openedGrid->getGeometry();
        mTime = openedGrid->getTime();
        int i;
        int n;
        if (openAttributes) {
          n = openedGrid->getNumberAttributes();
          shared_ptr<XdmfAttribute> openedAttribute;
          for (i = 0; i < n; i++) {
            openedAttribute = openedGrid->getAttribute(i);
            mAttributes.push_back(openedAttribute);
            mPreviousAttributes.push_back(openedAttribute);
          }
        }
        if (openMaps) {
          n = openedGrid->getNumberMaps();
          shared_ptr<XdmfMap> openedMap;
          for (i = 0; i < n; i++) {
            openedMap = openedGrid->getMap(i);
            mMaps.push_back(openedMap);
            mPreviousMaps.push_back(openedMap);
          }
        }
        if (openInformation) {
          n = openedGrid->getNumberInformations();
          shared_ptr<XdmfInformation> openedInformation;
          for (i = 0; i < n; i++) {
            openedInformation = openedGrid->getInformation(i);
            mInformations.push_back(openedInformation);
            mPreviousInformations.push_back(openedInformation);
          }
        }
        if (openSets) {
          n = openedGrid->getNumberSets();
          shared_ptr<XdmfSet> openedSet;
          for (i = 0; i < n; i++) {
            openedSet = openedGrid->getSet(i);
            mSets.push_back(openedSet);
          }
        }
      }
      else {
        try {
          XdmfError::message(XdmfError::FATAL,
                             "Error: Index out of range.");
        }
        catch (XdmfError e) {
          throw e;
        }
      }
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Invalid Grid Type.");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "There is no grid collection currently loaded.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

void
XdmfFortran::removeGridCollectionGrid(const int gridType, const int index)
{
  if (!mGridCollections.empty()) {
    if (gridType == XDMF_GRID_TYPE_CURVILINEAR) {
      if (index < (int)mGridCollections.top()->getNumberCurvilinearGrids()) {
        mGridCollections.top()->removeCurvilinearGrid(index);
      }
      else {
        try {
          XdmfError::message(XdmfError::FATAL,
                             "Error: Index out of range.");
        }
        catch (XdmfError e) {
          throw e;
        }
      }
    }
    else if (gridType == XDMF_GRID_TYPE_RECTILINEAR) {
      if (index < (int)mGridCollections.top()->getNumberRectilinearGrids()) {
        mGridCollections.top()->removeRectilinearGrid(index);
      }
      else {
        try {
          XdmfError::message(XdmfError::FATAL,
                             "Error: Index out of range.");
        }
        catch (XdmfError e) {
          throw e;
        }
      }
    }
    else if (gridType == XDMF_GRID_TYPE_REGULAR) {
      if (index < (int)mGridCollections.top()->getNumberRegularGrids()) {
        mGridCollections.top()->removeRegularGrid(index);
      }
      else {
        try {
          XdmfError::message(XdmfError::FATAL,
                             "Error: Index out of range.");
        }
        catch (XdmfError e) {
          throw e;
        }
      }
    }
    else if (gridType == XDMF_GRID_TYPE_UNSTRUCTURED) {
      if (index < (int)mGridCollections.top()->getNumberUnstructuredGrids()) {
        mGridCollections.top()->removeUnstructuredGrid(index);
      }
      else {
        try {
          XdmfError::message(XdmfError::FATAL,
                             "Error: Index out of range.");
        }
        catch (XdmfError e) {
          throw e;
        }
      }
    }
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "There is no grid collection currently loaded.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

void
XdmfFortran::replaceGridCollectionGrid(const int gridType, const int index, char * name)
{
  if (!mGridCollections.empty()) {
    int i;
    if (gridType == XDMF_GRID_TYPE_CURVILINEAR) {
      if (index < (int)mGridCollections.top()->getNumberCurvilinearGrids()) {
        if(mDimensions == NULL) {
          try {
            XdmfError::message(XdmfError::FATAL,
                               "Must set dimensions before replacing grid.");
          }
          catch (XdmfError e) {
            throw e;
          }
        }
        if(mGeometry == NULL) {
          try {
            XdmfError::message(XdmfError::FATAL, 
                               "Must set geometry before adding grid.");
          }
          catch (XdmfError e) {
            throw e;
          }
        }

        shared_ptr<XdmfCurvilinearGrid> grid =
          mGridCollections.top()->getCurvilinearGrid(index);
        grid->setName(name);

        grid->setGeometry(mGeometry);
        grid->setDimensions(mDimensions);

        for (i=grid->getNumberAttributes()-1;i>=0;i--) {
          grid->removeAttribute(0);
        }

        for(std::vector<shared_ptr<XdmfAttribute> >::const_iterator iter =
            mAttributes.begin(); 
            iter != mAttributes.end();
            ++iter) {
          grid->insert(*iter);
        }

        mAttributes.clear();

        for( i=grid->getNumberInformations()-1;i>=0;i--) {
          grid->removeInformation(0);
        }

        for(std::vector<shared_ptr<XdmfInformation> >::const_iterator iter =
            mInformations.begin(); 
            iter != mInformations.end();
            ++iter) {
          grid->insert(*iter);
        }

        mInformations.clear();

        for (i=grid->getNumberSets()-1;i>=0;i--) {
          grid->removeSet(0);
        }

        for(std::vector<shared_ptr<XdmfSet> >::const_iterator iter =
            mSets.begin(); 
            iter != mSets.end();
            ++iter) {
          grid->insert(*iter);
        }

        mSets.clear();

        for (i=grid->getNumberMaps()-1;i>=0;i--) {
          grid->removeMap(0);
        }

        for(std::vector<shared_ptr<XdmfMap> >::const_iterator iter =
            mMaps.begin(); 
            iter != mMaps.end();
            ++iter) {
          grid->insert(*iter);
        }

        mMaps.clear();
    
        if(mTime) {
          grid->setTime(mTime);
        }
      }
      else {
        try {
          XdmfError::message(XdmfError::FATAL,
                             "Error: Index out of range."); 
        }
        catch (XdmfError e) {
          throw e;
        }
      }
    }
    else if (gridType == XDMF_GRID_TYPE_RECTILINEAR) {
      if (index < (int)mGridCollections.top()->getNumberRectilinearGrids()) {
        if(mCoordinates.empty()) {
          try {
            XdmfError::message(XdmfError::FATAL, 
                               "Must set Coordinates before adding grid.");
          }
          catch (XdmfError e) {
            throw e;
          }
        }

        shared_ptr<XdmfRectilinearGrid> grid =
          mGridCollections.top()->getRectilinearGrid(index);
        grid->setCoordinates(mCoordinates);
        mCoordinates.clear();
        for (i=grid->getNumberAttributes()-1;i>=0;i--) {
          grid->removeAttribute(0);
        }

        for(std::vector<shared_ptr<XdmfAttribute> >::const_iterator iter =
            mAttributes.begin(); 
            iter != mAttributes.end();
            ++iter) {
          grid->insert(*iter);
        }

        mAttributes.clear();

        for (i=grid->getNumberInformations()-1;i>=0;i--) {
          grid->removeInformation(0);
        }

        for(std::vector<shared_ptr<XdmfInformation> >::const_iterator iter =
            mInformations.begin(); 
            iter != mInformations.end();
            ++iter) {
          grid->insert(*iter);
        }

        mInformations.clear();

        for (i=grid->getNumberSets()-1;i>=0;i--) {
          grid->removeSet(0);
        }

        for(std::vector<shared_ptr<XdmfSet> >::const_iterator iter =
            mSets.begin(); 
            iter != mSets.end();
            ++iter) {
          grid->insert(*iter);
        }

        mSets.clear();

        for (i=grid->getNumberMaps()-1;i>=0;i--) {
          grid->removeMap(0);
        }

        for(std::vector<shared_ptr<XdmfMap> >::const_iterator iter =
            mMaps.begin(); 
            iter != mMaps.end();
            ++iter) {
          grid->insert(*iter);
        }

        mMaps.clear();
    
        if(mTime) {
          grid->setTime(mTime);
        }
      }
      else {
        try {
          XdmfError::message(XdmfError::FATAL,
                             "Error: Index out of range.");
        }
        catch (XdmfError e) {
          throw e;
        }
      }
    }
    else if (gridType == XDMF_GRID_TYPE_REGULAR) {
      if (index < (int)mGridCollections.top()->getNumberRegularGrids()) {
        if(mBrick == NULL) {
          try {
            XdmfError::message(XdmfError::FATAL, 
                               "Must set brick size before adding grid.");
          }
          catch (XdmfError e) {
            throw e;
          }
        }

        if(mDimensions == NULL) {
          try {
            XdmfError::message(XdmfError::FATAL, 
                               "Must set dimensions before adding grid.");
          }
          catch (XdmfError e) {
            throw e;
          }
        }

        if(mOrigin == NULL) {
          try {
            XdmfError::message(XdmfError::FATAL, 
                               "Must set origin before adding grid.");
          }
          catch (XdmfError e) {
            throw e;
          }
        }
  
        shared_ptr<XdmfRegularGrid> grid = mGridCollections.top()->getRegularGrid(index);
        grid->setOrigin(mOrigin);
        grid->setDimensions(mDimensions);
        grid->setBrickSize(mBrick);

        for (i=grid->getNumberAttributes()-1;i>=0;i--) {
          grid->removeAttribute(0);
        }

        for(std::vector<shared_ptr<XdmfAttribute> >::const_iterator iter =
            mAttributes.begin(); 
            iter != mAttributes.end();
            ++iter) {
          grid->insert(*iter);
        }

        mAttributes.clear();

        for (i=grid->getNumberInformations()-1;i>=0;i--) {
          grid->removeInformation(0);
        }

        for(std::vector<shared_ptr<XdmfInformation> >::const_iterator iter =
            mInformations.begin(); 
            iter != mInformations.end();
            ++iter) {
          grid->insert(*iter);
        }

        mInformations.clear();

        for (i=grid->getNumberSets()-1;i>=0;i--) {
          grid->removeSet(0);
        }

        for(std::vector<shared_ptr<XdmfSet> >::const_iterator iter =
            mSets.begin(); 
            iter != mSets.end();
            ++iter) {
          grid->insert(*iter);
        }

        mSets.clear();

        for (i=grid->getNumberMaps()-1;i>=0;i--) {
          grid->removeMap(0);
        }

        for(std::vector<shared_ptr<XdmfMap> >::const_iterator iter =
            mMaps.begin(); 
            iter != mMaps.end();
            ++iter) {
          grid->insert(*iter);
        }

        mMaps.clear();
    
        if(mTime) {
          grid->setTime(mTime);
        }
      }
      else {
        try {
          XdmfError::message(XdmfError::FATAL,
                             "Error: Index out of range.");
        }
        catch (XdmfError e) {
          throw e;
        }
      }
    }
    else if (gridType == XDMF_GRID_TYPE_UNSTRUCTURED) {
      if (index < (int)mGridCollections.top()->getNumberUnstructuredGrids()) {
        if(mGeometry == NULL) {
          try {
            XdmfError::message(XdmfError::FATAL, 
                               "Must set geometry before adding grid.");
          }
          catch (XdmfError e) {
            throw e;
          }
        }
        if(mTopology == NULL) {
          try {
            XdmfError::message(XdmfError::FATAL, 
                               "Must set topology before adding grid.");
          }
          catch (XdmfError e) {
            throw e;
          }
        }
        shared_ptr<XdmfUnstructuredGrid> grid =
          mGridCollections.top()->getUnstructuredGrid(index);
        grid->setName(name);
        grid->setGeometry(mGeometry);
        grid->setTopology(mTopology);

        for (i=grid->getNumberAttributes()-1;i>=0;i--) {
          grid->removeAttribute(0);
        }

        for(std::vector<shared_ptr<XdmfAttribute> >::const_iterator iter =
            mAttributes.begin(); 
            iter != mAttributes.end();
            ++iter) {
          grid->insert(*iter);
        }

        mAttributes.clear();

        for (i=grid->getNumberInformations()-1;i>=0;i--) {
          grid->removeInformation(0);
        }

        for(std::vector<shared_ptr<XdmfInformation> >::const_iterator iter =
            mInformations.begin(); 
            iter != mInformations.end();
            ++iter) {
          grid->insert(*iter);
        }

        mInformations.clear();

        for (i=grid->getNumberSets()-1;i>=0;i--) {
          grid->removeSet(0);
        }

        for(std::vector<shared_ptr<XdmfSet> >::const_iterator iter =
            mSets.begin(); 
            iter != mSets.end();
            ++iter) {
          grid->insert(*iter);
        }

        mSets.clear();

        for (i=grid->getNumberMaps()-1;i>=0;i--) {
          grid->removeMap(0);
        }

        for(std::vector<shared_ptr<XdmfMap> >::const_iterator iter =
            mMaps.begin(); 
            iter != mMaps.end();
            ++iter) {
          grid->insert(*iter);
        }

        mMaps.clear();
    
        if(mTime) {
          grid->setTime(mTime);
        }
      }
      else {
        try {
          XdmfError::message(XdmfError::FATAL,
                             "Error: Index out of range.");
        }
        catch (XdmfError e) {
          throw e;
        }
      }
    }
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "There is no grid collection currently loaded.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

void
XdmfFortran::retrieveGridCollectionGridName(const int gridType,
                                            const int index,
                                            char * returnName,
                                            const int nameLength)
{
  if (!mGridCollections.empty()) {
    shared_ptr<XdmfGrid> openedGrid;
    if (gridType == XDMF_GRID_TYPE_CURVILINEAR) {
      if (index < (int)mGridCollections.top()->getNumberCurvilinearGrids()) {
        openedGrid = mGridCollections.top()->getCurvilinearGrid(index);
      }
      else {
        try {
          XdmfError::message(XdmfError::FATAL,
                             "Error: Index out of range.");
        }
        catch (XdmfError e) {
          throw e;
        }
      }
    }
    else if (gridType == XDMF_GRID_TYPE_RECTILINEAR) {
      if (index < (int)mGridCollections.top()->getNumberRectilinearGrids()) {
        openedGrid = mGridCollections.top()->getRectilinearGrid(index);
      }
      else {
        try {
          XdmfError::message(XdmfError::FATAL,
                             "Error: Index out of range.");
        }
        catch (XdmfError e) {
          throw e;
        }
      }
    }
    else if (gridType == XDMF_GRID_TYPE_REGULAR) {
      if (index < (int)mGridCollections.top()->getNumberRegularGrids()) {
        openedGrid = mGridCollections.top()->getRegularGrid(index);
      }
      else {
        try {
          XdmfError::message(XdmfError::FATAL,
                             "Error: Index out of range.");
        }
        catch (XdmfError e) {
          throw e;
        }
      }
    }
    else if (gridType == XDMF_GRID_TYPE_UNSTRUCTURED) {
      if (index < (int)mGridCollections.top()->getNumberUnstructuredGrids()) {
        openedGrid = mGridCollections.top()->getUnstructuredGrid(index);
      }
      else {
        try {
          XdmfError::message(XdmfError::FATAL,
                             "Error: Index out of range.");
        }
        catch (XdmfError e) {
          throw e;
        }
      }
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Invalid Grid Type.");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
    char * tempName = strdup(openedGrid->getName().c_str());
    memset(returnName, 0, nameLength);
    memcpy(returnName, tempName, strlen(tempName)+1);
    delete [] tempName;
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "There is no grid collection currently loaded.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

void
XdmfFortran::retrieveGridCollectionGridTag(const int gridType,
                                           const int index,
                                           char * returnTag,
                                           const int tagLength)
{
  if (!mGridCollections.empty()) {
    shared_ptr<XdmfGrid> openedGrid;
    if (gridType == XDMF_GRID_TYPE_CURVILINEAR) {
      if (index < (int)mGridCollections.top()->getNumberCurvilinearGrids()) {
        openedGrid = mGridCollections.top()->getCurvilinearGrid(index);
      }
      else {
        try {
          XdmfError::message(XdmfError::FATAL,
                             "Error: Index out of range.");
        }
        catch (XdmfError e) {
          throw e;
        }
      }
    }
    else if (gridType == XDMF_GRID_TYPE_RECTILINEAR) {
      if (index < (int)mGridCollections.top()->getNumberRectilinearGrids()) {
        openedGrid = mGridCollections.top()->getRectilinearGrid(index);
      }
      else {
        try {
          XdmfError::message(XdmfError::FATAL,
                             "Error: Index out of range.");
        }
        catch (XdmfError e) {
          throw e;
        }
      }
    }
    else if (gridType == XDMF_GRID_TYPE_REGULAR) {
      if (index < (int)mGridCollections.top()->getNumberRegularGrids()) {
        openedGrid = mGridCollections.top()->getRegularGrid(index);
      }
      else {
        try {
          XdmfError::message(XdmfError::FATAL,
                             "Error: Index out of range.");
        }
        catch (XdmfError e) {
          throw e;
        }
      }
    }
    else if (gridType == XDMF_GRID_TYPE_UNSTRUCTURED) {
      if (index < (int)mGridCollections.top()->getNumberUnstructuredGrids()) {
        openedGrid = mGridCollections.top()->getUnstructuredGrid(index);
      }
      else {
        try {
          XdmfError::message(XdmfError::FATAL,
                             "Error: Index out of range.");
        }
        catch (XdmfError e) {
          throw e;
        }
      }
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Invalid Grid Type.");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
    char * tempTag = strdup(openedGrid->getItemTag().c_str());
    memset(returnTag, 0, tagLength);
    memcpy(returnTag, tempTag, strlen(tempTag)+1);
    delete [] tempTag;
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "There is no grid collection currently loaded.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

int
XdmfFortran::retrieveGridCollectionGridNumProperties(const int gridType, const int index)
{
  if (!mGridCollections.empty()) {
    if (gridType == XDMF_GRID_TYPE_CURVILINEAR) {
      if (index < (int)mGridCollections.top()->getNumberCurvilinearGrids()) {
        return mGridCollections.top()->getCurvilinearGrid(index)->getItemProperties().size();
      }
      else {
        try {
          XdmfError::message(XdmfError::FATAL,
                             "Error: Index out of range.");
        }
        catch (XdmfError e) {
          throw e;
        }
        return -1;
      }
    }
    else if (gridType == XDMF_GRID_TYPE_RECTILINEAR) {
      if (index < (int)mGridCollections.top()->getNumberRectilinearGrids()) {
        return mGridCollections.top()->getRectilinearGrid(index)->getItemProperties().size();
      }
      else {
        try {
          XdmfError::message(XdmfError::FATAL,
                             "Error: Index out of range.");
        }
        catch (XdmfError e) {
          throw e;
        }
        return -1;
      }
    }
    else if (gridType == XDMF_GRID_TYPE_REGULAR) {
      if (index < (int)mGridCollections.top()->getNumberRegularGrids()) {
        return mGridCollections.top()->getRegularGrid(index)->getItemProperties().size();
      }
      else {
        try {
          XdmfError::message(XdmfError::FATAL,
                             "Error: Index out of range.");
        }
        catch (XdmfError e) {
          throw e;
        }
        return -1;
      }
    }
    else if (gridType == XDMF_GRID_TYPE_UNSTRUCTURED) {
      if (index < (int)mGridCollections.top()->getNumberUnstructuredGrids()) {
        return mGridCollections.top()->getUnstructuredGrid(index)->getItemProperties().size();
      }
      else {
        try {
          XdmfError::message(XdmfError::FATAL,
                             "Error: Index out of range.");
        }
        catch (XdmfError e) {
          throw e;
        }
        return -1;
      }
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Invalid Grid Type.");
      }
      catch (XdmfError e) {
        throw e;
      }
      return -1;
    }
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "There is no grid collection currently loaded.");
    }
    catch (XdmfError e) {
      throw e;
    }
    return -1;
  }
}

void
XdmfFortran::retrieveGridCollectionGridProperty(const int gridType,
                                                const int gridIndex,
                                                const int index,
                                                char * key,
                                                const int keyLength,
                                                char * value,
                                                const int valueLength)
{
  if (!mGridCollections.empty()) {
    shared_ptr<XdmfGrid> openedGrid;
    if (gridType == XDMF_GRID_TYPE_CURVILINEAR) {
      if (index < (int)mGridCollections.top()->getNumberCurvilinearGrids()) {
        openedGrid = mGridCollections.top()->getCurvilinearGrid(index);
      }
      else {
        try {
          XdmfError::message(XdmfError::FATAL,
                             "Error: Index out of range.");
        }
        catch (XdmfError e) {
          throw e;
        }
      }
    }
    else if (gridType == XDMF_GRID_TYPE_RECTILINEAR) {
      if (index < (int)mGridCollections.top()->getNumberRectilinearGrids()) {
        openedGrid = mGridCollections.top()->getRectilinearGrid(index);
      }
      else {
        try {
          XdmfError::message(XdmfError::FATAL,
                             "Error: Index out of range.");
        }
        catch (XdmfError e) {
          throw e;
        }
      }
    }
    else if (gridType == XDMF_GRID_TYPE_REGULAR) {
      if (index < (int)mGridCollections.top()->getNumberRegularGrids()) {
        openedGrid = mGridCollections.top()->getRegularGrid(index);
      }
      else {
        try {
          XdmfError::message(XdmfError::FATAL,
                             "Error: Index out of range.");
        }
        catch (XdmfError e) {
          throw e;
        }
      }
    }
    else if (gridType == XDMF_GRID_TYPE_UNSTRUCTURED) {
      if (index < (int)mGridCollections.top()->getNumberUnstructuredGrids()) {
        openedGrid = mGridCollections.top()->getUnstructuredGrid(index);
      }
      else {
        try {
          XdmfError::message(XdmfError::FATAL,
                             "Error: Index out of range.");
        }
        catch (XdmfError e) {
          throw e;
        }
      }
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Invalid Grid Type.");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
    if (index < (int)openedGrid->getItemProperties().size()) {
      std::map<std::string, std::string>::iterator walker =
        openedGrid->getItemProperties().begin();
      for (int i = 0; i<index; i++) {
        walker++;
      }
      char * tempKey = strdup((*walker).first.c_str());
      memset(key, 0, keyLength);
      memcpy(key, tempKey, strlen(tempKey)+1);
      delete [] tempKey;
      char * tempValue = strdup((*walker).second.c_str());
      memset(value, 0, valueLength);
      memcpy(value, tempValue, strlen(tempValue)+1);
      delete [] tempValue;
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Index out of range.");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "There is no grid collection currently loaded.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

void
XdmfFortran::retrieveGridCollectionGridPropertyByKey(const int gridType,
                                                     const int index,
                                                     char * key,
                                                     char * value,
                                                     const int valueLength)
{
  if (!mGridCollections.empty()) {
    shared_ptr<XdmfGrid> openedGrid;
    if (gridType == XDMF_GRID_TYPE_CURVILINEAR) {
      if (index < (int)mGridCollections.top()->getNumberCurvilinearGrids()) {
        openedGrid = mGridCollections.top()->getCurvilinearGrid(index);
      }
      else {
        try {
          XdmfError::message(XdmfError::FATAL,
                             "Error: Index out of range.");
        }
        catch (XdmfError e) {
          throw e;
        }
      }
    }
    else if (gridType == XDMF_GRID_TYPE_RECTILINEAR) {
      if (index < (int)mGridCollections.top()->getNumberRectilinearGrids()) {
        openedGrid = mGridCollections.top()->getRectilinearGrid(index);
      }
      else {
        try {
          XdmfError::message(XdmfError::FATAL,
                             "Error: Index out of range.");
        }
        catch (XdmfError e) {
          throw e;
        }
      }
    }
    else if (gridType == XDMF_GRID_TYPE_REGULAR) {
      if (index < (int)mGridCollections.top()->getNumberRegularGrids()) {
        openedGrid = mGridCollections.top()->getRegularGrid(index);
      }
      else {
        try {
          XdmfError::message(XdmfError::FATAL,
                             "Error: Index out of range.");
        }
        catch (XdmfError e) {
          throw e;
        }
      }
    }
    else if (gridType == XDMF_GRID_TYPE_UNSTRUCTURED) {
      if (index < (int)mGridCollections.top()->getNumberUnstructuredGrids()) {
        openedGrid = mGridCollections.top()->getUnstructuredGrid(index);
      }
      else {
        try {
          XdmfError::message(XdmfError::FATAL,
                             "Error: Index out of range.");
        }
        catch (XdmfError e) {
          throw e;
        }
      }
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Invalid Grid Type.");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
    std::string tempString = key;
    memset(value, 0, valueLength);
    if ((openedGrid->getItemProperties().count(tempString))>0) {
      char * tempValue = strdup(openedGrid->getItemProperties()[tempString].c_str());
      memcpy(value, tempValue, strlen(tempValue)+1);
      delete [] tempValue;
    }
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "There is no grid collection currently loaded.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

int
XdmfFortran::numDomainGrids(const int gridType)
{
  if (gridType == XDMF_GRID_TYPE_CURVILINEAR) {
    return mDomain->getNumberCurvilinearGrids();
  }
  else if (gridType == XDMF_GRID_TYPE_RECTILINEAR) {
    return mDomain->getNumberRectilinearGrids();
  }
  else if (gridType == XDMF_GRID_TYPE_REGULAR) {
    return mDomain->getNumberRegularGrids();
  }
  else if (gridType == XDMF_GRID_TYPE_UNSTRUCTURED) {
    return mDomain->getNumberUnstructuredGrids();
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Invalid Grid Type.");
    }
    catch (XdmfError e) {
      throw e;
    }
    return -1;
  }
}

int
XdmfFortran::numGridCollectionGrids(const int gridType)
{
  if (!mGridCollections.empty()) {
    if (gridType == XDMF_GRID_TYPE_CURVILINEAR) {
      return mGridCollections.top()->getNumberCurvilinearGrids();
    }
    else if (gridType == XDMF_GRID_TYPE_RECTILINEAR) {
      return mGridCollections.top()->getNumberRectilinearGrids();
    }
    else if (gridType == XDMF_GRID_TYPE_REGULAR) {
      return mGridCollections.top()->getNumberRegularGrids();
    }
    else if (gridType == XDMF_GRID_TYPE_UNSTRUCTURED) {
      return mGridCollections.top()->getNumberUnstructuredGrids();
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Invalid Grid Type."); 
      }
      catch (XdmfError e) {
        throw e;
      }
      return -1;
    }
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "Error: No Grid Collections have been loaded.");
    }
    catch (XdmfError e) {
      throw e;
    }
    return -1;
  }
}
int
XdmfFortran::retrieveGridCollectionType()
{
  if (!mGridCollections.empty()) {
    shared_ptr<const XdmfGridCollectionType> returnType = mGridCollections.top()->getType();
    if (returnType == XdmfGridCollectionType::Spatial()) {
      return XDMF_GRID_COLLECTION_TYPE_SPATIAL;
    }
    else if (returnType == XdmfGridCollectionType::Temporal()) {
      return XDMF_GRID_COLLECTION_TYPE_TEMPORAL;
    }
    else if (returnType == XdmfGridCollectionType::NoCollectionType()) {
      try {
        XdmfError::message(XdmfError::FATAL, 
                           "Error: No Grid Collection Type.");
      }
      catch (XdmfError e) {
        throw e;
      }
      return -1;
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL, 
                           "Invalid Grid Collection Type.");
      }
      catch (XdmfError e) {
        throw e;
      }
      return -1;
    }
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "There is no grid collection currently loaded.");
    }
    catch (XdmfError e) {
      throw e;
    }
    return -1;
  }
}

float
XdmfFortran::retrieveTime()
{
  return static_cast<float>(mTime->getValue());
}

void
XdmfFortran::retrieveGeometryTag(char * tag, const int tagLength)
{
  if (mGeometry == NULL) {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "Must set geometry before its tag can be retrieved.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
  else {
    char * tempTag = strdup(mGeometry->getItemTag().c_str());
    memset(tag, 0, tagLength);
    memcpy(tag, tempTag, strlen(tempTag)+1);
    delete [] tempTag;
  }
}

int
XdmfFortran::retrieveGeometryType()
{
  if (mGeometry == NULL) {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "Must set geometry before its type can be retrieved.");
    }
    catch (XdmfError e) {
      throw e;
    }
    return -1;
  }
  else {
    shared_ptr<const XdmfGeometryType> returnType = mGeometry->getType();
    if (returnType == XdmfGeometryType::XY()) {
      return XDMF_GEOMETRY_TYPE_XY;
    }
    else if (returnType == XdmfGeometryType::XYZ()) {
      return XDMF_GEOMETRY_TYPE_XYZ;
    }
    else if (returnType == XdmfGeometryType::NoGeometryType()) {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Uninitialized geometry type");
      }
      catch (XdmfError e) {
        throw e;
      }
      return -1;
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Invalid geometry type");
      }
      catch (XdmfError e) {
        throw e;
      }
      return -1;
    }
  }
}

int
XdmfFortran::retrieveGeometryValueType()
{
  if (mGeometry == NULL) {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "Must set geometry before its value type can be retrieved.");
    }
    catch (XdmfError e) {
      throw e;
    }
    return -1;
  }
  else {
    shared_ptr<const XdmfArrayType> dataType = mGeometry->getArrayType();
    if (dataType == XdmfArrayType::Int8()) {
      return XDMF_ARRAY_TYPE_INT8;
    }
    else if (dataType == XdmfArrayType::Int16()) {
      return XDMF_ARRAY_TYPE_INT16;
    }
    else if (dataType == XdmfArrayType::Int32()) {
      return XDMF_ARRAY_TYPE_INT32;
    }
    else if (dataType == XdmfArrayType::Int64()) {
      return XDMF_ARRAY_TYPE_INT64;
    }
    else if (dataType == XdmfArrayType::UInt8()) {
      return XDMF_ARRAY_TYPE_UINT8;
    }
    else if (dataType == XdmfArrayType::UInt16()) {
      return XDMF_ARRAY_TYPE_UINT16;
    }
    else if (dataType == XdmfArrayType::UInt32()) {
      return XDMF_ARRAY_TYPE_UINT32;
    }
    else if (dataType == XdmfArrayType::Float32()) {
      return XDMF_ARRAY_TYPE_FLOAT32;
    }
    else if (dataType == XdmfArrayType::Float64()) {
      return XDMF_ARRAY_TYPE_FLOAT64;
    }
    else if (dataType == XdmfArrayType::Uninitialized()) {
      try {
        XdmfError::message(XdmfError::FATAL, 
                           "Uninitialized Geometry Data Type.");
      }
      catch (XdmfError e) {
        throw e;
      }
      return -1;
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL, 
                           "Invalid Geometry Data Type.");
      }
      catch (XdmfError e) {
        throw e;
      }
      return -1;
    }
  }
}

void
XdmfFortran::retrieveGeometryValues(void * values,
                                    const int dataType,
                                    const int numberRead,
                                    const int startIndex,
                                    const int arrayStride,
                                    const int valueStride)
{
  if(mGeometry == NULL) {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "Must set geometry before its values can be retrieved.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
  else {
    if (!mGeometry->isInitialized()) {
      mGeometry->read();
    }
    readFromArray(mGeometry,
                  dataType,
                  values,
                  numberRead,
                  startIndex,
                  arrayStride,
                  valueStride);
  }
}

int
XdmfFortran::retrieveGeometryNumPoints()
{
  if (mGeometry == NULL) {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "Must set geometry before the number of points"
                         " it contains can be retrieved.");
    }
    catch (XdmfError e) {
      throw e;
    }
    return -1;
  }
  else {
    return mGeometry->getNumberPoints();
  }
}

int
XdmfFortran::retrieveGeometrySize()
{
  if (mGeometry == NULL) {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "Must set geometry before its size can be retrieved.");
    }
    catch (XdmfError e) {
      throw e;
    }
    return -1;
  }
  else {
    return mGeometry->getSize();
  }
}

void
XdmfFortran::clearPreviousGeometries()
{
  mPreviousGeometries.clear();
}

void
XdmfFortran::modifyGeometryValues(void * values,
                                  const int arrayType,
                                  const int numValues,
                                  const int startIndex,
                                  const int arrayStride,
                                  const int valueStride)
{
  if(mGeometry == NULL) {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Must set geometry before its values can be modified.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
  else {
    if (!mGeometry->isInitialized()) {
      mGeometry->read();
    }
    writeToArray(mGeometry,
                 numValues,
                 arrayType,
                 values,
                 startIndex,
                 arrayStride,
                 valueStride);
  }
}

int
XdmfFortran::retrieveGeometryNumProperties()
{
  if (mGeometry == NULL) {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "Must set geometry before the number of properties"
                         " it contains can be retrieved.");
    }
    catch (XdmfError e) {
      throw e;
    }
    return -1;
  }
  else {
    return mGeometry->getItemProperties().size();
  } 
}

void
XdmfFortran::retrieveGeometryProperty(const int index,
                                      char * key,
                                      const int keyLength,
                                      char * value,
                                      const int valueLength)
{
  if (mGeometry == NULL) {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "Must set geometry before the properties"
                         " it contains can be retrieved.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
  else {
    if (index < (int)mGeometry->getItemProperties().size()) {
      std::map<std::string, std::string>::iterator walker =
        mGeometry->getItemProperties().begin();
      for (int i = 0; i<index; i++) {
        walker++;
      }
      char * tempKey = strdup((*walker).first.c_str());
      memset(key, 0, keyLength);
      memcpy(key, tempKey, strlen(tempKey)+1);
      delete [] tempKey;
      char * tempValue = strdup((*walker).second.c_str());
      memset(value, 0, valueLength);
      memcpy(value, tempValue, strlen(tempValue)+1);
      delete [] tempValue;
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Index out of range.");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
  }
}

void
XdmfFortran::retrieveGeometryPropertyByKey(char * key,
                                           char * value,
                                           const int valueLength)
{
  if (mGeometry == NULL) {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "Must set geometry before the properties"
                         " it contains can be retrieved.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
  else {
    std::string tempString = key;
    memset(value, 0, valueLength);
    if ((mGeometry->getItemProperties().count(tempString))>0) {
      char * tempValue = strdup(mGeometry->getItemProperties()[tempString].c_str());
      memcpy(value, tempValue, strlen(tempValue)+1);
      delete [] tempValue;
    }
  }
}

void
XdmfFortran::retrieveTopologyTag(char * tag, const int tagLength)
{
  if (mTopology == NULL) {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "Must set Topology before its tag can be retrieved.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
  else {
    char * tempTag = strdup(mTopology->getItemTag().c_str());
    memset(tag, 0, tagLength);
    memcpy(tag, tempTag, strlen(tempTag)+1);
    delete [] tempTag;
  }
}

int
XdmfFortran::retrieveTopologyType()
{
  if (mTopology == NULL) {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "Must set Topology before its type can be retrieved.");
    }
    catch (XdmfError e) {
      throw e;
    }
    return -1;
  }
  else {
    shared_ptr<const XdmfTopologyType> returnType =  mTopology->getType();
    if (returnType ==  XdmfTopologyType::Polyvertex()) {
      return XDMF_TOPOLOGY_TYPE_POLYVERTEX;
    }
    else if (returnType == XdmfTopologyType::Polyline(0)) {
      return XDMF_TOPOLOGY_TYPE_POLYLINE;
    }
    else if (returnType == XdmfTopologyType::Polygon(0)) {
      return XDMF_TOPOLOGY_TYPE_POLYGON;
    }
    else if (returnType == XdmfTopologyType::Triangle()) {
      return XDMF_TOPOLOGY_TYPE_TRIANGLE;
    }
    else if (returnType == XdmfTopologyType::Quadrilateral()) {
      return XDMF_TOPOLOGY_TYPE_QUADRILATERAL;
    }
    else if (returnType == XdmfTopologyType::Tetrahedron()) {
      return XDMF_TOPOLOGY_TYPE_TETRAHEDRON;
    }
    else if (returnType == XdmfTopologyType::Pyramid()) {
      return XDMF_TOPOLOGY_TYPE_PYRAMID;
    }
    else if (returnType == XdmfTopologyType::Wedge()) {
      return XDMF_TOPOLOGY_TYPE_WEDGE;
    }
    else if (returnType == XdmfTopologyType::Hexahedron()) {
      return XDMF_TOPOLOGY_TYPE_HEXAHEDRON;
    }
    else if (returnType == XdmfTopologyType::Edge_3()) {
      return XDMF_TOPOLOGY_TYPE_EDGE_3;
    }
    else if (returnType == XdmfTopologyType::Triangle_6()) {
      return XDMF_TOPOLOGY_TYPE_TRIANGLE_6;
    }
    else if (returnType == XdmfTopologyType::Quadrilateral_8()) {
      return XDMF_TOPOLOGY_TYPE_QUADRILATERAL_8;
    }
    else if (returnType == XdmfTopologyType::Quadrilateral_9()) {
      return XDMF_TOPOLOGY_TYPE_QUADRILATERAL_9;
    }
    else if (returnType == XdmfTopologyType::Tetrahedron_10()) {
      return XDMF_TOPOLOGY_TYPE_TETRAHEDRON_10;
    }
    else if (returnType == XdmfTopologyType::Pyramid_13()) {
      return XDMF_TOPOLOGY_TYPE_PYRAMID_13;
    }
    else if (returnType == XdmfTopologyType::Wedge_15()) {
      return XDMF_TOPOLOGY_TYPE_WEDGE_15;
    }
    else if (returnType == XdmfTopologyType::Wedge_18()) {
      return XDMF_TOPOLOGY_TYPE_WEDGE_18;
    }
    else if (returnType == XdmfTopologyType::Hexahedron_20()) {
      return XDMF_TOPOLOGY_TYPE_HEXAHEDRON_20;
    }
    else if (returnType == XdmfTopologyType::Hexahedron_24()) {
      return XDMF_TOPOLOGY_TYPE_HEXAHEDRON_24;
    }
    else if (returnType == XdmfTopologyType::Hexahedron_27()) {
      return XDMF_TOPOLOGY_TYPE_HEXAHEDRON_27;
    }
    else if (returnType == XdmfTopologyType::Hexahedron_64()) {
      return XDMF_TOPOLOGY_TYPE_HEXAHEDRON_64;
    }
    else if (returnType == XdmfTopologyType::Hexahedron_125()) {
      return XDMF_TOPOLOGY_TYPE_HEXAHEDRON_125;
    }
    else if (returnType == XdmfTopologyType::Hexahedron_216()) {
      return XDMF_TOPOLOGY_TYPE_HEXAHEDRON_216;
    }
    else if (returnType == XdmfTopologyType::Hexahedron_343()) {
      return XDMF_TOPOLOGY_TYPE_HEXAHEDRON_343;
    }
    else if (returnType == XdmfTopologyType::Hexahedron_512()) {
      return XDMF_TOPOLOGY_TYPE_HEXAHEDRON_512;
    }
    else if (returnType == XdmfTopologyType::Hexahedron_729()) {
      return XDMF_TOPOLOGY_TYPE_HEXAHEDRON_729;
    }
    else if (returnType == XdmfTopologyType::Hexahedron_1000()) {
      return XDMF_TOPOLOGY_TYPE_HEXAHEDRON_1000;
    }
    else if (returnType == XdmfTopologyType::Hexahedron_1331()) {
      return XDMF_TOPOLOGY_TYPE_HEXAHEDRON_1331;
    }
    else if (returnType == XdmfTopologyType::Mixed()) {
      return XDMF_TOPOLOGY_TYPE_MIXED;
    }
    else if (returnType == XdmfTopologyType::NoTopologyType()) {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Uninitialized topology type.");
      }
      catch (XdmfError e) {
        throw e;
      }
      return -1;
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Invalid topology type.");
      }
      catch (XdmfError e) {
        throw e;
      }
      return -1;
    }
  }
}

int
XdmfFortran::retrieveTopologyValueType()
{
  if (mTopology == NULL) {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "Must set Topology before its value type can be retrieved.");
    }
    catch (XdmfError e) {
      throw e;
    }
    return -1;
  }
  else {   
    shared_ptr<const XdmfArrayType> dataType = mTopology->getArrayType();
    if (dataType == XdmfArrayType::Int8()) {
      return XDMF_ARRAY_TYPE_INT8;
    }
    else if (dataType == XdmfArrayType::Int16()) {
      return XDMF_ARRAY_TYPE_INT16;
    }
    else if (dataType == XdmfArrayType::Int32()) {
      return XDMF_ARRAY_TYPE_INT32;
    }
    else if (dataType == XdmfArrayType::Int64()) {
      return XDMF_ARRAY_TYPE_INT64;
    }
    else if (dataType == XdmfArrayType::UInt8()) {
      return XDMF_ARRAY_TYPE_UINT8;
    }
    else if (dataType == XdmfArrayType::UInt16()) {
      return XDMF_ARRAY_TYPE_UINT16;
    }
    else if (dataType == XdmfArrayType::UInt32()) {
      return XDMF_ARRAY_TYPE_UINT32;
    }
    else if (dataType == XdmfArrayType::Float32()) {
      return XDMF_ARRAY_TYPE_FLOAT32;
    }
    else if (dataType == XdmfArrayType::Float64()) {
      return XDMF_ARRAY_TYPE_FLOAT64;
    }
    else if (dataType == XdmfArrayType::Uninitialized()) {
      try {
        XdmfError::message(XdmfError::FATAL, 
                           "Uninitialized Topology Data Type.");
      }
      catch (XdmfError e) {
        throw e;
      }
      return -1;
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL, 
                           "Invalid Topology Data Type.");
      }
      catch (XdmfError e) {
        throw e;
      }
      return -1;
    }
  }
}

void
XdmfFortran::retrieveTopologyValues(void * values,
                                    const int dataType,
                                    const int numberRead,
                                    const int startIndex,
                                    const int arrayStride,
                                    const int valueStride)
{
  if(mTopology == NULL) {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "Must set topology before its values can be retrieved.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
  else {
    if (!mTopology->isInitialized()) {
      mTopology->read();
    }
    readFromArray(mTopology,
                  dataType,
                  values,
                  numberRead,
                  startIndex,
                  arrayStride,
                  valueStride);
  }
}

int
XdmfFortran::retrieveTopologyNumElements()
{
  if (mTopology == NULL) {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "Must set Topology before the number of Elements"
                         " it contains can be retrieved.");
    }
    catch (XdmfError e) {
      throw e;
    }
    return -1;
  }
  else {
    return mTopology->getNumberElements();
  }
}

int
XdmfFortran::retrieveTopologySize()
{
  if (mTopology == NULL) {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "Must set Topology before its size can be retrieved.");
    }
    catch (XdmfError e) {
      throw e;
    }
    return -1;
  }
  else {
    return mTopology->getSize();
  }
}

void
XdmfFortran::clearPreviousTopologies()
{
  mPreviousTopologies.clear();
}

void
XdmfFortran::modifyTopologyValues(void * values,
                                  const int arrayType,
                                  const int numValues,
                                  const int startIndex,
                                  const int arrayStride,
                                  const int valueStride)
{
  if(mTopology == NULL) {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Must set topology before its values can be modified.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
  else {
    if (!mTopology->isInitialized()) {
      mTopology->read();
    }
    writeToArray(mTopology,
                 numValues,
                 arrayType,
                 values,
                 startIndex,
                 arrayStride,
                 valueStride);
  }
}

int
XdmfFortran::retrieveTopologyNumProperties()
{
  if (mTopology == NULL) {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "Must set Topology before the number of properties"
                         " it contains can be retrieved.");
    }
    catch (XdmfError e) {
      throw e;
    }
    return -1;
  }
  else {
    return mTopology->getItemProperties().size();
  } 
}

void
XdmfFortran::retrieveTopologyProperty(const int index,
                                      char * key,
                                      const int keyLength,
                                      char * value,
                                      const int valueLength)
{
  if (mTopology == NULL) {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "Must set Topology before the properties it contains"
                         " can be retrieved.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
  else {
    if (index < (int)mTopology->getItemProperties().size()) {
      std::map<std::string, std::string>::iterator walker = mTopology->getItemProperties().begin();
      for (int i = 0; i<index; i++) {
        walker++;
      }
      char * tempKey = strdup((*walker).first.c_str());
      memset(key, 0, keyLength);
      memcpy(key, tempKey, strlen(tempKey)+1);
      delete [] tempKey;
      char * tempValue = strdup((*walker).second.c_str());
      memset(value, 0, valueLength);
      memcpy(value, tempValue, strlen(tempValue)+1);
      delete [] tempValue;
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Index out of range.");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
  }
}

void
XdmfFortran::retrieveTopologyPropertyByKey(char * key, char * value, const int valueLength)
{
  if (mTopology == NULL) {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "Must set Topology before the properties"
                         " it contains can be retrieved.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
  else {
    std::string tempString = key;
    memset(value, 0, valueLength);
    if ((mTopology->getItemProperties().count(tempString))>0) {
      char * tempValue = strdup(mTopology->getItemProperties()[tempString].c_str());
      memcpy(value, tempValue, strlen(tempValue)+1);
      delete [] tempValue;
    }
  }
}

int
XdmfFortran::setDimensions(const int numValues, const int arrayType, void * pointValues)
{
  mDimensions = XdmfArray::New();

  // insert dimension values into array
  writeToArray(mDimensions, numValues, arrayType, pointValues);

  int id = mPreviousDimensions.size();
  mPreviousDimensions.push_back(mDimensions);
  return id;
}

void
XdmfFortran::openPreviousDimensions(const int index)
{
  if ((int)mPreviousDimensions.size()>index) {
    mDimensions = mPreviousDimensions[index];
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

void
XdmfFortran::clearPreviousDimensions()
{
  mPreviousDimensions.clear();
}

void
XdmfFortran::modifyDimensionsValues(void * values,
                                    const int arrayType,
                                    const int numValues,
                                    const int startIndex,
                                    const int arrayStride,
                                    const int valueStride)
{
  if(mDimensions == NULL) {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Must set dimensions before its values"
                         " can be modified.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
  else {
    if (!mDimensions->isInitialized()) {
      mDimensions->read();
    }
    writeToArray(mDimensions,
                 numValues,
                 arrayType,
                 values,
                 startIndex,
                 arrayStride,
                 valueStride);
  }
}

void
XdmfFortran::retrieveDimensionsTag(char * tag, const int tagLength)
{
  if (mDimensions == NULL) {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "Must set dimensions before its tag can be retrieved.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
  else {
    char * tempTag = strdup(mDimensions->getItemTag().c_str());
    memset(tag, 0, tagLength);
    memcpy(tag, tempTag, strlen(tempTag)+1);
    delete [] tempTag;
  }
}

int
XdmfFortran::retrieveDimensionsValueType()
{
  if (mDimensions == NULL) {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "Must set dimensions before its value type"
                         " can be retrieved.");
    }
    catch (XdmfError e) {
      throw e;
    }
    return -1;
  }
  else {
    shared_ptr<const XdmfArrayType> dataType = mDimensions->getArrayType();
    if (dataType == XdmfArrayType::Int8()) {
      return XDMF_ARRAY_TYPE_INT8;
    }
    else if (dataType == XdmfArrayType::Int16()) {
      return XDMF_ARRAY_TYPE_INT16;
    }
    else if (dataType == XdmfArrayType::Int32()) {
      return XDMF_ARRAY_TYPE_INT32;
    }
    else if (dataType == XdmfArrayType::Int64()) {
      return XDMF_ARRAY_TYPE_INT64;
    }
    else if (dataType == XdmfArrayType::UInt8()) {
      return XDMF_ARRAY_TYPE_UINT8;
    }
    else if (dataType == XdmfArrayType::UInt16()) {
      return XDMF_ARRAY_TYPE_UINT16;
    }
    else if (dataType == XdmfArrayType::UInt32()) {
      return XDMF_ARRAY_TYPE_UINT32;
    }
    else if (dataType == XdmfArrayType::Float32()) {
      return XDMF_ARRAY_TYPE_FLOAT32;
    }
    else if (dataType == XdmfArrayType::Float64()) {
      return XDMF_ARRAY_TYPE_FLOAT64;
    }
    else if (dataType == XdmfArrayType::Uninitialized()) {
      try {
        XdmfError::message(XdmfError::FATAL, 
                           "Uninitialized Dimension Data Type.");
      }
      catch (XdmfError e) {
        throw e;
      }
      return -1;
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL, 
                           "Invalid Dimension Data Type.");
      }
      catch (XdmfError e) {
        throw e;
      }
      return -1;
    }
  }
}

void
XdmfFortran::retrieveDimensionsValues(void * values,
                                      const int dataType,
                                      const int numberRead,
                                      const int startIndex,
                                      const int arrayStride,
                                      const int valueStride)
{
  if(mDimensions == NULL) {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "Must set dimensions before its values"
                         " can be retrieved.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
  else {
    if (!mDimensions->isInitialized()) {
      mDimensions->read();
    }
    readFromArray(mDimensions,
                  dataType,
                  values,
                  numberRead,
                  startIndex,
                  arrayStride,
                  valueStride);
  }
}

int
XdmfFortran::retrieveDimensionsSize()
{
  if (mDimensions == NULL) {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "Must set Dimensions before its size can be retrieved.");
    }
    catch (XdmfError e) {
      throw e;
    }
    return -1;
  }
  else {
    return mDimensions->getSize();
  }
}

int
XdmfFortran::retrieveDimensionsNumProperties()
{
  if (mDimensions == NULL) {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "Must set dimensions before the number of properties"
                         " it contains can be retrieved.");
    }
    catch (XdmfError e) {
      throw e;
    }
    return -1;
  }
  else {
    return mDimensions->getItemProperties().size();
  } 
}

void
XdmfFortran::retrieveDimensionsProperty(const int index,
                                        char * key,
                                        const int keyLength,
                                        char * value,
                                        const int valueLength)
{
  if (mDimensions == NULL) {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "Must set dimensions before the properties"
                         " it contains can be retrieved.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
  else {
    if (index < (int)mDimensions->getItemProperties().size()) {
      std::map<std::string, std::string>::iterator walker =
        mDimensions->getItemProperties().begin();
      for (int i = 0; i<index; i++) {
        walker++;
      }
      char * tempKey = strdup((*walker).first.c_str());
      memset(key, 0, keyLength);
      memcpy(key, tempKey, strlen(tempKey)+1);
      delete [] tempKey;
      char * tempValue = strdup((*walker).second.c_str());
      memset(value, 0, valueLength);
      memcpy(value, tempValue, strlen(tempValue)+1);
      delete [] tempValue;
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Index out of range.");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
  }
}

void
XdmfFortran::retrieveDimensionsPropertyByKey(char * key,
                                             char * value,
                                             const int valueLength)
{
  if (mDimensions == NULL) {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "Must set dimensions before the properties"
                         " it contains can be retrieved.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
  else {
    std::string tempString = key;
    memset(value, 0, valueLength);
    if ((mDimensions->getItemProperties().count(tempString))>0) {
      char * tempValue =
        strdup(mDimensions->getItemProperties()[tempString].c_str());
      memcpy(value, tempValue, strlen(tempValue)+1);
      delete [] tempValue;
    }
  }
}

int
XdmfFortran::setOrigin(const int numValues, const int arrayType, void * pointValues)
{
  mOrigin = XdmfArray::New();

  // insert origin values into array
  writeToArray(mOrigin, numValues, arrayType, pointValues);

  int id = mPreviousOrigins.size();
  mPreviousOrigins.push_back(mOrigin);
  return id;
}

void
XdmfFortran::setPreviousOrigin(const int index)
{
  if (index < (int)mPreviousOrigins.size()) {
    mOrigin = mPreviousOrigins[index];
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "Error: Index out of bounds.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

void
XdmfFortran::clearPreviousOrigins()
{
  mPreviousOrigins.clear();
}

void
XdmfFortran::modifyOriginValues(void * values,
                                const int arrayType,
                                const int numValues,
                                const int startIndex,
                                const int arrayStride,
                                const int valueStride)
{
  if(mOrigin == NULL) {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Must set origin before its values can be modified.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
  else {
    if (!mOrigin->isInitialized()) {
      mOrigin->read();
    }
    writeToArray(mOrigin,
                 numValues,
                 arrayType,
                 values,
                 startIndex,
                 arrayStride,
                 valueStride);
  }
}

void
XdmfFortran::retrieveOriginTag(char * tag, const int tagLength)
{
  if (mOrigin == NULL) {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "Must set origin before its tag can be retrieved.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
  else {
    char * tempTag = strdup(mOrigin->getItemTag().c_str());
    memset(tag, 0, tagLength);
    memcpy(tag, tempTag, strlen(tempTag)+1);
    delete [] tempTag;
  }
}

int
XdmfFortran::retrieveOriginValueType()
{
  if (mOrigin == NULL) {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "Must set origin before its value type"
                         " can be retrieved.");
    }
    catch (XdmfError e) {
      throw e;
    }
    return -1;
  }
  else {
    shared_ptr<const XdmfArrayType> dataType = mOrigin->getArrayType();
    if (dataType == XdmfArrayType::Int8()) {
      return XDMF_ARRAY_TYPE_INT8;
    }
    else if (dataType == XdmfArrayType::Int16()) {
      return XDMF_ARRAY_TYPE_INT16;
    }
    else if (dataType == XdmfArrayType::Int32()) {
      return XDMF_ARRAY_TYPE_INT32;
    }
    else if (dataType == XdmfArrayType::Int64()) {
      return XDMF_ARRAY_TYPE_INT64;
    }
    else if (dataType == XdmfArrayType::UInt8()) {
      return XDMF_ARRAY_TYPE_UINT8;
    }
    else if (dataType == XdmfArrayType::UInt16()) {
      return XDMF_ARRAY_TYPE_UINT16;
    }
    else if (dataType == XdmfArrayType::UInt32()) {
      return XDMF_ARRAY_TYPE_UINT32;
    }
    else if (dataType == XdmfArrayType::Float32()) {
      return XDMF_ARRAY_TYPE_FLOAT32;
    }
    else if (dataType == XdmfArrayType::Float64()) {
      return XDMF_ARRAY_TYPE_FLOAT64;
    }
    else if (dataType == XdmfArrayType::Uninitialized()) {
      try {
        XdmfError::message(XdmfError::FATAL, 
                           "Uninitialized Origin Data Type.");
      }
      catch (XdmfError e) {
        throw e;
      }
      return -1;
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL, 
                           "Invalid Origin Data Type.");
      }
      catch (XdmfError e) {
        throw e;
      }
      return -1;
    }
  }
}

void
XdmfFortran::retrieveOriginValues(void * values,
                                  const int dataType,
                                  const int numberRead,
                                  const int startIndex,
                                  const int arrayStride,
                                  const int valueStride)
{
  if(mOrigin == NULL) {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "Must set origin before its values can be retrieved.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
  else {
    if (!mOrigin->isInitialized()) {
      mOrigin->read();
    }
    readFromArray(mOrigin,
                  dataType,
                  values,
                  numberRead,
                  startIndex,
                  arrayStride,
                  valueStride);
  }
}

int
XdmfFortran::retrieveOriginSize()
{
  if (mOrigin == NULL) {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "Must set origin before its size can be retrieved.");
    }
    catch (XdmfError e) {
      throw e;
    }
    return -1;
  }
  else {
    return mOrigin->getSize();
  }
}

int
XdmfFortran::retrieveOriginNumProperties()
{
  if (mOrigin == NULL) {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "Must set origin before the number of properties"
                         " it contains can be retrieved.");
    }
    catch (XdmfError e) {
      throw e;
    }
    return -1;
  }
  else {
    return mOrigin->getItemProperties().size();
  } 
}

void
XdmfFortran::retrieveOriginProperty(const int index,
                                    char * key,
                                    const int keyLength,
                                    char * value,
                                    const int valueLength)
{
  if (mOrigin == NULL) {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "Must set Origin before the properties"
                         " it contains can be retrieved.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
  else {
    if (index < (int)mOrigin->getItemProperties().size()) {
      std::map<std::string, std::string>::iterator walker =
        mOrigin->getItemProperties().begin();
      for (int i = 0; i<index; i++) {
        walker++;
      }
      char * tempKey = strdup((*walker).first.c_str());
      memset(key, 0, keyLength);
      memcpy(key, tempKey, strlen(tempKey)+1);
      delete [] tempKey;
      char * tempValue = strdup((*walker).second.c_str());
      memset(value, 0, valueLength);
      memcpy(value, tempValue, strlen(tempValue)+1);
      delete [] tempValue;
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Index out of range.");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
  }
}

void
XdmfFortran::retrieveOriginPropertyByKey(char * key, char * value, const int valueLength)
{
  if (mOrigin == NULL) {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "Must set origin before the properties"
                         " it contains can be retrieved.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
  else {
    std::string tempString = key;
    memset(value, 0, valueLength);
    if ((mOrigin->getItemProperties().count(tempString))>0) {
      char * tempValue = strdup(mOrigin->getItemProperties()[tempString].c_str());
      memcpy(value, tempValue, strlen(tempValue)+1);
      delete [] tempValue;
    }
  }
}

int
XdmfFortran::setBrick(const int numValues, const int arrayType, void * pointValues)
{
  mBrick = XdmfArray::New();

  // insert brick values into array
  writeToArray(mBrick, numValues, arrayType, pointValues);

  int id = mPreviousBricks.size();
  mPreviousBricks.push_back(mBrick);
  return id;
}

void
XdmfFortran::setPreviousBrick(const int index)
{
  if (index < (int)mPreviousBricks.size()) {
    mBrick = mPreviousBricks[index];
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "Error: Index out of bounds.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

void
XdmfFortran::clearPreviousBricks()
{
  mPreviousBricks.clear();
}

void
XdmfFortran::modifyBrickValues(void * values,
                               const int arrayType,
                               const int numValues,
                               const int startIndex,
                               const int arrayStride,
                               const int valueStride)
{
  if(mBrick == NULL) {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Must set brick before its values can be modified.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
  else {
    if (!mBrick->isInitialized()) {
      mBrick->read();
    }
    writeToArray(mBrick,
                 numValues,
                 arrayType,
                 values,
                 startIndex,
                 arrayStride,
                 valueStride);
  }
}

void
XdmfFortran::retrieveBrickTag(char * tag, const int tagLength)
{
  if (mBrick == NULL) {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "Must set Brick before its tag can be retrieved.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
  else {
    char * tempTag = strdup(mBrick->getItemTag().c_str());
    memset(tag, 0, tagLength);
    memcpy(tag, tempTag, strlen(tempTag)+1);
    delete [] tempTag;
  }
}

int
XdmfFortran::retrieveBrickValueType()
{
  if (mBrick == NULL) {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "Must set brick before its value type can be retrieved.");
    }
    catch (XdmfError e) {
      throw e;
    }
    return -1;
  }
  else {
    shared_ptr<const XdmfArrayType> dataType = mBrick->getArrayType();
    if (dataType == XdmfArrayType::Int8()) {
      return XDMF_ARRAY_TYPE_INT8;
    }
    else if (dataType == XdmfArrayType::Int16()) {
      return XDMF_ARRAY_TYPE_INT16;
    }
    else if (dataType == XdmfArrayType::Int32()) {
      return XDMF_ARRAY_TYPE_INT32;
    }
    else if (dataType == XdmfArrayType::Int64()) {
      return XDMF_ARRAY_TYPE_INT64;
    }
    else if (dataType == XdmfArrayType::UInt8()) {
      return XDMF_ARRAY_TYPE_UINT8;
    }
    else if (dataType == XdmfArrayType::UInt16()) {
      return XDMF_ARRAY_TYPE_UINT16;
    }
    else if (dataType == XdmfArrayType::UInt32()) {
      return XDMF_ARRAY_TYPE_UINT32;
    }
    else if (dataType == XdmfArrayType::Float32()) {
      return XDMF_ARRAY_TYPE_FLOAT32;
    }
    else if (dataType == XdmfArrayType::Float64()) {
      return XDMF_ARRAY_TYPE_FLOAT64;
    }
    else if (dataType == XdmfArrayType::Uninitialized()) {
      try {
        XdmfError::message(XdmfError::FATAL, 
                           "Uninitialized Brick Data Type.");
      }
      catch (XdmfError e) {
        throw e;
      }
      return -1;
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL, 
                           "Invalid Brick Data Type.");
      }
      catch (XdmfError e) {
        throw e;
      }
      return -1;
    }
  }
}

void
XdmfFortran::retrieveBrickValues(void * values,
                                 const int dataType,
                                 const int numberRead,
                                 const int startIndex,
                                 const int arrayStride,
                                 const int valueStride)
{
  if(mBrick == NULL) {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "Must set brick before its values can be retrieved.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
  else {
    if (!mBrick->isInitialized()) {
      mBrick->read();
    }
    readFromArray(mBrick,
                  dataType,
                  values,
                  numberRead,
                  startIndex,
                  arrayStride,
                  valueStride);
  }
}

int
XdmfFortran::retrieveBrickSize()
{
  if (mBrick == NULL) {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "Must set brick before its size can be retrieved.");
    }
    catch (XdmfError e) {
      throw e;
    }
    return -1;
  }
  else {
    return mBrick->getSize();
  }
}

int
XdmfFortran::retrieveBrickNumProperties()
{
  if (mBrick == NULL) {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "Must set brick before the number of properties"
                         " it contains can be retrieved.");
    }
    catch (XdmfError e) {
      throw e;
    }
    return -1;
  }
  else {
    return mBrick->getItemProperties().size();
  } 
}

void
XdmfFortran::retrieveBrickProperty(const int index,
                                   char * key,
                                   const int keyLength,
                                   char * value,
                                   const int valueLength)
{
  if (mBrick == NULL) {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "Must set brick before the properties"
                         " it contains can be retrieved.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
  else {
    if (index < (int)mBrick->getItemProperties().size()) {
      std::map<std::string, std::string>::iterator walker =
        mBrick->getItemProperties().begin();
      for (int i = 0; i<index; i++) {
        walker++;
      }
      char * tempKey = strdup((*walker).first.c_str());
      memset(key, 0, keyLength);
      memcpy(key, tempKey, strlen(tempKey)+1);
      delete [] tempKey;
      char * tempValue = strdup((*walker).second.c_str());
      memset(value, 0, valueLength);
      memcpy(value, tempValue, strlen(tempValue)+1);
      delete [] tempValue;
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Index out of range.");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
  }
}

void
XdmfFortran::retrieveBrickPropertyByKey(char * key, char * value, const int valueLength)
{
  if (mBrick == NULL) {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "Must set brick before the properties"
                         " it contains can be retrieved.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
  else {
    std::string tempString = key;
    memset(value, 0, valueLength);
    if ((mBrick->getItemProperties().count(tempString))>0) {
      char * tempValue = strdup(mBrick->getItemProperties()[tempString].c_str());
      memcpy(value, tempValue, strlen(tempValue)+1);
      delete [] tempValue;
    }
  }
}

void
XdmfFortran::addMap(char * name)
{
  shared_ptr<XdmfMap> addedMap = XdmfMap::New();
  addedMap->setName(name);
  mMaps.push_back(addedMap);
}

int
XdmfFortran::retrieveNumMaps()
{
  return mMaps.size();
}

void
XdmfFortran::clearMaps()
{
  mMaps.clear();
}

void
XdmfFortran::retrieveMapTag(const int index, char * tag, const int tagLength)
{
  if (index < (int)mMaps.size()) {
    char * tempTag = strdup(mMaps[index]->getItemTag().c_str());
    memset(tag, 0, tagLength);
    memcpy(tag, tempTag, strlen(tempTag)+1);
    delete [] tempTag;
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

void
XdmfFortran::addRemoteNodeID(const int index, int localNodeID, int remoteTaskID, int remoteLocalNodeID)
{
  mMaps[index]->insert(remoteTaskID, localNodeID, remoteLocalNodeID);
}

void
XdmfFortran::retrieveRemoteNodeIDs(const int index, const int localNodeID, const int remoteTaskID, int * remoteNodeIDs)
{
  if ((int)mMaps.size()>index) {
    if (!mMaps[index]->isInitialized()) {
      mMaps[index]->read();
    }
    if (mMaps[index]->getRemoteNodeIds(remoteTaskID).count(localNodeID)>0) {
      std::set<int> returnSet = mMaps[index]->getRemoteNodeIds(remoteTaskID)[localNodeID];
      std::set<int>::iterator walker;
      int i = 0;
      for (walker = returnSet.begin(); walker != returnSet.end();walker++) {
        remoteNodeIDs[i] =  *walker;
        i++;
      }
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL, 
                           "The map does not contain a remote ID for the"
                           " requested node.");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "Error: Index out of Range.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

int
XdmfFortran::retrieveNumRemoteNodeIDs(const int index, const int localNodeID, const int remoteTaskID)
{
  if ((int)mMaps.size()<index) {
    if (!mMaps[index]->isInitialized()) {
      mMaps[index]->read();
    }
    if (mMaps[index]->getRemoteNodeIds(remoteTaskID).count(localNodeID)>0) {
      return mMaps[index]->getRemoteNodeIds(remoteTaskID)[localNodeID].size();
    }
    else {
      return 0;
    }
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "Error: Index out of Range.");
    }
    catch (XdmfError e) {
      throw e;
    }
    return -1;
  }
}

int
XdmfFortran::storeMap(const int index)
{
  if (index < (int)mMaps.size()) {
    int id = mPreviousMaps.size();
    mPreviousMaps.push_back(mMaps[index]);
    return id;
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "Error: Index out of Range.");
    }
    catch (XdmfError e) {
      throw e;
    }
    return -1;
  }
}

void
XdmfFortran::addPreviousMap(const int index)
{
  if (index < (int)mPreviousMaps.size()) {
    mMaps.push_back(mPreviousMaps[index]);
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

void
XdmfFortran::clearPreviousMaps()
{
  mPreviousMaps.clear();
}

void
XdmfFortran::removeMap(const int index)
{ 
  if (index < (int)mMaps.size()) {
    mMaps.erase(mMaps.begin()+index);
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

int
XdmfFortran::retrieveMapNumProperties(const int index)
{
  if (index < (int)mMaps.size()) {
    return mMaps[index]->getItemProperties().size();
  } 
  else {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
    return -1;
  }
}

void
XdmfFortran::retrieveMapProperty(const int mapIndex,
                                 const int index,
                                 char * key,
                                 const int keyLength,
                                 char * value,
                                 const int valueLength)
{
  if (mapIndex < (int)mMaps.size()) {
    if (index < (int)mMaps[index]->getItemProperties().size()) {
      std::map<std::string, std::string>::iterator walker =
        mMaps[index]->getItemProperties().begin();
      for (int i = 0; i<index; i++) {
        walker++;
      }
      char * tempKey = strdup((*walker).first.c_str());
      memset(key, 0, keyLength);
      memcpy(key, tempKey, strlen(tempKey)+1);
      delete [] tempKey;
      char * tempValue = strdup((*walker).second.c_str());
      memset(value, 0, valueLength);
      memcpy(value, tempValue, strlen(tempValue)+1);
      delete [] tempValue;
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Index out of range.");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

void
XdmfFortran::retrieveMapPropertyByKey(const int index,
                                      char * key,
                                      char * value,
                                      const int valueLength)
{
  if (index < (int)mMaps.size()) {
    std::string tempString = key;
    memset(value, 0, valueLength);
    if ((mMaps[index]->getItemProperties().count(tempString))>0) {
      char * tempValue = strdup(mMaps[index]->getItemProperties()[tempString].c_str());
      memcpy(value, tempValue, strlen(tempValue)+1);
      delete [] tempValue;
    }
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL, 
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

int
XdmfFortran::retrieveNumAttributes()
{
  return mAttributes.size();
}

void
XdmfFortran::clearAttributes()
{
  mAttributes.clear();
}

void
XdmfFortran::retrieveAttributeTag(const int index, char * tag, const int tagLength)
{
  if (index < (int)mAttributes.size()) {
    char * tempTag = strdup(mAttributes[index]->getItemTag().c_str());
    memset(tag, 0, tagLength);
    memcpy(tag, tempTag, strlen(tempTag)+1);
    delete [] tempTag;
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

void
XdmfFortran::retrieveAttributeName(const int index, char * name, const int nameLength)
{
  if (index < (int)mAttributes.size()) {
    char * tempName = strdup(mAttributes[index]->getName().c_str());
    memset(name, 0, nameLength);
    memcpy(name, tempName, strlen(tempName)+1);
    delete [] tempName;
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

int
XdmfFortran::retrieveAttributeValueType(const int index)
{
  if (index < (int)mAttributes.size()) {
    shared_ptr<const XdmfArrayType> dataType = mAttributes[index]->getArrayType();
    if (dataType == XdmfArrayType::Int8()) {
      return XDMF_ARRAY_TYPE_INT8;
    }
    else if (dataType == XdmfArrayType::Int16()) {
      return XDMF_ARRAY_TYPE_INT16;
    }
    else if (dataType == XdmfArrayType::Int32()) {
      return XDMF_ARRAY_TYPE_INT32;
    }
    else if (dataType == XdmfArrayType::Int64()) {
      return XDMF_ARRAY_TYPE_INT64;
    }
    else if (dataType == XdmfArrayType::UInt8()) {
      return XDMF_ARRAY_TYPE_UINT8;
    }
    else if (dataType == XdmfArrayType::UInt16()) {
      return XDMF_ARRAY_TYPE_UINT16;
    }
    else if (dataType == XdmfArrayType::UInt32()) {
      return XDMF_ARRAY_TYPE_UINT32;
    }
    else if (dataType == XdmfArrayType::Float32()) {
      return XDMF_ARRAY_TYPE_FLOAT32;
    }
    else if (dataType == XdmfArrayType::Float64()) {
      return XDMF_ARRAY_TYPE_FLOAT64;
    }
    else if (dataType == XdmfArrayType::Uninitialized()) {
      try {
        XdmfError::message(XdmfError::FATAL, 
                           "Uninitialized Attribute Data Type.");
      }
      catch (XdmfError e) {
        throw e;
      }
      return -1;
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL, 
                           "Invalid Attribute Data Type.");
      }
      catch (XdmfError e) {
        throw e;
      }
      return -1;
    }
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
    return -1;
  }
}

void
XdmfFortran::retrieveAttributeValues(const int index,
                                     void * values,
                                     const int dataType,
                                     const int numberRead,
                                     const int startIndex,
                                     const int arrayStride,
                                     const int valueStride)
{
  if (index < (int)mAttributes.size()) {
    if (!mAttributes[index]->isInitialized()) {
      mAttributes[index]->read();
    }
    readFromArray(mAttributes[index],
                  dataType,
                  values,
                  numberRead,
                  startIndex,
                  arrayStride,
                  valueStride);
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

void
XdmfFortran::removeAttribute(const int index)
{
  if (index < (int)mAttributes.size()) {
    mAttributes.erase(mAttributes.begin()+index);
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
  } 
}

void
XdmfFortran::replaceAttribute(const int index,
                              char * name,
                              int attributeCenter,
                              int attributeType,
                              const int numValues,
                              const int arrayType,
                              void * values)
{
  if (index < (int)mAttributes.size()) {
    shared_ptr<XdmfAttribute> currAttribute = XdmfAttribute::New();
    currAttribute->setName(name);
    switch(attributeCenter) {
      case XDMF_ATTRIBUTE_CENTER_GRID:
        currAttribute->setCenter(XdmfAttributeCenter::Grid());
        break;
      case XDMF_ATTRIBUTE_CENTER_CELL:
        currAttribute->setCenter(XdmfAttributeCenter::Cell());
        break;
      case XDMF_ATTRIBUTE_CENTER_FACE:
        currAttribute->setCenter(XdmfAttributeCenter::Face());
        break;
      case XDMF_ATTRIBUTE_CENTER_EDGE:
        currAttribute->setCenter(XdmfAttributeCenter::Edge());
        break;
      case XDMF_ATTRIBUTE_CENTER_NODE:
        currAttribute->setCenter(XdmfAttributeCenter::Node());
        break;
      default:
        try {
          XdmfError::message(XdmfError::FATAL,
                             "Invalid attribute center");
        }
        catch (XdmfError e) {
          throw e;
        }
    }

    switch(attributeType) {
      case XDMF_ATTRIBUTE_TYPE_SCALAR:
        currAttribute->setType(XdmfAttributeType::Scalar());
        break;
      case XDMF_ATTRIBUTE_TYPE_VECTOR:
        currAttribute->setType(XdmfAttributeType::Vector());
        break;
      case XDMF_ATTRIBUTE_TYPE_TENSOR:
        currAttribute->setType(XdmfAttributeType::Tensor());
        break;
      case XDMF_ATTRIBUTE_TYPE_MATRIX:
        currAttribute->setType(XdmfAttributeType::Matrix());
        break;
      case XDMF_ATTRIBUTE_TYPE_TENSOR6:
        currAttribute->setType(XdmfAttributeType::Tensor6());
        break;
      case XDMF_ATTRIBUTE_TYPE_GLOBALID:
        currAttribute->setType(XdmfAttributeType::GlobalId());
        break;
      default:
        try {
          XdmfError::message(XdmfError::FATAL,
                             "Invalid attribute type");
        }
        catch (XdmfError e) {
          throw e;
        }
    }

    writeToArray(currAttribute, numValues, arrayType, values);
    mAttributes[index] = currAttribute;
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
  } 
}

void
XdmfFortran::openAttribute(const int index)
{
  if (index < (int)mAttributes.size()) {
    int i;
    shared_ptr<XdmfAttribute> openedAttribute = mAttributes[index];
    int n = openedAttribute->getNumberInformations();
    shared_ptr<XdmfInformation> openedInformation;
    for (i = 0; i < n; i++) {
      openedInformation = openedAttribute->getInformation(i);
      mInformations.push_back(openedInformation);
      mPreviousInformations.push_back(openedInformation);
    }
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

int
XdmfFortran::retrieveAttributeSize(const int index)
{
  if (index < (int)mAttributes.size()) {
    return mAttributes[index]->getSize();
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
    return -1;
  }
}

int
XdmfFortran::retrieveAttributeType(const int index)
{
  if (index < (int)mAttributes.size()) {
    shared_ptr<const XdmfAttributeType> returnType =
      mAttributes[index]->getType();
    if (returnType == XdmfAttributeType::Scalar()) {
      return XDMF_ATTRIBUTE_TYPE_SCALAR;
    }
    else if (returnType == XdmfAttributeType::Vector()) {
      return XDMF_ATTRIBUTE_TYPE_VECTOR;
    }
    else if (returnType == XdmfAttributeType::Tensor()) {
      return XDMF_ATTRIBUTE_TYPE_TENSOR;
    }
    else if (returnType == XdmfAttributeType::Matrix()) {
      return XDMF_ATTRIBUTE_TYPE_MATRIX;
    }
    else if (returnType == XdmfAttributeType::Tensor6()) {
      return XDMF_ATTRIBUTE_TYPE_TENSOR6;
    }
    else if (returnType == XdmfAttributeType::GlobalId()) {
      return XDMF_ATTRIBUTE_TYPE_GLOBALID;
    }
    else {
      return XDMF_ATTRIBUTE_TYPE_NOTYPE;
    }
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
    return -1;
  }
}

int
XdmfFortran::retrieveAttributeCenter(const int index)
{
  if (index < (int)mAttributes.size()) {
    shared_ptr<const XdmfAttributeCenter> returnCenter =
      mAttributes[index]->getCenter();
    if (returnCenter == XdmfAttributeCenter::Grid()) {
      return XDMF_ATTRIBUTE_CENTER_GRID;
    }
    else if (returnCenter == XdmfAttributeCenter::Cell()) {
      return XDMF_ATTRIBUTE_CENTER_CELL;
    }
    else if (returnCenter == XdmfAttributeCenter::Face()) {
      return XDMF_ATTRIBUTE_CENTER_FACE;
    }
    else if (returnCenter == XdmfAttributeCenter::Edge()) {
      return XDMF_ATTRIBUTE_CENTER_EDGE;
    }
    else if (returnCenter == XdmfAttributeCenter::Node()) {
      return XDMF_ATTRIBUTE_CENTER_NODE;
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Invalid Attribute Center.");
      }
      catch (XdmfError e) {
        throw e;
      }
      return -1;
    }
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
    return -1;
  }
}

void
XdmfFortran::clearPreviousAttributes()
{
  mPreviousAttributes.clear();
}

void
XdmfFortran::modifyAttributeValues(const int index,
                                   void * values,
                                   const int arrayType,
                                   const int numValues,
                                   const int startIndex,
                                   const int arrayStride,
                                   const int valueStride)
{
  if (index < (int)mAttributes.size()) {
    if (!mAttributes[index]->isInitialized()) {
      mAttributes[index]->read();
    }
    writeToArray(mAttributes[index],
                 numValues,
                 arrayType,
                 values,
                 startIndex,
                 arrayStride,
                 valueStride);
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

int
XdmfFortran::retrieveAttributeNumProperties(const int index)
{
  if (index < (int)mAttributes.size()) {
    return mAttributes[index]->getItemProperties().size();
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
    return -1;
  } 
}

void
XdmfFortran::retrieveAttributeProperty(const int attributeIndex,
                                       const int index,
                                       char * key,
                                       const int keyLength,
                                       char * value,
                                       const int valueLength)
{
  if (attributeIndex < (int)mAttributes.size()) {
    if (index < (int)mAttributes[attributeIndex]->getItemProperties().size()) {
      std::map<std::string, std::string>::iterator walker =
        mAttributes[attributeIndex]->getItemProperties().begin();
      for (int i = 0; i<index; i++) {
        walker++;
      }
      char * tempKey = strdup((*walker).first.c_str());
      memset(key, 0, keyLength);
      memcpy(key, tempKey, strlen(tempKey)+1);
      delete [] tempKey;
      char * tempValue = strdup((*walker).second.c_str());
      memset(value, 0, valueLength);
      memcpy(value, tempValue, strlen(tempValue)+1);
      delete [] tempValue;
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Property index out of range.");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

void
XdmfFortran::retrieveAttributePropertyByKey(const int index,
                                            char * key,
                                            char * value,
                                            const int valueLength)
{
  if (index < (int)mAttributes.size()) {
    std::string tempString = key;
    memset(value, 0, valueLength);
    if ((mAttributes[index]->getItemProperties().count(tempString))>0) {
      char * tempValue = strdup(mAttributes[index]->getItemProperties()[tempString].c_str());
      memcpy(value, tempValue, strlen(tempValue)+1);
      delete [] tempValue;
    }
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

int
XdmfFortran::addCoordinate(char * name, const int numValues, const int arrayType, void * values)
{
  shared_ptr<XdmfArray> currArray = XdmfArray::New();
  currArray->setName(name);

  writeToArray(currArray, numValues, arrayType, values);

  mCoordinates.push_back(currArray);

  const int id = mPreviousCoordinates.size();
  mPreviousCoordinates.push_back(currArray);
  return id;
}

void
XdmfFortran::addPreviousCoordinate(const int index)
{
  if (index < (int)mPreviousCoordinates.size()) {
    mCoordinates.push_back(mPreviousCoordinates[index]);
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

void
XdmfFortran::clearPreviousCoordinates()
{
  mPreviousCoordinates.clear();
}

void
XdmfFortran::modifyCoordinateValues(const int index,
                                    void * values,
                                    const int arrayType,
                                    const int numValues,
                                    const int startIndex,
                                    const int arrayStride,
                                    const int valueStride)
{
  if (index < (int)mCoordinates.size()) {
    if (!mCoordinates[index]->isInitialized()) {
      mCoordinates[index]->read();
    }
    writeToArray(mCoordinates[index],
                 numValues,
                 arrayType,
                 values,
                 startIndex,
                 arrayStride,
                 valueStride);
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

int
XdmfFortran::retrieveNumCoordinates()
{
  return mCoordinates.size();
}

void
XdmfFortran::retrieveCoordinateTag(const int index, char * tag, const int tagLength)
{
  if (index < (int)mCoordinates.size()) {
    char * tempTag = strdup(mCoordinates[index]->getItemTag().c_str());
    memset(tag, 0, tagLength);
    memcpy(tag, tempTag, strlen(tempTag)+1);
    delete [] tempTag;
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

void
XdmfFortran::retrieveCoordinateName(const int index, char * name, const int nameLength)
{
  if (index < (int)mCoordinates.size()) {
    char * tempName = strdup(mCoordinates[index]->getName().c_str());
    memset(name, 0, nameLength);
    memcpy(name, tempName, strlen(tempName)+1);
    delete [] tempName;
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

int
XdmfFortran::retrieveCoordinateValueType(const int index)
{
  if (index < (int)mCoordinates.size()) {
    shared_ptr<const XdmfArrayType> dataType = mCoordinates[index]->getArrayType();
    if (dataType == XdmfArrayType::Int8()) {
      return XDMF_ARRAY_TYPE_INT8;
    }
    else if (dataType == XdmfArrayType::Int16()) {
      return XDMF_ARRAY_TYPE_INT16;
    }
    else if (dataType == XdmfArrayType::Int32()) {
      return XDMF_ARRAY_TYPE_INT32;
    }
    else if (dataType == XdmfArrayType::Int64()) {
      return XDMF_ARRAY_TYPE_INT64;
    }
    else if (dataType == XdmfArrayType::UInt8()) {
      return XDMF_ARRAY_TYPE_UINT8;
    }
    else if (dataType == XdmfArrayType::UInt16()) {
      return XDMF_ARRAY_TYPE_UINT16;
    }
    else if (dataType == XdmfArrayType::UInt32()) {
      return XDMF_ARRAY_TYPE_UINT32;
    }
    else if (dataType == XdmfArrayType::Float32()) {
      return XDMF_ARRAY_TYPE_FLOAT32;
    }
    else if (dataType == XdmfArrayType::Float64()) {
      return XDMF_ARRAY_TYPE_FLOAT64;
    }
    else if (dataType == XdmfArrayType::Uninitialized()) {
      try {
        XdmfError::message(XdmfError::FATAL, 
                           "Uninitialized Coordinate Data Type.");
      }
      catch (XdmfError e) {
        throw e;
      }
      return -1;
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL, 
                           "Invalid Coordinate Data Type.");
      }
      catch (XdmfError e) {
        throw e;
      }
      return -1;
    }
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
    return -1;
  }
}

void
XdmfFortran::retrieveCoordinateValues(const int index,
                                      void * values,
                                      const int dataType,
                                      const int numberRead,
                                      const int startIndex,
                                      const int arrayStride,
                                      const int valueStride)
{
  if (index < (int)mCoordinates.size()) {
    if (!mCoordinates[index]->isInitialized()) {
      mCoordinates[index]->read();
    }
    readFromArray(mCoordinates[index],
                  dataType,
                  values,
                  numberRead,
                  startIndex,
                  arrayStride,
                  valueStride);
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

void
XdmfFortran::removeCoordinate(const int index)
{
  if (index < (int)mCoordinates.size()) {
    mCoordinates.erase(mCoordinates.begin()+index);
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
  } 
}

void
XdmfFortran::replaceCoordinate(const int index,
                               char * name,
                               const int numValues,
                               const int arrayType,
                               void * values)
{
  if (index < (int)mCoordinates.size()) {
    shared_ptr<XdmfArray> currArray = XdmfArray::New();
    currArray->setName(name);
    writeToArray(currArray, numValues, arrayType, values);
    mCoordinates[index] = currArray;
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

int
XdmfFortran::retrieveCoordinateSize(const int index)
{
  if (index < (int)mCoordinates.size()) {
    return mCoordinates[index]->getSize();
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
    return -1;
  }
}

void
XdmfFortran::clearCoordinates()
{
  mCoordinates.clear();
}

int
XdmfFortran::retrieveCoordinateNumProperties(const int index)
{
  if (index < (int)mCoordinates.size()) {
    return mCoordinates[index]->getItemProperties().size();
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
    return -1;
  } 
}

void
XdmfFortran::retrieveCoordinateProperty(const int coordinateIndex,
                                        const int index,
                                        char * key,
                                        const int keyLength,
                                        char * value,
                                        const int valueLength)
{
  if (coordinateIndex < (int)mCoordinates.size()) {
    if (index < (int)mCoordinates[coordinateIndex]->getItemProperties().size()) {
      std::map<std::string, std::string>::iterator walker =
        mCoordinates[coordinateIndex]->getItemProperties().begin();
      for (int i = 0; i<index; i++) {
        walker++;
      }
      char * tempKey = strdup((*walker).first.c_str());
      memset(key, 0, keyLength);
      memcpy(key, tempKey, strlen(tempKey)+1);
      delete [] tempKey;
      char * tempValue = strdup((*walker).second.c_str());
      memset(value, 0, valueLength);
      memcpy(value, tempValue, strlen(tempValue)+1);
      delete [] tempValue;
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Property index out of range.");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

void
XdmfFortran::retrieveCoordinatePropertyByKey(const int index,
                                             char * key,
                                             char * value,
                                             const int valueLength)
{
  if (index < (int)mCoordinates.size()) {
    std::string tempString = key;
    memset(value, 0, valueLength);
    if ((mCoordinates[index]->getItemProperties().count(tempString))>0) {
      char * tempValue = strdup(mCoordinates[index]->getItemProperties()[tempString].c_str());
      memcpy(value, tempValue, strlen(tempValue)+1);
      delete [] tempValue;
    }
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

int 
XdmfFortran::addSet(char * name,
                    const int newSetType,
                    void * values,
                    const int numValues,
                    const int arrayType)
{
  const shared_ptr<XdmfSet> newSet = XdmfSet::New();
  newSet->setName(name);

  switch (newSetType) {
    case XDMF_SET_TYPE_NODE:
      newSet->setType(XdmfSetType::Node());
      break;
    case XDMF_SET_TYPE_CELL:
      newSet->setType(XdmfSetType::Cell());
      break;
    case XDMF_SET_TYPE_FACE:
      newSet->setType(XdmfSetType::Face());
      break;
    case XDMF_SET_TYPE_EDGE:
      newSet->setType(XdmfSetType::Edge());
      break;
    default:
      try {
        XdmfError::message(XdmfError::FATAL, 
                           "Invalid set type.");
      }
      catch (XdmfError e) {
        throw e;
      }
      return -1;
  }

  for(std::vector<shared_ptr<XdmfAttribute> >::const_iterator iter =
      mAttributes.begin(); 
      iter != mAttributes.end();
      ++iter) {
    newSet->insert(*iter);
  }
  mAttributes.clear();

  if (!mInformations.empty()) {
    for(std::vector<shared_ptr<XdmfInformation> >::const_iterator iter =
        mInformations.begin(); 
        iter != mInformations.end();
        ++iter) {
      newSet->insert(*iter);
    }
    mInformations.clear();
  }

  writeToArray(newSet, numValues, arrayType, values);

  mSets.push_back(newSet);
  int id = mPreviousSets.size();
  mPreviousSets.push_back(newSet);
  return id;
}

void
XdmfFortran::clearSets()
{
  mSets.clear();
}

void
XdmfFortran::addPreviousSet(const int index)
{
  if (index < (int)mPreviousSets.size()) {
    mSets.push_back(mPreviousSets[index]);
  }
  else
  {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

void
XdmfFortran::clearPreviousSets()
{
  mPreviousSets.clear();
}

void
XdmfFortran::modifySetValues(const int index,
                             void * values,
                             const int arrayType,
                             const int numValues,
                             const int startIndex,
                             const int arrayStride,
                             const int valueStride)
{
  if (index < (int)mSets.size()) {
    if (!mSets[index]->isInitialized()) {
      mSets[index]->read();
    }
    writeToArray(mSets[index],
                 numValues,
                 arrayType,
                 values,
                 startIndex,
                 arrayStride,
                 valueStride);
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

void
XdmfFortran::retrieveSetTag(const int index, char * tag, const int tagLength)
{
  if (index < (int)mSets.size()) {
    char * tempTag = strdup(mSets[index]->getItemTag().c_str());
    memset(tag, 0, tagLength);
    memcpy(tag, tempTag, strlen(tempTag)+1);
    delete [] tempTag;
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

void
XdmfFortran::retrieveSetName(const int index, char * name, const int nameLength)
{
  if (index < (int)mSets.size()) {
    char * tempName = strdup(mSets[index]->getName().c_str());
    memset(name, 0, nameLength);
    memcpy(name, tempName, strlen(tempName)+1);
    delete [] tempName;
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

int
XdmfFortran::retrieveNumSets()
{
  return mSets.size();
}

int
XdmfFortran::retrieveSetSize(const int index)
{
  if (index < (int)mSets.size()) {
    return mSets[index]->getSize();
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
    return -1;
  }
}

int
XdmfFortran::retrieveSetValueType(const int index)
{
  if (index < (int)mSets.size()) {
    shared_ptr<const XdmfArrayType> dataType = mSets[index]->getArrayType();
    if (dataType == XdmfArrayType::Int8()) {
      return XDMF_ARRAY_TYPE_INT8;
    }
    else if (dataType == XdmfArrayType::Int16()) {
      return XDMF_ARRAY_TYPE_INT16;
    }
    else if (dataType == XdmfArrayType::Int32()) {
      return XDMF_ARRAY_TYPE_INT32;
    }
    else if (dataType == XdmfArrayType::Int64()) {
      return XDMF_ARRAY_TYPE_INT64;
    }
    else if (dataType == XdmfArrayType::UInt8()) {
      return XDMF_ARRAY_TYPE_UINT8;
    }
    else if (dataType == XdmfArrayType::UInt16()) {
      return XDMF_ARRAY_TYPE_UINT16;
    }
    else if (dataType == XdmfArrayType::UInt32()) {
      return XDMF_ARRAY_TYPE_UINT32;
    }
    else if (dataType == XdmfArrayType::Float32()) {
      return XDMF_ARRAY_TYPE_FLOAT32;
    }
    else if (dataType == XdmfArrayType::Float64()) {
      return XDMF_ARRAY_TYPE_FLOAT64;
    }
    else if (dataType == XdmfArrayType::Uninitialized()) {
      try {
        XdmfError::message(XdmfError::FATAL, 
                           "Uninitialized Set Data Type.");
      }
      catch (XdmfError e) {
        throw e;
      }
      return -1;
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL, 
                           "Invalid Set Data Type.");
      }
      catch (XdmfError e) {
        throw e;
      }
      return -1;
    }
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
    return -1;
  }
}

void
XdmfFortran::retrieveSetValues(const int index,
                               void * values,
                               const int dataType,
                               const int numberRead,
                               const int startIndex,
                               const int arrayStride,
                               const int valueStride)
{
  if (index < (int)mSets.size()) {
    if (!mSets[index]->isInitialized()) {
      mSets[index]->read();
    }
    readFromArray(mSets[index],
                  dataType,
                  values,
                  numberRead,
                  startIndex,
                  arrayStride,
                  valueStride);
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

int
XdmfFortran::retrieveSetType(const int index)
{
  if (index < (int)mSets.size()) {
    shared_ptr<const XdmfSetType> returnType = mSets[index]->getType();
    if (returnType == XdmfSetType::Node()) {
      return XDMF_SET_TYPE_NODE;
    }
    else if (returnType == XdmfSetType::Cell()) {
      return XDMF_SET_TYPE_CELL;
    }
    else if (returnType == XdmfSetType::Face()) {
      return XDMF_SET_TYPE_FACE;
    }
    else if (returnType == XdmfSetType::Edge()) {
      return XDMF_SET_TYPE_EDGE;
    }
    else if (returnType == XdmfSetType::NoSetType()) {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Uninitialized set type.");
      }
      catch (XdmfError e) {
        throw e;
      }
      return -1;
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Invalid set type.");
      }
      catch (XdmfError e) {
        throw e;
      }
      return -1;
    }
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
    return -1;
  }
}

void
XdmfFortran::openSet(const int index, const int openAttribute, const int openInformation)
{
  if (index < (int)mSets.size()) {
    shared_ptr<XdmfSet> openedSet = mSets[index];
    int i;
    int n;
    n = openedSet->getNumberAttributes();
    if (openAttribute) {
      shared_ptr<XdmfAttribute> openedAttribute;
      for (i = 0; i < n; i++) {
        openedAttribute = openedSet->getAttribute(i);
        mAttributes.push_back(openedAttribute);
        mPreviousAttributes.push_back(openedAttribute);
      }
    }
    if (openInformation) {
      n = openedSet->getNumberInformations();
      shared_ptr<XdmfInformation> openedInformation;
      for (i = 0; i < n; i++) {
        openedInformation = openedSet->getInformation(i);
        mInformations.push_back(openedInformation);
        mPreviousInformations.push_back(openedInformation);
      }
    }
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

void
XdmfFortran::removeSet(const int index)
{
  if (index < (int)mSets.size()) {
    mSets.erase(mSets.begin()+index);
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

void 
XdmfFortran::replaceSet(const int index,
                        char * name,
                        const int newSetType,
                        void * values,
                        const int numValues,
                        const int arrayType)
{
  const shared_ptr<XdmfSet> newSet = XdmfSet::New();
  newSet->setName(name);

  switch (newSetType) {
    case XDMF_SET_TYPE_NODE:
      newSet->setType(XdmfSetType::Node());
      break;
    case XDMF_SET_TYPE_CELL:
      newSet->setType(XdmfSetType::Cell());
      break;
    case XDMF_SET_TYPE_FACE:
      newSet->setType(XdmfSetType::Face());
      break;
    case XDMF_SET_TYPE_EDGE:
      newSet->setType(XdmfSetType::Edge());
      break;
    default:
      try {
        XdmfError::message(XdmfError::FATAL, 
                           "Invalid set type.");
      }
      catch (XdmfError e) {
        throw e;
      }
  }

  for(std::vector<shared_ptr<XdmfAttribute> >::const_iterator iter =
      mAttributes.begin(); 
      iter != mAttributes.end();
      ++iter) {
    newSet->insert(*iter);
  }
  mAttributes.clear();

  if (!mInformations.empty()) {
    for(std::vector<shared_ptr<XdmfInformation> >::const_iterator iter =
        mInformations.begin(); 
        iter != mInformations.end();
        ++iter) {
      newSet->insert(*iter);
    }
    mInformations.clear();
  }

  writeToArray(newSet, numValues, arrayType, values);

  mSets[index] = newSet;
}

int
XdmfFortran::retrieveSetNumProperties(const int index)
{
  if (index < (int)mSets.size()) {
    return mSets[index]->getItemProperties().size();
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
    return -1;
  }
}

void
XdmfFortran::retrieveSetProperty(const int setIndex,
                                 const int index,
                                 char * key,
                                 const int keyLength,
                                 char * value,
                                 const int valueLength)
{
  if (setIndex < (int)mSets.size()) {
    if (index < (int)mSets[setIndex]->getItemProperties().size()) {
      std::map<std::string, std::string>::iterator walker = mSets[setIndex]->getItemProperties().begin();
      for (int i = 0; i<index; i++) {
        walker++;
      }
      char * tempKey = strdup((*walker).first.c_str());
      memset(key, 0, keyLength);
      memcpy(key, tempKey, strlen(tempKey)+1);
      delete [] tempKey;
      char * tempValue = strdup((*walker).second.c_str());
      memset(value, 0, valueLength);
      memcpy(value, tempValue, strlen(tempValue)+1);
      delete [] tempValue;
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Property index out of range.");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

void
XdmfFortran::retrieveSetPropertyByKey(const int index,
                                      char * key,
                                      char * value,
                                      const int valueLength)
{
  if (index < (int)mSets.size()) {
    std::string tempString = key;
    memset(value, 0, valueLength);
    if ((mSets[index]->getItemProperties().count(tempString))>0) {
      char * tempValue = strdup(mSets[index]->getItemProperties()[tempString].c_str());
      memcpy(value, tempValue, strlen(tempValue)+1);
      delete [] tempValue;
    }
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

int
XdmfFortran::retrieveNumInformation()
{
  return mInformations.size();
}

void
XdmfFortran::clearInformations()
{
  mInformations.clear();
}

void
XdmfFortran::retrieveInformationTag(const int index, char * tag, const int tagLength)
{
  if (index < (int)mInformations.size()) {
    char * tempTag = strdup(mInformations[index]->getItemTag().c_str());
    memset(tag, 0, tagLength);
    memcpy(tag, tempTag, strlen(tempTag)+1);
    delete [] tempTag;
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

void
XdmfFortran::retrieveInformation(const int index,
                                 char * key,
                                 const int keyLength,
                                 char * value,
                                 const int valueLength)
{
  if (index < (int)mInformations.size()) {
    char * tempKey = strdup(mInformations[index]->getKey().c_str());
    char * tempValue = strdup(mInformations[index]->getValue().c_str());
    memset(key, 0, keyLength);
    memset(value, 0, valueLength);
    memcpy(key, tempKey, strlen(tempKey)+1);
    memcpy(value, tempValue, strlen(tempValue)+1);
    delete [] tempKey;
    delete [] tempValue;
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

void
XdmfFortran::replaceInformation(const int index, char * key, char * value)
{
  if (index < (int)mInformations.size()) {
    mInformations[index]->setKey(key);
    mInformations[index]->setValue(value);
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

void
XdmfFortran::removeInformation(const int index)
{
  if (index < (int)mInformations.size()) {
    mInformations.erase(mInformations.begin()+index);
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

void
XdmfFortran::openInformation(const int index)
{
  if (index < (int)mInformations.size()) {
    shared_ptr<XdmfInformation> sourceInformation = mInformations[index];
    int i;
    int n = sourceInformation->getNumberInformations();
    shared_ptr<XdmfInformation> openedInformation;
    for (i = 0; i < n; i++) {
      openedInformation = sourceInformation->getInformation(i);
      mInformations.push_back(openedInformation);
      mPreviousInformations.push_back(openedInformation);
    }
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

void
XdmfFortran::retrieveInformationByKey(char * key, char * value, const int valueLength)
{
  unsigned int i;
  int found = 0;
  std::string searchString(key);
  char * tempValue;
  for (i=0;i<mInformations.size();i++) {
    if (searchString == mInformations[i]->getKey()) {
      found = 1;
      tempValue = strdup(mInformations[i]->getValue().c_str());
      memset(value, 0, valueLength);
      memcpy(value, tempValue, strlen(tempValue)+1);
      i = mInformations.size();
      delete [] tempValue;
    }
  }
  if (found == 0) {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Item with specifed key does not exist.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

void
XdmfFortran::replaceInformationByKey(char * key, char * value)
{
  unsigned int i;
  int found = 0;
  std::string searchString(key);
  for (i=0;i<mInformations.size();i++) {
    if (searchString == mInformations[i]->getKey()) {
      mInformations[i]->setValue(value);
      i = mInformations.size();
      found = 1;
    }
  }
  if (found == 0) {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Item with specifed key does not exist.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

void
XdmfFortran::removeInformationByKey(char * key)
{
  unsigned int i;
  int found = 0;
  std::string searchString(key);
  for (i=0;i<mInformations.size();i++) {
    if (searchString == mInformations[i]->getKey()) {
      mInformations.erase(mInformations.begin()+i);
      i = mInformations.size();
      found = 1;
    }
  }
  if (found == 0) {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Item with specifed key does not exist.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

void
XdmfFortran::clearPreviousInformation()
{
  mPreviousInformations.clear();
}

int
XdmfFortran::retrieveInformationNumProperties(int const index)
{
  if (index < (int)mInformations.size()) {
    return mInformations[index]->getItemProperties().size();
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
    return -1;
  }
}

void
XdmfFortran::retrieveInformationProperty(const int informationIndex,
                                         const int index,
                                         char * key,
                                         const int keyLength,
                                         char * value,
                                         const int valueLength)
{
  if (informationIndex < (int)mInformations.size()) {
    if (index < (int)mInformations[informationIndex]->getItemProperties().size()) {
      std::map<std::string, std::string>::iterator walker =
        mInformations[informationIndex]->getItemProperties().begin();
      for (int i = 0; i<index; i++) {
        walker++;
      }
      char * tempKey = strdup((*walker).first.c_str());
      memset(key, 0, keyLength);
      memcpy(key, tempKey, strlen(tempKey)+1);
      delete [] tempKey;
      char * tempValue = strdup((*walker).second.c_str());
      memset(value, 0, valueLength);
      memcpy(value, tempValue, strlen(tempValue)+1);
      delete [] tempValue;
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Property index out of range.");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

void
XdmfFortran::retrieveInformationPropertyByKey(const int index,
                                              char * key,
                                              char * value,
                                              const int valueLength)
{
  if (index < (int)mInformations.size()) {
    std::string tempString = key;
    memset(value, 0, valueLength);
    if ((mInformations[index]->getItemProperties().count(tempString))>0) {
      char * tempValue =
        strdup(mInformations[index]->getItemProperties()[tempString].c_str());
      memcpy(value, tempValue, strlen(tempValue)+1);
      delete [] tempValue;
    }
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

void
XdmfFortran::addInformationArray(const int index,
                                 void * values,
                                 const int numValues,
                                 const int arrayType)
{
  if (index < (int)mInformations.size()) {
    shared_ptr<XdmfArray> newArray = XdmfArray::New();
    writeToArray(newArray, numValues, arrayType, values);
    mInformations[index]->insert(newArray);
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

void
XdmfFortran::insertInformationIntoInformation(const int toIndex,
                                              const int fromIndex,
                                              const bool removeFromArray)
{
  if (toIndex < (int)mInformations.size() && fromIndex < (int)mInformations.size()) {
    if (toIndex != fromIndex) {
      mInformations[toIndex]->insert(mInformations[fromIndex]);
      if (removeFromArray) {
        mInformations.erase(mInformations.begin()+fromIndex);
      }
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Information may not be inserted into itself."
                           " Doing so would cause an infinte loop when writing.");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

void
XdmfFortran::modifyInformationArray(const int index,
                                    const int arrayIndex,
                                    void * values,
                                    const int arrayType,
                                    const int numValues,
                                    const int insertStart,
                                    const int arrayStride,
                                    const int valueStride)
{
  if (index < (int)mInformations.size()) {
    if (arrayIndex < (int)mInformations[index]->getNumberArrays()) {
      shared_ptr<XdmfArray> modifiedArray =
        mInformations[index]->getArray(arrayIndex);
      if (!modifiedArray->isInitialized()) {
        modifiedArray->read();
      }
      writeToArray(modifiedArray,
                   numValues,
                   arrayType,
                   values,
                   insertStart,
                   arrayStride,
                   valueStride);
    }
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

void
XdmfFortran::removeInformationArray(const int index,
                                    const int arrayIndex)
{
  if (index < (int)mInformations.size()) {
    if (arrayIndex < (int)mInformations[index]->getNumberArrays()) {
      mInformations[index]->removeArray(arrayIndex);
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Array index out of range.");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

int
XdmfFortran::retrieveInformationArraySize(const int index,
                                          const int arrayIndex)
{
  if (index < (int)mInformations.size()) {
    if (arrayIndex < (int)mInformations[index]->getNumberArrays()) {
      return mInformations[index]->getArray(arrayIndex)->getSize();
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Array index out of range.");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
  return -1;
}

int
XdmfFortran::retrieveInformationArrayValueType(const int index,
                                               const int arrayIndex)
{
  if (index < (int)mInformations.size()) {
    if (arrayIndex < (int)mInformations[index]->getNumberArrays()) {
      shared_ptr<const XdmfArrayType> dataType = mInformations[index]->getArray(arrayIndex)->getArrayType();
      if (dataType == XdmfArrayType::Int8()) {
        return XDMF_ARRAY_TYPE_INT8;
      }
      else if (dataType == XdmfArrayType::Int16()) {
        return XDMF_ARRAY_TYPE_INT16;
      }
      else if (dataType == XdmfArrayType::Int32()) {
        return XDMF_ARRAY_TYPE_INT32;
      }
      else if (dataType == XdmfArrayType::Int64()) {
        return XDMF_ARRAY_TYPE_INT64;
      }
      else if (dataType == XdmfArrayType::UInt8()) {
        return XDMF_ARRAY_TYPE_UINT8;
      }
      else if (dataType == XdmfArrayType::UInt16()) {
        return XDMF_ARRAY_TYPE_UINT16;
      }
      else if (dataType == XdmfArrayType::UInt32()) {
        return XDMF_ARRAY_TYPE_UINT32;
      }
      else if (dataType == XdmfArrayType::Float32()) {
        return XDMF_ARRAY_TYPE_FLOAT32;
      }
      else if (dataType == XdmfArrayType::Float64()) {
        return XDMF_ARRAY_TYPE_FLOAT64;
      }
      else if (dataType == XdmfArrayType::Uninitialized()) {
        try {
          XdmfError::message(XdmfError::FATAL,
                             "Uninitialized Set Data Type.");
        }
        catch (XdmfError e) {
          throw e;
        }
        return -1;
      }
      else {
        try {
          XdmfError::message(XdmfError::FATAL,
                             "Invalid Set Data Type.");
        }
        catch (XdmfError e) {
          throw e;
        }
        return -1;
      }
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Array index out of range.");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
  return -1;
}

void
XdmfFortran::retrieveInformationArrayValues(const int index,
                                            const int arrayIndex,
                                            void * values,
                                            const int dataType,
                                            const int numberRead,
                                            const int startIndex,
                                            const int arrayStride,
                                            const int valueStride)
{
  if (index < (int)mInformations.size()) {
    if (arrayIndex < (int)mInformations[index]->getNumberArrays()) {
      if (!mInformations[index]->getArray(arrayIndex)->isInitialized()) {
        mInformations[index]->getArray(arrayIndex)->read();
      }
      readFromArray(mInformations[index]->getArray(arrayIndex),
                    dataType,
                    values,
                    numberRead,
                    startIndex,
                    arrayStride,
                    valueStride);
    }
    else {
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Array index out of range.");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Index out of range.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

void
XdmfFortran::clearPrevious()
{
  mPreviousTopologies.clear();
  mPreviousGeometries.clear();
  mPreviousAttributes.clear();
  mPreviousMaps.clear();
  mPreviousSets.clear();
  mPreviousDimensions.clear();
  mPreviousOrigins.clear();
  mPreviousBricks.clear();
  mPreviousCoordinates.clear();
}

void
XdmfFortran::setAllowSetSplitting(bool newAllow)
{
  mAllowSetSplitting = newAllow;
}

void
XdmfFortran::setMaxFileSize(int newSize)
{
  mMaxFileSize = newSize;
}

void 
XdmfFortran::write(const char * const xmlFilePath,
                   const int datalimit,
                   const bool release)
{
  shared_ptr<XdmfWriter> writer;
  if (mHeavyDataWriter == NULL) {
    writer = XdmfWriter::New(xmlFilePath);
    mHeavyDataWriter = writer->getHeavyDataWriter();
  }
  else {
    writer = XdmfWriter::New(xmlFilePath, mHeavyDataWriter);
  }
  writer->setLightDataLimit(datalimit);
  writer->getHeavyDataWriter()->setReleaseData(release);
  shared_dynamic_cast<XdmfHDF5Writer>(writer->getHeavyDataWriter())->setFileSizeLimit(mMaxFileSize);
  shared_dynamic_cast<XdmfHDF5Writer>(writer->getHeavyDataWriter())->setAllowSetSplitting(mAllowSetSplitting);
  mDomain->accept(writer);
}

void 
XdmfFortran::writeHDF5(const char * const xmlFilePath,
                       const bool release)
{
  if (mHeavyDataWriter == NULL || mHeavyDataWriter->getFilePath() != xmlFilePath) {
    shared_ptr<XdmfHDF5Writer> writer = XdmfHDF5Writer::New(xmlFilePath);
    writer->setFileSizeLimit(mMaxFileSize);
    writer->setAllowSetSplitting(mAllowSetSplitting);
    writer->setReleaseData( release );
    mHeavyDataWriter = writer;
  }
  if (mHeavyDataWriter->getReleaseData() != release) {
    mHeavyDataWriter->setReleaseData(release);
  }
  mHeavyDataWriter->openFile();
  mDomain->accept(mHeavyDataWriter);
  mHeavyDataWriter->closeFile();
}

void
XdmfFortran::initHDF5(const char * const xmlFilePath,
                      const bool release)
{
  shared_ptr<XdmfHDF5Writer> writer = XdmfHDF5Writer::New(xmlFilePath);
  writer->setFileSizeLimit(mMaxFileSize);
  writer->setAllowSetSplitting(mAllowSetSplitting);
  writer->setReleaseData(release);
  mHeavyDataWriter = writer;
}

#ifdef XDMF_BUILD_DSM

void
XdmfFortran::initDSMServer(const char * const filePath,
                           MPI_Comm comm,
                           int bufferSize,
                           int startCoreIndex,
                           int endCoreIndex)
{
  // Non-Threaded version
  std::string writtenFile(filePath);
  if (bufferSize > 0) {
    mDSMWriter = XdmfHDF5WriterDSM::New(writtenFile, comm, (unsigned int) bufferSize, startCoreIndex, endCoreIndex);
    mDSMWriter->setMode(XdmfHeavyDataWriter::Hyperslab);
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Non-positive DSM buffer size.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

void
XdmfFortran::acceptDSM(int numConnections)
{
  if (mDSMWriter) {
    mDSMWriter->getServerBuffer()->GetComm()->OpenPort();
    mDSMWriter->getServerBuffer()->SendAccept(numConnections);
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Attempting to accept connection when DSM is not set up.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

void
XdmfFortran::closeDSMPort()
{
  if (mDSMWriter) {
    mDSMWriter->getServerBuffer()->GetComm()->ClosePort();
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Attempting to close a port when DSM is not set up.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

void
XdmfFortran::connectDSM(const char * const filePath,
                        MPI_Comm comm)
{
  std::string writtenFile(filePath);
  XdmfDSMCommMPI * dsmComm = new XdmfDSMCommMPI();
  dsmComm->DupComm(comm);
  dsmComm->Init();
  XdmfDSMBuffer * dsmBuffer = new XdmfDSMBuffer();
  dsmBuffer->SetIsServer(false);
  dsmBuffer->SetComm(dsmComm);
  dsmBuffer->SetIsConnected(true);

  mDSMWriter = XdmfHDF5WriterDSM::New(writtenFile, dsmBuffer);

  mDSMWriter->setMode(XdmfHeavyDataWriter::Hyperslab);

  // Currently uses default config file name
  mDSMWriter->getServerBuffer()->GetComm()->ReadDsmPortName();

  mDSMWriter->getServerManager()->Connect();
  // To check if the DSM writer is using server mode
  // bool test = mDSMWriter->getServerMode();
}

MPI_Comm
XdmfFortran::getDSMInterComm()
{
  // Sanity check
  if (mDSMWriter) {
    return mDSMWriter->getServerBuffer()->GetComm()->GetInterComm();
  }
  else {
    return MPI_COMM_NULL;
  }
}

MPI_Comm
XdmfFortran::getDSMIntraComm()
{
  // Sanity check
  if (mDSMWriter) {
    return mDSMWriter->getServerBuffer()->GetComm()->GetIntraComm();
  }
  else {
    return MPI_COMM_NULL;
  }
}

// Call only on one core
void
XdmfFortran::stopDSM()
{
  if (mDSMWriter) {
    mDSMWriter->stopDSM();
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Stop called when DSM not initialized.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
  mDSMWriter =  shared_ptr<XdmfHDF5WriterDSM>();
}

void
XdmfFortran::readFromDSM(const char * const dsmDataSetPath,
                         const int arrayType,
                         void * values,
                         const int start,
                         const int stride,
                         const int dimensions,
                         const int dataspace)
{
  if (mDSMWriter) {
    shared_ptr<const XdmfArrayType> writtenArrayType = shared_ptr<const XdmfArrayType>();
    switch(arrayType) {
    case XDMF_ARRAY_TYPE_INT8:
      writtenArrayType = XdmfArrayType::Int8();
      break;
    case XDMF_ARRAY_TYPE_INT16:
      writtenArrayType = XdmfArrayType::Int16();
      break;
    case XDMF_ARRAY_TYPE_INT32:
      writtenArrayType = XdmfArrayType::Int32();
      break;
    case XDMF_ARRAY_TYPE_INT64:
      writtenArrayType = XdmfArrayType::Int64();
      break;
    case XDMF_ARRAY_TYPE_UINT8:
      writtenArrayType = XdmfArrayType::UInt8();
      break;
    case XDMF_ARRAY_TYPE_UINT16:
      writtenArrayType = XdmfArrayType::UInt16();
      break;
    case XDMF_ARRAY_TYPE_UINT32:
      writtenArrayType = XdmfArrayType::UInt32();
      break;
    case XDMF_ARRAY_TYPE_FLOAT32:
      writtenArrayType = XdmfArrayType::Float32();
      break;
    case XDMF_ARRAY_TYPE_FLOAT64:
      writtenArrayType = XdmfArrayType::Float64();
      break;
    default:
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Invalid array number type");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
    std::vector<unsigned int> startVector;
    startVector.push_back(start);
    std::vector<unsigned int> strideVector;
    strideVector.push_back(stride);
    std::vector<unsigned int> dimVector;
    dimVector.push_back(dimensions);
    std::vector<unsigned int> dataVector;
    dataVector.push_back(dataspace);
    std::string writtenPath(dsmDataSetPath);
    shared_ptr<XdmfHDF5ControllerDSM> writerController =
      XdmfHDF5ControllerDSM::New(mDSMWriter->getFilePath(),
                                 writtenPath,
                                 writtenArrayType,
                                 startVector,
                                 strideVector,
                                 dimVector,
                                 dataVector,
                                 mDSMWriter->getServerBuffer());

    shared_ptr<XdmfArray> readArray = XdmfArray::New();

    readArray->insert(writerController);
    readArray->read();

    readFromArray(readArray,
                  arrayType,
                  values,
                  dimensions,
                  0,
                  1,
                  1);
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Attempting to read from DSM when DSM is not set up.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

void
XdmfFortran::writeToDSM(const char * const dsmDataSetPath,
                        const int arrayType,
                        void * values,
                        const int start,
                        const int stride,
                        const int dimensions,
                        const int dataspace)
{
  if (mDSMWriter) {
    shared_ptr<const XdmfArrayType> writtenArrayType = shared_ptr<const XdmfArrayType>();
    switch(arrayType) {
    case XDMF_ARRAY_TYPE_INT8:
      writtenArrayType = XdmfArrayType::Int8();
      break;
    case XDMF_ARRAY_TYPE_INT16:
      writtenArrayType = XdmfArrayType::Int16();
      break;
    case XDMF_ARRAY_TYPE_INT32:
      writtenArrayType = XdmfArrayType::Int32();
      break;
    case XDMF_ARRAY_TYPE_INT64:
      writtenArrayType = XdmfArrayType::Int64();
      break;
    case XDMF_ARRAY_TYPE_UINT8:
      writtenArrayType = XdmfArrayType::UInt8();
      break;
    case XDMF_ARRAY_TYPE_UINT16:
      writtenArrayType = XdmfArrayType::UInt16();
      break;
    case XDMF_ARRAY_TYPE_UINT32:
      writtenArrayType = XdmfArrayType::UInt32();
      break;
    case XDMF_ARRAY_TYPE_FLOAT32:
      writtenArrayType = XdmfArrayType::Float32();
      break;
    case XDMF_ARRAY_TYPE_FLOAT64:
      writtenArrayType = XdmfArrayType::Float64();
      break;
    default:
      try {
        XdmfError::message(XdmfError::FATAL,
                           "Invalid array number type");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
    std::vector<unsigned int> startVector;
    startVector.push_back(start);
    std::vector<unsigned int> strideVector;
    strideVector.push_back(stride);
    std::vector<unsigned int> dimVector;
    dimVector.push_back(dimensions);
    std::vector<unsigned int> dataVector;
    dataVector.push_back(dataspace);
    std::string writtenPath(dsmDataSetPath);
    shared_ptr<XdmfHDF5ControllerDSM> writerController =
      XdmfHDF5ControllerDSM::New(mDSMWriter->getFilePath(),
                                 writtenPath,
                                 writtenArrayType,
                                 startVector,
                                 strideVector,
                                 dimVector,
                                 dataVector,
                                 mDSMWriter->getServerBuffer());

    shared_ptr<XdmfArray> writtenArray = XdmfArray::New();
    writeToArray(writtenArray,
                 dimensions,
                 arrayType,
                 values);
    writtenArray->insert(writerController);
    writtenArray->accept(mDSMWriter);
  }
  else {
    try {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Attempting to write to DSM when DSM is not set up.");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

#endif

void
XdmfFortran::read(const char * const xmlFilePath)
{
  shared_ptr<XdmfReader> reader = XdmfReader::New();
  mDomain = shared_dynamic_cast<XdmfDomain>(reader->read( xmlFilePath )); 
}

//temporary fix, hopefully
int
XdmfFortran::setTopologyPolyline(const unsigned int nodesPerElement,
                                 const unsigned int numValues,
                                 const int arrayType,
                                 const void * const connectivityValues)
{
  mTopology = XdmfTopology::New();
  mTopology->setType(XdmfTopologyType::Polyline(nodesPerElement));
  // insert connectivity values into array
  writeToArray(mTopology,
               numValues,
               arrayType,
               connectivityValues);
  const int id = mPreviousTopologies.size();
  mPreviousTopologies.push_back(mTopology);
  return id;
}

int
XdmfFortran::setTopologyPolygon(const unsigned int nodesPerElement,
                                const unsigned int numValues,
                                const int arrayType,
                                const void * const connectivityValues)
{
  mTopology = XdmfTopology::New();
  mTopology->setType(XdmfTopologyType::Polygon(nodesPerElement));
  // insert connectivity values into array
  writeToArray(mTopology,
               numValues,
               arrayType,
               connectivityValues);
  const int id = mPreviousTopologies.size();
  mPreviousTopologies.push_back(mTopology);
  return id;
}

//
// C++ will mangle the name based on the argument list. This tells the
// compiler not to mangle the name so we can call it from 'C' (but
// really Fortran in this case)
//
extern "C"
{
  void
  XdmfInit(long * pointer)
  {
    XdmfFortran * xdmfFortran = new XdmfFortran();
    *pointer = reinterpret_cast<long>(xdmfFortran);
  }

  void
  XdmfClose(long * pointer)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    delete xdmfFortran;
  }

  int
  XdmfAddAttribute(long * pointer, 
                   char * const name,
                   int  * attributeCenter,
                   int  * attributeType,
                   int  * numValues,
                   int  * arrayType,
                   void * values)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    return xdmfFortran->addAttribute(name,
                                     *attributeCenter,
                                     *attributeType,
                                     *numValues,
                                     *arrayType, 
                                     values);
  }

  void
  XdmfAddGrid(long * pointer, 
              char * gridName,
              bool * writeToHDF5)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->addGrid(gridName, *writeToHDF5);
  }

  void
  XdmfAddGridCurvilinear(long * pointer,
                         char * gridName,
                         bool * writeToHDF5)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->addGridCurvilinear(gridName, *writeToHDF5);
  }

  void
  XdmfAddGridRectilinear(long * pointer,
                         char * gridName,
                         bool * writeToHDF5)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->addGridRectilinear(gridName, *writeToHDF5);
  }

  void
  XdmfAddGridRegular(long * pointer,
                     char * gridName,
                     bool * writeToHDF5)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->addGridRegular(gridName, *writeToHDF5);
  }

  void
  XdmfAddGridCollection(long * pointer, 
                        char * name,
                        int  * gridCollectionType)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->addGridCollection(name,
                                   *gridCollectionType);
  }

  int
  XdmfAddInformation(long * pointer, 
                     char * key, 
                     char * value)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    return xdmfFortran->addInformation(key, value);
  }

  void
  XdmfAddPreviousAttribute(long * pointer, 
                           int  * attributeId)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->addPreviousAttribute(*attributeId);
  }

  void
  XdmfAddPreviousInformation(long * pointer, 
                             int  * informationId)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->addPreviousInformation(*informationId);
  }

  void
  XdmfCloseGridCollection(long * pointer, bool * writeToHDF5)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->closeGridCollection(*writeToHDF5);
  }

  int
  XdmfSetGeometry(long * pointer, 
                  int  * geometryType, 
                  int  * numValues,
                  int  * arrayType, 
                  void * pointValues)
  {

    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    return xdmfFortran->setGeometry(*geometryType, 
                                    *numValues,
                                    *arrayType, 
                                    pointValues);
  }

  void
  XdmfSetPreviousGeometry(long * pointer, 
                          int  * geometryId)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->setPreviousGeometry(*geometryId);
  }

  void
  XdmfSetPreviousTopology(long * pointer, 
                          int  * topologyId)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->setPreviousTopology(*topologyId);
  }

  void
  XdmfSetTime(long   * pointer, 
              double * time)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->setTime(*time);
  }

  int
  XdmfSetTopology(long * pointer, 
                  int  * topologyType, 
                  int  * numValues,
                  int  * arrayType,
                  void * connectivityValues)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    return xdmfFortran->setTopology(*topologyType, 
                                    *numValues, 
                                    *arrayType,
                                    connectivityValues,
                                    0);
  }

  void
  XdmfRetrieveNumDomainGridCollections(long * pointer, int * total)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    *total =  xdmfFortran->retrieveNumDomainGridCollections();
  }

  void
  XdmfRetrieveNumGridCollectionGridCollections(long * pointer, int * total)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    *total =  xdmfFortran->numGridCollectionGridCollections();
  }

  void
  XdmfRetrieveDomainTag(long * pointer, char * tag, int * tagLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveDomainTag(tag, *tagLength);
  }

  void
  XdmfRetrieveDomainNumProperties(long * pointer, int * total)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    *total = xdmfFortran->retrieveDomainNumProperties();
  }

  void
  XdmfRetrieveDomainProperty(long * pointer,
                             int * index,
                             char * key,
                             int * keyLength,
                             char * value,
                             int * valueLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveDomainProperty(*index,
                                        key,
                                        *keyLength,
                                        value,
                                        *valueLength);
  }

  void
  XdmfRetrieveDomainPropertyByKey(long * pointer,
                                  char * key,
                                  char * value,
                                  int * valueLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveDomainPropertyByKey(key, value, *valueLength);
  }

  void
  XdmfOpenDomainGridCollection(long * pointer,
                               int * index,
                               int * openMaps,
                               int * openAttributes,
                               int * openInformation,
                               int * openSets)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->openDomainGridCollection(*index,
                                          *openMaps,
                                          *openAttributes,
                                          *openInformation,
                                          *openSets);
  }

  void
  XdmfRemoveDomainGridCollection(long * pointer, int * index)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->removeDomainGridCollection(*index);
  }

  void
  XdmfOpenGridCollectionGridCollection(long * pointer,
                                       int * index,
                                       int * openMaps,
                                       int * openAttributes,
                                       int * openInformation,
                                       int * openSets)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->openGridCollectionGridCollection(*index,
                                                  *openMaps,
                                                  *openAttributes,
                                                  *openInformation,
                                                  *openSets);
  }

  void
  XdmfRemoveGridCollectionGridCollection(long * pointer, int * index)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->removeGridCollectionGridCollection(*index);
  }

  void
  XdmfRetrieveGridCollectionTag(long * pointer, char * tag, int * tagLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveGridCollectionTag(tag, *tagLength);
  }

  void
  XdmfRetrieveGridCollectionNumProperties(long * pointer, int * total)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    *total = xdmfFortran->retrieveGridCollectionNumProperties();
  }

  void
  XdmfRetrieveGridCollectionProperty(long * pointer,
                                     int * index,
                                     char * key,
                                     int * keyLength,
                                     char * value,
                                     int * valueLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveGridCollectionProperty(*index,
                                                key,
                                                *keyLength,
                                                value,
                                                *valueLength);
  }

  void
  XdmfRetrieveGridCollectionPropertyByKey(long * pointer,
                                          char * key,
                                          char * value,
                                          int * valueLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveGridCollectionPropertyByKey(key, value, *valueLength);
  }

  void
  XdmfRetrieveGridCollectionNumGrids(long * pointer, int * gridType, int * total)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    *total = xdmfFortran->numGridCollectionGrids(*gridType);
  }

  void
  XdmfRetrieveDomainNumGrids(long * pointer, int * gridType, int * total)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    *total = xdmfFortran->numDomainGrids(*gridType);
  }
  
  void
  XdmfOpenDomainGrid(long * pointer,
                     int * gridType,
                     int * index,
                     int * openMaps,
                     int * openAttributes,
                     int * openInformation,
                     int * openSets)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->openDomainGrid(*gridType,
                                *index,
                                *openMaps,
                                *openAttributes,
                                *openInformation,
                                *openSets);
  }

  void
  XdmfRemoveDomainGrid(long * pointer, int * gridType, int * index)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->removeDomainGrid(*gridType, *index);
  }

  void
  XdmfReplaceDomainGrid(long * pointer, int * gridType, int * index, char * name)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->replaceDomainGrid(*gridType, *index, name);
  }

  void
  XdmfRetrieveDomainGridTag(long * pointer,
                            int * gridType,
                            int * index,
                            char * tag,
                            int * tagLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveDomainGridTag(*gridType, *index, tag, *tagLength);
  }

  void
  XdmfRetrieveDomainGridName(long * pointer,
                             int * gridType,
                             int * index,
                             char * name,
                             int * nameLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveDomainGridName(*gridType, *index, name, *nameLength);
  }

  void
  XdmfRetrieveDomainGridNumProperties(long * pointer,
                                      int * gridType,
                                      int * index,
                                      int * total)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    *total = xdmfFortran->retrieveDomainGridNumProperties(*gridType, *index);
  }

  void
  XdmfRetrieveDomainGridProperty(long * pointer,
                                 int * gridType,
                                 int * gridIndex,
                                 int * index,
                                 char * key,
                                 int * keyLength,
                                 char * value,
                                 int * valueLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveDomainGridProperty(*gridType,
                                            *gridIndex,
                                            *index,
                                            key,
                                            *keyLength,
                                            value,
                                            *valueLength);
  }

  void
  XdmfRetrieveDomainGridPropertyByKey(long * pointer,
                                      int * gridType,
                                      int * index,
                                      char * key,
                                      char * value,
                                      int * valueLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveDomainGridPropertyByKey(*gridType, *index, key, value, *valueLength);
  }

  void
  XdmfOpenGridCollectionGrid(long * pointer,
                             int * gridType,
                             int * index,
                             int * openMaps,
                             int * openAttributes,
                             int * openInformation,
                             int * openSets)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->openGridCollectionGrid(*gridType,
                                        *index,
                                        *openMaps,
                                        *openAttributes,
                                        *openInformation,
                                        *openSets);
  }

  void
  XdmfRemoveGridCollectionGrid(long * pointer, int * gridType, int * index)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->removeGridCollectionGrid(*gridType, *index);
  }

  void
  XdmfReplaceGridCollectionGrid(long * pointer, int * gridType, int * index, char * name)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->replaceGridCollectionGrid(*gridType, *index, name);
  }

  void
  XdmfRetrieveGridCollectionGridTag(long * pointer,
                                    int * gridType,
                                    int * index,
                                    char * tag,
                                    int * tagLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveGridCollectionGridTag(*gridType, *index, tag, *tagLength);
  }

  void
  XdmfRetrieveGridCollectionGridName(long * pointer,
                                     int * gridType,
                                     int * index,
                                     char * name,
                                     int * nameLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveGridCollectionGridName(*gridType, *index, name, *nameLength);
  }

  void
  XdmfRetrieveGridCollectionGridNumProperties(long * pointer,
                                              int * gridType,
                                              int * index,
                                              int * total)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    *total = xdmfFortran->retrieveGridCollectionGridNumProperties(*gridType, *index);
  }

  void
  XdmfRetrieveGridCollectionGridProperty(long * pointer,
                                         int * gridType,
                                         int * gridIndex,
                                         int * index,
                                         char * key,
                                         int * keyLength,
                                         char * value,
                                         int * valueLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveGridCollectionGridProperty(*gridType,
                                                    *gridIndex,
                                                    *index,
                                                    key,
                                                    *keyLength,
                                                    value,
                                                    *valueLength);
  }

  void
  XdmfRetrieveGridCollectionGridPropertyByKey(long * pointer,
                                              int * gridType,
                                              int *index,
                                              char * key,
                                              char * value,
                                              int * valueLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveGridCollectionGridPropertyByKey(*gridType,
                                                         *index,
                                                         key,
                                                         value,
                                                         *valueLength);
  }

  void
  XdmfRetrieveGridCollectionType(long * pointer, int * returnType)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    *returnType = xdmfFortran->retrieveGridCollectionType();
  }

  void
  XdmfRetrieveTime(long * pointer, double * total)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    *total =  xdmfFortran->retrieveTime();
  }

  void
  XdmfRetrieveTopologyTag(long * pointer, char * tag, int * tagLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveTopologyTag(tag, *tagLength);
  }

  void
  XdmfRetrieveTopologyValues(long * pointer,
                             void * values,
                             int * dataType,
                             int * numberRead,
                             int * startIndex,
                             int * arrayStride,
                             int * valueStride)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveTopologyValues(values,
                                        *dataType,
                                        *numberRead,
                                        *startIndex,
                                        *arrayStride,
                                        *valueStride);
  }

  void
  XdmfRetrieveTopologyType(long * pointer, int * returnType)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    *returnType =  xdmfFortran->retrieveTopologyType();
  }

  void
  XdmfRetrieveTopologyValueType(long * pointer, int * returnType)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    *returnType =  xdmfFortran->retrieveTopologyValueType();
  }

  void
  XdmfRetrieveTopologyNumElements(long * pointer, int * total)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    *total = xdmfFortran->retrieveTopologyNumElements();
  }

  void
  XdmfRetrieveTopologySize(long * pointer, int * total)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    *total = xdmfFortran->retrieveTopologySize();
  }

  void
  XdmfClearPreviousTopologies(long * pointer)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->clearPreviousTopologies();
  }

  void
  XdmfModifyTopologyValues(long * pointer,
                           void * values,
                           int * arrayType,
                           int * numValues,
                           int * startIndex,
                           int * arrayStride,
                           int * valueStride)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->modifyTopologyValues(values,
                                      *arrayType,
                                      *numValues,
                                      *startIndex,
                                      *arrayStride,
                                      *valueStride);
  }

  void  
  XdmfRetrieveTopologyNumProperties(long * pointer, int * total)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    *total = xdmfFortran->retrieveTopologyNumProperties();
  }

  void
  XdmfRetrieveTopologyProperty(long * pointer,
                               int * index,
                               char * key,
                               int * keyLength,
                               char * value,
                               int * valueLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveTopologyProperty(*index, key, *keyLength, value, *valueLength);
  }

  void
  XdmfRetrieveTopologyPropertyByKey(long * pointer,
                                    char * key,
                                    char * value,
                                    int * valueLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveTopologyPropertyByKey(key, value, *valueLength);
  }

  void
  XdmfRetrieveGeometryTag(long * pointer, char * tag, int * tagLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveGeometryTag(tag, *tagLength);
  }

  void
  XdmfRetrieveGeometryValues(long * pointer,
                             void * values,
                             int * dataType,
                             int * numberRead,
                             int * startIndex,
                             int * arrayStride,
                             int * valueStride)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveGeometryValues(values,
                                        *dataType,
                                        *numberRead,
                                        *startIndex,
                                        *arrayStride,
                                        *valueStride);
  }

  void
  XdmfRetrieveGeometryType(long * pointer, int * returnType)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    *returnType = xdmfFortran->retrieveGeometryType();
  }

  void
  XdmfRetrieveGeometryValueType(long * pointer, int * returnType)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    *returnType = xdmfFortran->retrieveGeometryValueType();
  }

  void
  XdmfRetrieveGeometryNumPoints(long * pointer, int * total)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    *total = xdmfFortran->retrieveGeometryNumPoints();
  }

  void
  XdmfRetrieveGeometrySize(long * pointer, int * total)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    *total = xdmfFortran->retrieveGeometrySize();
  }

  void
  XdmfClearPreviousGeometries(long * pointer)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->clearPreviousGeometries();
  }

  void
  XdmfModifyGeometryValues(long * pointer,
                           void * values,
                           int * arrayType,
                           int * numValues,
                           int * startIndex,
                           int * arrayStride,
                           int * valueStride)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->modifyGeometryValues(values,
                                      *arrayType,
                                      *numValues,
                                      *startIndex,
                                      *arrayStride,
                                      *valueStride);
  }

  void  
  XdmfRetrieveGeometryNumProperties(long * pointer, int * total)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    *total = xdmfFortran->retrieveGeometryNumProperties();
  }

  void
  XdmfRetrieveGeometryProperty(long * pointer,
                               int * index,
                               char * key,
                               int * keyLength,
                               char * value,
                               int * valueLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveGeometryProperty(*index, key, *keyLength, value, *valueLength);
  }

  void
  XdmfRetrieveGeometryPropertyByKey(long * pointer,
                                    char * key,
                                    char * value,
                                    int * valueLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveGeometryPropertyByKey(key, value, *valueLength);
  }

  int
  XdmfSetDimensions(long * pointer, int  * numValues, int  * arrayType, void * pointValues)
  {

    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    return xdmfFortran->setDimensions(*numValues, *arrayType, pointValues);
  }

  void
  XdmfOpenPreviousDimensions(long * pointer, int * index)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->openPreviousDimensions(*index);  
  }

  void
  XdmfClearPreviousDimensions(long * pointer)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->clearPreviousDimensions();
  }

  void
  XdmfModifyDimensionsValues(long * pointer,
                             void * values,
                             int * arrayType,
                             int * numValues,
                             int * startIndex,
                             int * arrayStride,
                             int * valueStride)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->modifyDimensionsValues(values,
                                        *arrayType,
                                        *numValues,
                                        *startIndex,
                                        *arrayStride,
                                        *valueStride);
  }

  void
  XdmfRetrieveDimensionsTag(long * pointer, char * tag, int * tagLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveDimensionsTag(tag, *tagLength);
  }

  void
  XdmfRetrieveDimensionsValues(long * pointer,
                               void * values,
                               int * dataType,
                               int * numberRead,
                               int * startIndex,
                               int * arrayStride,
                               int * valueStride)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveDimensionsValues(values,
                                          *dataType,
                                          *numberRead,
                                          *startIndex,
                                          *arrayStride,
                                          *valueStride);
  }

  void
  XdmfRetrieveDimensionsValueType(long * pointer, int * returnType)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    *returnType =  xdmfFortran->retrieveDimensionsValueType();
  }

  void
  XdmfRetrieveDimensionsSize(long * pointer, int * total)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    *total = xdmfFortran->retrieveDimensionsSize();
  }

  void  
  XdmfRetrieveDimensionsNumProperties(long * pointer, int * total)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    *total = xdmfFortran->retrieveDimensionsNumProperties();
  }

  void
  XdmfRetrieveDimensionsProperty(long * pointer,
                                 int * index,
                                 char * key,
                                 int * keyLength,
                                 char * value,
                                 int * valueLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveDimensionsProperty(*index, key, *keyLength, value, *valueLength);
  }

  void
  XdmfRetrieveDimensionsPropertyByKey(long * pointer, char * key, char * value, int * valueLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveDimensionsPropertyByKey(key, value, *valueLength);
  }

  int
  XdmfSetOrigin(long * pointer, int  * numValues, int  * arrayType, void * pointValues)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    return xdmfFortran->setOrigin(*numValues, *arrayType, pointValues);
  }

  void
  XdmfSetPreviousOrigin(long * pointer, int * index)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->setPreviousOrigin(*index);
  }

  void
  XdmfClearPreviousOrigins(long * pointer)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->clearPreviousOrigins();
  }

  void
  XdmfModifyOriginValues(long * pointer,
                         void * values,
                         int * arrayType,
                         int * numValues,
                         int * startIndex,
                         int * arrayStride,
                         int * valueStride)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->modifyOriginValues(values,
                                    *arrayType,
                                    *numValues,
                                    *startIndex,
                                    *arrayStride,
                                    *valueStride);
  }

  void
  XdmfRetrieveOriginTag(long * pointer, char * tag, int * tagLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveOriginTag(tag, *tagLength);
  }

  void
  XdmfRetrieveOriginValues(long * pointer,
                           void * values,
                           int * dataType,
                           int * numberRead,
                           int * startIndex,
                           int * arrayStride,
                           int * valueStride)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveOriginValues(values,
                                      *dataType,
                                      *numberRead,
                                      *startIndex,
                                      *arrayStride,
                                      *valueStride);
  }

  void
  XdmfRetrieveOriginValueType(long * pointer, int * returnType)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    *returnType =  xdmfFortran->retrieveOriginValueType();
  }

  void
  XdmfRetrieveOriginSize(long * pointer, int * total)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    *total = xdmfFortran->retrieveOriginSize();
  }

  void  
  XdmfRetrieveOriginNumProperties(long * pointer, int * total)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    *total = xdmfFortran->retrieveOriginNumProperties();
  }

  void
  XdmfRetrieveOriginProperty(long * pointer,
                             int * index,
                             char * key,
                             int * keyLength,
                             char * value,
                             int * valueLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveOriginProperty(*index, key, *keyLength, value, *valueLength);
  }

  void
  XdmfRetrieveOriginPropertyByKey(long * pointer,
                                  char * key,
                                  char * value,
                                  int * valueLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveOriginPropertyByKey(key, value, *valueLength);
  }

  int
  XdmfSetBrick(long * pointer, int  * numValues, int  * arrayType, void * pointValues)
  {

    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    return xdmfFortran->setBrick(*numValues, *arrayType, pointValues);
  }

  void
  XdmfSetPreviousBrick(long * pointer, int * index)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->setPreviousBrick(*index);
  }

  void
  XdmfClearPreviousBricks(long * pointer)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->clearPreviousBricks();
  }

  void
  XdmfModifyBrickValues(long * pointer,
                        void * values,
                        int * arrayType,
                        int * numValues,
                        int * startIndex,
                        int * arrayStride,
                        int * valueStride)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->modifyBrickValues(values,
                                   *arrayType,
                                   *numValues,
                                   *startIndex,
                                   *arrayStride,
                                   *valueStride);
  }

  void
  XdmfRetrieveBrickTag(long * pointer, char * tag, int * tagLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveBrickTag(tag, *tagLength);
  }

  void
  XdmfRetrieveBrickValues(long * pointer,
                          void * values,
                          int * dataType,
                          int * numberRead,
                          int * startIndex,
                          int * arrayStride,
                          int * valueStride)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveBrickValues(values,
                                     *dataType,
                                     *numberRead,
                                     *startIndex,
                                     *arrayStride,
                                     *valueStride);
  }

  void
  XdmfRetrieveBrickValueType(long * pointer, int * returnType)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    *returnType =  xdmfFortran->retrieveBrickValueType();
  }

  void
  XdmfRetrieveBrickSize(long * pointer, int * total)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    *total = xdmfFortran->retrieveBrickSize();
  }

  void  
  XdmfRetrieveBrickNumProperties(long * pointer, int * total)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    *total = xdmfFortran->retrieveBrickNumProperties();
  }

  void
  XdmfRetrieveBrickProperty(long * pointer,
                            int * index,
                            char * key,
                            int * keyLength,
                            char * value,
                            int * valueLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveBrickProperty(*index, key, *keyLength, value, *valueLength);
  }

  void
  XdmfRetrieveBrickPropertyByKey(long * pointer, char * key, char * value, int * valueLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveBrickPropertyByKey(key, value, *valueLength);
  }

  void
  XdmfAddMap(long * pointer, char * name)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->addMap(name);
  }

  void
  XdmfRetrieveNumMaps(long * pointer, int * total)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    *total = xdmfFortran->retrieveNumMaps();
  }

  void
  XdmfRetrieveMapTag(long * pointer, int * index, char * tag, int * tagLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveMapTag(*index, tag, *tagLength);
  }

  void
  XdmfAddRemoteNodeID(long * pointer,
                      int * index,
                      int * localNodeID,
                      int * remoteTaskID,
                      int * remoteLocalNodeID)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->addRemoteNodeID(*index,
                                 *localNodeID,
                                 *remoteTaskID,
                                 *remoteLocalNodeID);
  }

  void
  XdmfRetrieveRemoteNodeIDs(long * pointer,
                            int * index,
                            int * localNodeID,
                            int * remoteTaskID,
                            int * remoteLocalNodeIDs)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveRemoteNodeIDs(*index,
                                       *localNodeID,
                                       *remoteTaskID,
                                       remoteLocalNodeIDs);
  }

  void
  XdmfRetrieveNumRemoteNodeIDs(long * pointer,
                               int * index,
                               int * localNodeID,
                               int * remoteTaskID,
                               int * total)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    *total = xdmfFortran->retrieveNumRemoteNodeIDs(*index, *localNodeID, *remoteTaskID);
  }

  void
  XdmfClearMaps(long * pointer)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->clearMaps();
  }

  void
  XdmfRemoveMap(long * pointer, int * index)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->removeMap(*index);
  }

  int
  XdmfStoreMap(long * pointer, int * index)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    return xdmfFortran->storeMap(*index);
  }

  void
  XdmfAddPreviousMap(long * pointer, int * index)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->addPreviousMap(*index);
  }

  void
  XdmfClearPreviousMaps(long * pointer)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->clearPreviousMaps();
  }

  void  
  XdmfRetrieveMapNumProperties(long * pointer, int * index, int * total)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    *total = xdmfFortran->retrieveMapNumProperties(*index);
  }

  void
  XdmfRetrieveMapProperty(long * pointer,
                          int * mapIndex,
                          int * index,
                          char * key,
                          int * keyLength,
                          char * value,
                          int * valueLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveMapProperty(*mapIndex,
                                     *index,
                                     key,
                                     *keyLength,
                                     value,
                                     *valueLength);
  }

  void
  XdmfRetrieveMapPropertyByKey(long * pointer,
                               int * index,
                               char * key,
                               char * value,
                               int * valueLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveMapPropertyByKey(*index, key, value, *valueLength);
  }

  void
  XdmfClearAttributes(long * pointer)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->clearAttributes();
  }

  void
  XdmfModifyAttributeValues(long * pointer,
                            int * index,
                            void * values,
                            int * arrayType,
                            int * numValues,
                            int * startIndex,
                            int * arrayStride,
                            int * valueStride)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->modifyAttributeValues(*index,
                                       values,
                                       *arrayType,
                                       *numValues,
                                       *startIndex,
                                       *arrayStride,
                                       *valueStride);
  }

  void
  XdmfRetrieveNumAttributes(long * pointer, int * total)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    *total = xdmfFortran->retrieveNumAttributes();
  }

  void
  XdmfReplaceAttribute(long * pointer,
                       int * index,
                       char * name,
                       int * attributeCenter,
                       int * attributeType,
                       int * numValues,
                       int * arrayType,
                       void * values)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->replaceAttribute(*index,
                                  name,
                                  *attributeCenter,
                                  *attributeType,
                                  *numValues,
                                  *arrayType,
                                  values);
  }

  void
  XdmfRemoveAttribute(long * pointer, int * index)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->removeAttribute(*index);
  }

  void
  XdmfOpenAttribute(long * pointer, int * index)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->openAttribute(*index);
  }

  void
  XdmfRetrieveAttributeTag(long * pointer, int * index, char * tag, int * tagLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveAttributeTag(*index, tag, *tagLength);
  }

  void
  XdmfRetrieveAttributeName(long * pointer, int * index, char * name, int * nameLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveAttributeName(*index, name, *nameLength);
  }

  void
  XdmfRetrieveAttributeValues(long * pointer,
                              int * index,
                              void * values,
                              int * dataType,
                              int * numberRead,
                              int * startIndex,
                              int * arrayStride,
                              int * valueStride)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveAttributeValues(*index,
                                         values,
                                         *dataType,
                                         *numberRead,
                                         *startIndex,
                                         *arrayStride,
                                         *valueStride);
  }

  void
  XdmfRetrieveAttributeType(long * pointer, int * index, int * returnType)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    *returnType = xdmfFortran->retrieveAttributeType(*index);
  }

  void
  XdmfRetrieveAttributeCenter(long * pointer, int * index, int * returnCenter)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    *returnCenter = xdmfFortran->retrieveAttributeCenter(*index);
  }

  void
  XdmfRetrieveAttributeValueType(long * pointer, int * index, int * returnType)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    *returnType = xdmfFortran->retrieveAttributeValueType(*index);
  }

  void
  XdmfRetrieveAttributeSize(long * pointer, int * index, int * total)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    *total = xdmfFortran->retrieveAttributeSize(*index);
  }

  void
  XdmfClearPreviousAttributes(long * pointer)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->clearPreviousAttributes();
  }

  void
  XdmfRetrieveAttributeNumProperties(long * pointer, int * index, int * total)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    *total = xdmfFortran->retrieveAttributeNumProperties(*index);
  }

  void
  XdmfRetrieveAttributeProperty(long * pointer,
                                int * attributeIndex,
                                int * index,
                                char * key,
                                int * keyLength,
                                char * value,
                                int * valueLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveAttributeProperty(*attributeIndex,
                                           *index,
                                           key,
                                           *keyLength,
                                           value,
                                           *valueLength);
  }

  void
  XdmfRetrieveAttributePropertyByKey(long * pointer,
                                     int * index,
                                     char * key,
                                     char * value,
                                     int * valueLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveAttributePropertyByKey(*index, key, value, *valueLength);
  }

  void
  XdmfRetrieveNumCoordinates(long * pointer, int * total)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    *total = xdmfFortran->retrieveNumCoordinates();
  }

  int
  XdmfAddCoordinate(long * pointer, char * name, int  * numValues, int  * arrayType, void * values)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    return xdmfFortran->addCoordinate(name, *numValues, *arrayType, values);
  }

  void
  XdmfAddPreviousCoordinate(long * pointer, int * index)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->addPreviousCoordinate(*index);
  }

  void
  XdmfClearPreviousCoordinates(long * pointer)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->clearPreviousCoordinates();
  }

  void
  XdmfReplaceCoordinate(long * pointer,
                        int * index,
                        char * name,
                        int * numValues,
                        int * arrayType,
                        void * values)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->replaceCoordinate(*index, name, *numValues, *arrayType, values);
  }

  void
  XdmfRemoveCoordinate(long * pointer, int * index)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->removeCoordinate(*index);
  }

  void
  XdmfRetrieveCoordinateTag(long * pointer, int * index, char * tag, int * tagLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveCoordinateTag(*index, tag, *tagLength);
  }

  void
  XdmfRetrieveCooridnateName(long * pointer, int * index, char * name, int * nameLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveCoordinateName(*index, name, *nameLength);
  }

  void
  XdmfRetrieveCoordinateValues(long * pointer,
                               int * index,
                               void * values,
                               int * dataType,
                               int * numberRead,
                               int * startIndex,
                               int * arrayStride,
                               int * valueStride)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveCoordinateValues(*index,
                                          values,
                                          *dataType,
                                          *numberRead,
                                          *startIndex,
                                          *arrayStride,
                                          *valueStride);
  }

  void
  XdmfRetrieveCoordinateValueType(long * pointer, int * index, int * returnType)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    *returnType = xdmfFortran->retrieveCoordinateValueType(*index);
  }

  void
  XdmfRetrieveCoordinateSize(long * pointer, int * index, int * total)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    *total = xdmfFortran->retrieveCoordinateSize(*index);
  }

  void
  XdmfClearCoordinates(long * pointer)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->clearCoordinates();
  }

  void
  XdmfModifyCoordinateValues(long * pointer,
                             int * index,
                             void * values,
                             int * arrayType,
                             int * numValues,
                             int * startIndex,
                             int * arrayStride,
                             int * valueStride)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->modifyCoordinateValues(*index,
                                        values,
                                        *arrayType,
                                        *numValues,
                                        *startIndex,
                                        *arrayStride,
                                        *valueStride);
  }

  void
  XdmfRetrieveCoordinateNumProperties(long * pointer, int * index, int * total)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    *total = xdmfFortran->retrieveCoordinateNumProperties(*index);
  }

  void
  XdmfRetrieveCoordinateProperty(long * pointer,
                                 int * coordinateIndex,
                                 int * index,
                                 char * key,
                                 int * keyLength,
                                 char * value,
                                 int * valueLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveCoordinateProperty(*coordinateIndex,
                                            *index,
                                            key,
                                            *keyLength,
                                            value,
                                            *valueLength);
  }

  void
  XdmfRetrieveCoordinatePropertyByKey(long * pointer,
                                      int * index,
                                      char * key,
                                      char * value,
                                      int * valueLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveCoordinatePropertyByKey(*index, key, value, *valueLength);
  }

  void
  XdmfRetrieveSetTag(long * pointer, int * index, char * tag, int * tagLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveSetTag(*index, tag, *tagLength);
  }

  void
  XdmfRetrieveSetName(long * pointer, int * index, char * name, int * nameLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveSetTag(*index, name, *nameLength);
  }

  void
  XdmfRetrieveSetType(long * pointer, int * index, int * returnType)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    *returnType = xdmfFortran->retrieveSetType(*index);
  }

  int
  XdmfAddSet(long * pointer,
             char * name,
             int * newSetType,
             char * values,
             int * numValues,
             int * arrayType)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    return xdmfFortran->addSet(name, *newSetType, values, *numValues, *arrayType);
  }

  void
  XdmfAddPreviousSet(long * pointer, int * index)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->addPreviousSet(*index);
  }

  void
  XdmfClearPreviousSets(long * pointer)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->clearPreviousSets();
  }

  void
  XdmfClearSets(long * pointer)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->clearSets();
  }

  void
  XdmfModifySetValues(long * pointer,
                      int * index,
                      void * values,
                      int * arrayType,
                      int * numValues,
                      int * startIndex,
                      int * arrayStride,
                      int * valueStride)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->modifySetValues(*index,
                                 values,
                                 *arrayType,
                                 *numValues,
                                 *startIndex,
                                 *arrayStride,
                                 *valueStride);
  }

  void
  XdmfRetrieveNumSets(long * pointer, int * total)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    *total = xdmfFortran->retrieveNumSets();
  }

  void
  XdmfRetrieveSetSize(long * pointer, int * total, int * index)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    *total = xdmfFortran->retrieveSetSize(*index);
  }

  void
  XdmfRetrieveSetValueType(long * pointer, int * index, int * returnType)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    *returnType = xdmfFortran->retrieveSetValueType(*index);
  }

  void
  XdmfRetrieveSetValues(long * pointer,
                        int * index,
                        void * values,
                        int * dataType,
                        int * numberRead,
                        int * startIndex,
                        int * arrayStride,
                        int * valueStride)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveSetValues(*index,
                                   values,
                                   *dataType,
                                   *numberRead,
                                   *startIndex,
                                   *arrayStride,
                                   *valueStride);
  }

  void
  XdmfOpenSet(long * pointer, int * index, int * openAttributes, int * openInformation)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->openSet(*index, *openAttributes, *openInformation);
  }

  void
  XdmfRemoveSet(long * pointer, int * index)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->removeSet(*index);
  }

  void
  XdmfReplaceSet(long * pointer,
                 int * index,
                 char * name,
                 int * newSetType,
                 char * values,
                 int * numValues,
                 int * arrayType)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->replaceSet(*index, name, *newSetType, values, *numValues, *arrayType);
  }

  void
  XdmfRetrieveSetNumProperties(long * pointer, int * index, int * total)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    *total = xdmfFortran->retrieveSetNumProperties(*index);
  }

  void
  XdmfRetrieveSetProperty(long * pointer,
                          int * setIndex,
                          int * index,
                          char * key,
                          int * keyLength,
                          char * value,
                          int * valueLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveSetProperty(*setIndex, *index, key, *keyLength, value, *valueLength);
  }

  void
  XdmfRetrieveSetPropertyByKey(long * pointer,
                               int * index,
                               char * key,
                               char * value,
                               int * valueLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveSetPropertyByKey(*index, key, value, *valueLength);
  }

  void
  XdmfRetrieveNumInformation(long * pointer, int * total)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    *total = xdmfFortran->retrieveNumInformation();
  }

  void
  XdmfRetrieveInformationTag(long * pointer, int * index, char * tag, int * tagLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveInformationTag(*index, tag, *tagLength);
  }

  void
  XdmfRetrieveInformation(long * pointer,
                          int * index, 
                          char * key,
                          int * keyLength,
                          char * value,
                          int * valueLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveInformation(*index, key, *keyLength, value, *valueLength);
  }

  void
  XdmfRemoveInformation(long * pointer, int * index)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->removeInformation(*index);
  }

  void
  XdmfReplaceInformation(long * pointer, int * index, char * key, char * value)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->replaceInformation(*index, key, value);
  }

  void
  XdmfOpenInformation(long * pointer, int * index)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->openInformation(*index);
  }

  void
  XdmfRetrieveInformationByKey(long * pointer, char * key, char * value, int * valueLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveInformationByKey(key, value, *valueLength);
  }

  void
  XdmfRemoveInformationByKey(long * pointer, char * key)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->removeInformationByKey(key);
  }

  void
  XdmfReplaceInformationByKey(long * pointer, char * key, char * value)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->replaceInformationByKey(key, value);
  }

  void
  XdmfClearPreviousInformation(long * pointer)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->clearPreviousInformation();
  }

  void
  XdmfClearInformations(long * pointer)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->clearInformations();
  }

  void
  XdmfRetrieveInformationNumProperties(long * pointer, int * index, int * total)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    *total = xdmfFortran->retrieveInformationNumProperties(*index);
  }

  void
  XdmfRetrieveInformationProperty(long * pointer,
                                  int * informationIndex,
                                  int * index,
                                  char * key,
                                  int * keyLength,
                                  char * value,
                                  int * valueLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveInformationProperty(*informationIndex,
                                             *index,
                                             key,
                                             *keyLength,
                                             value,
                                             *valueLength);
  }

  void
  XdmfRetrieveInformationPropertyByKey(long * pointer, int * index, char * key, char * value, int * valueLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveInformationPropertyByKey(*index, key, value, *valueLength);
  }

  void
  XdmfAddInformationArray(long * pointer, int * index, void * values, int * numValues, int * arrayType)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->addInformationArray(*index, values, *numValues, *arrayType);
  }

  void
  XdmfInsertInformationIntoInformation(long * pointer,
                                       int * toIndex,
                                       int * fromIndex,
                                       bool * removeFromArray)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->insertInformationIntoInformation(*toIndex, *fromIndex, *removeFromArray);
  }

  void
  XdmfModifyInformationArray(long * pointer,
                             int * index,
                             int * arrayIndex,
                             void * values,
                             int * arrayType,
                             int * numValues,
                             int * insertStart,
                             int * arrayStride,
                             int * valueStride)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->modifyInformationArray(*index,
                                        *arrayIndex,
                                        values,
                                        *arrayType,
                                        *numValues,
                                        *insertStart,
                                        *arrayStride,
                                        *valueStride);
  }

  void
  XdmfRemoveInformationArray(long * pointer, int * index, int * arrayIndex)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->removeInformationArray(*index, *arrayIndex);
  }

  void
  XdmfRetrieveInformationArraySize(long * pointer,
                                   int * index,
                                   int * arrayIndex,
                                   int * total)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    *total = xdmfFortran->retrieveInformationArraySize(*index, *arrayIndex);
  }

  void
  XdmfRetrieveInformationArrayValueType(long * pointer,
                                        int * index,
                                        int * arrayIndex,
                                        int * returnType)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    *returnType = xdmfFortran->retrieveInformationArrayValueType(*index, *arrayIndex);
  }

  void
  XdmfRetrieveInformationArrayValues(long * pointer,
                                     int * index,
                                     int * arrayIndex,
                                     void * values,
                                     int * dataType,
                                     int * numberRead,
                                     int * startIndex,
                                     int * arrayStride, 
                                     int * valueStride)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveInformationArrayValues(*index,
                                                *arrayIndex,
                                                values,
                                                *dataType,
                                                *numberRead,
                                                *startIndex,
                                                *arrayStride,
                                                *valueStride);
  }

  void
  XdmfClearPrevious(long * pointer)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->clearPrevious();
  }

  void
  XdmfSetAllowSetSplitting(long * pointer, bool * newAllow)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->setAllowSetSplitting(*newAllow);
  }

  void
  XdmfSetMaxFileSize(long * pointer, int * newSize)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->setMaxFileSize(*newSize);
  }

  void
  XdmfWrite(long * pointer,
            char * xmlFilePath,
            int * datalimit,
            bool * release)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->write(xmlFilePath, *datalimit, *release);
  }

#ifdef XDMF_BUILD_DSM

  void
  XdmfInitDSMServer(long * pointer,
                    char * filePath,
                    MPI_Fint * comm,
                    int * bufferSize,
                    int * startCoreIndex,
                    int * endCoreIndex)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    MPI_Comm tempComm = MPI_Comm_f2c(*comm);
    xdmfFortran->initDSMServer(filePath,
                               tempComm,
                               *bufferSize,
                               *startCoreIndex,
                               *endCoreIndex);
  }

  void
  XdmfAcceptDSM(long * pointer, int * numConnections)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->acceptDSM(*numConnections);
  }

  void
  XdmfCloseDSMPort(long * pointer)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->closeDSMPort();
  }

  void
  XdmfConnectDSM(long * pointer,
                 char * filePath,
                 MPI_Fint * comm)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    MPI_Comm tempComm = MPI_Comm_f2c(*comm);
    xdmfFortran->connectDSM(filePath, tempComm);
  }

  void
  XdmfGetDSMInterComm(long * pointer, MPI_Fint * returnComm)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    MPI_Comm tempComm = xdmfFortran->getDSMInterComm();
    *returnComm = MPI_Comm_c2f(tempComm);
  }

  void
  XdmfGetDSMIntraComm(long * pointer, MPI_Fint * returnComm)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    MPI_Comm tempComm = xdmfFortran->getDSMIntraComm();
    *returnComm = MPI_Comm_c2f(tempComm);
  }

  void
  XdmfStopDSM(long * pointer)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->stopDSM();
  }

  void
  XdmfReadFromDSM(long * pointer,
                  char * dsmDataSetPath,
                  int * arrayType,
                  void * values,
                  int * start,
                  int * stride,
                  int * dimensions,
                  int * dataspace)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->readFromDSM(dsmDataSetPath,
                             *arrayType,
                             values,
                             *start,
                             *stride,
                             *dimensions,
                             *dataspace);
  }

  void
  XdmfWriteToDSM(long * pointer,
                 char * dsmDataSetPath,
                 int * arrayType,
                 void * values,
                 int * start,
                 int * stride,
                 int * dimensions,
                 int * dataspace)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->writeToDSM(dsmDataSetPath,
                            *arrayType,
                            values,
                            *start,
                            *stride,
                            *dimensions,
                            *dataspace);
  }

#endif

  void
  XdmfWriteHDF5(long * pointer,
                char * xmlFilePath,
                bool * release)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->writeHDF5(xmlFilePath, release);
  }

  void
  XdmfInitHDF5(long * pointer,
                char * xmlFilePath,
                bool * release)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->initHDF5(xmlFilePath, release);
  }

  unsigned int
  XdmfGetMaxOpenedFiles()
  {
    return XdmfHDF5Controller::getMaxOpenedFiles();
  }

  void
  XdmfSetMaxOpenedFiles(int * numFilePtr)
  {
    XdmfHDF5Controller::setMaxOpenedFiles(*numFilePtr);
  }

  void
  XdmfCloseOpenedHDF5Files()
  {
    XdmfHDF5Controller::closeFiles();
  }

  void
  XdmfRead(long * pointer,
           char * xmlFilePath)
  {
    struct stat buffer;
    if ( stat(xmlFilePath, &buffer) == 0 ) { 
       XdmfFortran * xdmfFortran = (XdmfFortran *) *pointer;
       xdmfFortran->read( xmlFilePath );
    }   
  }

  int
  XdmfSetTopologyPolyline(long * pointer,
                          int * nodesPerElement,
                          int * numValues,
                          int * arrayType,
                          void * connectivityValues)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    return xdmfFortran->setTopologyPolyline(*nodesPerElement,
                                            *numValues,
                                            *arrayType,
                                            connectivityValues);
  }

  int
  XdmfSetTopologyPolygon(long * pointer,
                         int * nodesPerElement,
                         int * numValues,
                         int * arrayType,
                         void * connectivityValues)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    return xdmfFortran->setTopologyPolygon(*nodesPerElement,
                                           *numValues,
                                           *arrayType,
                                           connectivityValues);
  }
}
