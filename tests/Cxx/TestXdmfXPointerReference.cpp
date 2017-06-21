#include <iostream>
#include <XdmfDomain.hpp>
#include <XdmfGridCollection.hpp>
#include <XdmfWriter.hpp>
#include <XdmfUnstructuredGrid.hpp>
#include <XdmfTime.hpp>
#include <XdmfReader.hpp>

int main(int ac, char *av[])
{
  shared_ptr<XdmfReader> reader = XdmfReader::New();

  shared_ptr<XdmfDomain> domain = XdmfDomain::New();

  shared_ptr<XdmfGridCollection> collection1 = XdmfGridCollection::New();
  shared_ptr<XdmfGridCollection> collection2 = XdmfGridCollection::New();

  shared_ptr<XdmfUnstructuredGrid> unstructuredGrid = XdmfUnstructuredGrid::New();

  collection1->insert(unstructuredGrid);
  collection2->insert(unstructuredGrid);
  domain->insert(collection1);
  domain->insert(collection2);

  shared_ptr<XdmfWriter> writer = XdmfWriter::New("duplicateXpointer.xmf");

  domain->accept(writer);

  shared_ptr<XdmfDomain> readDomain = shared_dynamic_cast<XdmfDomain>(reader->read("duplicateXpointer.xmf"));

  assert(readDomain->getGridCollection(0)->getUnstructuredGrid(0) == readDomain->getGridCollection(1)->getUnstructuredGrid(0));

  assert(readDomain->getGridCollection(0)->getUnstructuredGrid(0).get() == readDomain->getGridCollection(1)->getUnstructuredGrid(0).get());

  readDomain->getGridCollection(0)->getUnstructuredGrid(0)->setName("Test Grid");

  shared_ptr<XdmfTime> time = XdmfTime::New(5.5);

  readDomain->getGridCollection(1)->getUnstructuredGrid(0)->setTime(time);

  shared_ptr<XdmfWriter> writer2 = XdmfWriter::New("duplicateXpointer2.xmf");

  readDomain->accept(writer2);

  shared_ptr<XdmfDomain> readDomain2 = shared_dynamic_cast<XdmfDomain>(reader->read("duplicateXpointer2.xmf"));

  std::cout << readDomain2->getGridCollection(1)->getUnstructuredGrid(0)->getName() << " ?= Test Grid" << std::endl;

  std::cout << readDomain2->getGridCollection(0)->getUnstructuredGrid(0)->getTime()->getValue() << " ?= " << 5.5 << std::endl;

  assert(readDomain2->getGridCollection(1)->getUnstructuredGrid(0)->getName() =="Test Grid");

  assert(readDomain2->getGridCollection(0)->getUnstructuredGrid(0)->getTime()->getValue() == 5.5);


  return 0;
}
