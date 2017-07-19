#include <math.h>
#include "stdio.h"
#include "string.h"
#include "assert.h"
#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfAttribute.hpp"
#include "XdmfAttributeCenter.hpp"
#include "XdmfAttributeType.hpp"
#include "XdmfDiff.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfGeometryType.hpp"
#include "XdmfTopology.hpp"
#include "XdmfTopologyConverter.hpp"
#include "XdmfTopologyType.hpp"
#include "XdmfUnstructuredGrid.hpp"
#include "XdmfWriter.hpp"

int main()
{
  int status = 0;

  XDMFUNSTRUCTUREDGRID * grid1 = XdmfUnstructuredGridNew();

  XDMFTOPOLOGY * topology1 = XdmfTopologyNew();

  int connectivity[16] = {0, 1, 7, 6, 3, 4, 10, 9, 1, 2, 8, 7, 4, 5, 11, 10};
  XdmfTopologySetType(topology1, XDMF_TOPOLOGY_TYPE_HEXAHEDRON, &status);
  XdmfTopologyInsertDataFromPointer(topology1, connectivity, XDMF_ARRAY_TYPE_INT32, 0, 16, 1, 1, &status);

  XdmfUnstructuredGridSetTopology(grid1, topology1, 1);
  XdmfTopologyFree(topology1);

  XDMFGEOMETRY * geometry1 = XdmfGeometryNew();

  double points[36] = {0.1, 0.1, 1.1, 1.1, 0.1, 1.1, 3.1, 0.1, 2.1, 0.1, 1.1,
                       1.1, 1.1, 1.1, 1.1, 3.1, 2.1, 2.1, 0.1, 0.1, -1.1, 1.1,
                       0.1, -1.1, 3.1, 0.1, -2.1, 0.1, 1.1, -1.1, 1.1, 1.1,
                       -1.1, 3.1, 2.1, -2.1};
  XdmfGeometrySetType(geometry1, XDMF_GEOMETRY_TYPE_XYZ, &status);
  XdmfGeometryInsertDataFromPointer(geometry1, points, XDMF_ARRAY_TYPE_FLOAT64, 0, 36, 1, 1, &status);

  XdmfUnstructuredGridSetGeometry(grid1, geometry1, 1);
  XdmfGeometryFree(geometry1);

  XDMFATTRIBUTE * attr1 = XdmfAttributeNew();

  int nodeValues[12] = {100, 200, 300, 300, 400, 500, 300, 400, 500, 500, 600,
                        700};
  XdmfAttributeSetName(attr1, "Nodal Attribute", &status);
  XdmfAttributeSetType(attr1, XDMF_ATTRIBUTE_TYPE_SCALAR, &status);
  XdmfAttributeSetCenter(attr1, XDMF_ATTRIBUTE_CENTER_NODE, &status);
  XdmfAttributeInsertDataFromPointer(attr1, nodeValues, XDMF_ARRAY_TYPE_INT32, 0, 12, 1, 1, &status);

  XdmfUnstructuredGridInsertAttribute(grid1, attr1, 1);
  XdmfAttributeFree(attr1);

  // Grid with the same structure

  XDMFUNSTRUCTUREDGRID * grid2 = XdmfUnstructuredGridNew();

  XDMFTOPOLOGY * topology2 = XdmfTopologyNew();

  XdmfTopologySetType(topology2, XDMF_TOPOLOGY_TYPE_HEXAHEDRON, &status);
  XdmfTopologyInsertDataFromPointer(topology2, connectivity, XDMF_ARRAY_TYPE_INT32, 0, 16, 1, 1, &status);

  XdmfUnstructuredGridSetTopology(grid2, topology2, 1);
  XdmfTopologyFree(topology2);

  XDMFGEOMETRY * geometry2 = XdmfGeometryNew();

  XdmfGeometrySetType(geometry2, XDMF_GEOMETRY_TYPE_XYZ, &status);
  XdmfGeometryInsertDataFromPointer(geometry2, points, XDMF_ARRAY_TYPE_FLOAT64, 0, 36, 1, 1, &status);

  XdmfUnstructuredGridSetGeometry(grid2, geometry2, 1);
  XdmfGeometryFree(geometry2);

  XDMFATTRIBUTE * attr2 = XdmfAttributeNew();

  XdmfAttributeSetName(attr2, "Nodal Attribute", &status);
  XdmfAttributeSetType(attr2, XDMF_ATTRIBUTE_TYPE_SCALAR, &status);
  XdmfAttributeSetCenter(attr2, XDMF_ATTRIBUTE_CENTER_NODE, &status);
  XdmfAttributeInsertDataFromPointer(attr2, nodeValues, XDMF_ARRAY_TYPE_INT32, 0, 12, 1, 1, &status);

  XdmfUnstructuredGridInsertAttribute(grid2, attr2, 1);
  XdmfAttributeFree(attr2);

  // Grid with structure partially off, difference of 5.

  XDMFUNSTRUCTUREDGRID * grid3 = XdmfUnstructuredGridNew();

  XDMFTOPOLOGY * topology3 = XdmfTopologyNew();

  XdmfTopologySetType(topology3, XDMF_TOPOLOGY_TYPE_HEXAHEDRON, &status);
  XdmfTopologyInsertDataFromPointer(topology3, connectivity, XDMF_ARRAY_TYPE_INT32, 0, 16, 1, 1, &status);

  XdmfUnstructuredGridSetTopology(grid3, topology3, 1);
  XdmfTopologyFree(topology3);

  XDMFGEOMETRY * geometry3 = XdmfGeometryNew();

  double pointsdiff[36] = {4.1, 0.1, 1.1, 1.1, 0.1, 3.1, 3.1, 0.1, 2.1, 0.1, 1.1,
                           1.1, 1.1, 1.1, 1.1, 6.1, 2.1, 2.1, 0.1, 0.1, -1.1, 1.1,
                           0.1, -1.1, 3.1, 3.1, -4.1, 0.1, 1.1, -1.1, 1.1, 1.1,
                           -1.1, 3.1, 2.1, -2.1};
  XdmfGeometrySetType(geometry3, XDMF_GEOMETRY_TYPE_XYZ, &status);
  XdmfGeometryInsertDataFromPointer(geometry3, pointsdiff, XDMF_ARRAY_TYPE_FLOAT64, 0, 36, 1, 1, &status);

  XdmfUnstructuredGridSetGeometry(grid3, geometry3, 1);
  XdmfGeometryFree(geometry3);

  XDMFATTRIBUTE * attr3 = XdmfAttributeNew();

  XdmfAttributeSetName(attr3, "Nodal Attribute", &status);
  XdmfAttributeSetType(attr3, XDMF_ATTRIBUTE_TYPE_SCALAR, &status);
  XdmfAttributeSetCenter(attr3, XDMF_ATTRIBUTE_CENTER_NODE, &status);
  XdmfAttributeInsertDataFromPointer(attr3, nodeValues, XDMF_ARRAY_TYPE_INT32, 0, 12, 1, 1, &status);

  XdmfUnstructuredGridInsertAttribute(grid3, attr3, 1);
  XdmfAttributeFree(attr3);

  // Make diff checks

  XDMFDIFF * diff = XdmfDiffNew();

  if (!XdmfDiffCompare(diff, (XDMFITEM *)grid1, (XDMFITEM *)grid2)) {
    printf("equivalent grids are not compared correctly\n");
  }
  assert(XdmfDiffCompare(diff, (XDMFITEM *)grid1, (XDMFITEM *)grid2));

  if (XdmfDiffCompare(diff, (XDMFITEM *)grid1, (XDMFITEM *)grid3)) {
    printf("dissimilar grids are not compared correctly\n");
  }
  assert(!XdmfDiffCompare(diff, (XDMFITEM *)grid1, (XDMFITEM *)grid3));

  printf("default tolerance = %lf\n", XdmfDiffGetAbsoluteTolerance(diff));

  XdmfDiffSetAbsoluteTolerance(diff, 6);

  if (!XdmfDiffCompare(diff, (XDMFITEM *)grid1, (XDMFITEM *)grid3)) {
    printf("tolerance is not applied correctly\n");
  }
  assert(XdmfDiffCompare(diff, (XDMFITEM *)grid1, (XDMFITEM *)grid3));

  XdmfGridFree(grid1);
  XdmfGridFree(grid2);
  XdmfGridFree(grid3);
  XdmfDiffFree(diff);

  return 0;
}
