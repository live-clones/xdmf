#include "XdmfArray.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfGeometryType.hpp"
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

  std::cout << grid->getDimensions()->getValuesString() << " ?= 4 3 3" << std::endl;
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
    XdmfWriter::New("TestXdmfRectilinearGrid1.xmf");
  grid->accept(writer);

  shared_ptr<XdmfReader> reader = XdmfReader::New();
  shared_ptr<XdmfRectilinearGrid> grid2 = 
    shared_dynamic_cast<XdmfRectilinearGrid>
    (reader->read("TestXdmfRectilinearGrid1.xmf"));

  shared_ptr<XdmfWriter> writer2 =
    XdmfWriter::New("TestXdmfRectilinearGrid2.xmf");
  grid2->accept(writer2);

  if (XdmfTestCompareFiles::compareFiles("TestXdmfRectilinearGrid1.xmf",
                                            "TestXdmfRectilinearGrid2.xmf"))
  {
    std::cout << "compared files are equal" << std::endl;
  }
  else
  {
    std::cout << "compared files are not equal" << std::endl;
  }

  assert(XdmfTestCompareFiles::compareFiles("TestXdmfRectilinearGrid1.xmf",
                                            "TestXdmfRectilinearGrid2.xmf"));

  // Testing 1d meshes

  shared_ptr<XdmfArray> dim1d = XdmfArray::New();
  dim1d->pushBack(4);
  dim1d->pushBack(5);
  dim1d->pushBack(6);
  dim1d->pushBack(7);

  std::vector<shared_ptr<XdmfArray> > dimvector;
  dimvector.push_back(dim1d);

  shared_ptr<XdmfRectilinearGrid> grid3 = XdmfRectilinearGrid::New(dimvector);

  shared_ptr<XdmfWriter> writer3 =
    XdmfWriter::New("TestXdmfRectilinearGrid3.xmf");

  grid3->accept(writer3);

  shared_ptr<XdmfRectilinearGrid> readgrid =
    shared_dynamic_cast<XdmfRectilinearGrid>(reader->read("TestXdmfRectilinearGrid3.xmf"));

  return 0;
}
