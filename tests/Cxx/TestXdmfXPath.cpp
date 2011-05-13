#include "XdmfDomain.hpp"
#include "XdmfReader.hpp"
#include "XdmfWriter.hpp"

#include "XdmfTestCompareFiles.hpp"
#include "XdmfTestDataGenerator.hpp"

int main(int, char **)
{
  shared_ptr<XdmfWriter> writer = XdmfWriter::New("XdmfXPath1.xmf");
  shared_ptr<XdmfUnstructuredGrid> grid =
    XdmfTestDataGenerator::createHexahedron();

  shared_ptr<XdmfUnstructuredGrid> newGrid = XdmfUnstructuredGrid::New();
  newGrid->setName("NoAttributes");
  newGrid->setGeometry(grid->getGeometry());
  newGrid->setTopology(grid->getTopology());

  shared_ptr<XdmfDomain> domain = XdmfDomain::New();
  domain->insert(grid);
  domain->insert(grid);
  domain->insert(newGrid);
  domain->accept(writer);

  // Try to find xpaths written to file
  std::ifstream file("XdmfXPath1.xmf");
  std::stringstream fileBuffer;
  fileBuffer << file.rdbuf();
  std::string fileContents(fileBuffer.str());

  assert(fileContents.find("xpointer=\"element(/1/1/1)\"") !=
         std::string::npos);
  assert(fileContents.find("xpointer=\"element(/1/1/1/2)\"") !=
         std::string::npos);
  assert(fileContents.find("xpointer=\"element(/1/1/1/3)\"") !=
         std::string::npos);

  // Make sure when we read it in we get the same structure as when we wrote
  // it out (multiple items holding the same shared pointers)
  shared_ptr<XdmfReader> reader = XdmfReader::New();
  shared_ptr<XdmfDomain> domain2 =
    shared_dynamic_cast<XdmfDomain>(reader->read("XdmfXPath1.xmf"));
  shared_ptr<XdmfWriter> writer2 =
    XdmfWriter::New("XdmfXPath2.xmf");
  domain2->accept(writer2);

  assert(XdmfTestCompareFiles::compareFiles("XdmfXPath1.xmf",
                                            "XdmfXPath2.xmf"));

  return 0;
}
