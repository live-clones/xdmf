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

#include "XdmfArray.h"
#include "XdmfAttribute.h"
#include "XdmfDomain.h"
#include "XdmfHex125Generator.h"
#include "XdmfGeometry.h"
#include "XdmfGrid.h"
#include "XdmfSet.h"
#include "XdmfTopology.h"

#include <math.h>
#include <map>
#include <vector>

XdmfHex125Generator::XdmfHex125Generator()
{
}

XdmfHex125Generator::~XdmfHex125Generator()
{
}

struct PointComparison {
  bool operator()(const std::vector<XdmfFloat64> & point1, const std::vector<XdmfFloat64> & point2) const
  {
    double epsilon = 1e-6;
    for(int i=0; i<3; ++i)
    {
      if(fabs(point1[i] - point2[i]) > epsilon)
      {
        return point1[i] < point2[i];
      }
    }
    return false;
  }
};

inline void ComputeInteriorPoints(std::vector<XdmfFloat64> & quarterPoint, std::vector<XdmfFloat64> & midPoint, std::vector<XdmfFloat64> & threeQuarterPoint, const std::vector<XdmfFloat64> & point1,
  const std::vector<XdmfFloat64> & point2, bool & quarter_point_creation, bool & mid_point_creation, bool & three_quarter_point_creation)
{
  quarter_point_creation = true;
  mid_point_creation = true;
  three_quarter_point_creation = true;

  quarterPoint[0] = (1.0/4.0)*(point2[0] + 3*point1[0]);
  quarterPoint[1] = (1.0/4.0)*(point2[1] + 3*point1[1]);
  quarterPoint[2] = (1.0/4.0)*(point2[2] + 3*point1[2]);

  midPoint[0] = (1.0/2.0)*(point2[0] + point1[0]);
  midPoint[1] = (1.0/2.0)*(point2[1] + point1[1]);
  midPoint[2] = (1.0/2.0)*(point2[2] + point1[2]);

  threeQuarterPoint[0] = (1.0/4.0)*(3.0*point2[0] + point1[0]);
  threeQuarterPoint[1] = (1.0/4.0)*(3.0*point2[1] + point1[1]);
  threeQuarterPoint[2] = (1.0/4.0)*(3.0*point2[2] + point1[2]);
}

inline void ComputeQuarterPoint (std::vector<XdmfFloat64> & quarterPoint, const std::vector<XdmfFloat64> & point1, const std::vector<XdmfFloat64> & point2, bool & quarter_point_creation, bool & mid_point_creation, 
  bool & three_quarter_point_creation)
{
    quarter_point_creation = true;
    mid_point_creation = false;
    three_quarter_point_creation = false;

    quarterPoint[0] = (1.0/4.0)*(point2[0] + 3*point1[0]);
    quarterPoint[1] = (1.0/4.0)*(point2[1] + 3*point1[1]);
    quarterPoint[2] = (1.0/4.0)*(point2[2] + 3*point1[2]);
}

inline void ComputeMidPoint(std::vector<XdmfFloat64> & midPoint, const std::vector<XdmfFloat64> & point1, const std::vector<XdmfFloat64> & point2, bool & quarter_point_creation, bool & mid_point_creation,
  bool & three_quarter_point_creation)
{
  quarter_point_creation = false;
  mid_point_creation = true;
  three_quarter_point_creation = false;

  midPoint[0] = (1.0/2.0)*(point2[0] + point1[0]);
  midPoint[1] = (1.0/2.0)*(point2[1] + point1[1]);
  midPoint[2] = (1.0/2.0)*(point2[2] + point1[2]);
}

inline void ComputeThreeQuarterPoint(std::vector<XdmfFloat64> & threeQuarterPoint, const std::vector<XdmfFloat64> & point1, const std::vector<XdmfFloat64> & point2, bool & quarter_point_creation,
  bool & mid_point_creation, bool & three_quarter_point_creation)
{
     quarter_point_creation = false;
     mid_point_creation = false;
     three_quarter_point_creation = true;

     threeQuarterPoint[0] = (1.0/4.0)*(3.0*point2[0] + point1[0]);
     threeQuarterPoint[1] = (1.0/4.0)*(3.0*point2[1] + point1[1]);
     threeQuarterPoint[2] = (1.0/4.0)*(3.0*point2[2] + point1[2]);
}

XdmfGrid * XdmfHex125Generator::Generate(XdmfGrid * grid, XdmfElement * parentElement)
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
  std::vector<XdmfFloat64> quarterPoint(3);
  std::vector<XdmfFloat64> midPoint(3);
  std::vector<XdmfFloat64> threeQuarterPoint(3);
  bool quarter_point_creation;
  bool mid_point_creation;
  bool three_quarter_point_creation;

  std::map<std::vector<XdmfFloat64>, XdmfInt32, PointComparison> coordToIdMap;

  // Iterate over all elements, for each element compute new coordinate points and insert those values into newGeometry and newConnectivity.
  for(int i=0; i<grid->GetTopology()->GetNumberOfElements(); ++i)
  {
    std::cout << "Converting Element " << i << std::endl;

    // Fill localNodes with original coordinate information.
    std::vector<std::vector<XdmfFloat64> > localNodes(8);
    localNodes.reserve(125);
    for(int j=0; j<8; ++j)
    {
      localNodes[j] = std::vector<XdmfFloat64>(3);
      grid->GetGeometry()->GetPoints()->GetValues(grid->GetTopology()->GetConnectivity()->GetValueAsInt64(8*i + j) * 3, &localNodes[j][0], 3);
    }

    // Add old connectivity information to newConnectivity.
    int startIndex = newConnectivity.size();
    newConnectivity.resize(newConnectivity.size() + 8);
    grid->GetTopology()->GetConnectivity()->GetValues(8*i, &newConnectivity[startIndex], 8);
    
    for(int j=0; j<57; ++j)
    {
      switch(j)
      {
        case 0:
        {
          ComputeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[0], localNodes[1], quarter_point_creation, mid_point_creation, three_quarter_point_creation);
          localNodes.push_back(quarterPoint);
          localNodes.push_back(midPoint);
          localNodes.push_back(threeQuarterPoint);
          break;
        }
        case 1:
        {
          ComputeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[1], localNodes[2], quarter_point_creation, mid_point_creation, three_quarter_point_creation);
          localNodes.push_back(quarterPoint);
          localNodes.push_back(midPoint);
          localNodes.push_back(threeQuarterPoint);
          break;
        }
        case 2:
        {
          ComputeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[2], localNodes[3], quarter_point_creation, mid_point_creation, three_quarter_point_creation);
          localNodes.push_back(quarterPoint);
          localNodes.push_back(midPoint);
          localNodes.push_back(threeQuarterPoint);
          break;
        }
        case 3:
        {
          ComputeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[3], localNodes[0], quarter_point_creation, mid_point_creation, three_quarter_point_creation);
          localNodes.push_back(quarterPoint);
          localNodes.push_back(midPoint);
          localNodes.push_back(threeQuarterPoint);
          break;
        }
        case 4:
        {
          ComputeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[4], localNodes[5], quarter_point_creation, mid_point_creation, three_quarter_point_creation);
          localNodes.push_back(quarterPoint);
          localNodes.push_back(midPoint);
          localNodes.push_back(threeQuarterPoint);
          break;
        }
        case 5:
        {
          ComputeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[5], localNodes[6], quarter_point_creation, mid_point_creation, three_quarter_point_creation);
          localNodes.push_back(quarterPoint);
          localNodes.push_back(midPoint);
          localNodes.push_back(threeQuarterPoint);
          break;
        }
        case 6:
        {
          ComputeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[6], localNodes[7], quarter_point_creation, mid_point_creation, three_quarter_point_creation);
          localNodes.push_back(quarterPoint);
          localNodes.push_back(midPoint);
          localNodes.push_back(threeQuarterPoint);
          break;
        }
        case 7:
        {
          ComputeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[7], localNodes[4], quarter_point_creation, mid_point_creation, three_quarter_point_creation);
          localNodes.push_back(quarterPoint);
          localNodes.push_back(midPoint);
          localNodes.push_back(threeQuarterPoint);
          break;
        }
        case 8:
        {
          ComputeQuarterPoint(quarterPoint, localNodes[0], localNodes[4], quarter_point_creation, mid_point_creation, three_quarter_point_creation);
          localNodes.push_back(quarterPoint);
          break;
        }
        case 9:
        {
          ComputeQuarterPoint(quarterPoint, localNodes[1], localNodes[5], quarter_point_creation, mid_point_creation, three_quarter_point_creation);
          localNodes.push_back(quarterPoint);
          break;
        }
        case 10:
        {
          ComputeQuarterPoint(quarterPoint, localNodes[2], localNodes[6], quarter_point_creation, mid_point_creation, three_quarter_point_creation);
          localNodes.push_back(quarterPoint);
          break;
        }
        case 11:
        {
          ComputeQuarterPoint(quarterPoint, localNodes[3], localNodes[7], quarter_point_creation, mid_point_creation, three_quarter_point_creation);
          localNodes.push_back(quarterPoint);
          break;
        }
        case 12:
        {
          ComputeThreeQuarterPoint(threeQuarterPoint, localNodes[0], localNodes[4], quarter_point_creation, mid_point_creation, three_quarter_point_creation);
          localNodes.push_back(threeQuarterPoint);
          break;
        }
        case 13:
        {
          ComputeThreeQuarterPoint(threeQuarterPoint, localNodes[1], localNodes[5], quarter_point_creation, mid_point_creation, three_quarter_point_creation);
          localNodes.push_back(threeQuarterPoint);
          break;
        }
        case 14:
        {
          ComputeThreeQuarterPoint(threeQuarterPoint, localNodes[2], localNodes[6], quarter_point_creation, mid_point_creation, three_quarter_point_creation);
          localNodes.push_back(threeQuarterPoint);
          break;
        }
        case 15:
        {
          ComputeThreeQuarterPoint(threeQuarterPoint, localNodes[3], localNodes[7], quarter_point_creation, mid_point_creation, three_quarter_point_creation);
          localNodes.push_back(threeQuarterPoint);
          break;
        }
        case 16:
        {
          ComputeMidPoint(midPoint, localNodes[0], localNodes[4], quarter_point_creation, mid_point_creation, three_quarter_point_creation);
          localNodes.push_back(midPoint);
          break;
        }
        case 17:
        {
          ComputeMidPoint(midPoint, localNodes[1], localNodes[5], quarter_point_creation, mid_point_creation, three_quarter_point_creation);
          localNodes.push_back(midPoint);
          break;
        }
        case 18:
        {
          ComputeMidPoint(midPoint, localNodes[2], localNodes[6], quarter_point_creation, mid_point_creation, three_quarter_point_creation);
          localNodes.push_back(midPoint);
          break;
        }
        case 19:
        {
          ComputeMidPoint(midPoint, localNodes[3], localNodes[7], quarter_point_creation, mid_point_creation, three_quarter_point_creation);
          localNodes.push_back(midPoint);
          break;
        }
        case 20:
        {
          ComputeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[32], localNodes[33], quarter_point_creation, mid_point_creation, three_quarter_point_creation);
          localNodes.push_back(quarterPoint);
          localNodes.push_back(midPoint);
          localNodes.push_back(threeQuarterPoint);
          break;
        }
        case 21:
        {
          ComputeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[33], localNodes[34], quarter_point_creation, mid_point_creation, three_quarter_point_creation);
          localNodes.push_back(quarterPoint);
          localNodes.push_back(midPoint);
          localNodes.push_back(threeQuarterPoint);
          break;
        }
        case 22:
        {
          ComputeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[34], localNodes[35], quarter_point_creation, mid_point_creation, three_quarter_point_creation);
          localNodes.push_back(quarterPoint);
          localNodes.push_back(midPoint);
          localNodes.push_back(threeQuarterPoint);
          break;
        }
        case 23:
        {
          ComputeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[35], localNodes[32], quarter_point_creation, mid_point_creation, three_quarter_point_creation);
          localNodes.push_back(quarterPoint);
          localNodes.push_back(midPoint);
          localNodes.push_back(threeQuarterPoint);
          break;
        }
        case 24:
        {
          ComputeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[36], localNodes[37], quarter_point_creation, mid_point_creation, three_quarter_point_creation);
          localNodes.push_back(quarterPoint);
          localNodes.push_back(midPoint);
          localNodes.push_back(threeQuarterPoint);
          break;
        }
        case 25:
        {
          ComputeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[37], localNodes[38], quarter_point_creation, mid_point_creation, three_quarter_point_creation);
          localNodes.push_back(quarterPoint);
          localNodes.push_back(midPoint);
          localNodes.push_back(threeQuarterPoint);
          break;
        }
        case 26:
        {
          ComputeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[38], localNodes[39], quarter_point_creation, mid_point_creation, three_quarter_point_creation);
          localNodes.push_back(quarterPoint);
          localNodes.push_back(midPoint);
          localNodes.push_back(threeQuarterPoint);
          break;
        }
        case 27:
        {
          ComputeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[39], localNodes[36], quarter_point_creation, mid_point_creation, three_quarter_point_creation);
          localNodes.push_back(quarterPoint);
          localNodes.push_back(midPoint);
          localNodes.push_back(threeQuarterPoint);
          break;
        }
        case 28:
        {
          ComputeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[40], localNodes[41], quarter_point_creation, mid_point_creation, three_quarter_point_creation);
          localNodes.push_back(quarterPoint);
          localNodes.push_back(midPoint);
          localNodes.push_back(threeQuarterPoint);
          break;
        }
        case 29:
        {
          ComputeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[41], localNodes[42], quarter_point_creation, mid_point_creation, three_quarter_point_creation);
          localNodes.push_back(quarterPoint);
          localNodes.push_back(midPoint);
          localNodes.push_back(threeQuarterPoint);
          break;
        }
        case 30:
        {
          ComputeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[42], localNodes[43], quarter_point_creation, mid_point_creation, three_quarter_point_creation);
          localNodes.push_back(quarterPoint);
          localNodes.push_back(midPoint);
          localNodes.push_back(threeQuarterPoint);
          break;
        }
        case 31:
        {
          ComputeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[43], localNodes[40], quarter_point_creation, mid_point_creation, three_quarter_point_creation);
          localNodes.push_back(quarterPoint);
          localNodes.push_back(midPoint);
          localNodes.push_back(threeQuarterPoint);
          break;
        }
        case 32:
        {
          ComputeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[19], localNodes[11], quarter_point_creation, mid_point_creation, three_quarter_point_creation);
          localNodes.push_back(quarterPoint);
          localNodes.push_back(midPoint);
          localNodes.push_back(threeQuarterPoint);
          break;
        }
        case 33:
        {
          ComputeMidPoint(midPoint, localNodes[10], localNodes[14], quarter_point_creation, mid_point_creation, three_quarter_point_creation);
          localNodes.push_back(midPoint);
          break;
        }
        case 34:
        {
          ComputeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[13], localNodes[17], quarter_point_creation, mid_point_creation, three_quarter_point_creation);
          localNodes.push_back(quarterPoint);
          localNodes.push_back(midPoint);
          localNodes.push_back(threeQuarterPoint);
          break;
        }
        case 35:
        {
          ComputeMidPoint(midPoint, localNodes[16], localNodes[8], quarter_point_creation, mid_point_creation, three_quarter_point_creation);
          localNodes.push_back(midPoint);
          break;
        }
        case 36:
        {
          ComputeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[31], localNodes[23], quarter_point_creation, mid_point_creation, three_quarter_point_creation);
          localNodes.push_back(quarterPoint);
          localNodes.push_back(midPoint);
          localNodes.push_back(threeQuarterPoint);
          break;
        }
        case 37:
        {
          ComputeMidPoint(midPoint, localNodes[22], localNodes[26], quarter_point_creation, mid_point_creation, three_quarter_point_creation);
          localNodes.push_back(midPoint);
          break;
        }
        case 38:
        {
          ComputeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[25], localNodes[29], quarter_point_creation, mid_point_creation, three_quarter_point_creation);
          localNodes.push_back(quarterPoint);
          localNodes.push_back(midPoint);
          localNodes.push_back(threeQuarterPoint);
          break;
        }
        case 39:
        {
          ComputeMidPoint(midPoint, localNodes[28], localNodes[20], quarter_point_creation, mid_point_creation, three_quarter_point_creation);
          localNodes.push_back(midPoint);
          break;
        }
        case 40:
        {
          ComputeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[55], localNodes[47], quarter_point_creation, mid_point_creation, three_quarter_point_creation);
          localNodes.push_back(quarterPoint);
          localNodes.push_back(midPoint);
          localNodes.push_back(threeQuarterPoint);
          break;
        }
        case 41:
        {
          ComputeMidPoint(midPoint, localNodes[46], localNodes[50], quarter_point_creation, mid_point_creation, three_quarter_point_creation);
          localNodes.push_back(midPoint);
          break;
        }
        case 42:
        {
          ComputeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[49], localNodes[53], quarter_point_creation, mid_point_creation, three_quarter_point_creation);
          localNodes.push_back(quarterPoint);
          localNodes.push_back(midPoint);
          localNodes.push_back(threeQuarterPoint);
          break;
        }
        case 43:
        {
          ComputeMidPoint(midPoint, localNodes[52], localNodes[44], quarter_point_creation, mid_point_creation, three_quarter_point_creation);
          localNodes.push_back(midPoint);
          break;
        }
        case 44:
        {
          ComputeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[67], localNodes[59], quarter_point_creation, mid_point_creation, three_quarter_point_creation);
          localNodes.push_back(quarterPoint);
          localNodes.push_back(midPoint);
          localNodes.push_back(threeQuarterPoint);
          break;
        }
        case 45:
        {
          ComputeMidPoint(midPoint, localNodes[62], localNodes[58], quarter_point_creation, mid_point_creation, three_quarter_point_creation);
          localNodes.push_back(midPoint);
          break;
        }
        case 46:
        {
          ComputeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[61], localNodes[65], quarter_point_creation, mid_point_creation, three_quarter_point_creation);
          localNodes.push_back(quarterPoint);
          localNodes.push_back(midPoint);
          localNodes.push_back(threeQuarterPoint);
          break;
        }
        case 47:
        {
          ComputeMidPoint(midPoint, localNodes[56], localNodes[64], quarter_point_creation, mid_point_creation, three_quarter_point_creation);
          localNodes.push_back(midPoint);
          break;
        }
        case 48:
        {
          ComputeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[79], localNodes[71], quarter_point_creation, mid_point_creation, three_quarter_point_creation);
          localNodes.push_back(quarterPoint);
          localNodes.push_back(midPoint);
          localNodes.push_back(threeQuarterPoint);
          break;
        }
        case 49:
        {
          ComputeMidPoint(midPoint, localNodes[70], localNodes[74], quarter_point_creation, mid_point_creation, three_quarter_point_creation);
          localNodes.push_back(midPoint);
          break;
        }
        case 50:
        {
          ComputeInteriorPoints(quarterPoint, midPoint, threeQuarterPoint, localNodes[73], localNodes[77], quarter_point_creation, mid_point_creation, three_quarter_point_creation);
          localNodes.push_back(quarterPoint);
          localNodes.push_back(midPoint);
          localNodes.push_back(threeQuarterPoint);
          break;
        }
        case 51:
        {
          ComputeMidPoint(midPoint, localNodes[76], localNodes[68], quarter_point_creation, mid_point_creation, three_quarter_point_creation);
          localNodes.push_back(midPoint);
          break;
        }
        case 52:
        {
          ComputeMidPoint(midPoint, localNodes[12], localNodes[18], quarter_point_creation, mid_point_creation, three_quarter_point_creation);
          localNodes.push_back(midPoint);
          break;
        }
        case 53:
        {
          ComputeMidPoint(midPoint, localNodes[24], localNodes[30], quarter_point_creation, mid_point_creation, three_quarter_point_creation);
          localNodes.push_back(midPoint);
          break;
        }
        case 54:
        {
          ComputeMidPoint(midPoint, localNodes[48], localNodes[54], quarter_point_creation, mid_point_creation, three_quarter_point_creation);
          localNodes.push_back(midPoint);
          break;
        }
        case 55:
        {
          ComputeMidPoint(midPoint, localNodes[60], localNodes[66], quarter_point_creation, mid_point_creation, three_quarter_point_creation);
          localNodes.push_back(midPoint);
          break;
        }
        case 56:
        {
          ComputeMidPoint(midPoint, localNodes[72], localNodes[78], quarter_point_creation, mid_point_creation, three_quarter_point_creation);
          localNodes.push_back(midPoint);
          break;
        }
      }
      if(quarter_point_creation)
      {
        std::map<std::vector<XdmfFloat64>, XdmfInt32>::const_iterator iter = coordToIdMap.find(quarterPoint);
        if(iter == coordToIdMap.end())
        {
          XdmfInt32 newId = newPoints.size() / 3;
          coordToIdMap[quarterPoint] = newId;
          newConnectivity.push_back(newId);
          newPoints.push_back(quarterPoint[0]);
          newPoints.push_back(quarterPoint[1]);
          newPoints.push_back(quarterPoint[2]);
        }
        else
        {
          newConnectivity.push_back(iter->second);
        }
      }
      if(mid_point_creation)
      {
        std::map<std::vector<XdmfFloat64>, XdmfInt32>::const_iterator iter = coordToIdMap.find(midPoint);
        if(iter == coordToIdMap.end())
        {
          XdmfInt32 newId = newPoints.size() / 3;
          coordToIdMap[midPoint] = newId;
          newConnectivity.push_back(newId);
          newPoints.push_back(midPoint[0]);
          newPoints.push_back(midPoint[1]);
          newPoints.push_back(midPoint[2]);
        }
        else
        {
          newConnectivity.push_back(iter->second);
        }
      }
      if(three_quarter_point_creation)
      {
        std::map<std::vector<XdmfFloat64>, XdmfInt32>::const_iterator iter = coordToIdMap.find(threeQuarterPoint);
        if(iter == coordToIdMap.end())
        {
          XdmfInt32 newId = newPoints.size() / 3;
          coordToIdMap[threeQuarterPoint] = newId;
          newConnectivity.push_back(newId);
          newPoints.push_back(threeQuarterPoint[0]);
          newPoints.push_back(threeQuarterPoint[1]);
          newPoints.push_back(threeQuarterPoint[2]);
        }
        else
        {
          newConnectivity.push_back(iter->second);
        }
      }
    }
  }

  XdmfGrid * newGrid = new XdmfGrid();
  newGrid->SetDeleteOnGridDelete(true);
  newGrid->SetName(grid->GetName());

  parentElement->Insert(newGrid);

  newGrid->GetGeometry()->SetGeometryType(grid->GetGeometry()->GetGeometryType());
  newGrid->GetGeometry()->SetNumberOfPoints(newPoints.size() / 3);
  newGrid->GetGeometry()->SetDeleteOnGridDelete(true);
  newGrid->GetGeometry()->GetPoints()->SetNumberType(grid->GetGeometry()->GetPoints()->GetNumberType());
  newGrid->GetGeometry()->GetPoints()->SetNumberOfElements(newPoints.size());
  newGrid->GetGeometry()->GetPoints()->SetValues(0, &newPoints[0], newPoints.size());

  newGrid->GetTopology()->SetTopologyType(XDMF_HEX_125);
  newGrid->GetTopology()->SetNumberOfElements(grid->GetTopology()->GetNumberOfElements());
  newGrid->GetTopology()->SetDeleteOnGridDelete(true);
  newGrid->GetTopology()->GetConnectivity()->SetNumberType(grid->GetTopology()->GetConnectivity()->GetNumberType());
  newGrid->GetTopology()->GetConnectivity()->SetNumberOfElements(newConnectivity.size());
  newGrid->GetTopology()->GetConnectivity()->SetValues(0, &newConnectivity[0], newConnectivity.size());

  for(int i=0; i<grid->GetNumberOfSets(); i++)
  {
    XdmfSet * currSet = grid->GetSets(i);
    if(currSet->GetSetType() == XDMF_SET_TYPE_NODE)
    {
      newGrid->Insert(currSet);
      currSet->SetDeleteOnGridDelete(false);
    }
  }
  return newGrid;
}

XdmfGrid * XdmfHex125Generator::Split(XdmfGrid * grid, XdmfElement * parentElement)
{
  if(grid->GetTopology()->GetTopologyType() != XDMF_HEX_125)
  {
    std::cout << "This requires a Tri-Quartic Hexahedron Grid" << std::endl;
    return NULL;
  }

  if(grid->GetGeometry()->GetGeometryType() != XDMF_GEOMETRY_XYZ)
  {
    std::cout << "This requires XYZ Geometry" << std::endl;
    return NULL;
  }

  XdmfGrid * newGrid = new XdmfGrid();
  newGrid->SetDeleteOnGridDelete(true);
  newGrid->SetName(grid->GetName());

  parentElement->Insert(newGrid);

  XdmfGeometry * newGeometry = newGrid->GetGeometry();
  newGeometry->SetGeometryType(grid->GetGeometry()->GetGeometryType());
  newGeometry->SetNumberOfPoints(grid->GetGeometry()->GetNumberOfPoints());
  newGeometry->SetDeleteOnGridDelete(false);

  XdmfArray * newPoints = newGeometry->GetPoints();
  newPoints->SetNumberType(grid->GetGeometry()->GetPoints()->GetNumberType());
  newPoints->SetNumberOfElements(grid->GetGeometry()->GetPoints()->GetNumberOfElements());
  newPoints->SetValues(0, grid->GetGeometry()->GetPoints(), grid->GetGeometry()->GetPoints()->GetNumberOfElements(), 0);

  XdmfTopology * newTopology = newGrid->GetTopology();
  newTopology->SetTopologyType(XDMF_HEX);
  newTopology->SetNumberOfElements(grid->GetTopology()->GetNumberOfElements() * 64);
  newTopology->SetDeleteOnGridDelete(true);

  XdmfArray * newConnectivity = newTopology->GetConnectivity();
  newConnectivity->SetNumberType(grid->GetTopology()->GetConnectivity()->GetNumberType());
  newConnectivity->SetNumberOfElements(newGrid->GetTopology()->GetNumberOfElements() * newGrid->GetTopology()->GetNodesPerElement());

  XdmfArray * oldConnectivity = grid->GetTopology()->GetConnectivity();

  for(int i=0; i<grid->GetTopology()->GetNumberOfElements(); ++i)
  {
    newConnectivity->SetValues(i*512 + 0, oldConnectivity, 1, 0 + 125 * i);
    newConnectivity->SetValues(i*512 + 1, oldConnectivity, 1, 8 + 125 * i);
    newConnectivity->SetValues(i*512 + 2, oldConnectivity, 1, 80 + 125 * i);
    newConnectivity->SetValues(i*512 + 3, oldConnectivity, 1, 19 + 125 * i);
    newConnectivity->SetValues(i*512 + 4, oldConnectivity, 1, 32 + 125 * i);
    newConnectivity->SetValues(i*512 + 5, oldConnectivity, 1, 44 + 125 * i);
    newConnectivity->SetValues(i*512 + 6, oldConnectivity, 1, 96 + 125 * i);
    newConnectivity->SetValues(i*512 + 7, oldConnectivity, 1, 55 + 125 * i);
    newConnectivity->SetValues(i*512 + 8, oldConnectivity, 1, 8 + 125 * i);
    newConnectivity->SetValues(i*512 + 9, oldConnectivity, 1, 9 + 125 * i);
    newConnectivity->SetValues(i*512 + 10, oldConnectivity, 1, 81 + 125 * i);
    newConnectivity->SetValues(i*512 + 11, oldConnectivity, 1, 80 + 125 * i);
    newConnectivity->SetValues(i*512 + 12, oldConnectivity, 1, 44 + 125 * i);
    newConnectivity->SetValues(i*512 + 13, oldConnectivity, 1, 45 + 125 * i);
    newConnectivity->SetValues(i*512 + 14, oldConnectivity, 1, 97 + 125 * i);
    newConnectivity->SetValues(i*512 + 15, oldConnectivity, 1, 96 + 125 * i);
    newConnectivity->SetValues(i*512 + 16, oldConnectivity, 1, 9 + 125 * i);
    newConnectivity->SetValues(i*512 + 17, oldConnectivity, 1, 10 + 125 * i);
    newConnectivity->SetValues(i*512 + 18, oldConnectivity, 1, 82 + 125 * i);
    newConnectivity->SetValues(i*512 + 19, oldConnectivity, 1, 81 + 125 * i);
    newConnectivity->SetValues(i*512 + 20, oldConnectivity, 1, 45 + 125 * i);
    newConnectivity->SetValues(i*512 + 21, oldConnectivity, 1, 46 + 125 * i);
    newConnectivity->SetValues(i*512 + 22, oldConnectivity, 1, 98 + 125 * i);
    newConnectivity->SetValues(i*512 + 23, oldConnectivity, 1, 97 + 125 * i);
    newConnectivity->SetValues(i*512 + 24, oldConnectivity, 1, 10 + 125 * i);
    newConnectivity->SetValues(i*512 + 25, oldConnectivity, 1, 1 + 125 * i);
    newConnectivity->SetValues(i*512 + 26, oldConnectivity, 1, 11 + 125 * i);
    newConnectivity->SetValues(i*512 + 27, oldConnectivity, 1, 82 + 125 * i);
    newConnectivity->SetValues(i*512 + 28, oldConnectivity, 1, 46 + 125 * i);
    newConnectivity->SetValues(i*512 + 29, oldConnectivity, 1, 33 + 125 * i);
    newConnectivity->SetValues(i*512 + 30, oldConnectivity, 1, 47 + 125 * i);
    newConnectivity->SetValues(i*512 + 31, oldConnectivity, 1, 98 + 125 * i);
    newConnectivity->SetValues(i*512 + 32, oldConnectivity, 1, 19 + 125 * i);
    newConnectivity->SetValues(i*512 + 33, oldConnectivity, 1, 80 + 125 * i);
    newConnectivity->SetValues(i*512 + 34, oldConnectivity, 1, 87 + 125 * i);
    newConnectivity->SetValues(i*512 + 35, oldConnectivity, 1, 18 + 125 * i);
    newConnectivity->SetValues(i*512 + 36, oldConnectivity, 1, 55 + 125 * i);
    newConnectivity->SetValues(i*512 + 37, oldConnectivity, 1, 96 + 125 * i);
    newConnectivity->SetValues(i*512 + 38, oldConnectivity, 1, 103 + 125 * i);
    newConnectivity->SetValues(i*512 + 39, oldConnectivity, 1, 54 + 125 * i);
    newConnectivity->SetValues(i*512 + 40, oldConnectivity, 1, 80 + 125 * i);
    newConnectivity->SetValues(i*512 + 41, oldConnectivity, 1, 81 + 125 * i);
    newConnectivity->SetValues(i*512 + 42, oldConnectivity, 1, 120 + 125 * i);
    newConnectivity->SetValues(i*512 + 43, oldConnectivity, 1, 87 + 125 * i);
    newConnectivity->SetValues(i*512 + 44, oldConnectivity, 1, 96 + 125 * i);
    newConnectivity->SetValues(i*512 + 45, oldConnectivity, 1, 97 + 125 * i);
    newConnectivity->SetValues(i*512 + 46, oldConnectivity, 1, 122 + 125 * i);
    newConnectivity->SetValues(i*512 + 47, oldConnectivity, 1, 103 + 125 * i);
    newConnectivity->SetValues(i*512 + 48, oldConnectivity, 1, 81 + 125 * i);
    newConnectivity->SetValues(i*512 + 49, oldConnectivity, 1, 82 + 125 * i);
    newConnectivity->SetValues(i*512 + 50, oldConnectivity, 1, 83 + 125 * i);
    newConnectivity->SetValues(i*512 + 51, oldConnectivity, 1, 120 + 125 * i);
    newConnectivity->SetValues(i*512 + 52, oldConnectivity, 1, 97 + 125 * i);
    newConnectivity->SetValues(i*512 + 53, oldConnectivity, 1, 98 + 125 * i);
    newConnectivity->SetValues(i*512 + 54, oldConnectivity, 1, 99 + 125 * i);
    newConnectivity->SetValues(i*512 + 55, oldConnectivity, 1, 122 + 125 * i);
    newConnectivity->SetValues(i*512 + 56, oldConnectivity, 1, 82 + 125 * i);
    newConnectivity->SetValues(i*512 + 57, oldConnectivity, 1, 11 + 125 * i);
    newConnectivity->SetValues(i*512 + 58, oldConnectivity, 1, 12 + 125 * i);
    newConnectivity->SetValues(i*512 + 59, oldConnectivity, 1, 83 + 125 * i);
    newConnectivity->SetValues(i*512 + 60, oldConnectivity, 1, 98 + 125 * i);
    newConnectivity->SetValues(i*512 + 61, oldConnectivity, 1, 47 + 125 * i);
    newConnectivity->SetValues(i*512 + 62, oldConnectivity, 1, 48 + 125 * i);
    newConnectivity->SetValues(i*512 + 63, oldConnectivity, 1, 99 + 125 * i);
    newConnectivity->SetValues(i*512 + 64, oldConnectivity, 1, 18 + 125 * i);
    newConnectivity->SetValues(i*512 + 65, oldConnectivity, 1, 87 + 125 * i);
    newConnectivity->SetValues(i*512 + 66, oldConnectivity, 1, 86 + 125 * i);
    newConnectivity->SetValues(i*512 + 67, oldConnectivity, 1, 17 + 125 * i);
    newConnectivity->SetValues(i*512 + 68, oldConnectivity, 1, 54 + 125 * i);
    newConnectivity->SetValues(i*512 + 69, oldConnectivity, 1, 103 + 125 * i);
    newConnectivity->SetValues(i*512 + 70, oldConnectivity, 1, 102 + 125 * i);
    newConnectivity->SetValues(i*512 + 71, oldConnectivity, 1, 53 + 125 * i);
    newConnectivity->SetValues(i*512 + 72, oldConnectivity, 1, 87 + 125 * i);
    newConnectivity->SetValues(i*512 + 73, oldConnectivity, 1, 120 + 125 * i);
    newConnectivity->SetValues(i*512 + 74, oldConnectivity, 1, 85 + 125 * i);
    newConnectivity->SetValues(i*512 + 75, oldConnectivity, 1, 86 + 125 * i);
    newConnectivity->SetValues(i*512 + 76, oldConnectivity, 1, 103 + 125 * i);
    newConnectivity->SetValues(i*512 + 77, oldConnectivity, 1, 122 + 125 * i);
    newConnectivity->SetValues(i*512 + 78, oldConnectivity, 1, 101 + 125 * i);
    newConnectivity->SetValues(i*512 + 79, oldConnectivity, 1, 102 + 125 * i);
    newConnectivity->SetValues(i*512 + 80, oldConnectivity, 1, 120 + 125 * i);
    newConnectivity->SetValues(i*512 + 81, oldConnectivity, 1, 83 + 125 * i);
    newConnectivity->SetValues(i*512 + 82, oldConnectivity, 1, 84 + 125 * i);
    newConnectivity->SetValues(i*512 + 83, oldConnectivity, 1, 85 + 125 * i);
    newConnectivity->SetValues(i*512 + 84, oldConnectivity, 1, 122 + 125 * i);
    newConnectivity->SetValues(i*512 + 85, oldConnectivity, 1, 99 + 125 * i);
    newConnectivity->SetValues(i*512 + 86, oldConnectivity, 1, 100 + 125 * i);
    newConnectivity->SetValues(i*512 + 87, oldConnectivity, 1, 101 + 125 * i);
    newConnectivity->SetValues(i*512 + 88, oldConnectivity, 1, 83 + 125 * i);
    newConnectivity->SetValues(i*512 + 89, oldConnectivity, 1, 12 + 125 * i);
    newConnectivity->SetValues(i*512 + 90, oldConnectivity, 1, 13 + 125 * i);
    newConnectivity->SetValues(i*512 + 91, oldConnectivity, 1, 84 + 125 * i);
    newConnectivity->SetValues(i*512 + 92, oldConnectivity, 1, 99 + 125 * i);
    newConnectivity->SetValues(i*512 + 93, oldConnectivity, 1, 48 + 125 * i);
    newConnectivity->SetValues(i*512 + 94, oldConnectivity, 1, 49 + 125 * i);
    newConnectivity->SetValues(i*512 + 95, oldConnectivity, 1, 100 + 125 * i);
    newConnectivity->SetValues(i*512 + 96, oldConnectivity, 1, 17 + 125 * i);
    newConnectivity->SetValues(i*512 + 97, oldConnectivity, 1, 86 + 125 * i);
    newConnectivity->SetValues(i*512 + 98, oldConnectivity, 1, 16 + 125 * i);
    newConnectivity->SetValues(i*512 + 99, oldConnectivity, 1, 3 + 125 * i);
    newConnectivity->SetValues(i*512 + 100, oldConnectivity, 1, 53 + 125 * i);
    newConnectivity->SetValues(i*512 + 101, oldConnectivity, 1, 102 + 125 * i);
    newConnectivity->SetValues(i*512 + 102, oldConnectivity, 1, 52 + 125 * i);
    newConnectivity->SetValues(i*512 + 103, oldConnectivity, 1, 35 + 125 * i);
    newConnectivity->SetValues(i*512 + 104, oldConnectivity, 1, 86 + 125 * i);
    newConnectivity->SetValues(i*512 + 105, oldConnectivity, 1, 85 + 125 * i);
    newConnectivity->SetValues(i*512 + 106, oldConnectivity, 1, 15 + 125 * i);
    newConnectivity->SetValues(i*512 + 107, oldConnectivity, 1, 16 + 125 * i);
    newConnectivity->SetValues(i*512 + 108, oldConnectivity, 1, 102 + 125 * i);
    newConnectivity->SetValues(i*512 + 109, oldConnectivity, 1, 101 + 125 * i);
    newConnectivity->SetValues(i*512 + 110, oldConnectivity, 1, 51 + 125 * i);
    newConnectivity->SetValues(i*512 + 111, oldConnectivity, 1, 52 + 125 * i);
    newConnectivity->SetValues(i*512 + 112, oldConnectivity, 1, 85 + 125 * i);
    newConnectivity->SetValues(i*512 + 113, oldConnectivity, 1, 84 + 125 * i);
    newConnectivity->SetValues(i*512 + 114, oldConnectivity, 1, 14 + 125 * i);
    newConnectivity->SetValues(i*512 + 115, oldConnectivity, 1, 15 + 125 * i);
    newConnectivity->SetValues(i*512 + 116, oldConnectivity, 1, 101 + 125 * i);
    newConnectivity->SetValues(i*512 + 117, oldConnectivity, 1, 100 + 125 * i);
    newConnectivity->SetValues(i*512 + 118, oldConnectivity, 1, 50 + 125 * i);
    newConnectivity->SetValues(i*512 + 119, oldConnectivity, 1, 51 + 125 * i);
    newConnectivity->SetValues(i*512 + 120, oldConnectivity, 1, 84 + 125 * i);
    newConnectivity->SetValues(i*512 + 121, oldConnectivity, 1, 13 + 125 * i);
    newConnectivity->SetValues(i*512 + 122, oldConnectivity, 1, 2 + 125 * i);
    newConnectivity->SetValues(i*512 + 123, oldConnectivity, 1, 14 + 125 * i);
    newConnectivity->SetValues(i*512 + 124, oldConnectivity, 1, 100 + 125 * i);
    newConnectivity->SetValues(i*512 + 125, oldConnectivity, 1, 49 + 125 * i);
    newConnectivity->SetValues(i*512 + 126, oldConnectivity, 1, 34 + 125 * i);
    newConnectivity->SetValues(i*512 + 127, oldConnectivity, 1, 50 + 125 * i);
    newConnectivity->SetValues(i*512 + 128, oldConnectivity, 1, 32 + 125 * i);
    newConnectivity->SetValues(i*512 + 129, oldConnectivity, 1, 44 + 125 * i);
    newConnectivity->SetValues(i*512 + 130, oldConnectivity, 1, 96 + 125 * i);
    newConnectivity->SetValues(i*512 + 131, oldConnectivity, 1, 55 + 125 * i);
    newConnectivity->SetValues(i*512 + 132, oldConnectivity, 1, 40 + 125 * i);
    newConnectivity->SetValues(i*512 + 133, oldConnectivity, 1, 68 + 125 * i);
    newConnectivity->SetValues(i*512 + 134, oldConnectivity, 1, 112 + 125 * i);
    newConnectivity->SetValues(i*512 + 135, oldConnectivity, 1, 79 + 125 * i);
    newConnectivity->SetValues(i*512 + 136, oldConnectivity, 1, 44 + 125 * i);
    newConnectivity->SetValues(i*512 + 137, oldConnectivity, 1, 45 + 125 * i);
    newConnectivity->SetValues(i*512 + 138, oldConnectivity, 1, 97 + 125 * i);
    newConnectivity->SetValues(i*512 + 139, oldConnectivity, 1, 96 + 125 * i);
    newConnectivity->SetValues(i*512 + 140, oldConnectivity, 1, 68 + 125 * i);
    newConnectivity->SetValues(i*512 + 141, oldConnectivity, 1, 69 + 125 * i);
    newConnectivity->SetValues(i*512 + 142, oldConnectivity, 1, 113 + 125 * i);
    newConnectivity->SetValues(i*512 + 143, oldConnectivity, 1, 112 + 125 * i);
    newConnectivity->SetValues(i*512 + 144, oldConnectivity, 1, 45 + 125 * i);
    newConnectivity->SetValues(i*512 + 145, oldConnectivity, 1, 46 + 125 * i);
    newConnectivity->SetValues(i*512 + 146, oldConnectivity, 1, 98 + 125 * i);
    newConnectivity->SetValues(i*512 + 147, oldConnectivity, 1, 97 + 125 * i);
    newConnectivity->SetValues(i*512 + 148, oldConnectivity, 1, 69 + 125 * i);
    newConnectivity->SetValues(i*512 + 149, oldConnectivity, 1, 70 + 125 * i);
    newConnectivity->SetValues(i*512 + 150, oldConnectivity, 1, 114 + 125 * i);
    newConnectivity->SetValues(i*512 + 151, oldConnectivity, 1, 113 + 125 * i);
    newConnectivity->SetValues(i*512 + 152, oldConnectivity, 1, 46 + 125 * i);
    newConnectivity->SetValues(i*512 + 153, oldConnectivity, 1, 33 + 125 * i);
    newConnectivity->SetValues(i*512 + 154, oldConnectivity, 1, 47 + 125 * i);
    newConnectivity->SetValues(i*512 + 155, oldConnectivity, 1, 98 + 125 * i);
    newConnectivity->SetValues(i*512 + 156, oldConnectivity, 1, 70 + 125 * i);
    newConnectivity->SetValues(i*512 + 157, oldConnectivity, 1, 41 + 125 * i);
    newConnectivity->SetValues(i*512 + 158, oldConnectivity, 1, 71 + 125 * i);
    newConnectivity->SetValues(i*512 + 159, oldConnectivity, 1, 114 + 125 * i);
    newConnectivity->SetValues(i*512 + 160, oldConnectivity, 1, 55 + 125 * i);
    newConnectivity->SetValues(i*512 + 161, oldConnectivity, 1, 96 + 125 * i);
    newConnectivity->SetValues(i*512 + 162, oldConnectivity, 1, 103 + 125 * i);
    newConnectivity->SetValues(i*512 + 163, oldConnectivity, 1, 54 + 125 * i);
    newConnectivity->SetValues(i*512 + 164, oldConnectivity, 1, 79 + 125 * i);
    newConnectivity->SetValues(i*512 + 165, oldConnectivity, 1, 112 + 125 * i);
    newConnectivity->SetValues(i*512 + 166, oldConnectivity, 1, 119 + 125 * i);
    newConnectivity->SetValues(i*512 + 167, oldConnectivity, 1, 78 + 125 * i);
    newConnectivity->SetValues(i*512 + 168, oldConnectivity, 1, 96 + 125 * i);
    newConnectivity->SetValues(i*512 + 169, oldConnectivity, 1, 97 + 125 * i);
    newConnectivity->SetValues(i*512 + 170, oldConnectivity, 1, 122 + 125 * i);
    newConnectivity->SetValues(i*512 + 171, oldConnectivity, 1, 103 + 125 * i);
    newConnectivity->SetValues(i*512 + 172, oldConnectivity, 1, 112 + 125 * i);
    newConnectivity->SetValues(i*512 + 173, oldConnectivity, 1, 113 + 125 * i);
    newConnectivity->SetValues(i*512 + 174, oldConnectivity, 1, 124 + 125 * i);
    newConnectivity->SetValues(i*512 + 175, oldConnectivity, 1, 119 + 125 * i);
    newConnectivity->SetValues(i*512 + 176, oldConnectivity, 1, 97 + 125 * i);
    newConnectivity->SetValues(i*512 + 177, oldConnectivity, 1, 98 + 125 * i);
    newConnectivity->SetValues(i*512 + 178, oldConnectivity, 1, 99 + 125 * i);
    newConnectivity->SetValues(i*512 + 179, oldConnectivity, 1, 122 + 125 * i);
    newConnectivity->SetValues(i*512 + 180, oldConnectivity, 1, 113 + 125 * i);
    newConnectivity->SetValues(i*512 + 181, oldConnectivity, 1, 114 + 125 * i);
    newConnectivity->SetValues(i*512 + 182, oldConnectivity, 1, 115 + 125 * i);
    newConnectivity->SetValues(i*512 + 183, oldConnectivity, 1, 124 + 125 * i);
    newConnectivity->SetValues(i*512 + 184, oldConnectivity, 1, 98 + 125 * i);
    newConnectivity->SetValues(i*512 + 185, oldConnectivity, 1, 47 + 125 * i);
    newConnectivity->SetValues(i*512 + 186, oldConnectivity, 1, 48 + 125 * i);
    newConnectivity->SetValues(i*512 + 187, oldConnectivity, 1, 99 + 125 * i);
    newConnectivity->SetValues(i*512 + 188, oldConnectivity, 1, 114 + 125 * i);
    newConnectivity->SetValues(i*512 + 189, oldConnectivity, 1, 71 + 125 * i);
    newConnectivity->SetValues(i*512 + 190, oldConnectivity, 1, 72 + 125 * i);
    newConnectivity->SetValues(i*512 + 191, oldConnectivity, 1, 115 + 125 * i);
    newConnectivity->SetValues(i*512 + 192, oldConnectivity, 1, 54 + 125 * i);
    newConnectivity->SetValues(i*512 + 193, oldConnectivity, 1, 103 + 125 * i);
    newConnectivity->SetValues(i*512 + 194, oldConnectivity, 1, 102 + 125 * i);
    newConnectivity->SetValues(i*512 + 195, oldConnectivity, 1, 53 + 125 * i);
    newConnectivity->SetValues(i*512 + 196, oldConnectivity, 1, 78 + 125 * i);
    newConnectivity->SetValues(i*512 + 197, oldConnectivity, 1, 119 + 125 * i);
    newConnectivity->SetValues(i*512 + 198, oldConnectivity, 1, 118 + 125 * i);
    newConnectivity->SetValues(i*512 + 199, oldConnectivity, 1, 77 + 125 * i);
    newConnectivity->SetValues(i*512 + 200, oldConnectivity, 1, 103 + 125 * i);
    newConnectivity->SetValues(i*512 + 201, oldConnectivity, 1, 122 + 125 * i);
    newConnectivity->SetValues(i*512 + 202, oldConnectivity, 1, 101 + 125 * i);
    newConnectivity->SetValues(i*512 + 203, oldConnectivity, 1, 102 + 125 * i);
    newConnectivity->SetValues(i*512 + 204, oldConnectivity, 1, 119 + 125 * i);
    newConnectivity->SetValues(i*512 + 205, oldConnectivity, 1, 124 + 125 * i);
    newConnectivity->SetValues(i*512 + 206, oldConnectivity, 1, 117 + 125 * i);
    newConnectivity->SetValues(i*512 + 207, oldConnectivity, 1, 118 + 125 * i);
    newConnectivity->SetValues(i*512 + 208, oldConnectivity, 1, 122 + 125 * i);
    newConnectivity->SetValues(i*512 + 209, oldConnectivity, 1, 99 + 125 * i);
    newConnectivity->SetValues(i*512 + 210, oldConnectivity, 1, 100 + 125 * i);
    newConnectivity->SetValues(i*512 + 211, oldConnectivity, 1, 101 + 125 * i);
    newConnectivity->SetValues(i*512 + 212, oldConnectivity, 1, 124 + 125 * i);
    newConnectivity->SetValues(i*512 + 213, oldConnectivity, 1, 115 + 125 * i);
    newConnectivity->SetValues(i*512 + 214, oldConnectivity, 1, 116 + 125 * i);
    newConnectivity->SetValues(i*512 + 215, oldConnectivity, 1, 117 + 125 * i);
    newConnectivity->SetValues(i*512 + 216, oldConnectivity, 1, 99 + 125 * i);
    newConnectivity->SetValues(i*512 + 217, oldConnectivity, 1, 48 + 125 * i);
    newConnectivity->SetValues(i*512 + 218, oldConnectivity, 1, 49 + 125 * i);
    newConnectivity->SetValues(i*512 + 219, oldConnectivity, 1, 100 + 125 * i);
    newConnectivity->SetValues(i*512 + 220, oldConnectivity, 1, 115 + 125 * i);
    newConnectivity->SetValues(i*512 + 221, oldConnectivity, 1, 72 + 125 * i);
    newConnectivity->SetValues(i*512 + 222, oldConnectivity, 1, 73 + 125 * i);
    newConnectivity->SetValues(i*512 + 223, oldConnectivity, 1, 116 + 125 * i);
    newConnectivity->SetValues(i*512 + 224, oldConnectivity, 1, 53 + 125 * i);
    newConnectivity->SetValues(i*512 + 225, oldConnectivity, 1, 102 + 125 * i);
    newConnectivity->SetValues(i*512 + 226, oldConnectivity, 1, 52 + 125 * i);
    newConnectivity->SetValues(i*512 + 227, oldConnectivity, 1, 35 + 125 * i);
    newConnectivity->SetValues(i*512 + 228, oldConnectivity, 1, 77 + 125 * i);
    newConnectivity->SetValues(i*512 + 229, oldConnectivity, 1, 118 + 125 * i);
    newConnectivity->SetValues(i*512 + 230, oldConnectivity, 1, 76 + 125 * i);
    newConnectivity->SetValues(i*512 + 231, oldConnectivity, 1, 43 + 125 * i);
    newConnectivity->SetValues(i*512 + 232, oldConnectivity, 1, 102 + 125 * i);
    newConnectivity->SetValues(i*512 + 233, oldConnectivity, 1, 101 + 125 * i);
    newConnectivity->SetValues(i*512 + 234, oldConnectivity, 1, 51 + 125 * i);
    newConnectivity->SetValues(i*512 + 235, oldConnectivity, 1, 52 + 125 * i);
    newConnectivity->SetValues(i*512 + 236, oldConnectivity, 1, 118 + 125 * i);
    newConnectivity->SetValues(i*512 + 237, oldConnectivity, 1, 117 + 125 * i);
    newConnectivity->SetValues(i*512 + 238, oldConnectivity, 1, 75 + 125 * i);
    newConnectivity->SetValues(i*512 + 239, oldConnectivity, 1, 76 + 125 * i);
    newConnectivity->SetValues(i*512 + 240, oldConnectivity, 1, 101 + 125 * i);
    newConnectivity->SetValues(i*512 + 241, oldConnectivity, 1, 100 + 125 * i);
    newConnectivity->SetValues(i*512 + 242, oldConnectivity, 1, 50 + 125 * i);
    newConnectivity->SetValues(i*512 + 243, oldConnectivity, 1, 51 + 125 * i);
    newConnectivity->SetValues(i*512 + 244, oldConnectivity, 1, 117 + 125 * i);
    newConnectivity->SetValues(i*512 + 245, oldConnectivity, 1, 116 + 125 * i);
    newConnectivity->SetValues(i*512 + 246, oldConnectivity, 1, 74 + 125 * i);
    newConnectivity->SetValues(i*512 + 247, oldConnectivity, 1, 75 + 125 * i);
    newConnectivity->SetValues(i*512 + 248, oldConnectivity, 1, 100 + 125 * i);
    newConnectivity->SetValues(i*512 + 249, oldConnectivity, 1, 49 + 125 * i);
    newConnectivity->SetValues(i*512 + 250, oldConnectivity, 1, 34 + 125 * i);
    newConnectivity->SetValues(i*512 + 251, oldConnectivity, 1, 50 + 125 * i);
    newConnectivity->SetValues(i*512 + 252, oldConnectivity, 1, 116 + 125 * i);
    newConnectivity->SetValues(i*512 + 253, oldConnectivity, 1, 73 + 125 * i);
    newConnectivity->SetValues(i*512 + 254, oldConnectivity, 1, 42 + 125 * i);
    newConnectivity->SetValues(i*512 + 255, oldConnectivity, 1, 74 + 125 * i);
    newConnectivity->SetValues(i*512 + 256, oldConnectivity, 1, 40 + 125 * i);
    newConnectivity->SetValues(i*512 + 257, oldConnectivity, 1, 68 + 125 * i);
    newConnectivity->SetValues(i*512 + 258, oldConnectivity, 1, 112 + 125 * i);
    newConnectivity->SetValues(i*512 + 259, oldConnectivity, 1, 79 + 125 * i);
    newConnectivity->SetValues(i*512 + 260, oldConnectivity, 1, 36 + 125 * i);
    newConnectivity->SetValues(i*512 + 261, oldConnectivity, 1, 56 + 125 * i);
    newConnectivity->SetValues(i*512 + 262, oldConnectivity, 1, 104 + 125 * i);
    newConnectivity->SetValues(i*512 + 263, oldConnectivity, 1, 67 + 125 * i);
    newConnectivity->SetValues(i*512 + 264, oldConnectivity, 1, 68 + 125 * i);
    newConnectivity->SetValues(i*512 + 265, oldConnectivity, 1, 69 + 125 * i);
    newConnectivity->SetValues(i*512 + 266, oldConnectivity, 1, 113 + 125 * i);
    newConnectivity->SetValues(i*512 + 267, oldConnectivity, 1, 112 + 125 * i);
    newConnectivity->SetValues(i*512 + 268, oldConnectivity, 1, 56 + 125 * i);
    newConnectivity->SetValues(i*512 + 269, oldConnectivity, 1, 57 + 125 * i);
    newConnectivity->SetValues(i*512 + 270, oldConnectivity, 1, 105 + 125 * i);
    newConnectivity->SetValues(i*512 + 271, oldConnectivity, 1, 104 + 125 * i);
    newConnectivity->SetValues(i*512 + 272, oldConnectivity, 1, 69 + 125 * i);
    newConnectivity->SetValues(i*512 + 273, oldConnectivity, 1, 70 + 125 * i);
    newConnectivity->SetValues(i*512 + 274, oldConnectivity, 1, 114 + 125 * i);
    newConnectivity->SetValues(i*512 + 275, oldConnectivity, 1, 113 + 125 * i);
    newConnectivity->SetValues(i*512 + 276, oldConnectivity, 1, 57 + 125 * i);
    newConnectivity->SetValues(i*512 + 277, oldConnectivity, 1, 58 + 125 * i);
    newConnectivity->SetValues(i*512 + 278, oldConnectivity, 1, 106 + 125 * i);
    newConnectivity->SetValues(i*512 + 279, oldConnectivity, 1, 105 + 125 * i);
    newConnectivity->SetValues(i*512 + 280, oldConnectivity, 1, 70 + 125 * i);
    newConnectivity->SetValues(i*512 + 281, oldConnectivity, 1, 41 + 125 * i);
    newConnectivity->SetValues(i*512 + 282, oldConnectivity, 1, 71 + 125 * i);
    newConnectivity->SetValues(i*512 + 283, oldConnectivity, 1, 114 + 125 * i);
    newConnectivity->SetValues(i*512 + 284, oldConnectivity, 1, 58 + 125 * i);
    newConnectivity->SetValues(i*512 + 285, oldConnectivity, 1, 37 + 125 * i);
    newConnectivity->SetValues(i*512 + 286, oldConnectivity, 1, 59 + 125 * i);
    newConnectivity->SetValues(i*512 + 287, oldConnectivity, 1, 106 + 125 * i);
    newConnectivity->SetValues(i*512 + 288, oldConnectivity, 1, 79 + 125 * i);
    newConnectivity->SetValues(i*512 + 289, oldConnectivity, 1, 112 + 125 * i);
    newConnectivity->SetValues(i*512 + 290, oldConnectivity, 1, 119 + 125 * i);
    newConnectivity->SetValues(i*512 + 291, oldConnectivity, 1, 78 + 125 * i);
    newConnectivity->SetValues(i*512 + 292, oldConnectivity, 1, 67 + 125 * i);
    newConnectivity->SetValues(i*512 + 293, oldConnectivity, 1, 104 + 125 * i);
    newConnectivity->SetValues(i*512 + 294, oldConnectivity, 1, 111 + 125 * i);
    newConnectivity->SetValues(i*512 + 295, oldConnectivity, 1, 66 + 125 * i);
    newConnectivity->SetValues(i*512 + 296, oldConnectivity, 1, 112 + 125 * i);
    newConnectivity->SetValues(i*512 + 297, oldConnectivity, 1, 113 + 125 * i);
    newConnectivity->SetValues(i*512 + 298, oldConnectivity, 1, 124 + 125 * i);
    newConnectivity->SetValues(i*512 + 299, oldConnectivity, 1, 119 + 125 * i);
    newConnectivity->SetValues(i*512 + 300, oldConnectivity, 1, 104 + 125 * i);
    newConnectivity->SetValues(i*512 + 301, oldConnectivity, 1, 105 + 125 * i);
    newConnectivity->SetValues(i*512 + 302, oldConnectivity, 1, 123 + 125 * i);
    newConnectivity->SetValues(i*512 + 303, oldConnectivity, 1, 111 + 125 * i);
    newConnectivity->SetValues(i*512 + 304, oldConnectivity, 1, 113 + 125 * i);
    newConnectivity->SetValues(i*512 + 305, oldConnectivity, 1, 114 + 125 * i);
    newConnectivity->SetValues(i*512 + 306, oldConnectivity, 1, 115 + 125 * i);
    newConnectivity->SetValues(i*512 + 307, oldConnectivity, 1, 124 + 125 * i);
    newConnectivity->SetValues(i*512 + 308, oldConnectivity, 1, 105 + 125 * i);
    newConnectivity->SetValues(i*512 + 309, oldConnectivity, 1, 106 + 125 * i);
    newConnectivity->SetValues(i*512 + 310, oldConnectivity, 1, 107 + 125 * i);
    newConnectivity->SetValues(i*512 + 311, oldConnectivity, 1, 123 + 125 * i);
    newConnectivity->SetValues(i*512 + 312, oldConnectivity, 1, 114 + 125 * i);
    newConnectivity->SetValues(i*512 + 313, oldConnectivity, 1, 71 + 125 * i);
    newConnectivity->SetValues(i*512 + 314, oldConnectivity, 1, 72 + 125 * i);
    newConnectivity->SetValues(i*512 + 315, oldConnectivity, 1, 115 + 125 * i);
    newConnectivity->SetValues(i*512 + 316, oldConnectivity, 1, 106 + 125 * i);
    newConnectivity->SetValues(i*512 + 317, oldConnectivity, 1, 59 + 125 * i);
    newConnectivity->SetValues(i*512 + 318, oldConnectivity, 1, 60 + 125 * i);
    newConnectivity->SetValues(i*512 + 319, oldConnectivity, 1, 107 + 125 * i);
    newConnectivity->SetValues(i*512 + 320, oldConnectivity, 1, 78 + 125 * i);
    newConnectivity->SetValues(i*512 + 321, oldConnectivity, 1, 119 + 125 * i);
    newConnectivity->SetValues(i*512 + 322, oldConnectivity, 1, 118 + 125 * i);
    newConnectivity->SetValues(i*512 + 323, oldConnectivity, 1, 77 + 125 * i);
    newConnectivity->SetValues(i*512 + 324, oldConnectivity, 1, 66 + 125 * i);
    newConnectivity->SetValues(i*512 + 325, oldConnectivity, 1, 111 + 125 * i);
    newConnectivity->SetValues(i*512 + 326, oldConnectivity, 1, 110 + 125 * i);
    newConnectivity->SetValues(i*512 + 327, oldConnectivity, 1, 65 + 125 * i);
    newConnectivity->SetValues(i*512 + 328, oldConnectivity, 1, 119 + 125 * i);
    newConnectivity->SetValues(i*512 + 329, oldConnectivity, 1, 124 + 125 * i);
    newConnectivity->SetValues(i*512 + 330, oldConnectivity, 1, 117 + 125 * i);
    newConnectivity->SetValues(i*512 + 331, oldConnectivity, 1, 118 + 125 * i);
    newConnectivity->SetValues(i*512 + 332, oldConnectivity, 1, 111 + 125 * i);
    newConnectivity->SetValues(i*512 + 333, oldConnectivity, 1, 123 + 125 * i);
    newConnectivity->SetValues(i*512 + 334, oldConnectivity, 1, 109 + 125 * i);
    newConnectivity->SetValues(i*512 + 335, oldConnectivity, 1, 110 + 125 * i);
    newConnectivity->SetValues(i*512 + 336, oldConnectivity, 1, 124 + 125 * i);
    newConnectivity->SetValues(i*512 + 337, oldConnectivity, 1, 115 + 125 * i);
    newConnectivity->SetValues(i*512 + 338, oldConnectivity, 1, 116 + 125 * i);
    newConnectivity->SetValues(i*512 + 339, oldConnectivity, 1, 117 + 125 * i);
    newConnectivity->SetValues(i*512 + 340, oldConnectivity, 1, 123 + 125 * i);
    newConnectivity->SetValues(i*512 + 341, oldConnectivity, 1, 107 + 125 * i);
    newConnectivity->SetValues(i*512 + 342, oldConnectivity, 1, 108 + 125 * i);
    newConnectivity->SetValues(i*512 + 343, oldConnectivity, 1, 109 + 125 * i);
    newConnectivity->SetValues(i*512 + 344, oldConnectivity, 1, 115 + 125 * i);
    newConnectivity->SetValues(i*512 + 345, oldConnectivity, 1, 72 + 125 * i);
    newConnectivity->SetValues(i*512 + 346, oldConnectivity, 1, 73 + 125 * i);
    newConnectivity->SetValues(i*512 + 347, oldConnectivity, 1, 116 + 125 * i);
    newConnectivity->SetValues(i*512 + 348, oldConnectivity, 1, 107 + 125 * i);
    newConnectivity->SetValues(i*512 + 349, oldConnectivity, 1, 60 + 125 * i);
    newConnectivity->SetValues(i*512 + 350, oldConnectivity, 1, 61 + 125 * i);
    newConnectivity->SetValues(i*512 + 351, oldConnectivity, 1, 108 + 125 * i);
    newConnectivity->SetValues(i*512 + 352, oldConnectivity, 1, 77 + 125 * i);
    newConnectivity->SetValues(i*512 + 353, oldConnectivity, 1, 118 + 125 * i);
    newConnectivity->SetValues(i*512 + 354, oldConnectivity, 1, 76 + 125 * i);
    newConnectivity->SetValues(i*512 + 355, oldConnectivity, 1, 43 + 125 * i);
    newConnectivity->SetValues(i*512 + 356, oldConnectivity, 1, 65 + 125 * i);
    newConnectivity->SetValues(i*512 + 357, oldConnectivity, 1, 110 + 125 * i);
    newConnectivity->SetValues(i*512 + 358, oldConnectivity, 1, 64 + 125 * i);
    newConnectivity->SetValues(i*512 + 359, oldConnectivity, 1, 39 + 125 * i);
    newConnectivity->SetValues(i*512 + 360, oldConnectivity, 1, 118 + 125 * i);
    newConnectivity->SetValues(i*512 + 361, oldConnectivity, 1, 117 + 125 * i);
    newConnectivity->SetValues(i*512 + 362, oldConnectivity, 1, 75 + 125 * i);
    newConnectivity->SetValues(i*512 + 363, oldConnectivity, 1, 76 + 125 * i);
    newConnectivity->SetValues(i*512 + 364, oldConnectivity, 1, 110 + 125 * i);
    newConnectivity->SetValues(i*512 + 365, oldConnectivity, 1, 109 + 125 * i);
    newConnectivity->SetValues(i*512 + 366, oldConnectivity, 1, 63 + 125 * i);
    newConnectivity->SetValues(i*512 + 367, oldConnectivity, 1, 64 + 125 * i);
    newConnectivity->SetValues(i*512 + 368, oldConnectivity, 1, 117 + 125 * i);
    newConnectivity->SetValues(i*512 + 369, oldConnectivity, 1, 116 + 125 * i);
    newConnectivity->SetValues(i*512 + 370, oldConnectivity, 1, 74 + 125 * i);
    newConnectivity->SetValues(i*512 + 371, oldConnectivity, 1, 75 + 125 * i);
    newConnectivity->SetValues(i*512 + 372, oldConnectivity, 1, 109 + 125 * i);
    newConnectivity->SetValues(i*512 + 373, oldConnectivity, 1, 108 + 125 * i);
    newConnectivity->SetValues(i*512 + 374, oldConnectivity, 1, 62 + 125 * i);
    newConnectivity->SetValues(i*512 + 375, oldConnectivity, 1, 63 + 125 * i);
    newConnectivity->SetValues(i*512 + 376, oldConnectivity, 1, 116 + 125 * i);
    newConnectivity->SetValues(i*512 + 377, oldConnectivity, 1, 73 + 125 * i);
    newConnectivity->SetValues(i*512 + 378, oldConnectivity, 1, 42 + 125 * i);
    newConnectivity->SetValues(i*512 + 379, oldConnectivity, 1, 74 + 125 * i);
    newConnectivity->SetValues(i*512 + 380, oldConnectivity, 1, 108 + 125 * i);
    newConnectivity->SetValues(i*512 + 381, oldConnectivity, 1, 61 + 125 * i);
    newConnectivity->SetValues(i*512 + 382, oldConnectivity, 1, 38 + 125 * i);
    newConnectivity->SetValues(i*512 + 383, oldConnectivity, 1, 62 + 125 * i);
    newConnectivity->SetValues(i*512 + 384, oldConnectivity, 1, 36 + 125 * i);
    newConnectivity->SetValues(i*512 + 385, oldConnectivity, 1, 56 + 125 * i);
    newConnectivity->SetValues(i*512 + 386, oldConnectivity, 1, 104 + 125 * i);
    newConnectivity->SetValues(i*512 + 387, oldConnectivity, 1, 67 + 125 * i);
    newConnectivity->SetValues(i*512 + 388, oldConnectivity, 1, 4 + 125 * i);
    newConnectivity->SetValues(i*512 + 389, oldConnectivity, 1, 20 + 125 * i);
    newConnectivity->SetValues(i*512 + 390, oldConnectivity, 1, 88 + 125 * i);
    newConnectivity->SetValues(i*512 + 391, oldConnectivity, 1, 31 + 125 * i);
    newConnectivity->SetValues(i*512 + 392, oldConnectivity, 1, 56 + 125 * i);
    newConnectivity->SetValues(i*512 + 393, oldConnectivity, 1, 57 + 125 * i);
    newConnectivity->SetValues(i*512 + 394, oldConnectivity, 1, 105 + 125 * i);
    newConnectivity->SetValues(i*512 + 395, oldConnectivity, 1, 104 + 125 * i);
    newConnectivity->SetValues(i*512 + 396, oldConnectivity, 1, 20 + 125 * i);
    newConnectivity->SetValues(i*512 + 397, oldConnectivity, 1, 21 + 125 * i);
    newConnectivity->SetValues(i*512 + 398, oldConnectivity, 1, 89 + 125 * i);
    newConnectivity->SetValues(i*512 + 399, oldConnectivity, 1, 88 + 125 * i);
    newConnectivity->SetValues(i*512 + 400, oldConnectivity, 1, 57 + 125 * i);
    newConnectivity->SetValues(i*512 + 401, oldConnectivity, 1, 58 + 125 * i);
    newConnectivity->SetValues(i*512 + 402, oldConnectivity, 1, 106 + 125 * i);
    newConnectivity->SetValues(i*512 + 403, oldConnectivity, 1, 105 + 125 * i);
    newConnectivity->SetValues(i*512 + 404, oldConnectivity, 1, 21 + 125 * i);
    newConnectivity->SetValues(i*512 + 405, oldConnectivity, 1, 22 + 125 * i);
    newConnectivity->SetValues(i*512 + 406, oldConnectivity, 1, 90 + 125 * i);
    newConnectivity->SetValues(i*512 + 407, oldConnectivity, 1, 89 + 125 * i);
    newConnectivity->SetValues(i*512 + 408, oldConnectivity, 1, 58 + 125 * i);
    newConnectivity->SetValues(i*512 + 409, oldConnectivity, 1, 37 + 125 * i);
    newConnectivity->SetValues(i*512 + 410, oldConnectivity, 1, 59 + 125 * i);
    newConnectivity->SetValues(i*512 + 411, oldConnectivity, 1, 106 + 125 * i);
    newConnectivity->SetValues(i*512 + 412, oldConnectivity, 1, 22 + 125 * i);
    newConnectivity->SetValues(i*512 + 413, oldConnectivity, 1, 5 + 125 * i);
    newConnectivity->SetValues(i*512 + 414, oldConnectivity, 1, 23 + 125 * i);
    newConnectivity->SetValues(i*512 + 415, oldConnectivity, 1, 90 + 125 * i);
    newConnectivity->SetValues(i*512 + 416, oldConnectivity, 1, 67 + 125 * i);
    newConnectivity->SetValues(i*512 + 417, oldConnectivity, 1, 104 + 125 * i);
    newConnectivity->SetValues(i*512 + 418, oldConnectivity, 1, 111 + 125 * i);
    newConnectivity->SetValues(i*512 + 419, oldConnectivity, 1, 66 + 125 * i);
    newConnectivity->SetValues(i*512 + 420, oldConnectivity, 1, 31 + 125 * i);
    newConnectivity->SetValues(i*512 + 421, oldConnectivity, 1, 88 + 125 * i);
    newConnectivity->SetValues(i*512 + 422, oldConnectivity, 1, 95 + 125 * i);
    newConnectivity->SetValues(i*512 + 423, oldConnectivity, 1, 30 + 125 * i);
    newConnectivity->SetValues(i*512 + 424, oldConnectivity, 1, 104 + 125 * i);
    newConnectivity->SetValues(i*512 + 425, oldConnectivity, 1, 105 + 125 * i);
    newConnectivity->SetValues(i*512 + 426, oldConnectivity, 1, 123 + 125 * i);
    newConnectivity->SetValues(i*512 + 427, oldConnectivity, 1, 111 + 125 * i);
    newConnectivity->SetValues(i*512 + 428, oldConnectivity, 1, 88 + 125 * i);
    newConnectivity->SetValues(i*512 + 429, oldConnectivity, 1, 89 + 125 * i);
    newConnectivity->SetValues(i*512 + 430, oldConnectivity, 1, 121 + 125 * i);
    newConnectivity->SetValues(i*512 + 431, oldConnectivity, 1, 95 + 125 * i);
    newConnectivity->SetValues(i*512 + 432, oldConnectivity, 1, 105 + 125 * i);
    newConnectivity->SetValues(i*512 + 433, oldConnectivity, 1, 106 + 125 * i);
    newConnectivity->SetValues(i*512 + 434, oldConnectivity, 1, 107 + 125 * i);
    newConnectivity->SetValues(i*512 + 435, oldConnectivity, 1, 123 + 125 * i);
    newConnectivity->SetValues(i*512 + 436, oldConnectivity, 1, 89 + 125 * i);
    newConnectivity->SetValues(i*512 + 437, oldConnectivity, 1, 90 + 125 * i);
    newConnectivity->SetValues(i*512 + 438, oldConnectivity, 1, 91 + 125 * i);
    newConnectivity->SetValues(i*512 + 439, oldConnectivity, 1, 121 + 125 * i);
    newConnectivity->SetValues(i*512 + 440, oldConnectivity, 1, 106 + 125 * i);
    newConnectivity->SetValues(i*512 + 441, oldConnectivity, 1, 59 + 125 * i);
    newConnectivity->SetValues(i*512 + 442, oldConnectivity, 1, 60 + 125 * i);
    newConnectivity->SetValues(i*512 + 443, oldConnectivity, 1, 107 + 125 * i);
    newConnectivity->SetValues(i*512 + 444, oldConnectivity, 1, 90 + 125 * i);
    newConnectivity->SetValues(i*512 + 445, oldConnectivity, 1, 23 + 125 * i);
    newConnectivity->SetValues(i*512 + 446, oldConnectivity, 1, 24 + 125 * i);
    newConnectivity->SetValues(i*512 + 447, oldConnectivity, 1, 91 + 125 * i);
    newConnectivity->SetValues(i*512 + 448, oldConnectivity, 1, 66 + 125 * i);
    newConnectivity->SetValues(i*512 + 449, oldConnectivity, 1, 111 + 125 * i);
    newConnectivity->SetValues(i*512 + 450, oldConnectivity, 1, 110 + 125 * i);
    newConnectivity->SetValues(i*512 + 451, oldConnectivity, 1, 65 + 125 * i);
    newConnectivity->SetValues(i*512 + 452, oldConnectivity, 1, 30 + 125 * i);
    newConnectivity->SetValues(i*512 + 453, oldConnectivity, 1, 95 + 125 * i);
    newConnectivity->SetValues(i*512 + 454, oldConnectivity, 1, 94 + 125 * i);
    newConnectivity->SetValues(i*512 + 455, oldConnectivity, 1, 29 + 125 * i);
    newConnectivity->SetValues(i*512 + 456, oldConnectivity, 1, 111 + 125 * i);
    newConnectivity->SetValues(i*512 + 457, oldConnectivity, 1, 123 + 125 * i);
    newConnectivity->SetValues(i*512 + 458, oldConnectivity, 1, 109 + 125 * i);
    newConnectivity->SetValues(i*512 + 459, oldConnectivity, 1, 110 + 125 * i);
    newConnectivity->SetValues(i*512 + 460, oldConnectivity, 1, 95 + 125 * i);
    newConnectivity->SetValues(i*512 + 461, oldConnectivity, 1, 121 + 125 * i);
    newConnectivity->SetValues(i*512 + 462, oldConnectivity, 1, 93 + 125 * i);
    newConnectivity->SetValues(i*512 + 463, oldConnectivity, 1, 94 + 125 * i);
    newConnectivity->SetValues(i*512 + 464, oldConnectivity, 1, 123 + 125 * i);
    newConnectivity->SetValues(i*512 + 465, oldConnectivity, 1, 107 + 125 * i);
    newConnectivity->SetValues(i*512 + 466, oldConnectivity, 1, 108 + 125 * i);
    newConnectivity->SetValues(i*512 + 467, oldConnectivity, 1, 109 + 125 * i);
    newConnectivity->SetValues(i*512 + 468, oldConnectivity, 1, 121 + 125 * i);
    newConnectivity->SetValues(i*512 + 469, oldConnectivity, 1, 91 + 125 * i);
    newConnectivity->SetValues(i*512 + 470, oldConnectivity, 1, 92 + 125 * i);
    newConnectivity->SetValues(i*512 + 471, oldConnectivity, 1, 93 + 125 * i);
    newConnectivity->SetValues(i*512 + 472, oldConnectivity, 1, 107 + 125 * i);
    newConnectivity->SetValues(i*512 + 473, oldConnectivity, 1, 60 + 125 * i);
    newConnectivity->SetValues(i*512 + 474, oldConnectivity, 1, 61 + 125 * i);
    newConnectivity->SetValues(i*512 + 475, oldConnectivity, 1, 108 + 125 * i);
    newConnectivity->SetValues(i*512 + 476, oldConnectivity, 1, 91 + 125 * i);
    newConnectivity->SetValues(i*512 + 477, oldConnectivity, 1, 24 + 125 * i);
    newConnectivity->SetValues(i*512 + 478, oldConnectivity, 1, 25 + 125 * i);
    newConnectivity->SetValues(i*512 + 479, oldConnectivity, 1, 92 + 125 * i);
    newConnectivity->SetValues(i*512 + 480, oldConnectivity, 1, 65 + 125 * i);
    newConnectivity->SetValues(i*512 + 481, oldConnectivity, 1, 110 + 125 * i);
    newConnectivity->SetValues(i*512 + 482, oldConnectivity, 1, 64 + 125 * i);
    newConnectivity->SetValues(i*512 + 483, oldConnectivity, 1, 39 + 125 * i);
    newConnectivity->SetValues(i*512 + 484, oldConnectivity, 1, 29 + 125 * i);
    newConnectivity->SetValues(i*512 + 485, oldConnectivity, 1, 94 + 125 * i);
    newConnectivity->SetValues(i*512 + 486, oldConnectivity, 1, 28 + 125 * i);
    newConnectivity->SetValues(i*512 + 487, oldConnectivity, 1, 7 + 125 * i);
    newConnectivity->SetValues(i*512 + 488, oldConnectivity, 1, 110 + 125 * i);
    newConnectivity->SetValues(i*512 + 489, oldConnectivity, 1, 109 + 125 * i);
    newConnectivity->SetValues(i*512 + 490, oldConnectivity, 1, 63 + 125 * i);
    newConnectivity->SetValues(i*512 + 491, oldConnectivity, 1, 64 + 125 * i);
    newConnectivity->SetValues(i*512 + 492, oldConnectivity, 1, 94 + 125 * i);
    newConnectivity->SetValues(i*512 + 493, oldConnectivity, 1, 93 + 125 * i);
    newConnectivity->SetValues(i*512 + 494, oldConnectivity, 1, 27 + 125 * i);
    newConnectivity->SetValues(i*512 + 495, oldConnectivity, 1, 28 + 125 * i);
    newConnectivity->SetValues(i*512 + 496, oldConnectivity, 1, 109 + 125 * i);
    newConnectivity->SetValues(i*512 + 497, oldConnectivity, 1, 108 + 125 * i);
    newConnectivity->SetValues(i*512 + 498, oldConnectivity, 1, 62 + 125 * i);
    newConnectivity->SetValues(i*512 + 499, oldConnectivity, 1, 63 + 125 * i);
    newConnectivity->SetValues(i*512 + 500, oldConnectivity, 1, 93 + 125 * i);
    newConnectivity->SetValues(i*512 + 501, oldConnectivity, 1, 92 + 125 * i);
    newConnectivity->SetValues(i*512 + 502, oldConnectivity, 1, 26 + 125 * i);
    newConnectivity->SetValues(i*512 + 503, oldConnectivity, 1, 27 + 125 * i);
    newConnectivity->SetValues(i*512 + 504, oldConnectivity, 1, 108 + 125 * i);
    newConnectivity->SetValues(i*512 + 505, oldConnectivity, 1, 61 + 125 * i);
    newConnectivity->SetValues(i*512 + 506, oldConnectivity, 1, 38 + 125 * i);
    newConnectivity->SetValues(i*512 + 507, oldConnectivity, 1, 62 + 125 * i);
    newConnectivity->SetValues(i*512 + 508, oldConnectivity, 1, 92 + 125 * i);
    newConnectivity->SetValues(i*512 + 509, oldConnectivity, 1, 25 + 125 * i);
    newConnectivity->SetValues(i*512 + 510, oldConnectivity, 1, 6 + 125 * i);
    newConnectivity->SetValues(i*512 + 511, oldConnectivity, 1, 26 + 125 * i);

  }

  for(int i=0; i<grid->GetNumberOfAttributes(); ++i)
  {
    XdmfAttribute * currAttribute = grid->GetAttribute(i);
    currAttribute->Update();
    if(currAttribute->GetAttributeCenter() == XDMF_ATTRIBUTE_CENTER_NODE)
    {
      newGrid->Insert(currAttribute);
      currAttribute->SetDeleteOnGridDelete(false);
    }
    else if(currAttribute->GetAttributeType() == XDMF_ATTRIBUTE_CENTER_CELL)
    {
      XdmfAttribute * newAttribute = new XdmfAttribute();
      newAttribute->SetName(currAttribute->GetName());
      newAttribute->SetAttributeType(currAttribute->GetAttributeType());
      newAttribute->SetAttributeCenter(currAttribute->GetAttributeCenter());
      newAttribute->SetDeleteOnGridDelete(true);

      XdmfArray * vals = newAttribute->GetValues();
      vals->SetNumberType(currAttribute->GetValues()->GetNumberType());
      vals->SetNumberOfElements(currAttribute->GetValues()->GetNumberOfElements() * 64);
      for(int j=0; j<currAttribute->GetValues()->GetNumberOfElements(); ++j)
      {

        vals->SetValues(j*64, currAttribute->GetValues(), 64, j, 1, 0);
      }
      newGrid->Insert(newAttribute);
    }
  }

  return newGrid;
}
