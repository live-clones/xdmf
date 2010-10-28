#include "XdmfDomain.hpp"
#include "XdmfHDF5Writer.hpp"
#include "XdmfWriter.hpp"

#include "XdmfTestDataGenerator.hpp"

int main(int, char *)
{
	boost::shared_ptr<XdmfUnstructuredGrid> grid = XdmfTestDataGenerator::createHexahedron();

	// First write and release heavy data
	boost::shared_ptr<XdmfHDF5Writer> hdf5Writer = XdmfHDF5Writer::New("output.h5");
	grid->getGeometry()->accept(hdf5Writer);
	grid->getGeometry()->release();

	grid->getTopology()->accept(hdf5Writer);
	grid->getTopology()->release();

	for(unsigned int i=0; i<grid->getNumberAttributes(); ++i)
	{
		grid->getAttribute(i)->accept(hdf5Writer);
		grid->getAttribute(i)->release();
	}

	// Now insert into domain and write light data
	boost::shared_ptr<XdmfDomain> domain = XdmfDomain::New();
	domain->insert(grid);

	boost::shared_ptr<XdmfWriter> writer = XdmfWriter::New("output.xmf", hdf5Writer);
	writer->setLightDataLimit(10);
	domain->accept(writer);

	return 0;
}
