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
   *
   * This should be called once per element block (spatial partition) in
   * the xdmf file.
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
                     std::map<std::string, std::set<int> > & nodeSets,
                     std::map<std::string, std::set<int> > & sideSets)
  {

    // get topology
    const shared_ptr<XdmfTopology> topology = grid->getTopology();
    const int numberElements = topology->getNumberElements();

    num_dim = grid->getGeometry()->getType()->getDimensions();
    num_elem += numberElements;
    num_elem_blk++;
    elem_blk_ids.push_back(10 + num_elem_blk);

    // get exodus topology type
    const std::string exodusTopologyType =
      xdmfToExodusTopologyType(topology->getType());
    char * cellTypeCStr = new char[exodusTopologyType.size() + 1];
    strcpy(cellTypeCStr, exodusTopologyType.c_str());
    elem_type.push_back(cellTypeCStr);
    
    num_elem_this_blk.push_back(numberElements);
    num_nodes_per_elem.push_back(topology->getType()->getNodesPerElement()); 
    num_attr.push_back(0);
      
    for(unsigned int i=0; i<grid->getNumberSets(); ++i) {
      shared_ptr<XdmfSet> set = grid->getSet(i);
      if(set->getType() == XdmfSetType::Cell()) {
        sideSets[set->getName()] = std::set<int>();
      }
      else if(set->getType() == XdmfSetType::Node()) {
        nodeSets[set->getName()] = std::set<int>();
      }
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
      
      if(set->getType() == XdmfSetType::Cell()) {
        ex_put_side_set_param(exodusHandle, setId + i, setSize, 0);
        ex_put_side_set(exodusHandle, setId + i, &(values[0]), NULL);
        ex_put_name(exodusHandle, EX_SIDE_SET, setId + i, name.c_str());
      }
      else if(set->getType() == XdmfSetType::Node()) {
        ex_put_node_set_param(exodusHandle, setId + i, setSize, 0);
        ex_put_node_set(exodusHandle, setId + i, &(values[0]));
        ex_put_name(exodusHandle, EX_NODE_SET, setId + i, name.c_str());
      }

      if(releaseSet) {
        set->release();
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

  // open exodus file
  int wordSize = 8;
  int storeSize = 8;
  int exodusHandle = ex_create(filePath.c_str(),
                               EX_CLOBBER,
                               &wordSize,
                               &storeSize);

  // initialize exodus file
  std::string title = gridToWrite->getName();
  if(title.size() > MAX_STR_LENGTH) {
    title = title.substr(0, MAX_STR_LENGTH);
  }

  shared_ptr<XdmfUnstructuredGrid> currGrid = gridToWrite;

  // make sure geometry and topology are non null
  if(!(currGrid->getGeometry() && currGrid->getTopology()))
    XdmfError::message(XdmfError::FATAL, 
                       "Current grid's geometry or topology is null in "
                       "XdmfExodusWriter::write");

  int num_dim = currGrid->getGeometry()->getType()->getDimensions();
  int num_nodes = currGrid->getGeometry()->getNumberPoints();
  int num_elem = currGrid->getTopology()->getNumberElements();
  int num_elem_blk = 1;
  int num_node_sets = 0;
  int num_side_sets = 0;

  std::vector<shared_ptr<XdmfSet> > blockSets;

  for (unsigned int i=0; i<currGrid->getNumberSets(); ++i) {
    if(currGrid->getSet(i)->getType() == XdmfSetType::Cell()) {
      num_side_sets++;
      // check if we have block information written as a set
      if(currGrid->getSet(i)->getName().find("Block") == 0) {
        blockSets.push_back(currGrid->getSet(i));
      }
    }
    else if(currGrid->getSet(i)->getType() == XdmfSetType::Node()) {
      num_node_sets++;
    }
  }

  //
  // we have a single block
  //
  if(blockSets.size() > 0) {
    num_elem_blk = blockSets.size();
  }

  ex_put_init(exodusHandle,
              title.c_str(),
              num_dim,
              num_nodes,
              num_elem,
              num_elem_blk,
              num_node_sets,
              num_side_sets);

  std::cout << "End Init" << std::endl;

  shared_ptr<XdmfGeometry> geometry = currGrid->getGeometry(); 
  bool releaseGeometry = false;
  if(!geometry->isInitialized()) {
    geometry->read();
    releaseGeometry = true;
  }
  double * x = new double[num_nodes];
  double * y = new double[num_nodes];
  double * z = new double[num_nodes];
  // Write nodal coordinate values to exodus
  geometry->getValues(0, x, num_nodes, 3);
  geometry->getValues(1, y, num_nodes, 3);
  if(geometry->getType() == XdmfGeometryType::XYZ()) {
    geometry->getValues(2, z, num_nodes, 3);
  }
  if(releaseGeometry) {
    geometry->release();
  }
  ex_put_coord(exodusHandle, x ,y ,z);
  delete [] x;
  delete [] y;
  delete [] z;

  std::cout << "End Geometry" << std::endl;

  shared_ptr<XdmfTopology> topology = currGrid->getTopology();

  // write element block parameters
  std::string exodusTopologyType =
    xdmfToExodusTopologyType(topology->getType());

  const int nodesPerElement = topology->getType()->getNodesPerElement();

  if(blockSets.size() > 0) {
    
    std::vector<int> elem_blk_ids;
    std::vector<int> num_elem_this_blk;
    std::vector<char*> elem_type;
    std::vector<int>  num_nodes_per_elem(blockSets.size(), nodesPerElement);
    std::vector<int> num_attr(blockSets.size(), 0);
    
    for(unsigned int i=0; i<blockSets.size(); ++i) {
      elem_blk_ids.push_back(10 + i);
      num_elem_this_blk.push_back(blockSets[i]->getSize());
      
      char * cellTypeCStr = new char[exodusTopologyType.size() + 1];
      strcpy(cellTypeCStr, exodusTopologyType.c_str());
      elem_type.push_back(cellTypeCStr);
    }
    
    const int define_maps  = 0;
    ex_put_concat_elem_block(exodusHandle,
                             &(elem_blk_ids[0]),
                             &(elem_type[0]),
                             &(num_elem_this_blk[0]),
                             &(num_nodes_per_elem[0]),
                             &(num_attr[0]),
                             define_maps);

    // write element connectivity
    bool releaseTopology = false;
    if(!topology->isInitialized()) {
      topology->read();
      releaseTopology = true;
    }
    
    for(unsigned int i=0; i<blockSets.size(); ++i) {
      
      shared_ptr<XdmfSet> & set = blockSets[i];

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
                                  topology->getType(),
                                  num_elem);
      ex_put_elem_conn(exodusHandle, 10 + i, elem_connectivity);
      delete [] elem_connectivity;
      
    }
    
    if(releaseTopology) {
      topology->release();
    }

    for(unsigned int i=0; i<blockSets.size(); ++i) {
      delete [] elem_type[i];
    }
    
  }
  else {
    ex_put_elem_block(exodusHandle,
                      10,
                      exodusTopologyType.c_str(),
                      num_elem,
                      nodesPerElement,
                      num_side_sets);

    // write element connectivity
    bool releaseTopology = false;
    if(!topology->isInitialized()) {
      topology->read();
      releaseTopology = true;
    }

    const unsigned int connectivitySize = topology->getSize();
    int * elem_connectivity = new int[connectivitySize];
    for(unsigned int i=0; i<connectivitySize; ++i) {
      elem_connectivity[i] = topology->getValue<int>(i) + 1;
    }
    if(releaseTopology) {
      topology->release();
    }
    convertXdmfToExodusTopology(elem_connectivity,
                                topology->getType(),
                                num_elem);
    ex_put_elem_conn(exodusHandle, 10, elem_connectivity);
    delete [] elem_connectivity;
  }

  std::cout << "End Topology" << std::endl;
  
  // write attributes
  int numGlobalAttributes = 0;
  int numNodalAttributes = 0;
  int numElementAttributes = 0;

  std::vector<int> globalComponents;
  std::vector<int> nodalComponents;
  std::vector<int> elementComponents;
  std::vector<std::string> globalAttributeNames;
  std::vector<std::string> nodalAttributeNames;
  std::vector<std::string> elementAttributeNames;

  for(unsigned int i=0; i<currGrid->getNumberAttributes(); ++i) {
    shared_ptr<XdmfAttribute> currAttribute = currGrid->getAttribute(i);
    if(currAttribute->getCenter() == XdmfAttributeCenter::Grid()) {
      int numComponents = currAttribute->getSize();
      globalComponents.push_back(numComponents);
      numGlobalAttributes += numComponents;
      constructAttributeNames(currAttribute->getName(),
                              globalAttributeNames,
                              numComponents);
    }
    else if(currAttribute->getCenter() == XdmfAttributeCenter::Node()) {
      int numComponents = currAttribute->getSize() / num_nodes;
      nodalComponents.push_back(numComponents);
      numNodalAttributes += numComponents;
      constructAttributeNames(currAttribute->getName(),
                              nodalAttributeNames,
                              numComponents);
    }
    else if(currAttribute->getCenter() == XdmfAttributeCenter::Cell()) {
      int numComponents = currAttribute->getSize() / num_elem;
      elementComponents.push_back(numComponents);
      numElementAttributes += numComponents;
      constructAttributeNames(currAttribute->getName(),
                              elementAttributeNames,
                              numComponents);
    }
  }

  ex_put_var_param(exodusHandle, "g", numGlobalAttributes);
  ex_put_var_param(exodusHandle, "n", numNodalAttributes);
  ex_put_var_param(exodusHandle, "e", numElementAttributes);

  std::cout << "End Attribute Params" << std::endl;

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

  ex_put_var_names(exodusHandle, "g", numGlobalAttributes, globalNames);
  ex_put_var_names(exodusHandle, "n", numNodalAttributes, nodalNames);
  ex_put_var_names(exodusHandle, "e", numElementAttributes, elementNames);

  delete [] globalNames;
  delete [] nodalNames;
  delete [] elementNames;

  std::cout << "End Attribute Names" << std::endl;

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

  std::cout << "Done truth table" << std::endl;

  writeSets(exodusHandle,
            currGrid);

  std::cout << "Done sets" << std::endl;

  double * globalAttributeVals = new double[numGlobalAttributes];

  int globalIndex = 0;
  int globalComponentIndex = 0;
  int nodalIndex = 0;
  int nodalComponentIndex = 0;
  int elementIndex = 0;
  int elementComponentIndex = 0;

  for(unsigned int j=0; j<currGrid->getNumberAttributes(); ++j) {
    shared_ptr<XdmfAttribute> currAttribute = currGrid->getAttribute(j);
    std::cout << currAttribute->getName() << std::endl;
    bool releaseAttribute = false;
    if(!currAttribute->isInitialized()) {
      currAttribute->read();
      releaseAttribute = true;
    }
    if(currAttribute->getCenter() == XdmfAttributeCenter::Grid()) {
      for(int k=0; k<globalComponents[globalComponentIndex]; ++k) {
        currAttribute->getValues(k, globalAttributeVals + globalIndex, 1);
        globalIndex++;
      }
      globalComponentIndex++;
    }
    else if(currAttribute->getCenter() == XdmfAttributeCenter::Node()) {
      for(int k=0; k<nodalComponents[nodalComponentIndex]; ++k) {
        double * nodalValues = new double[num_nodes];
        currAttribute->getValues(k,
                                 nodalValues,
                                 num_nodes,
                                 nodalComponents[nodalComponentIndex]);
        ex_put_nodal_var(exodusHandle,
                         1,
                         nodalIndex+1,
                         num_nodes,
                         nodalValues);
        ex_update(exodusHandle);
        delete [] nodalValues;
        nodalIndex++;
      }
      nodalComponentIndex++;
    }
    else if(currAttribute->getCenter() == XdmfAttributeCenter::Cell()) {
      if(blockSets.size() > 0) {
        for(unsigned int k=0; k<blockSets.size(); ++k) {
          
          const shared_ptr<XdmfSet> & set = blockSets[k];
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
          std::cout << numberComponents << std::endl;

          for(int l=0; l<numberComponents; ++l) {
            for(int m=0; m<num_elem_in_blk; ++m) {
              const int elementId = set->getValue<int>(m);
              elementValues[m] = 
                currAttribute->getValue<double>(numberComponents * elementId + l);
            }
            ex_put_elem_var(exodusHandle,
                            1,
                            elementIndex+l+1,
                            10+k,
                            num_elem,
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
        for(int k=0; k<elementComponents[elementComponentIndex]; ++k) {
          double * elementValues = new double[num_elem];
          currAttribute->getValues(k,
                                   elementValues,
                                   num_elem,
                                   elementComponents[elementComponentIndex]);
          ex_put_elem_var(exodusHandle,
                          1,
                          elementIndex+1,
                          10, 
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
      currAttribute->release();
    }
  }
  ex_put_glob_vars(exodusHandle,
                   1,
                   numGlobalAttributes,
                   globalAttributeVals);
  ex_update(exodusHandle);
  delete [] globalAttributeVals;

  std::cout << "Done attribute" << std::endl;
 
  // close exodus file
  ex_close(exodusHandle);
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

  std::vector<int> elem_blk_ids;
  std::vector<char *> elem_type;
  std::vector<int> num_elem_this_blk;
  std::vector<int> num_nodes_per_elem;
  std::vector<int> num_attr;
  std::vector<shared_ptr<XdmfAttribute> > globalNodeIds;

  std::map<std::string, std::set<int> > nodeSets;
  std::map<std::string, std::set<int> > sideSets;
 
  // initialize file

  if(isSpatial) {
    
    // need to figure out number of points and elements
    for(unsigned int i=0; i<gridCollection->getNumberUnstructuredGrids(); 
        ++i) {

      const shared_ptr<XdmfUnstructuredGrid> unstructuredGrid =
        gridCollection->getUnstructuredGrid(i);
      
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
                         nodeSets,
                         sideSets);

      shared_ptr<XdmfAttribute> globalNodeId =
        unstructuredGrid->getAttribute("GlobalNodeId");
      globalNodeIds.push_back(globalNodeId);
     
      if(!globalNodeId) {
        XdmfError::message(XdmfError::FATAL,
                           "Must have GlobalNodeId attribute to convert "
                           "spatially partitioned Xdmf file to Exodus.");
      }
 
      bool releaseGlobalNodeId = false;
      if(!globalNodeId->isInitialized()) {
        globalNodeId->read();
        releaseGlobalNodeId = true;
      }
      
      for(unsigned int j=0; j<globalNodeId->getSize(); ++j) {
        const int index = globalNodeId->getValue<int>(j);
        num_nodes = std::max(index + 1, num_nodes);
      }
      
      if(releaseGlobalNodeId) {
        globalNodeId->release();
      }
      
    }
    
  }
  else {
    
    const shared_ptr<XdmfUnstructuredGrid> unstructuredGrid =
      gridCollection->getUnstructuredGrid(0);

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
                       nodeSets,
                       sideSets);

    num_nodes = unstructuredGrid->getGeometry()->getNumberPoints();
    
  }
 
  const int num_node_sets = nodeSets.size();
  const int num_side_sets = sideSets.size();
 
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

  std::cout << "End Init" << std::endl;

  // write geometry

  double * x = new double[num_nodes];
  double * y = new double[num_nodes];
  double * z = NULL;

  if(num_dim == 3) {
    z = new double[num_nodes];
  }

  if(isSpatial) {
    for(int i=0; i<num_elem_blk; ++i) {

      shared_ptr<XdmfUnstructuredGrid> unstructuredGrid = 
        gridCollection->getUnstructuredGrid(i);

      // get geometry
      shared_ptr<XdmfGeometry> geometry = unstructuredGrid->getGeometry();
      bool releaseGeometry = false;
      if(!geometry->isInitialized()) {
        geometry->read();
        releaseGeometry = true;
      }

      // get global node id
      shared_ptr<XdmfAttribute> globalNodeId = globalNodeIds[i];
      bool releaseGlobalNodeId = false;
      if(!globalNodeId->isInitialized()) {
        globalNodeId->read();
        releaseGlobalNodeId = true;
      }

      // read nodal positions
      const unsigned int numberPoints = geometry->getNumberPoints();
      unsigned int arrayOffset = 0;
      for(unsigned int j=0; j<numberPoints; ++j) {
        const unsigned int currId = globalNodeId->getValue<unsigned int>(j);
        x[currId] = geometry->getValue<double>(arrayOffset++);
        y[currId] = geometry->getValue<double>(arrayOffset++);
        if(num_dim == 3) {
          z[currId] = geometry->getValue<double>(arrayOffset++);
        }
      }
    
      // release data
      if(releaseGlobalNodeId) {
        globalNodeId->release();
      }
      if(releaseGeometry) {
        geometry->release();
      }
    }
  }
  else
  {
    shared_ptr<XdmfUnstructuredGrid> unstructuredGrid = 
      gridCollection->getUnstructuredGrid(0);
    
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
  }

  error = ex_put_coord(exodusHandle, x ,y ,z);

  delete [] x;
  delete [] y;
  delete [] z;

  std::cout << "End Geometry" << std::endl;

  // write topology

  for(int iNumBlocks=0; iNumBlocks < num_elem_blk; ++iNumBlocks) {

    shared_ptr<XdmfTopology> topology = 
      gridCollection->getUnstructuredGrid(iNumBlocks)->getTopology();

    bool releaseTopology = false;
    if(!topology->isInitialized()) {
      topology->read();
      releaseTopology = true;
    }

    // write element connectivity
    const unsigned int connectivitySize = topology->getSize();;
    int * elem_connectivity = new int[connectivitySize];

    if(isSpatial) {
      shared_ptr<XdmfAttribute> globalNodeId = globalNodeIds[iNumBlocks];

      bool releaseGlobalNodeId = false;
      if(!globalNodeId->isInitialized()) {
        globalNodeId->read();
        releaseGlobalNodeId = true;
      }

      for(unsigned int i=0; i<connectivitySize; ++i) {
        const int pointId = topology->getValue<int>(i);
        const int globalPointId = globalNodeId->getValue<int>(pointId) + 1;
        elem_connectivity[i] = globalPointId;
      }
      
      if(releaseGlobalNodeId) {
        globalNodeId->release();
      }
    }
    else {
      for(unsigned int i=0; i<connectivitySize; ++i) {
        elem_connectivity[i] = topology->getValue<int>(i) + 1;
      }
    }

    if(releaseTopology) {
      topology->release();
    }

    convertXdmfToExodusTopology(elem_connectivity,
                                topology->getType(),
                                topology->getNumberElements());
        
    error = ex_put_elem_conn(exodusHandle, 
                             elem_blk_ids[iNumBlocks], 
                             elem_connectivity);

    delete [] elem_connectivity;

  }

  std::cout << "End Topology" << std::endl;

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
    gridCollection->getUnstructuredGrid(0)->getNumberAttributes();

  for(unsigned int i=0; i<numberAttributes; ++i) {

    const shared_ptr<XdmfUnstructuredGrid> unstructuredGrid = 
      gridCollection->getUnstructuredGrid(0);
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

  std::cout << "Done attribute params" << std::endl;

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

  std::cout << "Done attribute names" << std::endl;

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

  std::cout << "Done truth table" << std::endl;

  unsigned int numTemporalGrids = 1;
  if(isTemporal) {
    if(grid->getNumberUnstructuredGrids() == 0) {
      numTemporalGrids = grid->getNumberGridCollections();
    }
    else {
      numTemporalGrids = grid->getNumberUnstructuredGrids();
    }
  }

  // Write Sets
  int setId = 20;

  // Set name to set of ids.
  if(isSpatial) {
    unsigned int elementOffset = 0;
    for(int iNumBlocks=0; iNumBlocks<num_elem_blk; ++iNumBlocks) {

      shared_ptr<XdmfUnstructuredGrid> currGrid;
      if(isTemporal) {
        currGrid = grid->getGridCollection(0)->getUnstructuredGrid(iNumBlocks);
      }
      else {
        currGrid = grid->getUnstructuredGrid(iNumBlocks);
      }

      shared_ptr<XdmfAttribute> globalNodeId = globalNodeIds[iNumBlocks];
      bool releaseGlobalNodeId = false;
      if(currGrid->getNumberSets() > 0 && !globalNodeId->isInitialized()) {
        globalNodeId->read();
        releaseGlobalNodeId = true;
      }
      
      for(unsigned int i=0; i<currGrid->getNumberSets(); ++i) {
        const shared_ptr<XdmfSet> set = currGrid->getSet(i);
        bool releaseSet = false;
        if(!set->isInitialized()) {
          set->read();
          releaseSet = true;
        }
        std::set<int> * setToAddTo = NULL;
	if(set->getType() == XdmfSetType::Cell()) {
	  setToAddTo = &sideSets[set->getName()];
          for(unsigned int j=0; j<set->getSize(); ++j) {
            const int id = set->getValue<int>(j);
            setToAddTo->insert(elementOffset + id + 1);
          }
	}
	else if(set->getType() == XdmfSetType::Node()) {
          setToAddTo = &nodeSets[set->getName()];
          for(unsigned int j=0; j<set->getSize(); ++j) {
            const int id = set->getValue<int>(j);
            const int globalId = globalNodeId->getValue<int>(id);
            setToAddTo->insert(globalId + 1);
          }
	}
        if(releaseSet) {
          set->release();
        }
      }
      if(releaseGlobalNodeId) {
        globalNodeId->release();
      }
      elementOffset += num_elem_this_blk[iNumBlocks];
    }

    for(std::map<std::string, std::set<int> >::const_iterator iter = 
          sideSets.begin(); iter != sideSets.end(); ++iter) {
      const std::string & currSetName = iter->first;
      const std::set<int> & currSet = iter->second;
      std::vector<int> setValues(currSet.begin(),
                                 currSet.end());
      std::string name = currSetName;
      if(name.length() > MAX_STR_LENGTH) {
        name = name.substr(0, MAX_STR_LENGTH);
      }
      ex_put_side_set_param(exodusHandle, setId + 1, setValues.size(), 0);
      ex_put_side_set(exodusHandle, setId + 1, &setValues[0], NULL);
      ex_put_name(exodusHandle, EX_SIDE_SET, setId + 1, name.c_str());
      ++setId;
    }

    for(std::map<std::string, std::set<int> >::const_iterator iter = 
          nodeSets.begin(); iter != nodeSets.end(); ++iter) {
      const std::string & currSetName = iter->first;
      const std::set<int> & currSet = iter->second;
      std::vector<int> setValues(currSet.begin(),
                                 currSet.end());
      std::string name = currSetName;
      if(name.length() > MAX_STR_LENGTH) {
        name = name.substr(0, MAX_STR_LENGTH);
      }
      ex_put_node_set_param(exodusHandle, setId + 1, setValues.size(), 0);
      ex_put_node_set(exodusHandle, setId + 1, &setValues[0]);
      ex_put_name(exodusHandle, EX_NODE_SET, setId + 1, name.c_str());
      ++setId;
    }

  }
  else {
    // only write for first timestep
    writeSets(exodusHandle,
              grid->getUnstructuredGrid(0));
  }
 
  std::cout << "Done Sets" << std::endl;

  for(unsigned int i=0; i<numTemporalGrids; ++i) {

    std::cout << i << std::endl;

    double * globalAttributeVals = new double[numGlobalAttributes];

    unsigned int globalIndex = 0;
    unsigned int globalComponentIndex = 0;
    unsigned int nodalIndex = 0;
    unsigned int nodalComponentIndex = 0;
    unsigned int elementIndex = 0;
    unsigned int elementComponentIndex = 0;
    
    std::vector<double> elementArray;

    for(unsigned int j=0; j<numberAttributes; ++j) {
      
      clock_t start = clock();
      std::string attributeName;

      std::vector<std::vector<double> > nodalArrays;
      shared_ptr<const XdmfAttributeCenter> attributeCenter;

      for(int iNumBlocks=0; iNumBlocks<num_elem_blk; ++iNumBlocks) {

        shared_ptr<XdmfAttribute> attribute;

        if(isTemporal) {
          if(isSpatial) {
            attribute = 
              grid->getGridCollection(i)->getUnstructuredGrid(iNumBlocks)->getAttribute(j);
          }
          else {
            attribute = grid->getUnstructuredGrid(i)->getAttribute(j);
          }
        }
        else {
          attribute = grid->getUnstructuredGrid(iNumBlocks)->getAttribute(j);
        }
        
        bool releaseAttribute = false;
        if(!attribute->isInitialized()) {
          attribute->read();
          releaseAttribute = true;
        }

        attributeName = attribute->getName();

        attributeCenter = attribute->getCenter();
      
        if(attributeCenter == XdmfAttributeCenter::Grid()) {
          for(unsigned int k=0; 
              k<globalComponents[globalComponentIndex]; ++k) {
            globalAttributeVals[globalIndex] = attribute->getValue<double>(k);
            globalIndex++;
          }
          globalComponentIndex++;
        }
        else if(attributeCenter == XdmfAttributeCenter::Node()) {

          const unsigned int numComponents =
            nodalComponents[nodalComponentIndex];

          // resize storage if this is the first block
          if(iNumBlocks == 0) {
            nodalArrays.resize(numComponents);
            for(unsigned int k=0; k<numComponents; ++k) {
              nodalArrays[k].resize(num_nodes);
            }
          }
          
          const unsigned int numberValues = 
            attribute->getSize() / numComponents;

          if(isSpatial) {
            shared_ptr<XdmfAttribute> globalNodeId = globalNodeIds[iNumBlocks];
            bool releaseGlobalNodeId = true;
            if(!globalNodeId->isInitialized()) {
              globalNodeId->read();
              releaseGlobalNodeId = true;
            }
            unsigned int index = 0;
            for(unsigned int l=0; l<numberValues; ++l) {
              const int globalId = globalNodeId->getValue<int>(l);
              for(unsigned int k=0; k<numComponents; ++k) {
                nodalArrays[k][globalId] = 
                  attribute->getValue<double>(index++);
              }
            }
            if(releaseGlobalNodeId) {
              globalNodeId->release();
            }
          }
          else {
            for(unsigned int k=0; k<numComponents; ++k) {
              attribute->getValues(k,
                                   &(nodalArrays[k][0]),
                                   numberValues,
                                   numComponents);
            }
          }
        }
        else if(attributeCenter == XdmfAttributeCenter::Cell()) {
          const unsigned int numElemInBlock = num_elem_this_blk[iNumBlocks];
          elementArray.resize(numElemInBlock);
          const unsigned int numComponents = 
            elementComponents[elementComponentIndex];
          for(unsigned int k=0; k<numComponents; ++k){
            attribute->getValues(k, 
                                 &(elementArray[0]), 
                                 numElemInBlock, 
                                 numComponents);
            ex_put_elem_var(exodusHandle, 
                            i+1, 
                            elementIndex+k+1, 
                            elem_blk_ids[iNumBlocks], 
                            numElemInBlock, 
                            &elementArray[0]);
          }
        }
        
        if(releaseAttribute) {
          attribute->release();
        }
      }
      if(attributeCenter == XdmfAttributeCenter::Cell()) {
        elementIndex+=elementComponents[elementComponentIndex];
        elementComponentIndex++;
      }
      else if(attributeCenter == XdmfAttributeCenter::Node()) {
        for(unsigned int k=0; k<nodalComponents[nodalComponentIndex]; ++k) {
          ex_put_nodal_var(exodusHandle, 
                           i+1, 
                           nodalIndex+1, 
                           nodalArrays[k].size(), 
                           &nodalArrays[k][0]);
          nodalIndex++;
        }
        nodalComponentIndex++;
      }
      clock_t end = clock();
      std::cout << attributeName << " " 
                << ((double)(end-start) / CLOCKS_PER_SEC) 
                << std::endl;
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

  std::cout << "Done Attributes" << std::endl;

  // close exodus file
  ex_close(exodusHandle);

}
