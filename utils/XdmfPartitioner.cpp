/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfPartitioner.cpp                                                 */
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

#ifndef BUILD_EXE

extern "C"
{
#include <metis.h>
}

#include <iostream>
#include <sstream>
#include "XdmfAttribute.hpp"
#include "XdmfAttributeCenter.hpp"
#include "XdmfAttributeType.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfGeometryType.hpp"
#include "XdmfGridCollection.hpp"
#include "XdmfGridCollectionType.hpp"
#include "XdmfHeavyDataWriter.hpp"
#include "XdmfMap.hpp"
#include "XdmfPartitioner.hpp"
#include "XdmfSet.hpp"
#include "XdmfSetType.hpp"
#include "XdmfTopology.hpp"
#include "XdmfTopologyType.hpp"
#include "XdmfUnstructuredGrid.hpp"

boost::shared_ptr<XdmfPartitioner>
XdmfPartitioner::New()
{
  boost::shared_ptr<XdmfPartitioner> p(new XdmfPartitioner());
  return p;
}

XdmfPartitioner::XdmfPartitioner()
{
}

XdmfPartitioner::~XdmfPartitioner()
{
}

boost::shared_ptr<XdmfGridCollection>
XdmfPartitioner::partition(const boost::shared_ptr<XdmfUnstructuredGrid> gridToPartition,
                           const unsigned int numberOfPartitions,
                           const boost::shared_ptr<XdmfHeavyDataWriter> heavyDataWriter) const
{
  // Make sure geometry and topology are non null
  assert(gridToPartition->getGeometry() && gridToPartition->getTopology());

  const boost::shared_ptr<XdmfGeometry> geometry =
    gridToPartition->getGeometry();
  const boost::shared_ptr<const XdmfGeometryType> geometryType =
    geometry->getType();
  const boost::shared_ptr<XdmfTopology> topology =
    gridToPartition->getTopology();
  const boost::shared_ptr<const XdmfTopologyType> topologyType =
    topology->getType();

  int metisElementType;
  unsigned int nodesPerElement;

  if(topologyType == XdmfTopologyType::Triangle() ||
     topologyType == XdmfTopologyType::Triangle_6()) {
    metisElementType = 1;
    nodesPerElement = 3;
  }
  else if(topologyType == XdmfTopologyType::Quadrilateral() ||
          topologyType == XdmfTopologyType::Quadrilateral_8()) {
    metisElementType = 4;
    nodesPerElement = 4;
  }
  else if(topologyType == XdmfTopologyType::Tetrahedron() ||
          topologyType == XdmfTopologyType::Tetrahedron_10()) {
    metisElementType = 2;
    nodesPerElement = 4;
  }
  else if(topologyType == XdmfTopologyType::Hexahedron() ||
          topologyType == XdmfTopologyType::Hexahedron_20() ||
          topologyType == XdmfTopologyType::Hexahedron_24() ||
          topologyType == XdmfTopologyType::Hexahedron_27() ||
          topologyType == XdmfTopologyType::Hexahedron_64()) {
    metisElementType = 3;
    nodesPerElement = 8;
  }
  else {
    assert(false);
  }

  bool releaseTopology = false;
  if(!topology->isInitialized()) {
    topology->read();
    releaseTopology = true;
  }

  int numElements = topology->getNumberElements();
  idxtype * metisConnectivity = new idxtype[nodesPerElement * numElements];
  unsigned int connectivityOffset = 0;
  idxtype * metisConnectivityPtr = metisConnectivity;
  for(int i=0; i<numElements; ++i) {
    topology->getValues(connectivityOffset,
                        metisConnectivityPtr,
                        nodesPerElement);
    connectivityOffset += topologyType->getNodesPerElement();
    metisConnectivityPtr += nodesPerElement;
  }

  int numNodes = geometry->getNumberPoints();

  // Need to remap connectivity for nonlinear elements so that metis handles
  // it properly.
  std::map<idxtype, idxtype> xdmfIdToMetisId;
  if(nodesPerElement != topologyType->getNodesPerElement()) {
    unsigned int index = 0;
    for (unsigned int i=0; i<numElements * nodesPerElement; ++i) {
      std::map<idxtype, idxtype>::const_iterator val =
        xdmfIdToMetisId.find(metisConnectivity[i]);
      if (val != xdmfIdToMetisId.end()) {
        metisConnectivity[i] = val->second;
      }
      else {
        // Haven't seen this id before, map to index and set to new id
        xdmfIdToMetisId[metisConnectivity[i]] = index;
        metisConnectivity[i] = index;
        index++;
      }
    }
    numNodes = index;
  }

  int startIndex = 0;
  int numCutEdges = 0;

  idxtype * elementsPartition = new idxtype[numElements];
  idxtype * nodesPartition = new idxtype[numNodes];

  METIS_PartMeshDual(&numElements,
                     &numNodes,
                     metisConnectivity,
                     &metisElementType,
                     &startIndex,
                     (int*)&numberOfPartitions,
                     &numCutEdges,
                     elementsPartition,
                     nodesPartition);
  /*
  METIS_PartMeshNodal(&numElements,
                      &numNodes,
                      metisConnectivity,
                      &metisElementType,
                      &startIndex,
                      &numPartitions,
                      &numCutEdges,
                      elementsPartition,
                      nodesPartition);
  */
  delete [] metisConnectivity;
  delete [] nodesPartition;

  // For each partition, map global to local node id
  std::vector<std::map<unsigned int, unsigned int> > globalToLocalNodeIdMap;
  // For each partition, list global element id.
  std::vector<std::vector<unsigned int> > globalElementIds;
  for(unsigned int i=0; i<numberOfPartitions; ++i) {
    std::map<unsigned int, unsigned int> nodeMap;
    globalToLocalNodeIdMap.push_back(nodeMap);
    std::vector<unsigned int> elementIds;
    globalElementIds.push_back(elementIds);
  }

  // Fill in globalNodeId for each partition
  unsigned int totalIndex = 0;
  for (int i=0; i<numElements; ++i) {
    unsigned int partitionId = elementsPartition[i];
    for (unsigned int j=0; j<topologyType->getNodesPerElement(); ++j) {
      unsigned int globalNodeId = topology->getValue<unsigned int>(totalIndex);
      if (globalToLocalNodeIdMap[partitionId].count(globalNodeId) == 0) {
        // Have not seen this node, need to add to map
        unsigned int size = globalToLocalNodeIdMap[partitionId].size();
        globalToLocalNodeIdMap[partitionId][globalNodeId] = size;
      }
      totalIndex++;
    }
    globalElementIds[partitionId].push_back(i);
  }
  delete [] elementsPartition;

  bool generateGlobalNodeIds = !gridToPartition->getAttribute("GlobalNodeId");

  boost::shared_ptr<XdmfGridCollection> partitionedGrids =
    XdmfGridCollection::New();
  partitionedGrids->setType(XdmfGridCollectionType::Spatial());

  bool releaseGeometry = false;
  if(!geometry->isInitialized()) {
    geometry->read();
    releaseGeometry = true;
  }

  // Split geometry and topology into proper partitions
  for(unsigned int i=0; i<numberOfPartitions; ++i) {
    std::map<unsigned int, unsigned int> & currNodeMap =
      globalToLocalNodeIdMap[i];
    std::vector<unsigned int> & currElemIds = globalElementIds[i];

    if(currElemIds.size() > 0) {
      std::stringstream name;
      name << gridToPartition->getName() << "_" << i;

      boost::shared_ptr<XdmfUnstructuredGrid> partitioned =
        XdmfUnstructuredGrid::New();
      partitioned->setName(name.str());
      partitionedGrids->insert(partitioned);

      // Fill in geometry for this partition
      partitioned->getGeometry()->setType(geometryType);
      unsigned int numDimensions = geometryType->getDimensions();
      partitioned->getGeometry()->initialize(geometry->getArrayType(),
                                             currNodeMap.size() * numDimensions);

      for(std::map<unsigned int, unsigned int>::const_iterator iter =
            currNodeMap.begin();
          iter != currNodeMap.end();
          ++iter) {
        partitioned->getGeometry()->insert(iter->second * numDimensions,
                                           geometry,
                                           iter->first * numDimensions,
                                           numDimensions);
      }

      if(heavyDataWriter) {
        partitioned->getGeometry()->accept(heavyDataWriter);
        partitioned->getGeometry()->release();
      }

      // Fill in topology for this partition
      partitioned->getTopology()->setType(topologyType);
      partitioned->getTopology()->initialize(topology->getArrayType(),
                                             currElemIds.size() * topologyType->getNodesPerElement());
      unsigned int index = 0;
      for(std::vector<unsigned int>::const_iterator iter = currElemIds.begin();
          iter != currElemIds.end();
          ++iter) {
        // Translate these global node ids to local node ids
        for(unsigned int j=0; j<topologyType->getNodesPerElement(); ++j) {
          unsigned int globalNodeId =
            currNodeMap[topology->getValue<unsigned int>(*iter * topologyType->getNodesPerElement() + j)];
          partitioned->getTopology()->insert(index, &globalNodeId, 1);
          index++;
        }
      }

      if(heavyDataWriter) {
        partitioned->getTopology()->accept(heavyDataWriter);
        partitioned->getTopology()->release();
      }

      if (generateGlobalNodeIds) {
        boost::shared_ptr<XdmfAttribute> globalNodeIds = XdmfAttribute::New();
        globalNodeIds->setName("GlobalNodeId");
        globalNodeIds->setType(XdmfAttributeType::GlobalId());
        globalNodeIds->setCenter(XdmfAttributeCenter::Node());
        globalNodeIds->initialize<unsigned int>(currNodeMap.size());
        for(std::map<unsigned int, unsigned int>::const_iterator iter =
              currNodeMap.begin();
            iter != currNodeMap.end();
            ++iter) {
          globalNodeIds->insert(iter->second, &iter->first, 1);
        }
        partitioned->insert(globalNodeIds);

        if(heavyDataWriter) {
          globalNodeIds->accept(heavyDataWriter);
          globalNodeIds->release();
        }
      }
    }
  }

  if(releaseGeometry) {
    gridToPartition->getGeometry()->release();
  }
  if(releaseTopology) {
    gridToPartition->getTopology()->release();
  }

  // Split attributes into proper partitions
  for(unsigned int i=0; i<gridToPartition->getNumberAttributes(); ++i) {
    boost::shared_ptr<XdmfAttribute> currAttribute =
      gridToPartition->getAttribute(i);
    bool releaseAttribute = false;
    if(!currAttribute->isInitialized()) {
      currAttribute->read();
      releaseAttribute = true;
    }
    unsigned int partitionId = 0;
    for(unsigned int j=0; j<numberOfPartitions; ++j) {
      std::map<unsigned int, unsigned int> & currNodeMap =
        globalToLocalNodeIdMap[j];
      std::vector<unsigned int> & currElemIds = globalElementIds[j];
      if(currElemIds.size() > 0) {
        boost::shared_ptr<XdmfUnstructuredGrid> partitioned =
          partitionedGrids->getUnstructuredGrid(partitionId);
        partitionId++;
        boost::shared_ptr<XdmfAttribute> createdAttribute =
          boost::shared_ptr<XdmfAttribute>();
        if(currAttribute->getCenter() == XdmfAttributeCenter::Grid()) {
          // Insert into each partition
          createdAttribute = currAttribute;
        }
        else if(currAttribute->getCenter() == XdmfAttributeCenter::Cell() ||
                currAttribute->getCenter() == XdmfAttributeCenter::Face() ||
                currAttribute->getCenter() == XdmfAttributeCenter::Edge()) {
          createdAttribute = XdmfAttribute::New();
          createdAttribute->setName(currAttribute->getName());
          createdAttribute->setCenter(currAttribute->getCenter());
          createdAttribute->setType(currAttribute->getType());
          unsigned int index = 0;
          unsigned int numValsPerComponent =
            currAttribute->getSize() / topology->getNumberElements();
          createdAttribute->initialize(currAttribute->getArrayType(),
                                       currElemIds.size() * numValsPerComponent);
          for(std::vector<unsigned int>::const_iterator iter =
                currElemIds.begin();
              iter != currElemIds.end();
              ++iter) {
            createdAttribute->insert(index,
                                     currAttribute,
                                     *iter * numValsPerComponent,
                                     numValsPerComponent);
            index += numValsPerComponent;
          }
        }
        else if(currAttribute->getCenter() == XdmfAttributeCenter::Node()) {
          createdAttribute = XdmfAttribute::New();
          createdAttribute->setName(currAttribute->getName());
          createdAttribute->setCenter(currAttribute->getCenter());
          createdAttribute->setType(currAttribute->getType());
          createdAttribute->initialize(currAttribute->getArrayType(), currNodeMap.size());
          for(std::map<unsigned int, unsigned int>::const_iterator iter =
                currNodeMap.begin();
              iter != currNodeMap.end();
              ++iter) {
            createdAttribute->insert(iter->second,
                                     currAttribute,
                                     iter->first,
                                     1);
          }
        }
        if(createdAttribute) {
          partitioned->insert(createdAttribute);
          if(heavyDataWriter) {
            if(!createdAttribute->isInitialized()) {
              createdAttribute->read();
            }
            createdAttribute->accept(heavyDataWriter);
            createdAttribute->release();
          }
        }
      }
    }
    if(releaseAttribute) {
      currAttribute->release();
    }
  }

  // Split sets into proper partitions
  for(unsigned int i=0; i<gridToPartition->getNumberSets(); ++i) {
    boost::shared_ptr<XdmfSet> currSet = gridToPartition->getSet(i);
    bool releaseSet = false;
    if(!currSet->isInitialized()) {
      currSet->read();
      releaseSet = true;
    }
    unsigned int partitionId = 0;
    for(unsigned int j=0; j<numberOfPartitions; ++j) {
      std::map<unsigned int, unsigned int> & currNodeMap =
        globalToLocalNodeIdMap[j];
      std::vector<unsigned int> & currElemIds = globalElementIds[j];
      if(currElemIds.size() > 0) {
        boost::shared_ptr<XdmfUnstructuredGrid> partitioned =
          partitionedGrids->getUnstructuredGrid(partitionId);
        partitionId++;

        boost::shared_ptr<XdmfSet> partitionedSet = XdmfSet::New();

        if(currSet->getType() == XdmfSetType::Cell() ||
           currSet->getType() == XdmfSetType::Face() ||
           currSet->getType() == XdmfSetType::Edge()) {
          for(unsigned int k=0; k<currSet->getSize(); ++k) {
            std::vector<unsigned int>::const_iterator val =
              std::find(currElemIds.begin(),
                        currElemIds.end(),
                        currSet->getValue<unsigned int>(k));
            if(val != currElemIds.end()) {
              unsigned int valToPush = val - currElemIds.begin();
              partitionedSet->pushBack(valToPush);
            }
          }
        }
        else if(currSet->getType() == XdmfSetType::Node()) {
          for(unsigned int k=0; k<currSet->getSize(); ++k) {
            std::map<unsigned int, unsigned int>::const_iterator val =
              currNodeMap.find(currSet->getValue<unsigned int>(k));
            if(val != currNodeMap.end()) {
              partitionedSet->pushBack(val->second);
            }
          }
        }
        if(partitionedSet->getSize() > 0) {
          partitioned->insert(partitionedSet);
          partitionedSet->setName(currSet->getName());
          partitionedSet->setType(currSet->getType());
          if(heavyDataWriter) {
            partitionedSet->accept(heavyDataWriter);
            partitionedSet->release();
          }
        }
      }
    }
    if(releaseSet) {
      currSet->release();
    }
  }

  // Add XdmfMap to map boundary nodes between partitions
  std::vector<boost::shared_ptr<XdmfAttribute> > globalNodeIds;
  for(unsigned int i=0;
      i<partitionedGrids->getNumberUnstructuredGrids();
      ++i) {
    boost::shared_ptr<XdmfAttribute> globalNodeId =
      partitionedGrids->getUnstructuredGrid(i)->getAttribute("GlobalNodeId");
      if(!globalNodeId->isInitialized()) {
        globalNodeId->read();
      }
      globalNodeIds.push_back(globalNodeId);
  }

  std::vector<boost::shared_ptr<XdmfMap> > maps = XdmfMap::New(globalNodeIds);
  for(unsigned int i=0;
      i<partitionedGrids->getNumberUnstructuredGrids();
      ++i) {
    boost::shared_ptr<XdmfMap> map = maps[i];
    partitionedGrids->getUnstructuredGrid(i)->setMap(map);
    if(heavyDataWriter) {
      globalNodeIds[i]->release();
      map->accept(heavyDataWriter);
      map->release();
    }
  }

  return partitionedGrids;
}

#else

#include <iostream>
#include <sstream>
#include "XdmfDomain.hpp"
#include "XdmfGridCollection.hpp"
#include "XdmfHDF5Writer.hpp"
#include "XdmfPartitioner.hpp"
#include "XdmfReader.hpp"
#include "XdmfWriter.hpp"

/**
 * XdmfPartitioner is a command line utility for partitioning Xdmf grids.
 * The XdmfPartitioner uses the metis library to partition Triangular,
 * Quadrilateral, Tetrahedral, and Hexahedral XdmfGridUnstructureds.
 *
 * Usage:
 *     XdmfPartitioner <path-of-file-to-partition> <num-partitions>
 *                     (Optional: <path-to-output-file>)
 *
 */
int main(int argc, char* argv[])
{
  std::string usage = "Partitions an Xdmf grid using the metis library: \n \n "
    "Usage: \n \n   XdmfPartitioner <path-of-file-to-partition> "
    "<num-partitions> (Optional: <path-to-output-file>)";
  std::string meshName = "";

  if (argc < 3) {
    std::cout << usage << std::endl;
    return 1;
  }

  FILE * refFile = fopen(argv[1], "r");
  if (refFile) {
    // Success
    meshName = argv[1];
    fclose(refFile);
  }
  else {
    std::cout << "Cannot open file: " << argv[1] << std::endl;
    return 1;
  }

  unsigned int numPartitions = atoi(argv[2]);

  if (argc >= 4) {
    meshName = argv[3];
  }

  if(meshName.find_last_of("/\\") != std::string::npos) {
    meshName = meshName.substr(meshName.find_last_of("/\\")+1,
                               meshName.length());
  }

  if (meshName.rfind(".") != std::string::npos) {
    meshName = meshName.substr(0, meshName.rfind("."));
  }

  if(argc < 4) {
    meshName = meshName + "-partitioned";
  }

  boost::shared_ptr<XdmfReader> reader = XdmfReader::New();
  boost::shared_ptr<XdmfDomain> domain =
    boost::shared_dynamic_cast<XdmfDomain>(reader->read(argv[1]));

  if(domain->getNumberUnstructuredGrids() <= 0) {
    std::cout << "No grids to partition" << std::endl;
    return 1;
  }

  std::stringstream heavyFileName;
  heavyFileName << meshName << ".h5";
  boost::shared_ptr<XdmfHDF5Writer> heavyDataWriter =
    XdmfHDF5Writer::New(heavyFileName.str());

  boost::shared_ptr<XdmfPartitioner> partitioner = XdmfPartitioner::New();
  boost::shared_ptr<XdmfGridCollection> toWrite =
    partitioner->partition(domain->getUnstructuredGrid(0),
                           numPartitions,
                           heavyDataWriter);

  boost::shared_ptr<XdmfDomain> newDomain = XdmfDomain::New();
  newDomain->insert(toWrite);

  std::stringstream xmlFileName;
  xmlFileName << meshName << ".xmf";
  boost::shared_ptr<XdmfWriter> writer = XdmfWriter::New(xmlFileName.str(),
                                                         heavyDataWriter);
  newDomain->accept(writer);

  std::cout << "Wrote: " << xmlFileName.str() << std::endl;
}

#endif //BUILD_EXE
