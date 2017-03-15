#include <math.h>
#include <iostream>
#include <assert.h>
#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfGeometryType.hpp"
#include "XdmfTopology.hpp"
#include "XdmfTopologyConverter.hpp"
#include "XdmfTopologyType.hpp"
#include "XdmfUnstructuredGrid.hpp"
#include "XdmfWriter.hpp"

int main(int, char **)
{
  shared_ptr<XdmfTopologyConverter> converter =
    XdmfTopologyConverter::New();

  // Create Hexahedron Grid
  shared_ptr<XdmfUnstructuredGrid> hexGrid =
    XdmfUnstructuredGrid::New();
  double hexPoints[24] = {0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0,
                          1, 1, 1, 1, 0, 1, 1};
  hexGrid->getGeometry()->setType(XdmfGeometryType::XYZ());
  hexGrid->getGeometry()->resize<double>(24, 0);
  hexGrid->getGeometry()->insert(0, hexPoints, 24);
  unsigned int hexConn[8] = {0, 1, 2, 3, 4, 5, 6, 7};
  hexGrid->getTopology()->setType(XdmfTopologyType::Hexahedron());
  hexGrid->getTopology()->resize<unsigned int>(8, 0);
  hexGrid->getTopology()->insert(0, hexConn, 8);

  /*
   * Hexahedron to Hexahedron_64
   */
  shared_ptr<XdmfUnstructuredGrid> hex64Grid =
    converter->convert(hexGrid, XdmfTopologyType::Hexahedron_64());

  assert(hex64Grid->getGeometry()->getType() == XdmfGeometryType::XYZ());
  assert(hex64Grid->getGeometry()->getNumberPoints() == 64);
  assert(hex64Grid->getTopology()->getType() ==
         XdmfTopologyType::Hexahedron_64());
  assert(hex64Grid->getTopology()->getNumberElements() == 1);
  for(unsigned int i=0; i<64; ++i) {
    assert(i == hex64Grid->getTopology()->getValue<unsigned int>(i));
  }

  /*
   * Hexahedron to Hexahedron_125
   */
  shared_ptr<XdmfUnstructuredGrid> hex125Grid =
    converter->convert(hexGrid, XdmfTopologyType::Hexahedron_125());

  assert(hex125Grid->getGeometry()->getType() == XdmfGeometryType::XYZ());
  assert(hex125Grid->getGeometry()->getNumberPoints() == 125);
  assert(hex125Grid->getTopology()->getType() ==
         XdmfTopologyType::Hexahedron_125());
  assert(hex125Grid->getTopology()->getNumberElements() == 1);
  for(unsigned int i=0; i<125; ++i) {
    assert(i == hex125Grid->getTopology()->getValue<unsigned int>(i));
  }

  /*
   * Hexahedron_64 to Hexahedron
   */
  shared_ptr<XdmfUnstructuredGrid> newHexGrid =
    converter->convert(hex64Grid, XdmfTopologyType::Hexahedron());
  assert(newHexGrid->getGeometry()->getType() == XdmfGeometryType::XYZ());
  assert(newHexGrid->getGeometry()->getNumberPoints() == 64);
  assert(newHexGrid->getTopology()->getType() ==
         XdmfTopologyType::Hexahedron());
  assert(newHexGrid->getTopology()->getNumberElements() == 27);


  shared_ptr<XdmfTopology> faceTopology;

  /**
   * Tetrahedron to Triangle
   */
  shared_ptr<XdmfTopology> tetTopology = XdmfTopology::New();
  tetTopology->setType(XdmfTopologyType::Tetrahedron());
  long tetValues[8] = {0, 1, 2, 3, 0, 1, 2, 4};// temporary
  tetTopology->insert(0, tetValues, 8);
  std::cout << "tetrahedrons prior to splitting into faces " << std::endl
            << tetTopology->getValuesString() << std::endl;
  faceTopology = converter->getExternalFaces(tetTopology);
  std::cout << "after splitting into faces" << std::endl
            << faceTopology->getValuesString() << std::endl;
  assert(faceTopology->getValuesString().compare("0 1 3 "
                                                 "0 3 2 "
                                                 "0 1 4 "
                                                 "0 4 2 "
                                                 "1 2 3 "
                                                 "1 2 4") == 0);

  /**
   * Tetrahedron_10 to Triangle_6
   */
  shared_ptr<XdmfTopology> tet10Topology = XdmfTopology::New();
  tet10Topology->setType(XdmfTopologyType::Tetrahedron_10());
  long tet10Values[20] = {0, 1, 2, 3, 5, 6,  7,  8,  9,  10,
                          0, 1, 2, 4, 9, 10, 11, 12, 13, 14};// temporary
  tet10Topology->insert(0, tet10Values, 20);
  std::cout << "tetrahedron_10s prior to splitting into faces " << std::endl
            << tet10Topology->getValuesString() << std::endl;
  faceTopology = converter->getExternalFaces(tet10Topology);
  std::cout << "after splitting into faces" << std::endl
            << faceTopology->getValuesString() << std::endl;
  assert(faceTopology->getValuesString().compare("0 1 3 5 9 8 "
                                                 "0 3 2 8 10 7 "
                                                 "0 1 4 9 13 12 "
                                                 "0 4 2 12 14 11 "
                                                 "1 2 3 6 10 9 "
                                                 "1 2 4 10 14 13") == 0);

  /**
   * Hexahedron to Quadrilateral
   */
  shared_ptr<XdmfTopology> hexTopology = XdmfTopology::New();
  hexTopology->setType(XdmfTopologyType::Hexahedron());
  long hexValues[16] = {0, 1, 2, 3, 4, 5, 6, 7,
                         0, 1, 2, 3, 8, 9, 10, 11};// temporary
  hexTopology->insert(0, hexValues, 16);
  std::cout << "Hexahedrons prior to splitting into faces " << std::endl
            << hexTopology->getValuesString() << std::endl;
  faceTopology = converter->getExternalFaces(hexTopology);
  std::cout << "after splitting into faces" << std::endl
            << faceTopology->getValuesString() << std::endl;
  assert(faceTopology->getValuesString().compare("0 1 5 4 "
                                                 "0 4 7 3 "
                                                 "0 1 9 8 "
                                                 "0 8 11 3 "
                                                 "1 2 6 5 "
                                                 "1 2 10 9 "
                                                 "2 3 7 6 "
                                                 "2 3 11 10 "
                                                 "4 5 6 7 "
                                                 "8 9 10 11") == 0);

  /**
   * Hexahedron_20 to Quadrilateral_8
   */
  shared_ptr<XdmfTopology> hex20Topology = XdmfTopology::New();
  hex20Topology->setType(XdmfTopologyType::Hexahedron_20());
  long hex20Values[40] = {0, 1, 2, 3, 4, 5, 6,  7,  12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
                           0, 1, 2, 3, 8, 9, 10, 11, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35};// temporary
  hex20Topology->insert(0, hex20Values, 40);
  std::cout << "Hexahedron_20s prior to splitting into faces " << std::endl
            << hex20Topology->getValuesString() << std::endl;
  faceTopology = converter->getExternalFaces(hex20Topology);
  std::cout << "after splitting into faces" << std::endl
            << faceTopology->getValuesString() << std::endl;
  assert(faceTopology->getValuesString().compare("0 1 5 4 12 21 16 20 "
                                                 "0 4 7 3 20 19 23 15 "
                                                 "0 1 9 8 24 33 28 32 "
                                                 "0 8 11 3 32 31 35 27 "
                                                 "1 2 6 5 13 22 17 21 "
                                                 "1 2 10 9 25 34 29 33 "
                                                 "2 3 7 6 14 23 18 22 "
                                                 "2 3 11 10 26 35 30 34 "
                                                 "4 5 6 7 16 17 18 19 "
                                                 "8 9 10 11 28 29 30 31") == 0);

  return 0;
}
