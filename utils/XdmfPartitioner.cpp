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
#include "XdmfError.hpp"

#include "XdmfArrayType.hpp"

shared_ptr<XdmfPartitioner>
XdmfPartitioner::New()
{
  shared_ptr<XdmfPartitioner> p(new XdmfPartitioner());
  return p;
}

XdmfPartitioner::XdmfPartitioner()
{
}

XdmfPartitioner::~XdmfPartitioner()
{
}

void
XdmfPartitioner::ignore(const shared_ptr<const XdmfSet> set)
{
  mIgnoredSets.insert(set);
}

shared_ptr<XdmfGridCollection>
XdmfPartitioner::partition(const shared_ptr<XdmfUnstructuredGrid> gridToPartition,
                           const unsigned int numberOfPartitions,
                           const MetisScheme metisScheme,
                           const shared_ptr<XdmfHeavyDataWriter> heavyDataWriter) const
{
  // Make sure geometry and topology are non null
  if(!(gridToPartition->getGeometry() && gridToPartition->getTopology()))
    XdmfError::message(XdmfError::FATAL, 
                       "Current grid's geometry or topology is null in "
                       "XdmfPartitioner::partition");

  const shared_ptr<XdmfGeometry> geometry =
    gridToPartition->getGeometry();
  const shared_ptr<const XdmfGeometryType> geometryType =
    geometry->getType();
  const shared_ptr<XdmfTopology> topology =
    gridToPartition->getTopology();
  const shared_ptr<const XdmfTopologyType> topologyType =
    topology->getType();

  const unsigned int nodesPerElement = topologyType->getNodesPerElement();

  bool releaseTopology = false;
  if(!topology->isInitialized()) {
    topology->read();
    releaseTopology = true;
  }

  int numElements = topology->getNumberElements();
  int numNodes = geometry->getNumberPoints();
  
  // allocate metisConnectivity arrays
  idx_t * metisConnectivityEptr = new idx_t[numElements + 1];
  idx_t * metisConnectivityEind = new idx_t[nodesPerElement * numElements];
  
  metisConnectivityEptr[0] = 0;

  unsigned int metisConnectivityEptrValue = 0;
  unsigned int connectivityOffset = 0;
  idx_t * metisConnectivityPtr = metisConnectivityEind;
  for(int i=0; i<numElements; ++i) {
    metisConnectivityEptrValue += nodesPerElement;
    metisConnectivityEptr[i + 1] = metisConnectivityEptrValue;
    topology->getValues(connectivityOffset,
                        metisConnectivityPtr,
                        nodesPerElement);
    connectivityOffset += topologyType->getNodesPerElement();
    metisConnectivityPtr += nodesPerElement;
  }

  idx_t * vwgt = NULL; // equal weight
  idx_t * vsize = NULL; // equal size
  idx_t ncommon = 1; // FIXME
  idx_t nparts = numberOfPartitions;
  real_t * tpwgts = NULL;
  idx_t * options = NULL;
  idx_t objval;

  idx_t * elementsPartition = new idx_t[numElements];
  idx_t * nodesPartition = new idx_t[numNodes];
  
  if(metisScheme == DUAL_GRAPH) {
    METIS_PartMeshDual(&numElements,
                       &numNodes,
                       metisConnectivityEptr,
                       metisConnectivityEind,
                       vwgt,
                       vsize,
                       &ncommon,
                       &nparts,
                       tpwgts,
                       options,
                       &objval,
                       elementsPartition,
                       nodesPartition);
  }
  else if(metisScheme == NODAL_GRAPH) {
    METIS_PartMeshNodal(&numElements,
                        &numNodes,
                        metisConnectivityEptr,
                        metisConnectivityEind,
                        vwgt,
                        vsize,
                        &nparts,
                        tpwgts,
                        options,
                        &objval,
                        elementsPartition,
                        nodesPartition);
  }
  else {
    XdmfError::message(XdmfError::FATAL, 
                       "Invalid metis partitioning scheme selected in "
                       "XdmfPartitioner::partition");

  }

  delete [] metisConnectivityEptr;
  delete [] metisConnectivityEind;
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

  shared_ptr<XdmfGridCollection> partitionedGrids =
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

      shared_ptr<XdmfUnstructuredGrid> partitioned =
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
        shared_ptr<XdmfAttribute> globalNodeIds = XdmfAttribute::New();
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
    shared_ptr<XdmfAttribute> currAttribute =
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
        shared_ptr<XdmfUnstructuredGrid> partitioned =
          partitionedGrids->getUnstructuredGrid(partitionId);
        partitionId++;
        shared_ptr<XdmfAttribute> createdAttribute =
          shared_ptr<XdmfAttribute>();
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
          const unsigned int numberComponents = 
	    currAttribute->getSize() / topology->getNumberElements();
          createdAttribute->initialize(currAttribute->getArrayType(),
                                       currElemIds.size() * numberComponents);
          for(std::vector<unsigned int>::const_iterator iter =
                currElemIds.begin();
              iter != currElemIds.end();
              ++iter) {
            createdAttribute->insert(index,
                                     currAttribute,
                                     *iter * numberComponents,
                                     numberComponents);
            index += numberComponents;
          }
        }
        else if(currAttribute->getCenter() == XdmfAttributeCenter::Node()) {
          createdAttribute = XdmfAttribute::New();
          createdAttribute->setName(currAttribute->getName());
          createdAttribute->setCenter(currAttribute->getCenter());
          createdAttribute->setType(currAttribute->getType());
          createdAttribute->initialize(currAttribute->getArrayType(), 
				       currNodeMap.size());
	  const unsigned int numberComponents = 
	    currAttribute->getSize() / geometry->getNumberPoints();
          for(std::map<unsigned int, unsigned int>::const_iterator iter =
                currNodeMap.begin();
              iter != currNodeMap.end();
              ++iter) {
            createdAttribute->insert(iter->second * numberComponents,
                                     currAttribute,
                                     iter->first * numberComponents,
                                     numberComponents);
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
    shared_ptr<XdmfSet> currSet = gridToPartition->getSet(i);
    if(mIgnoredSets.find(currSet) == mIgnoredSets.end()) {
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
          shared_ptr<XdmfUnstructuredGrid> partitioned =
            partitionedGrids->getUnstructuredGrid(partitionId);
          partitionId++;

          shared_ptr<XdmfSet> partitionedSet = XdmfSet::New();
          std::vector<unsigned int> partitionedSetIndex;

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
                partitionedSetIndex.push_back(k);
              }
            }
          }
          else if(currSet->getType() == XdmfSetType::Node()) {
            for(unsigned int k=0; k<currSet->getSize(); ++k) {
              std::map<unsigned int, unsigned int>::const_iterator val =
                currNodeMap.find(currSet->getValue<unsigned int>(k));
              if(val != currNodeMap.end()) {
                partitionedSet->pushBack(val->second);
                partitionedSetIndex.push_back(k);
              }
            }
          }

          if(partitionedSet->getSize() > 0) {

            for(unsigned int k=0; k<currSet->getNumberAttributes(); ++k) {
              const shared_ptr<XdmfAttribute> currAttribute =
                currSet->getAttribute(k);
              if(currAttribute->getCenter() == XdmfAttributeCenter::Node()) {
                bool releaseAttribute = false;
                if(!currAttribute->isInitialized()) {
                  currAttribute->read();
                  releaseAttribute = true;
                }
                const shared_ptr<XdmfAttribute> partitionedAttribute =
                  XdmfAttribute::New();
                partitionedAttribute->setCenter(currAttribute->getCenter());
                partitionedAttribute->setName(currAttribute->getName());
                partitionedAttribute->setType(currAttribute->getType());
                partitionedAttribute->initialize(currAttribute->getArrayType(),
                                                 partitionedSetIndex.size());
                for(unsigned int l=0; l<partitionedSetIndex.size(); ++l) {
                  partitionedAttribute->insert(l,
                                               currAttribute,
                                               partitionedSetIndex[l]);
                }
                partitionedSet->insert(partitionedAttribute);
                if(heavyDataWriter) {
                  partitionedAttribute->accept(heavyDataWriter);
                  partitionedAttribute->release();
                }
              }
            }

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
  }

  // Add XdmfMap to map boundary nodes between partitions
  std::vector<shared_ptr<XdmfAttribute> > globalNodeIds;
  for(unsigned int i=0;
      i<partitionedGrids->getNumberUnstructuredGrids();
      ++i) {
    shared_ptr<XdmfAttribute> globalNodeId =
      partitionedGrids->getUnstructuredGrid(i)->getAttribute("GlobalNodeId");
    if(!globalNodeId->isInitialized()) {
      globalNodeId->read();
    }
    globalNodeIds.push_back(globalNodeId);
  }

  std::vector<shared_ptr<XdmfMap> > maps = XdmfMap::New(globalNodeIds);
  for(unsigned int i=0;
      i<partitionedGrids->getNumberUnstructuredGrids();
      ++i) {
    shared_ptr<XdmfMap> map = maps[i];
    map->setName("Subdomain Boundary");
    partitionedGrids->getUnstructuredGrid(i)->insert(map);
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

namespace {

  //
  // print usage
  //
  inline void
  printUsage(const char * programName)
  {

    std::cerr << "usage: " << programName << " "
              << "[-s metis_scheme] "
              << "<input file> <number of partitions> [output file]"
              << std::endl;
    std::cerr << "metis_scheme: 1 - Dual Graph" << std::endl;
    std::cerr << "metis_scheme: 2 - Node Graph" << std::endl;
 
    //
    //
    //
    return;

  }

  //
  // process command line
  //
  void
  processCommandLine(std::string                  & inputFileName,
                     std::string                  & outputFileName,
		     unsigned int                 & numPartitions,
                     XdmfPartitioner::MetisScheme & metisScheme,
		     int                            ac,
                     char                         * av[])
  {

    int c;
    bool errorFlag = false;
    
    while( (c=getopt(ac, av, "s:")) != -1 )
    switch(c){

    case 's': {
      const int value = std::atoi(optarg);
      if(value == 1) {
        metisScheme = XdmfPartitioner::DUAL_GRAPH;
      }
      else if(value == 2) {
        metisScheme = XdmfPartitioner::NODAL_GRAPH;
      }
      else {
        errorFlag = true;
      }
      break;
    }
    case '?':
      errorFlag = true;
      break;
    }

    if (optind >= ac)
      errorFlag = true;
    else {
      inputFileName = av[optind];
      ++optind;
    }

    if (optind >= ac)
      errorFlag = true;
    else {
      numPartitions = atoi(av[optind]);
      ++optind;
    }

    if (optind < ac) {
      outputFileName = av[optind];
      ++optind;
    }
   
    //
    // check errorFlag
    //
    if (errorFlag == true) {
      printUsage(av[0]);
      std::exit(EXIT_FAILURE);
    }

  }

}

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

  std::string inputFileName = "";
  std::string outputFileName = "";
  unsigned int numPartitions;
  XdmfPartitioner::MetisScheme metisScheme = XdmfPartitioner::DUAL_GRAPH;

  processCommandLine(inputFileName,
		     outputFileName,
		     numPartitions,
		     metisScheme,
		     argc,
		     argv);

  std::cout << inputFileName << std::endl;

  FILE * refFile = fopen(inputFileName.c_str(), "r");
  if (refFile) {
    // Success
    fclose(refFile);
  }
  else {
    std::cout << "Cannot open file: " << argv[1] << std::endl;
    return 1;
  }

  std::string meshName;
  if(outputFileName.compare("") == 0) {
    meshName = inputFileName;
  }
  else {
    meshName = outputFileName;
  }

  if(meshName.find_last_of("/\\") != std::string::npos) {
    meshName = meshName.substr(meshName.find_last_of("/\\") + 1,
                               meshName.length());
  }

  if (meshName.rfind(".") != std::string::npos) {
    meshName = meshName.substr(0, meshName.rfind("."));
  }

  if(outputFileName.compare("") == 0) {
    std::stringstream partitionedMeshName;
    partitionedMeshName << meshName << "_p" << numPartitions;
    meshName = partitionedMeshName.str();
  }

  shared_ptr<XdmfReader> reader = XdmfReader::New();
  shared_ptr<XdmfDomain> domain =
    shared_dynamic_cast<XdmfDomain>(reader->read(inputFileName));

  if(domain->getNumberUnstructuredGrids() <= 0) {
    std::cout << "No grids to partition" << std::endl;
    return 1;
  }

  std::stringstream heavyFileName;
  heavyFileName << meshName << ".h5";
  shared_ptr<XdmfHDF5Writer> heavyDataWriter =
    XdmfHDF5Writer::New(heavyFileName.str());

  shared_ptr<XdmfPartitioner> partitioner = XdmfPartitioner::New();
  shared_ptr<XdmfGridCollection> toWrite =
    partitioner->partition(domain->getUnstructuredGrid(0),
                           numPartitions,
                           metisScheme,
                           heavyDataWriter);

  shared_ptr<XdmfDomain> newDomain = XdmfDomain::New();
  newDomain->insert(toWrite);

  std::stringstream xmlFileName;
  xmlFileName << meshName << ".xmf";
  shared_ptr<XdmfWriter> writer = XdmfWriter::New(xmlFileName.str(),
                                                  heavyDataWriter);
  newDomain->accept(writer);

  std::cout << "Wrote: " << xmlFileName.str() << std::endl;
}

#endif // BUILD_EXE
