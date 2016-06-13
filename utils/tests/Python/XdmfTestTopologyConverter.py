from Xdmf import *
from XdmfUtils import *

if __name__ == "__main__":
  converter = XdmfTopologyConverter.New()

  # Create Hexahedron Grid
  hexGrid = XdmfUnstructuredGrid.New();
  hexPoints = [0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0,
               1, 1, 1, 1, 0, 1, 1]
  hexGrid.getGeometry().setType(XdmfGeometryType.XYZ())
  hexGrid.getGeometry().resizeAsFloat64(24, 0)
  hexGrid.getGeometry().insertAsFloat64(0, hexPoints)
  hexConn = [0, 1, 2, 3, 4, 5, 6, 7]
  hexGrid.getTopology().setType(XdmfTopologyType.Hexahedron())
  hexGrid.getTopology().resizeAsUInt32(8, 0)
  hexGrid.getTopology().insertAsUInt32(0, hexConn)

  #
  # Hexahedron to Hexahedron_64
  #
  hex64Grid = converter.convert(hexGrid, XdmfTopologyType.Hexahedron_64())

  assert(hex64Grid.getGeometry().getType() == XdmfGeometryType.XYZ())
  assert(hex64Grid.getGeometry().getNumberPoints() == 64)
  assert(hex64Grid.getTopology().getType() ==
         XdmfTopologyType.Hexahedron_64())
  assert(hex64Grid.getTopology().getNumberElements() == 1)
  for i in range(64):
    assert(i == hex64Grid.getTopology().getValueAsUInt32(i))


  #
  # Hexahedron to Hexahedron_125
  #
  hex125Grid = converter.convert(hexGrid, XdmfTopologyType.Hexahedron_125())

  assert(hex125Grid.getGeometry().getType() == XdmfGeometryType.XYZ())
  assert(hex125Grid.getGeometry().getNumberPoints() == 125)
  assert(hex125Grid.getTopology().getType() ==
         XdmfTopologyType.Hexahedron_125())
  assert(hex125Grid.getTopology().getNumberElements() == 1)
  for i in range(125):
    assert(i == hex125Grid.getTopology().getValueAsUInt32(i))


  #
  # Hexahedron_64 to Hexahedron
  #
  newHexGrid = converter.convert(hex64Grid, XdmfTopologyType.Hexahedron())
  assert(newHexGrid.getGeometry().getType() == XdmfGeometryType.XYZ());
  assert(newHexGrid.getGeometry().getNumberPoints() == 64);
  assert(newHexGrid.getTopology().getType() ==
         XdmfTopologyType.Hexahedron());
  assert(newHexGrid.getTopology().getNumberElements() == 27);

  #
  # Tetrahedron to Triangle
  #
  tetTopology = XdmfTopology.New()
  tetTopology.setType(XdmfTopologyType.Tetrahedron())
  tetValues = [0, 1, 2, 3, 0, 1, 2, 4] # temporary
  tetTopology.insertAsInt64(0, tetValues)
  print "tetrahedrons prior to splitting into faces"
  print tetTopology.getValuesString()
  faceTopology = converter.getExternalFaces(tetTopology)
  print "after splitting into faces"
  print faceTopology.getValuesString()
  assert(faceTopology.getValuesString() == "0 1 3 "
                                           "0 3 2 "
                                           "0 1 4 "
                                           "0 4 2 "
                                           "1 2 3 "
                                           "1 2 4");

  #
  # Tetrahedron_10 to Triangle_6
  #
  tet10Topology = XdmfTopology.New()
  tet10Topology.setType(XdmfTopologyType.Tetrahedron_10())
  tet10Values = [0, 1, 2, 3, 5, 6,  7,  8,  9,  10,
                 0, 1, 2, 4, 9, 10, 11, 12, 13, 14] # temporary
  tet10Topology.insertAsInt64(0, tet10Values);
  print "tetrahedron_10s prior to splitting into faces "
  print tet10Topology.getValuesString()
  faceTopology = converter.getExternalFaces(tet10Topology)
  print "after splitting into faces"
  print faceTopology.getValuesString()
  assert(faceTopology.getValuesString() == "0 1 3 5 9 8 "
                                           "0 3 2 8 10 7 "
                                           "0 1 4 9 13 12 "
                                           "0 4 2 12 14 11 "
                                           "1 2 3 6 10 9 "
                                           "1 2 4 10 14 13");

  #
  # Hexahedron to Quadrilateral
  #
  hexTopology = XdmfTopology.New()
  hexTopology.setType(XdmfTopologyType.Hexahedron())
  hexValues = [0, 1, 2, 3, 4, 5, 6, 7,
               0, 1, 2, 3, 8, 9, 10, 11] # temporary
  hexTopology.insertAsInt64(0, hexValues)
  print "Hexahedrons prior to splitting into faces"
  print hexTopology.getValuesString()
  faceTopology = converter.getExternalFaces(hexTopology)
  print "after splitting into faces"
  print faceTopology.getValuesString()
  assert(faceTopology.getValuesString() == "0 1 5 4 "
                                           "0 4 7 3 "
                                           "0 1 9 8 "
                                           "0 8 11 3 "
                                           "1 2 6 5 "
                                           "1 2 10 9 "
                                           "2 3 7 6 "
                                           "2 3 11 10 "
                                           "4 5 6 7 "
                                           "8 9 10 11")

  #
  # Hexahedron_20 to Quadrilateral_8
  #
  hex20Topology = XdmfTopology.New()
  hex20Topology.setType(XdmfTopologyType.Hexahedron_20())
  hex20Values = [0, 1, 2, 3, 4, 5, 6,  7,  12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
                 0, 1, 2, 3, 8, 9, 10, 11, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35] # temporary
  hex20Topology.insertAsInt64(0, hex20Values)
  print "Hexahedron_20s prior to splitting into faces"
  print hex20Topology.getValuesString()
  faceTopology = converter.getExternalFaces(hex20Topology)
  print "after splitting into faces"
  print faceTopology.getValuesString()
  assert(faceTopology.getValuesString() == "0 1 5 4 12 21 16 20 "
                                           "0 4 7 3 20 19 23 15 "
                                           "0 1 9 8 24 33 28 32 "
                                           "0 8 11 3 32 31 35 27 "
                                           "1 2 6 5 13 22 17 21 "
                                           "1 2 10 9 25 34 29 33 "
                                           "2 3 7 6 14 23 18 22 "
                                           "2 3 11 10 26 35 30 34 "
                                           "4 5 6 7 16 17 18 19 "
                                           "8 9 10 11 28 29 30 31")
