#include <fstream>
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
	boost::shared_ptr<XdmfDomain> readDomain = reader->read("TestXdmfReader1.xmf");

	boost::shared_ptr<XdmfWriter> writer2 = XdmfWriter::New("TestXdmfReader2.xmf");
	readDomain->accept(writer2);

	std::ifstream firstFile("TestXdmfReader1.xmf");
	std::ifstream secondFile("TestXdmfReader2.xmf");

	std::string firstLine;
	std::string secondLine;

	while(!firstFile.eof())
	{
		std::getline(firstFile, firstLine);
		std::getline(secondFile, secondLine);
		if(firstLine.compare(secondLine) != 0)
		{
			assert(false);
		}
	}

	return 0;
}
