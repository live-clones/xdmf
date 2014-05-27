#include "XdmfGridCollection.hpp"
#include "XdmfGridCollectionType.hpp"
#include "XdmfInformation.hpp"
#include "XdmfReader.hpp"
#include "XdmfWriter.hpp"

#include "XdmfTestCompareFiles.hpp"
#include "XdmfTestDataGenerator.hpp"

int main(int, char **)
{
  // Test != and == operators
  assert(XdmfGridCollectionType::Spatial() == 
         XdmfGridCollectionType::Spatial());
  assert((XdmfGridCollectionType::Spatial() ==
         XdmfGridCollectionType::Temporal()) == false);
  assert((XdmfGridCollectionType::Spatial() !=
         XdmfGridCollectionType::Spatial()) == false);
  assert(XdmfGridCollectionType::Spatial() !=
         XdmfGridCollectionType::Temporal());

  shared_ptr<XdmfGridCollection> gridCollection = XdmfGridCollection::New();

  gridCollection->setName("Collection2");
  assert(gridCollection->getName().compare("Collection2") == 0);

  shared_ptr<XdmfUnstructuredGrid> childGrid1 = 
    XdmfTestDataGenerator::createHexahedron();
  shared_ptr<XdmfUnstructuredGrid> childGrid2 =
    XdmfTestDataGenerator::createHexahedron();

  assert(gridCollection->getNumberUnstructuredGrids() == 0);
  gridCollection->insert(childGrid1);
  assert(gridCollection->getNumberUnstructuredGrids() == 1);
  assert(gridCollection->getUnstructuredGrid(0) == childGrid1);
  gridCollection->removeUnstructuredGrid(0);
  assert(gridCollection->getNumberUnstructuredGrids() == 0);
  gridCollection->insert(childGrid1);
  gridCollection->insert(childGrid2);

  shared_ptr<XdmfInformation> information = XdmfInformation::New("Key", 
                                                                 "Value");
  gridCollection->insert(information);

  gridCollection->setType(XdmfGridCollectionType::Temporal());
  assert(gridCollection->getType() == XdmfGridCollectionType::Temporal());

  shared_ptr<XdmfWriter> writer = 
    XdmfWriter::New("TestXdmfGridCollection1.xmf");
  gridCollection->accept(writer);

  shared_ptr<XdmfReader> reader = XdmfReader::New();
  shared_ptr<XdmfGridCollection> gridCollection2 =
    shared_dynamic_cast<XdmfGridCollection>
    (reader->read("TestXdmfGridCollection1.xmf"));

  shared_ptr<XdmfWriter> writer2 =
    XdmfWriter::New("TestXdmfGridCollection2.xmf");
  gridCollection2->accept(writer2);

  assert(XdmfTestCompareFiles::compareFiles("TestXdmfGridCollection1.xmf",
                                            "TestXdmfGridCollection2.xmf"));

  shared_ptr<XdmfWriter> writer3 =
    XdmfWriter::New("TestXdmfGridCollectionHDF1.xmf");
  writer3->setLightDataLimit(0);
  gridCollection->accept(writer3);

  shared_ptr<XdmfGridCollection> gridCollection3 =
    shared_dynamic_cast<XdmfGridCollection>
    (reader->read("TestXdmfGridCollectionHDF1.xmf"));

  shared_ptr<XdmfWriter> writer4 =
    XdmfWriter::New("TestXdmfGridCollectionHDF2.xmf");
  writer4->setLightDataLimit(0);
  writer4->setMode(XdmfWriter::DistributedHeavyData);
  gridCollection3->accept(writer4);

  assert(XdmfTestCompareFiles::compareFiles("TestXdmfGridCollectionHDF1.xmf",
                                            "TestXdmfGridCollectionHDF2.xmf"));

  return 0;
}
