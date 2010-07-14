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
#include "XdmfHex64Generator.h"
#include "XdmfGeometry.h"
#include "XdmfGrid.h"
#include "XdmfInformation.h"
#include "XdmfSet.h"
#include "XdmfTopology.h"

#include <math.h>
#include <map>
#include <vector>

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

class XdmfHex64Generator::Operations
{
public:
  virtual void ComputeInteriorPoints(std::vector<XdmfFloat64> & leftPoint, std::vector<XdmfFloat64> & rightPoint, const std::vector<XdmfFloat64> & point1, const std::vector<XdmfFloat64> & point2) = 0;
  virtual void ComputeLeftPoint(std::vector<XdmfFloat64> & leftPoint, const std::vector<XdmfFloat64> & point1, const std::vector<XdmfFloat64> & point2) = 0;
  virtual void ComputeRightPoint(std::vector<XdmfFloat64> & rightPoint, const std::vector<XdmfFloat64> & point1, const std::vector<XdmfFloat64> & point2) = 0;
};

class XdmfHex64Generator::NormalOperations : public XdmfHex64Generator::Operations
{
public:
  void ComputeInteriorPoints(std::vector<XdmfFloat64> & leftPoint, std::vector<XdmfFloat64> & rightPoint, const std::vector<XdmfFloat64> & point1, const std::vector<XdmfFloat64> & point2);
  void ComputeLeftPoint(std::vector<XdmfFloat64> & leftPoint, const std::vector<XdmfFloat64> & point1, const std::vector<XdmfFloat64> & point2);
  void ComputeRightPoint(std::vector<XdmfFloat64> & rightPoint, const std::vector<XdmfFloat64> & point1, const std::vector<XdmfFloat64> & point2) ;
};

class XdmfHex64Generator::SpectralOperations : public XdmfHex64Generator::Operations
{
public:
  static const double C = 0.44721359549995793; // 1 / sqrt(5)
  virtual void ComputeInteriorPoints(std::vector<XdmfFloat64> & leftPoint, std::vector<XdmfFloat64> & rightPoint, const std::vector<XdmfFloat64> & point1, const std::vector<XdmfFloat64> & point2);
  virtual void ComputeLeftPoint(std::vector<XdmfFloat64> & leftPoint, const std::vector<XdmfFloat64> & point1, const std::vector<XdmfFloat64> & point2);
  virtual void ComputeRightPoint(std::vector<XdmfFloat64> & rightPoint, const std::vector<XdmfFloat64> & point1, const std::vector<XdmfFloat64> & point2);
};

inline void XdmfHex64Generator::NormalOperations::ComputeInteriorPoints(std::vector<XdmfFloat64> & leftPoint, std::vector<XdmfFloat64> & rightPoint, const std::vector<XdmfFloat64> & point1, const std::vector<XdmfFloat64> & point2)
{
  leftPoint[0] = (1.0/3.0)*(point2[0] + 2*point1[0]);
  leftPoint[1] = (1.0/3.0)*(point2[1] + 2*point1[1]);
  leftPoint[2] = (1.0/3.0)*(point2[2] + 2*point1[2]);

  rightPoint[0] = (1.0/3.0)*(2*point2[0] + point1[0]);
  rightPoint[1] = (1.0/3.0)*(2*point2[1] + point1[1]);
  rightPoint[2] = (1.0/3.0)*(2*point2[2] + point1[2]);
}

inline void XdmfHex64Generator::NormalOperations::ComputeLeftPoint(std::vector<XdmfFloat64> & leftPoint, const std::vector<XdmfFloat64> & point1, const std::vector<XdmfFloat64> & point2)
{
  leftPoint[0] = (1.0/3.0)*(point2[0] + 2*point1[0]);
  leftPoint[1] = (1.0/3.0)*(point2[1] + 2*point1[1]);
  leftPoint[2] = (1.0/3.0)*(point2[2] + 2*point1[2]);
}

inline void XdmfHex64Generator::NormalOperations::ComputeRightPoint(std::vector<XdmfFloat64> & rightPoint, const std::vector<XdmfFloat64> & point1, const std::vector<XdmfFloat64> & point2)
{
  rightPoint[0] = (1.0/3.0)*(2*point2[0] + point1[0]);
  rightPoint[1] = (1.0/3.0)*(2*point2[1] + point1[1]);
  rightPoint[2] = (1.0/3.0)*(2*point2[2] + point1[2]);
}

inline void XdmfHex64Generator::SpectralOperations::ComputeInteriorPoints(std::vector<XdmfFloat64> & leftPoint, std::vector<XdmfFloat64> & rightPoint, const std::vector<XdmfFloat64> & point1, const std::vector<XdmfFloat64> & point2)
{
  leftPoint[0] = (1.0/2.0)*((1 - C)*point2[0] + (1 + C)*point1[0]);
  leftPoint[1] = (1.0/2.0)*((1 - C)*point2[1] + (1 + C)*point1[1]);
  leftPoint[2] = (1.0/2.0)*((1 - C)*point2[2] + (1 + C)*point1[2]);

  rightPoint[0] = (1.0/2.0)*((1+C)*point2[0] + (1-C)*point1[0]);
  rightPoint[1] = (1.0/2.0)*((1+C)*point2[1] + (1-C)*point1[1]);
  rightPoint[2] = (1.0/2.0)*((1+C)*point2[2] + (1-C)*point1[2]);
}

inline void XdmfHex64Generator::SpectralOperations::ComputeLeftPoint(std::vector<XdmfFloat64> & leftPoint, const std::vector<XdmfFloat64> & point1, const std::vector<XdmfFloat64> & point2)
{
  leftPoint[0] = (1.0/2.0)*((1 - C)*point2[0] + (1 + C)*point1[0]);
  leftPoint[1] = (1.0/2.0)*((1 - C)*point2[1] + (1 + C)*point1[1]);
  leftPoint[2] = (1.0/2.0)*((1 - C)*point2[2] + (1 + C)*point1[2]);
}

inline void XdmfHex64Generator::SpectralOperations::ComputeRightPoint(std::vector<XdmfFloat64> & rightPoint, const std::vector<XdmfFloat64> & point1, const std::vector<XdmfFloat64> & point2)
{
  rightPoint[0] = (1.0/2.0)*((1+C)*point2[0] + (1-C)*point1[0]);
  rightPoint[1] = (1.0/2.0)*((1+C)*point2[1] + (1-C)*point1[1]);
  rightPoint[2] = (1.0/2.0)*((1+C)*point2[2] + (1-C)*point1[2]);
}

XdmfHex64Generator::XdmfHex64Generator()
{
}

XdmfHex64Generator::~XdmfHex64Generator()
{
}

inline void InsertPointWithoutCheck(const std::vector<XdmfFloat64> & newPoint, std::vector<XdmfInt32> & newConnectivity, std::vector<XdmfFloat64> & newPoints)
{
  newConnectivity.push_back(newPoints.size() / 3);
  newPoints.push_back(newPoint[0]);
  newPoints.push_back(newPoint[1]);
  newPoints.push_back(newPoint[2]);
}

inline void InsertPointWithCheck(const std::vector<XdmfFloat64> & newPoint, std::map<std::vector<XdmfFloat64>, XdmfInt32, PointComparison> & coordToIdMap, std::vector<XdmfInt32> & newConnectivity, std::vector<XdmfFloat64> & newPoints)
{
  std::map<std::vector<XdmfFloat64>, XdmfInt32>::const_iterator iter = coordToIdMap.find(newPoint);
  if(iter == coordToIdMap.end())
  {
    // Not inserted before
    coordToIdMap[newPoint] = newPoints.size() / 3;
    InsertPointWithoutCheck(newPoint, newConnectivity, newPoints);
  }
  else
  {
    newConnectivity.push_back(iter->second);
  }
}

XdmfGrid * XdmfHex64Generator::Generate(XdmfGrid * grid, XdmfElement * parentElement, Type type)
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

  Operations * operation = NULL;
  XdmfInformation * elementGenerationType = new XdmfInformation();
  elementGenerationType->SetDeleteOnGridDelete(true);
  elementGenerationType->SetName("ElementGenerationType");
  if(type == Normal)
  {
    operation = new NormalOperations();
    elementGenerationType->SetValue("Normal");
  }
  else if(type == Spectral)
  {
    operation = new SpectralOperations();
    elementGenerationType->SetValue("Spectral");
  }

  // Copy all coordinate values into new geometry.
  std::vector<XdmfFloat64> newPoints(grid->GetGeometry()->GetPoints()->GetNumberOfElements());
  grid->GetGeometry()->GetPoints()->GetValues(0, &newPoints[0], grid->GetGeometry()->GetPoints()->GetNumberOfElements());

  std::vector<XdmfInt32> newConnectivity;
  std::vector<XdmfFloat64> leftPoint(3);
  std::vector<XdmfFloat64> rightPoint(3);

  newConnectivity.reserve(64 * grid->GetTopology()->GetNumberOfElements());

  std::map<std::vector<XdmfFloat64>, XdmfInt32, PointComparison> coordToIdMap;

  // Iterate over all elements, for each element compute new coordinate points and insert those values into newGeometry and newConnectivity.
  for(int i=0; i<grid->GetTopology()->GetNumberOfElements(); ++i)
  {
    std::cout << "Converting Element " << i << std::endl;

    // Fill localNodes with original coordinate information.
    std::vector<std::vector<XdmfFloat64> > localNodes(8, std::vector<XdmfFloat64>(3));
    localNodes.reserve(44);
    for(int j=0; j<8; ++j)
    {
      grid->GetGeometry()->GetPoints()->GetValues(grid->GetTopology()->GetConnectivity()->GetValueAsInt64(8*i + j) * 3, &localNodes[j][0], 3);
    }

    // Add old connectivity information to newConnectivity.
    int startIndex = newConnectivity.size();
    newConnectivity.resize(newConnectivity.size() + 8);
    grid->GetTopology()->GetConnectivity()->GetValues(8*i, &newConnectivity[startIndex], 8);
    
    // Case 0
    operation->ComputeInteriorPoints(leftPoint, rightPoint, localNodes[0], localNodes[1]);
    localNodes.push_back(leftPoint);
    localNodes.push_back(rightPoint);
    InsertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);
    InsertPointWithCheck(rightPoint, coordToIdMap, newConnectivity, newPoints);

    // Case 1
    operation->ComputeInteriorPoints(leftPoint, rightPoint, localNodes[1], localNodes[2]);
    localNodes.push_back(leftPoint);
    localNodes.push_back(rightPoint);
    InsertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);
    InsertPointWithCheck(rightPoint, coordToIdMap, newConnectivity, newPoints);

    // Case 2
    operation->ComputeInteriorPoints(leftPoint, rightPoint, localNodes[2], localNodes[3]);
    localNodes.push_back(leftPoint);
    localNodes.push_back(rightPoint);
    InsertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);
    InsertPointWithCheck(rightPoint, coordToIdMap, newConnectivity, newPoints);

    // Case 3
    operation->ComputeInteriorPoints(leftPoint, rightPoint, localNodes[3], localNodes[0]);
    localNodes.push_back(leftPoint);
    localNodes.push_back(rightPoint);
    InsertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);
    InsertPointWithCheck(rightPoint, coordToIdMap, newConnectivity, newPoints);

    // Case 4
    operation->ComputeInteriorPoints(leftPoint, rightPoint, localNodes[4], localNodes[5]);
    localNodes.push_back(leftPoint);
    localNodes.push_back(rightPoint);
    InsertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);
    InsertPointWithCheck(rightPoint, coordToIdMap, newConnectivity, newPoints);

    // Case 5
    operation->ComputeInteriorPoints(leftPoint, rightPoint, localNodes[5], localNodes[6]);
    localNodes.push_back(leftPoint);
    localNodes.push_back(rightPoint);
    InsertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);
    InsertPointWithCheck(rightPoint, coordToIdMap, newConnectivity, newPoints);

    // Case 6
    operation->ComputeInteriorPoints(leftPoint, rightPoint, localNodes[6], localNodes[7]);
    localNodes.push_back(leftPoint);
    localNodes.push_back(rightPoint);
    InsertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);
    InsertPointWithCheck(rightPoint, coordToIdMap, newConnectivity, newPoints);

    // Case 7
    operation->ComputeInteriorPoints(leftPoint, rightPoint, localNodes[7], localNodes[4]);
    localNodes.push_back(leftPoint);
    localNodes.push_back(rightPoint);
    InsertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);
    InsertPointWithCheck(rightPoint, coordToIdMap, newConnectivity, newPoints);

    // Case 8
    operation->ComputeLeftPoint(leftPoint, localNodes[0], localNodes[4]);
    localNodes.push_back(leftPoint);
    InsertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);

    // Case 9
    operation->ComputeLeftPoint(leftPoint, localNodes[1], localNodes[5]);
    localNodes.push_back(leftPoint);
    InsertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);

    // Case 10
    operation->ComputeLeftPoint(leftPoint, localNodes[2], localNodes[6]);
    localNodes.push_back(leftPoint);
    InsertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);

    // Case 11
    operation->ComputeLeftPoint(leftPoint, localNodes[3], localNodes[7]);
    localNodes.push_back(leftPoint);
    InsertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);

    // Case 12
    operation->ComputeRightPoint(leftPoint, localNodes[0], localNodes[4]);
    localNodes.push_back(leftPoint);
    InsertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);

    // Case 13
    operation->ComputeRightPoint(leftPoint, localNodes[1], localNodes[5]);
    localNodes.push_back(leftPoint);
    InsertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);

    // Case 14
    operation->ComputeRightPoint(leftPoint, localNodes[2], localNodes[6]);
    localNodes.push_back(leftPoint);
    InsertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);

    // Case 15
    operation->ComputeRightPoint(leftPoint, localNodes[3], localNodes[7]);
    localNodes.push_back(leftPoint);
    InsertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);

    // Case 16
    operation->ComputeInteriorPoints(leftPoint, rightPoint, localNodes[27], localNodes[24]);
    localNodes.push_back(leftPoint);
    localNodes.push_back(rightPoint);
    InsertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);
    InsertPointWithCheck(rightPoint, coordToIdMap, newConnectivity, newPoints);

    // Case 17
    operation->ComputeInteriorPoints(leftPoint, rightPoint, localNodes[25], localNodes[26]);
    localNodes.push_back(leftPoint);
    localNodes.push_back(rightPoint);
    InsertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);
    InsertPointWithCheck(rightPoint, coordToIdMap, newConnectivity, newPoints);

    // Case 18
    operation->ComputeInteriorPoints(leftPoint, rightPoint, localNodes[24], localNodes[25]);
    localNodes.push_back(leftPoint);
    localNodes.push_back(rightPoint);
    InsertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);
    InsertPointWithCheck(rightPoint, coordToIdMap, newConnectivity, newPoints);

    // Case 19
    operation->ComputeInteriorPoints(leftPoint, rightPoint, localNodes[26], localNodes[27]);
    localNodes.push_back(leftPoint);
    localNodes.push_back(rightPoint);
    InsertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);
    InsertPointWithCheck(rightPoint, coordToIdMap, newConnectivity, newPoints);

    // Case 20
    operation->ComputeInteriorPoints(leftPoint, rightPoint, localNodes[31], localNodes[28]);
    localNodes.push_back(leftPoint);
    localNodes.push_back(rightPoint);
    InsertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);
    InsertPointWithCheck(rightPoint, coordToIdMap, newConnectivity, newPoints);

    // Case 21
    operation->ComputeInteriorPoints(leftPoint, rightPoint, localNodes[29], localNodes[30]);
    localNodes.push_back(leftPoint);
    localNodes.push_back(rightPoint);
    InsertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);
    InsertPointWithCheck(rightPoint, coordToIdMap, newConnectivity, newPoints);

    // Case 22
    operation->ComputeInteriorPoints(leftPoint, rightPoint, localNodes[28], localNodes[29]);
    InsertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);
    InsertPointWithCheck(rightPoint, coordToIdMap, newConnectivity, newPoints);

    // Case 23
    operation->ComputeInteriorPoints(leftPoint, rightPoint, localNodes[30], localNodes[31]);
    InsertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);
    InsertPointWithCheck(rightPoint, coordToIdMap, newConnectivity, newPoints);

    // Case 24
    operation->ComputeInteriorPoints(leftPoint, rightPoint, localNodes[15], localNodes[10]);
    InsertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);
    InsertPointWithCheck(rightPoint, coordToIdMap, newConnectivity, newPoints);

    // Case 25
    operation->ComputeInteriorPoints(leftPoint, rightPoint, localNodes[11], localNodes[14]);
    InsertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);
    InsertPointWithCheck(rightPoint, coordToIdMap, newConnectivity, newPoints);

    // Case 26
    operation->ComputeInteriorPoints(leftPoint, rightPoint, localNodes[23], localNodes[18]);
    InsertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);
    InsertPointWithCheck(rightPoint, coordToIdMap, newConnectivity, newPoints);

    // Case 27
    operation->ComputeInteriorPoints(leftPoint, rightPoint, localNodes[19], localNodes[22]);
    InsertPointWithCheck(leftPoint, coordToIdMap, newConnectivity, newPoints);
    InsertPointWithCheck(rightPoint, coordToIdMap, newConnectivity, newPoints);

    // Case 28
    operation->ComputeInteriorPoints(leftPoint, rightPoint, localNodes[33], localNodes[34]);
    InsertPointWithoutCheck(leftPoint, newConnectivity, newPoints);
    InsertPointWithoutCheck(rightPoint, newConnectivity, newPoints);

    // Case 29
    operation->ComputeInteriorPoints(leftPoint, rightPoint, localNodes[35], localNodes[32]);
    InsertPointWithoutCheck(leftPoint, newConnectivity, newPoints);
    InsertPointWithoutCheck(rightPoint, newConnectivity, newPoints);

    // Case 30
    operation->ComputeInteriorPoints(leftPoint, rightPoint, localNodes[41], localNodes[42]);
    InsertPointWithoutCheck(leftPoint, newConnectivity, newPoints);
    InsertPointWithoutCheck(rightPoint, newConnectivity, newPoints);

    // Case 31
    operation->ComputeInteriorPoints(leftPoint, rightPoint, localNodes[43], localNodes[40]);
    InsertPointWithoutCheck(leftPoint, newConnectivity, newPoints);
    InsertPointWithoutCheck(rightPoint, newConnectivity, newPoints);
  }

  delete operation;

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

  newGrid->GetTopology()->SetTopologyType(XDMF_HEX_64);
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

  newGrid->Insert(elementGenerationType);

  return newGrid;
}

XdmfGrid * XdmfHex64Generator::Split(XdmfGrid * grid, XdmfElement * parentElement)
{
  if(grid->GetTopology()->GetTopologyType() != XDMF_HEX_64)
  {
    std::cout << "This requires a Tri-Cubic Hexahedron Grid" << std::endl;
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
  newTopology->SetNumberOfElements(grid->GetTopology()->GetNumberOfElements() * 27);
  newTopology->SetDeleteOnGridDelete(true);

  XdmfArray * newConnectivity = newTopology->GetConnectivity();
  newConnectivity->SetNumberType(grid->GetTopology()->GetConnectivity()->GetNumberType());
  newConnectivity->SetNumberOfElements(newGrid->GetTopology()->GetNumberOfElements() * newGrid->GetTopology()->GetNodesPerElement());

  XdmfArray * oldConnectivity = grid->GetTopology()->GetConnectivity();

  for(int i=0; i<grid->GetTopology()->GetNumberOfElements(); ++i)
  {
    newConnectivity->SetValues(i*216 + 0, oldConnectivity, 1, 0 + 64 * i);
    newConnectivity->SetValues(i*216 + 1, oldConnectivity, 1, 8 + 64 * i);
    newConnectivity->SetValues(i*216 + 2, oldConnectivity, 1, 48 + 64 * i);
    newConnectivity->SetValues(i*216 + 3, oldConnectivity, 1, 15 + 64 * i);
    newConnectivity->SetValues(i*216 + 4, oldConnectivity, 1, 24 + 64 * i);
    newConnectivity->SetValues(i*216 + 5, oldConnectivity, 1, 36 + 64 * i);
    newConnectivity->SetValues(i*216 + 6, oldConnectivity, 1, 56 + 64 * i);
    newConnectivity->SetValues(i*216 + 7, oldConnectivity, 1, 33 + 64 * i);
    newConnectivity->SetValues(i*216 + 8, oldConnectivity, 1, 8 + 64 * i);
    newConnectivity->SetValues(i*216 + 9, oldConnectivity, 1, 9 + 64 * i);
    newConnectivity->SetValues(i*216 + 10, oldConnectivity, 1, 49 + 64 * i);
    newConnectivity->SetValues(i*216 + 11, oldConnectivity, 1, 48 + 64 * i);
    newConnectivity->SetValues(i*216 + 12, oldConnectivity, 1, 36 + 64 * i);
    newConnectivity->SetValues(i*216 + 13, oldConnectivity, 1, 37 + 64 * i);
    newConnectivity->SetValues(i*216 + 14, oldConnectivity, 1, 57 + 64 * i);
    newConnectivity->SetValues(i*216 + 15, oldConnectivity, 1, 56 + 64 * i);
    newConnectivity->SetValues(i*216 + 16, oldConnectivity, 1, 9 + 64 * i);
    newConnectivity->SetValues(i*216 + 17, oldConnectivity, 1, 1 + 64 * i);
    newConnectivity->SetValues(i*216 + 18, oldConnectivity, 1, 10 + 64 * i);
    newConnectivity->SetValues(i*216 + 19, oldConnectivity, 1, 49 + 64 * i);
    newConnectivity->SetValues(i*216 + 20, oldConnectivity, 1, 37 + 64 * i);
    newConnectivity->SetValues(i*216 + 21, oldConnectivity, 1, 25 + 64 * i);
    newConnectivity->SetValues(i*216 + 22, oldConnectivity, 1, 34 + 64 * i);
    newConnectivity->SetValues(i*216 + 23, oldConnectivity, 1, 57 + 64 * i);
    newConnectivity->SetValues(i*216 + 24, oldConnectivity, 1, 15 + 64 * i);
    newConnectivity->SetValues(i*216 + 25, oldConnectivity, 1, 48 + 64 * i);
    newConnectivity->SetValues(i*216 + 26, oldConnectivity, 1, 51 + 64 * i);
    newConnectivity->SetValues(i*216 + 27, oldConnectivity, 1, 14 + 64 * i);
    newConnectivity->SetValues(i*216 + 28, oldConnectivity, 1, 33 + 64 * i);
    newConnectivity->SetValues(i*216 + 29, oldConnectivity, 1, 56 + 64 * i);
    newConnectivity->SetValues(i*216 + 30, oldConnectivity, 1, 59 + 64 * i);
    newConnectivity->SetValues(i*216 + 31, oldConnectivity, 1, 32 + 64 * i);
    newConnectivity->SetValues(i*216 + 32, oldConnectivity, 1, 48 + 64 * i);
    newConnectivity->SetValues(i*216 + 33, oldConnectivity, 1, 49 + 64 * i);
    newConnectivity->SetValues(i*216 + 34, oldConnectivity, 1, 50 + 64 * i);
    newConnectivity->SetValues(i*216 + 35, oldConnectivity, 1, 51 + 64 * i);
    newConnectivity->SetValues(i*216 + 36, oldConnectivity, 1, 56 + 64 * i);
    newConnectivity->SetValues(i*216 + 37, oldConnectivity, 1, 57 + 64 * i);
    newConnectivity->SetValues(i*216 + 38, oldConnectivity, 1, 58 + 64 * i);
    newConnectivity->SetValues(i*216 + 39, oldConnectivity, 1, 59 + 64 * i);
    newConnectivity->SetValues(i*216 + 40, oldConnectivity, 1, 49 + 64 * i);
    newConnectivity->SetValues(i*216 + 41, oldConnectivity, 1, 10 + 64 * i);
    newConnectivity->SetValues(i*216 + 42, oldConnectivity, 1, 11 + 64 * i);
    newConnectivity->SetValues(i*216 + 43, oldConnectivity, 1, 50 + 64 * i);
    newConnectivity->SetValues(i*216 + 44, oldConnectivity, 1, 57 + 64 * i);
    newConnectivity->SetValues(i*216 + 45, oldConnectivity, 1, 34 + 64 * i);
    newConnectivity->SetValues(i*216 + 46, oldConnectivity, 1, 35 + 64 * i);
    newConnectivity->SetValues(i*216 + 47, oldConnectivity, 1, 58 + 64 * i);
    newConnectivity->SetValues(i*216 + 48, oldConnectivity, 1, 14 + 64 * i);
    newConnectivity->SetValues(i*216 + 49, oldConnectivity, 1, 51 + 64 * i);
    newConnectivity->SetValues(i*216 + 50, oldConnectivity, 1, 13 + 64 * i);
    newConnectivity->SetValues(i*216 + 51, oldConnectivity, 1, 3 + 64 * i);
    newConnectivity->SetValues(i*216 + 52, oldConnectivity, 1, 32 + 64 * i);
    newConnectivity->SetValues(i*216 + 53, oldConnectivity, 1, 59 + 64 * i);
    newConnectivity->SetValues(i*216 + 54, oldConnectivity, 1, 39 + 64 * i);
    newConnectivity->SetValues(i*216 + 55, oldConnectivity, 1, 27 + 64 * i);
    newConnectivity->SetValues(i*216 + 56, oldConnectivity, 1, 51 + 64 * i);
    newConnectivity->SetValues(i*216 + 57, oldConnectivity, 1, 50 + 64 * i);
    newConnectivity->SetValues(i*216 + 58, oldConnectivity, 1, 12 + 64 * i);
    newConnectivity->SetValues(i*216 + 59, oldConnectivity, 1, 13 + 64 * i);
    newConnectivity->SetValues(i*216 + 60, oldConnectivity, 1, 59 + 64 * i);
    newConnectivity->SetValues(i*216 + 61, oldConnectivity, 1, 58 + 64 * i);
    newConnectivity->SetValues(i*216 + 62, oldConnectivity, 1, 38 + 64 * i);
    newConnectivity->SetValues(i*216 + 63, oldConnectivity, 1, 39 + 64 * i);
    newConnectivity->SetValues(i*216 + 64, oldConnectivity, 1, 50 + 64 * i);
    newConnectivity->SetValues(i*216 + 65, oldConnectivity, 1, 11 + 64 * i);
    newConnectivity->SetValues(i*216 + 66, oldConnectivity, 1, 2 + 64 * i);
    newConnectivity->SetValues(i*216 + 67, oldConnectivity, 1, 12 + 64 * i);
    newConnectivity->SetValues(i*216 + 68, oldConnectivity, 1, 58 + 64 * i);
    newConnectivity->SetValues(i*216 + 69, oldConnectivity, 1, 35 + 64 * i);
    newConnectivity->SetValues(i*216 + 70, oldConnectivity, 1, 26 + 64 * i);
    newConnectivity->SetValues(i*216 + 71, oldConnectivity, 1, 38 + 64 * i);
    newConnectivity->SetValues(i*216 + 72, oldConnectivity, 1, 24 + 64 * i);
    newConnectivity->SetValues(i*216 + 73, oldConnectivity, 1, 36 + 64 * i);
    newConnectivity->SetValues(i*216 + 74, oldConnectivity, 1, 56 + 64 * i);
    newConnectivity->SetValues(i*216 + 75, oldConnectivity, 1, 33 + 64 * i);
    newConnectivity->SetValues(i*216 + 76, oldConnectivity, 1, 28 + 64 * i);
    newConnectivity->SetValues(i*216 + 77, oldConnectivity, 1, 44 + 64 * i);
    newConnectivity->SetValues(i*216 + 78, oldConnectivity, 1, 60 + 64 * i);
    newConnectivity->SetValues(i*216 + 79, oldConnectivity, 1, 41 + 64 * i);
    newConnectivity->SetValues(i*216 + 80, oldConnectivity, 1, 36 + 64 * i);
    newConnectivity->SetValues(i*216 + 81, oldConnectivity, 1, 37 + 64 * i);
    newConnectivity->SetValues(i*216 + 82, oldConnectivity, 1, 57 + 64 * i);
    newConnectivity->SetValues(i*216 + 83, oldConnectivity, 1, 56 + 64 * i);
    newConnectivity->SetValues(i*216 + 84, oldConnectivity, 1, 44 + 64 * i);
    newConnectivity->SetValues(i*216 + 85, oldConnectivity, 1, 45 + 64 * i);
    newConnectivity->SetValues(i*216 + 86, oldConnectivity, 1, 61 + 64 * i);
    newConnectivity->SetValues(i*216 + 87, oldConnectivity, 1, 60 + 64 * i);
    newConnectivity->SetValues(i*216 + 88, oldConnectivity, 1, 37 + 64 * i);
    newConnectivity->SetValues(i*216 + 89, oldConnectivity, 1, 25 + 64 * i);
    newConnectivity->SetValues(i*216 + 90, oldConnectivity, 1, 34 + 64 * i);
    newConnectivity->SetValues(i*216 + 91, oldConnectivity, 1, 57 + 64 * i);
    newConnectivity->SetValues(i*216 + 92, oldConnectivity, 1, 45 + 64 * i);
    newConnectivity->SetValues(i*216 + 93, oldConnectivity, 1, 29 + 64 * i);
    newConnectivity->SetValues(i*216 + 94, oldConnectivity, 1, 42 + 64 * i);
    newConnectivity->SetValues(i*216 + 95, oldConnectivity, 1, 61 + 64 * i);
    newConnectivity->SetValues(i*216 + 96, oldConnectivity, 1, 33 + 64 * i);
    newConnectivity->SetValues(i*216 + 97, oldConnectivity, 1, 56 + 64 * i);
    newConnectivity->SetValues(i*216 + 98, oldConnectivity, 1, 59 + 64 * i);
    newConnectivity->SetValues(i*216 + 99, oldConnectivity, 1, 32 + 64 * i);
    newConnectivity->SetValues(i*216 + 100, oldConnectivity, 1, 41 + 64 * i);
    newConnectivity->SetValues(i*216 + 101, oldConnectivity, 1, 60 + 64 * i);
    newConnectivity->SetValues(i*216 + 102, oldConnectivity, 1, 63 + 64 * i);
    newConnectivity->SetValues(i*216 + 103, oldConnectivity, 1, 40 + 64 * i);
    newConnectivity->SetValues(i*216 + 104, oldConnectivity, 1, 56 + 64 * i);
    newConnectivity->SetValues(i*216 + 105, oldConnectivity, 1, 57 + 64 * i);
    newConnectivity->SetValues(i*216 + 106, oldConnectivity, 1, 58 + 64 * i);
    newConnectivity->SetValues(i*216 + 107, oldConnectivity, 1, 59 + 64 * i);
    newConnectivity->SetValues(i*216 + 108, oldConnectivity, 1, 60 + 64 * i);
    newConnectivity->SetValues(i*216 + 109, oldConnectivity, 1, 61 + 64 * i);
    newConnectivity->SetValues(i*216 + 110, oldConnectivity, 1, 62 + 64 * i);
    newConnectivity->SetValues(i*216 + 111, oldConnectivity, 1, 63 + 64 * i);
    newConnectivity->SetValues(i*216 + 112, oldConnectivity, 1, 57 + 64 * i);
    newConnectivity->SetValues(i*216 + 113, oldConnectivity, 1, 34 + 64 * i);
    newConnectivity->SetValues(i*216 + 114, oldConnectivity, 1, 35 + 64 * i);
    newConnectivity->SetValues(i*216 + 115, oldConnectivity, 1, 58 + 64 * i);
    newConnectivity->SetValues(i*216 + 116, oldConnectivity, 1, 61 + 64 * i);
    newConnectivity->SetValues(i*216 + 117, oldConnectivity, 1, 42 + 64 * i);
    newConnectivity->SetValues(i*216 + 118, oldConnectivity, 1, 43 + 64 * i);
    newConnectivity->SetValues(i*216 + 119, oldConnectivity, 1, 62 + 64 * i);
    newConnectivity->SetValues(i*216 + 120, oldConnectivity, 1, 32 + 64 * i);
    newConnectivity->SetValues(i*216 + 121, oldConnectivity, 1, 59 + 64 * i);
    newConnectivity->SetValues(i*216 + 122, oldConnectivity, 1, 39 + 64 * i);
    newConnectivity->SetValues(i*216 + 123, oldConnectivity, 1, 27 + 64 * i);
    newConnectivity->SetValues(i*216 + 124, oldConnectivity, 1, 40 + 64 * i);
    newConnectivity->SetValues(i*216 + 125, oldConnectivity, 1, 63 + 64 * i);
    newConnectivity->SetValues(i*216 + 126, oldConnectivity, 1, 47 + 64 * i);
    newConnectivity->SetValues(i*216 + 127, oldConnectivity, 1, 31 + 64 * i);
    newConnectivity->SetValues(i*216 + 128, oldConnectivity, 1, 59 + 64 * i);
    newConnectivity->SetValues(i*216 + 129, oldConnectivity, 1, 58 + 64 * i);
    newConnectivity->SetValues(i*216 + 130, oldConnectivity, 1, 38 + 64 * i);
    newConnectivity->SetValues(i*216 + 131, oldConnectivity, 1, 39 + 64 * i);
    newConnectivity->SetValues(i*216 + 132, oldConnectivity, 1, 63 + 64 * i);
    newConnectivity->SetValues(i*216 + 133, oldConnectivity, 1, 62 + 64 * i);
    newConnectivity->SetValues(i*216 + 134, oldConnectivity, 1, 46 + 64 * i);
    newConnectivity->SetValues(i*216 + 135, oldConnectivity, 1, 47 + 64 * i);
    newConnectivity->SetValues(i*216 + 136, oldConnectivity, 1, 58 + 64 * i);
    newConnectivity->SetValues(i*216 + 137, oldConnectivity, 1, 35 + 64 * i);
    newConnectivity->SetValues(i*216 + 138, oldConnectivity, 1, 26 + 64 * i);
    newConnectivity->SetValues(i*216 + 139, oldConnectivity, 1, 38 + 64 * i);
    newConnectivity->SetValues(i*216 + 140, oldConnectivity, 1, 62 + 64 * i);
    newConnectivity->SetValues(i*216 + 141, oldConnectivity, 1, 43 + 64 * i);
    newConnectivity->SetValues(i*216 + 142, oldConnectivity, 1, 30 + 64 * i);
    newConnectivity->SetValues(i*216 + 143, oldConnectivity, 1, 46 + 64 * i);
    newConnectivity->SetValues(i*216 + 144, oldConnectivity, 1, 28 + 64 * i);
    newConnectivity->SetValues(i*216 + 145, oldConnectivity, 1, 44 + 64 * i);
    newConnectivity->SetValues(i*216 + 146, oldConnectivity, 1, 60 + 64 * i);
    newConnectivity->SetValues(i*216 + 147, oldConnectivity, 1, 41 + 64 * i);
    newConnectivity->SetValues(i*216 + 148, oldConnectivity, 1, 4 + 64 * i);
    newConnectivity->SetValues(i*216 + 149, oldConnectivity, 1, 16 + 64 * i);
    newConnectivity->SetValues(i*216 + 150, oldConnectivity, 1, 52 + 64 * i);
    newConnectivity->SetValues(i*216 + 151, oldConnectivity, 1, 23 + 64 * i);
    newConnectivity->SetValues(i*216 + 152, oldConnectivity, 1, 44 + 64 * i);
    newConnectivity->SetValues(i*216 + 153, oldConnectivity, 1, 45 + 64 * i);
    newConnectivity->SetValues(i*216 + 154, oldConnectivity, 1, 61 + 64 * i);
    newConnectivity->SetValues(i*216 + 155, oldConnectivity, 1, 60 + 64 * i);
    newConnectivity->SetValues(i*216 + 156, oldConnectivity, 1, 16 + 64 * i);
    newConnectivity->SetValues(i*216 + 157, oldConnectivity, 1, 17 + 64 * i);
    newConnectivity->SetValues(i*216 + 158, oldConnectivity, 1, 53 + 64 * i);
    newConnectivity->SetValues(i*216 + 159, oldConnectivity, 1, 52 + 64 * i);
    newConnectivity->SetValues(i*216 + 160, oldConnectivity, 1, 45 + 64 * i);
    newConnectivity->SetValues(i*216 + 161, oldConnectivity, 1, 29 + 64 * i);
    newConnectivity->SetValues(i*216 + 162, oldConnectivity, 1, 42 + 64 * i);
    newConnectivity->SetValues(i*216 + 163, oldConnectivity, 1, 61 + 64 * i);
    newConnectivity->SetValues(i*216 + 164, oldConnectivity, 1, 17 + 64 * i);
    newConnectivity->SetValues(i*216 + 165, oldConnectivity, 1, 5 + 64 * i);
    newConnectivity->SetValues(i*216 + 166, oldConnectivity, 1, 18 + 64 * i);
    newConnectivity->SetValues(i*216 + 167, oldConnectivity, 1, 53 + 64 * i);
    newConnectivity->SetValues(i*216 + 168, oldConnectivity, 1, 41 + 64 * i);
    newConnectivity->SetValues(i*216 + 169, oldConnectivity, 1, 60 + 64 * i);
    newConnectivity->SetValues(i*216 + 170, oldConnectivity, 1, 63 + 64 * i);
    newConnectivity->SetValues(i*216 + 171, oldConnectivity, 1, 40 + 64 * i);
    newConnectivity->SetValues(i*216 + 172, oldConnectivity, 1, 23 + 64 * i);
    newConnectivity->SetValues(i*216 + 173, oldConnectivity, 1, 52 + 64 * i);
    newConnectivity->SetValues(i*216 + 174, oldConnectivity, 1, 55 + 64 * i);
    newConnectivity->SetValues(i*216 + 175, oldConnectivity, 1, 22 + 64 * i);
    newConnectivity->SetValues(i*216 + 176, oldConnectivity, 1, 60 + 64 * i);
    newConnectivity->SetValues(i*216 + 177, oldConnectivity, 1, 61 + 64 * i);
    newConnectivity->SetValues(i*216 + 178, oldConnectivity, 1, 62 + 64 * i);
    newConnectivity->SetValues(i*216 + 179, oldConnectivity, 1, 63 + 64 * i);
    newConnectivity->SetValues(i*216 + 180, oldConnectivity, 1, 52 + 64 * i);
    newConnectivity->SetValues(i*216 + 181, oldConnectivity, 1, 53 + 64 * i);
    newConnectivity->SetValues(i*216 + 182, oldConnectivity, 1, 54 + 64 * i);
    newConnectivity->SetValues(i*216 + 183, oldConnectivity, 1, 55 + 64 * i);
    newConnectivity->SetValues(i*216 + 184, oldConnectivity, 1, 61 + 64 * i);
    newConnectivity->SetValues(i*216 + 185, oldConnectivity, 1, 42 + 64 * i);
    newConnectivity->SetValues(i*216 + 186, oldConnectivity, 1, 43 + 64 * i);
    newConnectivity->SetValues(i*216 + 187, oldConnectivity, 1, 62 + 64 * i);
    newConnectivity->SetValues(i*216 + 188, oldConnectivity, 1, 53 + 64 * i);
    newConnectivity->SetValues(i*216 + 189, oldConnectivity, 1, 18 + 64 * i);
    newConnectivity->SetValues(i*216 + 190, oldConnectivity, 1, 19 + 64 * i);
    newConnectivity->SetValues(i*216 + 191, oldConnectivity, 1, 54 + 64 * i);
    newConnectivity->SetValues(i*216 + 192, oldConnectivity, 1, 40 + 64 * i);
    newConnectivity->SetValues(i*216 + 193, oldConnectivity, 1, 63 + 64 * i);
    newConnectivity->SetValues(i*216 + 194, oldConnectivity, 1, 47 + 64 * i);
    newConnectivity->SetValues(i*216 + 195, oldConnectivity, 1, 31 + 64 * i);
    newConnectivity->SetValues(i*216 + 196, oldConnectivity, 1, 22 + 64 * i);
    newConnectivity->SetValues(i*216 + 197, oldConnectivity, 1, 55 + 64 * i);
    newConnectivity->SetValues(i*216 + 198, oldConnectivity, 1, 21 + 64 * i);
    newConnectivity->SetValues(i*216 + 199, oldConnectivity, 1, 7 + 64 * i);
    newConnectivity->SetValues(i*216 + 200, oldConnectivity, 1, 63 + 64 * i);
    newConnectivity->SetValues(i*216 + 201, oldConnectivity, 1, 62 + 64 * i);
    newConnectivity->SetValues(i*216 + 202, oldConnectivity, 1, 46 + 64 * i);
    newConnectivity->SetValues(i*216 + 203, oldConnectivity, 1, 47 + 64 * i);
    newConnectivity->SetValues(i*216 + 204, oldConnectivity, 1, 55 + 64 * i);
    newConnectivity->SetValues(i*216 + 205, oldConnectivity, 1, 54 + 64 * i);
    newConnectivity->SetValues(i*216 + 206, oldConnectivity, 1, 20 + 64 * i);
    newConnectivity->SetValues(i*216 + 207, oldConnectivity, 1, 21 + 64 * i);
    newConnectivity->SetValues(i*216 + 208, oldConnectivity, 1, 62 + 64 * i);
    newConnectivity->SetValues(i*216 + 209, oldConnectivity, 1, 43 + 64 * i);
    newConnectivity->SetValues(i*216 + 210, oldConnectivity, 1, 30 + 64 * i);
    newConnectivity->SetValues(i*216 + 211, oldConnectivity, 1, 46 + 64 * i);
    newConnectivity->SetValues(i*216 + 212, oldConnectivity, 1, 54 + 64 * i);
    newConnectivity->SetValues(i*216 + 213, oldConnectivity, 1, 19 + 64 * i);
    newConnectivity->SetValues(i*216 + 214, oldConnectivity, 1, 6 + 64 * i);
    newConnectivity->SetValues(i*216 + 215, oldConnectivity, 1, 20 + 64 * i);
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
      vals->SetNumberOfElements(currAttribute->GetValues()->GetNumberOfElements() * 27);
      for(int j=0; j<currAttribute->GetValues()->GetNumberOfElements(); ++j)
      {

        vals->SetValues(j*27, currAttribute->GetValues(), 27, j, 1, 0);
      }
      newGrid->Insert(newAttribute);
    }
  }

  return newGrid;
}
