#include "XdmfArray.hpp"
#include "XdmfCurvilinearGrid.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfGeometryType.hpp"
#include "XdmfReader.hpp"
#include "XdmfTopology.hpp"
#include "XdmfTopologyType.hpp"
#include "XdmfWriter.hpp"
#include <iostream>
#include <assert.h>

#include "XdmfTestCompareFiles.hpp"

int main(int, char **)
{
  shared_ptr<XdmfCurvilinearGrid> grid = XdmfCurvilinearGrid::New(2, 2, 3);
  shared_ptr<XdmfArray> dimensions = grid->getDimensions();
  std::cout << dimensions->getSize() << " ?= " << 3 << std::endl;
  std::cout << dimensions->getValue<unsigned int>(0) << " ?= " << 2 << std::endl;
  std::cout << dimensions->getValue<unsigned int>(1) << " ?= " << 2 << std::endl;
  std::cout << dimensions->getValue<unsigned int>(2) << " ?= " << 3 << std::endl;
  assert(dimensions->getSize() == 3);
  assert(dimensions->getValue<unsigned int>(0) == 2);
  assert(dimensions->getValue<unsigned int>(1) == 2);
  assert(dimensions->getValue<unsigned int>(2) == 3);

  shared_ptr<XdmfGeometry> geometry = grid->getGeometry();
  geometry->setType(XdmfGeometryType::XYZ());
  geometry->initialize<int>(36);
  const int coordinates[36] = {0, 0, 0, 1, 0, 0, 4, 0, 0, 0, 0, 2, 1, 0, 2, 4,
			       0, 2, 0, 1, 0, 1, 1, 0, 4, 1, 0, 0, 1, 2, 1, 1,
			       2, 4, 1, 2};
  geometry->insert(0, &coordinates[0], 36);

  // Check values under the hood

  shared_ptr<const XdmfTopology> topology = grid->getTopology();
  std::cout << topology->getNumberElements() << " ?= " << 2 << std::endl;
  assert(topology->getNumberElements() == 2);
  shared_ptr<const XdmfTopologyType> topologyType = topology->getType();
  std::cout << topologyType->getNodesPerElement() << " ?= " << 8 << std::endl;
  assert(topologyType->getNodesPerElement() == 8);

  std::cout << geometry->getNumberPoints() << " ?= " << 12 << std::endl;
  assert(geometry->getNumberPoints() == 12);
  shared_ptr<const XdmfGeometryType> geometryType = geometry->getType();
  std::cout << geometryType->getDimensions() << " ?= " << 3 << std::endl;
  assert(geometryType->getDimensions() == 3);

  // Input / Output

  shared_ptr<XdmfWriter> writer =
    XdmfWriter::New("TestXdmfCurvilinearGrid1.xmf");
  grid->accept(writer);

  shared_ptr<XdmfReader> reader = XdmfReader::New();
  shared_ptr<XdmfCurvilinearGrid> grid2 = 
    shared_dynamic_cast<XdmfCurvilinearGrid>
    (reader->read("TestXdmfCurvilinearGrid1.xmf"));

  shared_ptr<XdmfWriter> writer2 = 
    XdmfWriter::New("TestXdmfCurvilinearGrid2.xmf");
  grid2->accept(writer2);

  if (XdmfTestCompareFiles::compareFiles("TestXdmfCurvilinearGrid1.xmf",
                                         "TestXdmfCurvilinearGrid2.xmf"))
  {
    std::cout << "compared files are the same" << std::endl;
  }
  else
  {
    std::cout << "compared files are not the same" << std::endl;
  }

  assert(XdmfTestCompareFiles::compareFiles("TestXdmfCurvilinearGrid1.xmf",
					    "TestXdmfCurvilinearGrid2.xmf"));

  return 0;
}
