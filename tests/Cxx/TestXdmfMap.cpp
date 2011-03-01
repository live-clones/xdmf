#include "XdmfAttribute.hpp"
#include "XdmfAttributeCenter.hpp"
#include "XdmfAttributeType.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfGeometryType.hpp"
#include "XdmfGrid.hpp"
#include "XdmfGridCollection.hpp"
#include "XdmfGridCollectionType.hpp"
#include "XdmfHDF5Writer.hpp"
#include "XdmfMap.hpp"
#include "XdmfReader.hpp"
#include "XdmfTestCompareFiles.hpp"
#include "XdmfTopology.hpp"
#include "XdmfTopologyType.hpp"
#include "XdmfWriter.hpp"

void performTests(std::vector<boost::shared_ptr<XdmfMap> > & boundaryMaps)
{
  boundaryMaps[0]->read();
  boundaryMaps[1]->read();
  std::map<unsigned int, unsigned int> mapping;
  mapping = boundaryMaps[0]->getRemoteNodeIds(0);
  assert(mapping.size() == 0);
  mapping = boundaryMaps[0]->getRemoteNodeIds(1);
  assert(mapping.size() == 1);
  assert(mapping[1] == 0);
  mapping = boundaryMaps[1]->getRemoteNodeIds(0);
  assert(mapping.size() == 1);
  assert(mapping[0] == 1);
}

/*
 * (local, global)
 *
 * Grid 0:
 *
 * (0, 0) - - - (1, 1)
 *
 * Grid 1:
 *
 * (0, 1) - - - (1, 2)
 *
 */

int main(int, char *)
{
  // Grid 0
  boost::shared_ptr<XdmfGrid> grid0 = XdmfGrid::New();
  grid0->getGeometry()->setType(XdmfGeometryType::XYZ());
  double points0[] = {-1, 0, 0, 0, 0, 0};
  grid0->getGeometry()->insert(0, &points0[0], 6);
  grid0->getTopology()->setType(XdmfTopologyType::Polyline());
  unsigned int connectivity0[] = {0, 1};
  grid0->getTopology()->insert(0, &connectivity0[0], 2);
  boost::shared_ptr<XdmfAttribute> globalNodeIds0 = XdmfAttribute::New();
  globalNodeIds0->setName("GlobalNodeId");
  globalNodeIds0->setCenter(XdmfAttributeCenter::Node());
  globalNodeIds0->setType(XdmfAttributeType::GlobalId());
  unsigned int globalVals0[] = {0, 1};
  globalNodeIds0->insert(0, &globalVals0[0], 2);

  // Grid 1
  boost::shared_ptr<XdmfGrid> grid1 = XdmfGrid::New();
  grid1->getGeometry()->setType(XdmfGeometryType::XYZ());
  double points1[] = {0, 0, 0, 1, 0, 0};
  grid1->getGeometry()->insert(0, &points1[0], 6);
  grid1->getTopology()->setType(XdmfTopologyType::Polyline());
  unsigned int connectivity1[] = {0, 1};
  grid1->getTopology()->insert(0, &connectivity1[0], 2);
  boost::shared_ptr<XdmfAttribute> globalNodeIds1 = XdmfAttribute::New();
  globalNodeIds1->setName("GlobalNodeId");
  globalNodeIds1->setCenter(XdmfAttributeCenter::Node());
  globalNodeIds1->setType(XdmfAttributeType::GlobalId());
  unsigned int globalVals1[] = {1, 2};
  globalNodeIds1->insert(0, &globalVals1[0], 2);

  std::vector<boost::shared_ptr<XdmfAttribute> > globalNodeIds;
  globalNodeIds.push_back(globalNodeIds0);
  globalNodeIds.push_back(globalNodeIds1);

  std::vector<boost::shared_ptr<XdmfMap> > boundaryMaps =
    XdmfMap::New(globalNodeIds);

  performTests(boundaryMaps);
  grid0->setMap(boundaryMaps[0]);
  grid1->setMap(boundaryMaps[1]);

  // Grid Collection
  boost::shared_ptr<XdmfGridCollection> collection = XdmfGridCollection::New();
  collection->setType(XdmfGridCollectionType::Spatial());
  collection->insert(grid0);
  collection->insert(grid1);
  boost::shared_ptr<XdmfDomain> domain = XdmfDomain::New();
  domain->insert(collection);

  boost::shared_ptr<XdmfWriter> writer = XdmfWriter::New("TestXdmfMap1.xmf");
  domain->accept(writer);

  boost::shared_ptr<XdmfReader> reader = XdmfReader::New();
  boost::shared_ptr<XdmfDomain> domain2 =
    boost::shared_dynamic_cast<XdmfDomain>(reader->read("TestXdmfMap1.xmf"));

  boundaryMaps.clear();
  boundaryMaps.push_back(domain2->getGridCollection(0)->getGrid(0)->getMap());
  boundaryMaps.push_back(domain2->getGridCollection(0)->getGrid(1)->getMap());
  performTests(boundaryMaps);

  boost::shared_ptr<XdmfWriter> writer2 = XdmfWriter::New("TestXdmfMap2.xmf");
  domain2->accept(writer2);

  assert(XdmfTestCompareFiles::compareFiles("TestXdmfMap1.xmf",
                                            "TestXdmfMap2.xmf"));

  boost::shared_ptr<XdmfWriter> writerHDF =
    XdmfWriter::New("TestXdmfMapHDF1.xmf");
  writerHDF->setLightDataLimit(0);
  domain->accept(writerHDF);

  boost::shared_ptr<XdmfDomain> domainHDF =
    boost::shared_dynamic_cast<XdmfDomain>
    (reader->read("TestXdmfMapHDF1.xmf"));

  boundaryMaps.clear();
  boundaryMaps.push_back(domainHDF->getGridCollection(0)->getGrid(0)->getMap());
  boundaryMaps.push_back(domainHDF->getGridCollection(0)->getGrid(1)->getMap());
  performTests(boundaryMaps);

  boost::shared_ptr<XdmfWriter> writerHDF2 =
    XdmfWriter::New("TestXdmfMapHDF2.xmf");
  writerHDF2->getHDF5Writer()->setMode(XdmfHDF5Writer::Overwrite);
  writerHDF2->setLightDataLimit(0);
  domainHDF->accept(writerHDF2);

  assert(XdmfTestCompareFiles::compareFiles("TestXdmfMapHDF1.xmf",
                                            "TestXdmfMapHDF2.xmf"));

  return 0;
}
