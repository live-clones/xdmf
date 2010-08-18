#include "XdmfDomain.hpp"
#include "XdmfExodusReader.hpp"
#include "XdmfExodusWriter.hpp"
#include "XdmfHDF5Writer.hpp"
#include "XdmfWriter.hpp"

#include "XdmfTestDataGenerator.hpp"

int main(int argc, char* argv[])
{
	boost::shared_ptr<XdmfExodusWriter> exodusWriter = XdmfExodusWriter::New();
	boost::shared_ptr<XdmfGrid> hexahedron = XdmfTestDataGenerator::createHexahedron();
	exodusWriter->write("TestXdmfExodusIO.exo", hexahedron);

	boost::shared_ptr<XdmfExodusReader> reader = XdmfExodusReader::New();
	boost::shared_ptr<XdmfGrid> grid = reader->read("TestXdmfExodusIO.exo");
	assert(grid->getName() == hexahedron->getName());
	assert(grid->getGeometry()->getType() == hexahedron->getGeometry()->getType());
	assert(grid->getGeometry()->getNumberPoints() == hexahedron->getGeometry()->getNumberPoints());
	for(unsigned int i=0; i<grid->getGeometry()->size(); ++i)
	{
		assert(grid->getGeometry()->getValueCopy<double>(i) == hexahedron->getGeometry()->getValueCopy<double>(i));
	}
	assert(grid->getTopology()->getType() == hexahedron->getTopology()->getType());
	assert(grid->getTopology()->getNumberElements() == hexahedron->getTopology()->getNumberElements());
	for(unsigned int i=0; i<grid->getTopology()->size(); ++i)
	{
		assert(grid->getTopology()->getValueCopy<double>(i) == hexahedron->getTopology()->getValueCopy<double>(i));
	}
	assert(hexahedron->getNumberAttributes() + 1 == grid->getNumberAttributes());
	for(unsigned int i=0; i<hexahedron->getNumberAttributes(); ++i)
	{
		boost::shared_ptr<XdmfAttribute> attribute1 = hexahedron->getAttribute(i);
		boost::shared_ptr<XdmfAttribute> attribute2 = grid->getAttribute(attribute1->getName());
		assert(attribute1->getCenter() == attribute2->getCenter());
		assert(attribute1->getType() == attribute2->getType());
		assert(attribute1->size() == attribute2->size());
		for(unsigned int j=0; j<attribute1->size(); ++j)
		{
			assert(attribute1->getValueCopy<double>(i) == attribute2->getValueCopy<double>(i));
		}
	}
}
