//
//
//
#include "ExodusReader.h"
#include <vector>
#include <exodusII.h>
//
//
//
namespace dft
{
  //
  // Construct ExodusReader.
  //
  ExodusReader::ExodusReader()
  {
    return;
  }

  //
  // Destroy ExodusReader object.
  //
  ExodusReader::~ExodusReader()
  {
    return;
  }

  //
  // Translate from exodus ii to xdmf topologies
  // this was taken directly from vtkExodusIIReader and modified to fit xdmf elements
  //
  XdmfInt32 ExodusReader::DetermineXdmfCellType(char * exoElemType, int numPointsPerCell)
  {
    // Make exoType uppercase
    std::string elemType = exoElemType;
    std::transform(elemType.begin(), elemType.end(), elemType.begin(), toupper);

    // Check for quadratic elements
    if (elemType.substr(0,3) == "TRI" && numPointsPerCell == 6)
    {
      return XDMF_TRI_6;
    }
    else if (elemType.substr(0,3) == "SHE" && numPointsPerCell == 8)
    { 
      return XDMF_QUAD_8;
    }
    else if (elemType.substr(0,3) == "SHE" && numPointsPerCell == 9)
    { 
      return XDMF_QUAD_8;
    }
    else if (elemType.substr(0,3) == "TET" && numPointsPerCell == 10)
    { 
      return XDMF_TET_10;
    }
    else if (elemType.substr(0,3) == "TET" && numPointsPerCell == 11)
    { 
      return XDMF_TET_10;
    }
    else if (elemType.substr(0,3) == "HEX" && numPointsPerCell == 20)
    { 
      return XDMF_HEX_20;
    }
    else if (elemType.substr(0,3) == "HEX" && numPointsPerCell == 21)
    { 
      return XDMF_HEX_20;
    }
    else if (elemType.substr(0,3) == "HEX" && numPointsPerCell == 27)
    {
      // VTK_TRIQUADRATIC_HEXAHEDRON;
      // Currently unsupported in Xdmf
      // TODO: Throw dft exception
      return XDMF_NOTOPOLOGY;
    }
    else if (elemType.substr(0,3) == "QUA" && numPointsPerCell == 8)
    { 
      return XDMF_QUAD_8;
    }
    else if (elemType.substr(0,3) == "QUA" && numPointsPerCell == 9)
    { 
      // VTK_BIQUADRATIC_QUAD;
      // Currently unsupported in Xdmf
      // TODO: Throw dft exception
      return XDMF_NOTOPOLOGY;
    }
    else if (elemType.substr(0,3) == "TRU" && numPointsPerCell == 3)
    { 
      return XDMF_EDGE_3;
    }
    else if (elemType.substr(0,3) == "BEA" && numPointsPerCell == 3)
    { 
      return XDMF_EDGE_3;
    }
    else if (elemType.substr(0,3) == "BAR" && numPointsPerCell == 3)
    { 
      return XDMF_EDGE_3;
    }
    else if (elemType.substr(0,3) == "EDG" && numPointsPerCell == 3)
    { 
      return XDMF_EDGE_3;
    }

    // Check for regular elements
    else if (elemType.substr(0,3) == "CIR")
    {
      // VTK_VERTEX;
      // Currently unsupported in Xdmf
      // TODO: Throw dft exception
      return XDMF_NOTOPOLOGY;
    }
    else if (elemType.substr(0,3) == "SPH") 
    {
      // VTK_VERTEX;
      // Currently unsupported in Xdmf
      // TODO: Throw dft exception
      return XDMF_NOTOPOLOGY;
    }
    else if (elemType.substr(0,3) == "BAR") 
    {
      // VTK_LINE;
      // Currently unsupported in Xdmf
      // TODO: Throw dft exception
      return XDMF_NOTOPOLOGY;
    }
    else if (elemType.substr(0,3) == "TRU") 
    {
      // VTK_LINE;
      // Currently unsupported in Xdmf
      // TODO: Throw dft exception
      return XDMF_NOTOPOLOGY;
    }
    else if (elemType.substr(0,3) == "BEA") 
    {
      // VTK_LINE;
      // Currently unsupported in Xdmf
      // TODO: Throw dft exception
      return XDMF_NOTOPOLOGY;
    }
    else if (elemType.substr(0,3) == "EDG") 
    {
      // VTK_LINE;
      // Currently unsupported in Xdmf
      // TODO: Throw dft exception
      return XDMF_NOTOPOLOGY;
    }
    else if (elemType.substr(0,3) == "TRI") 
    {
      return XDMF_TRI;
    }
    else if (elemType.substr(0,3) == "QUA") 
    {
      return XDMF_QUAD;
    }
    else if (elemType.substr(0,3) == "TET") 
    {
      return XDMF_TET;
    }
    else if (elemType.substr(0,3) == "PYR") 
    {
      return XDMF_PYRAMID;
    }
    else if (elemType.substr(0,3) == "WED") 
    {
      return XDMF_WEDGE;
    }
    else if (elemType.substr(0,3) == "HEX") 
    {
      return XDMF_HEX;
    }
    else if (elemType.substr(0,3) == "SHE" && numPointsPerCell == 3)
    {
      return XDMF_TRI;
    }
    else if (elemType.substr(0,3) == "SHE" && numPointsPerCell == 4)
    {
      return XDMF_QUAD;
    }
    else if (elemType.substr(0,8) == "STRAIGHT" && numPointsPerCell == 2)
    {
      // VTK_LINE;
      // Currently unsupported in Xdmf
      // TODO: Throw dft exception
      return XDMF_NOTOPOLOGY;
    }
    else if (elemType.substr(0,3) == "SUP")
    {
      return XDMF_POLYVERTEX;
    }
    //TODO: Throw dft exception
    //vtkErrorMacro("Unsupported element type: " << elemType.c_str());
    return XDMF_NOTOPOLOGY;
  }

  //
  // Read contents of the exodus file and fill in xdmf grids.
  //
  XdmfGrid * ExodusReader::read(const char * fileName, XdmfElement * parentElement)
  {
    // TODO: Better introspection of exodus number types
    
    // Initialize XdmfGrid to fill with info from exodus and insert into parent
    XdmfGrid * grid = new XdmfGrid();
    parentElement->Insert(grid);

    // Read Exodus II file to XdmfGrid via Exodus II API
 
    float version;
    int CPU_word_size = sizeof(double);
    int IO_word_size = 0; // Get from file
    int exodusHandle = ex_open(fileName, EX_READ, &CPU_word_size, &IO_word_size, &version);
      
    char * title = new char[MAX_LINE_LENGTH+1];
    int num_dim, num_nodes, num_elem, num_elem_blk, num_node_sets, num_side_sets;
    ex_get_init (exodusHandle, title, &num_dim, &num_nodes, &num_elem, &num_elem_blk, &num_node_sets, &num_side_sets);

    /*
    cout << "Title: " << title <<
      "\nNum Dim: " << num_dim <<
      "\nNum Nodes: " << num_nodes <<
      "\nNum Elem: " << num_elem <<
      "\nNum Elem Blk: " << num_elem_blk <<
      "\nNum Node Sets: " << num_node_sets <<
      "\nNum Side Sets: " << num_side_sets << endl;
    */

    // Allocate point arrays
    double * x = new double[num_nodes];
    double * y = new double[num_nodes];
    double * z = new double[num_nodes];

    ex_get_coord(exodusHandle, x, y, z);

    // Write points to xdmf 
    // In the future we may want to do XDMF_GEOMETRY_X_Y_Z?
    XdmfGeometry * geom = grid->GetGeometry();
    geom->SetGeometryType(XDMF_GEOMETRY_XYZ);
    geom->SetNumberOfPoints(num_nodes);
      
    XdmfArray * points = geom->GetPoints();
    points->SetNumberType(XDMF_FLOAT64_TYPE);
    points->SetNumberOfElements(num_nodes * 3);
    for (int j=0; j<num_nodes; j++)
    {
      points->SetValue((j*3), x[j]);
      points->SetValue((j*3)+1, y[j]);
      points->SetValue((j*3)+2, z[j]);
    }      

    // Get element block ids and iterate over blocks to read connectivity
    int * blockIds = new int[num_elem_blk];
    ex_get_elem_blk_ids(exodusHandle, blockIds);

    // TODO: Make this work for mixed topologies?
    std::vector<int> conn;
    XdmfInt32 topType;
    int totalNumElem = 0;
    for (int j=0; j<num_elem_blk; j++)
    {
      // Read block info
      char * elem_type = new char[MAX_LINE_LENGTH+1];
      int num_nodes_per_elem, num_elem_this_blk, num_attr;
      ex_get_elem_block(exodusHandle, blockIds[j], elem_type, &num_elem_this_blk, &num_nodes_per_elem, &num_attr);
      
      /*
      cout << "Block Id: " << blockIds[j] <<
        "\nElem Type: " << elem_type <<
        "\nNum Elem in Blk: " << num_elem_this_blk <<
        "\nNum Nodes per Elem: " << num_nodes_per_elem <<
        "\nNum Attr: " << num_attr << endl;
      */

      XdmfInt32 currTopType = this->DetermineXdmfCellType(elem_type, num_nodes_per_elem);
      if(currTopType != XDMF_NOTOPOLOGY)
      {
        topType = currTopType;

        // Read connectivity
        int * currConn = new int[num_nodes_per_elem * num_elem_this_blk];
        ex_get_elem_conn(exodusHandle, blockIds[j], currConn);
      
        // Insert connectivity into overall connectivity array 
        conn.insert(conn.end(), currConn, currConn + (num_nodes_per_elem * num_elem_this_blk));
  
        totalNumElem = totalNumElem + num_elem_this_blk;
      }
    }

    // Write connectivity to xdmf
    XdmfTopology * topology = grid->GetTopology();
    topology->SetTopologyType(topType);
    topology->SetNumberOfElements(totalNumElem);
      
    XdmfArray * connections = topology->GetConnectivity();
    connections->SetNumberType(XDMF_INT32_TYPE);
    connections->SetNumberOfElements(conn.size());
    connections->SetValues(0, &conn[0], conn.size(), 1, 1);
    // Subtract all node ids by 1 since exodus indices start at 1
    *connections - 1;

    // Get nodal map to global ids
    int * node_map = new int[num_nodes];
    ex_get_node_num_map(exodusHandle, node_map);

    // Write global ids to xdmf
    XdmfAttribute * globalIds = new XdmfAttribute();
    globalIds->SetName("GlobalNodeId");
    globalIds->SetAttributeType(XDMF_ATTRIBUTE_TYPE_SCALAR);
    globalIds->SetAttributeCenter(XDMF_ATTRIBUTE_CENTER_NODE);

    XdmfArray * globalNodeIdVals = globalIds->GetValues();
    globalNodeIdVals->SetNumberType(XDMF_INT32_TYPE);
    globalNodeIdVals->SetNumberOfElements(num_nodes);
    globalNodeIdVals->SetValues(0, &node_map[0], num_nodes, 1, 1);
    // Subtract all node ids by 1 since exodus indices start at 1
    *globalNodeIdVals - 1;

    grid->Insert(globalIds);

    // Read node sets
    // Get node set ids and iterate over sets to read
    int * nodeSetIds = new int[num_node_sets];
    ex_get_node_set_ids(exodusHandle, nodeSetIds);

    // Get node set names
    char * node_set_names[num_node_sets];
    for (int j=0; j<num_node_sets; j++)
    {
      node_set_names[j] = new char[MAX_LINE_LENGTH+1];
    }
    ex_get_names(exodusHandle, EX_NODE_SET, node_set_names);
 
    for (int j=0; j<num_node_sets; j++)
    {
      int num_nodes_in_set, num_df_in_set;
      ex_get_node_set_param(exodusHandle, nodeSetIds[j], &num_nodes_in_set, &num_df_in_set);
      
      /*
      cout << "Node Set Id: " << nodeSetIds[j] <<
        "\nNode Set Name: " << node_set_names[j] << 
        "\nNum Nodes in Set: "<< num_nodes_in_set <<
        "\nNum Distrub Factors: " << num_df_in_set << endl;
      */

      if(num_nodes_in_set > 0)
      {
        // Read node set values
        int * node_set_node_list = new int[num_nodes_in_set];
        ex_get_node_set(exodusHandle, nodeSetIds[j], node_set_node_list);

        XdmfSet * set = new XdmfSet();
        set->SetName(node_set_names[j]);
        set->SetSetType(XDMF_SET_TYPE_NODE);
        set->SetSize(num_nodes_in_set);
        
        XdmfArray * ids = set->GetIds();
        ids->SetNumberType(XDMF_INT32_TYPE);
        ids->SetNumberOfElements(num_nodes_in_set);
        ids->SetValues(0, node_set_node_list, num_nodes_in_set, 1, 1);
        // Subtract all node ids by 1 since exodus indices start at 1
        *ids - 1;
        grid->Insert(set);
      }
    }
    ex_close(exodusHandle);
    return grid;
  }
}
