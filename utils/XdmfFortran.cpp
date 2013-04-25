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
#include "XdmfSystemUtils.hpp"
#include "XdmfTime.hpp"
#include "XdmfTopology.hpp"
#include "XdmfTopologyType.hpp"
#include "XdmfUnstructuredGrid.hpp"
#include "XdmfWriter.hpp"
#include "XdmfHDF5Writer.hpp"

namespace {

  template <typename T>
  void
  insertElements(const T grid,
                 std::vector<shared_ptr<XdmfAttribute> > & mAttributes,
                 std::vector<shared_ptr<XdmfInformation> > & mInformations,
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
  readFromArray(shared_ptr<XdmfArray> array, 
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
      XdmfError::message(XdmfError::FATAL, "Invalid array number type");  
    }  
  }

  // write values to xdmf array for a number type
  void 
  writeToArray(shared_ptr<XdmfArray> array,
               const unsigned int numValues,
               const int arrayType, 
               const void * const values)
  {
    switch(arrayType) {
    case XDMF_ARRAY_TYPE_INT8:
      array->insert(0, 
                    static_cast<const char *>(values), 
                    numValues); 
      break;
    case XDMF_ARRAY_TYPE_INT16:
      array->insert(0, 
                    static_cast<const short *>(values), 
                    numValues); 
      break;
    case XDMF_ARRAY_TYPE_INT32:
      array->insert(0, 
                    static_cast<const int *>(values), 
                    numValues); 
      break;
    case XDMF_ARRAY_TYPE_INT64:
      array->insert(0, 
                    static_cast<const long *>(values), 
                    numValues); 
      break;
    case XDMF_ARRAY_TYPE_UINT8:
      array->insert(0, 
                    static_cast<const unsigned char *>(values), 
                    numValues); 
      break;
    case XDMF_ARRAY_TYPE_UINT16:
      array->insert(0, 
                    static_cast<const unsigned short *>(values), 
                    numValues); 
      break;
    case XDMF_ARRAY_TYPE_UINT32:
      array->insert(0, 
                    static_cast<const unsigned int *>(values), 
                    numValues); 
      break;
    case XDMF_ARRAY_TYPE_FLOAT32:
      array->insert(0, 
                    static_cast<const float *>(values), 
                    numValues); 
      break;
    case XDMF_ARRAY_TYPE_FLOAT64:
      array->insert(0, 
                    static_cast<const double *>(values), 
                    numValues); 
      break;
    default:
      XdmfError::message(XdmfError::FATAL, "Invalid array type");  
    }          
  }

}

XdmfFortran::XdmfFortran() :
  mDomain(XdmfDomain::New()),
  mGeometry(shared_ptr<XdmfGeometry>()),
  mTime(shared_ptr<XdmfTime>()),
  mTopology(shared_ptr<XdmfTopology>())
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
    XdmfError::message(XdmfError::FATAL, "Invalid attribute center");
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
    XdmfError::message(XdmfError::FATAL, "Invalid attribute type");
  }

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

void 
XdmfFortran::addGrid(const char * const name)
{
  const shared_ptr<XdmfUnstructuredGrid> grid = XdmfUnstructuredGrid::New();
  grid->setName(name);

  if(mGeometry == NULL) {
    XdmfError::message(XdmfError::FATAL, 
                       "Must set geometry before adding grid.");
  }

  if(mTopology == NULL) {
    XdmfError::message(XdmfError::FATAL, 
                       "Must set topology before adding grid.");
  }
  
  grid->setGeometry(mGeometry);
  grid->setTopology(mTopology);
  
  insertElements(grid,
                 mAttributes,
                 mInformations,
                 mTime,
                 mDomain,
                 mGridCollections);
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
    XdmfError::message(XdmfError::FATAL, "Invalid grid collection type");
  }

  insertElements(gridCollection,
                 mAttributes,
                 mInformations,
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
    XdmfError::message(XdmfError::FATAL, "Invalid attribute id");
  }
  mAttributes.push_back(mPreviousAttributes[attributeId]);
}

void 
XdmfFortran::addPreviousInformation(const int informationId)
{
  if(informationId >= (int)mPreviousInformations.size()) {
    XdmfError::message(XdmfError::FATAL, "Invalid information id");
  }
  mInformations.push_back(mPreviousInformations[informationId]);
}

void 
XdmfFortran::closeGridCollection()
{
  if(!mGridCollections.empty()) {
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
    XdmfError::message(XdmfError::FATAL, "Invalid geometry type."); 
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
    XdmfError::message(XdmfError::FATAL, "Invalid geometry id");
  }
  mGeometry = mPreviousGeometries[geometryId];
}

void 
XdmfFortran::setPreviousTopology(const int topologyId)
{
  if(topologyId >= (int)mPreviousTopologies.size()) {
    XdmfError::message(XdmfError::FATAL, "Invalid topology id");
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
                         const int polyNodesPerElement)
{
  mTopology = XdmfTopology::New();

  switch(topologyType) {
  case XDMF_TOPOLOGY_TYPE_POLYVERTEX:
    mTopology->setType(XdmfTopologyType::Polyvertex());
    break;
  case XDMF_TOPOLOGY_TYPE_POLYLINE:
    mTopology->setType(XdmfTopologyType::Polyline(polyNodesPerElement));
    break;
  case XDMF_TOPOLOGY_TYPE_POLYGON:
    mTopology->setType(XdmfTopologyType::Polygon(polyNodesPerElement));
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
    XdmfError::message(XdmfError::FATAL, "Invalid topology type."); 
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

void 
XdmfFortran::write(const char * const xmlFilePath, 
                   const unsigned int numValues)
{
  shared_ptr<XdmfWriter> writer = XdmfWriter::New(xmlFilePath);

  // check if we have a previous heavy data writer around
  shared_ptr<XdmfHeavyDataWriter> heavyDataWriter = 
    writer->getHeavyDataWriter();
  const std::string heavyDataWriterPath = heavyDataWriter->getFilePath();
  std::map<std::string, shared_ptr<XdmfHeavyDataWriter> >::iterator iter =
    mPreviousWriters.find(heavyDataWriterPath);
  if(iter != mPreviousWriters.end()) {
    writer->setHeavyDataWriter(iter->second);
  }
  else {
    heavyDataWriter->setReleaseData(true);
    mPreviousWriters[heavyDataWriterPath] = heavyDataWriter;
  }

  writer->setLightDataLimit(numValues);
  mDomain->accept(writer);
}

void 
XdmfFortran::writeHDF5(const char * const hdf5FilePath)
{
  // convert path to fully resolved path
  const std::string fullPath = XdmfSystemUtils::getRealPath(hdf5FilePath);

  // check if we have a previous heavy data writer around
  shared_ptr<XdmfHeavyDataWriter> writer;
  std::map<std::string, shared_ptr<XdmfHeavyDataWriter> >::iterator iter =
    mPreviousWriters.find(fullPath);
  if(iter != mPreviousWriters.end()) {
    writer = iter->second;
  }
  else {
    writer = XdmfHDF5Writer::New(fullPath);
    writer->setReleaseData(true);
    mPreviousWriters[fullPath] = writer;
  }

  mDomain->accept(writer);
}

void 
XdmfFortran::read(const char * const xmlFilePath)
{
  shared_ptr<XdmfReader> reader = XdmfReader::New();
  mDomain = shared_dynamic_cast<XdmfDomain>(reader->read( xmlFilePath )); 
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
              char * gridName)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->addGrid(gridName);
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
  XdmfCloseGridCollection(long * pointer)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->closeGridCollection();
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
                                    connectivityValues);
  }

  int
  XdmfSetTopologyPolyline(long * pointer,
                          int  * nodesPerElement,
                          int  * numValues,
                          int  * arrayType,
                          void * connectivityValues)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    return xdmfFortran->setTopology(XDMF_TOPOLOGY_TYPE_POLYLINE, 
                                    *numValues, 
                                    *arrayType,
                                    connectivityValues,
                                    *nodesPerElement);
  }

  int
  XdmfSetTopologyPolyvertex(long * pointer,
                            int  * nodesPerElement,
                            int  * numValues,
                            int  * arrayType,
                            void * connectivityValues)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    return xdmfFortran->setTopology(XDMF_TOPOLOGY_TYPE_POLYVERTEX, 
                                    *numValues, 
                                    *arrayType,
                                    connectivityValues,
                                    *nodesPerElement);
  }

  void
  XdmfWrite(long * pointer,
            char * xmlFilePath,
            const unsigned int * numValues)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->write(xmlFilePath, *numValues);
  }


  void
  XdmfWriteHDF5(long * pointer,
                char * hdf5FilePath)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->writeHDF5(hdf5FilePath);
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
  
}
