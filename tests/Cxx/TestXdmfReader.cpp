#include "XdmfDomain.hpp"
#include "XdmfReader.hpp"
#include "XdmfWriter.hpp"
#include <iostream>
#include <assert.h>

#include "XdmfTestCompareFiles.hpp"
#include "XdmfTestDataGenerator.hpp"

int main(int, char **)
{
  shared_ptr<XdmfWriter> writer = XdmfWriter::New("TestXdmfReader1.xmf");
  writer->setLightDataLimit(10);

  shared_ptr<XdmfUnstructuredGrid> grid1 =
    XdmfTestDataGenerator::createHexahedron();
  shared_ptr<XdmfUnstructuredGrid> grid2 =
    XdmfTestDataGenerator::createHexahedron();

  shared_ptr<XdmfDomain> domain = XdmfDomain::New();
  domain->insert(grid1);
  domain->insert(grid2);
  domain->accept(writer);

  shared_ptr<XdmfReader> reader = XdmfReader::New();
  shared_ptr<XdmfDomain> readDomain =
    shared_dynamic_cast<XdmfDomain>(reader->read("TestXdmfReader1.xmf"));

  shared_ptr<XdmfWriter> writer2 = XdmfWriter::New("TestXdmfReader2.xmf");
  writer2->setMode(XdmfWriter::DistributedHeavyData);
  readDomain->accept(writer2);

  if (XdmfTestCompareFiles::compareFiles("TestXdmfReader1.xmf",
                                         "TestXdmfReader2.xmf"))
  {
    std::cout << "compared files are the same" << std::endl;
  }
  else
  {
    std::cout << "compared files are not the same" << std::endl;
  }

  assert(XdmfTestCompareFiles::compareFiles("TestXdmfReader1.xmf",
                                            "TestXdmfReader2.xmf"));

  std::vector<shared_ptr<XdmfItem> > readItems =
    reader->read("TestXdmfReader1.xmf", "/Xdmf/Domain/Grid[1]");

  std::cout << readItems.size() << " ?= " << 1 << std::endl;

  assert(readItems.size() == 1);
  shared_ptr<XdmfUnstructuredGrid> readGrid =
    shared_dynamic_cast<XdmfUnstructuredGrid>(readItems[0]);

  std::cout << readGrid->getName() << " ?= " << "Hexahedron" << std::endl;

  assert(readGrid->getName().compare("Hexahedron") == 0);

  std::vector<shared_ptr<XdmfItem> > readItems2 =
    reader->read("TestXdmfReader1.xmf", "//Attribute");

  std::cout << readItems2.size() << " ?= " << 8 << std::endl;

  assert(readItems2.size() == 8);
  shared_ptr<XdmfAttribute> readAttribute =
    shared_dynamic_cast<XdmfAttribute>(readItems2[0]);

  std::cout << readAttribute->getName() << " ?= " << "Nodal Attribute" << std::endl;

  assert(readAttribute->getName().compare("Nodal Attribute") == 0);

  return 0;
}
