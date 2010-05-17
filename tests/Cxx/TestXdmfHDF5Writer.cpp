#include "XdmfHDF5Writer.hpp"

#include "XdmfTestDataGenerator.hpp"

int main(int argc, char* argv[])
{
	boost::shared_ptr<XdmfHDF5Writer> writer = XdmfHDF5Writer::New("hdf5WriterTest.h5");

	boost::shared_ptr<XdmfGrid> grid = XdmfTestDataGenerator::createHexahedron();

	boost::shared_ptr<XdmfDomain> domain = XdmfDomain::New();
	domain->insert(grid);
	domain->accept(writer);

	return 0;
}
