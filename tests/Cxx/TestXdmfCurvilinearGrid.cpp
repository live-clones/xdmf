#include "XdmfArray.hpp"
#include "XdmfCurvilinearGrid.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfGeometryType.hpp"
#include "XdmfReader.hpp"
#include "XdmfTopology.hpp"
#include "XdmfTopologyType.hpp"
#include "XdmfWriter.hpp"

#include "XdmfTestCompareFiles.hpp"

int main(int, char **)
{
  shared_ptr<XdmfCurvilinearGrid> grid = XdmfCurvilinearGrid::New(2, 2, 3);
  shared_ptr<XdmfArray> dimensions = grid->getDimensions();
  assert(dimensions->getSize() == 3);
  assert(dimensions->getValue<unsigned int>(0) == 2);
  assert(dimensions->getValue<unsigned int>(1) == 2);
  assert(dimensions->getValue<unsigned int>(2) == 3);

  shared_ptr<XdmfGeometry> geometry = grid->getGeometry();
  geometry->setType(XdmfGeometryType::XYZ());
  geometry->resize<int>(36);
  const int coordinates[36] = {0, 0, 0, 1, 0, 0, 4, 0, 0, 0, 0, 2, 1, 0, 2, 4,
			       0, 2, 0, 1, 0, 1, 1, 0, 4, 1, 0, 0, 1, 2, 1, 1,
			       2, 4, 1, 2};
  geometry->insert(0, &coordinates[0], 36);

  // Check values under the hood

  shared_ptr<const XdmfTopology> topology = grid->getTopology();
  assert(topology->getNumberElements() == 2);
  shared_ptr<const XdmfTopologyType> topologyType = topology->getType();
  assert(topologyType->getNodesPerElement() == 8);

  assert(geometry->getNumberPoints() == 12);
  shared_ptr<const XdmfGeometryType> geometryType = geometry->getType();
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

  assert(XdmfTestCompareFiles::compareFiles("TestXdmfCurvilinearGrid1.xmf",
					    "TestXdmfCurvilinearGrid2.xmf"));

  return 0;
}
