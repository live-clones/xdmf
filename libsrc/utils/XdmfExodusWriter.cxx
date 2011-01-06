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

#include "XdmfExodusWriter.h"

#include <cassert>
#include <exodusII.h>
#include <set>
#include <sstream>
#include <string>
#include <vector>

class XdmfExodusWriterNameHandler
{
  public:
    // Helper function to construct attribute names for attributes with > 1 component since exodus
    // cannot store vectors.  Also deals with MAX_STR_LENGTH limitation in exodus.
    void ConstructAttributeName(const char * attributeName, std::vector<std::string>& names, int numComponents)
    {
      std::string name = attributeName;
      if(numComponents == 1)
      {
        if(name.length() > MAX_STR_LENGTH)
        {
          name = name.substr(0, MAX_STR_LENGTH);
        }
        names.push_back(name);
      }
      else if(numComponents > 1)
      {
        int numComponentDigits = int(numComponents / 10);
        if(name.length() + numComponentDigits > MAX_STR_LENGTH)
        {
          name = name.substr(0, MAX_STR_LENGTH - numComponentDigits);
        }
        for(int j=0; j<numComponents; ++j)
        {
          std::stringstream toAdd;
          toAdd << name << "-" << j+1;
          names.push_back(toAdd.str());
        }
      }
    }
};

//
// Construct XdmfExodusWriter.
//
XdmfExodusWriter::XdmfExodusWriter()
{
  nameHandler = new XdmfExodusWriterNameHandler();
  return;
}

//
// Destroy XdmfExodusWriter
//
XdmfExodusWriter::~XdmfExodusWriter()
{
  delete nameHandler;
  return;
}

// Take Xdmf TopologyType and return Exodus Topology Type
std::string XdmfExodusWriter::DetermineExodusCellType(XdmfInt32 xdmfElementType)
{
  switch(xdmfElementType)
  {
    case(XDMF_POLYVERTEX):
    {
      return "SUP";
    }
    case(XDMF_TRI):
    {
      return "TRIANGLE";
    }
    case(XDMF_QUAD):
    {
      return "QUAD";
    }
    case(XDMF_TET):
    {
      return "TETRA";
    }
    case(XDMF_PYRAMID):
    {
      return "PYRAMID";
    }
    case(XDMF_WEDGE):
    {
      return "WEDGE";
    }
    case(XDMF_HEX):
    {
      return "HEX";
    }
    case(XDMF_EDGE_3):
    {
      return "EDGE";
    }
    case(XDMF_TRI_6):
    {
      return "TRIANGLE";
    }
    case(XDMF_QUAD_8):
    {
      return "QUAD";
    }
    case(XDMF_QUAD_9):
    {
      return "QUAD";
    }
    case(XDMF_TET_10):
    {
      return "TETRA";
    }
    case(XDMF_WEDGE_15):
    {
      return "WEDGE";
    }
    case(XDMF_HEX_20):
    {
      return "HEX";
    }
    case(XDMF_HEX_27):
    {
      return "HEX";
    }
  }
  return "";
}

//
// Write contents of the XdmfGrid to exodus file.
//
void XdmfExodusWriter::write(const char * fileName, XdmfGrid * gridToWrite)
{
  // Open Exodus File
  int wordSize = 8;
  int storeSize = 8;
  int exodusHandle = ex_create(fileName, EX_CLOBBER, &wordSize, &storeSize);

  // Initialize Exodus File
  std::string title = gridToWrite->GetName();
  if(title.length() > MAX_STR_LENGTH)
  {
    title = title.substr(0, MAX_STR_LENGTH);
  }

  XdmfGrid * currGrid = gridToWrite;
  bool temporalCollection = false;
  bool spatialCollection = false;
  if(currGrid->GetGridType() == XDMF_GRID_COLLECTION && currGrid->GetCollectionType() == XDMF_GRID_COLLECTION_TEMPORAL)
  {
    // Writing a temporal collection, use first timestep for geometry and topology values.
    currGrid = currGrid->GetChild(0);
    currGrid->Update();
    temporalCollection = true;
  }

  if(currGrid->GetGridType() == XDMF_GRID_COLLECTION && currGrid->GetCollectionType() == XDMF_GRID_COLLECTION_SPATIAL)
  {
    // Writing a spatial collection
    spatialCollection = true;
  }

  XdmfInt32 geomType;
  if(spatialCollection)
  {
    geomType = currGrid->GetChild(0)->GetGeometry()->GetGeometryType();
  }
  else
  {
    geomType = currGrid->GetGeometry()->GetGeometryType();
  }

  int num_dim;
  switch(geomType)
  {
    case(XDMF_GEOMETRY_XYZ):
      num_dim = 3;
      break;
    case(XDMF_GEOMETRY_XY):
      num_dim = 2;
      break;
    case(XDMF_GEOMETRY_X_Y_Z):
      num_dim = 3;
      break;
    case(XDMF_GEOMETRY_X_Y):
      num_dim = 2;
      break;
    default:
      XdmfErrorMessage("Cannot write grid with geometry " << currGrid->GetGeometry()->GetGeometryTypeAsString() << " to exodus file.");
      return;
  }

  int num_nodes;
  int num_elem;
  int num_elem_blk;;
  int num_node_sets = 0;
  int num_side_sets = 0;

  int * elem_blk_ids;
  char ** elem_type;
  int * num_elem_this_blk;
  int * num_nodes_per_elem;
  int * num_attr;
  XdmfAttribute ** globalNodeIdsArray;

  if(spatialCollection)
  {
    num_elem_blk = currGrid->GetNumberOfChildren();
    // Need to figure out number of pts and elements.
    num_nodes = 0;
    num_elem = 0;
    elem_blk_ids = new int[num_elem_blk];
    elem_type = new char*[num_elem_blk];
    num_elem_this_blk = new int[num_elem_blk];
    num_nodes_per_elem = new int[num_elem_blk];
    num_attr = new int[num_elem_blk];
    globalNodeIdsArray = new XdmfAttribute*[num_elem_blk];
    for(unsigned int i=0; i<num_elem_blk; ++i)
    {
      XdmfGrid * grid = currGrid->GetChild(i);

      // Write Element block parameters
      XdmfInt32 topType = grid->GetTopology()->GetTopologyType();
      std::string cellType = this->DetermineExodusCellType(topType);
      char * cellTypeCStr = new char[cellType.size() + 1];
      strcpy(cellTypeCStr, cellType.c_str());
      elem_type[i] = const_cast<char *>(cellTypeCStr);
      if (cellType.compare("") == 0)
      {
        XdmfErrorMessage("Cannot write grid with topology " << grid->GetTopology()->GetTopologyTypeAsString());
        return;
      }

      for(unsigned int j=0; j<grid->GetNumberOfAttributes(); ++j)
      {
        XdmfAttribute * attribute = grid->GetAttribute(j);
        if(strcmp(attribute->GetName(), "GlobalNodeId") == 0)
        {
          globalNodeIdsArray[i] = attribute;
          attribute->Update();
          const int maxIndex = attribute->GetValues()->GetMaxAsInt64();
          if(maxIndex > num_nodes)
          {
            num_nodes = maxIndex;
          }
          attribute->Release();
          break;
        }
      }
      int numberElements = grid->GetTopology()->GetNumberOfElements();
      num_elem += numberElements;
      elem_blk_ids[i] = 10 + i;
      num_elem_this_blk[i] = numberElements;
      num_nodes_per_elem[i] = grid->GetTopology()->GetNodesPerElement();
      num_attr[i] = 0;
    }
    if(num_nodes == 0)
    {
      XdmfErrorMessage("No GlobalNodeId attribute found for this spatial collection.  Exodus needs coordinate values stored globally, which means that we need to have a way to put them back together from Xdmf.");
      return;
    }
    num_nodes++;
  }
  else
  {
    num_nodes = currGrid->GetGeometry()->GetNumberOfPoints();
    num_elem = currGrid->GetTopology()->GetNumberOfElements();
    num_elem_blk = 1;
    elem_blk_ids = new int[1];
    elem_blk_ids[0] = 10;
    elem_type = new char*[1];
    XdmfInt32 topType = currGrid->GetTopology()->GetTopologyType();
    std::string cellType = this->DetermineExodusCellType(topType);
    char * cellTypeCStr = new char[cellType.size() + 1];
    strcpy(cellTypeCStr, cellType.c_str());
    elem_type[0] = const_cast<char *>(cellTypeCStr);
    num_elem_this_blk = new int[1];
    num_elem_this_blk[0] = num_elem;
    num_nodes_per_elem = new int[1];
    num_nodes_per_elem[0] = currGrid->GetTopology()->GetNodesPerElement();
    num_attr = new int[1];
    num_attr[0] = 0;
    globalNodeIdsArray = NULL;
  }
  
  ex_put_init(exodusHandle, title.c_str(), num_dim, num_nodes, num_elem, num_elem_blk, num_node_sets, num_side_sets);
  int define_maps(0);
  ex_put_concat_elem_block(exodusHandle, elem_blk_ids, elem_type, num_elem_this_blk, num_nodes_per_elem, num_attr, define_maps);

  for(int i=0; i<num_elem_blk; ++i)
  {
    delete [] elem_type[i];
  }

  delete [] num_nodes_per_elem;
  delete [] num_elem_this_blk;
  delete [] elem_type;
  delete [] elem_blk_ids;
  delete [] num_attr;

  double * x = new double[num_nodes];
  double * y = new double[num_nodes];
  double * z = new double[num_nodes];
  if(spatialCollection)
  {
    for(unsigned int i=0; i<currGrid->GetNumberOfChildren(); ++i)
    {
      XdmfGrid * grid = currGrid->GetChild(i);
      XdmfAttribute * globalNodeIds = globalNodeIdsArray[i];
      globalNodeIds->Update();
      XdmfArray * globalNodeIdVals = globalNodeIds->GetValues();
      XdmfArray * geometryVals = grid->GetGeometry()->GetPoints();
      const XdmfInt32 numberPoints = grid->GetGeometry()->GetNumberOfPoints();
      const XdmfInt32 geometryType = grid->GetGeometry()->GetGeometryType();
      unsigned int arrayOffset = 0;
      for(unsigned int j=0; j<numberPoints; ++j)
      {
        unsigned int currId = globalNodeIdVals->GetValueAsInt64(j);
        if(geometryType == XDMF_GEOMETRY_XYZ || 
           geometryType == XDMF_GEOMETRY_XY)
        {
          geometryVals->GetValues(arrayOffset++, &x[currId], 1);
          geometryVals->GetValues(arrayOffset++, &y[currId], 1);
          if(geometryType == XDMF_GEOMETRY_XYZ)
          {
            geometryVals->GetValues(arrayOffset++, &z[currId], 1);
          }
        }
        else if(geometryType == XDMF_GEOMETRY_X_Y_Z || 
                geometryType == XDMF_GEOMETRY_X_Y)
        {
          XdmfErrorMessage("Non-interlaced geometry types are currently not "
                           "supported in the exodus writer");
          return;
        }
      }
      grid->GetGeometry()->Release();
      globalNodeIds->Release();
    }
  }
  else
  {
    const XdmfInt32 geometryType = currGrid->GetGeometry()->GetGeometryType();
    XdmfArray * geometryVals = currGrid->GetGeometry()->GetPoints();
    // Write nodal coordinate values to exodus
    if(geometryType == XDMF_GEOMETRY_XYZ || 
       geometryType == XDMF_GEOMETRY_XY)
    {
      geometryVals->GetValues(0, x, num_nodes, 3);
      geometryVals->GetValues(1, y, num_nodes, 3);
      if(geometryType == XDMF_GEOMETRY_XYZ)
      {
        geometryVals->GetValues(2, z, num_nodes, 3);
      }
    }
    else if(geometryType == XDMF_GEOMETRY_X_Y_Z || 
	    geometryType == XDMF_GEOMETRY_X_Y)
    {
      XdmfErrorMessage("Non-interlaced geometry types are currently not "
		       "supported in the exodus writer");
      return;
    }
    currGrid->GetGeometry()->Release();
  }

  ex_put_coord(exodusHandle, x ,y ,z);
  delete [] x;
  delete [] y;
  delete [] z;

  clock_t start = clock();

  for(unsigned int iNumBlocks=0; iNumBlocks<num_elem_blk; ++iNumBlocks)
  {
    XdmfTopology * topology;
    if(spatialCollection)
    {
      topology = currGrid->GetChild(iNumBlocks)->GetTopology();
    }
    else
    {
      topology = currGrid->GetTopology();
    }

    XdmfArray * topValues = topology->GetConnectivity();
    XdmfInt32 topType = topology->GetTopologyType();

    // Write Element Connectivity
    int numberConnVals = topValues->GetNumberOfElements();
    int * elem_connectivity = new int[numberConnVals];

    if(spatialCollection)
    {
      XdmfAttribute * globalNodeIds = globalNodeIdsArray[iNumBlocks];
      globalNodeIds->Update();
      XdmfArray * globalNodeIdVals = globalNodeIds->GetValues();
      for(unsigned int i=0; i<numberConnVals; ++i)
      {
        elem_connectivity[i] = globalNodeIdVals->GetValueAsInt32(topValues->GetValueAsInt32(i)) + 1;
      }
      globalNodeIds->Release();
    }
    else
    {
      // Add 1 to connectivity array since exodus indices start at 1
      *topValues + 1;
      topValues->GetValues(0, elem_connectivity, numberConnVals);
    }

    topology->Release();

    if(topType == XDMF_HEX_20 || topType == XDMF_HEX_27)
    {
      int * ptr = elem_connectivity;
      int k;
      int itmp[4];

      // Exodus Node ordering does not match Xdmf, we must convert.
      for (int i=0; i<topology->GetNumberOfElements(); i++)
      {
        ptr += 12;

        for ( k = 0; k < 4; ++k, ++ptr)
        {
          itmp[k] = *ptr;
          *ptr = ptr[4];
        }

        for ( k = 0; k < 4; ++k, ++ptr )
        {
          *ptr = itmp[k];
        }

        if(topType == XDMF_HEX_27)
        {
          itmp[0] = *ptr;
          *(ptr++) = ptr[6];
          itmp[1] = *ptr;
          *(ptr++) = ptr[3];
          itmp[2] = *ptr;
          *(ptr++) = ptr[3];
          itmp[3] = *ptr;
          for ( k = 0; k < 4; ++k, ++ptr )
          {
            *ptr = itmp[k];
          }
        }
      }
    }
    else if (topType == XDMF_WEDGE_15 || topType == XDMF_WEDGE_18)
    {
      int * ptr = elem_connectivity;
      int k;
      int itmp[3];

      // Exodus Node ordering does not match Xdmf, we must convert.
      for (int i=0; i<topology->GetNumberOfElements(); i++)
      {
        ptr += 9;

        for (k = 0; k < 3; ++k, ++ptr)
        {
          itmp[k] = *ptr;
          *ptr = ptr[3];
        }

        for (k = 0; k < 3; ++k, ++ptr)
        {
          *ptr = itmp[k];
        }

        if(topType == XDMF_WEDGE_18)
        {
          itmp[0] = *(ptr);
          itmp[1] = *(ptr+1);
          itmp[2] = *(ptr+2);
          *(ptr++) = itmp[2];
          *(ptr++) = itmp[0];
          *(ptr++) = itmp[1];
        }
      }
    }

    ex_put_elem_conn(exodusHandle, 10 + iNumBlocks, elem_connectivity);
    delete [] elem_connectivity;
  }

  // Write Attributes
  int numGlobalAttributes = 0;
  int numNodalAttributes = 0;
  int numElementAttributes = 0;

  std::vector<int> globalComponents;
  std::vector<int> nodalComponents;
  std::vector<int> elementComponents;
  std::vector<std::string> globalAttributeNames;
  std::vector<std::string> nodalAttributeNames;
  std::vector<std::string> elementAttributeNames;

  int numberAttributes;
  if(spatialCollection)
  {
    numberAttributes = currGrid->GetChild(0)->GetNumberOfAttributes();
  }
  else
  {
    numberAttributes = currGrid->GetNumberOfAttributes();
  }

  for(int i=0; i<numberAttributes; ++i)
  {
    XdmfAttribute * currAttribute;
    int numElemInBlock;
    int numNodesInBlock;
    if(spatialCollection)
    {
      currAttribute = currGrid->GetChild(0)->GetAttribute(i);
      numElemInBlock = currGrid->GetChild(0)->GetTopology()->GetNumberOfElements();
      numNodesInBlock = currGrid->GetChild(0)->GetGeometry()->GetNumberOfPoints();
    }
    else
    {
      currAttribute = currGrid->GetAttribute(i);
      numElemInBlock = currGrid->GetTopology()->GetNumberOfElements();
      numNodesInBlock = currGrid->GetGeometry()->GetNumberOfPoints();
    }
    currAttribute->Update();
    int numComponents = 0;
    switch(currAttribute->GetAttributeCenter())
    {
      case(XDMF_ATTRIBUTE_CENTER_GRID):
      {
        numComponents = currAttribute->GetValues()->GetNumberOfElements();
        globalComponents.push_back(numComponents);
        numGlobalAttributes += numComponents;
        nameHandler->ConstructAttributeName(currAttribute->GetName(), globalAttributeNames, numComponents);
        break;
      }
      case(XDMF_ATTRIBUTE_CENTER_NODE):
      {
        numComponents = currAttribute->GetValues()->GetNumberOfElements() / numNodesInBlock;
        nodalComponents.push_back(numComponents);
        numNodalAttributes += numComponents;
        nameHandler->ConstructAttributeName(currAttribute->GetName(), nodalAttributeNames, numComponents);
        break;
      }
      case(XDMF_ATTRIBUTE_CENTER_CELL):
      {
        numComponents = currAttribute->GetValues()->GetNumberOfElements() / numElemInBlock;
        elementComponents.push_back(numComponents);
        numElementAttributes += numComponents;
        nameHandler->ConstructAttributeName(currAttribute->GetName(), elementAttributeNames, numComponents);
        break;
      }
    }
    currAttribute->Release();
  }

  ex_put_var_param(exodusHandle, "g", numGlobalAttributes);
  ex_put_var_param(exodusHandle, "n", numNodalAttributes);
  ex_put_var_param(exodusHandle, "e", numElementAttributes);

  char ** globalNames = new char*[numGlobalAttributes];
  char ** nodalNames = new char*[numNodalAttributes];
  char ** elementNames = new char*[numElementAttributes];

  for(int i=0; i<numGlobalAttributes; ++i)
  {
    globalNames[i] = (char*)globalAttributeNames[i].c_str();
  }

  for(int i=0; i<numNodalAttributes; ++i)
  {
    nodalNames[i] = (char*)nodalAttributeNames[i].c_str();
  }

  for(int i=0; i<numElementAttributes; ++i)
  {
    elementNames[i] = (char*)elementAttributeNames[i].c_str();
  }

  ex_put_var_names(exodusHandle, "g", numGlobalAttributes, globalNames);
  ex_put_var_names(exodusHandle, "n", numNodalAttributes, nodalNames);
  ex_put_var_names(exodusHandle, "e", numElementAttributes, elementNames);

  delete [] globalNames;
  delete [] nodalNames;
  delete [] elementNames;

  int numTemporalGrids = 1;
  if(temporalCollection)
  {
    numTemporalGrids = gridToWrite->GetNumberOfChildren();
  }
 
  for(int i=0; i<numTemporalGrids; ++i)
  {
    double * globalAttributeVals = new double[numGlobalAttributes];

    int globalIndex = 0;
    int globalComponentIndex = 0;
    int nodalIndex = 0;
    int nodalComponentIndex = 0;
    int elementIndex = 0;
    int elementComponentIndex = 0;

    if(temporalCollection)
    {
      currGrid = gridToWrite->GetChild(i);
      currGrid->Update();
    }

    for(int j=0; j<numberAttributes; ++j)
    {
      XdmfInt32 attributeCenter;
      std::vector<std::vector<double> > nodalArrays;

      for(unsigned int iNumBlocks=0; iNumBlocks<num_elem_blk; ++iNumBlocks)
      {
        XdmfAttribute * currAttribute;
        XdmfAttribute * globalNodeIds = NULL;
        if(spatialCollection)
        {
          currAttribute = currGrid->GetChild(iNumBlocks)->GetAttribute(j);
          globalNodeIds = globalNodeIdsArray[iNumBlocks];
          globalNodeIds->Update();
        }
        else
        {
          currAttribute = currGrid->GetAttribute(j);
        }
        currAttribute->Update();
        attributeCenter = currAttribute->GetAttributeCenter();
        switch(currAttribute->GetAttributeCenter())
        {
          case(XDMF_ATTRIBUTE_CENTER_GRID):
          {
            for(int k=0; k<globalComponents[globalComponentIndex]; ++k)
            {
              currAttribute->GetValues()->GetValues(k, &globalAttributeVals[globalIndex], 1);
              globalIndex++;
            }
            globalComponentIndex++;
            break;
          }
          case(XDMF_ATTRIBUTE_CENTER_NODE):
          {
            int numComponents = nodalComponents[nodalComponentIndex];
            for(int k=0; k<numComponents; ++k)
            {
              if(nodalArrays.size() == 0)
              {
                nodalArrays.resize(numComponents);
              }
              if(iNumBlocks == 0)
              {
                nodalArrays[k].resize(num_nodes);
              }
              if(spatialCollection)
              {
                int numVals = currAttribute->GetValues()->GetNumberOfElements() / numComponents;
                int index = k;
                for(int l=0; l<numVals; ++l, index+=numComponents)
                {
                  nodalArrays[k][globalNodeIds->GetValues()->GetValueAsInt32(l)] = currAttribute->GetValues()->GetValueAsFloat64(index);
                }
              }
              else
              {
                currAttribute->GetValues()->GetValues(k, &nodalArrays[k][0], currAttribute->GetValues()->GetNumberOfElements() / numComponents, numComponents);
              }
            }
            break;
          }
          case(XDMF_ATTRIBUTE_CENTER_CELL):
          {
            int numElemInBlock;
            if(spatialCollection)
            {
              numElemInBlock = currGrid->GetChild(iNumBlocks)->GetTopology()->GetNumberOfElements();
            }
            else
            {
              numElemInBlock = currGrid->GetTopology()->GetNumberOfElements();
            }
            for(int k=0; k<elementComponents[elementComponentIndex]; ++k)
            {
              double * elementValues = new double[numElemInBlock];
              currAttribute->GetValues()->GetValues(k, elementValues, numElemInBlock, elementComponents[elementComponentIndex]);
              ex_put_elem_var(exodusHandle, i+1, elementIndex+k+1, 10 + iNumBlocks, numElemInBlock, elementValues);
              ex_update(exodusHandle);
              delete [] elementValues;
            }
            break;
          }
        }
        if(globalNodeIds)
        {
          globalNodeIds->Release();
        }
        currAttribute->Release();
      }
      switch(attributeCenter)
      {
        case(XDMF_ATTRIBUTE_CENTER_CELL):
        {
          elementIndex+=elementComponents[elementComponentIndex];
          elementComponentIndex++;
          break;
        }
        case(XDMF_ATTRIBUTE_CENTER_NODE):
        {
          for(int k=0; k<nodalComponents[nodalComponentIndex]; ++k)
          {
            ex_put_nodal_var(exodusHandle, i+1, nodalIndex+1, nodalArrays[k].size(), &nodalArrays[k][0]);
            ex_update(exodusHandle);
            nodalArrays[k].clear();
            nodalIndex++;
          }
          nodalComponentIndex++;
          break;
        }
      }
      nodalArrays.clear();
    }
    ex_put_glob_vars(exodusHandle, i+1, numGlobalAttributes, globalAttributeVals);
    ex_update(exodusHandle);
    delete [] globalAttributeVals;
  }

  // Write Sets
  int setId = 20;

  // Set name to set of ids.
  if(spatialCollection)
  {
    std::map<std::string, std::set<int> > allSets;
    std::map<std::string, XdmfInt32> allSetsType;
    for(unsigned int iNumBlocks=0; iNumBlocks<num_elem_blk; ++iNumBlocks)
    {
      XdmfGrid * grid = currGrid->GetChild(iNumBlocks);
      XdmfAttribute * globalNodeIds = globalNodeIdsArray[iNumBlocks];
      globalNodeIds->Update();
      for(unsigned int i=0; i<grid->GetNumberOfSets(); ++i)
      {
        XdmfSet * currSet = grid->GetSets(i);
        currSet->Update();
        if(allSets.find(currSet->GetName()) == allSets.end())
        {
          allSets[currSet->GetName()] = std::set<int>();
          allSetsType[currSet->GetName()] = currSet->GetSetType();
        }
        std::set<int> & setToAddTo = allSets[currSet->GetName()];
        for(int j=0; j<currSet->GetIds()->GetNumberOfElements(); ++j)
        {
          setToAddTo.insert(globalNodeIds->GetValues()->GetValueAsInt32(currSet->GetIds()->GetValueAsInt32(j)) + 1);
        }
        currSet->Release();
      }
      globalNodeIds->Release();
    }

    for(std::map<std::string, std::set<int> >::const_iterator iter = allSets.begin(); iter != allSets.end(); ++iter)
    {
      const std::string & currSetName = iter->first;
      const std::set<int> & currSet = iter->second;
      XdmfInt32 currSetType = allSetsType[currSetName];
      std::vector<int> setValues;
      setValues.resize(currSet.size());
      int index = 0;
      for(std::set<int>::const_iterator setIter = currSet.begin(); setIter != currSet.end(); ++setIter)
      {
        setValues[index] = *setIter;
        ++index;
      }
      std::string name = currSetName;
      if(name.length() > MAX_STR_LENGTH)
      {
        name = name.substr(0, MAX_STR_LENGTH);
      }
      switch(currSetType)
      {
        case(XDMF_SET_TYPE_CELL):
        {
          ex_put_side_set_param(exodusHandle, setId + 1, setValues.size(), 0);
          ex_put_side_set(exodusHandle, setId + 1, &setValues[0], NULL);
          ex_put_name(exodusHandle, EX_SIDE_SET, setId + 1, name.c_str());
          break;
        }
        case(XDMF_SET_TYPE_NODE):
        {
          ex_put_node_set_param(exodusHandle, setId + 1, setValues.size(), 0);
          ex_put_node_set(exodusHandle, setId + 1, &setValues[0]);
          ex_put_name(exodusHandle, EX_NODE_SET, setId + 1, name.c_str());
          break;
        }
      }
      ++setId;
    }
  }
  else
  {
    int setId = 20;
    for (int i=0; i<currGrid->GetNumberOfSets(); ++i)
    {
      XdmfSet * currSet = currGrid->GetSets(i);
      currSet->Update();
      int numValues = currSet->GetIds()->GetNumberOfElements();
      std::string name = currSet->GetName();
      if(name.length() > MAX_STR_LENGTH)
      {
        name = name.substr(0, MAX_STR_LENGTH);
      }
      switch(currSet->GetSetType())
      {
        case(XDMF_SET_TYPE_CELL):
        {
          ex_put_side_set_param(exodusHandle, setId + i, numValues, 0);
          int * values = new int[numValues];
          // Add 1 to xdmf ids because exodus ids begin at 1
          *currSet->GetIds() + 1;
          currSet->GetIds()->GetValues(0, values, numValues);
          ex_put_side_set(exodusHandle, setId + i, values, NULL);
          ex_put_name(exodusHandle, EX_SIDE_SET, setId + i, name.c_str());
          delete [] values;
          break;
        }
        case(XDMF_SET_TYPE_NODE):
        {
          ex_put_node_set_param(exodusHandle, setId + i, numValues, 0);
          int * values = new int[numValues];
          // Add 1 to xdmf ids because exodus ids begin at 1
          *currSet->GetIds() + 1;
          currSet->GetIds()->GetValues(0, values, numValues);
          ex_put_node_set(exodusHandle, setId + i, values);
          ex_put_name(exodusHandle, EX_NODE_SET, setId + i, name.c_str());
          delete [] values;
          break;
        }
      }
    }
  }
  
  delete [] globalNodeIdsArray;

  // Close Exodus File
  ex_close(exodusHandle);

}
