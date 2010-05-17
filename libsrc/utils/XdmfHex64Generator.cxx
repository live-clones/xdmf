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

#include "XdmfHex64Generator.h"

#ifndef BUILD_EXE

#include <math.h>
#include <vector>

XdmfHex64Generator::XdmfHex64Generator()
{
}

XdmfHex64Generator::~XdmfHex64Generator()
{
}

inline int compare(XdmfFloat64 * pt1, XdmfFloat64 * pt2)
{
  if (fabs(pt1[0] - pt2[0]) <= 1e-6)
  {
    if (fabs(pt1[1] - pt2[1]) <= 1e-6)
    {
      if (fabs(pt1[2] - pt2[2]) <= 1e-6)
      {
        return 1;
      }
    }
  }
  return 0;
}

inline void XdmfHex64Generator::ComputeInteriorPoints(std::vector<XdmfFloat64> & midpoint1, std::vector<XdmfFloat64> & midpoint2, std::vector<XdmfFloat64> & point1, std::vector<XdmfFloat64> & point2)
{
  midpoint1[0] = (1.0/3.0)*(point2[0] + 2*point1[0]);
  midpoint1[1] = (1.0/3.0)*(point2[1] + 2*point1[1]);
  midpoint1[2] = (1.0/3.0)*(point2[2] + 2*point1[2]);
    
  midpoint2[0] = (1.0/3.0)*(2*point2[0] + point1[0]);
  midpoint2[1] = (1.0/3.0)*(2*point2[1] + point1[1]);
  midpoint2[2] = (1.0/3.0)*(2*point2[2] + point1[2]);
}

inline void XdmfHex64Generator::ComputeOneThirdPoint(std::vector<XdmfFloat64> & midpoint1, std::vector<XdmfFloat64> & point1, std::vector<XdmfFloat64> & point2)
{
  midpoint1[0] = (1.0/3.0)*(point2[0] + 2*point1[0]);
  midpoint1[1] = (1.0/3.0)*(point2[1] + 2*point1[1]);
  midpoint1[2] = (1.0/3.0)*(point2[2] + 2*point1[2]);
}

inline void XdmfHex64Generator::ComputeTwoThirdPoint(std::vector<XdmfFloat64> & midpoint1, std::vector<XdmfFloat64> & point1, std::vector<XdmfFloat64> & point2)
{
  midpoint1[0] = (1.0/3.0)*(2*point2[0] + point1[0]);
  midpoint1[1] = (1.0/3.0)*(2*point2[1] + point1[1]);
  midpoint1[2] = (1.0/3.0)*(2*point2[2] + point1[2]);
}

XdmfGrid * XdmfHex64Generator::Generate(XdmfGrid * grid)
{
  if(grid->GetTopology()->GetTopologyType() != XDMF_HEX)
  {
    std::cout << "This requires a Linear Hexahedron Grid" << std::endl;
    return NULL;
  }

  if(grid->GetGeometry()->GetGeometryType() != XDMF_GEOMETRY_XYZ)
  {
    std::cout << "This requires XYZ Geometry" << std::endl;
    return NULL;
  }
  
  // Copy all coordinate values into new geometry.
  std::vector<XdmfFloat64> newPoints(grid->GetGeometry()->GetPoints()->GetNumberOfElements());
  grid->GetGeometry()->GetPoints()->GetValues(0, &newPoints[0], grid->GetGeometry()->GetPoints()->GetNumberOfElements());

  std::vector<XdmfInt32> newConnectivity;
  std::vector<XdmfFloat64> midpoint1(3);
  std::vector<XdmfFloat64> midpoint2(3);

  // Iterate over all elements, for each element compute new coordinate points and insert those values into newGeometry and newConnectivity.
  for(int i=0; i<grid->GetTopology()->GetNumberOfElements(); ++i)
  {
    // Fill localNodes with original coordinate information.
    std::vector<std::vector<XdmfFloat64> > localNodes(8);
    for(int j=0; j<8; ++j)
    {
      localNodes[j] = std::vector<XdmfFloat64>(3);
      grid->GetGeometry()->GetPoints()->GetValues(grid->GetTopology()->GetConnectivity()->GetValueAsInt64(8*i + j) * 3, &localNodes[j][0], 3);
    }

    // Add old connectivity information to newConnectivity.
    int startIndex = newConnectivity.size();
    newConnectivity.resize(newConnectivity.size() + 8);
    grid->GetTopology()->GetConnectivity()->GetValues(8*i, &newConnectivity[startIndex], 8);
    
    for(int j=0; j<32; ++j)
    {
      switch(j)
      {
        case 0:
        {
          ComputeInteriorPoints(midpoint1, midpoint2, localNodes[0], localNodes[1]);
          localNodes.push_back(midpoint1);
          localNodes.push_back(midpoint2);
          break;
        }
        case 1:
        {
          ComputeInteriorPoints(midpoint1, midpoint2, localNodes[1], localNodes[2]);
          localNodes.push_back(midpoint1);
          localNodes.push_back(midpoint2);
          break;
        }
        case 2:
        {
          ComputeInteriorPoints(midpoint1, midpoint2, localNodes[2], localNodes[3]);
          localNodes.push_back(midpoint1);
          localNodes.push_back(midpoint2);
          break;
        }
        case 3:
        {
          ComputeInteriorPoints(midpoint1, midpoint2, localNodes[3], localNodes[0]);
          localNodes.push_back(midpoint1);
          localNodes.push_back(midpoint2);
          break;
        }
        case 4:
        {
          ComputeInteriorPoints(midpoint1, midpoint2, localNodes[4], localNodes[5]);
          localNodes.push_back(midpoint1);
          localNodes.push_back(midpoint2);
          break;
        }
        case 5:
        {
          ComputeInteriorPoints(midpoint1, midpoint2, localNodes[5], localNodes[6]);
          localNodes.push_back(midpoint1);
          localNodes.push_back(midpoint2);
          break;
        }
        case 6:
        {
          ComputeInteriorPoints(midpoint1, midpoint2, localNodes[6], localNodes[7]);
          localNodes.push_back(midpoint1);
          localNodes.push_back(midpoint2);
          break;
        }
        case 7:
        {
          ComputeInteriorPoints(midpoint1, midpoint2, localNodes[7], localNodes[4]);
          localNodes.push_back(midpoint1);
          localNodes.push_back(midpoint2);
          break;
        }
        case 8:
        {
          ComputeOneThirdPoint(midpoint1, localNodes[0], localNodes[4]);
          localNodes.push_back(midpoint1);
          break;
        }
        case 9:
        {
          ComputeOneThirdPoint(midpoint1, localNodes[1], localNodes[5]);
          localNodes.push_back(midpoint1);
          break;
        }
        case 10:
        {
          ComputeOneThirdPoint(midpoint1, localNodes[2], localNodes[6]);
          localNodes.push_back(midpoint1);
          break;
        }
        case 11:
        {
          ComputeOneThirdPoint(midpoint1, localNodes[3], localNodes[7]);
          localNodes.push_back(midpoint1);
          break;
        }
        case 12:
        {
          ComputeTwoThirdPoint(midpoint1, localNodes[0], localNodes[4]);
          localNodes.push_back(midpoint1);
          break;
        }
        case 13:
        {
          ComputeTwoThirdPoint(midpoint1, localNodes[1], localNodes[5]);
          localNodes.push_back(midpoint1);
          break;
        }
        case 14:
        {
          ComputeTwoThirdPoint(midpoint1, localNodes[2], localNodes[6]);
          localNodes.push_back(midpoint1);
          break;
        }
        case 15:
        {
          ComputeTwoThirdPoint(midpoint1, localNodes[3], localNodes[7]);
          localNodes.push_back(midpoint1);
          break;
        }
        case 16:
        {
          ComputeInteriorPoints(midpoint1, midpoint2, localNodes[27], localNodes[24]);
          localNodes.push_back(midpoint1);
          localNodes.push_back(midpoint2);
          break;
        }
        case 17:
        {
          ComputeInteriorPoints(midpoint1, midpoint2, localNodes[25], localNodes[26]);
          localNodes.push_back(midpoint1);
          localNodes.push_back(midpoint2);
          break;
        }
        case 18:
        {
          ComputeInteriorPoints(midpoint1, midpoint2, localNodes[24], localNodes[25]);
          localNodes.push_back(midpoint1);
          localNodes.push_back(midpoint2);
          break;
        }
        case 19:
        {
          ComputeInteriorPoints(midpoint1, midpoint2, localNodes[26], localNodes[27]);
          localNodes.push_back(midpoint1);
          localNodes.push_back(midpoint2);
          break;
        }
        case 20:
        {
          ComputeInteriorPoints(midpoint1, midpoint2, localNodes[31], localNodes[28]);
          localNodes.push_back(midpoint1);
          localNodes.push_back(midpoint2);
          break;
        }
        case 21:
        {
          ComputeInteriorPoints(midpoint1, midpoint2, localNodes[29], localNodes[30]);
          localNodes.push_back(midpoint1);
          localNodes.push_back(midpoint2);
          break;
        }
        case 22:
        {
          ComputeInteriorPoints(midpoint1, midpoint2, localNodes[28], localNodes[29]);
          localNodes.push_back(midpoint1);
          localNodes.push_back(midpoint2);
          break;
        }
        case 23:
        {
          ComputeInteriorPoints(midpoint1, midpoint2, localNodes[30], localNodes[31]);
          localNodes.push_back(midpoint1);
          localNodes.push_back(midpoint2);
          break;
        }
        case 24:
        {
          ComputeInteriorPoints(midpoint1, midpoint2, localNodes[15], localNodes[10]);
          localNodes.push_back(midpoint1);
          localNodes.push_back(midpoint2);
          break;
        }
        case 25:
        {
          ComputeInteriorPoints(midpoint1, midpoint2, localNodes[11], localNodes[14]);
          localNodes.push_back(midpoint1);
          localNodes.push_back(midpoint2);
          break;
        }
        case 26:
        {
          ComputeInteriorPoints(midpoint1, midpoint2, localNodes[23], localNodes[18]);
          localNodes.push_back(midpoint1);
          localNodes.push_back(midpoint2);
          break;
        }
        case 27:
        {
          ComputeInteriorPoints(midpoint1, midpoint2, localNodes[19], localNodes[22]);
          localNodes.push_back(midpoint1);
          localNodes.push_back(midpoint2);
          break;
        }
        case 28:
        {
          ComputeInteriorPoints(midpoint1, midpoint2, localNodes[33], localNodes[34]);
          localNodes.push_back(midpoint1);
          localNodes.push_back(midpoint2);
          break;
        }
        case 29:
        {
          ComputeInteriorPoints(midpoint1, midpoint2, localNodes[35], localNodes[32]);
          localNodes.push_back(midpoint1);
          localNodes.push_back(midpoint2);
          break;
        }
        case 30:
        {
          ComputeInteriorPoints(midpoint1, midpoint2, localNodes[41], localNodes[42]);
          localNodes.push_back(midpoint1);
          localNodes.push_back(midpoint2);
          break;
        }
        case 31:
        {
          ComputeInteriorPoints(midpoint1, midpoint2, localNodes[43], localNodes[40]);
          localNodes.push_back(midpoint1);
          localNodes.push_back(midpoint2);
          break;
        }
      }

      // Check to see if generated points already exists
      int pointExists = 0;
      int nid;
      for(int k= grid->GetGeometry()->GetNumberOfPoints(); k < newPoints.size() / 3; ++k)
      {
        if(compare(&newPoints[k*3], &midpoint1[0]) == 1)
        {
          pointExists = 1;
          nid = k;
          break;
        }
      }

      // If pointExists just push that id to newConnectivity array, else we need to add a new point.
      if(pointExists)
      {
        newConnectivity.push_back(nid);
      }
      else
      {
        newConnectivity.push_back(newPoints.size() / 3);
        newPoints.push_back(midpoint1[0]);
        newPoints.push_back(midpoint1[1]);
        newPoints.push_back(midpoint1[2]);
      }

      if(j != 8 && j != 9 && j != 10 && j != 11 && j != 12 && j != 13 && j != 14 && j != 15)
      {
        pointExists = 0;
        for (int k = grid->GetGeometry()->GetNumberOfPoints(); k < newPoints.size() / 3; ++k)
        {
          if (compare(&newPoints[k*3], &midpoint2[0]) == 1)
          {
            pointExists = 1;
            nid = k;
            break;
          }
        }

        if (pointExists)
        {
          newConnectivity.push_back(nid);
        }
        else
        {
          newConnectivity.push_back(newPoints.size() / 3);
          newPoints.push_back(midpoint2[0]);
          newPoints.push_back(midpoint2[1]);
          newPoints.push_back(midpoint2[2]);
        }
      }
    }
  }

  XdmfGrid * newGrid = new XdmfGrid();
  newGrid->SetDeleteOnGridDelete(true);
  newGrid->SetName(grid->GetName());

  newGrid->GetGeometry()->SetGeometryType(grid->GetGeometry()->GetGeometryType());
  newGrid->GetGeometry()->SetNumberOfPoints(newPoints.size() / 3);
  newGrid->GetGeometry()->SetDeleteOnGridDelete(true);
  newGrid->GetGeometry()->GetPoints()->SetNumberType(grid->GetGeometry()->GetPoints()->GetNumberType());
  newGrid->GetGeometry()->GetPoints()->SetNumberOfElements(newPoints.size());
  newGrid->GetGeometry()->GetPoints()->SetValues(0, &newPoints[0], newPoints.size());

  newGrid->GetTopology()->SetTopologyType(XDMF_HEX_64);
  newGrid->GetTopology()->SetNumberOfElements(grid->GetTopology()->GetNumberOfElements());
  newGrid->GetTopology()->SetDeleteOnGridDelete(true);
  newGrid->GetTopology()->GetConnectivity()->SetNumberType(grid->GetTopology()->GetConnectivity()->GetNumberType());
  newGrid->GetTopology()->GetConnectivity()->SetNumberOfElements(newConnectivity.size());
  newGrid->GetTopology()->GetConnectivity()->SetValues(0, &newConnectivity[0], newConnectivity.size());

  return newGrid;
}

#else

#include <sstream>

/**
 * XdmfHex64Generator is a command line utility for generating Tri-Cubic Hexahedron Xdmf grids from Linear
 * Hexahedron Grids.
 *
 * Usage:
 *     XdmfHex64Generator <path-of-file-containing-linear-hexahedron> (Optional: <path-to-output-file>)
 *
 */
int main(int argc, char* argv[])
{
  std::string usage = "Generates Tri-Cubic Hexahedron Xdmf grids from Linear Hexahedron Grids: \n \n Usage: \n \n   XdmfHex64Generator <path-of-file-containing-linear-hexahedron> (Optional: <path-to-output-file>)";
  std::string meshName = "";

  if (argc < 2)
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

  if (argc >= 3)
  {
    meshName = argv[2];
  }

  if(meshName.find_last_of("/\\") != std::string::npos)
  {
    meshName = meshName.substr(meshName.find_last_of("/\\")+1, meshName.length());
  }

  if (meshName.rfind(".") != std::string::npos)
  {
    meshName = meshName.substr(0, meshName.rfind("."));
  }

  if(argc < 3)
  {
    meshName = meshName + "-hex64";
  }

  XdmfDOM dom;
  XdmfInt32 error = dom.Parse(argv[1]);
  
  XdmfGrid * grid = new XdmfGrid();
  grid->SetDOM(&dom);
  grid->SetElement(dom.FindElementByPath("/Xdmf/Domain/Grid"));
  grid->Update();
  
  XdmfDOM newDOM;
  XdmfRoot newRoot;
  XdmfDomain newDomain;
    
  newRoot.SetDOM(&newDOM);
  newRoot.Build();
  newRoot.Insert(&newDomain);
  
  XdmfHex64Generator generator;
  XdmfGrid * mesh = generator.Generate(grid);
  delete grid;

  // Set heavy data set names for geometry and topology
  std::stringstream heavyPointName;
  heavyPointName << meshName << ".h5:/" << meshName << "/XYZ";
  mesh->GetGeometry()->GetPoints()->SetHeavyDataSetName(heavyPointName.str().c_str());

  std::stringstream heavyConnName;
  heavyConnName << meshName << ".h5:/" << meshName << "/Connections";
  mesh->GetTopology()->GetConnectivity()->SetHeavyDataSetName(heavyConnName.str().c_str());

  newDomain.Insert(mesh);
  mesh->Build();

  std::stringstream outputFileName;
  outputFileName << meshName << ".xmf";

  newDOM.Write(outputFileName.str().c_str());

  delete mesh;
  std::cout << "Wrote: " << outputFileName.str().c_str() << std::endl;
}

#endif //BUILD_EXE
