#include "XdmfDomain.hpp"
#include "XdmfReader.hpp"
#include "XdmfWriter.hpp"

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

  assert(XdmfTestCompareFiles::compareFiles("TestXdmfReader1.xmf",
                                            "TestXdmfReader2.xmf"));

  std::vector<shared_ptr<XdmfItem> > readItems =
    reader->read("TestXdmfReader1.xmf", "/Xdmf/Domain/Grid[1]");
  assert(readItems.size() == 1);
  shared_ptr<XdmfUnstructuredGrid> readGrid =
    shared_dynamic_cast<XdmfUnstructuredGrid>(readItems[0]);
  assert(readGrid->getName().compare("Hexahedron") == 0);

  std::vector<shared_ptr<XdmfItem> > readItems2 =
    reader->read("TestXdmfReader1.xmf", "//Attribute");
  assert(readItems2.size() == 6);
  shared_ptr<XdmfAttribute> readAttribute =
    shared_dynamic_cast<XdmfAttribute>(readItems2[0]);
  assert(readAttribute->getName().compare("Nodal Attribute") == 0);

  return 0;
}
