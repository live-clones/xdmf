#include "XdmfAttribute.hpp"
#include "XdmfAttributeType.hpp"
#include "XdmfAttributeCenter.hpp"
#include "XdmfArray.hpp"
#include "XdmfDomain.hpp"
#include "XdmfError.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfGeometryType.hpp"
#include "XdmfGridCollection.hpp"
#include "XdmfGridController.hpp"
#include "XdmfCurvilinearGrid.hpp"
#include "XdmfRectilinearGrid.hpp"
#include "XdmfRegularGrid.hpp"
#include "XdmfUnstructuredGrid.hpp"
#include "XdmfTopology.hpp"
#include "XdmfTopologyType.hpp"
#include "XdmfInformation.hpp"
#include "XdmfReader.hpp"
#include "XdmfWriter.hpp"
#include <iostream>
#include <assert.h>

int main(int, char **)
{
  shared_ptr<XdmfDomain> domain = XdmfDomain::New();

  shared_ptr<XdmfGridCollection> collection = XdmfGridCollection::New();

  for (unsigned int i = 0; i < 5; ++i)
  {
    shared_ptr<XdmfUnstructuredGrid> colgrid = XdmfUnstructuredGrid::New();

    collection->insert(colgrid);


    shared_ptr<XdmfGeometry> colgeometry = XdmfGeometry::New();

    for (unsigned int j = 0; j < 9; ++j)
    {
      colgeometry->pushBack(j+i);
    }

    colgeometry->setType(XdmfGeometryType::XYZ());

    colgrid->setGeometry(colgeometry);

    shared_ptr<XdmfTopology> coltopology = XdmfTopology::New();

    for (unsigned int j = 0; j < 3; ++j)
    {
      coltopology->pushBack(j);
    }

    coltopology->setType(XdmfTopologyType::Triangle());

    colgrid->setTopology(coltopology);

    shared_ptr<XdmfAttribute> colattr = XdmfAttribute::New();

    for (unsigned int j = 0; j < 3; ++j)
    {
      colattr->pushBack(j+i);
    }

    colattr->setType(XdmfAttributeType::Scalar());
    colattr->setCenter(XdmfAttributeCenter::Node());

    colgrid->insert(colattr);
  }

  domain->insert(collection);

  shared_ptr<XdmfCurvilinearGrid> curvgrid = XdmfCurvilinearGrid::New(10, 10);

  shared_ptr<XdmfGeometry> curvgeometry = XdmfGeometry::New();

  for (unsigned int i = 0; i < 9; ++i)
  {
    curvgeometry->pushBack(i);
  }

  curvgeometry->setType(XdmfGeometryType::XYZ());

  curvgrid->setGeometry(curvgeometry);

  shared_ptr<XdmfAttribute> curvattr = XdmfAttribute::New();

  for (unsigned int i = 0; i < 100; ++i)
  {
    curvattr->pushBack(i);
  }

  curvattr->setType(XdmfAttributeType::Scalar());
  curvattr->setCenter(XdmfAttributeCenter::Node());

  curvgrid->insert(curvattr);

  domain->insert(curvgrid);

  shared_ptr<XdmfArray> xCoordinates = XdmfArray::New();
  shared_ptr<XdmfArray> yCoordinates = XdmfArray::New();
  for (unsigned int i = 0; i < 10; ++i)
  {
    xCoordinates->pushBack(i);
    yCoordinates->pushBack(i);
  }

  shared_ptr<XdmfRectilinearGrid> rectgrid = XdmfRectilinearGrid::New(xCoordinates, yCoordinates);

  domain->insert(rectgrid);

  shared_ptr<XdmfAttribute> rectattr = XdmfAttribute::New();

  for (unsigned int i = 0; i < 100; ++i)
  {
    rectattr->pushBack(i);
  }

  rectattr->setType(XdmfAttributeType::Scalar());
  rectattr->setCenter(XdmfAttributeCenter::Node());

  rectgrid->insert(rectattr);

  shared_ptr<XdmfRegularGrid> reggrid = XdmfRegularGrid::New(1, 1, 10, 10, 0, 0);

  domain->insert(reggrid);

  shared_ptr<XdmfAttribute> regattr = XdmfAttribute::New();

  for (unsigned int i = 0; i < 100; ++i)
  {
    regattr->pushBack(i);
  }

  regattr->setType(XdmfAttributeType::Scalar());
  regattr->setCenter(XdmfAttributeCenter::Node());

  reggrid->insert(regattr);

  shared_ptr<XdmfUnstructuredGrid> ungrid = XdmfUnstructuredGrid::New();

  domain->insert(ungrid);

  shared_ptr<XdmfGeometry> ungeometry = XdmfGeometry::New();

  for (unsigned int i = 0; i < 9; ++i)
  {
    ungeometry->pushBack(i);
  }

  ungeometry->setType(XdmfGeometryType::XYZ());

  ungrid->setGeometry(ungeometry);

  shared_ptr<XdmfTopology> untopology = XdmfTopology::New();

  for (unsigned int i = 0; i < 3; ++i)
  {
    untopology->pushBack(i);
  }

  untopology->setType(XdmfTopologyType::Triangle());

  ungrid->setTopology(untopology);

  shared_ptr<XdmfAttribute> unattr = XdmfAttribute::New();

  for (unsigned int i = 0; i < 3; ++i)
  {
    unattr->pushBack(i);
  }

  unattr->setType(XdmfAttributeType::Scalar());
  unattr->setCenter(XdmfAttributeCenter::Node());

  ungrid->insert(unattr);

//  shared_ptr<XdmfCurvilinearGrid> curvgrid = XdmfCurvilinearGrid::New();

  shared_ptr<XdmfWriter> writer = XdmfWriter::New("gridControllerReference.xmf");

  domain->accept(writer);

  std::cout << "Finished writing reference file" << std::endl;

/*
XdmfGridCollection
XdmfCurvilinearGrid
XdmfGraph
XdmfRectilinearGrid
XdmfRegularGrid
XdmfUnstructuredGrid
*/


  shared_ptr<XdmfDomain> equalDomain = XdmfDomain::New();

  shared_ptr<XdmfGridCollection> equalCollection = XdmfGridCollection::New();

  shared_ptr<XdmfGridController> collectionController = XdmfGridController::New("gridControllerReference.xmf", "/Xdmf/Domain/Grid[1]");

  equalCollection->setGridController(collectionController);

  equalDomain->insert(equalCollection);

  shared_ptr<XdmfCurvilinearGrid> equalCurvGrid = XdmfCurvilinearGrid::New(0, 0);

  shared_ptr<XdmfGridController> curvGridController = XdmfGridController::New("gridControllerReference.xmf", "/Xdmf/Domain/Grid[2]");

  equalCurvGrid->setGridController(curvGridController);

  equalDomain->insert(equalCurvGrid);

  shared_ptr<XdmfArray> placeholderXArray = XdmfArray::New();
  shared_ptr<XdmfArray> placeholderYArray = XdmfArray::New();

  shared_ptr<XdmfRectilinearGrid> equalRectGrid = XdmfRectilinearGrid::New(placeholderXArray,
                                                                           placeholderYArray);

  shared_ptr<XdmfGridController> rectGridController = XdmfGridController::New("gridControllerReference.xmf", "/Xdmf/Domain/Grid[3]");

  equalRectGrid->setGridController(rectGridController);

  equalDomain->insert(equalRectGrid);

  shared_ptr<XdmfRegularGrid> equalRegGrid = XdmfRegularGrid::New(0, 0, 0, 0, 0, 0);

  shared_ptr<XdmfGridController> regGridController = XdmfGridController::New("gridControllerReference.xmf", "/Xdmf/Domain/Grid[4]");

  equalRegGrid->setGridController(regGridController);

  equalDomain->insert(equalRegGrid);

  shared_ptr<XdmfUnstructuredGrid> equalUnGrid = XdmfUnstructuredGrid::New();

  shared_ptr<XdmfGridController> unGridController = XdmfGridController::New("gridControllerReference.xmf", "/Xdmf/Domain/Grid[5]");

  equalUnGrid->setGridController(unGridController);

  equalDomain->insert(equalUnGrid);

  shared_ptr<XdmfWriter> controllerWriter = XdmfWriter::New("gridController.xmf");

  equalDomain->accept(controllerWriter);










  shared_ptr<XdmfReader> reader = XdmfReader::New();

  shared_ptr<XdmfDomain> readReference = shared_dynamic_cast<XdmfDomain>(reader->read("gridControllerReference.xmf"));

  shared_ptr<XdmfDomain> readControllerDomain = shared_dynamic_cast<XdmfDomain>(reader->read("gridController.xmf"));

  shared_ptr<XdmfGridCollection> collectionReference = readReference->getGridCollection(0);

  shared_ptr<XdmfGridCollection> compareCollection = readControllerDomain->getGridCollection(0);

  compareCollection->read();

  std::cout << compareCollection->getNumberUnstructuredGrids() << " ?= " << collectionReference->getNumberUnstructuredGrids() << std::endl;

  assert(compareCollection->getNumberUnstructuredGrids() == collectionReference->getNumberUnstructuredGrids());


  for (unsigned int i = 0; i < compareCollection->getNumberUnstructuredGrids(); ++i)
  {
    shared_ptr<XdmfUnstructuredGrid> unCollectionReference = collectionReference->getUnstructuredGrid(i);
    shared_ptr<XdmfUnstructuredGrid> unCollectionRead = compareCollection->getUnstructuredGrid(i);

    std::cout << unCollectionRead->getGeometry()->getValuesString() << " ?= " << unCollectionReference->getGeometry()->getValuesString() << std::endl;

    assert(unCollectionRead->getGeometry()->getValuesString().compare(unCollectionReference->getGeometry()->getValuesString()) == 0);

    std::cout << unCollectionRead->getTopology()->getValuesString() << " ?= " << unCollectionReference->getTopology()->getValuesString() << std::endl;

    assert(unCollectionRead->getTopology()->getValuesString().compare(unCollectionReference->getTopology()->getValuesString()) == 0);

    std::cout << unCollectionRead->getAttribute(0)->getValuesString() << " ?= " << unCollectionReference->getAttribute(0)->getValuesString() << std::endl;

    assert(unCollectionRead->getAttribute(0)->getValuesString().compare(unCollectionReference->getAttribute(0)->getValuesString()) == 0);
  }

  compareCollection->release();




  shared_ptr<XdmfUnstructuredGrid> unCompareReference = readReference->getUnstructuredGrid(0);

  shared_ptr<XdmfUnstructuredGrid> unCompare = readControllerDomain->getUnstructuredGrid(0);

  unCompare->read();

  std::cout << unCompare->getGeometry()->getValuesString() << " ?= " << unCompareReference->getGeometry()->getValuesString() << std::endl;

  assert(unCompare->getGeometry()->getValuesString().compare(unCompareReference->getGeometry()->getValuesString()) == 0);

  std::cout << unCompare->getTopology()->getValuesString() << " ?= " << unCompareReference->getTopology()->getValuesString() << std::endl;

  assert(unCompare->getTopology()->getValuesString().compare(unCompareReference->getTopology()->getValuesString()) == 0);

  std::cout << unCompare->getAttribute(0)->getValuesString() << " ?= " << unCompareReference->getAttribute(0)->getValuesString() << std::endl;

  assert(unCompare->getAttribute(0)->getValuesString().compare(unCompareReference->getAttribute(0)->getValuesString()) == 0);

  unCompare->release();

  shared_ptr<XdmfRegularGrid> regCompareReference = readReference->getRegularGrid(0);

  shared_ptr<XdmfRegularGrid> regCompare = readControllerDomain->getRegularGrid(0);

  regCompare->read();

  std::cout << regCompare->getOrigin()->getValuesString() << " ?= " << regCompareReference->getOrigin()->getValuesString() << std::endl;

  assert(regCompare->getOrigin()->getValuesString().compare(regCompareReference->getOrigin()->getValuesString()) == 0);

  std::cout << regCompare->getDimensions()->getValuesString() << " ?= " << regCompareReference->getDimensions()->getValuesString() << std::endl;

  assert(regCompare->getDimensions()->getValuesString().compare(regCompareReference->getDimensions()->getValuesString()) == 0);

  std::cout << regCompare->getBrickSize()->getValuesString() << " ?= " << regCompareReference->getBrickSize()->getValuesString() << std::endl;

  assert(regCompare->getBrickSize()->getValuesString().compare(regCompareReference->getBrickSize()->getValuesString()) == 0);

  std::cout << regCompare->getAttribute(0)->getValuesString() << " ?= " << regCompareReference->getAttribute(0)->getValuesString() << std::endl;

  assert(regCompare->getAttribute(0)->getValuesString().compare(regCompareReference->getAttribute(0)->getValuesString()) == 0);

  regCompare->release();

  shared_ptr<XdmfRectilinearGrid> rectCompareReference = readReference->getRectilinearGrid(0);

  shared_ptr<XdmfRectilinearGrid> rectCompare = readControllerDomain->getRectilinearGrid(0);

  rectCompare->read();

  std::cout << rectCompare->getAttribute(0)->getValuesString() << " ?= " << rectCompareReference->getAttribute(0)->getValuesString() << std::endl;

  assert(rectCompare->getAttribute(0)->getValuesString().compare(rectCompareReference->getAttribute(0)->getValuesString()) == 0);

  std::cout << rectCompare->getCoordinates(0)->getValuesString() << " ?= " << rectCompareReference->getCoordinates(0)->getValuesString() << std::endl;

  assert(rectCompare->getCoordinates(0)->getValuesString().compare(rectCompareReference->getCoordinates(0)->getValuesString()) == 0);

  std::cout << rectCompare->getCoordinates(1)->getValuesString() << " ?= " << rectCompareReference->getCoordinates(1)->getValuesString() << std::endl;

  assert(rectCompare->getCoordinates(1)->getValuesString().compare(rectCompareReference->getCoordinates(1)->getValuesString()) == 0);

  rectCompare->release();

  shared_ptr<XdmfCurvilinearGrid> curvCompareReference = readReference->getCurvilinearGrid(0);

  shared_ptr<XdmfCurvilinearGrid> curvCompare = readControllerDomain->getCurvilinearGrid(0);

  curvCompare->read();

  std::cout << curvCompare->getGeometry()->getValuesString() << " ?= " << curvCompareReference->getGeometry()->getValuesString() << std::endl;

  assert(curvCompare->getGeometry()->getValuesString().compare(curvCompareReference->getGeometry()->getValuesString()) == 0);

  std::cout << curvCompare->getAttribute(0)->getValuesString() << " ?= " << curvCompareReference->getAttribute(0)->getValuesString() << std::endl;

  assert(curvCompare->getAttribute(0)->getValuesString().compare(curvCompareReference->getAttribute(0)->getValuesString()) == 0);

  std::cout << curvCompare->getDimensions()->getValuesString() << " ?= " << curvCompareReference->getDimensions()->getValuesString() << std::endl;

  assert(curvCompare->getDimensions()->getValuesString().compare(curvCompareReference->getDimensions()->getValuesString()) == 0);

  curvCompare->release();




  // Error Checking

  shared_ptr<XdmfDomain> errorDomain = XdmfDomain::New();

  shared_ptr<XdmfUnstructuredGrid> errorUnGrid = XdmfUnstructuredGrid::New();

  shared_ptr<XdmfGridController> errorUnGridController = XdmfGridController::New("gridControllerReference.xmf", "/Xdmf/Domain/Grid[3]");

  errorUnGrid->setGridController(errorUnGridController);

  errorDomain->insert(errorUnGrid);

  shared_ptr<XdmfWriter> errorControllerWriter = XdmfWriter::New("errorGridController.xmf");

  errorDomain->accept(errorControllerWriter);



  shared_ptr<XdmfDomain> readErrorDomain = shared_dynamic_cast<XdmfDomain>(reader->read("errorGridController.xmf"));

  try
  {
    readErrorDomain->getUnstructuredGrid(0)->read();
  }
  catch (XdmfError e)
  {
    std::cout << "Intentional Error" << std::endl;
  }

  return 0;
}
