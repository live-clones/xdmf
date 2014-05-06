/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfExodusWriter.cpp                                                */
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

#include <cstring>
#include <exodusII.h>
#include <set>
#include <sstream>
#include "XdmfArrayType.hpp"
#include "XdmfAttribute.hpp"
#include "XdmfAttributeCenter.hpp"
#include "XdmfExodusWriter.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfGeometryType.hpp"
#include "XdmfGridCollection.hpp"
#include "XdmfGridCollectionType.hpp"
#include "XdmfPartitioner.hpp"
#include "XdmfSet.hpp"
#include "XdmfSetType.hpp"
#include "XdmfTopology.hpp"
#include "XdmfTopologyType.hpp"
#include "XdmfUnstructuredGrid.hpp"
#include "XdmfError.hpp"

#include <iostream>

//
// local methods
//
namespace {

  /**
   * Constructs attribute names for ExodusII files since exodus cannot
   * store vectors. Also handles the exodus MAX_STR_LENGTH limitation.
   *
   * @param attributeName the attribute name in Xdmf.
   * @param names a vector of names to add the constructed attribute names to.
   * @param numComponents the number of components in the attribute
   * (e.g. for an xyz vector this is 3)
   */
  void
  constructAttributeNames(std::string attributeName,
                          std::vector<std::string> & names,
                          const int numComponents)
  {
    if(numComponents == 1) {
      if(attributeName.length() > MAX_STR_LENGTH) {
        attributeName = attributeName.substr(0, MAX_STR_LENGTH);
      }
      names.push_back(attributeName);
    }
    else if(numComponents > 1) {
      int numComponentDigits = int(numComponents / 10);
      if(attributeName.length() + numComponentDigits > MAX_STR_LENGTH) {
        attributeName =
          attributeName.substr(0,
                               MAX_STR_LENGTH - numComponentDigits);
      }
      for(int i=0; i<numComponents; ++i) {
        std::stringstream toAdd;
        toAdd << attributeName << "-" << i+1;
        names.push_back(toAdd.str());
      }
    }
  }

  /**
   * Converts xdmf topology types to equivalent exodus topology types.
   *
   * @param topologyType an xdmf topology type to convert to the equivalent
   * exodus topology type.
   *
   * @return string containing the name of the equivalent exodus topology type.
   */
  std::string
  xdmfToExodusTopologyType(shared_ptr<const XdmfTopologyType> topologyType)
  {
    if(topologyType == XdmfTopologyType::Polyvertex()) {
      return "SUP";
    }
    else if(topologyType == XdmfTopologyType::Triangle() ||
            topologyType == XdmfTopologyType::Triangle_6()) {
      return "TRIANGLE";
    }
    else if(topologyType == XdmfTopologyType::Quadrilateral() ||
            topologyType == XdmfTopologyType::Quadrilateral_8() ||
            topologyType == XdmfTopologyType::Quadrilateral_9()) {
      return "QUAD";
    }
    else if(topologyType == XdmfTopologyType::Tetrahedron() ||
            topologyType == XdmfTopologyType::Tetrahedron_10()) {
      return "TETRA";
    }
    else if(topologyType == XdmfTopologyType::Pyramid() ||
            topologyType == XdmfTopologyType::Pyramid_13()) {
      return "PYRAMID";
    }
    else if(topologyType == XdmfTopologyType::Wedge() ||
            topologyType == XdmfTopologyType::Wedge_15() ||
            topologyType == XdmfTopologyType::Wedge_18()) {
      return "WEDGE";
    }
    else if(topologyType == XdmfTopologyType::Hexahedron() ||
            topologyType == XdmfTopologyType::Hexahedron_20() ||
            topologyType == XdmfTopologyType::Hexahedron_24() ||
            topologyType == XdmfTopologyType::Hexahedron_27()) {
      return "HEX";
    }
    else if(topologyType == XdmfTopologyType::Edge_3()) {
      return "EDGE";
    }
    XdmfError::message(XdmfError::FATAL, 
                       "Topology type not supported by ExodusII");
    return "";
  }

  /**
   * Pull general information from grid in order to initialize exodus
   * file.
   */
  void
  getGridInformation(const shared_ptr<XdmfUnstructuredGrid> grid,
                     int & num_dim,
                     int & num_nodes,
                     int & num_elem,
                     int & num_elem_blk,
                     std::vector<int> & elem_blk_ids,
                     std::vector<char *> & elem_type,
                     std::vector<int> & num_elem_this_blk,
                     std::vector<int> & num_nodes_per_elem,
                     std::vector<int> & num_attr,
                     int & num_node_sets,
                     std::vector<shared_ptr<XdmfSet> > & blocks)
  {

    // get topology
    const shared_ptr<XdmfTopology> topology = grid->getTopology();
    const shared_ptr<const XdmfTopologyType> topologyType = 
      topology->getType();
    const int numberElements = topology->getNumberElements();
    const int nodesPerElement = topologyType->getNodesPerElement();

    // get geometry
    const shared_ptr<XdmfGeometry> geometry = grid->getGeometry();
    const shared_ptr<const XdmfGeometryType> geometryType = 
      geometry->getType();
    const int numberNodes = geometry->getNumberPoints();

    num_dim = geometryType->getDimensions();
    num_nodes = numberNodes;
    num_elem = numberElements;

    // get exodus topology type
    const std::string exodusTopologyType = 
      xdmfToExodusTopologyType(topologyType);
     
    // search for blocks in file
    int elementBlockId = 10;
    for(unsigned int i=0; i<grid->getNumberSets(); ++i) {
      shared_ptr<XdmfSet> set = grid->getSet(i);
      if(set->getType() == XdmfSetType::Cell()) {
        if(set->getName().find("Block") == 0) {
          ++num_elem_blk;
          blocks.push_back(set);
          elem_blk_ids.push_back(elementBlockId++);
          char * cellTypeCStr = new char[exodusTopologyType.size() + 1];
          strcpy(cellTypeCStr, exodusTopologyType.c_str());
          elem_type.push_back(cellTypeCStr);   
          num_elem_this_blk.push_back(set->getSize());
          num_nodes_per_elem.push_back(nodesPerElement);
          num_attr.push_back(0);
        }
      }
      else if(set->getType() == XdmfSetType::Node()) {
        ++num_node_sets;
      }
    }

    // adjust for case where there are no blocks found in the xdmf file
    if(blocks.size() == 0) {
      num_elem_blk = 1;
      elem_blk_ids.push_back(elementBlockId);
      elem_blk_ids.push_back(elementBlockId++);
      char * cellTypeCStr = new char[exodusTopologyType.size() + 1];
      strcpy(cellTypeCStr, exodusTopologyType.c_str());
      elem_type.push_back(cellTypeCStr);   
      num_elem_this_blk.push_back(numberElements);
      num_nodes_per_elem.push_back(nodesPerElement);
      num_attr.push_back(0);
    }
    
  }

  /**
   * Convert between Xdmf and Exodus topology numberings
   */
  void
  convertXdmfToExodusTopology(int * elem_connectivity,
                              const shared_ptr<const XdmfTopologyType> topType,
                              int num_elem)
  {
    if(topType == XdmfTopologyType::Hexahedron_20() ||
       topType == XdmfTopologyType::Hexahedron_27()) {
      int * ptr = elem_connectivity;
      int itmp[4];
      
      for(int i=0; i<num_elem; ++i) {
        ptr += 12;
        
        for (unsigned int j=0; j<4; ++j, ++ptr) {
          itmp[j] = *ptr;
          *ptr = ptr[4];
        }
        
        for(unsigned int j=0; j<4; ++j, ++ptr) {
          *ptr = itmp[j];
        }
        
        if(topType == XdmfTopologyType::Hexahedron_27()) {
          itmp[0] = *ptr;
          *ptr = ptr[6];
          ptr++;
          itmp[1] = *ptr;
          *ptr = ptr[3];
          ptr++;
          itmp[2] = *ptr;
          *ptr = ptr[3];
          ptr++;
          itmp[3] = *ptr;
          for (unsigned int j=0; j<4; ++j, ++ptr) {
            *ptr = itmp[j];
          }
        }
      }
    }
    else if(topType == XdmfTopologyType::Wedge_15() ||
            topType == XdmfTopologyType::Wedge_18()) {
      int * ptr = elem_connectivity;
      int itmp[3];
      
      for(int i=0; i<num_elem; ++i) {
        ptr += 9;
        
        for(unsigned int j=0; j<3; ++j, ++ptr) {
          itmp[j] = *ptr;
          *ptr = ptr[3];
        }
        
        for(unsigned int j=0; j<3; ++j, ++ptr) {
          *ptr = itmp[j];
        }
        
        if(topType == XdmfTopologyType::Wedge_18()) {
          itmp[0] = *(ptr);
          itmp[1] = *(ptr+1);
          itmp[2] = *(ptr+2);
          *(ptr++) = itmp[2];
          *(ptr++) = itmp[0];
          *(ptr++) = itmp[1];
        }
      }
    }
  }

  /**
   * Write sets from grid to exodus file.
   */
  void
  writeSets(int exodusHandle,
            const shared_ptr<XdmfUnstructuredGrid> grid)
  {
    const unsigned int numberSets = grid->getNumberSets();
    const int setId = 20;
    
    std::vector<int> values;

    for(unsigned int i=0; i<numberSets; ++i) {
      const shared_ptr<XdmfSet> set = grid->getSet(i);

      if(set->getType() == XdmfSetType::Node()) {

        bool releaseSet = false;
        if(!set->isInitialized()) {
          set->read();
          releaseSet = true;
        }
        
        std::string name = set->getName();
        if(name.size() > MAX_STR_LENGTH) {
          name = name.substr(0, MAX_STR_LENGTH);
        }
        
        const unsigned int setSize = set->getSize();
        values.resize(setSize);
        for(unsigned int k=0; k<setSize; ++k) {
          // Add 1 to xdmf ids because exodus ids begin at 1
          values[k] = set->getValue<int>(k) + 1;
        }
      
        /*
          if(set->getType() == XdmfSetType::Cell()) {
          ex_put_side_set_param(exodusHandle, setId + i, setSize, 0);
          ex_put_side_set(exodusHandle, setId + i, &(values[0]), NULL);
          ex_put_name(exodusHandle, EX_SIDE_SET, setId + i, name.c_str());
          }
        */

        
        ex_put_node_set_param(exodusHandle, setId + i, setSize, 0);
        ex_put_node_set(exodusHandle, setId + i, &(values[0]));
        ex_put_name(exodusHandle, EX_NODE_SET, setId + i, name.c_str());

        if(releaseSet) {
          set->release();
        }
      }
    }
  }

}

shared_ptr<XdmfExodusWriter>
XdmfExodusWriter::New()
{
  shared_ptr<XdmfExodusWriter> p(new XdmfExodusWriter());
  return p;
}

XdmfExodusWriter::XdmfExodusWriter()
{
}

XdmfExodusWriter::~XdmfExodusWriter()
{
}

void
XdmfExodusWriter::write(const std::string & filePath,
                        const shared_ptr<XdmfUnstructuredGrid> gridToWrite) const
{

  // add this grid to a simple grid collection and write
  shared_ptr<XdmfGridCollection> collection = XdmfGridCollection::New();
  collection->setName(gridToWrite->getName());
  collection->setType(XdmfGridCollectionType::Temporal());
  collection->insert(gridToWrite);
  return write(filePath,
               collection);

}

void
XdmfExodusWriter::write(const std::string & filePath,
                        const shared_ptr<XdmfGridCollection> grid) const
{

  int error;
  ex_opts(EX_VERBOSE);

  // open exodus file
  int wordSize = 8;
  int storeSize = 8;
  int exodusHandle = ex_create(filePath.c_str(),
                               EX_CLOBBER,
                               &wordSize,
                               &storeSize);

  // initialize exodus file
  std::string title = grid->getName();
  if(title.size() > MAX_STR_LENGTH) {
    title = title.substr(0, MAX_STR_LENGTH);
  }

  // determine type of collections present
  bool isSpatial = false;
  bool isTemporal = false;
  shared_ptr<XdmfGridCollection> gridCollection = grid;

  if(grid->getType() == XdmfGridCollectionType::Temporal()) {
    isTemporal = true;
    if(grid->getNumberGridCollections() != 0) {
      gridCollection = grid->getGridCollection(0);
      if(gridCollection->getType() == XdmfGridCollectionType::Spatial()) {
        isSpatial = true;
      }
    }
  }
  else if(grid->getType() == XdmfGridCollectionType::Spatial()) {
    isSpatial = true;
  }

  int num_dim = 0;
  int num_nodes = 0;
  int num_elem = 0;
  int num_elem_blk = 0;
  int num_node_sets = 0;
  int num_side_sets = 0;

  std::vector<int> elem_blk_ids;
  std::vector<char *> elem_type;
  std::vector<int> num_elem_this_blk;
  std::vector<int> num_nodes_per_elem;
  std::vector<int> num_attr;
  std::vector<shared_ptr<XdmfAttribute> > globalNodeIds;
 
  // initialize file

  shared_ptr<XdmfUnstructuredGrid> unstructuredGrid;
    
  if(isSpatial) {
        
    // unpartition grid as exodus does not store partitioning information
    shared_ptr<XdmfPartitioner> partitioner = XdmfPartitioner::New();
    unstructuredGrid = partitioner->unpartition(gridCollection);

  }
  else {

    // get first unstructured grid from temporal collection
    unstructuredGrid = gridCollection->getUnstructuredGrid(0);
    
  }

  std::vector<shared_ptr<XdmfSet> > blocks;
  getGridInformation(unstructuredGrid,
                     num_dim,
                     num_nodes,
                     num_elem,
                     num_elem_blk,
                     elem_blk_ids,
                     elem_type,
                     num_elem_this_blk,
                     num_nodes_per_elem,
                     num_attr,
                     num_node_sets,
                     blocks);

  /*
  std::cout << "num_dim: " << num_dim << std::endl;
  std::cout << "num_nodes: " << num_nodes << std::endl;
  std::cout << "num_elem: " << num_elem << std::endl;
  std::cout << "num_elem_blk: " << num_elem_blk << std::endl;
  std::cout << "num_node_sets: " << num_node_sets << std::endl;
  std::cout << "num_side_sets: " << num_side_sets << std::endl;
  */

  error = ex_put_init(exodusHandle,
                      title.c_str(),
                      num_dim,
                      num_nodes,
                      num_elem,
                      num_elem_blk,
                      num_node_sets,
                      num_side_sets);
  
  if(error != 0) {
    XdmfError::message(XdmfError::FATAL,
                       "Error initializing exodus file.");
  }
 
  const int define_maps  = 0;
  error = ex_put_concat_elem_block(exodusHandle, 
                                   &(elem_blk_ids[0]), 
                                   &(elem_type[0]), 
                                   &(num_elem_this_blk[0]), 
                                   &(num_nodes_per_elem[0]),
                                   &(num_attr[0]), 
                                   define_maps);
  
  if(error != 0) {
    XdmfError::message(XdmfError::FATAL,
                       "Error initializing element blocks in exodus file");
    return;
  }

  for(int i=0; i<num_elem_blk; ++i) {
    delete [] elem_type[i];
  }

  /*
  std::cout << "End Init" << std::endl;
  */

  // write geometry

  double * x = new double[num_nodes];
  double * y = new double[num_nodes];
  double * z = NULL;

  if(num_dim == 3) {
    z = new double[num_nodes];
  }
   
  // get geometry
  shared_ptr<XdmfGeometry> geometry = unstructuredGrid->getGeometry();
  bool releaseGeometry = false;
  if(!geometry->isInitialized()) {
    geometry->read();
    releaseGeometry = true;
  }
  
  // read nodal positions
  geometry->getValues(0, x, num_nodes, 3);
  geometry->getValues(1, y, num_nodes, 3);
  if(num_dim == 3) {
    geometry->getValues(2, z, num_nodes, 3);
  }
    
  // release data
  if(releaseGeometry) {
    geometry->release();
  }

  error = ex_put_coord(exodusHandle, x ,y ,z);

  delete [] x;
  delete [] y;
  delete [] z;

  /*
  std::cout << "End Geometry" << std::endl;
  */

  // write topology
  shared_ptr<XdmfTopology> topology = unstructuredGrid->getTopology();
  shared_ptr<const XdmfTopologyType> topologyType = topology->getType();
  const int nodesPerElement = topologyType->getNodesPerElement();
  bool releaseTopology = false;
  if(!topology->isInitialized()) {
    topology->read();
    releaseTopology = true;
  }
    
  if(blocks.size() > 0) {

    // if blocks, write topology to blocks
    for(unsigned int i=0; i<blocks.size(); ++i) {
      
      shared_ptr<XdmfSet> & set = blocks[i];
      
      bool releaseSet = false;
      if(!set->isInitialized()) {
        set->read();
        releaseSet = true;
      }
      
      const unsigned int connectivitySize = set->getSize() * nodesPerElement;
      int * elem_connectivity = new int[connectivitySize];      
      int offset = 0;
      for(unsigned int j=0; j<set->getSize(); ++j) {
        const int elementId = set->getValue<int>(j);
        const int topologyOffset = elementId * nodesPerElement;
        for(int k=0; k<nodesPerElement; ++k) {
          elem_connectivity[offset] = 
            topology->getValue<int>(topologyOffset + k) + 1;
          ++offset;
        }
      }
      
      if(releaseSet) {
        set->release();
      }
      
      convertXdmfToExodusTopology(elem_connectivity,
                                  topologyType,
                                  num_elem);
      
      ex_put_elem_conn(exodusHandle, 
                       elem_blk_ids[i], 
                       elem_connectivity);
      
      delete [] elem_connectivity;
      
    }
  }
  else {
    
    // write to single block
    const unsigned int connectivitySize = topology->getSize();
    int * elem_connectivity = new int[connectivitySize];
    for(unsigned int i=0; i<connectivitySize; ++i) {
      elem_connectivity[i] = topology->getValue<int>(i) + 1;
    }
    if(releaseTopology) {
      topology->release();
    }
    convertXdmfToExodusTopology(elem_connectivity,
                                topologyType,
                                num_elem);
    ex_put_elem_conn(exodusHandle, 
                     elem_blk_ids[0], 
                     elem_connectivity);

    delete [] elem_connectivity;
  }
    
  if(releaseTopology) {
    topology->release();
  }

  /*
  std::cout << "End Topology" << std::endl;
  */

  // write attributes
  int numGlobalAttributes = 0;
  int numNodalAttributes = 0;
  int numElementAttributes = 0;

  std::vector<unsigned int> globalComponents;
  std::vector<unsigned int> nodalComponents;
  std::vector<unsigned int> elementComponents;
  std::vector<std::string> globalAttributeNames;
  std::vector<std::string> nodalAttributeNames;
  std::vector<std::string> elementAttributeNames;

  const unsigned int numberAttributes = 
    unstructuredGrid->getNumberAttributes();

  for(unsigned int i=0; i<numberAttributes; ++i) {

    const shared_ptr<XdmfAttribute> attribute = 
      unstructuredGrid->getAttribute(i);
    const unsigned int numberElements = 
      unstructuredGrid->getTopology()->getNumberElements();
    const unsigned int numberPoints = 
      unstructuredGrid->getGeometry()->getNumberPoints();

    const shared_ptr<const XdmfAttributeCenter> attributeCenter =
      attribute->getCenter();

    if(attributeCenter == XdmfAttributeCenter::Grid()) {
      const int numComponents = attribute->getSize();
      globalComponents.push_back(numComponents);
      numGlobalAttributes += numComponents;
      constructAttributeNames(attribute->getName(),
                              globalAttributeNames,
                              numComponents);
    }
    else if(attributeCenter == XdmfAttributeCenter::Node()) {
      const int numComponents = attribute->getSize() / numberPoints;
      nodalComponents.push_back(numComponents);
      numNodalAttributes += numComponents;
      constructAttributeNames(attribute->getName(),
                              nodalAttributeNames,
                              numComponents);
    }
    else if(attributeCenter == XdmfAttributeCenter::Cell()) {
      const int numComponents = attribute->getSize() / numberElements;
      elementComponents.push_back(numComponents);
      numElementAttributes += numComponents;
      constructAttributeNames(attribute->getName(),
                              elementAttributeNames,
                              numComponents);
    }

  }

  if(numGlobalAttributes > 0) {
    ex_put_var_param(exodusHandle, "g", numGlobalAttributes);
  }
  if(numNodalAttributes > 0) {
    ex_put_var_param(exodusHandle, "n", numNodalAttributes);
  }
  if(numElementAttributes > 0) {
    ex_put_var_param(exodusHandle, "e", numElementAttributes);
  }

  /*
  std::cout << "Done attribute params" << std::endl;
  */

  char ** globalNames = new char*[numGlobalAttributes];
  char ** nodalNames = new char*[numNodalAttributes];
  char ** elementNames = new char*[numElementAttributes];

  for(int i=0; i<numGlobalAttributes; ++i) {
    globalNames[i] = (char*)globalAttributeNames[i].c_str();
  }

  for(int i=0; i<numNodalAttributes; ++i) {
    nodalNames[i] = (char*)nodalAttributeNames[i].c_str();
  }

  for(int i=0; i<numElementAttributes; ++i) {
    elementNames[i] = (char*)elementAttributeNames[i].c_str();
  }

  if(numGlobalAttributes > 0) {
    ex_put_var_names(exodusHandle, "g", numGlobalAttributes, globalNames);
  }
  if(numNodalAttributes > 0) {
    ex_put_var_names(exodusHandle, "n", numNodalAttributes, nodalNames);
  }
  if(numElementAttributes > 0) {
    ex_put_var_names(exodusHandle, "e", numElementAttributes, elementNames);
  }

  delete [] globalNames;
  delete [] nodalNames;
  delete [] elementNames;

  /*
  std::cout << "Done attribute names" << std::endl;
  */

  if(numElementAttributes > 0) {
    const int numTruthTableValues = num_elem_blk * numElementAttributes;
    int * truthTable = new int[numTruthTableValues];
    for(int i=0; i<numTruthTableValues; ++i) {
      truthTable[i] = 1;
    }
    ex_put_elem_var_tab(exodusHandle, 
                        num_elem_blk, 
                        numElementAttributes, 
                        truthTable);
    delete [] truthTable;
  }

  /*
  std::cout << "Done truth table" << std::endl;
  */

  unsigned int numTemporalGrids = 1;
  if(isTemporal) {
    if(grid->getNumberUnstructuredGrids() == 0) {
      numTemporalGrids = grid->getNumberGridCollections();
    }
    else {
      numTemporalGrids = grid->getNumberUnstructuredGrids();
    }
  }

  // only write for first timestep
  writeSets(exodusHandle,
            unstructuredGrid);

  /*
  std::cout << "Done Sets" << std::endl;
  */

  for(unsigned int i=0; i<numTemporalGrids; ++i) {

    // get appropriate grid
    if(isTemporal) {
      if(isSpatial) {
        // we already unpartitioned the first timestep, avoid doing it again
        if(i != 0) {
          // unpartition spatial partitioning
          std::cout << "unpartition" << std::endl;
          gridCollection = grid->getGridCollection(i);
          shared_ptr<XdmfPartitioner> partitioner = XdmfPartitioner::New();
          unstructuredGrid = partitioner->unpartition(gridCollection);
        }
      }
      else {
        unstructuredGrid = grid->getUnstructuredGrid(i);
      }
    }

    double * globalAttributeVals = new double[numGlobalAttributes];

    unsigned int globalIndex = 0;
    unsigned int globalComponentIndex = 0;
    unsigned int nodalIndex = 0;
    unsigned int nodalComponentIndex = 0;
    unsigned int elementIndex = 0;
    unsigned int elementComponentIndex = 0;
    
    std::vector<double> elementArray;

    for(unsigned int j=0; j<numberAttributes; ++j) {
      
      shared_ptr<XdmfAttribute> attribute = unstructuredGrid->getAttribute(j);
      shared_ptr<const XdmfAttributeCenter> attributeCenter = 
        attribute->getCenter();

      /*
      std::cout << attribute->getName() << std::endl;
      */

      bool releaseAttribute = false;
      if(!attribute->isInitialized()) {
        attribute->read();
        releaseAttribute = true;
      }

      if(attribute->getCenter() == XdmfAttributeCenter::Grid()) {
        for(unsigned int k=0; k<globalComponents[globalComponentIndex]; ++k) {
          attribute->getValues(k, 
                               globalAttributeVals + globalIndex, 
                               1);
          globalIndex++;
        }
        globalComponentIndex++;
      }
      else if(attribute->getCenter() == XdmfAttributeCenter::Node()) {
        for(unsigned int k=0; k<nodalComponents[nodalComponentIndex]; ++k) {
          double * nodalValues = new double[num_nodes];
          attribute->getValues(k,
                               nodalValues,
                               num_nodes,
                               nodalComponents[nodalComponentIndex]);
          ex_put_nodal_var(exodusHandle,
                           i+1,
                           nodalIndex+1,
                           num_nodes,
                           nodalValues);
          ex_update(exodusHandle);
          delete [] nodalValues;
          nodalIndex++;
        }
        nodalComponentIndex++;
      }
      else if(attribute->getCenter() == XdmfAttributeCenter::Cell()) {
        if(blocks.size() > 0) {
          for(unsigned int k=0; k<blocks.size(); ++k) {
            
            const shared_ptr<XdmfSet> & set = blocks[k];
            const int num_elem_in_blk = set->getSize();
            
            bool releaseSet = false;
            if(!set->isInitialized()) {
              set->read();
              releaseSet = true;
            }
            
            double * elementValues = new double[num_elem_in_blk];
            
            // loop over components
            const int numberComponents = 
              elementComponents[elementComponentIndex];
            
            for(int l=0; l<numberComponents; ++l) {
              for(int m=0; m<num_elem_in_blk; ++m) {
                const int elementId = set->getValue<int>(m);
                elementValues[m] = 
                  attribute->getValue<double>(numberComponents*elementId + l);
              }
              ex_put_elem_var(exodusHandle,
                              i+1,
                              elementIndex+l+1,
                              elem_blk_ids[k],
                              num_elem_in_blk,
                              elementValues);
              ex_update(exodusHandle);
            }        
            
            delete [] elementValues;
            
            if(releaseSet) {
              set->release();
            }
          }
          elementIndex += elementComponents[elementComponentIndex];
          elementComponentIndex++;
        }
        else {
          for(unsigned int k=0; 
              k<elementComponents[elementComponentIndex]; ++k) {
            double * elementValues = new double[num_elem];
            attribute->getValues(k,
                                 elementValues,
                                 num_elem,
                                 elementComponents[elementComponentIndex]);
            ex_put_elem_var(exodusHandle,
                            i+1,
                            elementIndex+1,
                            elem_blk_ids[0], 
                            num_elem,
                            elementValues);
            ex_update(exodusHandle);
            delete [] elementValues;
            elementIndex++;
          }
          elementComponentIndex++;
        }
      }
      if(releaseAttribute) {
        attribute->release();
      }
    }
    if(numGlobalAttributes > 0) {
      ex_put_glob_vars(exodusHandle,
                       i+1,
                       numGlobalAttributes,
                       globalAttributeVals);
    }
    ex_update(exodusHandle);
    delete [] globalAttributeVals;

  }

  /*
  std::cout << "Done Attributes" << std::endl;
  */

  // close exodus file
  ex_close(exodusHandle);

}
