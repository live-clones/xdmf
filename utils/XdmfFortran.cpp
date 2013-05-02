
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
#include "string.h"

#include <stdio.h>
#include <stdarg.h>

namespace {

  template <typename T>
  void
  insertElements(const T grid,
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
      try {
        XdmfError::message(XdmfError::FATAL, "Invalid array number type");
      }
      catch (XdmfError e) {
        throw e;
      }
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
      try {
        XdmfError::message(XdmfError::FATAL, "Invalid array type");
      }
      catch (XdmfError e) {
        throw e;
      }
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

void 
XdmfFortran::addGrid(const char * const name, const bool writeToHDF5)//unstructured version
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
    grid->accept(mHeavyDataWriter);
  }
}

void
XdmfFortran::addGridCurvilinear(const char * const name, const bool writeToHDF5)
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

  const shared_ptr<XdmfCurvilinearGrid> grid = XdmfCurvilinearGrid::New(mDimensions);
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
    grid->accept(mHeavyDataWriter);
  }
}

void
XdmfFortran::addGridRectilinear(const char * const name, const bool writeToHDF5)
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

  const shared_ptr<XdmfRectilinearGrid> grid = XdmfRectilinearGrid::New(mCoordinates);
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
    grid->accept(mHeavyDataWriter);
  }
}

void
XdmfFortran::addGridRegular(const char * const name, const bool writeToHDF5)
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

  const shared_ptr<XdmfRegularGrid> grid = XdmfRegularGrid::New(mBrick, mDimensions, mOrigin);
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
    grid->accept(mHeavyDataWriter);
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
      XdmfError::message(XdmfError::FATAL, "Invalid grid collection type");
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
      XdmfError::message(XdmfError::FATAL, "Invalid attribute id");
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
      XdmfError::message(XdmfError::FATAL, "Invalid information id");
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
      mGridCollections.top()->accept(mHeavyDataWriter);
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
      XdmfError::message(XdmfError::FATAL, "Invalid geometry type.");
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
      XdmfError::message(XdmfError::FATAL, "Invalid geometry id");
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
      XdmfError::message(XdmfError::FATAL, "Invalid topology id");
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
      XdmfError::message(XdmfError::FATAL, "Invalid topology type.");
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
	if (!mGridCollections.empty())
	{
		return mGridCollections.top()->getNumberGridCollections();
	}
	else
	{
		try 
		{
			XdmfError::message(XdmfError::FATAL, "Error: No grid collections are open."); 
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

void
XdmfFortran::retrieveDomainTag(char * returnTag, int tagLength)
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
XdmfFortran::retrieveDomainProperty(int index, char * key, int keyLength, char * value, int valueLength)
{
	if (index < mDomain->getItemProperties().size())
	{
		std::map<std::string, std::string>::iterator walker = mDomain->getItemProperties().begin();
		for (int i = 0; i<index; i++)
		{
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
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

void
XdmfFortran::retrieveDomainPropertyByKey(char * key, char * value, int valueLength)
{
	std::string tempString = key;
	memset(value, 0, valueLength);
	if ((mDomain->getItemProperties().count(tempString))>0)
	{
		char * tempValue = strdup(mDomain->getItemProperties()[tempString].c_str());
		memcpy(value, tempValue, strlen(tempValue)+1);
		delete [] tempValue;
	}
}

void
XdmfFortran::removeDomainGridCollection(int index)
{
	if (mDomain->getNumberGridCollections() > index)
	{
		mDomain->removeGridCollection(index);
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

void
XdmfFortran::openDomainGridCollection(int index, int openMaps, int openAttributes, int openInformation, int openSets)
{
	if (mDomain->getNumberGridCollections() > index)
	{
		shared_ptr<XdmfGridCollection> openedGridCollection = mDomain->getGridCollection(index);
		int i;
		int n;
		if (openMaps == 1)
		{
			n = openedGridCollection->getNumberMaps();
			shared_ptr<XdmfMap> openedMap;
			for (i = 0; i < n; i++)
			{
				openedMap = openedGridCollection->getMap(i);
				mMaps.push_back(openedMap);
				mPreviousMaps.push_back(openedMap);
			}
		}
		if (openAttributes == 1)
		{
			n = openedGridCollection->getNumberAttributes();
			shared_ptr<XdmfAttribute> openedAttribute;
			for (i = 0; i < n; i++)
			{
				openedAttribute = openedGridCollection->getAttribute(i);
				mAttributes.push_back(openedAttribute);
				mPreviousAttributes.push_back(openedAttribute);
			}
		}
		if (openInformation == 1)
		{
			n = openedGridCollection->getNumberInformations();
			shared_ptr<XdmfInformation> openedInformation;
			for (i = 0; i < n; i++)
			{
				openedInformation = openedGridCollection->getInformation(i);
				mInformations.push_back(openedInformation);
				mPreviousInformations.push_back(openedInformation);
			}
		}
		if (openSets == 1)
		{
			n = openedGridCollection->getNumberSets();
			shared_ptr<XdmfSet> openedSet;
			for (i = 0; i < n; i++)
			{
				openedSet = openedGridCollection->getSet(i);
				mSets.push_back(openedSet);
			}
			mGridCollections.push(openedGridCollection);
		}
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

void
XdmfFortran::removeGridCollectionGridCollection(int index)
{
	if (!mGridCollections.empty())
	{
		if (mGridCollections.top()->getNumberGridCollections() > index)
		{
			mGridCollections.top()->removeGridCollection(index);
		}
		else
		{
			try 
			{
				XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: No grid collections are open.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

void
XdmfFortran::openGridCollectionGridCollection(int index, int openMaps, int openAttributes, int openInformation, int openSets)
{
	if (!mGridCollections.empty())
	{
		if (mGridCollections.top()->getNumberGridCollections() > index)
		{
			shared_ptr<XdmfGridCollection> openedGridCollection = mGridCollections.top()->getGridCollection(index);
			int i;
			int n;
			if (openMaps == 1)
			{
				n = openedGridCollection->getNumberMaps();
				shared_ptr<XdmfMap> openedMap;
				for (i = 0; i < n; i++)
				{
					openedMap = openedGridCollection->getMap(i);
					mMaps.push_back(openedMap);
					mPreviousMaps.push_back(openedMap);
				}
			}
			if (openAttributes == 1)
			{
				n = openedGridCollection->getNumberAttributes();
				shared_ptr<XdmfAttribute> openedAttribute;
				for (i = 0; i < n; i++)
				{
					openedAttribute = openedGridCollection->getAttribute(i);
					mAttributes.push_back(openedAttribute);
					mPreviousAttributes.push_back(openedAttribute);
				}
			}
			if (openInformation == 1)
			{
				n = openedGridCollection->getNumberInformations();
				shared_ptr<XdmfInformation> openedInformation;
				for (i = 0; i < n; i++)
				{
					openedInformation = openedGridCollection->getInformation(i);
					mInformations.push_back(openedInformation);
					mPreviousInformations.push_back(openedInformation);
				}
			}
			if (openSets == 1)
			{
				n = openedGridCollection->getNumberSets();
				shared_ptr<XdmfSet> openedSet;
				for (i = 0; i < n; i++)
				{
					openedSet = openedGridCollection->getSet(i);
					mSets.push_back(openedSet);
				}
				mGridCollections.push(openedGridCollection);
			}
		}
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: No grid collections are open.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

void
XdmfFortran::retrieveGridCollectionTag(char * returnTag, int tagLength)
{
	if (!mGridCollections.empty())
	{
		char * tempTag = strdup(mGridCollections.top()->getItemTag().c_str());
		memset(returnTag, 0, tagLength);
		memcpy(returnTag, tempTag, strlen(tempTag)+1);
		delete [] tempTag;
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: No grid collections are open.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

void
XdmfFortran::retrieveGridCollectionName(char * returnName, int nameLength)
{
	if (!mGridCollections.empty())
	{
		char * tempName = strdup(mGridCollections.top()->getName().c_str());
		memset(returnName, 0, nameLength);
		memcpy(returnName, tempName, strlen(tempName)+1);
		delete [] tempName;
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: No grid collections are open.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

int
XdmfFortran::retrieveGridCollectionNumProperties()
{
	if (!mGridCollections.empty())
	{
		return mGridCollections.top()->getItemProperties().size();
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: No grid collections are open.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

void
XdmfFortran::retrieveGridCollectionProperty(int index, char * key, int keyLength, char * value, int valueLength)
{
	if (!mGridCollections.empty())
	{
		if (index < mGridCollections.top()->getItemProperties().size())
		{
			std::map<std::string, std::string>::iterator walker = mGridCollections.top()->getItemProperties().begin();
			for (int i = 0; i<index; i++)
			{
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
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: No grid collections are open.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

void
XdmfFortran::retrieveGridCollectionPropertyByKey(char * key, char * value, int valueLength)
{
	if (!mGridCollections.empty())
	{
		std::string tempString = key;
		memset(value, 0, valueLength);
		if ((mGridCollections.top()->getItemProperties().count(tempString))>0)
		{
			char * tempValue = strdup(mGridCollections.top()->getItemProperties()[tempString].c_str());
			memcpy(value, tempValue, strlen(tempValue)+1);
			delete [] tempValue;
		}
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: No grid collections are open.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

void
XdmfFortran::openDomainGrid(int gridType, int index, int openMaps, int openAttributes, int openInformation, int openSets)
{
	if (gridType == XDMF_GRID_TYPE_CURVILINEAR)
	{
		if (index < mDomain->getNumberCurvilinearGrids())
		{
			shared_ptr<XdmfCurvilinearGrid> openedGrid = mDomain->getCurvilinearGrid(index);
			shared_ptr<const XdmfArrayType> dataType;
			mGeometry = openedGrid->getGeometry();
			mDimensions = openedGrid->getDimensions();
			mTime = openedGrid->getTime();
			int i;
			int n;
			if (openAttributes == 1)
			{
				n = openedGrid->getNumberAttributes();
				shared_ptr<XdmfAttribute> openedAttribute;
				for (i = 0; i < n; i++)
				{
					openedAttribute = openedGrid->getAttribute(i);
					mAttributes.push_back(openedAttribute);
					mPreviousAttributes.push_back(openedAttribute);
				}
			}
			if (openMaps == 1)
			{
				n = openedGrid->getNumberMaps();
				shared_ptr<XdmfMap> openedMap;
				for (i = 0; i < n; i++)
				{
					openedMap = openedGrid->getMap(i);
					mMaps.push_back(openedMap);
					mPreviousMaps.push_back(openedMap);
				}
			}
			if (openInformation == 1)
			{
				n = openedGrid->getNumberInformations();
				shared_ptr<XdmfInformation> openedInformation;
				for (i = 0; i < n; i++)
				{
					openedInformation = openedGrid->getInformation(i);
					mInformations.push_back(openedInformation);
					mPreviousInformations.push_back(openedInformation);
				}
			}
			if (openSets == 1)
			{
				n = openedGrid->getNumberSets();
				shared_ptr<XdmfSet> openedSet;
				for (i = 0; i < n; i++)
				{
					openedSet = openedGrid->getSet(i);
					mSets.push_back(openedSet);
				}
			}
		}
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
	else if (gridType == XDMF_GRID_TYPE_RECTILINEAR)
	{
		if (index < mDomain->getNumberRectilinearGrids())
		{
			shared_ptr<XdmfRectilinearGrid> openedGrid = mDomain->getRectilinearGrid(index);
			shared_ptr<const XdmfArrayType> dataType;
			mCoordinates = openedGrid->getCoordinates();
			mTime = openedGrid->getTime();
			int i;
			int n;
			if (openAttributes == 1)
			{
				n = openedGrid->getNumberAttributes();
				shared_ptr<XdmfAttribute> openedAttribute;
				for (i = 0; i < n; i++)
				{
					openedAttribute = openedGrid->getAttribute(i);
					mAttributes.push_back(openedAttribute);
					mPreviousAttributes.push_back(openedAttribute);
				}
			}
			if (openMaps == 1)
			{
				n = openedGrid->getNumberMaps();
				shared_ptr<XdmfMap> openedMap;
				for (i = 0; i < n; i++)
				{
					openedMap = openedGrid->getMap(i);
					mMaps.push_back(openedMap);
					mPreviousMaps.push_back(openedMap);
				}
			}
			if (openInformation == 1)
			{
				n = openedGrid->getNumberInformations();
				shared_ptr<XdmfInformation> openedInformation;
				for (i = 0; i < n; i++)
				{
					openedInformation = openedGrid->getInformation(i);
					mInformations.push_back(openedInformation);
					mPreviousInformations.push_back(openedInformation);
				}
			}
			if (openSets == 1)
			{
				n = openedGrid->getNumberSets();
				shared_ptr<XdmfSet> openedSet;
				for (i = 0; i < n; i++)
				{
					openedSet = openedGrid->getSet(i);
					mSets.push_back(openedSet);
				}
			}
		}
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
	else if (gridType == XDMF_GRID_TYPE_REGULAR)
	{
		if (index < mDomain->getNumberRegularGrids())
		{
			shared_ptr<XdmfRegularGrid> openedGrid = mDomain->getRegularGrid(index);
			shared_ptr<const XdmfArrayType> dataType;
			mTime = openedGrid->getTime();
			mBrick = openedGrid->getBrickSize();
			mOrigin = openedGrid->getOrigin();
			mDimensions = openedGrid->getDimensions();
			int i;
			int n;
			if (openAttributes == 1)
			{
				n = openedGrid->getNumberAttributes();
				shared_ptr<XdmfAttribute> openedAttribute;
				for (i = 0; i < n; i++)
				{
					openedAttribute = openedGrid->getAttribute(i);
					mAttributes.push_back(openedAttribute);
					mPreviousAttributes.push_back(openedAttribute);
				}
			}
			if (openMaps == 1)
			{
				n = openedGrid->getNumberMaps();
				shared_ptr<XdmfMap> openedMap;
				for (i = 0; i < n; i++)
				{
					openedMap = openedGrid->getMap(i);
					mMaps.push_back(openedMap);
					mPreviousMaps.push_back(openedMap);
				}
			}
			if (openInformation == 1)
			{
				n = openedGrid->getNumberInformations();
				shared_ptr<XdmfInformation> openedInformation;
				for (i = 0; i < n; i++)
				{
					openedInformation = openedGrid->getInformation(i);
					mInformations.push_back(openedInformation);
					mPreviousInformations.push_back(openedInformation);
				}
			}
			if (openSets == 1)
			{
				n = openedGrid->getNumberSets();
				shared_ptr<XdmfSet> openedSet;
				for (i = 0; i < n; i++)
				{
					openedSet = openedGrid->getSet(i);
					mSets.push_back(openedSet);
				}
			}
		}
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
	else if (gridType == XDMF_GRID_TYPE_UNSTRUCTURED)
	{
		if (index < mDomain->getNumberUnstructuredGrids())
		{
			shared_ptr<XdmfUnstructuredGrid> openedGrid = mDomain->getUnstructuredGrid(index);
			mTopology = openedGrid->getTopology();
			mGeometry = openedGrid->getGeometry();
			mTime = openedGrid->getTime();
			int i;
			int n;
			if (openAttributes == 1)
			{
				n = openedGrid->getNumberAttributes();
				shared_ptr<XdmfAttribute> openedAttribute;
				for (i = 0; i < n; i++)
				{
					openedAttribute = openedGrid->getAttribute(i);
					mAttributes.push_back(openedAttribute);
					mPreviousAttributes.push_back(openedAttribute);
				}
			}
			if (openMaps == 1)
			{
				n = openedGrid->getNumberMaps();
				shared_ptr<XdmfMap> openedMap;
				for (i = 0; i < n; i++)
				{
					openedMap = openedGrid->getMap(i);
					mMaps.push_back(openedMap);
					mPreviousMaps.push_back(openedMap);
				}
			}
			if (openInformation == 1)
			{
				n = openedGrid->getNumberInformations();
				shared_ptr<XdmfInformation> openedInformation;
				for (i = 0; i < n; i++)
				{
					openedInformation = openedGrid->getInformation(i);
					mInformations.push_back(openedInformation);
					mPreviousInformations.push_back(openedInformation);
				}
			}
			if (openSets == 1)
			{
				n = openedGrid->getNumberSets();
				shared_ptr<XdmfSet> openedSet;
				for (i = 0; i < n; i++)
				{
					openedSet = openedGrid->getSet(i);
					mSets.push_back(openedSet);
				}
			}
		}
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Invalid Grid Type.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

void
XdmfFortran::removeDomainGrid(int gridType, int index)
{
	if (gridType == XDMF_GRID_TYPE_CURVILINEAR)
	{
		if (index < mDomain->getNumberCurvilinearGrids())
		{
			mDomain->removeCurvilinearGrid(index);
		}
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
	else if (gridType == XDMF_GRID_TYPE_RECTILINEAR)
	{
		if (index < mDomain->getNumberRectilinearGrids())
		{
			mDomain->removeRectilinearGrid(index);
		}
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
	else if (gridType == XDMF_GRID_TYPE_REGULAR)
	{
		if (index < mDomain->getNumberRegularGrids())
		{
			mDomain->removeRegularGrid(index);
		}
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
	else if (gridType == XDMF_GRID_TYPE_UNSTRUCTURED)
	{
		if (index < mDomain->getNumberUnstructuredGrids())
		{
			mDomain->removeUnstructuredGrid(index);
		}
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Invalid Grid Type.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

void
XdmfFortran::replaceDomainGrid(int gridType, int index, char * name)
{
	int i;
	if (gridType == XDMF_GRID_TYPE_CURVILINEAR)
	{
		if (index < mDomain->getNumberCurvilinearGrids())
		{
			if(mDimensions == NULL)
			{
				try
				{
					XdmfError::message(XdmfError::FATAL, "Must set dimensions before replacing grid.");
				}
				catch (XdmfError e)
				{
					throw e;
				}
			}
			if(mGeometry == NULL)
			{
				try
				{
					XdmfError::message(XdmfError::FATAL, 
						"Must set geometry before adding grid.");
				}
				catch (XdmfError e)
				{
					throw e;
				}
			}

			shared_ptr<XdmfCurvilinearGrid> grid = mDomain->getCurvilinearGrid(index);
			grid->setName(name);

			grid->setGeometry(mGeometry);
			grid->setDimensions(mDimensions);

			for (i=grid->getNumberAttributes()-1;i>=0;i--)
			{
				grid->removeAttribute(0);
			}

			for(std::vector<shared_ptr<XdmfAttribute> >::const_iterator iter =
				mAttributes.begin(); 
				iter != mAttributes.end();
				++iter)
			{
				grid->insert(*iter);
			}

			mAttributes.clear();

			for( i=grid->getNumberInformations()-1;i>=0;i--)
			{
				grid->removeInformation(0);
			}

			for(std::vector<shared_ptr<XdmfInformation> >::const_iterator iter =
				mInformations.begin(); 
				iter != mInformations.end();
				++iter)
			{
				grid->insert(*iter);
			}

			mInformations.clear();

			for (i=grid->getNumberSets()-1;i>=0;i--)
			{
				grid->removeSet(0);
			}

			for(std::vector<shared_ptr<XdmfSet> >::const_iterator iter =
				mSets.begin(); 
				iter != mSets.end();
				++iter)
			{
				grid->insert(*iter);
			}

			mSets.clear();

			for (i=grid->getNumberMaps()-1;i>=0;i--)
			{
				grid->removeMap(0);
			}

			for(std::vector<shared_ptr<XdmfMap> >::const_iterator iter =
				mMaps.begin(); 
				iter != mMaps.end();
				++iter)
			{
				grid->insert(*iter);
			}

			mMaps.clear();
    
			if(mTime)
			{
				grid->setTime(mTime);
			}
		}
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
	else if (gridType == XDMF_GRID_TYPE_RECTILINEAR)
	{
		if (index < mDomain->getNumberRectilinearGrids())
		{
			if(mCoordinates.empty())
			{
				try
				{
					XdmfError::message(XdmfError::FATAL, 
						"Must set Coordinates before adding grid.");
				}
				catch (XdmfError e)
				{
					throw e;
				}
			}

			shared_ptr<XdmfRectilinearGrid> grid = mDomain->getRectilinearGrid(index);
			grid->setCoordinates(mCoordinates);
			mCoordinates.clear();
			for (i=grid->getNumberAttributes()-1;i>=0;i--)
			{
				grid->removeAttribute(0);
			}

			for(std::vector<shared_ptr<XdmfAttribute> >::const_iterator iter =
				mAttributes.begin(); 
				iter != mAttributes.end();
				++iter)
			{
				grid->insert(*iter);
			}

			mAttributes.clear();

			for (i=grid->getNumberInformations()-1;i>=0;i--)
			{
				grid->removeInformation(0);
			}

			for(std::vector<shared_ptr<XdmfInformation> >::const_iterator iter =
				mInformations.begin(); 
				iter != mInformations.end();
				++iter)
			{
				grid->insert(*iter);
			}

			mInformations.clear();

			for (i=grid->getNumberSets()-1;i>=0;i--)
			{
				grid->removeSet(0);
			}

			for(std::vector<shared_ptr<XdmfSet> >::const_iterator iter =
				mSets.begin(); 
				iter != mSets.end();
				++iter)
			{
				grid->insert(*iter);
			}

			mSets.clear();

			for (i=grid->getNumberMaps()-1;i>=0;i--)
			{
				grid->removeMap(0);
			}

			for(std::vector<shared_ptr<XdmfMap> >::const_iterator iter =
				mMaps.begin(); 
				iter != mMaps.end();
				++iter)
			{
				grid->insert(*iter);
			}

			mMaps.clear();
    
			if(mTime)
			{
				grid->setTime(mTime);
			}
		}
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
	else if (gridType == XDMF_GRID_TYPE_REGULAR)
	{
		if (index < mDomain->getNumberRegularGrids())
		{
			if(mBrick == NULL)
			{
				try
				{
					XdmfError::message(XdmfError::FATAL, 
						"Must set brick size before adding grid.");
				}
				catch (XdmfError e)
				{
					throw e;
				}
			}

			if(mDimensions == NULL)
			{
				try
				{
					XdmfError::message(XdmfError::FATAL, 
						"Must set dimensions before adding grid.");
				}
				catch (XdmfError e)
				{
					throw e;
				}
			}

			if(mOrigin == NULL)
			{
				try
				{
					XdmfError::message(XdmfError::FATAL, 
						"Must set origin before adding grid.");
				}
				catch (XdmfError e)
				{
					throw e;
				}
			}
  
	    		shared_ptr<XdmfRegularGrid> grid = mDomain->getRegularGrid(index);
			grid->setOrigin(mOrigin);
			grid->setDimensions(mDimensions);
			grid->setBrickSize(mBrick);

			for (i=grid->getNumberAttributes()-1;i>=0;i--)
			{
				grid->removeAttribute(0);
			}

			for(std::vector<shared_ptr<XdmfAttribute> >::const_iterator iter =
				mAttributes.begin(); 
				iter != mAttributes.end();
				++iter)
			{
				grid->insert(*iter);
			}

			mAttributes.clear();

			for (i=grid->getNumberInformations()-1;i>=0;i--)
			{
				grid->removeInformation(0);
			}

			for(std::vector<shared_ptr<XdmfInformation> >::const_iterator iter =
				mInformations.begin(); 
				iter != mInformations.end();
				++iter)
			{
				grid->insert(*iter);
			}

			mInformations.clear();

			for (i=grid->getNumberSets()-1;i>=0;i--)
			{
				grid->removeSet(0);
			}

			for(std::vector<shared_ptr<XdmfSet> >::const_iterator iter =
				mSets.begin(); 
				iter != mSets.end();
				++iter)
			{
				grid->insert(*iter);
			}

			mSets.clear();

			for (i=grid->getNumberMaps()-1;i>=0;i--)
			{
				grid->removeMap(0);
			}

			for(std::vector<shared_ptr<XdmfMap> >::const_iterator iter =
				mMaps.begin(); 
				iter != mMaps.end();
				++iter)
			{
				grid->insert(*iter);
			}

			mMaps.clear();
    
			if(mTime)
			{
				grid->setTime(mTime);
			}
		}
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
	else if (gridType == XDMF_GRID_TYPE_UNSTRUCTURED)
	{
		if (index < mDomain->getNumberUnstructuredGrids())
		{
			if(mGeometry == NULL)
			{
				try
				{
					XdmfError::message(XdmfError::FATAL, 
						"Must set geometry before adding grid.");
				}
				catch (XdmfError e)
				{
					throw e;
				}
			}
			if(mTopology == NULL)
			{
				try
				{
					XdmfError::message(XdmfError::FATAL, 
						"Must set topology before adding grid.");
				}
				catch (XdmfError e)
				{
					throw e;
				}
			}
			shared_ptr<XdmfUnstructuredGrid> grid = mDomain->getUnstructuredGrid(index);
			grid->setName(name);
			grid->setGeometry(mGeometry);
			grid->setTopology(mTopology);

			for (i=grid->getNumberAttributes()-1;i>=0;i--)
			{
				grid->removeAttribute(0);
			}

			for(std::vector<shared_ptr<XdmfAttribute> >::const_iterator iter =
				mAttributes.begin(); 
				iter != mAttributes.end();
				++iter)
			{
				grid->insert(*iter);
			}

			mAttributes.clear();

			for (i=grid->getNumberInformations()-1;i>=0;i--)
			{
				grid->removeInformation(0);
			}

			for(std::vector<shared_ptr<XdmfInformation> >::const_iterator iter =
				mInformations.begin(); 
				iter != mInformations.end();
				++iter)
			{
				grid->insert(*iter);
			}

			mInformations.clear();

			for (i=grid->getNumberSets()-1;i>=0;i--)
			{
				grid->removeSet(0);
			}

			for(std::vector<shared_ptr<XdmfSet> >::const_iterator iter =
				mSets.begin(); 
				iter != mSets.end();
				++iter)
			{
				grid->insert(*iter);
			}

			mSets.clear();

			for (i=grid->getNumberMaps()-1;i>=0;i--)
			{
				grid->removeMap(0);
			}

			for(std::vector<shared_ptr<XdmfMap> >::const_iterator iter =
				mMaps.begin(); 
				iter != mMaps.end();
				++iter)
			{
				grid->insert(*iter);
			}

			mMaps.clear();
    
			if(mTime)
			{
				grid->setTime(mTime);
			}
		}
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
}

void
XdmfFortran::retrieveDomainGridName(int gridType, int index, char * returnName, int nameLength)
{
	shared_ptr<XdmfGrid> openedGrid;
	if (gridType == XDMF_GRID_TYPE_CURVILINEAR)
	{
		if (index < mDomain->getNumberCurvilinearGrids())
		{
			openedGrid = mDomain->getCurvilinearGrid(index);
		}
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
	else if (gridType == XDMF_GRID_TYPE_RECTILINEAR)
	{
		if (index < mDomain->getNumberRectilinearGrids())
		{
			openedGrid = mDomain->getRectilinearGrid(index);
		}
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
	else if (gridType == XDMF_GRID_TYPE_REGULAR)
	{
		if (index < mDomain->getNumberRegularGrids())
		{
			openedGrid = mDomain->getRegularGrid(index);
		}
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
	else if (gridType == XDMF_GRID_TYPE_UNSTRUCTURED)
	{
		if (index < mDomain->getNumberUnstructuredGrids())
		{
			openedGrid = mDomain->getUnstructuredGrid(index);
		}
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Invalid Grid Type.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
	char * tempName = strdup(openedGrid->getName().c_str());
	memset(returnName, 0, nameLength);
	memcpy(returnName, tempName, strlen(tempName)+1);
	delete [] tempName;
}

void
XdmfFortran::retrieveDomainGridTag(int gridType, int index, char * returnTag, int tagLength)
{
	shared_ptr<XdmfGrid> openedGrid;
	if (gridType == XDMF_GRID_TYPE_CURVILINEAR)
	{
		if (index < mDomain->getNumberCurvilinearGrids())
		{
			openedGrid = mDomain->getCurvilinearGrid(index);
		}
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
	else if (gridType == XDMF_GRID_TYPE_RECTILINEAR)
	{
		if (index < mDomain->getNumberRectilinearGrids())
		{
			openedGrid = mDomain->getRectilinearGrid(index);
		}
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
	else if (gridType == XDMF_GRID_TYPE_REGULAR)
	{
		if (index < mDomain->getNumberRegularGrids())
		{
			openedGrid = mDomain->getRegularGrid(index);
		}
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
	else if (gridType == XDMF_GRID_TYPE_UNSTRUCTURED)
	{
		if (index < mDomain->getNumberUnstructuredGrids())
		{
			openedGrid = mDomain->getUnstructuredGrid(index);
		}
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Invalid Grid Type.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
	char * tempTag = strdup(openedGrid->getItemTag().c_str());
	memset(returnTag, 0, tagLength);
	memcpy(returnTag, tempTag, strlen(tempTag)+1);
	delete [] tempTag;
}

int
XdmfFortran::retrieveDomainGridNumProperties(int gridType, int index)
{
	if (gridType == XDMF_GRID_TYPE_CURVILINEAR)
	{
		if (index < mDomain->getNumberCurvilinearGrids())
		{
			return mDomain->getCurvilinearGrid(index)->getItemProperties().size();
		}
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
	else if (gridType == XDMF_GRID_TYPE_RECTILINEAR)
	{
		if (index < mDomain->getNumberRectilinearGrids())
		{
			return mDomain->getRectilinearGrid(index)->getItemProperties().size();
		}
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
	else if (gridType == XDMF_GRID_TYPE_REGULAR)
	{
		if (index < mDomain->getNumberRegularGrids())
		{
			return mDomain->getRegularGrid(index)->getItemProperties().size();
		}
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
	else if (gridType == XDMF_GRID_TYPE_UNSTRUCTURED)
	{
		if (index < mDomain->getNumberUnstructuredGrids())
		{
			return mDomain->getUnstructuredGrid(index)->getItemProperties().size();
		}
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Invalid Grid Type.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

void
XdmfFortran::retrieveDomainGridProperty(int gridType, int gridIndex, int index, char * key, int keyLength, char * value, int valueLength)
{
	shared_ptr<XdmfGrid> openedGrid;
	if (gridType == XDMF_GRID_TYPE_CURVILINEAR)
	{
		if (index < mDomain->getNumberCurvilinearGrids())
		{
			openedGrid = mDomain->getCurvilinearGrid(index);
		}
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
	else if (gridType == XDMF_GRID_TYPE_RECTILINEAR)
	{
		if (index < mDomain->getNumberRectilinearGrids())
		{
			openedGrid = mDomain->getRectilinearGrid(index);
		}
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
	else if (gridType == XDMF_GRID_TYPE_REGULAR)
	{
		if (index < mDomain->getNumberRegularGrids())
		{
			openedGrid = mDomain->getRegularGrid(index);
		}
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
	else if (gridType == XDMF_GRID_TYPE_UNSTRUCTURED)
	{
		if (index < mDomain->getNumberUnstructuredGrids())
		{
			openedGrid = mDomain->getUnstructuredGrid(index);
		}
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Invalid Grid Type.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
	if (index < openedGrid->getItemProperties().size())
	{
		std::map<std::string, std::string>::iterator walker = openedGrid->getItemProperties().begin();
		for (int i = 0; i<index; i++)
		{
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
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

void
XdmfFortran::retrieveDomainGridPropertyByKey(int gridType, int index, char * key, char * value, int valueLength)
{
	shared_ptr<XdmfGrid> openedGrid;
	if (gridType == XDMF_GRID_TYPE_CURVILINEAR)
	{
		if (index < mDomain->getNumberCurvilinearGrids())
		{
			openedGrid = mDomain->getCurvilinearGrid(index);
		}
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
	else if (gridType == XDMF_GRID_TYPE_RECTILINEAR)
	{
		if (index < mDomain->getNumberRectilinearGrids())
		{
			openedGrid = mDomain->getRectilinearGrid(index);
		}
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
	else if (gridType == XDMF_GRID_TYPE_REGULAR)
	{
		if (index < mDomain->getNumberRegularGrids())
		{
			openedGrid = mDomain->getRegularGrid(index);
		}
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
	else if (gridType == XDMF_GRID_TYPE_UNSTRUCTURED)
	{
		if (index < mDomain->getNumberUnstructuredGrids())
		{
			openedGrid = mDomain->getUnstructuredGrid(index);
		}
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Invalid Grid Type.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
	std::string tempString = key;
	memset(value, 0, valueLength);
	if ((openedGrid->getItemProperties().count(tempString))>0)
	{
		char * tempValue = strdup(openedGrid->getItemProperties()[tempString].c_str());
		memcpy(value, tempValue, strlen(tempValue)+1);
		delete [] tempValue;
	}
}

void
XdmfFortran::openGridCollectionGrid(int gridType, int index, int openMaps, int openAttributes, int openInformation, int openSets)
{
	if (!mGridCollections.empty())
	{
		if (gridType == XDMF_GRID_TYPE_CURVILINEAR)
		{
			if (index < mGridCollections.top()->getNumberCurvilinearGrids())
			{
				shared_ptr<XdmfCurvilinearGrid> openedGrid = mGridCollections.top()->getCurvilinearGrid(index);
				shared_ptr<const XdmfArrayType> dataType;
				mGeometry = openedGrid->getGeometry();
				mDimensions = openedGrid->getDimensions();
				mTime = openedGrid->getTime();
				int i;
				int n;
				if (openAttributes == 1)
				{
					n = openedGrid->getNumberAttributes();
					shared_ptr<XdmfAttribute> openedAttribute;
					for (i = 0; i < n; i++)
					{
						openedAttribute = openedGrid->getAttribute(i);
						mAttributes.push_back(openedAttribute);
						mPreviousAttributes.push_back(openedAttribute);
					}
				}
				if (openMaps == 1)
				{
					n = openedGrid->getNumberMaps();
					shared_ptr<XdmfMap> openedMap;
					for (i = 0; i < n; i++)
					{
						openedMap = openedGrid->getMap(i);
						mMaps.push_back(openedMap);
						mPreviousMaps.push_back(openedMap);
					}
				}
				if (openInformation == 1)
				{
					n = openedGrid->getNumberInformations();
					shared_ptr<XdmfInformation> openedInformation;
					for (i = 0; i < n; i++)
					{
						openedInformation = openedGrid->getInformation(i);
						mInformations.push_back(openedInformation);
						mPreviousInformations.push_back(openedInformation);
					}
				}
				if (openSets == 1)
				{
					n = openedGrid->getNumberSets();
					shared_ptr<XdmfSet> openedSet;
					for (i = 0; i < n; i++)
					{
						openedSet = openedGrid->getSet(i);
						mSets.push_back(openedSet);
					}
				}
			}
			else
			{
				try
				{
					XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
				}
				catch (XdmfError e)
				{
					throw e;
				}
			}
		}
		else if (gridType == XDMF_GRID_TYPE_RECTILINEAR)
		{
			if (index < mGridCollections.top()->getNumberRectilinearGrids())
			{
				shared_ptr<XdmfRectilinearGrid> openedGrid = mGridCollections.top()->getRectilinearGrid(index);
				shared_ptr<const XdmfArrayType> dataType;
				mCoordinates = openedGrid->getCoordinates();
				mTime = openedGrid->getTime();
				int i;
				int n;
				if (openAttributes == 1)
				{
					n = openedGrid->getNumberAttributes();
					shared_ptr<XdmfAttribute> openedAttribute;
					for (i = 0; i < n; i++)
					{
						openedAttribute = openedGrid->getAttribute(i);
						mAttributes.push_back(openedAttribute);
						mPreviousAttributes.push_back(openedAttribute);
					}
				}
				if (openMaps == 1)
				{
					n = openedGrid->getNumberMaps();
					shared_ptr<XdmfMap> openedMap;
					for (i = 0; i < n; i++)
					{
						openedMap = openedGrid->getMap(i);
						mMaps.push_back(openedMap);
						mPreviousMaps.push_back(openedMap);
					}
				}
				if (openInformation == 1)
				{
					n = openedGrid->getNumberInformations();
					shared_ptr<XdmfInformation> openedInformation;
					for (i = 0; i < n; i++)
					{
						openedInformation = openedGrid->getInformation(i);
						mInformations.push_back(openedInformation);
						mPreviousInformations.push_back(openedInformation);
					}
				}
				if (openSets == 1)
				{
					n = openedGrid->getNumberSets();
					shared_ptr<XdmfSet> openedSet;
					for (i = 0; i < n; i++)
					{
						openedSet = openedGrid->getSet(i);
						mSets.push_back(openedSet);
					}
				}
			}
			else
			{
				try
				{
					XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
				}
				catch (XdmfError e)
				{
					throw e;
				}
			}
		}
		else if (gridType == XDMF_GRID_TYPE_REGULAR)
		{
			if (index < mGridCollections.top()->getNumberRegularGrids())
			{
				shared_ptr<XdmfRegularGrid> openedGrid = mGridCollections.top()->getRegularGrid(index);
				shared_ptr<const XdmfArrayType> dataType;
				mTime = openedGrid->getTime();
				mBrick = openedGrid->getBrickSize();
				mOrigin = openedGrid->getOrigin();
				mDimensions = openedGrid->getDimensions();
				int i;
				int n;
				if (openAttributes == 1)
				{
					n = openedGrid->getNumberAttributes();
					shared_ptr<XdmfAttribute> openedAttribute;
					for (i = 0; i < n; i++)
					{
						openedAttribute = openedGrid->getAttribute(i);
						mAttributes.push_back(openedAttribute);
						mPreviousAttributes.push_back(openedAttribute);
					}
				}
				if (openMaps == 1)
				{
					n = openedGrid->getNumberMaps();
					shared_ptr<XdmfMap> openedMap;
					for (i = 0; i < n; i++)
					{
						openedMap = openedGrid->getMap(i);
						mMaps.push_back(openedMap);
						mPreviousMaps.push_back(openedMap);
					}
				}
				if (openInformation == 1)
				{
					n = openedGrid->getNumberInformations();
					shared_ptr<XdmfInformation> openedInformation;
					for (i = 0; i < n; i++)
					{
						openedInformation = openedGrid->getInformation(i);
						mInformations.push_back(openedInformation);
						mPreviousInformations.push_back(openedInformation);
					}
				}
				if (openSets == 1)
				{
					n = openedGrid->getNumberSets();
					shared_ptr<XdmfSet> openedSet;
					for (i = 0; i < n; i++)
					{
						openedSet = openedGrid->getSet(i);
						mSets.push_back(openedSet);
					}
				}
			}
			else
			{
				try
				{
					XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
				}
				catch (XdmfError e)
				{
					throw e;
				}
			}
		}
		else if (gridType == XDMF_GRID_TYPE_UNSTRUCTURED)
		{
			if (index < mGridCollections.top()->getNumberUnstructuredGrids())
			{
				shared_ptr<XdmfUnstructuredGrid> openedGrid = mGridCollections.top()->getUnstructuredGrid(index);
				mTopology = openedGrid->getTopology();
				mGeometry = openedGrid->getGeometry();
				mTime = openedGrid->getTime();
				int i;
				int n;
				if (openAttributes == 1)
				{
					n = openedGrid->getNumberAttributes();
					shared_ptr<XdmfAttribute> openedAttribute;
					for (i = 0; i < n; i++)
					{
						openedAttribute = openedGrid->getAttribute(i);
						mAttributes.push_back(openedAttribute);
						mPreviousAttributes.push_back(openedAttribute);
					}
				}
				if (openMaps == 1)
				{
					n = openedGrid->getNumberMaps();
					shared_ptr<XdmfMap> openedMap;
					for (i = 0; i < n; i++)
					{
						openedMap = openedGrid->getMap(i);
						mMaps.push_back(openedMap);
						mPreviousMaps.push_back(openedMap);
					}
				}
				if (openInformation == 1)
				{
					n = openedGrid->getNumberInformations();
					shared_ptr<XdmfInformation> openedInformation;
					for (i = 0; i < n; i++)
					{
						openedInformation = openedGrid->getInformation(i);
						mInformations.push_back(openedInformation);
						mPreviousInformations.push_back(openedInformation);
					}
				}
				if (openSets == 1)
				{
					n = openedGrid->getNumberSets();
					shared_ptr<XdmfSet> openedSet;
					for (i = 0; i < n; i++)
					{
						openedSet = openedGrid->getSet(i);
						mSets.push_back(openedSet);
					}
				}
			}
			else
			{
				try
				{
					XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
				}
				catch (XdmfError e)
				{
					throw e;
				}
			}
		}
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Error: Invalid Grid Type.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"There is no grid collection currently loaded.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

void
XdmfFortran::removeGridCollectionGrid(int gridType, int index)
{	
	if (!mGridCollections.empty())
	{
		if (gridType == XDMF_GRID_TYPE_CURVILINEAR)
		{
			if (index < mGridCollections.top()->getNumberCurvilinearGrids())
			{
				mGridCollections.top()->removeCurvilinearGrid(index);
			}
			else
			{
				try
				{
					XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
				}
				catch (XdmfError e)
				{
					throw e;
				}
			}
		}
		else if (gridType == XDMF_GRID_TYPE_RECTILINEAR)
		{
			if (index < mGridCollections.top()->getNumberRectilinearGrids())
			{
				mGridCollections.top()->removeRectilinearGrid(index);
			}
			else
			{
				try
				{
					XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
				}
				catch (XdmfError e)
				{
					throw e;
				}
			}
		}
		else if (gridType == XDMF_GRID_TYPE_REGULAR)
		{
			if (index < mGridCollections.top()->getNumberRegularGrids())
			{
				mGridCollections.top()->removeRegularGrid(index);
			}
			else
			{
				try
				{
					XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
				}
				catch (XdmfError e)
				{
					throw e;
				}
			}
		}
		else if (gridType == XDMF_GRID_TYPE_UNSTRUCTURED)
		{
			if (index < mGridCollections.top()->getNumberUnstructuredGrids())
			{
				mGridCollections.top()->removeUnstructuredGrid(index);
			}
			else
			{
				try
				{
					XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
				}
				catch (XdmfError e)
				{
					throw e;
				}
			}
		}
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"There is no grid collection currently loaded.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

void
XdmfFortran::replaceGridCollectionGrid(int gridType, int index, char * name)
{
	if (!mGridCollections.empty())
	{
		int i;
		if (gridType == XDMF_GRID_TYPE_CURVILINEAR)
		{
			if (index < mGridCollections.top()->getNumberCurvilinearGrids())
			{
				if(mDimensions == NULL)
				{
					try
					{
						XdmfError::message(XdmfError::FATAL, "Must set dimensions before replacing grid.");
					}
					catch (XdmfError e)
					{
						throw e;
					}
				}
				if(mGeometry == NULL)
				{
					try
					{
						XdmfError::message(XdmfError::FATAL, 
							"Must set geometry before adding grid.");
					}
					catch (XdmfError e)
					{
						throw e;
					}
				}

				shared_ptr<XdmfCurvilinearGrid> grid = mGridCollections.top()->getCurvilinearGrid(index);
				grid->setName(name);

				grid->setGeometry(mGeometry);
				grid->setDimensions(mDimensions);

				for (i=grid->getNumberAttributes()-1;i>=0;i--)
				{
					grid->removeAttribute(0);
				}

				for(std::vector<shared_ptr<XdmfAttribute> >::const_iterator iter =
					mAttributes.begin(); 
					iter != mAttributes.end();
					++iter)
				{
					grid->insert(*iter);
				}

				mAttributes.clear();

				for( i=grid->getNumberInformations()-1;i>=0;i--)
				{
					grid->removeInformation(0);
				}

				for(std::vector<shared_ptr<XdmfInformation> >::const_iterator iter =
					mInformations.begin(); 
					iter != mInformations.end();
					++iter)
				{
					grid->insert(*iter);
				}

				mInformations.clear();

				for (i=grid->getNumberSets()-1;i>=0;i--)
				{
					grid->removeSet(0);
				}

				for(std::vector<shared_ptr<XdmfSet> >::const_iterator iter =
					mSets.begin(); 
					iter != mSets.end();
					++iter)
				{
					grid->insert(*iter);
				}

				mSets.clear();

				for (i=grid->getNumberMaps()-1;i>=0;i--)
				{
					grid->removeMap(0);
				}

				for(std::vector<shared_ptr<XdmfMap> >::const_iterator iter =
					mMaps.begin(); 
					iter != mMaps.end();
					++iter)
				{
					grid->insert(*iter);
				}

				mMaps.clear();
    
				if(mTime)
				{
					grid->setTime(mTime);
				}
			}
			else
			{
				try
				{
					XdmfError::message(XdmfError::FATAL, "Error: Index out of range."); 
				}
				catch (XdmfError e)
				{
					throw e;
				}
			}
		}
		else if (gridType == XDMF_GRID_TYPE_RECTILINEAR)
		{
			if (index < mGridCollections.top()->getNumberRectilinearGrids())
			{
				if(mCoordinates.empty())
				{
					try
					{
						XdmfError::message(XdmfError::FATAL, 
							"Must set Coordinates before adding grid.");
					}
					catch (XdmfError e)
					{
						throw e;
					}
				}

				shared_ptr<XdmfRectilinearGrid> grid = mGridCollections.top()->getRectilinearGrid(index);
				grid->setCoordinates(mCoordinates);
				mCoordinates.clear();
				for (i=grid->getNumberAttributes()-1;i>=0;i--)
				{
					grid->removeAttribute(0);
				}

				for(std::vector<shared_ptr<XdmfAttribute> >::const_iterator iter =
					mAttributes.begin(); 
					iter != mAttributes.end();
					++iter)
				{
					grid->insert(*iter);
				}

				mAttributes.clear();

				for (i=grid->getNumberInformations()-1;i>=0;i--)
				{
					grid->removeInformation(0);
				}

				for(std::vector<shared_ptr<XdmfInformation> >::const_iterator iter =
					mInformations.begin(); 
					iter != mInformations.end();
					++iter)
				{
					grid->insert(*iter);
				}

				mInformations.clear();

				for (i=grid->getNumberSets()-1;i>=0;i--)
				{
					grid->removeSet(0);
				}

				for(std::vector<shared_ptr<XdmfSet> >::const_iterator iter =
					mSets.begin(); 
					iter != mSets.end();
					++iter)
				{
					grid->insert(*iter);
				}

				mSets.clear();

				for (i=grid->getNumberMaps()-1;i>=0;i--)
				{
					grid->removeMap(0);
				}

				for(std::vector<shared_ptr<XdmfMap> >::const_iterator iter =
					mMaps.begin(); 
					iter != mMaps.end();
					++iter)
				{
					grid->insert(*iter);
				}

				mMaps.clear();
    
				if(mTime)
				{
					grid->setTime(mTime);
				}
			}
			else
			{
				try
				{
					XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
				}
				catch (XdmfError e)
				{
					throw e;
				}
			}
		}
		else if (gridType == XDMF_GRID_TYPE_REGULAR)
		{
			if (index < mGridCollections.top()->getNumberRegularGrids())
			{
				if(mBrick == NULL)
				{
					try
					{
						XdmfError::message(XdmfError::FATAL, 
							"Must set brick size before adding grid.");
					}
					catch (XdmfError e)
					{
						throw e;
					}
				}

				if(mDimensions == NULL)
				{
					try
					{
						XdmfError::message(XdmfError::FATAL, 
							"Must set dimensions before adding grid.");
					}
					catch (XdmfError e)
					{
						throw e;
					}
				}

				if(mOrigin == NULL)
				{
					try
					{
						XdmfError::message(XdmfError::FATAL, 
							"Must set origin before adding grid.");
					}
					catch (XdmfError e)
					{
						throw e;
					}
				}
  
		    		shared_ptr<XdmfRegularGrid> grid = mGridCollections.top()->getRegularGrid(index);
				grid->setOrigin(mOrigin);
				grid->setDimensions(mDimensions);
				grid->setBrickSize(mBrick);

				for (i=grid->getNumberAttributes()-1;i>=0;i--)
				{
					grid->removeAttribute(0);
				}

				for(std::vector<shared_ptr<XdmfAttribute> >::const_iterator iter =
					mAttributes.begin(); 
					iter != mAttributes.end();
					++iter)
				{
					grid->insert(*iter);
				}

				mAttributes.clear();

				for (i=grid->getNumberInformations()-1;i>=0;i--)
				{
					grid->removeInformation(0);
				}

				for(std::vector<shared_ptr<XdmfInformation> >::const_iterator iter =
					mInformations.begin(); 
					iter != mInformations.end();
					++iter)
				{
					grid->insert(*iter);
				}

				mInformations.clear();

				for (i=grid->getNumberSets()-1;i>=0;i--)
				{
					grid->removeSet(0);
				}

				for(std::vector<shared_ptr<XdmfSet> >::const_iterator iter =
					mSets.begin(); 
					iter != mSets.end();
					++iter)
				{
					grid->insert(*iter);
				}

				mSets.clear();

				for (i=grid->getNumberMaps()-1;i>=0;i--)
				{
					grid->removeMap(0);
				}

				for(std::vector<shared_ptr<XdmfMap> >::const_iterator iter =
					mMaps.begin(); 
					iter != mMaps.end();
					++iter)
				{
					grid->insert(*iter);
				}

				mMaps.clear();
    
				if(mTime)
				{
					grid->setTime(mTime);
				}
			}
			else
			{
				try
				{
					XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
				}
				catch (XdmfError e)
				{
					throw e;
				}
			}
		}
		else if (gridType == XDMF_GRID_TYPE_UNSTRUCTURED)
		{
			if (index < mGridCollections.top()->getNumberUnstructuredGrids())
			{
				if(mGeometry == NULL)
				{
					try
					{
						XdmfError::message(XdmfError::FATAL, 
							"Must set geometry before adding grid.");
					}
					catch (XdmfError e)
					{
						throw e;
					}
				}
				if(mTopology == NULL)
				{
					try
					{
						XdmfError::message(XdmfError::FATAL, 
							"Must set topology before adding grid.");
					}
					catch (XdmfError e)
					{
						throw e;
					}
				}
				shared_ptr<XdmfUnstructuredGrid> grid = mGridCollections.top()->getUnstructuredGrid(index);
				grid->setName(name);
				grid->setGeometry(mGeometry);
				grid->setTopology(mTopology);

				for (i=grid->getNumberAttributes()-1;i>=0;i--)
				{
					grid->removeAttribute(0);
				}

				for(std::vector<shared_ptr<XdmfAttribute> >::const_iterator iter =
					mAttributes.begin(); 
					iter != mAttributes.end();
					++iter)
				{
					grid->insert(*iter);
				}

				mAttributes.clear();

				for (i=grid->getNumberInformations()-1;i>=0;i--)
				{
					grid->removeInformation(0);
				}

				for(std::vector<shared_ptr<XdmfInformation> >::const_iterator iter =
					mInformations.begin(); 
					iter != mInformations.end();
					++iter)
				{
					grid->insert(*iter);
				}

				mInformations.clear();

				for (i=grid->getNumberSets()-1;i>=0;i--)
				{
					grid->removeSet(0);
				}

				for(std::vector<shared_ptr<XdmfSet> >::const_iterator iter =
					mSets.begin(); 
					iter != mSets.end();
					++iter)
				{
					grid->insert(*iter);
				}

				mSets.clear();

				for (i=grid->getNumberMaps()-1;i>=0;i--)
				{
					grid->removeMap(0);
				}

				for(std::vector<shared_ptr<XdmfMap> >::const_iterator iter =
					mMaps.begin(); 
					iter != mMaps.end();
					++iter)
				{
					grid->insert(*iter);
				}

				mMaps.clear();
    
				if(mTime)
				{
					grid->setTime(mTime);
				}
			}
			else
			{
				try
				{
					XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
				}
				catch (XdmfError e)
				{
					throw e;
				}
			}
		}
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"There is no grid collection currently loaded.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

void
XdmfFortran::retrieveGridCollectionGridName(int gridType, int index, char * returnName, int nameLength)
{
	if (!mGridCollections.empty())
	{
		shared_ptr<XdmfGrid> openedGrid;
		if (gridType == XDMF_GRID_TYPE_CURVILINEAR)
		{
			if (index < mGridCollections.top()->getNumberCurvilinearGrids())
			{
				openedGrid = mGridCollections.top()->getCurvilinearGrid(index);
			}
			else
			{
				try
				{
					XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
				}
				catch (XdmfError e)
				{
					throw e;
				}
			}
		}
		else if (gridType == XDMF_GRID_TYPE_RECTILINEAR)
		{
			if (index < mGridCollections.top()->getNumberRectilinearGrids())
			{
				openedGrid = mGridCollections.top()->getRectilinearGrid(index);
			}
			else
			{
				try
				{
					XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
				}
				catch (XdmfError e)
				{
					throw e;
				}
			}
		}
		else if (gridType == XDMF_GRID_TYPE_REGULAR)
		{
			if (index < mGridCollections.top()->getNumberRegularGrids())
			{
				openedGrid = mGridCollections.top()->getRegularGrid(index);
			}
			else
			{
				try
				{
					XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
				}
				catch (XdmfError e)
				{
					throw e;
				}
			}
		}
		else if (gridType == XDMF_GRID_TYPE_UNSTRUCTURED)
		{
			if (index < mGridCollections.top()->getNumberUnstructuredGrids())
			{
				openedGrid = mGridCollections.top()->getUnstructuredGrid(index);
			}
			else
			{
				try
				{
					XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
				}
				catch (XdmfError e)
				{
					throw e;
				}
			}
		}
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Error: Invalid Grid Type.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
		char * tempName = strdup(openedGrid->getName().c_str());
		memset(returnName, 0, nameLength);
		memcpy(returnName, tempName, strlen(tempName)+1);
		delete [] tempName;
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"There is no grid collection currently loaded.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

void
XdmfFortran::retrieveGridCollectionGridTag(int gridType, int index, char * returnTag, int tagLength)
{
	if (!mGridCollections.empty())
	{
		shared_ptr<XdmfGrid> openedGrid;
		if (gridType == XDMF_GRID_TYPE_CURVILINEAR)
		{
			if (index < mGridCollections.top()->getNumberCurvilinearGrids())
			{
				openedGrid = mGridCollections.top()->getCurvilinearGrid(index);
			}
			else
			{
				try
				{
					XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
				}
				catch (XdmfError e)
				{
					throw e;
				}
			}
		}
		else if (gridType == XDMF_GRID_TYPE_RECTILINEAR)
		{
			if (index < mGridCollections.top()->getNumberRectilinearGrids())
			{
				openedGrid = mGridCollections.top()->getRectilinearGrid(index);
			}
			else
			{
				try
				{
					XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
				}
				catch (XdmfError e)
				{
					throw e;
				}
			}
		}
		else if (gridType == XDMF_GRID_TYPE_REGULAR)
		{
			if (index < mGridCollections.top()->getNumberRegularGrids())
			{
				openedGrid = mGridCollections.top()->getRegularGrid(index);
			}
			else
			{
				try
				{
					XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
				}
				catch (XdmfError e)
				{
					throw e;
				}
			}
		}
		else if (gridType == XDMF_GRID_TYPE_UNSTRUCTURED)
		{
			if (index < mGridCollections.top()->getNumberUnstructuredGrids())
			{
				openedGrid = mGridCollections.top()->getUnstructuredGrid(index);
			}
			else
			{
				try
				{
					XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
				}
				catch (XdmfError e)
				{
					throw e;
				}
			}
		}
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Error: Invalid Grid Type.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
		char * tempTag = strdup(openedGrid->getItemTag().c_str());
		memset(returnTag, 0, tagLength);
		memcpy(returnTag, tempTag, strlen(tempTag)+1);
		delete [] tempTag;
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"There is no grid collection currently loaded.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

int
XdmfFortran::retrieveGridCollectionGridNumProperties(int gridType, int index)
{
	if (!mGridCollections.empty())
	{
		if (gridType == XDMF_GRID_TYPE_CURVILINEAR)
		{
			if (index < mGridCollections.top()->getNumberCurvilinearGrids())
			{
				return mGridCollections.top()->getCurvilinearGrid(index)->getItemProperties().size();
			}
			else
			{
				try
				{
					XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
				}
				catch (XdmfError e)
				{
					throw e;
				}
			}
		}
		else if (gridType == XDMF_GRID_TYPE_RECTILINEAR)
		{
			if (index < mGridCollections.top()->getNumberRectilinearGrids())
			{
				return mGridCollections.top()->getRectilinearGrid(index)->getItemProperties().size();
			}
			else
			{
				try
				{
					XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
				}
				catch (XdmfError e)
				{
					throw e;
				}
			}
		}
		else if (gridType == XDMF_GRID_TYPE_REGULAR)
		{
			if (index < mGridCollections.top()->getNumberRegularGrids())
			{
				return mGridCollections.top()->getRegularGrid(index)->getItemProperties().size();
			}
			else
			{
				try
				{
					XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
				}
				catch (XdmfError e)
				{
					throw e;
				}
			}
		}
		else if (gridType == XDMF_GRID_TYPE_UNSTRUCTURED)
		{
			if (index < mGridCollections.top()->getNumberUnstructuredGrids())
			{
				return mGridCollections.top()->getUnstructuredGrid(index)->getItemProperties().size();
			}
			else
			{
				try
				{
					XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
				}
				catch (XdmfError e)
				{
					throw e;
				}
			}
		}
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Error: Invalid Grid Type.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"There is no grid collection currently loaded.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

void
XdmfFortran::retrieveGridCollectionGridProperty(int gridType, int gridIndex, int index,
	char * key, int keyLength, char * value, int valueLength)
{
	if (!mGridCollections.empty())
	{
		shared_ptr<XdmfGrid> openedGrid;
		if (gridType == XDMF_GRID_TYPE_CURVILINEAR)
		{
			if (index < mGridCollections.top()->getNumberCurvilinearGrids())
			{
				openedGrid = mGridCollections.top()->getCurvilinearGrid(index);
			}
			else
			{
				try
				{
					XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
				}
				catch (XdmfError e)
				{
					throw e;
				}
			}
		}
		else if (gridType == XDMF_GRID_TYPE_RECTILINEAR)
		{
			if (index < mGridCollections.top()->getNumberRectilinearGrids())
			{
				openedGrid = mGridCollections.top()->getRectilinearGrid(index);
			}
			else
			{
				try
				{
					XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
				}
				catch (XdmfError e)
				{
					throw e;
				}
			}
		}
		else if (gridType == XDMF_GRID_TYPE_REGULAR)
		{
			if (index < mGridCollections.top()->getNumberRegularGrids())
			{
				openedGrid = mGridCollections.top()->getRegularGrid(index);
			}
			else
			{
				try
				{
					XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
				}
				catch (XdmfError e)
				{
					throw e;
				}
			}
		}
		else if (gridType == XDMF_GRID_TYPE_UNSTRUCTURED)
		{
			if (index < mGridCollections.top()->getNumberUnstructuredGrids())
			{
				openedGrid = mGridCollections.top()->getUnstructuredGrid(index);
			}
			else
			{
				try
				{
					XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
				}
				catch (XdmfError e)
				{
					throw e;
				}
			}
		}
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Error: Invalid Grid Type.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
		if (index < openedGrid->getItemProperties().size())
		{
			std::map<std::string, std::string>::iterator walker = openedGrid->getItemProperties().begin();
			for (int i = 0; i<index; i++)
			{
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
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"There is no grid collection currently loaded.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

void
XdmfFortran::retrieveGridCollectionGridPropertyByKey(int gridType, int index, char * key, char * value, int valueLength)
{
	if (!mGridCollections.empty())
	{
		shared_ptr<XdmfGrid> openedGrid;
		if (gridType == XDMF_GRID_TYPE_CURVILINEAR)
		{
			if (index < mGridCollections.top()->getNumberCurvilinearGrids())
			{
				openedGrid = mGridCollections.top()->getCurvilinearGrid(index);
			}
			else
			{
				try
				{
					XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
				}
				catch (XdmfError e)
				{
					throw e;
				}
			}
		}
		else if (gridType == XDMF_GRID_TYPE_RECTILINEAR)
		{
			if (index < mGridCollections.top()->getNumberRectilinearGrids())
			{
				openedGrid = mGridCollections.top()->getRectilinearGrid(index);
			}
			else
			{
				try
				{
					XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
				}
				catch (XdmfError e)
				{
					throw e;
				}
			}
		}
		else if (gridType == XDMF_GRID_TYPE_REGULAR)
		{
			if (index < mGridCollections.top()->getNumberRegularGrids())
			{
				openedGrid = mGridCollections.top()->getRegularGrid(index);
			}
			else
			{
				try
				{
					XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
				}
				catch (XdmfError e)
				{
					throw e;
				}
			}
		}
		else if (gridType == XDMF_GRID_TYPE_UNSTRUCTURED)
		{
			if (index < mGridCollections.top()->getNumberUnstructuredGrids())
			{
				openedGrid = mGridCollections.top()->getUnstructuredGrid(index);
			}
			else
			{
				try
				{
					XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
				}
				catch (XdmfError e)
				{
					throw e;
				}
			}
		}
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Error: Invalid Grid Type.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
		std::string tempString = key;
		memset(value, 0, valueLength);
		if ((openedGrid->getItemProperties().count(tempString))>0)
		{
			char * tempValue = strdup(openedGrid->getItemProperties()[tempString].c_str());
			memcpy(value, tempValue, strlen(tempValue)+1);
			delete [] tempValue;
		}
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"There is no grid collection currently loaded.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

int
XdmfFortran::numDomainGrids(int gridType)
{
	if (gridType == XDMF_GRID_TYPE_CURVILINEAR)
	{
		return mDomain->getNumberCurvilinearGrids();
	}
	else if (gridType == XDMF_GRID_TYPE_RECTILINEAR)
	{
		return mDomain->getNumberRectilinearGrids();
	}
	else if (gridType == XDMF_GRID_TYPE_REGULAR)
	{
		return mDomain->getNumberRegularGrids();
	}
	else if (gridType == XDMF_GRID_TYPE_UNSTRUCTURED)
	{
		return mDomain->getNumberUnstructuredGrids();
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Invalid Grid Type.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

int
XdmfFortran::numGridCollectionGrids(int gridType)
{
	if (!mGridCollections.empty())
	{
		if (gridType == XDMF_GRID_TYPE_CURVILINEAR)
		{
			return mGridCollections.top()->getNumberCurvilinearGrids();
		}
		else if (gridType == XDMF_GRID_TYPE_RECTILINEAR)
		{
			return mGridCollections.top()->getNumberRectilinearGrids();
		}
		else if (gridType == XDMF_GRID_TYPE_REGULAR)
		{
			return mGridCollections.top()->getNumberRegularGrids();
		}
		else if (gridType == XDMF_GRID_TYPE_UNSTRUCTURED)
		{
			return mGridCollections.top()->getNumberUnstructuredGrids();
		}
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Error: Invalid Grid Type."); 
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"Error: No Grid Collections have been loaded.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}
int
XdmfFortran::retrieveGridCollectionType()
{
	if (!mGridCollections.empty())
	{
		shared_ptr<const XdmfGridCollectionType> returnType = mGridCollections.top()->getType();
		if (returnType == XdmfGridCollectionType::Spatial())
		{
			return XDMF_GRID_COLLECTION_TYPE_SPATIAL;
		}
		else if (returnType == XdmfGridCollectionType::Temporal())
		{
			return XDMF_GRID_COLLECTION_TYPE_TEMPORAL;
		}
		else if (returnType == XdmfGridCollectionType::NoCollectionType())
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, 
					"Error: No Grid Collection Type.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, 
					"Invalid Grid Collection Type.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"There is no grid collection currently loaded.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

float
XdmfFortran::retrieveTime()
{
	return static_cast<float>(mTime->getValue());
}

void
XdmfFortran::retrieveGeometryTag(char * tag, int tagLength)
{
	if (mGeometry == NULL)
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"Must set geometry before its tag can be retrieved.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
	else
	{
		char * tempTag = strdup(mGeometry->getItemTag().c_str());
		memset(tag, 0, tagLength);
		memcpy(tag, tempTag, strlen(tempTag)+1);
		delete [] tempTag;
	}
}

int
XdmfFortran::retrieveGeometryType()
{
	if (mGeometry == NULL)
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"Must set geometry before its type can be retrieved.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
	else
	{
		shared_ptr<const XdmfGeometryType> returnType = mGeometry->getType();
		if (returnType == XdmfGeometryType::XY())
		{
			return XDMF_GEOMETRY_TYPE_XY;
		}
		else if (returnType == XdmfGeometryType::XYZ())
		{
			return XDMF_GEOMETRY_TYPE_XYZ;
		}
		else if (returnType == XdmfGeometryType::NoGeometryType())
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Uninitialized geometry type");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Invalid geometry type");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
}

int
XdmfFortran::retrieveGeometryValueType()
{
	if (mGeometry == NULL)
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"Must set geometry before its value type can be retrieved.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
	else
	{
		shared_ptr<const XdmfArrayType> dataType = mGeometry->getArrayType();
		if (dataType == XdmfArrayType::Int8())
		{
			return XDMF_ARRAY_TYPE_INT8;
		}
		else if (dataType == XdmfArrayType::Int16())
		{
			return XDMF_ARRAY_TYPE_INT16;
		}
		else if (dataType == XdmfArrayType::Int32())
		{
			return XDMF_ARRAY_TYPE_INT32;
		}
		else if (dataType == XdmfArrayType::Int64())
		{
			return XDMF_ARRAY_TYPE_INT64;
		}
		else if (dataType == XdmfArrayType::UInt8())
		{
			return XDMF_ARRAY_TYPE_UINT8;
		}
		else if (dataType == XdmfArrayType::UInt16())
		{
			return XDMF_ARRAY_TYPE_UINT16;
		}
		else if (dataType == XdmfArrayType::UInt32())
		{
			return XDMF_ARRAY_TYPE_UINT32;
		}
		else if (dataType == XdmfArrayType::Float32())
		{
			return XDMF_ARRAY_TYPE_FLOAT32;
		}
		else if (dataType == XdmfArrayType::Float64())
		{
			return XDMF_ARRAY_TYPE_FLOAT64;
		}
		else if (dataType == XdmfArrayType::Uninitialized())
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, 
					"Uninitialized Geometry Data Type.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, 
					"Invalid Geometry Data Type.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
}

void
XdmfFortran::retrieveGeometryValues(void * values, int dataType, int numberRead, int startIndex, int arrayStride, int valueStride)
{
	if(mGeometry == NULL)
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"Must set geometry before its values can be retrieved.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
	else
	{
		if (!mGeometry->isInitialized())
		{
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
	if (mGeometry == NULL)
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"Must set geometry before the number of points it contains can be retrieved.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
	else
	{
		return mGeometry->getNumberPoints();
	}
}

int
XdmfFortran::retrieveGeometrySize()
{
	if (mGeometry == NULL)
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"Must set geometry before its size can be retrieved.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
	else
	{
		return mGeometry->getSize();
	}
}

void
XdmfFortran::clearPreviousGeometries()
{
	mPreviousGeometries.clear();
}

int
XdmfFortran::retrieveGeometryNumProperties()
{
	if (mGeometry == NULL)
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"Must set geometry before the number of properties it contains can be retrieved.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
	else
	{
		return mGeometry->getItemProperties().size();
	}	
}

void
XdmfFortran::retrieveGeometryProperty(int index, char * key, int keyLength, char * value, int valueLength)
{
	if (mGeometry == NULL)
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"Must set geometry before the properties it contains can be retrieved.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
	else
	{
		if (index < mGeometry->getItemProperties().size())
		{
			std::map<std::string, std::string>::iterator walker = mGeometry->getItemProperties().begin();
			for (int i = 0; i<index; i++)
			{
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
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
}

void
XdmfFortran::retrieveGeometryPropertyByKey(char * key, char * value, int valueLength)
{
	if (mGeometry == NULL)
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"Must set geometry before the properties it contains can be retrieved.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
	else
	{
		std::string tempString = key;
		memset(value, 0, valueLength);
		if ((mGeometry->getItemProperties().count(tempString))>0)
		{
			char * tempValue = strdup(mGeometry->getItemProperties()[tempString].c_str());
			memcpy(value, tempValue, strlen(tempValue)+1);
			delete [] tempValue;
		}
	}
}

void
XdmfFortran::retrieveTopologyTag(char * tag, int tagLength)
{
	if (mTopology == NULL)
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"Must set Topology before its tag can be retrieved.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
	else
	{
		char * tempTag = strdup(mTopology->getItemTag().c_str());
		memset(tag, 0, tagLength);
		memcpy(tag, tempTag, strlen(tempTag)+1);
		delete [] tempTag;
	}
}

int
XdmfFortran::retrieveTopologyType()
{
	if (mTopology == NULL)
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"Must set Topology before its type can be retrieved.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
	else
	{
		shared_ptr<const XdmfTopologyType> returnType =  mTopology->getType();
		if (returnType ==  XdmfTopologyType::Polyvertex())
		{
			return XDMF_TOPOLOGY_TYPE_POLYVERTEX;
		}
		else if (returnType == XdmfTopologyType::Polyline(0))
		{
			return XDMF_TOPOLOGY_TYPE_POLYLINE;
		}
		else if (returnType == XdmfTopologyType::Polygon(0))
		{
			return XDMF_TOPOLOGY_TYPE_POLYGON;
		}
		else if (returnType == XdmfTopologyType::Triangle())
		{
			return XDMF_TOPOLOGY_TYPE_TRIANGLE;
		}
		else if (returnType == XdmfTopologyType::Quadrilateral())
		{
			return XDMF_TOPOLOGY_TYPE_QUADRILATERAL;
		}
		else if (returnType == XdmfTopologyType::Tetrahedron())
		{
			return XDMF_TOPOLOGY_TYPE_TETRAHEDRON;
		}
		else if (returnType == XdmfTopologyType::Pyramid())
		{
			return XDMF_TOPOLOGY_TYPE_PYRAMID;
		}
		else if (returnType == XdmfTopologyType::Wedge())
		{
			return XDMF_TOPOLOGY_TYPE_WEDGE;
		}
		else if (returnType == XdmfTopologyType::Hexahedron())
		{
			return XDMF_TOPOLOGY_TYPE_HEXAHEDRON;
		}
		else if (returnType == XdmfTopologyType::Edge_3())
		{
			return XDMF_TOPOLOGY_TYPE_EDGE_3;
		}
		else if (returnType == XdmfTopologyType::Triangle_6())
		{
			return XDMF_TOPOLOGY_TYPE_TRIANGLE_6;
		}
		else if (returnType == XdmfTopologyType::Quadrilateral_8())
		{
			return XDMF_TOPOLOGY_TYPE_QUADRILATERAL_8;
		}
		else if (returnType == XdmfTopologyType::Quadrilateral_9())
		{
			return XDMF_TOPOLOGY_TYPE_QUADRILATERAL_9;
		}
		else if (returnType == XdmfTopologyType::Tetrahedron_10())
		{
			return XDMF_TOPOLOGY_TYPE_TETRAHEDRON_10;
		}
		else if (returnType == XdmfTopologyType::Pyramid_13())
		{
			return XDMF_TOPOLOGY_TYPE_PYRAMID_13;
		}
		else if (returnType == XdmfTopologyType::Wedge_15())
		{
			return XDMF_TOPOLOGY_TYPE_WEDGE_15;
		}
		else if (returnType == XdmfTopologyType::Wedge_18())
		{
			return XDMF_TOPOLOGY_TYPE_WEDGE_18;
		}
		else if (returnType == XdmfTopologyType::Hexahedron_20())
		{
			return XDMF_TOPOLOGY_TYPE_HEXAHEDRON_20;
		}
		else if (returnType == XdmfTopologyType::Hexahedron_24())
		{
			return XDMF_TOPOLOGY_TYPE_HEXAHEDRON_24;
		}
		else if (returnType == XdmfTopologyType::Hexahedron_27())
		{
			return XDMF_TOPOLOGY_TYPE_HEXAHEDRON_27;
		}
		else if (returnType == XdmfTopologyType::Hexahedron_64())
		{
			return XDMF_TOPOLOGY_TYPE_HEXAHEDRON_64;
		}
		else if (returnType == XdmfTopologyType::Hexahedron_125())
		{
			return XDMF_TOPOLOGY_TYPE_HEXAHEDRON_125;
		}
		else if (returnType == XdmfTopologyType::Hexahedron_216())
		{
			return XDMF_TOPOLOGY_TYPE_HEXAHEDRON_216;
		}
		else if (returnType == XdmfTopologyType::Hexahedron_343())
		{
			return XDMF_TOPOLOGY_TYPE_HEXAHEDRON_343;
		}
		else if (returnType == XdmfTopologyType::Hexahedron_512())
		{
			return XDMF_TOPOLOGY_TYPE_HEXAHEDRON_512;
		}
		else if (returnType == XdmfTopologyType::Hexahedron_729())
		{
			return XDMF_TOPOLOGY_TYPE_HEXAHEDRON_729;
		}
		else if (returnType == XdmfTopologyType::Hexahedron_1000())
		{
			return XDMF_TOPOLOGY_TYPE_HEXAHEDRON_1000;
		}
		else if (returnType == XdmfTopologyType::Hexahedron_1331())
		{
			return XDMF_TOPOLOGY_TYPE_HEXAHEDRON_1331;
		}
		else if (returnType == XdmfTopologyType::Mixed())
		{
			return XDMF_TOPOLOGY_TYPE_MIXED;
		}
		else if (returnType == XdmfTopologyType::NoTopologyType())
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Uninitialized topology type.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Invalid topology type.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
}

int
XdmfFortran::retrieveTopologyValueType()
{
	if (mTopology == NULL)
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"Must set Topology before its value type can be retrieved.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
	else
	{		
		shared_ptr<const XdmfArrayType> dataType = mTopology->getArrayType();
		if (dataType == XdmfArrayType::Int8())
		{
			return XDMF_ARRAY_TYPE_INT8;
		}
		else if (dataType == XdmfArrayType::Int16())
		{
			return XDMF_ARRAY_TYPE_INT16;
		}
		else if (dataType == XdmfArrayType::Int32())
		{
			return XDMF_ARRAY_TYPE_INT32;
		}
		else if (dataType == XdmfArrayType::Int64())
		{
			return XDMF_ARRAY_TYPE_INT64;
		}
		else if (dataType == XdmfArrayType::UInt8())
		{
			return XDMF_ARRAY_TYPE_UINT8;
		}
		else if (dataType == XdmfArrayType::UInt16())
		{
			return XDMF_ARRAY_TYPE_UINT16;
		}
		else if (dataType == XdmfArrayType::UInt32())
		{
			return XDMF_ARRAY_TYPE_UINT32;
		}
		else if (dataType == XdmfArrayType::Float32())
		{
			return XDMF_ARRAY_TYPE_FLOAT32;
		}
		else if (dataType == XdmfArrayType::Float64())
		{
			return XDMF_ARRAY_TYPE_FLOAT64;
		}
		else if (dataType == XdmfArrayType::Uninitialized())
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, 
					"Uninitialized Topology Data Type.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, 
					"Invalid Topology Data Type.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
}

void
XdmfFortran::retrieveTopologyValues(void * values, int dataType, int numberRead, int startIndex, int arrayStride, int valueStride)
{
	if(mTopology == NULL)
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"Must set topology before its values can be retrieved.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
	else
	{
		if (!mTopology->isInitialized())
		{
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
	if (mTopology == NULL)
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"Must set Topology before the number of Elements it contains can be retrieved.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
	else
	{
		return mTopology->getNumberElements();
	}
}

int
XdmfFortran::retrieveTopologySize()
{
	if (mTopology == NULL)
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"Must set Topology before its size can be retrieved.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
	else
	{
		return mTopology->getSize();
	}
}

void
XdmfFortran::clearPreviousTopologies()
{
	mPreviousTopologies.clear();
}

int
XdmfFortran::retrieveTopologyNumProperties()
{
	if (mTopology == NULL)
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"Must set Topology before the number of properties it contains can be retrieved.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
	else
	{
		return mTopology->getItemProperties().size();
	}	
}

void
XdmfFortran::retrieveTopologyProperty(int index, char * key, int keyLength, char * value, int valueLength)
{
	if (mTopology == NULL)
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"Must set Topology before the properties it contains can be retrieved.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
	else
	{
		if (index < mTopology->getItemProperties().size())
		{
			std::map<std::string, std::string>::iterator walker = mTopology->getItemProperties().begin();
			for (int i = 0; i<index; i++)
			{
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
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
}

void
XdmfFortran::retrieveTopologyPropertyByKey(char * key, char * value, int valueLength)
{
	if (mTopology == NULL)
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"Must set Topology before the properties it contains can be retrieved.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
	else
	{
		std::string tempString = key;
		memset(value, 0, valueLength);
		if ((mTopology->getItemProperties().count(tempString))>0)
		{
			char * tempValue = strdup(mTopology->getItemProperties()[tempString].c_str());
			memcpy(value, tempValue, strlen(tempValue)+1);
			delete [] tempValue;
		}
	}
}

int
XdmfFortran::setDimensions(int numValues, int arrayType, void * pointValues)
{
  mDimensions = XdmfArray::New();

  // insert dimension values into array
  writeToArray(mDimensions, numValues, arrayType, pointValues);

  int id = mPreviousDimensions.size();
  mPreviousDimensions.push_back(mDimensions);
  return id;
}

void
XdmfFortran::openPreviousDimensions(int index)
{
	if (mPreviousDimensions.size()>index)
	{
		mDimensions = mPreviousDimensions[index];
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"Error: Index out of range.");
		}
		catch (XdmfError e)
		{
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
XdmfFortran::retrieveDimensionsTag(char * tag, int tagLength)
{
	if (mDimensions == NULL)
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"Must set dimensions before its tag can be retrieved.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
	else
	{
		char * tempTag = strdup(mDimensions->getItemTag().c_str());
		memset(tag, 0, tagLength);
		memcpy(tag, tempTag, strlen(tempTag)+1);
		delete [] tempTag;
	}
}

int
XdmfFortran::retrieveDimensionsValueType()
{
	if (mDimensions == NULL)
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"Must set dimensions before its value type can be retrieved.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
	else
	{
		shared_ptr<const XdmfArrayType> dataType = mDimensions->getArrayType();
		if (dataType == XdmfArrayType::Int8())
		{
			return XDMF_ARRAY_TYPE_INT8;
		}
		else if (dataType == XdmfArrayType::Int16())
		{
			return XDMF_ARRAY_TYPE_INT16;
		}
		else if (dataType == XdmfArrayType::Int32())
		{
			return XDMF_ARRAY_TYPE_INT32;
		}
		else if (dataType == XdmfArrayType::Int64())
		{
			return XDMF_ARRAY_TYPE_INT64;
		}
		else if (dataType == XdmfArrayType::UInt8())
		{
			return XDMF_ARRAY_TYPE_UINT8;
		}
		else if (dataType == XdmfArrayType::UInt16())
		{
			return XDMF_ARRAY_TYPE_UINT16;
		}
		else if (dataType == XdmfArrayType::UInt32())
		{
			return XDMF_ARRAY_TYPE_UINT32;
		}
		else if (dataType == XdmfArrayType::Float32())
		{
			return XDMF_ARRAY_TYPE_FLOAT32;
		}
		else if (dataType == XdmfArrayType::Float64())
		{
			return XDMF_ARRAY_TYPE_FLOAT64;
		}
		else if (dataType == XdmfArrayType::Uninitialized())
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, 
					"Uninitialized Dimension Data Type.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, 
					"Invalid Dimension Data Type.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
}

void
XdmfFortran::retrieveDimensionsValues(void * values, int dataType, int numberRead, int startIndex, int arrayStride, int valueStride)
{
	if(mDimensions == NULL)
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"Must set dimensions before its values can be retrieved.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
	else
	{
		if (!mDimensions->isInitialized())
		{
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
	if (mDimensions == NULL)
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"Must set Dimensions before its size can be retrieved.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
	else
	{
		return mDimensions->getSize();
	}
}

int
XdmfFortran::retrieveDimensionsNumProperties()
{
	if (mDimensions == NULL)
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"Must set dimensions before the number of properties it contains can be retrieved.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
	else
	{
		return mDimensions->getItemProperties().size();
	}	
}

void
XdmfFortran::retrieveDimensionsProperty(int index, char * key, int keyLength, char * value, int valueLength)
{
	if (mDimensions == NULL)
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"Must set dimensions before the properties it contains can be retrieved.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
	else
	{
		if (index < mDimensions->getItemProperties().size())
		{
			std::map<std::string, std::string>::iterator walker = mDimensions->getItemProperties().begin();
			for (int i = 0; i<index; i++)
			{
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
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
}

void
XdmfFortran::retrieveDimensionsPropertyByKey(char * key, char * value, int valueLength)
{
	if (mDimensions == NULL)
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"Must set dimensions before the properties it contains can be retrieved.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
	else
	{
		std::string tempString = key;
		memset(value, 0, valueLength);
		if ((mDimensions->getItemProperties().count(tempString))>0)
		{
			char * tempValue = strdup(mDimensions->getItemProperties()[tempString].c_str());
			memcpy(value, tempValue, strlen(tempValue)+1);
			delete [] tempValue;
		}
	}
}

int
XdmfFortran::setOrigin(int numValues, int arrayType, void * pointValues)
{
  mOrigin = XdmfArray::New();

  // insert origin values into array
  writeToArray(mOrigin, numValues, arrayType, pointValues);

  int id = mPreviousOrigins.size();
  mPreviousOrigins.push_back(mOrigin);
  return id;
}

void
XdmfFortran::setPreviousOrigin(int index)
{
	if (index < mPreviousOrigins.size())
	{
		mOrigin = mPreviousOrigins[index];
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"Error: Index out of bounds.");
		}
		catch (XdmfError e)
		{
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
XdmfFortran::retrieveOriginTag(char * tag, int tagLength)
{
	if (mOrigin == NULL)
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"Must set origin before its tag can be retrieved.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
	else
	{
		char * tempTag = strdup(mOrigin->getItemTag().c_str());
		memset(tag, 0, tagLength);
		memcpy(tag, tempTag, strlen(tempTag)+1);
		delete [] tempTag;
	}
}

int
XdmfFortran::retrieveOriginValueType()
{
	if (mOrigin == NULL)
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"Must set origin before its value type can be retrieved.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
	else
	{
		shared_ptr<const XdmfArrayType> dataType = mOrigin->getArrayType();
		if (dataType == XdmfArrayType::Int8())
		{
			return XDMF_ARRAY_TYPE_INT8;
		}
		else if (dataType == XdmfArrayType::Int16())
		{
			return XDMF_ARRAY_TYPE_INT16;
		}
		else if (dataType == XdmfArrayType::Int32())
		{
			return XDMF_ARRAY_TYPE_INT32;
		}
		else if (dataType == XdmfArrayType::Int64())
		{
			return XDMF_ARRAY_TYPE_INT64;
		}
		else if (dataType == XdmfArrayType::UInt8())
		{
			return XDMF_ARRAY_TYPE_UINT8;
		}
		else if (dataType == XdmfArrayType::UInt16())
		{
			return XDMF_ARRAY_TYPE_UINT16;
		}
		else if (dataType == XdmfArrayType::UInt32())
		{
			return XDMF_ARRAY_TYPE_UINT32;
		}
		else if (dataType == XdmfArrayType::Float32())
		{
			return XDMF_ARRAY_TYPE_FLOAT32;
		}
		else if (dataType == XdmfArrayType::Float64())
		{
			return XDMF_ARRAY_TYPE_FLOAT64;
		}
		else if (dataType == XdmfArrayType::Uninitialized())
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, 
					"Uninitialized Origin Data Type.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, 
					"Invalid Origin Data Type.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
}

void
XdmfFortran::retrieveOriginValues(void * values, int dataType, int numberRead, int startIndex, int arrayStride, int valueStride)
{
	if(mOrigin == NULL)
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"Must set origin before its values can be retrieved.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
	else
	{
		if (!mOrigin->isInitialized())
		{
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
	if (mOrigin == NULL)
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"Must set origin before its size can be retrieved.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
	else
	{
		return mOrigin->getSize();
	}
}

int
XdmfFortran::retrieveOriginNumProperties()
{
	if (mOrigin == NULL)
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"Must set origin before the number of properties it contains can be retrieved.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
	else
	{
		return mOrigin->getItemProperties().size();
	}	
}

void
XdmfFortran::retrieveOriginProperty(int index, char * key, int keyLength, char * value, int valueLength)
{
	if (mOrigin == NULL)
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"Must set Origin before the properties it contains can be retrieved.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
	else
	{
		if (index < mOrigin->getItemProperties().size())
		{
			std::map<std::string, std::string>::iterator walker = mOrigin->getItemProperties().begin();
			for (int i = 0; i<index; i++)
			{
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
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
}

void
XdmfFortran::retrieveOriginPropertyByKey(char * key, char * value, int valueLength)
{
	if (mOrigin == NULL)
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"Must set origin before the properties it contains can be retrieved.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
	else
	{
		std::string tempString = key;
		memset(value, 0, valueLength);
		if ((mOrigin->getItemProperties().count(tempString))>0)
		{
			char * tempValue = strdup(mOrigin->getItemProperties()[tempString].c_str());
			memcpy(value, tempValue, strlen(tempValue)+1);
			delete [] tempValue;
		}
	}
}

int
XdmfFortran::setBrick(int numValues, int arrayType, void * pointValues)
{
  mBrick = XdmfArray::New();

  // insert brick values into array
  writeToArray(mBrick, numValues, arrayType, pointValues);

  int id = mPreviousBricks.size();
  mPreviousBricks.push_back(mBrick);
  return id;
}

void
XdmfFortran::setPreviousBrick(int index)
{
	if (index < mPreviousBricks.size())
	{
		mBrick = mPreviousBricks[index];
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"Error: Index out of bounds.");
		}
		catch (XdmfError e)
		{
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
XdmfFortran::retrieveBrickTag(char * tag, int tagLength)
{
	if (mBrick == NULL)
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"Must set Brick before its tag can be retrieved.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
	else
	{
		char * tempTag = strdup(mBrick->getItemTag().c_str());
		memset(tag, 0, tagLength);
		memcpy(tag, tempTag, strlen(tempTag)+1);
		delete [] tempTag;
	}
}

int
XdmfFortran::retrieveBrickValueType()
{
	if (mBrick == NULL)
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"Must set brick before its value type can be retrieved.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
	else
	{
		shared_ptr<const XdmfArrayType> dataType = mBrick->getArrayType();
		if (dataType == XdmfArrayType::Int8())
		{
			return XDMF_ARRAY_TYPE_INT8;
		}
		else if (dataType == XdmfArrayType::Int16())
		{
			return XDMF_ARRAY_TYPE_INT16;
		}
		else if (dataType == XdmfArrayType::Int32())
		{
			return XDMF_ARRAY_TYPE_INT32;
		}
		else if (dataType == XdmfArrayType::Int64())
		{
			return XDMF_ARRAY_TYPE_INT64;
		}
		else if (dataType == XdmfArrayType::UInt8())
		{
			return XDMF_ARRAY_TYPE_UINT8;
		}
		else if (dataType == XdmfArrayType::UInt16())
		{
			return XDMF_ARRAY_TYPE_UINT16;
		}
		else if (dataType == XdmfArrayType::UInt32())
		{
			return XDMF_ARRAY_TYPE_UINT32;
		}
		else if (dataType == XdmfArrayType::Float32())
		{
			return XDMF_ARRAY_TYPE_FLOAT32;
		}
		else if (dataType == XdmfArrayType::Float64())
		{
			return XDMF_ARRAY_TYPE_FLOAT64;
		}
		else if (dataType == XdmfArrayType::Uninitialized())
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, 
					"Uninitialized Brick Data Type.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, 
					"Invalid Brick Data Type.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
}

void
XdmfFortran::retrieveBrickValues(void * values, int dataType, int numberRead, int startIndex, int arrayStride, int valueStride)
{
	if(mBrick == NULL)
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"Must set brick before its values can be retrieved.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
	else
	{
		if (!mBrick->isInitialized())
		{
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
	if (mBrick == NULL)
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"Must set brick before its size can be retrieved.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
	else
	{
		return mBrick->getSize();
	}
}

int
XdmfFortran::retrieveBrickNumProperties()
{
	if (mBrick == NULL)
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"Must set brick before the number of properties it contains can be retrieved.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
	else
	{
		return mBrick->getItemProperties().size();
	}	
}

void
XdmfFortran::retrieveBrickProperty(int index, char * key, int keyLength, char * value, int valueLength)
{
	if (mBrick == NULL)
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"Must set brick before the properties it contains can be retrieved.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
	else
	{
		if (index < mBrick->getItemProperties().size())
		{
			std::map<std::string, std::string>::iterator walker = mBrick->getItemProperties().begin();
			for (int i = 0; i<index; i++)
			{
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
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
}

void
XdmfFortran::retrieveBrickPropertyByKey(char * key, char * value, int valueLength)
{
	if (mBrick == NULL)
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"Must set brick before the properties it contains can be retrieved.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
	else
	{
		std::string tempString = key;
		memset(value, 0, valueLength);
		if ((mBrick->getItemProperties().count(tempString))>0)
		{
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
XdmfFortran::retrieveMapTag(int index, char * tag, int tagLength)
{
	if (index < mMaps.size())
	{
		char * tempTag = strdup(mMaps[index]->getItemTag().c_str());
		memset(tag, 0, tagLength);
		memcpy(tag, tempTag, strlen(tempTag)+1);
		delete [] tempTag;
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

void
XdmfFortran::addRemoteNodeID(int index, int localNodeID, int remoteTaskID, int remoteLocalNodeID)
{
	mMaps[index]->insert(remoteTaskID, localNodeID, remoteLocalNodeID);
}

void
XdmfFortran::retrieveRemoteNodeIDs(int index, int localNodeID, int remoteTaskID, int * remoteNodeIDs)
{
	if (mMaps.size()>index)
	{
		if (mMaps[index]->getRemoteNodeIds(remoteTaskID).count(localNodeID)>0)
		{
			std::set<int> returnSet = mMaps[index]->getRemoteNodeIds(remoteTaskID)[localNodeID];
			std::set<int>::iterator walker;
			int i = 0;
			for (walker = returnSet.begin(); walker != returnSet.end();walker++)
			{
				remoteNodeIDs[i] =  *walker;
				i++;
			}
		}
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, 
					"The map does not contain a remote ID for the requested node.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"Error: Index out of Range.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

int
XdmfFortran::retrieveNumRemoteNodeIDs(int index, int localNodeID, int remoteTaskID)
{
	if (mMaps.size()<index)
	{
		if (mMaps[index]->getRemoteNodeIds(remoteTaskID).count(localNodeID)>0)
		{
			return mMaps[index]->getRemoteNodeIds(remoteTaskID)[localNodeID].size();
		}
		else
		{
			return 0;
		}
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"Error: Index out of Range.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

int
XdmfFortran::storeMap(int index)
{
	if (index < mMaps.size())
	{
		int id = mPreviousMaps.size();
		mPreviousMaps.push_back(mMaps[index]);
		return id;
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"Error: Index out of Range.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

void
XdmfFortran::addPreviousMap(int index)
{
	if (index < mPreviousMaps.size())
	{
		mMaps.push_back(mPreviousMaps[index]);
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"Error: Index out of range.");
		}
		catch (XdmfError e)
		{
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
XdmfFortran::removeMap(int index)
{	
	if (index < mMaps.size())
	{
		mMaps.erase(mMaps.begin()+index);
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"Error: Index out of range.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

int
XdmfFortran::retrieveMapNumProperties(int index)
{
	if (index < mMaps.size())
	{
		return mMaps[index]->getItemProperties().size();
	}	
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"Error: Index out of range.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

void
XdmfFortran::retrieveMapProperty(int mapIndex, int index, char * key, int keyLength, char * value, int valueLength)
{
	if (mapIndex < mMaps.size())
	{
		if (index < mMaps[index]->getItemProperties().size())
		{
			std::map<std::string, std::string>::iterator walker = mMaps[index]->getItemProperties().begin();
			for (int i = 0; i<index; i++)
			{
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
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"Error: Index out of range.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

void
XdmfFortran::retrieveMapPropertyByKey(int index, char * key, char * value, int valueLength)
{
	if (index < mMaps.size())
	{
		std::string tempString = key;
		memset(value, 0, valueLength);
		if ((mMaps[index]->getItemProperties().count(tempString))>0)
		{
			char * tempValue = strdup(mMaps[index]->getItemProperties()[tempString].c_str());
			memcpy(value, tempValue, strlen(tempValue)+1);
			delete [] tempValue;
		}
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, 
				"Error: Index out of range.");
		}
		catch (XdmfError e)
		{
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
XdmfFortran::retrieveAttributeTag(int index, char * tag, int tagLength)
{
	if (index < mAttributes.size())
	{
		char * tempTag = strdup(mAttributes[index]->getItemTag().c_str());
		memset(tag, 0, tagLength);
		memcpy(tag, tempTag, strlen(tempTag)+1);
		delete [] tempTag;
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

void
XdmfFortran::retrieveAttributeName(int index, char * name, int nameLength)
{
	if (index < mAttributes.size())
	{
		char * tempName = strdup(mAttributes[index]->getName().c_str());
		memset(name, 0, nameLength);
		memcpy(name, tempName, strlen(tempName)+1);
		delete [] tempName;
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

int
XdmfFortran::retrieveAttributeValueType(int index)
{
	if (index < mAttributes.size())
	{
		shared_ptr<const XdmfArrayType> dataType = mAttributes[index]->getArrayType();
		if (dataType == XdmfArrayType::Int8())
		{
			return XDMF_ARRAY_TYPE_INT8;
		}
		else if (dataType == XdmfArrayType::Int16())
		{
			return XDMF_ARRAY_TYPE_INT16;
		}
		else if (dataType == XdmfArrayType::Int32())
		{
			return XDMF_ARRAY_TYPE_INT32;
		}
		else if (dataType == XdmfArrayType::Int64())
		{
			return XDMF_ARRAY_TYPE_INT64;
		}
		else if (dataType == XdmfArrayType::UInt8())
		{
			return XDMF_ARRAY_TYPE_UINT8;
		}
		else if (dataType == XdmfArrayType::UInt16())
		{
			return XDMF_ARRAY_TYPE_UINT16;
		}
		else if (dataType == XdmfArrayType::UInt32())
		{
			return XDMF_ARRAY_TYPE_UINT32;
		}
		else if (dataType == XdmfArrayType::Float32())
		{
			return XDMF_ARRAY_TYPE_FLOAT32;
		}
		else if (dataType == XdmfArrayType::Float64())
		{
			return XDMF_ARRAY_TYPE_FLOAT64;
		}
		else if (dataType == XdmfArrayType::Uninitialized())
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, 
					"Uninitialized Attribute Data Type.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, 
					"Invalid Attribute Data Type.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

void
XdmfFortran::retrieveAttributeValues(int index, void * values, int dataType, int numberRead, int startIndex, int arrayStride, int valueStride)
{
	if (index < mAttributes.size())
	{
		if (!mAttributes[index]->isInitialized())
		{
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
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

void
XdmfFortran::removeAttribute(int index)
{
	if (index < mAttributes.size())
	{
		mAttributes.erase(mAttributes.begin()+index);
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}	
}

void
XdmfFortran::replaceAttribute(int index, char * name, int attributeCenter, int attributeType, int numValues, int arrayType, void * values)
{
	if (index < mAttributes.size())
	{
		shared_ptr<XdmfAttribute> currAttribute = XdmfAttribute::New();
		currAttribute->setName(name);
		switch(attributeCenter)
		{
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
				try
				{
					XdmfError::message(XdmfError::FATAL, "Invalid attribute center");
				}
				catch (XdmfError e)
				{
					throw e;
				}
		}

		switch(attributeType)
		{
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
				try
				{
					XdmfError::message(XdmfError::FATAL, "Invalid attribute type");
				}
				catch (XdmfError e)
				{
					throw e;
				}
		}

		writeToArray(currAttribute, numValues, arrayType, values);
		mAttributes[index] = currAttribute;
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}	
}

void
XdmfFortran::openAttribute(int index)
{
	if (index < mAttributes.size())
	{
		int i;
		shared_ptr<XdmfAttribute> openedAttribute = mAttributes[index];
		int n = openedAttribute->getNumberInformations();
		shared_ptr<XdmfInformation> openedInformation;
		for (i = 0; i < n; i++)
		{
			openedInformation = openedAttribute->getInformation(i);
			mInformations.push_back(openedInformation);
			mPreviousInformations.push_back(openedInformation);
		}
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

int
XdmfFortran::retrieveAttributeSize(int index)
{
	if (index < mAttributes.size())
	{
		return mAttributes[index]->getSize();
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

int
XdmfFortran::retrieveAttributeType(int index)
{
	if (index < mAttributes.size())
	{
		shared_ptr<const XdmfAttributeType> returnType = mAttributes[index]->getType();
		if (returnType == XdmfAttributeType::Scalar())
		{
			return XDMF_ATTRIBUTE_TYPE_SCALAR;
		}
		else if (returnType == XdmfAttributeType::Vector())
		{
			return XDMF_ATTRIBUTE_TYPE_VECTOR;
		}
		else if (returnType == XdmfAttributeType::Tensor())
		{
			return XDMF_ATTRIBUTE_TYPE_TENSOR;
		}
		else if (returnType == XdmfAttributeType::Matrix())
		{
			return XDMF_ATTRIBUTE_TYPE_MATRIX;
		}
		else if (returnType == XdmfAttributeType::Tensor6())
		{
			return XDMF_ATTRIBUTE_TYPE_TENSOR6;
		}
		else if (returnType == XdmfAttributeType::GlobalId())
		{
			return XDMF_ATTRIBUTE_TYPE_GLOBALID;
		}
		else
		{
			return XDMF_ATTRIBUTE_TYPE_NOTYPE;
		}
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

int
XdmfFortran::retrieveAttributeCenter(int index)
{
	if (index < mAttributes.size())
	{
		shared_ptr<const XdmfAttributeCenter> returnCenter = mAttributes[index]->getCenter();
		if (returnCenter == XdmfAttributeCenter::Grid())
		{
			return XDMF_ATTRIBUTE_CENTER_GRID;
		}
		else if (returnCenter == XdmfAttributeCenter::Cell())
		{
			return XDMF_ATTRIBUTE_CENTER_CELL;
		}
		else if (returnCenter == XdmfAttributeCenter::Face())
		{
			return XDMF_ATTRIBUTE_CENTER_FACE;
		}
		else if (returnCenter == XdmfAttributeCenter::Edge())
		{
			return XDMF_ATTRIBUTE_CENTER_EDGE;
		}
		else if (returnCenter == XdmfAttributeCenter::Node())
		{
			return XDMF_ATTRIBUTE_CENTER_NODE;
		}
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Error: Invalid Attribute Center.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

void
XdmfFortran::clearPreviousAttributes()
{
	mPreviousAttributes.clear();
}

int
XdmfFortran::retrieveAttributeNumProperties(int index)
{
	if (index < mAttributes.size())
	{
		return mAttributes[index]->getItemProperties().size();
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}	
}

void
XdmfFortran::retrieveAttributeProperty(int attributeIndex, int index, char * key, int keyLength, char * value, int valueLength)
{
	if (attributeIndex < mAttributes.size())
	{
		if (index < mAttributes[attributeIndex]->getItemProperties().size())
		{
			std::map<std::string, std::string>::iterator walker = mAttributes[attributeIndex]->getItemProperties().begin();
			for (int i = 0; i<index; i++)
			{
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
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Error: Property index out of range.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

void
XdmfFortran::retrieveAttributePropertyByKey(int index, char * key, char * value, int valueLength)
{
	if (index < mAttributes.size())
	{
		std::string tempString = key;
		memset(value, 0, valueLength);
		if ((mAttributes[index]->getItemProperties().count(tempString))>0)
		{
			char * tempValue = strdup(mAttributes[index]->getItemProperties()[tempString].c_str());
			memcpy(value, tempValue, strlen(tempValue)+1);
			delete [] tempValue;
		}
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

int
XdmfFortran::addCoordinate(char * name, int numValues, int arrayType, void * values)
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
XdmfFortran::addPreviousCoordinate(int index)
{
	if (index < mPreviousCoordinates.size())
	{
		mCoordinates.push_back(mPreviousCoordinates[index]);
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

void
XdmfFortran::clearPreviousCoordinates()
{
	mPreviousCoordinates.clear();
}

int
XdmfFortran::retrieveNumCoordinates()
{
	return mCoordinates.size();
}

void
XdmfFortran::retrieveCoordinateTag(int index, char * tag, int tagLength)
{
	if (index < mCoordinates.size())
	{
		char * tempTag = strdup(mCoordinates[index]->getItemTag().c_str());
		memset(tag, 0, tagLength);
		memcpy(tag, tempTag, strlen(tempTag)+1);
		delete [] tempTag;
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

void
XdmfFortran::retrieveCoordinateName(int index, char * name, int nameLength)
{
	if (index < mCoordinates.size())
	{
		char * tempName = strdup(mCoordinates[index]->getName().c_str());
		memset(name, 0, nameLength);
		memcpy(name, tempName, strlen(tempName)+1);
		delete [] tempName;
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

int
XdmfFortran::retrieveCoordinateValueType(int index)
{
	if (index < mCoordinates.size())
	{
		shared_ptr<const XdmfArrayType> dataType = mCoordinates[index]->getArrayType();
		if (dataType == XdmfArrayType::Int8())
		{
			return XDMF_ARRAY_TYPE_INT8;
		}
		else if (dataType == XdmfArrayType::Int16())
		{
			return XDMF_ARRAY_TYPE_INT16;
		}
		else if (dataType == XdmfArrayType::Int32())
		{
			return XDMF_ARRAY_TYPE_INT32;
		}
		else if (dataType == XdmfArrayType::Int64())
		{
			return XDMF_ARRAY_TYPE_INT64;
		}
		else if (dataType == XdmfArrayType::UInt8())
		{
			return XDMF_ARRAY_TYPE_UINT8;
		}
		else if (dataType == XdmfArrayType::UInt16())
		{
			return XDMF_ARRAY_TYPE_UINT16;
		}
		else if (dataType == XdmfArrayType::UInt32())
		{
			return XDMF_ARRAY_TYPE_UINT32;
		}
		else if (dataType == XdmfArrayType::Float32())
		{
			return XDMF_ARRAY_TYPE_FLOAT32;
		}
		else if (dataType == XdmfArrayType::Float64())
		{
			return XDMF_ARRAY_TYPE_FLOAT64;
		}
		else if (dataType == XdmfArrayType::Uninitialized())
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, 
					"Uninitialized Coordinate Data Type.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, 
					"Invalid Coordinate Data Type.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

void
XdmfFortran::retrieveCoordinateValues(int index, void * values, int dataType, int numberRead, int startIndex, int arrayStride, int valueStride)
{
	if (index < mCoordinates.size())
	{
		if (!mCoordinates[index]->isInitialized())
		{
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
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

void
XdmfFortran::removeCoordinate(int index)
{
	if (index < mCoordinates.size())
	{
		mCoordinates.erase(mCoordinates.begin()+index);
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}	
}

void
XdmfFortran::replaceCoordinate(int index, char * name, int numValues, int arrayType, void * values)
{
	if (index < mCoordinates.size())
	{
		shared_ptr<XdmfArray> currArray = XdmfArray::New();
		currArray->setName(name);
		writeToArray(currArray, numValues, arrayType, values);
		mCoordinates[index] = currArray;
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}	
}

int
XdmfFortran::retrieveCoordinateSize(int index)
{
	if (index < mCoordinates.size())
	{
		return mCoordinates[index]->getSize();
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

void
XdmfFortran::clearCoordinates()
{
	mCoordinates.clear();
}

int
XdmfFortran::retrieveCoordinateNumProperties(int index)
{
	if (index < mCoordinates.size())
	{
		return mCoordinates[index]->getItemProperties().size();
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}	
}

void
XdmfFortran::retrieveCoordinateProperty(int coordinateIndex, int index, char * key, int keyLength, char * value, int valueLength)
{
	if (coordinateIndex < mCoordinates.size())
	{
		if (index < mCoordinates[coordinateIndex]->getItemProperties().size())
		{
			std::map<std::string, std::string>::iterator walker = mCoordinates[coordinateIndex]->getItemProperties().begin();
			for (int i = 0; i<index; i++)
			{
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
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Error: Property index out of range.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

void
XdmfFortran::retrieveCoordinatePropertyByKey(int index, char * key, char * value, int valueLength)
{
	if (index < mCoordinates.size())
	{
		std::string tempString = key;
		memset(value, 0, valueLength);
		if ((mCoordinates[index]->getItemProperties().count(tempString))>0)
		{
			char * tempValue = strdup(mCoordinates[index]->getItemProperties()[tempString].c_str());
			memcpy(value, tempValue, strlen(tempValue)+1);
			delete [] tempValue;
		}
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

int 
XdmfFortran::addSet(char * name, int newSetType, void * values, int numValues,  int arrayType)
{
	const shared_ptr<XdmfSet> newSet = XdmfSet::New();
	newSet->setName(name);

	switch (newSetType)
	{
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
			try
			{
				XdmfError::message(XdmfError::FATAL, 
					"Invalid set type.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
	}

	for(std::vector<shared_ptr<XdmfAttribute> >::const_iterator iter =
		mAttributes.begin(); 
		iter != mAttributes.end();
		++iter)
	{
		newSet->insert(*iter);
	}
	mAttributes.clear();

	if (!mInformations.empty())
	{
		for(std::vector<shared_ptr<XdmfInformation> >::const_iterator iter =
			mInformations.begin(); 
			iter != mInformations.end();
			++iter)
		{
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
XdmfFortran::addPreviousSet(int index)
{
	if (index < mPreviousSets.size())
	{
		mSets.push_back(mPreviousSets[index]);
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
		}
		catch (XdmfError e)
		{
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
XdmfFortran::retrieveSetTag(int index, char * tag, int tagLength)
{
	if (index < mSets.size())
	{
		char * tempTag = strdup(mSets[index]->getItemTag().c_str());
		memset(tag, 0, tagLength);
		memcpy(tag, tempTag, strlen(tempTag)+1);
		delete [] tempTag;
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

void
XdmfFortran::retrieveSetName(int index, char * name, int nameLength)
{
	if (index < mSets.size())
	{
		char * tempName = strdup(mSets[index]->getName().c_str());
		memset(name, 0, nameLength);
		memcpy(name, tempName, strlen(tempName)+1);
		delete [] tempName;
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
		}
		catch (XdmfError e)
		{
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
XdmfFortran::retrieveSetSize(int index)
{
	if (index < mSets.size())
	{
		return mSets[index]->getSize();
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

int
XdmfFortran::retrieveSetValueType(int index)
{
	if (index < mSets.size())
	{
		shared_ptr<const XdmfArrayType> dataType = mSets[index]->getArrayType();
		if (dataType == XdmfArrayType::Int8())
		{
			return XDMF_ARRAY_TYPE_INT8;
		}
		else if (dataType == XdmfArrayType::Int16())
		{
			return XDMF_ARRAY_TYPE_INT16;
		}
		else if (dataType == XdmfArrayType::Int32())
		{
			return XDMF_ARRAY_TYPE_INT32;
		}
		else if (dataType == XdmfArrayType::Int64())
		{
			return XDMF_ARRAY_TYPE_INT64;
		}
		else if (dataType == XdmfArrayType::UInt8())
		{
			return XDMF_ARRAY_TYPE_UINT8;
		}
		else if (dataType == XdmfArrayType::UInt16())
		{
			return XDMF_ARRAY_TYPE_UINT16;
		}
		else if (dataType == XdmfArrayType::UInt32())
		{
			return XDMF_ARRAY_TYPE_UINT32;
		}
		else if (dataType == XdmfArrayType::Float32())
		{
			return XDMF_ARRAY_TYPE_FLOAT32;
		}
		else if (dataType == XdmfArrayType::Float64())
		{
			return XDMF_ARRAY_TYPE_FLOAT64;
		}
		else if (dataType == XdmfArrayType::Uninitialized())
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, 
					"Uninitialized Set Data Type.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, 
					"Invalid Set Data Type.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

void
XdmfFortran::retrieveSetValues(int index, void * values, int dataType, int numberRead, int startIndex, int arrayStride, int valueStride)
{
	if (index < mSets.size())
	{
		if (!mSets[index]->isInitialized())
		{
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
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

int
XdmfFortran::retrieveSetType(int index)
{
	if (index < mSets.size())
	{
		shared_ptr<const XdmfSetType> returnType = mSets[index]->getType();
		if (returnType == XdmfSetType::Node())
		{
			return XDMF_SET_TYPE_NODE;
		}
		else if (returnType == XdmfSetType::Cell())
		{
			return XDMF_SET_TYPE_CELL;
		}
		else if (returnType == XdmfSetType::Face())
		{
			return XDMF_SET_TYPE_FACE;
		}
		else if (returnType == XdmfSetType::Edge())
		{
			return XDMF_SET_TYPE_EDGE;
		}
		else if (returnType == XdmfSetType::NoSetType())
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Error: Uninitialized set type.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Error: Invalid set type.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

void
XdmfFortran::openSet(int index, int openAttribute, int openInformation)
{
	if (index < mSets.size())
	{
		shared_ptr<XdmfSet> openedSet = mSets[index];
		int i;
		int n;
		n = openedSet->getNumberAttributes();
		if (openAttribute == 1)
		{
			shared_ptr<XdmfAttribute> openedAttribute;
			for (i = 0; i < n; i++)
			{
				openedAttribute = openedSet->getAttribute(i);
				mAttributes.push_back(openedAttribute);
				mPreviousAttributes.push_back(openedAttribute);
			}
		}
		if (openInformation == 1)
		{
			n = openedSet->getNumberInformations();
			shared_ptr<XdmfInformation> openedInformation;
			for (i = 0; i < n; i++)
			{
				openedInformation = openedSet->getInformation(i);
				mInformations.push_back(openedInformation);
				mPreviousInformations.push_back(openedInformation);
			}
		}
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

void
XdmfFortran::removeSet(int index)
{
	if (index < mSets.size())
	{
		mSets.erase(mSets.begin()+index);
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

void 
XdmfFortran::replaceSet(int index, char * name, int newSetType, void * values, int numValues,  int arrayType)
{
	const shared_ptr<XdmfSet> newSet = XdmfSet::New();
	newSet->setName(name);

	switch (newSetType)
	{
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
			try
			{
				XdmfError::message(XdmfError::FATAL, 
					"Invalid set type.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
	}

	for(std::vector<shared_ptr<XdmfAttribute> >::const_iterator iter =
		mAttributes.begin(); 
		iter != mAttributes.end();
		++iter)
	{
		newSet->insert(*iter);
	}
	mAttributes.clear();

	if (!mInformations.empty())
	{
		for(std::vector<shared_ptr<XdmfInformation> >::const_iterator iter =
			mInformations.begin(); 
			iter != mInformations.end();
			++iter)
		{
			newSet->insert(*iter);
		}
		mInformations.clear();
	}

	writeToArray(newSet, numValues, arrayType, values);

	mSets[index] = newSet;
}

int
XdmfFortran::retrieveSetNumProperties(int index)
{
	if (index < mSets.size())
	{
		return mSets[index]->getItemProperties().size();
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

void
XdmfFortran::retrieveSetProperty(int setIndex, int index, char * key, int keyLength, char * value, int valueLength)
{
	if (setIndex < mSets.size())
	{
		if (index < mSets[setIndex]->getItemProperties().size())
		{
			std::map<std::string, std::string>::iterator walker = mSets[setIndex]->getItemProperties().begin();
			for (int i = 0; i<index; i++)
			{
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
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Error: Property index out of range.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

void
XdmfFortran::retrieveSetPropertyByKey(int index, char * key, char * value, int valueLength)
{
	if (index < mSets.size())
	{
		std::string tempString = key;
		memset(value, 0, valueLength);
		if ((mSets[index]->getItemProperties().count(tempString))>0)
		{
			char * tempValue = strdup(mSets[index]->getItemProperties()[tempString].c_str());
			memcpy(value, tempValue, strlen(tempValue)+1);
			delete [] tempValue;
		}
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
		}
		catch (XdmfError e)
		{
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
XdmfFortran::retrieveInformationTag(int index, char * tag, int tagLength)
{
	if (index < mInformations.size())
	{
		char * tempTag = strdup(mInformations[index]->getItemTag().c_str());
		memset(tag, 0, tagLength);
		memcpy(tag, tempTag, strlen(tempTag)+1);
		delete [] tempTag;
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

void
XdmfFortran::retrieveInformation(int index, char * key, int keyLength, char * value, int valueLength)
{
	if (index < mInformations.size())
	{
		char * tempKey = strdup(mInformations[index]->getKey().c_str());
		char * tempValue = strdup(mInformations[index]->getValue().c_str());		
		memset(key, 0, keyLength);
		memset(value, 0, valueLength);
		memcpy(key, tempKey, strlen(tempKey)+1);
		memcpy(value, tempValue, strlen(tempValue)+1);
		delete [] tempKey;
		delete [] tempValue;
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

void
XdmfFortran::replaceInformation(int index, char * key, char * value)
{
	if (index < mInformations.size())
	{
		mInformations[index]->setKey(key);
		mInformations[index]->setValue(value);
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

void
XdmfFortran::removeInformation(int index)
{
	if (index < mInformations.size())
	{
		mInformations.erase(mInformations.begin()+index);
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

void
XdmfFortran::openInformation(int index)
{
	if (index < mInformations.size())
	{
		shared_ptr<XdmfInformation> sourceInformation = mInformations[index];
		int i;
		int n = sourceInformation->getNumberInformations();
		shared_ptr<XdmfInformation> openedInformation;
		for (i = 0; i < n; i++)
		{
			openedInformation = sourceInformation->getInformation(i);
			mInformations.push_back(openedInformation);
			mPreviousInformations.push_back(openedInformation);
		}
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

void
XdmfFortran::retrieveInformationByKey(char * key, char * value, int valueLength)
{
	int i;
	int found = 0;
	std::string searchString(key);
	char * tempValue;
	for (i=0;i<mInformations.size();i++)
	{
		if (searchString == mInformations[i]->getKey())
		{
			found = 1;
			tempValue = strdup(mInformations[i]->getValue().c_str());
			memset(value, 0, valueLength);
			memcpy(value, tempValue, strlen(tempValue)+1);
			i = mInformations.size();
			delete [] tempValue;
		}
	}
	if (found == 0)
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Item with specifed key does not exist.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

void
XdmfFortran::replaceInformationByKey(char * key, char * value)
{
	int i;
	int found = 0;
	std::string searchString(key);
	for (i=0;i<mInformations.size();i++)
	{
		if (searchString == mInformations[i]->getKey())
		{
			mInformations[i]->setValue(value);
			i = mInformations.size();
			found = 1;
		}
	}
	if (found == 0)
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Item with specifed key does not exist.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

void
XdmfFortran::removeInformationByKey(char * key)
{
	int i;
	int found = 0;
	std::string searchString(key);
	for (i=0;i<mInformations.size();i++)
	{
		if (searchString == mInformations[i]->getKey())
		{
			mInformations.erase(mInformations.begin()+i);
			i = mInformations.size();
			found = 1;
		}
	}
	if (found == 0)
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Item with specifed key does not exist.");
		}
		catch (XdmfError e)
		{
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
XdmfFortran::retrieveInformationNumProperties(int index)
{
	if (index < mInformations.size())
	{
		return mInformations[index]->getItemProperties().size();
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

void
XdmfFortran::retrieveInformationProperty(int informationIndex, int index, char * key, int keyLength, char * value, int valueLength)
{
	if (informationIndex < mInformations.size())
	{
		if (index < mInformations[informationIndex]->getItemProperties().size())
		{
			std::map<std::string, std::string>::iterator walker = mInformations[informationIndex]->getItemProperties().begin();
			for (int i = 0; i<index; i++)
			{
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
		else
		{
			try
			{
				XdmfError::message(XdmfError::FATAL, "Error: Property index out of range.");
			}
			catch (XdmfError e)
			{
				throw e;
			}
		}
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
		}
		catch (XdmfError e)
		{
			throw e;
		}
	}
}

void
XdmfFortran::retrieveInformationPropertyByKey(int index, char * key, char * value, int valueLength)
{
	if (index < mInformations.size())
	{
		std::string tempString = key;
		memset(value, 0, valueLength);
		if ((mInformations[index]->getItemProperties().count(tempString))>0)
		{
			char * tempValue = strdup(mInformations[index]->getItemProperties()[tempString].c_str());
			memcpy(value, tempValue, strlen(tempValue)+1);
			delete [] tempValue;
		}
	}
	else
	{
		try
		{
			XdmfError::message(XdmfError::FATAL, "Error: Index out of range.");
		}
		catch (XdmfError e)
		{
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
XdmfFortran::write(const char * const xmlFilePath, const int datalimit, const bool release)
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
XdmfFortran::writeHDF5(const char * const xmlFilePath, const bool release)
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
  mDomain->accept(mHeavyDataWriter);
}

void
XdmfFortran::initHDF5(const char * const xmlFilePath, const bool release)
{
  shared_ptr<XdmfHDF5Writer> writer = XdmfHDF5Writer::New(xmlFilePath);
  writer->setFileSizeLimit(mMaxFileSize);
  writer->setAllowSetSplitting(mAllowSetSplitting);
  writer->setReleaseData( release );
  mHeavyDataWriter = writer;
}

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
  XdmfRetrieveDomainProperty(long * pointer, int * index, char * key, int * keyLength, char * value, int * valueLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveDomainProperty(*index, key, *keyLength, value, *valueLength);
  }

  void
  XdmfRetrieveDomainPropertyByKey(long * pointer, char * key, char * value, int * valueLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveDomainPropertyByKey(key, value, *valueLength);
  }

  void
  XdmfOpenDomainGridCollection(long * pointer, int * index, int * openMaps, int * openAttributes, int * openInformation, int * openSets)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->openDomainGridCollection(*index, *openMaps, *openAttributes, *openInformation, *openSets);
  }

  void
  XdmfRemoveDomainGridCollection(long * pointer, int * index)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->removeDomainGridCollection(*index);
  }

  void
  XdmfOpenGridCollectionGridCollection(long * pointer, int * index, int * openMaps, int * openAttributes, int * openInformation, int * openSets)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->openGridCollectionGridCollection(*index, *openMaps, *openAttributes, *openInformation, *openSets);
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
  XdmfRetrieveGridCollectionProperty(long * pointer, int * index, char * key, int * keyLength, char * value, int * valueLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveGridCollectionProperty(*index, key, *keyLength, value, *valueLength);
  }

  void
  XdmfRetrieveGridCollectionPropertyByKey(long * pointer, char * key, char * value, int * valueLength)
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
  XdmfOpenDomainGrid(long * pointer, int * gridType, int * index, int * openMaps, int * openAttributes, int * openInformation, int * openSets)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->openDomainGrid(*gridType, *index, *openMaps, *openAttributes, *openInformation, *openSets);
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
  XdmfRetrieveDomainGridTag(long * pointer, int * gridType, int * index, char * tag, int * tagLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveDomainGridTag(*gridType, *index, tag, *tagLength);
  }

  void
  XdmfRetrieveDomainGridName(long * pointer, int * gridType, int * index,  char * name, int * nameLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveDomainGridName(*gridType, *index, name, *nameLength);
  }

  void
  XdmfRetrieveDomainGridNumProperties(long * pointer, int * gridType, int * index, int * total)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    *total = xdmfFortran->retrieveDomainGridNumProperties(*gridType, *index);
  }

  void
  XdmfRetrieveDomainGridProperty(long * pointer, int * gridType, int * gridIndex, int * index,
	char * key, int * keyLength, char * value, int * valueLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveDomainGridProperty(*gridType, *gridIndex, *index, key, *keyLength, value, *valueLength);
  }

  void
  XdmfRetrieveDomainGridPropertyByKey(long * pointer, int * gridType, int * index, char * key, char * value, int * valueLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveDomainGridPropertyByKey(*gridType, *index, key, value, *valueLength);
  }

  void
  XdmfOpenGridCollectionGrid(long * pointer, int * gridType, int * index,
    int * openMaps, int * openAttributes, int * openInformation, int * openSets)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->openGridCollectionGrid(*gridType, *index, *openMaps, *openAttributes, *openInformation, *openSets);
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
  XdmfRetrieveGridCollectionGridTag(long * pointer, int * gridType, int * index, char * tag, int * tagLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveGridCollectionGridTag(*gridType, *index, tag, *tagLength);
  }

  void
  XdmfRetrieveGridCollectionGridName(long * pointer, int * gridType, int * index,  char * name, int * nameLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveGridCollectionGridName(*gridType, *index, name, *nameLength);
  }

  void
  XdmfRetrieveGridCollectionGridNumProperties(long * pointer,int * gridType, int * index, int * total)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    *total = xdmfFortran->retrieveGridCollectionGridNumProperties(*gridType, *index);
  }

  void
  XdmfRetrieveGridCollectionGridProperty(long * pointer, int * gridType, int * gridIndex, int * index,
	char * key, int * keyLength, char * value, int * valueLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveGridCollectionGridProperty(*gridType, *gridIndex, *index, key, *keyLength, value, *valueLength);
  }

  void
  XdmfRetrieveGridCollectionGridPropertyByKey(long * pointer, int * gridType, int *index, char * key, char * value, int * valueLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveGridCollectionGridPropertyByKey(*gridType, *index, key, value, *valueLength);
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
  XdmfRetrieveTopologyValues(long * pointer, void * values, int * dataType,
	int * numberRead, int * startIndex, int * arrayStride, int * valueStride)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveTopologyValues(values, *dataType, *numberRead, *startIndex, *arrayStride, *valueStride);
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
  XdmfRetrieveTopologyNumProperties(long * pointer, int * total)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    *total = xdmfFortran->retrieveTopologyNumProperties();
  }

  void
  XdmfRetrieveTopologyProperty(long * pointer, int * index, char * key, int * keyLength, char * value, int * valueLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveTopologyProperty(*index, key, *keyLength, value, *valueLength);
  }

  void
  XdmfRetrieveTopologyPropertyByKey(long * pointer, char * key, char * value, int * valueLength)
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
  XdmfRetrieveGeometryValues(long * pointer, void * values, int * dataType,
	int * numberRead, int * startIndex, int * arrayStride, int * valueStride)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveGeometryValues(values, *dataType, *numberRead, *startIndex, *arrayStride, *valueStride);
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
  XdmfRetrieveGeometryNumProperties(long * pointer, int * total)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    *total = xdmfFortran->retrieveGeometryNumProperties();
  }

  void
  XdmfRetrieveGeometryProperty(long * pointer, int * index, char * key, int * keyLength, char * value, int * valueLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveGeometryProperty(*index, key, *keyLength, value, *valueLength);
  }

  void
  XdmfRetrieveGeometryPropertyByKey(long * pointer, char * key, char * value, int * valueLength)
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
  XdmfRetrieveDimensionsTag(long * pointer, char * tag, int * tagLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveDimensionsTag(tag, *tagLength);
  }

  void
  XdmfRetrieveDimensionsValues(long * pointer, void * values, int * dataType,
	int * numberRead, int * startIndex, int * arrayStride, int * valueStride)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveDimensionsValues(values, *dataType, *numberRead, *startIndex, *arrayStride, *valueStride);
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
  XdmfRetrieveDimensionsProperty(long * pointer, int * index, char * key, int * keyLength, char * value, int * valueLength)
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
  XdmfRetrieveOriginTag(long * pointer, char * tag, int * tagLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveOriginTag(tag, *tagLength);
  }

  void
  XdmfRetrieveOriginValues(long * pointer, void * values, int * dataType,
	int * numberRead, int * startIndex, int * arrayStride, int * valueStride)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveOriginValues(values, *dataType, *numberRead, *startIndex, *arrayStride, *valueStride);
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
  XdmfRetrieveOriginProperty(long * pointer, int * index, char * key, int * keyLength, char * value, int * valueLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveOriginProperty(*index, key, *keyLength, value, *valueLength);
  }

  void
  XdmfRetrieveOriginPropertyByKey(long * pointer, char * key, char * value, int * valueLength)
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
  XdmfRetrieveBrickTag(long * pointer, char * tag, int * tagLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveBrickTag(tag, *tagLength);
  }

  void
  XdmfRetrieveBrickValues(long * pointer, void * values, int * dataType,
	int * numberRead, int * startIndex, int * arrayStride, int * valueStride)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveBrickValues(values, *dataType, *numberRead, *startIndex, *arrayStride, *valueStride);
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
  XdmfRetrieveBrickProperty(long * pointer, int * index, char * key, int * keyLength, char * value, int * valueLength)
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
  XdmfAddRemoteNodeID(long * pointer, int * index, int * localNodeID, int * remoteTaskID, int * remoteLocalNodeID)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->addRemoteNodeID(*index, *localNodeID, *remoteTaskID, *remoteLocalNodeID);
  }

  void
  XdmfRetrieveRemoteNodeIDs(long * pointer, int * index, int * localNodeID, int * remoteTaskID, int * remoteLocalNodeIDs)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveRemoteNodeIDs(*index, *localNodeID, *remoteTaskID, remoteLocalNodeIDs);
  }

  void
  XdmfRetrieveNumRemoteNodeIDs(long * pointer, int * index, int * localNodeID, int * remoteTaskID, int * total)
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
  XdmfRetrieveMapProperty(long * pointer, int * mapIndex, int * index, char * key, int * keyLength, char * value, int * valueLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveMapProperty(*mapIndex, *index, key, *keyLength, value, *valueLength);
  }

  void
  XdmfRetrieveMapPropertyByKey(long * pointer, int * index, char * key, char * value, int * valueLength)
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
  XdmfRetrieveNumAttributes(long * pointer, int * total)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    *total = xdmfFortran->retrieveNumAttributes();
  }

  void
  XdmfReplaceAttribute(long * pointer, int * index, char * name, int * attributeCenter,
	int * attributeType, int * numValues, int * arrayType, void * values)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->replaceAttribute(*index, name, *attributeCenter, *attributeType, *numValues, *arrayType, values);
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
  XdmfRetrieveAttributeValues(long * pointer, int * index,  void * values, int * dataType,
	int * numberRead, int * startIndex, int * arrayStride, int * valueStride)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveAttributeValues(*index, values, *dataType, *numberRead, *startIndex, *arrayStride, *valueStride);
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
  XdmfRetrieveAttributeProperty(long * pointer, int * attributeIndex, int * index, char * key, int * keyLength, char * value, int * valueLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveAttributeProperty(*attributeIndex, *index, key, *keyLength, value, *valueLength);
  }

  void
  XdmfRetrieveAttributePropertyByKey(long * pointer, int * index, char * key, char * value, int * valueLength)
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
  XdmfReplaceCoordinate(long * pointer, int * index, char * name, int * numValues, int * arrayType, void * values)
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
  XdmfRetrieveCoordinateValues(long * pointer, int * index,  void * values, int * dataType,
	int * numberRead, int * startIndex, int * arrayStride, int * valueStride)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveCoordinateValues(*index, values, *dataType, *numberRead, *startIndex, *arrayStride, *valueStride);
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
  XdmfRetrieveCoordinateNumProperties(long * pointer, int * index, int * total)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    *total = xdmfFortran->retrieveCoordinateNumProperties(*index);
  }

  void
  XdmfRetrieveCoordinateProperty(long * pointer, int * coordinateIndex, int * index, char * key, int * keyLength, char * value, int * valueLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveCoordinateProperty(*coordinateIndex, *index, key, *keyLength, value, *valueLength);
  }

  void
  XdmfRetrieveCoordinatePropertyByKey(long * pointer, int * index, char * key, char * value, int * valueLength)
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
  XdmfAddSet(long * pointer, char * name, int * newSetType, char * values, int * numValues, int * arrayType)
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
  XdmfRetrieveSetValues(long * pointer, int * index,  void * values, int * dataType,
	int * numberRead, int * startIndex, int * arrayStride, int * valueStride)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveSetValues(*index, values, *dataType, *numberRead, *startIndex, *arrayStride, *valueStride);
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
  XdmfReplaceSet(long * pointer, int * index, char * name, int * newSetType, char * values, int * numValues, int * arrayType)
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
  XdmfRetrieveSetProperty(long * pointer, int * setIndex, int * index, char * key, int * keyLength, char * value, int * valueLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveSetProperty(*setIndex, *index, key, *keyLength, value, *valueLength);
  }

  void
  XdmfRetrieveSetPropertyByKey(long * pointer, int * index, char * key, char * value, int * valueLength)
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
  XdmfRetrieveInformation(long * pointer, int * index, char * key, int * keyLength, char * value, int * valueLength)
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
  XdmfRetrieveInformationProperty(long * pointer, int * informationIndex, int * index, char * key, int * keyLength, char * value, int * valueLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveInformationProperty(*informationIndex, *index, key, *keyLength, value, *valueLength);
  }

  void
  XdmfRetrieveInformationPropertyByKey(long * pointer, int * index, char * key, char * value, int * valueLength)
  {
    XdmfFortran * xdmfFortran = reinterpret_cast<XdmfFortran *>(*pointer);
    xdmfFortran->retrieveInformationPropertyByKey(*index, key, value, *valueLength);
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
  
}
