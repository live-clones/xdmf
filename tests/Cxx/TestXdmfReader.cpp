#include "XdmfDomain.hpp"
#include "XdmfReader.hpp"
#include "XdmfWriter.hpp"

#include "XdmfTestCompareFiles.hpp"
#include "XdmfTestDataGenerator.hpp"

int main(int argc, char* argv[])
{
	boost::shared_ptr<XdmfWriter> writer = XdmfWriter::New("TestXdmfReader1.xmf");
	writer->setLightDataLimit(10);

	boost::shared_ptr<XdmfGrid> grid1 = XdmfTestDataGenerator::createHexahedron();
	boost::shared_ptr<XdmfGrid> grid2 = XdmfTestDataGenerator::createHexahedron();

	boost::shared_ptr<XdmfDomain> domain = XdmfDomain::New();
	domain->insert(grid1);
	domain->insert(grid2);
	domain->accept(writer);

	boost::shared_ptr<XdmfReader> reader = XdmfReader::New();
	boost::shared_ptr<XdmfDomain> readDomain = boost::shared_dynamic_cast<XdmfDomain>(reader->read("TestXdmfReader1.xmf"));

	boost::shared_ptr<XdmfWriter> writer2 = XdmfWriter::New("TestXdmfReader2.xmf");
	writer2->setMode(XdmfWriter::DistributedHeavyData);
	readDomain->accept(writer2);

	assert(XdmfTestCompareFiles::compareFiles("TestXdmfReader1.xmf", "TestXdmfReader2.xmf"));

	std::vector<boost::shared_ptr<XdmfItem> > readItems = reader->read("TestXdmfReader1.xmf", "/Xdmf/Domain/Grid[1]");
	assert(readItems.size() == 1);
	boost::shared_ptr<XdmfGrid> readGrid = boost::shared_dynamic_cast<XdmfGrid>(readItems[0]);
	assert(readGrid->getName().compare("Hexahedron") == 0);

	std::vector<boost::shared_ptr<XdmfItem> > readItems2 = reader->read("TestXdmfReader1.xmf", "//Attribute");
	assert(readItems2.size() == 4);
	boost::shared_ptr<XdmfAttribute> readAttribute = boost::shared_dynamic_cast<XdmfAttribute>(readItems2[0]);
	assert(readAttribute->getName().compare("Nodal Attribute") == 0 || readAttribute->getName().compare("Cell Attribute"));
	return 0;
}
