#include <math.h>
#include "stdio.h"
#include "string.h"
#include "assert.h"
#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
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
  unsigned int i = 0;

  XDMFTOPOLOGYCONVERTER * converter =
    XdmfTopologyConverterNew();

  // Create Hexahedron Grid
  XDMFUNSTRUCTUREDGRID * hexGrid =
    XdmfUnstructuredGridNew();
  double hexPoints[24] = {0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0,
                          1, 1, 1, 1, 0, 1, 1};
  int hexdims[1] = {24};
  XDMFGEOMETRY * hexGeometry = XdmfUnstructuredGridGetGeometry(hexGrid);
  XdmfGeometrySetType(hexGeometry, XDMF_GEOMETRY_TYPE_XYZ, &status);
  XdmfGeometryResize(hexGeometry, hexdims, 1, XDMF_ARRAY_TYPE_FLOAT64, &status);
  XdmfGeometryInsertDataFromPointer(hexGeometry, hexPoints, XDMF_ARRAY_TYPE_FLOAT64, 0, 24, 1, 1, &status);
  XDMFTOPOLOGY * hexTopology = XdmfUnstructuredGridGetTopology(hexGrid);
  unsigned int hexConn[8] = {0, 1, 2, 3, 4, 5, 6, 7};
  hexdims[0] = 8;
  XdmfTopologySetType(hexTopology, XDMF_TOPOLOGY_TYPE_HEXAHEDRON, &status);
  XdmfTopologyResize(hexTopology, hexdims, 1, XDMF_ARRAY_TYPE_UINT32, &status);
  XdmfTopologyInsertDataFromPointer(hexTopology, hexConn, XDMF_ARRAY_TYPE_UINT32, 0, 8, 1, 1, &status);

  /*
   * Hexahedron to Hexahedron_64
   */
  XDMFUNSTRUCTUREDGRID * hex64Grid =
    XdmfTopologyConverterConvert(converter, hexGrid, XDMF_TOPOLOGY_TYPE_HEXAHEDRON_64, NULL);
  XDMFGEOMETRY * hex64Geometry = XdmfUnstructuredGridGetGeometry(hex64Grid);
  assert(XdmfGeometryGetType(hex64Geometry) == XDMF_GEOMETRY_TYPE_XYZ);
  assert(XdmfGeometryGetNumberPoints(hex64Geometry) == 64);
  XDMFTOPOLOGY * hex64Topology = XdmfUnstructuredGridGetTopology(hex64Grid);
  assert(XdmfTopologyGetType(hex64Topology) ==
         XDMF_TOPOLOGY_TYPE_HEXAHEDRON_64);
  assert(XdmfTopologyGetNumberElements(hex64Topology, &status) == 1);
  for(i = 0; i < 64; ++i) {
    assert(i == ((unsigned int *)XdmfTopologyGetValue(hex64Topology, i, XDMF_ARRAY_TYPE_UINT32, &status))[0]);
  }

  /*
   * Hexahedron to Hexahedron_125
   */
  XDMFUNSTRUCTUREDGRID * hex125Grid =
    XdmfTopologyConverterConvert(converter, hexGrid, XDMF_TOPOLOGY_TYPE_HEXAHEDRON_125, NULL);

  XDMFGEOMETRY * hex125Geometry = XdmfUnstructuredGridGetGeometry(hex125Grid);
  assert(XdmfGeometryGetType(hex125Geometry) == XDMF_GEOMETRY_TYPE_XYZ);
  assert(XdmfGeometryGetNumberPoints(hex125Geometry) == 125);
  XDMFTOPOLOGY * hex125Topology = XdmfUnstructuredGridGetTopology(hex125Grid);
  assert(XdmfTopologyGetType(hex125Topology) ==
         XDMF_TOPOLOGY_TYPE_HEXAHEDRON_125);  
  assert(XdmfTopologyGetNumberElements(hex125Topology, &status) == 1);
  for(i=0; i<125; ++i) {
    assert(i == ((unsigned int *)XdmfTopologyGetValue(hex125Topology, i, XDMF_ARRAY_TYPE_UINT32, &status))[0]);
  }

  /*
   * Hexahedron_64 to Hexahedron
   */
  XDMFUNSTRUCTUREDGRID * newHexGrid =
    XdmfTopologyConverterConvert(converter, hex64Grid, XDMF_TOPOLOGY_TYPE_HEXAHEDRON, NULL);

  XDMFGEOMETRY * newHexGeometry = XdmfUnstructuredGridGetGeometry(newHexGrid);
  assert(XdmfGeometryGetType(newHexGeometry) == XDMF_GEOMETRY_TYPE_XYZ);
  assert(XdmfGeometryGetNumberPoints(newHexGeometry) == 64);
  XDMFTOPOLOGY * newHexTopology = XdmfUnstructuredGridGetTopology(newHexGrid);
  assert(XdmfTopologyGetType(newHexTopology) ==
         XDMF_TOPOLOGY_TYPE_HEXAHEDRON);
  assert(XdmfTopologyGetNumberElements(newHexTopology, &status) == 27);


  XDMFTOPOLOGY * faceTopology;

  /**
   * Tetrahedron to Triangle
   */
  XDMFTOPOLOGY * tetTopology = XdmfTopologyNew();
  XdmfTopologySetType(tetTopology, XDMF_TOPOLOGY_TYPE_TETRAHEDRON, &status);
  long tetValues[8] = {0, 1, 2, 3, 0, 1, 2, 4};// temporary
  XdmfTopologyInsertDataFromPointer(tetTopology, tetValues, XDMF_ARRAY_TYPE_INT64, 0, 8, 1, 1, &status);
  printf("tetrahedrons prior to splitting into faces \n%s\n", XdmfTopologyGetValuesString(tetTopology));
  faceTopology = XdmfTopologyConverterGetExternalFaces(converter, tetTopology);
  printf("after splitting into faces\n%s\n", XdmfTopologyGetValuesString(faceTopology));
  assert(strcmp(XdmfTopologyGetValuesString(faceTopology), "0 1 3 "
                                                           "0 3 2 "
                                                           "0 1 4 "
                                                           "0 4 2 "
                                                           "1 2 3 "
                                                           "1 2 4") == 0);

  XdmfTopologyFree(faceTopology);

  /**
   * Tetrahedron_10 to Triangle_6
   */
  XDMFTOPOLOGY * tet10Topology = XdmfTopologyNew();
  XdmfTopologySetType(tet10Topology, XDMF_TOPOLOGY_TYPE_TETRAHEDRON_10, &status);
  long tet10Values[20] = {0, 1, 2, 3, 5, 6,  7,  8,  9,  10,
                          0, 1, 2, 4, 9, 10, 11, 12, 13, 14};// temporary
  XdmfTopologyInsertDataFromPointer(tet10Topology, tet10Values, XDMF_ARRAY_TYPE_INT64, 0, 20, 1, 1, &status);
  printf("tetrahedron_10s prior to splitting into faces \n%s\n", XdmfTopologyGetValuesString(tetTopology));
  faceTopology = XdmfTopologyConverterGetExternalFaces(converter, tet10Topology);
  printf("after splitting into faces\n%s\n", XdmfTopologyGetValuesString(faceTopology));
  assert(strcmp(XdmfTopologyGetValuesString(faceTopology), "0 1 3 5 9 8 "
                                                           "0 3 2 8 10 7 "
                                                           "0 1 4 9 13 12 "
                                                           "0 4 2 12 14 11 "
                                                           "1 2 3 6 10 9 "
                                                           "1 2 4 10 14 13") == 0);

  XdmfTopologyFree(faceTopology);

  /**
   * Hexahedron to Quadrilateral
   */
  XDMFTOPOLOGY * baseHexTopology = XdmfTopologyNew();
  XdmfTopologySetType(baseHexTopology, XDMF_TOPOLOGY_TYPE_HEXAHEDRON, &status);
  long hexValues[16] = {0, 1, 2, 3, 4, 5, 6, 7,
                         0, 1, 2, 3, 8, 9, 10, 11};// temporary
  XdmfTopologyInsertDataFromPointer(baseHexTopology, hexValues, XDMF_ARRAY_TYPE_INT64, 0, 16, 1, 1, &status);
  printf("Hexahedrons prior to splitting into faces \n%s\n", XdmfTopologyGetValuesString(baseHexTopology));
  faceTopology = XdmfTopologyConverterGetExternalFaces(converter, baseHexTopology);
  printf("after splitting into faces\n%s\n", XdmfTopologyGetValuesString(faceTopology));
  assert(strcmp(XdmfTopologyGetValuesString(faceTopology), "0 1 5 4 "
                                                           "0 4 7 3 "
                                                           "0 1 9 8 "
                                                           "0 8 11 3 "
                                                           "1 2 6 5 "
                                                           "1 2 10 9 "
                                                           "2 3 7 6 "
                                                           "2 3 11 10 "
                                                           "4 5 6 7 "
                                                           "8 9 10 11") == 0);

  XdmfTopologyFree(faceTopology);

  /**
   * Hexahedron_20 to Quadrilateral_8
   */
  XDMFTOPOLOGY * hex20Topology = XdmfTopologyNew();
  XdmfTopologySetType(hex20Topology, XDMF_TOPOLOGY_TYPE_HEXAHEDRON_20, &status);
  long hex20Values[40] = {0, 1, 2, 3, 4, 5, 6,  7,  12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
                           0, 1, 2, 3, 8, 9, 10, 11, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35};// temporary
  XdmfTopologyInsertDataFromPointer(hex20Topology, hex20Values, XDMF_ARRAY_TYPE_INT64, 0, 40, 1, 1, &status);
  printf("Hexahedron_20s prior to splitting into faces \n%s\n", XdmfTopologyGetValuesString(hex20Topology));
  faceTopology = XdmfTopologyConverterGetExternalFaces(converter, hex20Topology);
  printf("after splitting into faces\n%s\n", XdmfTopologyGetValuesString(faceTopology));
  assert(strcmp(XdmfTopologyGetValuesString(faceTopology), "0 1 5 4 12 21 16 20 "
                                                           "0 4 7 3 20 19 23 15 "
                                                           "0 1 9 8 24 33 28 32 "
                                                           "0 8 11 3 32 31 35 27 "
                                                           "1 2 6 5 13 22 17 21 "
                                                           "1 2 10 9 25 34 29 33 "
                                                           "2 3 7 6 14 23 18 22 "
                                                           "2 3 11 10 26 35 30 34 "
                                                           "4 5 6 7 16 17 18 19 "
                                                           "8 9 10 11 28 29 30 31") == 0);

  XdmfTopologyFree(faceTopology);

  return 0;
}
