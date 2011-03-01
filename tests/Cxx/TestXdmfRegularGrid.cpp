#include "XdmfArray.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfGeometryType.hpp"
#include "XdmfReader.hpp"
#include "XdmfRegularGrid.hpp"
#include "XdmfTopology.hpp"
#include "XdmfTopologyType.hpp"
#include "XdmfWriter.hpp"

#include "XdmfTestCompareFiles.hpp"

int main(int, char *)
{
  boost::shared_ptr<XdmfRegularGrid> grid = XdmfRegularGrid::New(1, 1, 1,
                                                                 1, 1, 1,
                                                                 0, 0, 0);
  boost::shared_ptr<XdmfArray> brickSize = grid->getBrickSize();
  assert(brickSize->getSize() == 3);
  for(unsigned int i=0; i<brickSize->getSize(); ++i) {
    assert(brickSize->getValue<unsigned int>(i) == 1);
  }
  boost::shared_ptr<XdmfArray> dimensions = grid->getDimensions();
  assert(dimensions->getSize() == 3);
  for(unsigned int i=0; i<dimensions->getSize(); ++i) {
    assert(dimensions->getValue<unsigned int>(i) == 1);
  }
  boost::shared_ptr<XdmfArray> origin = grid->getOrigin();
  assert(origin->getSize() == 3);
  for(unsigned int i=0; i<origin->getSize(); ++i) {
    assert(origin->getValue<unsigned int>(i) == 0);
  }

  // Setting brickSize, dimensions, origin

  boost::shared_ptr<XdmfArray> newBrickSize = XdmfArray::New();
  newBrickSize->initialize<double>(3);
  newBrickSize->insert(0, 2);
  newBrickSize->insert(1, 2);
  newBrickSize->insert(2, 2);
  grid->setBrickSize(newBrickSize);
  brickSize = grid->getBrickSize();
  assert(brickSize->getSize() == 3);
  for(unsigned int i=0; i<brickSize->getSize(); ++i) {
    assert(brickSize->getValue<unsigned int>(i) == 2);
  }

  boost::shared_ptr<XdmfArray> newDimensions = XdmfArray::New();
  newDimensions->initialize<unsigned int>(3);
  newDimensions->insert(0, 2);
  newDimensions->insert(1, 2);
  newDimensions->insert(2, 2);
  grid->setDimensions(newDimensions);
  dimensions = grid->getDimensions();
  assert(dimensions->getSize() == 3);
  for(unsigned int i=0; i<dimensions->getSize(); ++i) {
    assert(dimensions->getValue<unsigned int>(i) == 2);
  }

  boost::shared_ptr<XdmfArray> newOrigin = XdmfArray::New();
  newOrigin->initialize<double>(3);
  newOrigin->insert(0, 1);
  newOrigin->insert(1, 1);
  newOrigin->insert(2, 1);
  grid->setOrigin(newOrigin);
  origin = grid->getOrigin();
  assert(origin->getSize() == 3);
  for(unsigned int i=0; i<origin->getSize(); ++i) {
    assert(origin->getValue<unsigned int>(i) == 1);
  }

  // Check values under the hood

  boost::shared_ptr<const XdmfTopology> topology = grid->getTopology();
  assert(topology->getNumberElements() == 1);
  boost::shared_ptr<const XdmfTopologyType> topologyType = topology->getType();
  assert(topologyType->getNodesPerElement() == 8);

  boost::shared_ptr<const XdmfGeometry> geometry = grid->getGeometry();
  assert(geometry->getNumberPoints() == 8);
  boost::shared_ptr<const XdmfGeometryType> geometryType = geometry->getType();
  assert(geometryType->getDimensions() == 3);

  // Input / Output

  boost::shared_ptr<XdmfWriter> writer =
    XdmfWriter::New("TestXdmfRegularGrid1.xmf");
  grid->accept(writer);

  boost::shared_ptr<XdmfReader> reader = XdmfReader::New();
  boost::shared_ptr<XdmfRegularGrid> grid2 =
    boost::shared_dynamic_cast<XdmfRegularGrid>
    (reader->read("TestXdmfRegularGrid1.xmf"));

  boost::shared_ptr<XdmfWriter> writer2 =
    XdmfWriter::New("TestXdmfRegularGrid2.xmf");
  grid2->accept(writer2);

  assert(XdmfTestCompareFiles::compareFiles("TestXdmfRegularGrid1.xmf",
                                            "TestXdmfRegularGrid2.xmf"));

  return 0;
}
