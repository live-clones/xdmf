#include "XdmfDomain.hpp"
#include "XdmfExodusReader.hpp"
#include "XdmfExodusWriter.hpp"
#include "XdmfHDF5Writer.hpp"
#include "XdmfWriter.hpp"

#include "XdmfTestDataGenerator.hpp"

int main(int, char *)
{
  shared_ptr<XdmfExodusWriter> exodusWriter = XdmfExodusWriter::New();
  shared_ptr<XdmfUnstructuredGrid> hexahedron =
    XdmfTestDataGenerator::createHexahedron();
  exodusWriter->write("TestXdmfExodusIO.exo", hexahedron);

  shared_ptr<XdmfExodusReader> reader = XdmfExodusReader::New();
  shared_ptr<XdmfUnstructuredGrid> grid =
    reader->read("TestXdmfExodusIO.exo");
  assert(grid->getName() == hexahedron->getName());
  assert(grid->getGeometry()->getType() ==
         hexahedron->getGeometry()->getType());
  assert(grid->getGeometry()->getNumberPoints() ==
         hexahedron->getGeometry()->getNumberPoints());
  for(unsigned int i=0; i<grid->getGeometry()->getSize(); ++i) {
    assert(grid->getGeometry()->getValue<double>(i) ==
           hexahedron->getGeometry()->getValue<double>(i));
  }
  assert(grid->getTopology()->getType() ==
         hexahedron->getTopology()->getType());
  assert(grid->getTopology()->getNumberElements() ==
         hexahedron->getTopology()->getNumberElements());
  for(unsigned int i=0; i<grid->getTopology()->getSize(); ++i) {
    assert(grid->getTopology()->getValue<double>(i) ==
           hexahedron->getTopology()->getValue<double>(i));
  }
  assert(hexahedron->getNumberAttributes() + 1 == grid->getNumberAttributes());
  for(unsigned int i=0; i<hexahedron->getNumberAttributes(); ++i) {
    shared_ptr<XdmfAttribute> attribute1 = hexahedron->getAttribute(i);
    shared_ptr<XdmfAttribute> attribute2 =
      grid->getAttribute(attribute1->getName());
    assert(attribute1->getCenter() == attribute2->getCenter());
    assert(attribute1->getType() == attribute2->getType());
    assert(attribute1->getSize() == attribute2->getSize());
    for(unsigned int j=0; j<attribute1->getSize(); ++j) {
      assert(attribute1->getValue<double>(i) ==
             attribute2->getValue<double>(i));
    }
  }
}
