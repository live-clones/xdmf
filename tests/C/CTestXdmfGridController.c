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
#include "stdio.h"

#include "assert.h"
#include "string.h"

int main()
{
  int status = 0;
  unsigned int i, j;
  XDMFDOMAIN * domain = XdmfDomainNew();

  XDMFGRIDCOLLECTION * collection = XdmfGridCollectionNew();

  for (i = 0; i < 5; ++i)
  {
    XDMFUNSTRUCTUREDGRID * colgrid = XdmfUnstructuredGridNew();

    XdmfGridCollectionInsertUnstructuredGrid(collection, colgrid, 1);

    XDMFGEOMETRY * colgeometry = XdmfGeometryNew();

    for (j = 0; j < 9; ++j)
    {
      unsigned int val = j+i;
      XdmfGeometryPushBack(colgeometry, &val, XDMF_ARRAY_TYPE_UINT32, &status);
    }

    XdmfGeometrySetType(colgeometry, XDMF_GEOMETRY_TYPE_XYZ, &status);

    XdmfUnstructuredGridSetGeometry(colgrid, colgeometry, 1);

    XDMFTOPOLOGY * coltopology = XdmfTopologyNew();

    for (j = 0; j < 3; ++j)
    {
      XdmfTopologyPushBack(coltopology, &j, XDMF_ARRAY_TYPE_UINT32, &status);
    }

    XdmfTopologySetType(coltopology, XDMF_TOPOLOGY_TYPE_TRIANGLE, &status);

    XdmfUnstructuredGridSetTopology(colgrid, coltopology, 1);

    XDMFATTRIBUTE * colattr = XdmfAttributeNew();

    for (j = 0; j < 3; ++j)
    {
      unsigned int val = j+i;
      XdmfAttributePushBack(colattr, &val, XDMF_ARRAY_TYPE_UINT32, &status);
    }

    XdmfAttributeSetType(colattr, XDMF_ATTRIBUTE_TYPE_SCALAR, &status);
    XdmfAttributeSetCenter(colattr, XDMF_ATTRIBUTE_CENTER_NODE, &status);

    XdmfUnstructuredGridInsertAttribute(colgrid, colattr, 1);
  }

  XdmfDomainInsertGridCollection(domain, collection, 1);

  XDMFCURVILINEARGRID * curvgrid = XdmfCurvilinearGridNew2D(10, 10);

  XDMFGEOMETRY * curvgeometry = XdmfGeometryNew();

  for (i = 0; i < 9; ++i)
  {
    XdmfGeometryPushBack(curvgeometry, &i, XDMF_ARRAY_TYPE_UINT32, &status);
  }

  XdmfGeometrySetType(curvgeometry, XDMF_GEOMETRY_TYPE_XYZ, &status);

  XdmfCurvilinearGridSetGeometry(curvgrid, curvgeometry, 1);

  XDMFATTRIBUTE * curvattr = XdmfAttributeNew();

  for (i = 0; i < 100; ++i)
  {
    XdmfAttributePushBack(curvattr, &i, XDMF_ARRAY_TYPE_UINT32, &status);
  }

  XdmfAttributeSetType(curvattr, XDMF_ATTRIBUTE_TYPE_SCALAR, &status);
  XdmfAttributeSetCenter(curvattr, XDMF_ATTRIBUTE_CENTER_NODE, &status);

  XdmfCurvilinearGridInsertAttribute(curvgrid, curvattr, 1);

  XdmfDomainInsertCurvilinearGrid(domain, curvgrid, 1);

  XDMFARRAY * xCoordinates = XdmfArrayNew();
  XDMFARRAY * yCoordinates = XdmfArrayNew();

  for (i = 0; i < 10; ++i)
  {
    XdmfArrayPushBack(xCoordinates, &i, XDMF_ARRAY_TYPE_UINT32, &status);
    XdmfArrayPushBack(yCoordinates, &i, XDMF_ARRAY_TYPE_UINT32, &status);
  }

  XDMFRECTILINEARGRID * rectgrid = XdmfRectilinearGridNew2D(xCoordinates, yCoordinates, 1);

  XdmfDomainInsertRectilinearGrid(domain, rectgrid, 1);

  XDMFATTRIBUTE * rectattr = XdmfAttributeNew();

  for (i = 0; i < 100; ++i)
  {
    XdmfAttributePushBack(rectattr, &i, XDMF_ARRAY_TYPE_UINT32, &status);
  }

  XdmfAttributeSetType(rectattr, XDMF_ATTRIBUTE_TYPE_SCALAR, &status);
  XdmfAttributeSetCenter(rectattr, XDMF_ATTRIBUTE_CENTER_NODE, &status);

  XdmfRectilinearGridInsertAttribute(rectgrid, rectattr, 1);

  XDMFREGULARGRID * reggrid = XdmfRegularGridNew2D(1, 1, 10, 10, 0, 0);

  XdmfDomainInsertRegularGrid(domain, reggrid, 1);

  XDMFATTRIBUTE * regattr = XdmfAttributeNew();

  for (i = 0; i < 100; ++i)
  {
    XdmfAttributePushBack(regattr, &i, XDMF_ARRAY_TYPE_UINT32, &status);
  }

  XdmfAttributeSetType(regattr, XDMF_ATTRIBUTE_TYPE_SCALAR, &status);
  XdmfAttributeSetCenter(regattr, XDMF_ATTRIBUTE_CENTER_NODE, &status);

  XdmfRegularGridInsertAttribute(reggrid, regattr, 1);

  XDMFUNSTRUCTUREDGRID * ungrid = XdmfUnstructuredGridNew();

  XdmfDomainInsertUnstructuredGrid(domain, ungrid, 1);

  XDMFGEOMETRY * ungeometry = XdmfGeometryNew();

  for (i = 0; i < 9; ++i)
  {
    XdmfGeometryPushBack(ungeometry, &i, XDMF_ARRAY_TYPE_UINT32, &status);
  }

  XdmfGeometrySetType(ungeometry, XDMF_GEOMETRY_TYPE_XYZ, &status);

  XdmfUnstructuredGridSetGeometry(ungrid, ungeometry, 1);

  XDMFTOPOLOGY * untopology = XdmfTopologyNew();

  for (i = 0; i < 3; ++i)
  {
    XdmfTopologyPushBack(untopology, &i, XDMF_ARRAY_TYPE_UINT32, &status);
  }

  XdmfTopologySetType(untopology, XDMF_TOPOLOGY_TYPE_TRIANGLE, &status);

  XdmfUnstructuredGridSetTopology(ungrid, untopology, 1);

  XDMFATTRIBUTE * unattr = XdmfAttributeNew();

  for (i = 0; i < 3; ++i)
  {
    XdmfAttributePushBack(unattr, &i, XDMF_ARRAY_TYPE_UINT32, &status);
  }

  XdmfAttributeSetType(unattr, XDMF_ATTRIBUTE_TYPE_SCALAR, &status);
  XdmfAttributeSetCenter(unattr, XDMF_ATTRIBUTE_CENTER_NODE, &status);

  XdmfUnstructuredGridInsertAttribute(ungrid, unattr, 1);

  XDMFWRITER * writer = XdmfWriterNew("gridControllerReference.xmf");

  XdmfDomainAccept(domain, (XDMFVISITOR *)writer, &status);

  printf("Finished writing reference file\n");

  XDMFDOMAIN * equalDomain = XdmfDomainNew();

  XDMFGRIDCOLLECTION * equalCollection = XdmfGridCollectionNew();

  XDMFGRIDCONTROLLER * collectionController = XdmfGridControllerNew("gridControllerReference.xmf", "/Xdmf/Domain/Grid[1]");

  XdmfGridCollectionSetGridController(equalCollection, collectionController, 1);

  XdmfDomainInsertGridCollection(equalDomain, equalCollection, 1);

  XDMFCURVILINEARGRID * equalCurvGrid = XdmfCurvilinearGridNew2D(0, 0);

  XDMFGRIDCONTROLLER * curvGridController = XdmfGridControllerNew("gridControllerReference.xmf", "/Xdmf/Domain/Grid[2]");

  XdmfCurvilinearGridSetGridController(equalCurvGrid, curvGridController, 1);

  XdmfDomainInsertCurvilinearGrid(equalDomain, equalCurvGrid, 1);

  XDMFARRAY * placeholderXArray = XdmfArrayNew();
  XDMFARRAY * placeholderYArray = XdmfArrayNew();

  XDMFRECTILINEARGRID * equalRectGrid = XdmfRectilinearGridNew2D(placeholderXArray,
                                                                 placeholderYArray,
                                                                 1);

  XDMFGRIDCONTROLLER * rectGridController = XdmfGridControllerNew("gridControllerReference.xmf", "/Xdmf/Domain/Grid[3]");

  XdmfRectilinearGridSetGridController(equalRectGrid, rectGridController, 1);

  XdmfDomainInsertRectilinearGrid(equalDomain, equalRectGrid, 1);

  XDMFREGULARGRID * equalRegGrid = XdmfRegularGridNew2D(0, 0, 0, 0, 0, 0);

  XDMFGRIDCONTROLLER * regGridController = XdmfGridControllerNew("gridControllerReference.xmf", "/Xdmf/Domain/Grid[4]");

  XdmfRegularGridSetGridController(equalRegGrid, regGridController, 1);

  XdmfDomainInsertRegularGrid(equalDomain, equalRegGrid, 1);

  XDMFUNSTRUCTUREDGRID * equalUnGrid = XdmfUnstructuredGridNew();

  XDMFGRIDCONTROLLER * unGridController = XdmfGridControllerNew("gridControllerReference.xmf", "/Xdmf/Domain/Grid[5]");

  XdmfUnstructuredGridSetGridController(equalUnGrid, unGridController, 1);

  XdmfDomainInsertUnstructuredGrid(equalDomain, equalUnGrid, 1);

  XDMFWRITER * controllerWriter = XdmfWriterNew("gridController.xmf");

  XdmfDomainAccept(equalDomain, (XDMFVISITOR *)controllerWriter, &status);











  XDMFREADER * reader = XdmfReaderNew();

  XDMFDOMAIN * readReference = (XDMFDOMAIN *)XdmfReaderRead(reader, "gridControllerReference.xmf", &status);

  XDMFDOMAIN * readControllerDomain = (XDMFDOMAIN *)XdmfReaderRead(reader, "gridController.xmf", &status);

  XDMFGRIDCOLLECTION * collectionReference = XdmfDomainGetGridCollection(readReference, 0);

  XDMFGRIDCOLLECTION * compareCollection = XdmfDomainGetGridCollection(readControllerDomain, 0);

  XdmfGridCollectionRead(compareCollection, &status);

  printf("%u ?= %u\n", XdmfGridCollectionGetNumberUnstructuredGrids(compareCollection), XdmfGridCollectionGetNumberUnstructuredGrids(collectionReference));

  assert(XdmfGridCollectionGetNumberUnstructuredGrids(compareCollection) == XdmfGridCollectionGetNumberUnstructuredGrids(collectionReference));

  for (i = 0; i < XdmfGridCollectionGetNumberUnstructuredGrids(compareCollection); ++i)
  {
    printf("Checking Grid %d\n", i);
    XDMFUNSTRUCTUREDGRID * unCollectionReference = XdmfGridCollectionGetUnstructuredGrid(collectionReference, i);
    XDMFUNSTRUCTUREDGRID * unCollectionRead = XdmfGridCollectionGetUnstructuredGrid(compareCollection, i);

    printf("%s ?= %s\n", XdmfGeometryGetValuesString(XdmfUnstructuredGridGetGeometry(unCollectionRead)), XdmfGeometryGetValuesString(XdmfUnstructuredGridGetGeometry(unCollectionReference)));

    assert(strcmp(XdmfGeometryGetValuesString(XdmfUnstructuredGridGetGeometry(unCollectionRead)), XdmfGeometryGetValuesString(XdmfUnstructuredGridGetGeometry(unCollectionReference))) == 0);

    printf("%s ?= %s\n", XdmfTopologyGetValuesString(XdmfUnstructuredGridGetTopology(unCollectionRead)), XdmfTopologyGetValuesString(XdmfUnstructuredGridGetTopology(unCollectionReference)));

    assert(strcmp(XdmfTopologyGetValuesString(XdmfUnstructuredGridGetTopology(unCollectionRead)), XdmfTopologyGetValuesString(XdmfUnstructuredGridGetTopology(unCollectionReference))) == 0);

    printf("%s ?= %s\n", XdmfAttributeGetValuesString(XdmfUnstructuredGridGetAttribute(unCollectionRead, 0)), XdmfAttributeGetValuesString(XdmfUnstructuredGridGetAttribute(unCollectionReference, 0)));

    assert(strcmp(XdmfAttributeGetValuesString(XdmfUnstructuredGridGetAttribute(unCollectionRead, 0)), XdmfAttributeGetValuesString(XdmfUnstructuredGridGetAttribute(unCollectionReference, 0))) == 0);
  }

  XdmfGridCollectionRelease(compareCollection);





  XDMFUNSTRUCTUREDGRID * unCompareReference = XdmfDomainGetUnstructuredGrid(readReference, 0);

  XDMFUNSTRUCTUREDGRID * unCompare = XdmfDomainGetUnstructuredGrid(readControllerDomain, 0);

  XdmfUnstructuredGridRead(unCompare, &status);

  printf("%s ?= %s\n", XdmfGeometryGetValuesString(XdmfUnstructuredGridGetGeometry(unCompare)), XdmfGeometryGetValuesString(XdmfUnstructuredGridGetGeometry(unCompareReference)));

  assert(strcmp(XdmfGeometryGetValuesString(XdmfUnstructuredGridGetGeometry(unCompare)), XdmfGeometryGetValuesString(XdmfUnstructuredGridGetGeometry(unCompareReference))) == 0);

  printf("%s ?= %s\n", XdmfTopologyGetValuesString(XdmfUnstructuredGridGetTopology(unCompare)), XdmfTopologyGetValuesString(XdmfUnstructuredGridGetTopology(unCompareReference)));

  assert(strcmp(XdmfTopologyGetValuesString(XdmfUnstructuredGridGetTopology(unCompare)), XdmfTopologyGetValuesString(XdmfUnstructuredGridGetTopology(unCompareReference))) == 0);

  printf("%s ?= %s\n", XdmfAttributeGetValuesString(XdmfUnstructuredGridGetAttribute(unCompare, 0)), XdmfAttributeGetValuesString(XdmfUnstructuredGridGetAttribute(unCompareReference, 0)));

  assert(strcmp(XdmfAttributeGetValuesString(XdmfUnstructuredGridGetAttribute(unCompare, 0)), XdmfAttributeGetValuesString(XdmfUnstructuredGridGetAttribute(unCompareReference, 0))) == 0);

  XdmfUnstructuredGridRelease(unCompare);

  XDMFREGULARGRID * regCompareReference = XdmfDomainGetRegularGrid(readReference, 0);

  XDMFREGULARGRID * regCompare = XdmfDomainGetRegularGrid(readControllerDomain, 0);

  XdmfRegularGridRead(regCompare, &status);

  printf("%s ?= %s\n", XdmfArrayGetValuesString(XdmfRegularGridGetOrigin(regCompare, &status)), XdmfArrayGetValuesString(XdmfRegularGridGetOrigin(regCompareReference, &status)));

  assert(strcmp(XdmfArrayGetValuesString(XdmfRegularGridGetOrigin(regCompare, &status)), XdmfArrayGetValuesString(XdmfRegularGridGetOrigin(regCompareReference, &status))) == 0);

  printf("%s ?= %s\n", XdmfArrayGetValuesString(XdmfRegularGridGetDimensions(regCompare, &status)), XdmfArrayGetValuesString(XdmfRegularGridGetDimensions(regCompareReference, &status)));

  assert(strcmp(XdmfArrayGetValuesString(XdmfRegularGridGetDimensions(regCompare, &status)), XdmfArrayGetValuesString(XdmfRegularGridGetDimensions(regCompareReference, &status))) == 0);

  printf("%s ?= %s\n", XdmfArrayGetValuesString(XdmfRegularGridGetBrickSize(regCompare, &status)), XdmfArrayGetValuesString(XdmfRegularGridGetBrickSize(regCompareReference, &status)));

  assert(strcmp(XdmfArrayGetValuesString(XdmfRegularGridGetBrickSize(regCompare, &status)), XdmfArrayGetValuesString(XdmfRegularGridGetBrickSize(regCompareReference, &status))) == 0);

  printf("%s ?= %s\n", XdmfAttributeGetValuesString(XdmfRegularGridGetAttribute(regCompare, 0)), XdmfAttributeGetValuesString(XdmfRegularGridGetAttribute(regCompareReference, 0)));

  assert(strcmp(XdmfAttributeGetValuesString(XdmfRegularGridGetAttribute(regCompare, 0)), XdmfAttributeGetValuesString(XdmfRegularGridGetAttribute(regCompareReference, 0))) == 0);

  XdmfRegularGridRelease(regCompare);

  XDMFRECTILINEARGRID * rectCompareReference = XdmfDomainGetRectilinearGrid(readReference, 0);

  XDMFRECTILINEARGRID * rectCompare = XdmfDomainGetRectilinearGrid(readControllerDomain, 0);

  XdmfRectilinearGridRead(rectCompare, &status);

  printf("%s ?= %s\n", XdmfAttributeGetValuesString(XdmfRectilinearGridGetAttribute(rectCompare, 0)), XdmfAttributeGetValuesString(XdmfRectilinearGridGetAttribute(rectCompareReference, 0)));

  assert(strcmp(XdmfAttributeGetValuesString(XdmfRectilinearGridGetAttribute(rectCompare, 0)), XdmfAttributeGetValuesString(XdmfRectilinearGridGetAttribute(rectCompareReference, 0))) == 0);

  printf("%s ?= %s\n", XdmfArrayGetValuesString(XdmfRectilinearGridGetCoordinatesByIndex(rectCompare, 0, &status)), XdmfArrayGetValuesString(XdmfRectilinearGridGetCoordinatesByIndex(rectCompareReference, 0, &status)));

  assert(strcmp(XdmfArrayGetValuesString(XdmfRectilinearGridGetCoordinatesByIndex(rectCompare, 0, &status)), XdmfArrayGetValuesString(XdmfRectilinearGridGetCoordinatesByIndex(rectCompareReference, 0, &status))) == 0);

  printf("%s ?= %s\n", XdmfArrayGetValuesString(XdmfRectilinearGridGetCoordinatesByIndex(rectCompare, 1, &status)), XdmfArrayGetValuesString(XdmfRectilinearGridGetCoordinatesByIndex(rectCompareReference, 1, &status)));

  assert(strcmp(XdmfArrayGetValuesString(XdmfRectilinearGridGetCoordinatesByIndex(rectCompare, 1, &status)), XdmfArrayGetValuesString(XdmfRectilinearGridGetCoordinatesByIndex(rectCompareReference, 1, &status))) == 0);

  XdmfRectilinearGridRelease(rectCompare);

  XDMFCURVILINEARGRID * curvCompareReference = XdmfDomainGetCurvilinearGrid(readReference, 0);

  XDMFCURVILINEARGRID * curvCompare = XdmfDomainGetCurvilinearGrid(readControllerDomain, 0);

  XdmfCurvilinearGridRead(curvCompare, &status);

  printf("%s ?= %s\n", XdmfGeometryGetValuesString(XdmfCurvilinearGridGetGeometry(curvCompare)), XdmfGeometryGetValuesString(XdmfCurvilinearGridGetGeometry(curvCompareReference)));

  assert(strcmp(XdmfGeometryGetValuesString(XdmfCurvilinearGridGetGeometry(curvCompare)), XdmfGeometryGetValuesString(XdmfCurvilinearGridGetGeometry(curvCompareReference))) == 0);

  printf("%s ?= %s\n", XdmfAttributeGetValuesString(XdmfCurvilinearGridGetAttribute(curvCompare, 0)), XdmfAttributeGetValuesString(XdmfCurvilinearGridGetAttribute(curvCompareReference, 0)));

  assert(strcmp(XdmfAttributeGetValuesString(XdmfCurvilinearGridGetAttribute(curvCompare, 0)), XdmfAttributeGetValuesString(XdmfCurvilinearGridGetAttribute(curvCompareReference, 0))) == 0);

  printf("%s ?= %s\n", XdmfArrayGetValuesString(XdmfCurvilinearGridGetDimensions(curvCompare, &status)), XdmfArrayGetValuesString(XdmfCurvilinearGridGetDimensions(curvCompareReference, &status)));

  assert(strcmp(XdmfArrayGetValuesString(XdmfCurvilinearGridGetDimensions(curvCompare, &status)), XdmfArrayGetValuesString(XdmfCurvilinearGridGetDimensions(curvCompareReference, &status))) == 0);

  XdmfCurvilinearGridRelease(curvCompare);

  return 0;
}
