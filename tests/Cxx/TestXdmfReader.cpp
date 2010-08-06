#include "XdmfDomain.hpp"
#include "XdmfReader.hpp"
#include "XdmfWriter.hpp"

#include "XdmfTestCompareFiles.hpp"
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
	writer2->setMode(XdmfWriter::DistributedHeavyData);
	readDomain->accept(writer2);

	assert(XdmfTestCompareFiles::compareFiles("TestXdmfReader1.xmf", "TestXdmfReader2.xmf"));

	return 0;
}
