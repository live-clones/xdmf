#include "XdmfArray.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfGeometryType.hpp"
#include "XdmfGridCollection.hpp"
#include "XdmfRectilinearGrid.hpp"
#include "XdmfReader.hpp"
#include "XdmfTopology.hpp"
#include "XdmfTopologyType.hpp"
#include "XdmfHDF5Controller.hpp"
#include "XdmfHDF5Writer.hpp"
#include "XdmfWriter.hpp"
#include <iostream>
#include <assert.h>

#include "XdmfTestCompareFiles.hpp"

int main(int, char **)
{
  shared_ptr<XdmfArray> vx = XdmfArray::New();
  vx->initialize<unsigned int>(4);
  vx->insert(0, 0);
  vx->insert(1, 1);
  vx->insert(2, 2);
  vx->insert(3, 4);

  shared_ptr<XdmfArray> vy = XdmfArray::New();
  vy->initialize<unsigned int>(3);
  vy->insert(0, 0);
  vy->insert(1, 1);
  vy->insert(2, 3);

  shared_ptr<XdmfArray> vz = XdmfArray::New();
  vz->initialize<unsigned int>(3);
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

  // Testing writing to specified datasets

  shared_ptr<XdmfArray> array1 = XdmfArray::New();

  shared_ptr<XdmfArray> array2 = XdmfArray::New();

  for (unsigned int i = 0; i < 10; ++i)
  {
    array1->pushBack(i);
    array2->pushBack(i + 5);
  }

  shared_ptr<XdmfHeavyDataController> controller1 =
    XdmfHDF5Controller::New("TestXdmfHDF5Visit.h5",
                            "Test Dataset1",
                            array1->getArrayType(),
                            std::vector<unsigned int>(1, 0),
                            std::vector<unsigned int>(1, 1),
                            std::vector<unsigned int>(1, 10),
                            std::vector<unsigned int>(1, 10));

  shared_ptr<XdmfHeavyDataController> controller2 =
    XdmfHDF5Controller::New("TestXdmfHDF5Visit.h5",
                            "Test Dataset2",
                            array2->getArrayType(),
                            std::vector<unsigned int>(1, 0),
                            std::vector<unsigned int>(1, 1),
                            std::vector<unsigned int>(1, 10),
                            std::vector<unsigned int>(1, 10));

  array1->insert(controller1);
  array2->insert(controller2);

  std::cout << "writing array 1 to specified dataset" << std::endl;

  array1->accept(writer->getHeavyDataWriter());

  std::cout << "writing array 2 to specified dataset" << std::endl;

  array2->accept(writer->getHeavyDataWriter());

  return 0;
}
