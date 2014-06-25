#include "XdmfArray.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfGeometryType.hpp"
#include "XdmfGridCollection.hpp"
#include "XdmfRectilinearGrid.hpp"
#include "XdmfReader.hpp"
#include "XdmfTopology.hpp"
#include "XdmfTopologyType.hpp"
#include "XdmfHDF5Writer.hpp"
#include "XdmfWriter.hpp"
#include <iostream>

#include "XdmfTestCompareFiles.hpp"

int main(int, char **)
{
  shared_ptr<XdmfArray> vx = XdmfArray::New();
  vx->resize<unsigned int>(4);
  vx->insert(0, 0);
  vx->insert(1, 1);
  vx->insert(2, 2);
  vx->insert(3, 4);

  shared_ptr<XdmfArray> vy = XdmfArray::New();
  vy->resize<unsigned int>(3);
  vy->insert(0, 0);
  vy->insert(1, 1);
  vy->insert(2, 3);

  shared_ptr<XdmfArray> vz = XdmfArray::New();
  vz->resize<unsigned int>(3);
  vz->insert(0, 0);
  vz->insert(1, 1);
  vz->insert(2, 3);

  shared_ptr<XdmfRectilinearGrid> grid = XdmfRectilinearGrid::New(vx,
                                                                  vy,
                                                                  vz);

  std::cout << grid->getDimensions()->getValuesString() << " ?= " << "4 3 3" << std::endl;
  std::cout << vx << " ?= " << grid->getCoordinates(0) << std::endl;
  std::cout << vy << " ?= " << grid->getCoordinates(1) << std::endl;
  std::cout << vz << " ?= " << grid->getCoordinates(2) << std::endl;
  assert(grid->getDimensions()->getValuesString().compare("4 3 3") == 0);
  assert(vx == grid->getCoordinates(0));
  assert(vy == grid->getCoordinates(1));
  assert(vz == grid->getCoordinates(2));

  // Check values under the hood
  shared_ptr<const XdmfTopology> topology = grid->getTopology();
  std::cout << topology->getNumberElements() << " ?= " << 12 << std::endl;
  assert(topology->getNumberElements() == 12);
  shared_ptr<const XdmfTopologyType> topologyType = topology->getType();
  std::cout << topologyType->getNodesPerElement() << " ?= " << 8 << std::endl;
  assert(topologyType->getNodesPerElement() == 8);

  shared_ptr<const XdmfGeometry> geometry = grid->getGeometry();
  std::cout << geometry->getNumberPoints() << " ?= " << 36 << std::endl;
  assert(geometry->getNumberPoints() == 36);
  shared_ptr<const XdmfGeometryType> geometryType = geometry->getType();
  std::cout << geometryType->getDimensions() << " ?= " << 3 << std::endl;
  assert(geometryType->getDimensions() == 3);

  // Input / Output

  shared_ptr<XdmfWriter> writer =
    XdmfWriter::New("TestXdmfHDF5Visit.xmf");
  grid->accept(writer->getHeavyDataWriter());
  grid->accept(writer);

  return 0;
}
