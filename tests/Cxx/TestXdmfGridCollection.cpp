#include <fstream>
#include <sstream>
#include "XdmfGridCollection.hpp"
#include "XdmfReader.hpp"
#include "XdmfWriter.hpp"

#include "XdmfTestDataGenerator.hpp"

int main(int argc, char* argv[])
{
	// Test != and == operators
	assert(XdmfGridCollectionType::Spatial() == XdmfGridCollectionType::Spatial());
	assert(XdmfGridCollectionType::Spatial() == XdmfGridCollectionType::Temporal() == false);
	assert(XdmfGridCollectionType::Spatial() != XdmfGridCollectionType::Spatial() == false);
	assert(XdmfGridCollectionType::Spatial() != XdmfGridCollectionType::Temporal());

	boost::shared_ptr<XdmfGridCollection> gridCollection = XdmfGridCollection::New();

	gridCollection->setName("Collection2");
	assert(gridCollection->getName().compare("Collection2") == 0);

	boost::shared_ptr<XdmfGrid> childGrid1 = XdmfTestDataGenerator::createHexahedron();
	boost::shared_ptr<XdmfGrid> childGrid2 = XdmfTestDataGenerator::createHexahedron();

	assert(gridCollection->getNumberOfGrids() == 0);
	gridCollection->insert(childGrid1);
	assert(gridCollection->getNumberOfGrids() == 1);
	assert(gridCollection->getGrid(0) == childGrid1);
	gridCollection->removeGrid(0);
	assert(gridCollection->getNumberOfGrids() == 0);
	gridCollection->insert(childGrid1);
	gridCollection->insert(childGrid2);

	gridCollection->setGridCollectionType(XdmfGridCollectionType::Temporal());


	boost::shared_ptr<XdmfWriter> writer = XdmfWriter::New("TestXdmfGridCollection1.xmf");
	gridCollection->accept(writer);

	boost::shared_ptr<XdmfReader> reader = XdmfReader::New();
	boost::shared_ptr<XdmfGridCollection> gridCollection2 = boost::shared_dynamic_cast<XdmfGridCollection>(reader->read("TestXdmfGridCollection1.xmf"));

	boost::shared_ptr<XdmfWriter> writer2 = XdmfWriter::New("TestXdmfGridCollection2.xmf");
	gridCollection2->accept(writer2);

	// Compare two files for equality
	std::ifstream firstFile("TestXdmfGridCollection1.xmf");
	std::ifstream secondFile("TestXdmfGridCollection2.xmf");

	std::stringstream firstBuffer;
	std::stringstream secondBuffer;

	firstBuffer << firstFile.rdbuf();
	secondBuffer << secondFile.rdbuf();

	std::string firstContents(firstBuffer.str());
	std::string secondContents(secondBuffer.str());

	assert(firstContents.compare(secondContents) == 0);

	return 0;
}
