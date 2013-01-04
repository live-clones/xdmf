/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfExodusReader.cpp                                                */
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

#include <exodusII.h>
#include <cstring>
#include "XdmfArrayType.hpp"
#include "XdmfAttribute.hpp"
#include "XdmfAttributeCenter.hpp"
#include "XdmfAttributeType.hpp"
#include "XdmfExodusReader.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfGeometryType.hpp"
#include "XdmfHeavyDataWriter.hpp"
#include "XdmfSet.hpp"
#include "XdmfSetType.hpp"
#include "XdmfTopology.hpp"
#include "XdmfTopologyType.hpp"
#include "XdmfUnstructuredGrid.hpp"
#include "XdmfError.hpp"

//
// local methods
//
namespace {

  /**
   * Convert exodus topology type to xdmf topology type.
   *
   * @param exodusTopologyType a string containing the name of the exodus
   * topology type.
   * @param pointsPerCell the number of points per cell for the exodus
   * topology type.
   *
   * @return the equivalent XdmfTopologyType. If no equivalent is found,
   * XdmfTopologyType::NoTopologyType() is returned.
   */
  shared_ptr<const XdmfTopologyType>
  exodusToXdmfTopologyType(std::string exodusTopologyType,
                           const int pointsPerCell)
  {
    // Convert exodusTopologyType to uppercase
    std::transform(exodusTopologyType.begin(),
                   exodusTopologyType.end(),
                   exodusTopologyType.begin(),
                   toupper);

    // First check for quadratic elements then look for linear elements
    if (exodusTopologyType.substr(0,3).compare("TRI") == 0 &&
        pointsPerCell == 6) {
      return XdmfTopologyType::Triangle_6();
    }
    else if (exodusTopologyType.substr(0,3).compare("SHE") == 0 &&
             pointsPerCell == 8) {
      return XdmfTopologyType::Quadrilateral_8();
    }
    else if (exodusTopologyType.substr(0,3).compare("SHE") == 0 &&
             pointsPerCell == 9) {
      return XdmfTopologyType::Quadrilateral_9();
    }
    else if (exodusTopologyType.substr(0,3).compare("TET") == 0 &&
             pointsPerCell == 10) {
      return XdmfTopologyType::Tetrahedron_10();
    }
    else if (exodusTopologyType.substr(0,3).compare("TET") == 0 &&
             pointsPerCell == 11) {
      // VTK_QUADRATIC_TETRA with 11 points
      // Currently unsupported in Xdmf
      return XdmfTopologyType::NoTopologyType();
    }
    else if (exodusTopologyType.substr(0,3).compare("WED") == 0 &&
             pointsPerCell == 15) {
      return XdmfTopologyType::Wedge_15();
    }
    else if (exodusTopologyType.substr(0,3).compare("WED") == 0 &&
             pointsPerCell == 18) {
      return XdmfTopologyType::Wedge_18();
    }
    else if (exodusTopologyType.substr(0,3).compare("HEX") == 0 &&
             pointsPerCell == 20) {
      return XdmfTopologyType::Hexahedron_20();
    }
    else if (exodusTopologyType.substr(0,3).compare("HEX") == 0 &&
             pointsPerCell == 21) {
      // VTK_QUADRATIC_HEXAHEDRON with 21 points
      // Currently unsupported in Xdmf
      return XdmfTopologyType::NoTopologyType();
    }
    else if (exodusTopologyType.substr(0,3).compare("HEX") == 0 &&
             pointsPerCell == 27) {
      return XdmfTopologyType::Hexahedron_27();
    }
    else if (exodusTopologyType.substr(0,3).compare("QUA") == 0 &&
             pointsPerCell == 8) {
      return XdmfTopologyType::Quadrilateral_8();
    }
    else if (exodusTopologyType.substr(0,3).compare("QUA") == 0 &&
             pointsPerCell == 9) {
      return XdmfTopologyType::Quadrilateral_9();
    }
    else if (exodusTopologyType.substr(0,3).compare("TRU") == 0 &&
             pointsPerCell == 3) {
      return XdmfTopologyType::Edge_3();
    }
    else if (exodusTopologyType.substr(0,3).compare("BEA") == 0 &&
             pointsPerCell == 3) {
      return XdmfTopologyType::Edge_3();
    }
    else if (exodusTopologyType.substr(0,3).compare("BAR") == 0 &&
             pointsPerCell == 3) {
      return XdmfTopologyType::Edge_3();
    }
    else if (exodusTopologyType.substr(0,3).compare("EDG") == 0 &&
             pointsPerCell == 3) {
      return XdmfTopologyType::Edge_3();
    }
    else if (exodusTopologyType.substr(0,3).compare("CIR") == 0) {
      // VTK_VERTEX;
      // Currently unsupported in Xdmf
      return XdmfTopologyType::NoTopologyType();
    }
    else if (exodusTopologyType.substr(0,3).compare("SPH") == 0) {
      // VTK_VERTEX;
      // Currently unsupported in Xdmf
      return XdmfTopologyType::NoTopologyType();
    }
    else if (exodusTopologyType.substr(0,3).compare("BAR") == 0) {
      // VTK_LINE;
      // Currently unsupported in Xdmf
      return XdmfTopologyType::NoTopologyType();
    }
    else if (exodusTopologyType.substr(0,3).compare("TRU") == 0) {
      // VTK_LINE;
      // Currently unsupported in Xdmf
      return XdmfTopologyType::NoTopologyType();
    }
    else if (exodusTopologyType.substr(0,3).compare("BEA") == 0) {
      // VTK_LINE;
      // Currently unsupported in Xdmf
      return XdmfTopologyType::NoTopologyType();
    }
    else if (exodusTopologyType.substr(0,3).compare("EDG") == 0) {
      // VTK_LINE;
      // Currently unsupported in Xdmf
      return XdmfTopologyType::NoTopologyType();
    }
    else if (exodusTopologyType.substr(0,3).compare("TRI") == 0) {
      return XdmfTopologyType::Triangle();
    }
    else if (exodusTopologyType.substr(0,3).compare("QUA") == 0) {
      return XdmfTopologyType::Quadrilateral();
    }
    else if (exodusTopologyType.substr(0,3).compare("TET") == 0) {
      return XdmfTopologyType::Tetrahedron();
    }
    else if (exodusTopologyType.substr(0,3).compare("PYR") == 0) {
      return XdmfTopologyType::Pyramid();
    }
    else if (exodusTopologyType.substr(0,3).compare("WED") == 0) {
      return XdmfTopologyType::Wedge();
    }
    else if (exodusTopologyType.substr(0,3).compare("HEX") == 0) {
      return XdmfTopologyType::Hexahedron();
    }
    else if (exodusTopologyType.substr(0,3).compare("SHE") == 0 &&
             pointsPerCell == 3) {
      return XdmfTopologyType::Triangle();
    }
    else if (exodusTopologyType.substr(0,3).compare("SHE") == 0 &&
             pointsPerCell == 4) {
      return XdmfTopologyType::Quadrilateral();
    }
    else if (exodusTopologyType.substr(0,8).compare("STRAIGHT") == 0 &&
             pointsPerCell == 2) {
      // VTK_LINE;
      // Currently unsupported in Xdmf
      return XdmfTopologyType::NoTopologyType();
    }
    else if (exodusTopologyType.substr(0,3).compare("SUP") == 0) {
      return XdmfTopologyType::Polyvertex();
    }
    return XdmfTopologyType::NoTopologyType();
  }
}

shared_ptr<XdmfExodusReader>
XdmfExodusReader::New()
{
  shared_ptr<XdmfExodusReader> p(new XdmfExodusReader());
  return p;
}

XdmfExodusReader::XdmfExodusReader()
{
}

XdmfExodusReader::~XdmfExodusReader()
{
}

shared_ptr<XdmfUnstructuredGrid>
XdmfExodusReader::read(const std::string & fileName,
                       const shared_ptr<XdmfHeavyDataWriter> heavyDataWriter) const
{
  if(heavyDataWriter) {
    heavyDataWriter->openFile();
  }

  shared_ptr<XdmfUnstructuredGrid> toReturn = XdmfUnstructuredGrid::New();

  // Read Exodus II file to XdmfGridUnstructured via Exodus II API
  float version;
  int CPU_word_size = sizeof(double);
  int IO_word_size = 0; // Get from file
  int exodusHandle = ex_open(fileName.c_str(),
                             EX_READ,
                             &CPU_word_size,
                             &IO_word_size,
                             &version);

  if(exodusHandle < 0) {
    // Invalid fileName
    XdmfError::message(XdmfError::FATAL, "Invalid fileName: "+fileName+" in XdmfExodusReader::read");
  }

  char * title = new char[MAX_LINE_LENGTH+1];
  int num_dim, num_nodes, num_elem, num_elem_blk, num_node_sets, num_side_sets;
  ex_get_init (exodusHandle,
               title,
               &num_dim,
               &num_nodes,
               &num_elem,
               &num_elem_blk,
               &num_node_sets,
               &num_side_sets);
  toReturn->setName(title);
  delete [] title;

  /*
    cout << "Title: " << title <<
    "\nNum Dim: " << num_dim <<
    "\nNum Nodes: " << num_nodes <<
    "\nNum Elem: " << num_elem <<
    "\nNum Elem Blk: " << num_elem_blk <<
    "\nNum Node Sets: " << num_node_sets <<
    "\nNum Side Sets: " << num_side_sets << endl;
  */

  // Read geometry values
  double * x = new double[num_nodes];
  double * y = new double[num_nodes];
  double * z = new double[num_nodes];

  ex_get_coord(exodusHandle, x, y, z);

  // In the future we may want to do XDMF_GEOMETRY_X_Y_Z?
  if(num_dim == 3) {
    toReturn->getGeometry()->setType(XdmfGeometryType::XYZ());
  }
  else if(num_dim == 2) {
    toReturn->getGeometry()->setType(XdmfGeometryType::XY());
  }
  else {
    // Xdmf does not support geometries with less than 2 dimensions
    std::ostringstream oss;
    oss << "Xdmf does not support geometries with less than 2 dimensions -- num_dim: " << num_dim << "-- in XdmfExodusReader::read";
    XdmfError::message(XdmfError::FATAL, oss.str());
  }

  toReturn->getGeometry()->initialize(XdmfArrayType::Float64());
  toReturn->getGeometry()->reserve(num_nodes * num_dim);
  for(int i=0; i<num_nodes; ++i) {
    toReturn->getGeometry()->pushBack(x[i]);
    toReturn->getGeometry()->pushBack(y[i]);
    if(num_dim == 3) {
      toReturn->getGeometry()->pushBack(z[i]);
    }
  }
  delete [] x;
  delete [] y;
  delete [] z;

  if(heavyDataWriter) {
    toReturn->getGeometry()->accept(heavyDataWriter);
    toReturn->getGeometry()->release();
  }

  int * blockIds = new int[num_elem_blk];
  ex_get_elem_blk_ids(exodusHandle, blockIds);

  int * numElemsInBlock = new int[num_elem_blk];
  int * numNodesPerElemInBlock = new int[num_elem_blk];
  int * numElemAttrInBlock = new int[num_elem_blk];
  std::vector<shared_ptr<const XdmfTopologyType> > topologyTypes;
  topologyTypes.reserve(num_elem_blk);
  int totalNumElem = 0;
  int totalConns = 0;
  for(int i=0; i<num_elem_blk; ++i) {
    char * elem_type = new char[MAX_STR_LENGTH+1];
    int num_nodes_per_elem, num_elem_this_blk, num_attr;
    ex_get_elem_block(exodusHandle,
                      blockIds[i],
                      elem_type,
                      &num_elem_this_blk,
                      &num_nodes_per_elem,
                      &num_attr);

    /*
      cout << "Block Id: " << blockIds[j] <<
      "\nElem Type: " << elem_type <<
      "\nNum Elem in Blk: " << num_elem_this_blk <<
      "\nNum Nodes per Elem: " << num_nodes_per_elem <<
      "\nNum Attr: " << num_attr << endl;
    */

    numElemsInBlock[i] = num_elem_this_blk;
    numNodesPerElemInBlock[i] = num_nodes_per_elem;
    numElemAttrInBlock[i] = num_attr;
    const shared_ptr<const XdmfTopologyType> topologyType =
      exodusToXdmfTopologyType(elem_type, num_nodes_per_elem);
    topologyTypes.push_back(topologyType);
    totalNumElem += num_elem_this_blk;
    totalConns += num_elem_this_blk * num_nodes_per_elem;
    delete [] elem_type;
  }

  if(topologyTypes.size() > 0) {
    toReturn->getTopology()->setType(topologyTypes[0]);
    if(topologyTypes.size() > 1) {
      for(std::vector<shared_ptr<const XdmfTopologyType> >::const_iterator
            iter = topologyTypes.begin() + 1;
          iter != topologyTypes.end();
          ++iter) {
        // Cannot be mixed topology!
        if(toReturn->getTopology()->getType() != *iter)
          XdmfError::message(XdmfError::FATAL, "Requested mix of topology types -- "+toReturn->getTopology()->getType()->getName()+" and "+(*iter)->getName()+" in XdmfExodusReader::read");
      }
    }
  }

  topologyTypes.clear();

  toReturn->getTopology()->initialize(XdmfArrayType::Int32(), totalConns);
  int * connectivityPointer =
    (int *)toReturn->getTopology()->getValuesInternal();
  // Read connectivity from element blocks
  int elemIndex = 0;
  for(int i=0; i<num_elem_blk; ++i) {
    ex_get_elem_conn(exodusHandle,
                     blockIds[i],
                     connectivityPointer + elemIndex);
    elemIndex += numElemsInBlock[i] * numNodesPerElemInBlock[i];
  }

  // This is taken from VTK's vtkExodusIIReader and adapted to fit Xdmf
  // element types, which have the same ordering as VTK.
  if(toReturn->getTopology()->getType() == XdmfTopologyType::Hexahedron_20() ||
     toReturn->getTopology()->getType() == XdmfTopologyType::Hexahedron_27()) {
    int * ptr = connectivityPointer;
    int itmp[4];

    // Exodus Node ordering does not match Xdmf, we must convert.
    for(int i=0; i<totalNumElem; ++i) {
      ptr += 12;

      for(unsigned int j=0; j<4; ++j, ++ptr) {
        itmp[j] = *ptr;
        *ptr = ptr[4];
      }

      for(unsigned int j=0; j<4; ++j, ++ptr) {
        *ptr = itmp[j];
      }

      if(toReturn->getTopology()->getType() == XdmfTopologyType::Hexahedron_27()) {
        for(unsigned int j=0; j<4; ++j, ++ptr) {
          itmp[j] = *ptr;
          *ptr = ptr[3];
        }
        *(ptr++) = itmp[1];
        *(ptr++) = itmp[2];
        *(ptr++) = itmp[0];
      }
    }
  }
  else if(toReturn->getTopology()->getType() == XdmfTopologyType::Wedge_15() ||
          toReturn->getTopology()->getType() == XdmfTopologyType::Wedge_18()) {
    int * ptr = connectivityPointer;
    int itmp[3];

    // Exodus Node ordering does not match Xdmf, we must convert.
    for (int i=0; i<totalNumElem; i++) {
      ptr += 9;

      for(unsigned int j=0; j<3; ++j, ++ptr) {
        itmp[j] = *ptr;
        *ptr = ptr[3];
      }

      for(unsigned int j=0; j<3; ++j, ++ptr) {
        *ptr = itmp[j];
      }

      if(toReturn->getTopology()->getType() == XdmfTopologyType::Wedge_18()) {
        itmp[0] = *(ptr);
        itmp[1] = *(ptr+1);
        itmp[2] = *(ptr+2);
        *(ptr++) = itmp[1];
        *(ptr++) = itmp[2];
        *(ptr++) = itmp[0];
      }
    }
  }

  // Subtract all node ids by 1 since exodus indices start at 1
  for(int i=0; i<totalConns; ++i) {
    connectivityPointer[i]--;
  }

  if(heavyDataWriter) {
    toReturn->getTopology()->accept(heavyDataWriter);
    toReturn->getTopology()->release();
  }

  shared_ptr<XdmfAttribute> globalIds = XdmfAttribute::New();
  globalIds->setName("GlobalNodeId");
  globalIds->setCenter(XdmfAttributeCenter::Node());
  globalIds->setType(XdmfAttributeType::GlobalId());
  globalIds->initialize(XdmfArrayType::Int32(), num_nodes);
  int * globalIdsPointer = (int*)globalIds->getValuesInternal();

  ex_get_node_num_map(exodusHandle, globalIdsPointer);

  // Subtract all node ids by 1 since exodus indices start at 1
  for(int i=0; i<num_nodes; ++i) {
    globalIdsPointer[i]--;
  }

  toReturn->insert(globalIds);

  if(heavyDataWriter) {
    globalIds->accept(heavyDataWriter);
    globalIds->release();
  }

  // Read node sets
  int * nodeSetIds = new int[num_node_sets];
  ex_get_node_set_ids(exodusHandle, nodeSetIds);

  char * node_set_names[num_node_sets];
  for (int i=0; i<num_node_sets; ++i) {
    node_set_names[i] = new char[MAX_STR_LENGTH+1];
  }
  ex_get_names(exodusHandle, EX_NODE_SET, node_set_names);

  for (int i=0; i<num_node_sets; ++i) {
    int num_nodes_in_set, num_df_in_set;
    ex_get_node_set_param(exodusHandle,
                          nodeSetIds[i],
                          &num_nodes_in_set,
                          &num_df_in_set);

    /*
      cout << "Node Set Id: " << nodeSetIds[j] <<
      "\nNode Set Name: " << node_set_names[j] <<
      "\nNum Nodes in Set: "<< num_nodes_in_set <<
      "\nNum Distrub Factors: " << num_df_in_set << endl;
    */

    if (num_nodes_in_set > 0) {
      shared_ptr<XdmfSet> set = XdmfSet::New();
      set->setName(node_set_names[i]);
      set->setType(XdmfSetType::Node());
      set->initialize(XdmfArrayType::Int32(), num_nodes_in_set);
      int * setPointer = (int*)set->getValuesInternal();
      ex_get_node_set(exodusHandle, nodeSetIds[i], setPointer);

      // Subtract all node ids by 1 since exodus indices start at 1
      for(int j=0; j<num_nodes_in_set; ++j) {
        setPointer[j]--;
      }

      toReturn->insert(set);

      if(heavyDataWriter) {
        set->accept(heavyDataWriter);
        set->release();
      }
    }
    delete [] node_set_names[i];
  }
  delete [] nodeSetIds;

  // Read result variables (attributes)
  int num_global_vars, num_nodal_vars, num_elem_vars;
  ex_get_var_param(exodusHandle, "g", &num_global_vars);
  ex_get_var_param(exodusHandle, "n", &num_nodal_vars);
  ex_get_var_param(exodusHandle, "e", &num_elem_vars);

  /*
    cout << "Num Global Vars: " << num_global_vars <<
    "\nNum Nodal Vars: " << num_nodal_vars <<
    "\nNum Elem Vars: " << num_elem_vars << endl;
  */

  char * global_var_names[num_global_vars];
  char * nodal_var_names[num_nodal_vars];
  char * elem_var_names[num_elem_vars];
  for (int j=0; j<num_global_vars; j++) {
    global_var_names[j] = new char[MAX_STR_LENGTH+1];
  }
  for (int j=0; j<num_nodal_vars; j++) {
    nodal_var_names[j] = new char[MAX_STR_LENGTH+1];
  }
  for (int j=0; j<num_elem_vars; j++) {
    elem_var_names[j] = new char[MAX_STR_LENGTH+1];
  }
  ex_get_var_names(exodusHandle, "g", num_global_vars, global_var_names);
  ex_get_var_names(exodusHandle, "n", num_nodal_vars, nodal_var_names);
  ex_get_var_names(exodusHandle, "e", num_elem_vars, elem_var_names);

  /*
    cout << "Global Vars Names: " << endl;
    for (int j=0; j<num_global_vars; j++)
    {
    cout << global_var_names[j] << endl;
    }
    cout << "Nodal Vars Names: " << endl;
    for (int j=0; j<num_nodal_vars; j++)
    {
    cout << nodal_var_names[j] << endl;
    }
    cout << "Elem Vars Names: " << endl;
    for (int j=0; j<num_elem_vars; j++)
    {
    cout << elem_var_names[j] << endl;
    }
  */

  // Get variable data
  // TODO: do this for all timesteps?

  // Global variable data
  double * global_var_vals = new double[num_global_vars];
  ex_get_glob_vars(exodusHandle, 1, num_global_vars, global_var_vals);
  for (int i=0; i<num_global_vars; ++i) {
    // Write global attribute to xdmf
    shared_ptr<XdmfAttribute> attribute = XdmfAttribute::New();
    attribute->setName(global_var_names[i]);
    attribute->setCenter(XdmfAttributeCenter::Grid());
    attribute->setType(XdmfAttributeType::Scalar());
    attribute->initialize(XdmfArrayType::Float64());
    attribute->pushBack(global_var_vals[i]);
    toReturn->insert(attribute);
    if(heavyDataWriter) {
      attribute->accept(heavyDataWriter);
      attribute->release();
    }
    delete [] global_var_names[i];
  }
  delete [] global_var_vals;

  // Nodal variable data
  for (int i=0; i<num_nodal_vars; ++i) {
    // The strcmp with "GlobalNodeId" is meant to prevent errors from occuring
    // when a nodal variable is named GlobalNodeId. A GlobalNodeId attribute
    // was added before when adding the nodal map which means that this
    // attribute should be ignored... This will probably only occur when doing
    // repeated conversions --- i.e. Xdmf to Exodus to Xdmf to Exodus...
    if (strcmp(nodal_var_names[i], "GlobalNodeId") != 0) {
      shared_ptr<XdmfAttribute> attribute = XdmfAttribute::New();
      attribute->setName(nodal_var_names[i]);
      attribute->setCenter(XdmfAttributeCenter::Node());
      attribute->setType(XdmfAttributeType::Scalar());
      attribute->initialize(XdmfArrayType::Float64(), num_nodes);
      ex_get_nodal_var(exodusHandle,
                       1,
                       i+1,
                       num_nodes,
                       (double*)attribute->getValuesInternal());
      toReturn->insert(attribute);
      if(heavyDataWriter) {
        attribute->accept(heavyDataWriter);
        attribute->release();
      }
      delete [] nodal_var_names[i];
    }
  }

  // Element variable data
  for (int i=0; i<num_elem_vars; ++i) {
    shared_ptr<XdmfAttribute> attribute = XdmfAttribute::New();
    attribute->setName(elem_var_names[i]);
    attribute->setCenter(XdmfAttributeCenter::Cell());
    attribute->setType(XdmfAttributeType::Scalar());
    attribute->initialize(XdmfArrayType::Float64(), totalNumElem);
    elemIndex = 0;
    for (int j=0; j<num_elem_blk; ++j) {
      ex_get_elem_var(exodusHandle,
                      1,
                      i+1,
                      blockIds[j],
                      numElemsInBlock[j],
                      (double*)attribute->getValuesInternal() + elemIndex);
      elemIndex += numElemsInBlock[j];
    }
    toReturn->insert(attribute);
    if(heavyDataWriter) {
      attribute->accept(heavyDataWriter);
      attribute->release();
    }
    delete [] elem_var_names[i];
  }

  ex_close(exodusHandle);

  delete [] blockIds;
  delete [] numElemsInBlock;
  delete [] numNodesPerElemInBlock;
  delete [] numElemAttrInBlock;

  if(heavyDataWriter) {
    heavyDataWriter->closeFile();
  }

  return toReturn;
}
