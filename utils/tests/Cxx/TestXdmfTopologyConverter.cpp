#include <math.h>
#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfGeometryType.hpp"
#include "XdmfTopology.hpp"
#include "XdmfTopologyConverter.hpp"
#include "XdmfTopologyType.hpp"
#include "XdmfUnstructuredGrid.hpp"
#include "XdmfWriter.hpp"

int main(int, char *)
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
  return 0;
}
