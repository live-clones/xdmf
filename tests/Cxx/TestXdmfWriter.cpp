#include "XdmfAttribute.hpp"
#include "XdmfDomain.hpp"
#include "XdmfReader.hpp"
#include "XdmfSystemUtils.hpp"
#include "XdmfWriter.hpp"
#include <iostream>

#include "XdmfTestDataGenerator.hpp"

namespace {

  void checkFile(shared_ptr<XdmfDomain> domain,
		 shared_ptr<XdmfUnstructuredGrid> grid)
  {

    shared_ptr<XdmfReader> reader = XdmfReader::New();
    shared_ptr<XdmfDomain> readDomain = 
      shared_dynamic_cast<XdmfDomain>(reader->read("output.xmf"));
    assert(readDomain->getNumberUnstructuredGrids() == 
	   domain->getNumberUnstructuredGrids());
    shared_ptr<XdmfUnstructuredGrid> readGrid = 
      readDomain->getUnstructuredGrid(0);
    const unsigned int readNumberAttributes = readGrid->getNumberAttributes();
    assert(readNumberAttributes == grid->getNumberAttributes());

    shared_ptr<XdmfTopology> readTopology = readGrid->getTopology();
    shared_ptr<XdmfTopology> topology = grid->getTopology();
    if(!readTopology->isInitialized()) {
      readTopology->read();
    }
    const std::string readTopologyString = readTopology->getValuesString();
    const std::string topologyString = topology->getValuesString();
    std::cout << readTopologyString << " ?= " << topologyString << std::endl;
    assert(readTopologyString.compare(topologyString) == 0);

    shared_ptr<XdmfGeometry> readGeometry = readGrid->getGeometry();
    shared_ptr<XdmfGeometry> geometry = grid->getGeometry();
    if(!readGeometry->isInitialized()) {
      readGeometry->read();
    }
    const std::string readGeometryString = readGeometry->getValuesString();
    const std::string geometryString = geometry->getValuesString();
    std::cout << readGeometryString << " ?= " << geometryString << std::endl;
    assert(readGeometryString.compare(geometryString) == 0);

    for(unsigned int i=0; i<readNumberAttributes; ++i) {
      shared_ptr<XdmfAttribute> readAttribute = readGrid->getAttribute(i);
      shared_ptr<XdmfAttribute> attribute = 
	grid->getAttribute(readAttribute->getName());
      if(readAttribute->isInitialized() == 0) {
	readAttribute->read();
      }
      assert(readAttribute->getSize() == attribute->getSize());
      assert(readAttribute->getType() == attribute->getType());
      assert(readAttribute->getCenter() == attribute->getCenter());
      assert(readAttribute->getArrayType() == attribute->getArrayType());
      const std::string readAttributeString = readAttribute->getValuesString();
      const std::string attributeString = attribute->getValuesString();
      std::cout << readAttributeString << " ?= " << attributeString 
		<< std::endl;
      assert(readAttributeString.compare(attributeString) == 0);
    }

  }

}

int main(int, char **)
{
  shared_ptr<XdmfWriter> writer = XdmfWriter::New("output.xmf");

  std::string realPath = XdmfSystemUtils::getRealPath("output.xmf");

  std::cout << writer->getFilePath() << " ?= " << realPath << std::endl;

  assert(writer->getFilePath().compare(realPath) == 0);
  writer->setLightDataLimit(10);

  std::cout << writer->getLightDataLimit() << " ?= " << 10 << std::endl;

  assert(writer->getLightDataLimit() == 10);

  shared_ptr<XdmfUnstructuredGrid> grid =
    XdmfTestDataGenerator::createHexahedron();

  shared_ptr<XdmfDomain> domain = XdmfDomain::New();
  domain->insert(grid);
  domain->accept(writer);
  checkFile(domain, grid);

  //
  // test rebuilding functionality
  //
  assert(writer->getRebuildXML() == 1);
  writer->setRebuildXML(0);
  assert(writer->getRebuildXML() == 0);
  domain->accept(writer);  
  checkFile(domain, grid);
  domain->accept(writer);  
  checkFile(domain, grid);

  //
  // update grid and ensure updated xml is generated
  //
  shared_ptr<XdmfAttribute> newAttribute = XdmfAttribute::New();
  newAttribute->setCenter(XdmfAttributeCenter::Grid());
  newAttribute->setType(XdmfAttributeType::Scalar());
  newAttribute->setName("New Attribute");
  newAttribute->pushBack<double>(10.0);
  grid->insert(newAttribute);
  domain->accept(writer);
  checkFile(domain, grid);

  //
  // this breaks! the XML caching appears to be half implemented -
  // this should trigger a call to the parent of the geometry (the
  // grid) and set the mIsChanged flag in both. However, none of the
  // parent registration has been implemented, despite many mChanged
  // flags being sprinkled everywhere in the code... Comment out for
  // now until this is fixed
  //
  /*
  grid->getGeometry()->insert<double>(0, 0.2);
  domain->accept(writer);
  checkFile(domain, grid);
  */

  return 0;
}
