#include <fstream>
#include <sstream>
#include "XdmfDomain.hpp"
#include "XdmfWriter.hpp"

#include "XdmfTestDataGenerator.hpp"

int main(int argc, char* argv[])
{
	boost::shared_ptr<XdmfWriter> writer = XdmfWriter::New("xpath.xmf");
	boost::shared_ptr<XdmfGrid> grid = XdmfTestDataGenerator::createHexahedron();

	boost::shared_ptr<XdmfGrid> newGrid = XdmfGrid::New();
	newGrid->setName("NoAttributes");
	newGrid->setGeometry(grid->getGeometry());
	newGrid->setTopology(grid->getTopology());

	boost::shared_ptr<XdmfDomain> domain = XdmfDomain::New();
	domain->insert(grid);
	domain->insert(grid);
	domain->insert(newGrid);
	domain->accept(writer);

	// Try to find xpaths written to file
	std::ifstream file("xpath.xmf");
	std::stringstream fileBuffer;
	fileBuffer << file.rdbuf();
	std::string fileContents(fileBuffer.str());

	assert(fileContents.find("xpointer=\"element(/1/1/1)\"") != std::string::npos);
	assert(fileContents.find("xpointer=\"element(/1/1/1/2)\"") != std::string::npos);
	assert(fileContents.find("xpointer=\"element(/1/1/1/3)\"") != std::string::npos);

	return 0;
}
