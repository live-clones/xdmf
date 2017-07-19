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
#include "stdlib.h"
#include "string.h"

int main()
{
  int status = 0;
  unsigned int i, j;
  XDMFDOMAIN * domain = XdmfDomainNew();

  XDMFGRIDCOLLECTION * collection = XdmfGridCollectionNew();

  for (i = 0; i < 5; ++i) {
    
    XDMFUNSTRUCTUREDGRID * colgrid = XdmfUnstructuredGridNew();
    
    XdmfGridCollectionInsertUnstructuredGrid(collection, colgrid, 1);

    XDMFGEOMETRY * colgeometry = XdmfGeometryNew();
    for (j = 0; j < 9; ++j) {
      unsigned int val = j+i;
      XdmfGeometryPushBack(colgeometry, &val, XDMF_ARRAY_TYPE_UINT32, &status);
    }
    XdmfGeometrySetType(colgeometry, XDMF_GEOMETRY_TYPE_XYZ, &status);
    XdmfUnstructuredGridSetGeometry(colgrid, colgeometry, 1);
    XdmfGeometryFree(colgeometry);

    XDMFTOPOLOGY * coltopology = XdmfTopologyNew();
    for (j = 0; j < 3; ++j) {
      XdmfTopologyPushBack(coltopology, &j, XDMF_ARRAY_TYPE_UINT32, &status);
    }
    XdmfTopologySetType(coltopology, XDMF_TOPOLOGY_TYPE_TRIANGLE, &status);
    XdmfUnstructuredGridSetTopology(colgrid, coltopology, 1);
    XdmfTopologyFree(coltopology);

    XDMFATTRIBUTE * colattr = XdmfAttributeNew();
    for (j = 0; j < 3; ++j) {
      unsigned int val = j+i;
      XdmfAttributePushBack(colattr, &val, XDMF_ARRAY_TYPE_UINT32, &status);
    }
    XdmfAttributeSetType(colattr, XDMF_ATTRIBUTE_TYPE_SCALAR, &status);
    XdmfAttributeSetCenter(colattr, XDMF_ATTRIBUTE_CENTER_NODE, &status);
    XdmfUnstructuredGridInsertAttribute(colgrid, colattr, 1);
    XdmfAttributeFree(colattr);

    XdmfUnstructuredGridFree(colgrid);
    
  }

  XdmfDomainInsertGridCollection(domain, collection, 1);
  XdmfGridCollectionFree(collection);

  XDMFCURVILINEARGRID * curvgrid = XdmfCurvilinearGridNew2D(10, 10);

  XDMFGEOMETRY * curvgeometry = XdmfGeometryNew();
  for (i = 0; i < 9; ++i) {
    XdmfGeometryPushBack(curvgeometry, &i, XDMF_ARRAY_TYPE_UINT32, &status);
  }
  XdmfGeometrySetType(curvgeometry, XDMF_GEOMETRY_TYPE_XYZ, &status);
  XdmfCurvilinearGridSetGeometry(curvgrid, curvgeometry, 1);
  XdmfGeometryFree(curvgeometry);
  
  XDMFATTRIBUTE * curvattr = XdmfAttributeNew();
  for (i = 0; i < 100; ++i) {
    XdmfAttributePushBack(curvattr, &i, XDMF_ARRAY_TYPE_UINT32, &status);
  }
  XdmfAttributeSetType(curvattr, XDMF_ATTRIBUTE_TYPE_SCALAR, &status);
  XdmfAttributeSetCenter(curvattr, XDMF_ATTRIBUTE_CENTER_NODE, &status);
  XdmfCurvilinearGridInsertAttribute(curvgrid, curvattr, 1);
  XdmfAttributeFree(curvattr);

  XdmfDomainInsertCurvilinearGrid(domain, curvgrid, 1);
  XdmfCurvilinearGridFree(curvgrid);

  XDMFARRAY * xCoordinates = XdmfArrayNew();
  XDMFARRAY * yCoordinates = XdmfArrayNew();

  for (i = 0; i < 10; ++i) {
    XdmfArrayPushBack(xCoordinates, &i, XDMF_ARRAY_TYPE_UINT32, &status);
    XdmfArrayPushBack(yCoordinates, &i, XDMF_ARRAY_TYPE_UINT32, &status);
  }

  XDMFRECTILINEARGRID * rectgrid = 
    XdmfRectilinearGridNew2D(xCoordinates, yCoordinates, 1);
  XdmfArrayFree(xCoordinates);
  XdmfArrayFree(yCoordinates);

  XdmfDomainInsertRectilinearGrid(domain, rectgrid, 1);

  XDMFATTRIBUTE * rectattr = XdmfAttributeNew();

  for (i = 0; i < 100; ++i) {
    XdmfAttributePushBack(rectattr, &i, XDMF_ARRAY_TYPE_UINT32, &status);
  }
  XdmfAttributeSetType(rectattr, XDMF_ATTRIBUTE_TYPE_SCALAR, &status);
  XdmfAttributeSetCenter(rectattr, XDMF_ATTRIBUTE_CENTER_NODE, &status);
  XdmfRectilinearGridInsertAttribute(rectgrid, rectattr, 1);
  XdmfAttributeFree(rectattr);
  XdmfRectilinearGridFree(rectgrid);

  XDMFREGULARGRID * reggrid = XdmfRegularGridNew2D(1, 1, 10, 10, 0, 0);
  XdmfDomainInsertRegularGrid(domain, reggrid, 1);
  
  XDMFATTRIBUTE * regattr = XdmfAttributeNew();
  for (i = 0; i < 100; ++i) {
    XdmfAttributePushBack(regattr, &i, XDMF_ARRAY_TYPE_UINT32, &status);
  }
  XdmfAttributeSetType(regattr, XDMF_ATTRIBUTE_TYPE_SCALAR, &status);
  XdmfAttributeSetCenter(regattr, XDMF_ATTRIBUTE_CENTER_NODE, &status);
  XdmfRegularGridInsertAttribute(reggrid, regattr, 1);
  XdmfAttributeFree(regattr);
  XdmfRegularGridFree(reggrid);

  XDMFUNSTRUCTUREDGRID * ungrid = XdmfUnstructuredGridNew();
  XdmfDomainInsertUnstructuredGrid(domain, ungrid, 1);

  XDMFGEOMETRY * ungeometry = XdmfGeometryNew();
  for (i = 0; i < 9; ++i) {
    XdmfGeometryPushBack(ungeometry, &i, XDMF_ARRAY_TYPE_UINT32, &status);
  }
  XdmfGeometrySetType(ungeometry, XDMF_GEOMETRY_TYPE_XYZ, &status);
  XdmfUnstructuredGridSetGeometry(ungrid, ungeometry, 1);
  XdmfGeometryFree(ungeometry);

  XDMFTOPOLOGY * untopology = XdmfTopologyNew();
  for (i = 0; i < 3; ++i) {
    XdmfTopologyPushBack(untopology, &i, XDMF_ARRAY_TYPE_UINT32, &status);
  }
  XdmfTopologySetType(untopology, XDMF_TOPOLOGY_TYPE_TRIANGLE, &status);
  XdmfUnstructuredGridSetTopology(ungrid, untopology, 1);
  XdmfTopologyFree(untopology);

  XDMFATTRIBUTE * unattr = XdmfAttributeNew();
  for (i = 0; i < 3; ++i) {
    XdmfAttributePushBack(unattr, &i, XDMF_ARRAY_TYPE_UINT32, &status);
  }
  XdmfAttributeSetType(unattr, XDMF_ATTRIBUTE_TYPE_SCALAR, &status);
  XdmfAttributeSetCenter(unattr, XDMF_ATTRIBUTE_CENTER_NODE, &status);
  XdmfUnstructuredGridInsertAttribute(ungrid, unattr, 1);
  XdmfAttributeFree(unattr);
  XdmfUnstructuredGridFree(ungrid);
  
  XDMFWRITER * writer = XdmfWriterNew("gridControllerReference.xmf");
  XdmfDomainAccept(domain, (XDMFVISITOR *)writer, &status);
  XdmfWriterFree(writer);
  XdmfDomainFree(domain);

  printf("Finished writing reference file\n");

  XDMFDOMAIN * equalDomain = XdmfDomainNew();
  XDMFGRIDCOLLECTION * equalCollection = XdmfGridCollectionNew();
  XDMFGRIDCONTROLLER * collectionController = 
    XdmfGridControllerNew("gridControllerReference.xmf", 
			  "/Xdmf/Domain/Grid[1]");

  XdmfGridCollectionSetGridController(equalCollection, 
				      collectionController, 
				      1);
  XdmfGridControllerFree(collectionController);
  XdmfDomainInsertGridCollection(equalDomain, equalCollection, 1);
  XdmfGridCollectionFree(equalCollection);

  XDMFCURVILINEARGRID * equalCurvGrid = XdmfCurvilinearGridNew2D(0, 0);
  XDMFGRIDCONTROLLER * curvGridController = 
    XdmfGridControllerNew("gridControllerReference.xmf", 
			  "/Xdmf/Domain/Grid[2]");
  XdmfCurvilinearGridSetGridController(equalCurvGrid, curvGridController, 1);
  XdmfGridControllerFree(curvGridController);
  XdmfDomainInsertCurvilinearGrid(equalDomain, equalCurvGrid, 1);
  XdmfCurvilinearGridFree(equalCurvGrid);

  XDMFARRAY * placeholderXArray = XdmfArrayNew();
  XDMFARRAY * placeholderYArray = XdmfArrayNew();

  XDMFRECTILINEARGRID * equalRectGrid = 
    XdmfRectilinearGridNew2D(placeholderXArray,
			     placeholderYArray,
			     1);
  XdmfArrayFree(placeholderXArray);
  XdmfArrayFree(placeholderYArray);

  XDMFGRIDCONTROLLER * rectGridController = 
    XdmfGridControllerNew("gridControllerReference.xmf", 
			  "/Xdmf/Domain/Grid[3]");
  XdmfRectilinearGridSetGridController(equalRectGrid, rectGridController, 1);
  XdmfGridControllerFree(rectGridController);
  XdmfDomainInsertRectilinearGrid(equalDomain, equalRectGrid, 1);
  XdmfRectilinearGridFree(equalRectGrid);

  XDMFREGULARGRID * equalRegGrid = XdmfRegularGridNew2D(0, 0, 0, 0, 0, 0);
  XDMFGRIDCONTROLLER * regGridController = 
    XdmfGridControllerNew("gridControllerReference.xmf", 
			  "/Xdmf/Domain/Grid[4]");
  XdmfRegularGridSetGridController(equalRegGrid, regGridController, 1);
  XdmfGridControllerFree(regGridController);
  XdmfDomainInsertRegularGrid(equalDomain, equalRegGrid, 1);
  XdmfRegularGridFree(equalRegGrid);

  XDMFUNSTRUCTUREDGRID * equalUnGrid = XdmfUnstructuredGridNew();
  XDMFGRIDCONTROLLER * unGridController =
    XdmfGridControllerNew("gridControllerReference.xmf", 
			  "/Xdmf/Domain/Grid[5]");
  XdmfUnstructuredGridSetGridController(equalUnGrid, unGridController, 1);
  XdmfGridControllerFree(unGridController);
  XdmfDomainInsertUnstructuredGrid(equalDomain, equalUnGrid, 1);
  XdmfUnstructuredGridFree(equalUnGrid);

  XDMFWRITER * controllerWriter = XdmfWriterNew("gridController.xmf");
  XdmfDomainAccept(equalDomain, (XDMFVISITOR *)controllerWriter, &status);
  XdmfWriterFree(controllerWriter);
  XdmfDomainFree(equalDomain);

  XDMFREADER * reader = XdmfReaderNew();
  XDMFITEM * readReferenceItem = 
    XdmfReaderRead(reader, "gridControllerReference.xmf", &status);
  XDMFDOMAIN * readReference = XdmfDomainCast(readReferenceItem);
  XdmfItemFree(readReferenceItem);

  XDMFITEM * readControllerItem = 
    XdmfReaderRead(reader, "gridController.xmf", &status);
  XDMFDOMAIN * readControllerDomain = XdmfDomainCast(readControllerItem);
  XdmfItemFree(readControllerItem);
  XdmfReaderFree(reader);

  XDMFGRIDCOLLECTION * collectionReference = 
    XdmfDomainGetGridCollection(readReference, 0);
  XDMFGRIDCOLLECTION * compareCollection = 
    XdmfDomainGetGridCollection(readControllerDomain, 0);
  XdmfGridCollectionRead(compareCollection, &status);

  unsigned int referenceNumberUnstructuredGrids = 
    XdmfGridCollectionGetNumberUnstructuredGrids(collectionReference);
  unsigned int compareNumberUnstructuredGrids =
    XdmfGridCollectionGetNumberUnstructuredGrids(compareCollection);
    
  printf("%u ?= %u\n",
	 compareNumberUnstructuredGrids,
	 referenceNumberUnstructuredGrids);

  assert(compareNumberUnstructuredGrids == referenceNumberUnstructuredGrids);

  for (i = 0; i < compareNumberUnstructuredGrids; ++i) {

    printf("Checking Grid %d\n", i);

    XDMFUNSTRUCTUREDGRID * unCollectionReference = 
      XdmfGridCollectionGetUnstructuredGrid(collectionReference, i);
    XDMFUNSTRUCTUREDGRID * unCollectionRead = 
      XdmfGridCollectionGetUnstructuredGrid(compareCollection, i);

    XDMFGEOMETRY * referenceGeometry = 
      XdmfUnstructuredGridGetGeometry(unCollectionReference);
    XDMFGEOMETRY * readGeometry = 
      XdmfUnstructuredGridGetGeometry(unCollectionRead);
    char * referenceValuesString = 
      XdmfGeometryGetValuesString(referenceGeometry);
    char * readValuesString = 
      XdmfGeometryGetValuesString(readGeometry);
    printf("%s ?= %s\n", referenceValuesString, readValuesString);
    assert(strcmp(referenceValuesString, readValuesString) == 0);
    XdmfGeometryFree(referenceGeometry);
    XdmfGeometryFree(readGeometry);
    free(referenceValuesString);
    free(readValuesString);

    XDMFTOPOLOGY * referenceTopology = 
      XdmfUnstructuredGridGetTopology(unCollectionReference);
    XDMFTOPOLOGY * readTopology = 
      XdmfUnstructuredGridGetTopology(unCollectionRead);
    referenceValuesString = 
      XdmfTopologyGetValuesString(referenceTopology);
    readValuesString = 
      XdmfTopologyGetValuesString(readTopology);
    printf("%s ?= %s\n", referenceValuesString, readValuesString);
    assert(strcmp(referenceValuesString, readValuesString) == 0);
    XdmfTopologyFree(referenceTopology);
    XdmfTopologyFree(readTopology);
    free(referenceValuesString);
    free(readValuesString);

    XDMFATTRIBUTE * referenceAttribute = 
      XdmfUnstructuredGridGetAttribute(unCollectionReference, 0);
    XDMFATTRIBUTE * readAttribute = 
      XdmfUnstructuredGridGetAttribute(unCollectionRead, 0);
    referenceValuesString = 
      XdmfAttributeGetValuesString(referenceAttribute);
    readValuesString = 
      XdmfAttributeGetValuesString(readAttribute);
    printf("%s ?= %s\n", referenceValuesString, readValuesString);
    assert(strcmp(referenceValuesString, readValuesString) == 0);
    XdmfAttributeFree(referenceAttribute);
    XdmfAttributeFree(readAttribute);
    free(referenceValuesString);
    free(readValuesString);

    XdmfUnstructuredGridFree(unCollectionReference);
    XdmfUnstructuredGridFree(unCollectionRead);

  }

  XdmfGridCollectionFree(collectionReference);
  XdmfGridCollectionFree(compareCollection);

  XDMFUNSTRUCTUREDGRID * unCompareReference = 
    XdmfDomainGetUnstructuredGrid(readReference, 0);
  XDMFUNSTRUCTUREDGRID * unCompare = 
    XdmfDomainGetUnstructuredGrid(readControllerDomain, 0);
  XdmfUnstructuredGridRead(unCompare, &status);

  XDMFGEOMETRY * referenceGeometry = 
    XdmfUnstructuredGridGetGeometry(unCompareReference);
  XDMFGEOMETRY * readGeometry = 
    XdmfUnstructuredGridGetGeometry(unCompare);
  char * referenceValuesString = 
    XdmfGeometryGetValuesString(referenceGeometry);
  char * readValuesString = 
    XdmfGeometryGetValuesString(readGeometry);
  printf("%s ?= %s\n", referenceValuesString, readValuesString);
  assert(strcmp(referenceValuesString, readValuesString) == 0);
  XdmfGeometryFree(referenceGeometry);
  XdmfGeometryFree(readGeometry);
  free(referenceValuesString);
  free(readValuesString);
  
  XDMFTOPOLOGY * referenceTopology = 
    XdmfUnstructuredGridGetTopology(unCompareReference);
  XDMFTOPOLOGY * readTopology = 
    XdmfUnstructuredGridGetTopology(unCompare);
  referenceValuesString = 
    XdmfTopologyGetValuesString(referenceTopology);
  readValuesString = 
    XdmfTopologyGetValuesString(readTopology);
  printf("%s ?= %s\n", referenceValuesString, readValuesString);
  assert(strcmp(referenceValuesString, readValuesString) == 0);
  XdmfTopologyFree(referenceTopology);
  XdmfTopologyFree(readTopology);
  free(referenceValuesString);
  free(readValuesString);
  
  XDMFATTRIBUTE * referenceAttribute = 
    XdmfUnstructuredGridGetAttribute(unCompareReference, 0);
  XDMFATTRIBUTE * readAttribute = 
    XdmfUnstructuredGridGetAttribute(unCompare, 0);
  referenceValuesString = 
    XdmfAttributeGetValuesString(referenceAttribute);
  readValuesString = 
    XdmfAttributeGetValuesString(readAttribute);
  printf("%s ?= %s\n", referenceValuesString, readValuesString);
  assert(strcmp(referenceValuesString, readValuesString) == 0);
  XdmfAttributeFree(referenceAttribute);
  XdmfAttributeFree(readAttribute);
  free(referenceValuesString);
  free(readValuesString);
  
  XdmfUnstructuredGridFree(unCompareReference);
  XdmfUnstructuredGridFree(unCompare);

  XDMFREGULARGRID * regCompareReference = 
    XdmfDomainGetRegularGrid(readReference, 0);
  XDMFREGULARGRID * regCompare = 
    XdmfDomainGetRegularGrid(readControllerDomain, 0);
  XdmfRegularGridRead(regCompare, &status);

  XDMFARRAY * referenceOrigin = 
    XdmfRegularGridGetOrigin(regCompareReference, &status);
  XDMFARRAY * readOrigin = 
    XdmfRegularGridGetOrigin(regCompare, &status);
  referenceValuesString = 
    XdmfArrayGetValuesString(referenceOrigin);
  readValuesString = 
    XdmfArrayGetValuesString(readOrigin);
  printf("%s ?= %s\n", referenceValuesString, readValuesString);
  assert(strcmp(referenceValuesString, readValuesString) == 0);
  XdmfArrayFree(referenceOrigin);
  XdmfArrayFree(readOrigin);
  free(referenceValuesString);
  free(readValuesString);

  XDMFARRAY * referenceDimensions = 
    XdmfRegularGridGetDimensions(regCompareReference, &status);
  XDMFARRAY * readDimensions = 
    XdmfRegularGridGetDimensions(regCompare, & status);
  referenceValuesString = 
    XdmfArrayGetValuesString(referenceDimensions);
  readValuesString = 
    XdmfArrayGetValuesString(readDimensions);
  printf("%s ?= %s\n", referenceValuesString, readValuesString);
  assert(strcmp(referenceValuesString, readValuesString) == 0);
  XdmfArrayFree(referenceDimensions);
  XdmfArrayFree(readDimensions);
  free(referenceValuesString);
  free(readValuesString);

  XDMFARRAY * referenceBrickSize = 
    XdmfRegularGridGetBrickSize(regCompareReference, &status);
  XDMFARRAY * readBrickSize = 
    XdmfRegularGridGetBrickSize(regCompare, &status);
  referenceValuesString = 
    XdmfArrayGetValuesString(referenceBrickSize);
  readValuesString = 
    XdmfArrayGetValuesString(readBrickSize);
  printf("%s ?= %s\n", referenceValuesString, readValuesString);
  assert(strcmp(referenceValuesString, readValuesString) == 0);
  XdmfArrayFree(referenceBrickSize);
  XdmfArrayFree(readBrickSize);
  free(referenceValuesString);
  free(readValuesString);  
  
  referenceAttribute = 
    XdmfRegularGridGetAttribute(regCompareReference, 0);
  readAttribute = 
    XdmfRegularGridGetAttribute(regCompare, 0);
  referenceValuesString = 
    XdmfAttributeGetValuesString(referenceAttribute);
  readValuesString = 
    XdmfAttributeGetValuesString(readAttribute);
  printf("%s ?= %s\n", referenceValuesString, readValuesString);
  assert(strcmp(referenceValuesString, readValuesString) == 0);
  XdmfAttributeFree(referenceAttribute);
  XdmfAttributeFree(readAttribute);
  free(referenceValuesString);
  free(readValuesString);
  
  XdmfRegularGridFree(regCompareReference);
  XdmfRegularGridFree(regCompare);

  XDMFRECTILINEARGRID * rectCompareReference = 
    XdmfDomainGetRectilinearGrid(readReference, 0);
  XDMFRECTILINEARGRID * rectCompare =
    XdmfDomainGetRectilinearGrid(readControllerDomain, 0);

  XdmfRectilinearGridRead(rectCompare, &status);

  XDMFARRAY * referenceCoordinates = 
    XdmfRectilinearGridGetCoordinatesByIndex(rectCompareReference, 0, &status);
  XDMFARRAY * readCoordinates = 
    XdmfRectilinearGridGetCoordinatesByIndex(rectCompare, 0, &status);
  referenceValuesString = 
    XdmfArrayGetValuesString(referenceCoordinates);
  readValuesString = 
    XdmfArrayGetValuesString(readCoordinates);
  printf("%s ?= %s\n", referenceValuesString, readValuesString);
  assert(strcmp(referenceValuesString, readValuesString) == 0);
  XdmfArrayFree(referenceCoordinates);
  XdmfArrayFree(readCoordinates);
  free(referenceValuesString);
  free(readValuesString);  

  referenceCoordinates = 
    XdmfRectilinearGridGetCoordinatesByIndex(rectCompareReference, 1, &status);
  readCoordinates = 
    XdmfRectilinearGridGetCoordinatesByIndex(rectCompare, 1, &status);
  referenceValuesString = 
    XdmfArrayGetValuesString(referenceCoordinates);
  readValuesString = 
    XdmfArrayGetValuesString(readCoordinates);
  printf("%s ?= %s\n", referenceValuesString, readValuesString);
  assert(strcmp(referenceValuesString, readValuesString) == 0);
  XdmfArrayFree(referenceCoordinates);
  XdmfArrayFree(readCoordinates);
  free(referenceValuesString);
  free(readValuesString);  

  referenceAttribute = 
    XdmfRectilinearGridGetAttribute(rectCompareReference, 0);
  readAttribute = 
    XdmfRectilinearGridGetAttribute(rectCompare, 0);
  referenceValuesString = 
    XdmfAttributeGetValuesString(referenceAttribute);
  readValuesString = 
    XdmfAttributeGetValuesString(readAttribute);
  printf("%s ?= %s\n", referenceValuesString, readValuesString);
  assert(strcmp(referenceValuesString, readValuesString) == 0);
  XdmfAttributeFree(referenceAttribute);
  XdmfAttributeFree(readAttribute);
  free(referenceValuesString);
  free(readValuesString);

  XdmfRectilinearGridFree(rectCompareReference);
  XdmfRectilinearGridFree(rectCompare);

  XDMFCURVILINEARGRID * curvCompareReference = 
    XdmfDomainGetCurvilinearGrid(readReference, 0);
  XDMFCURVILINEARGRID * curvCompare = 
    XdmfDomainGetCurvilinearGrid(readControllerDomain, 0);

  XdmfCurvilinearGridRead(curvCompare, &status);

  referenceGeometry = 
    XdmfCurvilinearGridGetGeometry(curvCompareReference);
  readGeometry = 
    XdmfCurvilinearGridGetGeometry(curvCompare);
  referenceValuesString = 
    XdmfGeometryGetValuesString(referenceGeometry);
  readValuesString = 
    XdmfGeometryGetValuesString(readGeometry);
  printf("%s ?= %s\n", referenceValuesString, readValuesString);
  assert(strcmp(referenceValuesString, readValuesString) == 0);
  XdmfGeometryFree(referenceGeometry);
  XdmfGeometryFree(readGeometry);
  free(referenceValuesString);
  free(readValuesString);

  referenceAttribute = 
    XdmfCurvilinearGridGetAttribute(curvCompareReference, 0);
  readAttribute = 
    XdmfCurvilinearGridGetAttribute(curvCompare, 0);
  referenceValuesString = 
    XdmfAttributeGetValuesString(referenceAttribute);
  readValuesString = 
    XdmfAttributeGetValuesString(readAttribute);
  printf("%s ?= %s\n", referenceValuesString, readValuesString);
  assert(strcmp(referenceValuesString, readValuesString) == 0);
  XdmfAttributeFree(referenceAttribute);
  XdmfAttributeFree(readAttribute);
  free(referenceValuesString);
  free(readValuesString);

  referenceDimensions = 
    XdmfCurvilinearGridGetDimensions(curvCompareReference, &status);
  readDimensions = 
    XdmfCurvilinearGridGetDimensions(curvCompare, &status);
  referenceValuesString = 
    XdmfArrayGetValuesString(referenceDimensions);
  readValuesString = 
    XdmfArrayGetValuesString(readDimensions);
  printf("%s ?= %s\n", referenceValuesString, readValuesString);
  assert(strcmp(referenceValuesString, readValuesString) == 0);
  XdmfArrayFree(referenceDimensions);
  XdmfArrayFree(readDimensions);
  free(referenceValuesString);
  free(readValuesString);  
  
  XdmfCurvilinearGridFree(curvCompare);
  XdmfCurvilinearGridFree(curvCompareReference);

  XdmfDomainFree(readReference);
  XdmfDomainFree(readControllerDomain);

  return 0;
}
