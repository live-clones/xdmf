#include "XdmfAttribute.hpp"
#include "XdmfDomain.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfTopology.hpp"
#include "XdmfUnstructuredGrid.hpp"
#include "XdmfInformation.hpp"
#include "XdmfSet.hpp"
#include "XdmfTime.hpp"
#include "XdmfHeavyDataWriter.hpp"
#include "XdmfHDF5Controller.hpp"
#include "XdmfHDF5Writer.hpp"
#include "XdmfWriter.hpp"
#include "XdmfReader.hpp"
#include "stdio.h"
#include <sstream>

int main(int, char **)
{
  shared_ptr<XdmfHDF5Writer> hdf5writer1 = XdmfHDF5Writer::New("attributefile.h5");
  shared_ptr<XdmfHDF5Writer> hdf5writer2 = XdmfHDF5Writer::New("setfile.h5");

  shared_ptr<XdmfDomain> domain = XdmfDomain::New();

  shared_ptr<XdmfGeometry> geometry = XdmfGeometry::New();

  shared_ptr<XdmfTopology> topology = XdmfTopology::New();

  unsigned int numGrids = 20;

  std::stringstream attributeName;
  std::stringstream setName;

  hdf5writer1->openFile();
  hdf5writer2->openFile();

  for (unsigned int i = 0; i < numGrids; ++i)
  {
    shared_ptr<XdmfUnstructuredGrid> grid = XdmfUnstructuredGrid::New();
    std::string gridName = "Grid" + i;

    grid->setName(gridName);
    assert(grid->getName().compare(gridName) == 0);

    attributeName << "Grid" << i << " Attribute";

    shared_ptr<XdmfAttribute> attr = XdmfAttribute::New();
    attr->setName(attributeName.str());
    attr->resize<int>(2000, 0);
    attr->accept(hdf5writer1);
    grid->insert(attr);

    attributeName.str(std::string());

    setName << "Grid" << i << " Set";

    shared_ptr<XdmfSet> set = XdmfSet::New();
    set->setName(setName.str());
    set->resize<int>(2000, 0);
    set->accept(hdf5writer2);
    grid->insert(set);

    setName.str(std::string());

    shared_ptr<XdmfTime> time = XdmfTime::New(i);
    grid->setTime(time);

    grid->setTopology(topology);
    grid->setGeometry(geometry);

    domain->insert(grid);
  }

  hdf5writer1->closeFile();
  hdf5writer2->closeFile();

  printf("data written to file\n");

  for (unsigned int i = 0; i < numGrids; ++i)
  {
    domain->getUnstructuredGrid(i)->getAttribute(0)->read();
    domain->getUnstructuredGrid(i)->getSet(0)->read();
  }

  XdmfHDF5Controller::closeFiles();

  for (unsigned int i = 0; i < numGrids; ++i)
  {
    domain->getUnstructuredGrid(i)->getAttribute(0)->release();
    domain->getUnstructuredGrid(i)->getSet(0)->release();
  }

  XdmfHDF5Controller::setMaxOpenedFiles(2);

  for (unsigned int i = 0; i < numGrids; ++i)
  {
    domain->getUnstructuredGrid(i)->getAttribute(0)->read();
    domain->getUnstructuredGrid(i)->getSet(0)->read();
  }

  XdmfHDF5Controller::closeFiles();

  for (unsigned int i = 0; i < numGrids; ++i)
  {
    domain->getUnstructuredGrid(i)->getAttribute(0)->release();
    domain->getUnstructuredGrid(i)->getSet(0)->release();
  }

  return 0;
}

