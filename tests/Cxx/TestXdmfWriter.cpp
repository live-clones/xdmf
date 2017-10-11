#include "XdmfDomain.hpp"
#include "XdmfSystemUtils.hpp"
#include "XdmfWriter.hpp"
#include <iostream>

#include "XdmfTestDataGenerator.hpp"

int main(int, char **)
{
  shared_ptr<XdmfWriter> writer = XdmfWriter::New("output.xmf");

  std::string realPath = XdmfSystemUtils::getRealPath("output.xmf");

  std::cout << writer->getFilePath() << " ?= " << realPath << std::endl;

  assert(writer->getFilePath().compare(realPath) == 0);
  writer->setLightDataLimit(10);

  std::cout << writer->getLightDataLimit() << " ?= " << 10 << std::endl;

  assert(writer->getLightDataLimit() == 10);

  shared_ptr<XdmfUnstructuredGrid> grid =
    XdmfTestDataGenerator::createHexahedron();

  shared_ptr<XdmfDomain> domain = XdmfDomain::New();
  domain->insert(grid);
  domain->accept(writer);

  return 0;
}
