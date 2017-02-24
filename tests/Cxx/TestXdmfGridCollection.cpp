#include "XdmfGridCollection.hpp"
#include "XdmfGridCollectionType.hpp"
#include "XdmfInformation.hpp"
#include "XdmfReader.hpp"
#include "XdmfWriter.hpp"
#include <iostream>
#include <assert.h>

#include "XdmfTestCompareFiles.hpp"
#include "XdmfTestDataGenerator.hpp"

int main(int, char **)
{
  // Test != and == operators
  std::cout << XdmfGridCollectionType::Spatial() << " ?= " << XdmfGridCollectionType::Spatial() << std::endl;
  std::cout << XdmfGridCollectionType::Spatial() << " ?!= " << XdmfGridCollectionType::Temporal() << std::endl;
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
  std::cout << gridCollection->getName() << " ?= " << "Collection2" << std::endl;
  assert(gridCollection->getName().compare("Collection2") == 0);

  shared_ptr<XdmfUnstructuredGrid> childGrid1 = 
    XdmfTestDataGenerator::createHexahedron();
  shared_ptr<XdmfUnstructuredGrid> childGrid2 =
    XdmfTestDataGenerator::createHexahedron();

  std::cout << gridCollection->getNumberUnstructuredGrids() << " ?= " << 0 << std::endl;
  assert(gridCollection->getNumberUnstructuredGrids() == 0);
  gridCollection->insert(childGrid1);
  std::cout << gridCollection->getNumberUnstructuredGrids() << " ?= " << 1 << std::endl;
  std::cout << gridCollection->getUnstructuredGrid(0) << " ?= " << childGrid1 << std::endl;
  assert(gridCollection->getNumberUnstructuredGrids() == 1);
  assert(gridCollection->getUnstructuredGrid(0) == childGrid1);
  gridCollection->removeUnstructuredGrid(0);
  std::cout << gridCollection->getNumberUnstructuredGrids() << " ?= " << 0 << std::endl;
  assert(gridCollection->getNumberUnstructuredGrids() == 0);
  gridCollection->insert(childGrid1);
  gridCollection->insert(childGrid2);

  shared_ptr<XdmfInformation> information = XdmfInformation::New("Key", 
                                                                 "Value");
  gridCollection->insert(information);

  gridCollection->setType(XdmfGridCollectionType::Temporal());
  std::cout << gridCollection->getType() << " ?= " << XdmfGridCollectionType::Temporal() << std::endl;
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

  if (XdmfTestCompareFiles::compareFiles("TestXdmfGridCollection1.xmf",
                                         "TestXdmfGridCollection2.xmf"))
  {
    std::cout << "compared files are the same" << std::endl;
  }
  else
  {
    std::cout << "compared files are not the same" << std::endl;
  }

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

  if (XdmfTestCompareFiles::compareFiles("TestXdmfGridCollectionHDF1.xmf",
                                         "TestXdmfGridCollectionHDF2.xmf"))
  {
    std::cout << "compared files are the same" << std::endl;
  }
  else
  {
    std::cout << "compared files are not the same" << std::endl;
  }

  assert(XdmfTestCompareFiles::compareFiles("TestXdmfGridCollectionHDF1.xmf",
                                            "TestXdmfGridCollectionHDF2.xmf"));

  return 0;
}
