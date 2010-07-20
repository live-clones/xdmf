#include "XdmfDomain.hpp"
#include "XdmfWriter.hpp"

#include "XdmfTestDataGenerator.hpp"

int main(int argc, char* argv[])
{
	boost::shared_ptr<XdmfWriter> writer = XdmfWriter::New("output.xmf");

	std::string realPath = XdmfObject::getRealPath("output.xmf");

	std::cout << realPath << " " << writer->getFilePath() << std::endl;
	assert(writer->getFilePath().compare(realPath) == 0);
	writer->setLightDataLimit(10);
	assert(writer->getLightDataLimit() == 10);

	boost::shared_ptr<XdmfGrid> grid = XdmfTestDataGenerator::createHexahedron();

	boost::shared_ptr<XdmfDomain> domain = XdmfDomain::New();
	domain->insert(grid);
	domain->accept(writer);

	return 0;
}
