#include "XdmfArray.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfGeometryType.hpp"
#include "XdmfRectilinearGrid.hpp"
#include "XdmfReader.hpp"
#include "XdmfTopology.hpp"
#include "XdmfTopologyType.hpp"
#include "XdmfWriter.hpp"

#include "XdmfTestCompareFiles.hpp"

int main(int, char *)
{
  boost::shared_ptr<XdmfArray> vx = XdmfArray::New();
  vx->resize<unsigned int>(4);
  vx->insert(0, 0);
  vx->insert(1, 1);
  vx->insert(2, 2);
  vx->insert(3, 4);

  boost::shared_ptr<XdmfArray> vy = XdmfArray::New();
  vy->resize<unsigned int>(3);
  vy->insert(0, 0);
  vy->insert(1, 1);
  vy->insert(2, 3);

  boost::shared_ptr<XdmfArray> vz = XdmfArray::New();
  vz->resize<unsigned int>(3);
  vz->insert(0, 0);
  vz->insert(1, 1);
  vz->insert(2, 3);

  boost::shared_ptr<XdmfRectilinearGrid> grid = XdmfRectilinearGrid::New(vx,
                                                                         vy,
                                                                         vz);

  assert(grid->getDimensions()->getValuesString().compare("4 3 3 ") == 0);
  assert(vx == grid->getCoordinates(0));
  assert(vy == grid->getCoordinates(1));
  assert(vz == grid->getCoordinates(2));

  // Check values under the hood
  boost::shared_ptr<const XdmfTopology> topology = grid->getTopology();
  assert(topology->getNumberElements() == 12);
  boost::shared_ptr<const XdmfTopologyType> topologyType = topology->getType();
  assert(topologyType->getNodesPerElement() == 8);

  boost::shared_ptr<const XdmfGeometry> geometry = grid->getGeometry();
  assert(geometry->getNumberPoints() == 36);
  boost::shared_ptr<const XdmfGeometryType> geometryType = geometry->getType();
  assert(geometryType->getDimensions() == 3);

  // Input / Output

  boost::shared_ptr<XdmfWriter> writer =
    XdmfWriter::New("TestXdmfRectilinearGrid1.xmf");
  grid->accept(writer);

  boost::shared_ptr<XdmfReader> reader = XdmfReader::New();
  boost::shared_ptr<XdmfRectilinearGrid> grid2 =
    boost::shared_dynamic_cast<XdmfRectilinearGrid>
    (reader->read("TestXdmfRectilinearGrid1.xmf"));

  boost::shared_ptr<XdmfWriter> writer2 =
    XdmfWriter::New("TestXdmfRectilinearGrid2.xmf");
  grid2->accept(writer2);

  assert(XdmfTestCompareFiles::compareFiles("TestXdmfRectilinearGrid1.xmf",
                                            "TestXdmfRectilinearGrid2.xmf"));

  return 0;
}
