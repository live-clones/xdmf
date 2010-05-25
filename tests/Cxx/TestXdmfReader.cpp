#include <fstream>
#include <sstream>
#include "XdmfItem.hpp"
#include "XdmfReader.hpp"
#include "XdmfWriter.hpp"

#include "XdmfTestDataGenerator.hpp"

int main(int argc, char* argv[])
{
	boost::shared_ptr<XdmfWriter> writer = XdmfWriter::New("TestXdmfReader1.xmf");
	writer->setLightDataLimit(10);

	boost::shared_ptr<XdmfGrid> grid = XdmfTestDataGenerator::createHexahedron();

	boost::shared_ptr<XdmfDomain> domain = XdmfDomain::New();
	domain->insert(grid);
	domain->accept(writer);

	boost::shared_ptr<XdmfReader> reader = XdmfReader::New();
	boost::shared_ptr<XdmfDomain> readDomain = boost::shared_dynamic_cast<XdmfDomain>(reader->read("TestXdmfReader1.xmf"));

	boost::shared_ptr<XdmfWriter> writer2 = XdmfWriter::New("TestXdmfReader2.xmf");
	readDomain->accept(writer2);

	// Compare two files for equality
	std::ifstream firstFile("TestXdmfReader1.xmf");
	std::ifstream secondFile("TestXdmfReader2.xmf");

	std::stringstream firstBuffer;
	std::stringstream secondBuffer;

	firstBuffer << firstFile.rdbuf();
	secondBuffer << secondFile.rdbuf();

	std::string firstContents(firstBuffer.str());
	std::string secondContents(secondBuffer.str());

	assert(firstContents.compare(secondContents) == 0);


	return 0;
}
