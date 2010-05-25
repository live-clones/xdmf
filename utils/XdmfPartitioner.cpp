/*******************************************************************/
/*                               XDMF                              */
/*                   eXtensible Data Model and Format              */
/*                                                                 */
/*  Id : Id  */
/*  Date : $Date$ */
/*  Version : $Revision$ */
/*                                                                 */
/*  Author:                                                        */
/*     Kenneth Leiter                                              */
/*     kenneth.leiter@arl.army.mil                                 */
/*     US Army Research Laboratory                                 */
/*     Aberdeen Proving Ground, MD                                 */
/*                                                                 */
/*     Copyright @ 2010 US Army Research Laboratory                */
/*     All Rights Reserved                                         */
/*     See Copyright.txt or http://www.arl.hpc.mil/ice for details */
/*                                                                 */
/*     This software is distributed WITHOUT ANY WARRANTY; without  */
/*     even the implied warranty of MERCHANTABILITY or FITNESS     */
/*     FOR A PARTICULAR PURPOSE.  See the above copyright notice   */
/*     for more information.                                       */
/*                                                                 */
/*******************************************************************/

#include "XdmfPartitioner.hpp"


#ifndef BUILD_EXE

extern "C"
{
#include <metis.h>
}

#include "XdmfGrid.hpp"

XdmfPartitioner::XdmfPartitioner()
{
}

XdmfPartitioner::~XdmfPartitioner()
{
}

boost::shared_ptr<XdmfGrid> XdmfPartitioner::partition(boost::shared_ptr<XdmfGrid> gridToPartition, const unsigned int numberOfPartitions)
{
  /*int metisElementType;
  int numNodesPerElement;
  switch(grid->GetTopology()->GetTopologyType())
  {
    case(XDMF_TRI):
    case(XDMF_TRI_6):
      metisElementType = 1;
      numNodesPerElement = 3;
      break;
    case(XDMF_QUAD):
    case(XDMF_QUAD_8):
      metisElementType = 4;
      numNodesPerElement = 4;
      break;
    case(XDMF_TET):
    case(XDMF_TET_10):
      metisElementType = 2;
      numNodesPerElement = 4;
      break;
    case(XDMF_HEX):
    case(XDMF_HEX_20):
    case(XDMF_HEX_24):
    case(XDMF_HEX_27):
    case(XDMF_HEX_64):
      metisElementType = 3;
      numNodesPerElement = 8;
      break;
    default:
      std::cout << "Cannot partition grid with element type: " << grid->GetTopology()->GetTopologyTypeAsString() << std::endl;
      return NULL;
  }

  int numElements = grid->GetTopology()->GetNumberOfElements();

  idxtype * metisConnectivity = new idxtype[numNodesPerElement * numElements];
  for(int i=0; i<numElements; ++i)
  {
    grid->GetTopology()->GetConnectivity()->GetValues(i*grid->GetTopology()->GetNodesPerElement(), &metisConnectivity[i*numNodesPerElement], numNodesPerElement);
  }

  int numNodes = grid->GetGeometry()->GetNumberOfPoints();

  // Need to remap connectivity for nonlinear elements so that metis handles it properly
  std::map<idxtype, idxtype> xdmfIdToMetisId;
  if(numNodesPerElement != grid->GetTopology()->GetNodesPerElement())
  {
    int index = 0;
    for(int i=0; i<numElements * numNodesPerElement; ++i)
    {
      std::map<idxtype, idxtype>::const_iterator val = xdmfIdToMetisId.find(metisConnectivity[i]);
      if(val != xdmfIdToMetisId.end())
      {
        metisConnectivity[i] = val->second;
      }
      else  
      {
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

  std::cout << "Entered METIS" << std::endl;
  METIS_PartMeshDual(&numElements, &numNodes, metisConnectivity, &metisElementType, &startIndex, &numPartitions, &numCutEdges, elementsPartition, nodesPartition);
  //METIS_PartMeshNodal(&numElements, &numNodes, metisConnectivity, &metisElementType, &startIndex, &numPartitions, &numCutEdges, elementsPartition, nodesPartition);
  std::cout << "Exited METIS" << std::endl;

  delete [] metisConnectivity;
  delete [] nodesPartition;

  // For each partition, map global to local node id
  std::vector<std::map<XdmfInt32, XdmfInt32> > globalToLocalNodeIdMap;
  // For each partition, list global element id.
  std::vector<std::vector<XdmfInt32> > globalElementIds;
  for(int i=0; i<numPartitions; ++i)
  {
    std::map<XdmfInt32, XdmfInt32> nodeMap;
    globalToLocalNodeIdMap.push_back(nodeMap);
    std::vector<XdmfInt32> elementIds;
    globalElementIds.push_back(elementIds);
  }

  // Fill in globalNodeId for each partition
  XdmfInt32 globalNodeId;
  for(int i=0; i<numElements; ++i)
  {
    for(int j=0; j<grid->GetTopology()->GetNodesPerElement(); ++j)
    {
      grid->GetTopology()->GetConnectivity()->GetValues(i*grid->GetTopology()->GetNodesPerElement() + j, &globalNodeId, 1);
      if(globalToLocalNodeIdMap[elementsPartition[i]].count(globalNodeId) == 0)
      {
        // Have not seen this node, need to add to map
        int size = globalToLocalNodeIdMap[elementsPartition[i]].size();
        globalToLocalNodeIdMap[elementsPartition[i]][globalNodeId] = size;
      }
    }
    globalElementIds[elementsPartition[i]].push_back(i);
  }

  delete [] elementsPartition;

  bool addGlobalNodeId = true;
  for(int i=0; i<grid->GetNumberOfAttributes(); ++i)
  {
    if(strcmp(grid->GetAttribute(i)->GetName(), "GlobalNodeId") == 0)
    {
      addGlobalNodeId = false;
    }
  }

  XdmfGrid * collection = new XdmfGrid();
  collection->SetName("Collection");
  collection->SetGridType(XDMF_GRID_COLLECTION);
  collection->SetCollectionType(XDMF_GRID_COLLECTION_SPATIAL);
  collection->SetDeleteOnGridDelete(true);

  parentElement->Insert(collection);

  std::vector<XdmfGrid*> partitions;

  for(int i=0; i<numPartitions; ++i)
  {
    std::map<XdmfInt32, XdmfInt32> currNodeMap = globalToLocalNodeIdMap[i];
    std::vector<XdmfInt32> currElemIds = globalElementIds[i];

    if(currElemIds.size() > 0)
    {
      std::stringstream name;
      name << grid->GetName() << "_" << i;

      XdmfGrid * partition = new XdmfGrid();
      partition->SetName(name.str().c_str());
      partition->SetDeleteOnGridDelete(true);
      partitions.push_back(partition);

      int numDimensions = 3;
      if(grid->GetGeometry()->GetGeometryType() == XDMF_GEOMETRY_XY || grid->GetGeometry()->GetGeometryType() == XDMF_GEOMETRY_X_Y)
      {
        numDimensions = 2;
      }

      XdmfGeometry * geom = partition->GetGeometry();
      geom->SetGeometryType(grid->GetGeometry()->GetGeometryType());
      geom->SetNumberOfPoints(currNodeMap.size());
      geom->SetDeleteOnGridDelete(true);

      XdmfArray * points = geom->GetPoints();
      points->SetNumberType(grid->GetGeometry()->GetPoints()->GetNumberType());
      points->SetNumberOfElements(currNodeMap.size() * numDimensions);
      for(std::map<XdmfInt32, XdmfInt32>::const_iterator iter = currNodeMap.begin(); iter != currNodeMap.end(); ++iter)
      {
        points->SetValues(iter->second * numDimensions, grid->GetGeometry()->GetPoints(), numDimensions, iter->first * 3);
      }

      XdmfTopology * top = partition->GetTopology();
      top->SetTopologyType(grid->GetTopology()->GetTopologyType());
      top->SetNumberOfElements(currElemIds.size());
      top->SetDeleteOnGridDelete(true);

      XdmfArray * connections = top->GetConnectivity();
      connections->SetNumberType(grid->GetTopology()->GetConnectivity()->GetNumberType());
      connections->SetNumberOfElements(currElemIds.size() * grid->GetTopology()->GetNodesPerElement());
      XdmfInt32 currGlobalNodeId;
      int index = 0;
      for(std::vector<XdmfInt32>::const_iterator iter = currElemIds.begin(); iter != currElemIds.end(); ++iter)
      {
        // Translate these global node ids to local node ids
        for(int j=0; j<grid->GetTopology()->GetNodesPerElement(); ++j)
        {
          grid->GetTopology()->GetConnectivity()->GetValues(*iter * grid->GetTopology()->GetNodesPerElement() + j, &currGlobalNodeId, 1);
          connections->SetValues(index, &currNodeMap[currGlobalNodeId], 1);
          index++;
        }
      }
      collection->Insert(partition);

      // Add GlobalNodeId Attribute
      if(addGlobalNodeId)
      {
        XdmfAttribute * globalIds = new XdmfAttribute();
        globalIds->SetName("GlobalNodeId");
        globalIds->SetAttributeType(XDMF_ATTRIBUTE_TYPE_SCALAR);
        globalIds->SetAttributeCenter(XDMF_ATTRIBUTE_CENTER_NODE);
        globalIds->SetDeleteOnGridDelete(true);

        XdmfArray * globalNodeIdVals = globalIds->GetValues();
        globalNodeIdVals->SetNumberType(XDMF_INT32_TYPE);
        globalNodeIdVals->SetNumberOfElements(currNodeMap.size());
        for(std::map<XdmfInt32, XdmfInt32>::const_iterator iter = currNodeMap.begin(); iter != currNodeMap.end(); ++iter)
        {
          globalNodeIdVals->SetValues(iter->second, (XdmfInt32*)&iter->first, 1);
        }
        partition->Insert(globalIds);
      }
    }
  } 

  grid->GetGeometry()->Release();
  grid->GetTopology()->Release();

  for(int j=0; j<grid->GetNumberOfAttributes(); ++j)
  {
    XdmfAttribute * currAttribute = grid->GetAttribute(j);
    // If data wasn't read in before, make sure it's released after processing
    bool releaseData = 0;
    if(currAttribute->GetValues()->GetNumberOfElements() == 0)
    {
      currAttribute->Update();
      releaseData = 1;
    }
    int partitionId = 0;
    for(int i=0; i<numPartitions; ++i)
    {
      std::map<XdmfInt32, XdmfInt32> currNodeMap = globalToLocalNodeIdMap[i];
      std::vector<XdmfInt32> currElemIds = globalElementIds[i];
      if(currElemIds.size() > 0)
      {
        XdmfGrid * partition = partitions[partitionId];
        partitionId++;
        switch(currAttribute->GetAttributeCenter())
        {
          case(XDMF_ATTRIBUTE_CENTER_GRID):
          {
            // Will continue to be true for entire collection - so insert at top level
            if(partitionId == 0)
            {
              collection->Insert(currAttribute);
            }
            break;
          }
          case(XDMF_ATTRIBUTE_CENTER_CELL):
          case(XDMF_ATTRIBUTE_CENTER_FACE):
          case(XDMF_ATTRIBUTE_CENTER_EDGE):
          {
            XdmfAttribute * attribute = new XdmfAttribute();
            attribute->SetName(currAttribute->GetName());
            attribute->SetAttributeType(currAttribute->GetAttributeType());
            attribute->SetAttributeCenter(currAttribute->GetAttributeCenter());
            attribute->SetDeleteOnGridDelete(true);
            XdmfArray * attributeVals = attribute->GetValues();
            attributeVals->SetNumberType(currAttribute->GetValues()->GetNumberType());
            int numValsPerComponent = currAttribute->GetValues()->GetNumberOfElements() / grid->GetTopology()->GetNumberOfElements();
            attributeVals->SetNumberOfElements(currElemIds.size() * numValsPerComponent);
            int index = 0;
            for(std::vector<XdmfInt32>::const_iterator iter = currElemIds.begin(); iter != currElemIds.end(); ++iter, ++index)
            {
              attributeVals->SetValues(index * numValsPerComponent, currAttribute->GetValues(), numValsPerComponent, *iter * numValsPerComponent);
            }
            partition->Insert(attribute);
            break;
          }
          case(XDMF_ATTRIBUTE_CENTER_NODE):
          {
            XdmfAttribute * attribute = new XdmfAttribute();
            attribute->SetName(currAttribute->GetName());
            attribute->SetAttributeType(currAttribute->GetAttributeType());
            attribute->SetAttributeCenter(currAttribute->GetAttributeCenter());
            attribute->SetDeleteOnGridDelete(true);
            XdmfArray * attributeVals = attribute->GetValues();
            attributeVals->SetNumberType(currAttribute->GetValues()->GetNumberType());
            attributeVals->SetNumberOfElements(currNodeMap.size());
            for(std::map<XdmfInt32, XdmfInt32>::const_iterator iter = currNodeMap.begin(); iter != currNodeMap.end(); ++iter)
            {
              attributeVals->SetValues(iter->second, currAttribute->GetValues(), 1, iter->first);
            }
            partition->Insert(attribute);
            break;
          }
          default:
          {
            std::cout << "Unknown attribute center encountered: " << currAttribute->GetAttributeCenterAsString() << std::endl;
            break;
          }
        }
      }
    }
    if(releaseData)
    {
      currAttribute->Release();
    }
  }

  // Split sets and add to grid
  for(int j=0; j<grid->GetNumberOfSets(); ++j)
  {
    XdmfSet * currSet = grid->GetSets(j);
    // If data wasn't read in before, make sure it's released after processing
    bool releaseData = 0;
    if(currSet->GetIds()->GetNumberOfElements() == 0)
    {
      currSet->Update();
      releaseData = 1;
    }
    int partitionId = 0;
    for(int i=0; i<numPartitions; ++i)
    {
      std::map<XdmfInt32, XdmfInt32> currNodeMap = globalToLocalNodeIdMap[i];
      std::vector<XdmfInt32> currElemIds = globalElementIds[i];
      if(currElemIds.size() > 0)
      {
        XdmfGrid * partition = partitions[partitionId];
        partitionId++;
        switch(currSet->GetSetType())
        {
          case(XDMF_SET_TYPE_CELL):
          case(XDMF_SET_TYPE_FACE):
          case(XDMF_SET_TYPE_EDGE):
          {
            std::vector<XdmfInt32> myIds;
            for(int j=0; j<currSet->GetIds()->GetNumberOfElements(); ++j)
            {
              std::vector<XdmfInt32>::const_iterator val = std::find(currElemIds.begin(), currElemIds.end(), currSet->GetIds()->GetValueAsInt32(j));
              if(val != currElemIds.end())
              {
                myIds.push_back(val - currElemIds.begin());
              }
            }
            if(myIds.size() != 0)
            {
              XdmfSet * set = new XdmfSet();
              set->SetName(currSet->GetName());
              set->SetSetType(currSet->GetSetType());
              set->SetSize(myIds.size());
              set->SetDeleteOnGridDelete(true);
              XdmfArray * ids = set->GetIds();
              ids->SetNumberType(XDMF_INT32_TYPE);
              ids->SetNumberOfElements(myIds.size());
              ids->SetValues(0, &myIds[0], myIds.size());
              partition->Insert(set);
            }
            break;
          }
          case(XDMF_SET_TYPE_NODE):
          {
            std::vector<XdmfInt32> myIds;
            for(int j=0; j<currSet->GetIds()->GetNumberOfElements(); j++)
            {
              std::map<XdmfInt32, XdmfInt32>::const_iterator val = currNodeMap.find(currSet->GetIds()->GetValueAsInt32(j));
              if(val != currNodeMap.end())
              {
                myIds.push_back(val->second);
              }
            }
            if(myIds.size() != 0)
            {
              XdmfSet * set = new XdmfSet();
              set->SetName(currSet->GetName());
              set->SetSetType(currSet->GetSetType());
              set->SetSize(myIds.size());
              set->SetDeleteOnGridDelete(true);
              XdmfArray * ids = set->GetIds();
              ids->SetNumberType(XDMF_INT32_TYPE);
              ids->SetNumberOfElements(myIds.size());
              ids->SetValues(0, &myIds[0], myIds.size());
              partition->Insert(set);
            }
            break;
          }
          default:
          {
            std::cout << "Unknown set type encountered: " << currSet->GetSetTypeAsString() << std::endl;  
            break;
          }
        }
      }
    }
    if(releaseData)
    {
      currSet->Release();
    }
  }
  return collection;*/
}

//#else

/**
 * XdmfPartitioner is a command line utility for partitioning Xdmf grids.
 * The XdmfPartitioner uses the metis library to partition Triangular, Quadrilateral, Tetrahedral,
 * and Hexahedral XdmfGrids.
 *
 * Usage:
 *     XdmfPartitioner <path-of-file-to-partition> <num-partitions> (Optional: <path-to-output-file>)
 *
 */
/*int main(int argc, char* argv[])
{
  std::string usage = "Partitions an XDMF grid using the metis library: \n \n Usage: \n \n   XdmfPartitioner <path-of-file-to-partition> <num-partitions> (Optional: <path-to-output-file>)";
  std::string meshName = "";

  if (argc < 3)
  {
    cout << usage << endl;
    return 1;
  }

  FILE * refFile = fopen(argv[1], "r");
  if (refFile)
  {
    // Success
    meshName = argv[1];
    fclose(refFile);
  }
  else
  {
    cout << "Cannot open file: " << argv[1] << endl;
    return 1;
  }

  int numPartitions = atoi(argv[2]);

  if (argc >= 4)
  {
    meshName = argv[3];
  }
  
  if(meshName.find_last_of("/\\") != std::string::npos)
  {
    meshName = meshName.substr(meshName.find_last_of("/\\")+1, meshName.length());
  }

  if (meshName.rfind(".") != std::string::npos)
  {
    meshName = meshName.substr(0, meshName.rfind("."));
  }

  if(argc < 4)
  {
    meshName = meshName + "-partitioned";
  }
  
  XdmfDOM dom;
  XdmfInt32 error = dom.Parse(argv[1]);

  std::string fileName = argv[1];
  size_t fileNameFound = fileName.find_last_of("/\\");
  if (fileNameFound != std::string::npos)
  {
    dom.SetWorkingDirectory(fileName.substr(0, fileNameFound).substr().c_str());
  }

  if(error == XDMF_FAIL)
  {
    std::cout << "File does not appear to be a valid Xdmf file" << std::endl;
    return 1;
  }
  XdmfXmlNode gridElement = dom.FindElementByPath("/Xdmf/Domain/Grid");
  if(gridElement == NULL)
  {
    std::cout << "Cannot parse Xdmf file!" << std::endl;
    return 1;
  }

  XdmfGrid * grid = new XdmfGrid();
  grid->SetDOM(&dom);
  grid->SetElement(gridElement);
  grid->Update();

  XdmfDOM newDOM;
  XdmfRoot newRoot;
  XdmfDomain newDomain;

  newRoot.SetDOM(&newDOM);
  newRoot.Build();
  newRoot.Insert(&newDomain);

  XdmfPartitioner partitioner;
  XdmfGrid * partitionedGrid = partitioner.Partition(grid, numPartitions, &newDomain);
  delete grid;

  for(int i=0; i<partitionedGrid->GetNumberOfChildren(); ++i)
  {
    XdmfGrid * child = partitionedGrid->GetChild(i);
    
    // Set heavy data set names for geometry and topology
    std::stringstream heavyPointName;
    heavyPointName << meshName << ".h5:/" << child->GetName() << "/XYZ";
    child->GetGeometry()->GetPoints()->SetHeavyDataSetName(heavyPointName.str().c_str());

    std::stringstream heavyConnName;
    heavyConnName << meshName << ".h5:/" << child->GetName() << "/Connections";
    child->GetTopology()->GetConnectivity()->SetHeavyDataSetName(heavyConnName.str().c_str());

    // Set heavy data set names for mesh attributes and sets
    for(int i=0; i<child->GetNumberOfAttributes(); i++)
    {
      std::stringstream heavyAttrName;
      heavyAttrName << meshName << ".h5:/" << child->GetName() << "/Attribute/" << child->GetAttribute(i)->GetAttributeCenterAsString() << "/" << child->GetAttribute(i)->GetName();
      child->GetAttribute(i)->GetValues()->SetHeavyDataSetName(heavyAttrName.str().c_str());
    }

    for(int i=0; i<child->GetNumberOfSets(); i++)
    {
      std::stringstream heavySetName;
      heavySetName << meshName << ".h5:/" << child->GetName() << "/Set/" << child->GetSets(i)->GetSetTypeAsString() << "/" << child->GetSets(i)->GetName();
      child->GetSets(i)->GetIds()->SetHeavyDataSetName(heavySetName.str().c_str());
    }
  }

  partitionedGrid->Build();
 
  std::stringstream outputFileName;
  outputFileName << meshName << ".xmf";
  
  newDOM.Write(outputFileName.str().c_str());
 
  delete partitionedGrid; 
  std::cout << "Wrote: " << outputFileName.str().c_str() << std::endl;
}*/

#endif //BUILD_EXE
