/*
XdmfPython.cpp:
swig -v -c++ -python -o XdmfPython.cpp Xdmf.i
*/

%{
#include <cstddef>
#include <iostream>
#if PY_VERSION_HEX >= 0x03020000
    #define SWIGPY_SLICE_ARG(obj) ((PyObject*) (obj))
#else
    #define SWIGPY_SLICE_ARG(obj) ((PySliceObject*) (obj))
#endif
%}

#ifdef XDMF_BUILD_DSM

%module Xdmf
%{
    // MPI Includes
    #include <mpi.h>

    // XdmfCore Includes
    #include <Xdmf.hpp>
    #include <XdmfArray.hpp>
    #include <XdmfArrayReference.hpp>
    #include <XdmfArrayType.hpp>
    #include <XdmfCoreItemFactory.hpp>
    #include <XdmfCoreReader.hpp>
    #include <XdmfDSMBuffer.hpp>
    #include <XdmfDSMCommMPI.hpp>
    #include <XdmfDSMItemFactory.hpp>
    #include <XdmfError.hpp>
    #include <XdmfFunction.hpp>
    #include <XdmfHDF5Controller.hpp>
    #include <XdmfHDF5Writer.hpp>
    #include <XdmfHDF5ControllerDSM.hpp>
    #include <XdmfHDF5WriterDSM.hpp>
    #include <XdmfHeavyDataController.hpp>
    #include <XdmfHeavyDataWriter.hpp>
    #include <XdmfInformation.hpp>
    #include <XdmfItem.hpp>
    #include <XdmfItemProperty.hpp>
    #include <XdmfSharedPtr.hpp>
    #include <XdmfSparseMatrix.hpp>
    #include <XdmfSubset.hpp>
    #include <XdmfSystemUtils.hpp>
    #include <XdmfTIFFController.hpp>
    #include <XdmfVisitor.hpp>
    #include <XdmfWriter.hpp>

    // Xdmf Includes
    #include <XdmfAggregate.hpp>
    #include <XdmfAttribute.hpp>
    #include <XdmfAttributeCenter.hpp>
    #include <XdmfAttributeType.hpp>
    #include <XdmfCurvilinearGrid.hpp>
    #include <XdmfDomain.hpp>
    #include <XdmfGeometry.hpp>
    #include <XdmfGeometryType.hpp>
    #include <XdmfGraph.hpp>
    #include <XdmfGrid.hpp>
    #include <XdmfGridCollection.hpp>
    #include <XdmfGridCollectionType.hpp>
    #include <XdmfItemFactory.hpp>
    #include <XdmfMap.hpp>
    #include <XdmfReader.hpp>
    #include <XdmfRectilinearGrid.hpp>
    #include <XdmfRegularGrid.hpp>
    #include <XdmfSet.hpp>
    #include <XdmfSetType.hpp>
    #include <XdmfTime.hpp>
    #include <XdmfTopology.hpp>
    #include <XdmfTopologyType.hpp>
    #include <XdmfUnstructuredGrid.hpp>

%}

#else
%module Xdmf
%{
    // XdmfCore Includes
    #include <Xdmf.hpp>
    #include <XdmfArray.hpp>
    #include <XdmfArrayReference.hpp>
    #include <XdmfArrayType.hpp>
    #include <XdmfCoreItemFactory.hpp>
    #include <XdmfCoreReader.hpp>
    #include <XdmfError.hpp>
    #include <XdmfFunction.hpp>
    #include <XdmfHDF5Controller.hpp>
    #include <XdmfHDF5Writer.hpp>
    #include <XdmfHeavyDataController.hpp>
    #include <XdmfHeavyDataWriter.hpp>
    #include <XdmfInformation.hpp>
    #include <XdmfItem.hpp>
    #include <XdmfItemProperty.hpp>
    #include <XdmfSharedPtr.hpp>
    #include <XdmfSparseMatrix.hpp>
    #include <XdmfSubset.hpp>
    #include <XdmfSystemUtils.hpp>
    #include <XdmfTIFFController.hpp>
    #include <XdmfVisitor.hpp>
    #include <XdmfWriter.hpp>

    // Xdmf Includes
    #include <XdmfAggregate.hpp>
    #include <XdmfAttribute.hpp>
    #include <XdmfAttributeCenter.hpp>
    #include <XdmfAttributeType.hpp>
    #include <XdmfCurvilinearGrid.hpp>
    #include <XdmfDomain.hpp>
    #include <XdmfGeometry.hpp>
    #include <XdmfGeometryType.hpp>
    #include <XdmfGraph.hpp>
    #include <XdmfGrid.hpp>
    #include <XdmfGridCollection.hpp>
    #include <XdmfGridCollectionType.hpp>
    #include <XdmfItemFactory.hpp>
    #include <XdmfMap.hpp>
    #include <XdmfReader.hpp>
    #include <XdmfRectilinearGrid.hpp>
    #include <XdmfRegularGrid.hpp>
    #include <XdmfSet.hpp>
    #include <XdmfSetType.hpp>
    #include <XdmfTime.hpp>
    #include <XdmfTopology.hpp>
    #include <XdmfTopologyType.hpp>
    #include <XdmfUnstructuredGrid.hpp>
%}

#endif

%import XdmfCore.i

// Ignoring C Wrappers

// XdmfAggregate

%ignore XdmfAggregateNew();
%ignore XdmfAggregateGetArray(XDMFAGGREGATE * aggregate, unsigned int index);
%ignore XdmfAggregateGetArrayByName(XDMFAGGREGATE * aggregate, char * name);
%ignore XdmfAggregateGetNumberArrays(XDMFAGGREGATE * aggregate);
%ignore XdmfAggregateInsertArray(XDMFAGGREGATE * aggregate, XDMFARRAY * array, int transferOwnership);
%ignore XdmfAggregateRemoveArray(XDMFAGGREGATE * aggregate, unsigned int index);
%ignore XdmfAggregateRemoveArrayByName(XDMFAGGREGATE * aggregate, char * name);

// XdmfAttributeCenter

%ignore XdmfAttributeCenterGrid();
%ignore XdmfAttributeCenterCell();
%ignore XdmfAttributeCenterFace();
%ignore XdmfAttributeCenterEdge();
%ignore XdmfAttributeCenterNode();

// XdmfAttribute

%ignore XdmfAttributeNew();
%ignore XdmfAttributeGetCenter(XDMFATTRIBUTE * attribute);
%ignore XdmfAttributeGetType(XDMFATTRIBUTE * attribute);
%ignore XdmfAttributeSetCenter(XDMFATTRIBUTE * attribute, int center, int * status);
%ignore XdmfAttributeSetType(XDMFATTRIBUTE * attribute, int type, int * status);
// XdmfAttribute inherited from XdmfArray
%ignore XdmfAttributeNew();
%ignore XdmfAttributeClear(XDMFATTRIBUTE * array);
%ignore XdmfAttributeErase(XDMFATTRIBUTE * array, unsigned int index);
%ignore XdmfAttributeGetArrayType(XDMFATTRIBUTE * array, int * status);
%ignore XdmfAttributeGetCapacity(XDMFATTRIBUTE * array);
%ignore XdmfAttributeGetDimensions(XDMFATTRIBUTE * array);
%ignore XdmfAttributeGetDimensionsString(XDMFATTRIBUTE * array);
%ignore XdmfAttributeGetHeavyDataController(XDMFATTRIBUTE * array, unsigned int index);
%ignore XdmfAttributeGetReadMode(XDMFATTRIBUTE * array, int * status);
%ignore XdmfAttributeGetName(XDMFATTRIBUTE * array);
%ignore XdmfAttributeGetNumberDimensions(XDMFATTRIBUTE * array);
%ignore XdmfAttributeGetNumberHeavyDataControllers(XDMFATTRIBUTE * array);
%ignore XdmfAttributeGetSize(XDMFATTRIBUTE * array);
%ignore XdmfAttributeGetReference(XDMFATTRIBUTE * array);
%ignore XdmfAttributeGetValue(XDMFATTRIBUTE * array, unsigned int index, int arrayType, int * status);
%ignore XdmfAttributeGetValues(XDMFATTRIBUTE * array, unsigned int startIndex, int arrayType, unsigned int numValues, unsigned int arrayStride, unsigned int valueStride, int * status);
%ignore XdmfAttributeGetValuesInternal(XDMFATTRIBUTE * array);
%ignore XdmfAttributeGetValuesString(XDMFATTRIBUTE * array);
%ignore XdmfAttributeInitialize(XDMFATTRIBUTE * array, int * dims, int numDims, int arrayType, int * status);
%ignore XdmfAttributeInsertDataFromPointer(XDMFATTRIBUTE * array, void * values, int arrayType, unsigned int startIndex, unsigned int numVals, unsigned int arrayStride, unsigned int valueStride, int * status);
%ignore XdmfAttributeInsertDataFromXdmfArray(XDMFATTRIBUTE * array, XDMFARRAY * valArray, int * arrayStarts, int * valueStarts, int * arrayCounts, int * valueCounts, int * arrayStrides, int * valueStrides, int * status);
%ignore XdmfAttributeInsertHeavyDataController(XDMFATTRIBUTE * array, XDMFHEAVYDATACONTROLLER * controller, int passControl);
%ignore XdmfAttributeInsertValue(XDMFATTRIBUTE * array, unsigned int index, void * value, int arrayType, int * status);
%ignore XdmfAttributeIsInitialized(XDMFATTRIBUTE * array);
%ignore XdmfAttributePushBack(XDMFATTRIBUTE * array, void * value, int arrayType, int * status);
%ignore XdmfAttributeRead(XDMFATTRIBUTE * array, int * status);
%ignore XdmfAttributeReadController(XDMFATTRIBUTE * array, int * status);
%ignore XdmfAttributeReadReference(XDMFATTRIBUTE * array, int * status);
%ignore XdmfAttributeRelease(XDMFATTRIBUTE * array);
%ignore XdmfAttributeRemoveHeavyDataController(XDMFATTRIBUTE * array, unsigned int index);
%ignore XdmfAttributeReserve(XDMFATTRIBUTE * array, int size);
%ignore XdmfAttributeResize(XDMFATTRIBUTE * array, int * dims, int numDims, int arrayType, int * status);
%ignore XdmfAttributeSetReadMode(XDMFATTRIBUTE * array, int readMode, int * status);
%ignore XdmfAttributeSetReference(XDMFATTRIBUTE * array, XDMFARRAYREFERENCE * reference, int passControl);
%ignore XdmfAttributeSetName(XDMFATTRIBUTE * array, char * name, int * status);
%ignore XdmfAttributeSetValuesInternal(XDMFATTRIBUTE * array, void * pointer, unsigned int numValues, int arrayType, int transferOwnership, int * status);
%ignore XdmfAttributeSwapWithXdmfArray(XDMFATTRIBUTE * array, XDMFARRAY * swapArray);
%ignore XdmfAttributeSwapWithArray(XDMFATTRIBUTE * array, void ** pointer, int numValues, int arrayType, int * status);
// XdmfAttribute inherited from XdmfItem
%ignore XdmfAttributeAccept(XDMFATTRIBUTE * item, XDMFVISITOR * visitor, int * status);
%ignore XdmfAttributeFree(void * item);
%ignore XdmfAttributeGetInformation(XDMFATTRIBUTE * item, unsigned int index);
%ignore XdmfAttributeGetInformationByKey(XDMFATTRIBUTE * item, char * key);
%ignore XdmfAttributeGetNumberInformations(XDMFATTRIBUTE * item);
%ignore XdmfAttributeInsertInformation(XDMFATTRIBUTE * item, XDMFINFORMATION * information, int passControl);
%ignore XdmfAttributeRemoveInformation(XDMFATTRIBUTE * item, unsigned int index);
%ignore XdmfAttributeRemoveInformationByKey(XDMFATTRIBUTE * item, char * key);
%ignore XdmfAttributeGetItemTag(XDMFATTRIBUTE * item);

// XdmfAttributeType

%ignore XdmfAttributeTypeScalar();
%ignore XdmfAttributeTypeVector();
%ignore XdmfAttributeTypeTensor();
%ignore XdmfAttributeTypeMatrix();
%ignore XdmfAttributeTypeTensor6();
%ignore XdmfAttributeTypeGlobalId();
%ignore XdmfAttributeTypeNoAttributeType();

// XdmfCurvilinearGrid

%ignore XdmfCurvilinearGridNew2D(unsigned int xNumPoints,
                                 unsigned int yNumPoints);
%ignore XdmfCurvilinearGridNew3D(unsigned int xNumPoints,
                                 unsigned int yNumPoints,
                                 unsigned int zNumPoints);
%ignore XdmfCurvilinearGridNew(XDMFARRAY * numPoints, int * status);
%ignore XdmfCurvilinearGridGetDimensions(XDMFCURVILINEARGRID * grid, int * status);
%ignore XdmfCurvilinearGridGetGeometry(XDMFCURVILINEARGRID * grid);
%ignore XdmfCurvilinearGridSetDimensions(XDMFCURVILINEARGRID * grid,
                                         XDMFARRAY * dimensions,
                                         int passControl,
                                         int * status);
%ignore XdmfCurvilinearGridSetGeometry(XDMFCURVILINEARGRID * grid,
                                       XDMFGEOMETRY * geometry,
                                       int passControl);
// XdmfCurvilinearGrid inherited from XdmfItem
%ignore XdmfCurvilinearGridAccept(XDMFCURVILINEARGRID * item, XDMFVISITOR * visitor, int * status);
%ignore XdmfCurvilinearGridFree(void * item);
%ignore XdmfCurvilinearGridGetInformation(XDMFCURVILINEARGRID * item, unsigned int index);
%ignore XdmfCurvilinearGridGetInformationByKey(XDMFCURVILINEARGRID * item, char * key);
%ignore XdmfCurvilinearGridGetNumberInformations(XDMFCURVILINEARGRID * item);
%ignore XdmfCurvilinearGridInsertInformation(XDMFCURVILINEARGRID * item, XDMFINFORMATION * information, int passControl);
%ignore XdmfCurvilinearGridRemoveInformation(XDMFCURVILINEARGRID * item, unsigned int index);
%ignore XdmfCurvilinearGridRemoveInformationByKey(XDMFCURVILINEARGRID * item, char * key);
%ignore XdmfCurvilinearGridGetItemTag(XDMFCURVILINEARGRID * item);
// XdmfCurvilinearGrid inherited from XdmfGrid
%ignore XdmfCurvilinearGridGetAttribute(XDMFCURVILINEARGRID * grid, unsigned int index);
%ignore XdmfCurvilinearGridGetAttributeByName(XDMFCURVILINEARGRID * grid, char * Name);
%ignore XdmfCurvilinearGridGetNumberAttributes(XDMFCURVILINEARGRID * grid);
%ignore XdmfCurvilinearGridInsertAttribute(XDMFCURVILINEARGRID * grid, XDMFATTRIBUTE * Attribute, int passControl);
%ignore XdmfCurvilinearGridRemoveAttribute(XDMFCURVILINEARGRID * grid, unsigned int index);
%ignore XdmfCurvilinearGridRemoveAttributeByName(XDMFCURVILINEARGRID * grid, char * Name);
%ignore XdmfCurvilinearGridGetSet(XDMFCURVILINEARGRID * grid, unsigned int index);
%ignore XdmfCurvilinearGridGetSetByName(XDMFCURVILINEARGRID * grid, char * Name);
%ignore XdmfCurvilinearGridGetNumberSets(XDMFCURVILINEARGRID * grid);
%ignore XdmfCurvilinearGridInsertSet(XDMFCURVILINEARGRID * grid, XDMFSET * Set, int passControl);
%ignore XdmfCurvilinearGridRemoveSet(XDMFCURVILINEARGRID * grid, unsigned int index);
%ignore XdmfCurvilinearGridRemoveSetByName(XDMFCURVILINEARGRID * grid, char * Name);
%ignore XdmfCurvilinearGridGetMap(XDMFCURVILINEARGRID * grid, unsigned int index);
%ignore XdmfCurvilinearGridGetMapByName(XDMFCURVILINEARGRID * grid, char * Name);
%ignore XdmfCurvilinearGridGetNumberMaps(XDMFCURVILINEARGRID * grid);
%ignore XdmfCurvilinearGridInsertMap(XDMFCURVILINEARGRID * grid, XDMFMAP * Map, int passControl);
%ignore XdmfCurvilinearGridRemoveMap(XDMFCURVILINEARGRID * grid, unsigned int index);
%ignore XdmfCurvilinearGridRemoveMapByName(XDMFCURVILINEARGRID * grid, char * Name);
%ignore XdmfCurvilinearGridGetName(XDMFCURVILINEARGRID * grid);
%ignore XdmfCurvilinearGridGetTime(XDMFCURVILINEARGRID * grid);
%ignore XdmfCurvilinearGridSetName(XDMFCURVILINEARGRID * grid, char * name, int * status);
%ignore XdmfCurvilinearGridSetTime(XDMFCURVILINEARGRID * grid, XDMFTIME * time, int passControl);

// XdmfDomain

%ignore XdmfDomainNew();
%ignore XdmfDomainGetGridCollection(XDMFDOMAIN * domain, unsigned int index);
%ignore XdmfDomainGetGridCollectionByName(XDMFDOMAIN * domain, char * Name);
%ignore XdmfDomainGetNumberGridCollections(XDMFDOMAIN * domain);
%ignore XdmfDomainInsertGridCollection(XDMFDOMAIN * domain,
                                       XDMFGRIDCOLLECTION * GridCollection,
                                       int passControl);
%ignore XdmfDomainRemoveGridCollection(XDMFDOMAIN * domain, unsigned int index);
%ignore XdmfDomainRemoveGridCollectionByName(XDMFDOMAIN * domain, char * Name);
%ignore XdmfDomainGetGraph(XDMFDOMAIN * domain, unsigned int index);
%ignore XdmfDomainGetGraphByName(XDMFDOMAIN * domain, char * Name);
%ignore XdmfDomainGetNumberGraphs(XDMFDOMAIN * domain);
%ignore XdmfDomainInsertGraph(XDMFDOMAIN * domain, XDMFGRAPH * graph, int passControl);
%ignore XdmfDomainRemoveGraph(XDMFDOMAIN * domain, unsigned int index);
%ignore XdmfDomainRemoveGraphByName(XDMFDOMAIN * domain, char * Name);
%ignore XdmfDomainGetCurvilinearGrid(XDMFDOMAIN * domain, unsigned int index);
%ignore XdmfDomainGetCurvilinearGridByName(XDMFDOMAIN * domain, char * Name);
%ignore XdmfDomainGetNumberCurvilinearGrids(XDMFDOMAIN * domain);
%ignore XdmfDomainInsertCurvilinearGrid(XDMFDOMAIN * domain,
                                        XDMFCURVILINEARGRID * CurvilinearGrid,
                                        int passControl);
%ignore XdmfDomainRemoveCurvilinearGrid(XDMFDOMAIN * domain, unsigned int index);
%ignore XdmfDomainRemoveCurvilinearGridByName(XDMFDOMAIN * domain, char * Name);
%ignore XdmfDomainGetRectilinearGrid(XDMFDOMAIN * domain, unsigned int index);
%ignore XdmfDomainGetRectilinearGridByName(XDMFDOMAIN * domain, char * Name);
%ignore XdmfDomainGetNumberRectilinearGrids(XDMFDOMAIN * domain);
%ignore XdmfDomainInsertRectilinearGrid(XDMFDOMAIN * domain,
                                        XDMFRECTILINEARGRID * RectilinearGrid,
                                        int passControl);
%ignore XdmfDomainRemoveRectilinearGrid(XDMFDOMAIN * domain, unsigned int index);
%ignore XdmfDomainRemoveRectilinearGridByName(XDMFDOMAIN * domain, char * Name);
%ignore XdmfDomainGetRegularGrid(XDMFDOMAIN * domain, unsigned int index);
%ignore XdmfDomainGetRegularGridByName(XDMFDOMAIN * domain, char * Name);
%ignore XdmfDomainGetNumberRegularGrids(XDMFDOMAIN * domain);
%ignore XdmfDomainInsertRegularGrid(XDMFDOMAIN * domain,
                                    XDMFREGULARGRID * RegularGrid,
                                    int passControl);
%ignore XdmfDomainRemoveRegularGrid(XDMFDOMAIN * domain, unsigned int index);
%ignore XdmfDomainRemoveRegularGridByName(XDMFDOMAIN * domain, char * Name);
%ignore XdmfDomainGetUnstructuredGrid(XDMFDOMAIN * domain, unsigned int index);
%ignore XdmfDomainGetUnstructuredGridByName(XDMFDOMAIN * domain, char * Name);
%ignore XdmfDomainGetNumberUnstructuredGrids(XDMFDOMAIN * domain);
%ignore XdmfDomainInsertUnstructuredGrid(XDMFDOMAIN * domain,
                                         XDMFUNSTRUCTUREDGRID * UnstructuredGrid,
                                         int passControl);
%ignore XdmfDomainRemoveUnstructuredGrid(XDMFDOMAIN * domain, unsigned int index);
%ignore XdmfDomainRemoveUnstructuredGridByName(XDMFDOMAIN * domain, char * Name);
// XdmfDomain inherited from XdmfItem
%ignore XdmfDomainAccept(XDMFDOMAIN * item, XDMFVISITOR * visitor, int * status);
%ignore XdmfDomainFree(void * item);
%ignore XdmfDomainGetInformation(XDMFDOMAIN * item, unsigned int index);
%ignore XdmfDomainGetInformationByKey(XDMFDOMAIN * item, char * key);
%ignore XdmfDomainGetNumberInformations(XDMFDOMAIN * item);
%ignore XdmfDomainInsertInformation(XDMFDOMAIN * item, XDMFINFORMATION * information, int passControl);
%ignore XdmfDomainRemoveInformation(XDMFDOMAIN * item, unsigned int index);
%ignore XdmfDomainRemoveInformationByKey(XDMFDOMAIN * item, char * key);
%ignore XdmfDomainGetItemTag(XDMFDOMAIN * item);

// XdmfGeometry

%ignore XdmfGeometryNew();
%ignore XdmfGeometryGetNumberPoints(XDMFGEOMETRY * geometry);
%ignore XdmfGeometryGetOrigin(XDMFGEOMETRY * geometry);
%ignore XdmfGeometryGetOriginSize(XDMFGEOMETRY * geometry);
%ignore XdmfGeometryGetType(XDMFGEOMETRY * geometry);
%ignore XdmfGeometrySetOrigin(XDMFGEOMETRY * geometry, double newX, double newY, double newZ);
%ignore XdmfGeometrySetOriginArray(XDMFGEOMETRY * geometry, double * originVals, unsigned int numDims);
%ignore XdmfGeometrySetType(XDMFGEOMETRY * geometry, int type, int * status);
// XdmfGeometry inherited from XdmfArray
%ignore XdmfGeometryNew();
%ignore XdmfGeometryClear(XDMFGEOMETRY * array);
%ignore XdmfGeometryErase(XDMFGEOMETRY * array, unsigned int index);
%ignore XdmfGeometryGetArrayType(XDMFGEOMETRY * array, int * status);
%ignore XdmfGeometryGetCapacity(XDMFGEOMETRY * array);
%ignore XdmfGeometryGetDimensions(XDMFGEOMETRY * array);
%ignore XdmfGeometryGetDimensionsString(XDMFGEOMETRY * array);
%ignore XdmfGeometryGetHeavyDataController(XDMFGEOMETRY * array, unsigned int index);
%ignore XdmfGeometryGetReadMode(XDMFGEOMETRY * array, int * status);
%ignore XdmfGeometryGetName(XDMFGEOMETRY * array);
%ignore XdmfGeometryGetNumberDimensions(XDMFGEOMETRY * array);
%ignore XdmfGeometryGetNumberHeavyDataControllers(XDMFGEOMETRY * array);
%ignore XdmfGeometryGetSize(XDMFGEOMETRY * array);
%ignore XdmfGeometryGetReference(XDMFGEOMETRY * array);
%ignore XdmfGeometryGetValue(XDMFGEOMETRY * array, unsigned int index, int arrayType, int * status);
%ignore XdmfGeometryGetValues(XDMFGEOMETRY * array, unsigned int startIndex, int arrayType, unsigned int numValues, unsigned int arrayStride, unsigned int valueStride, int * status);
%ignore XdmfGeometryGetValuesInternal(XDMFGEOMETRY * array);
%ignore XdmfGeometryGetValuesString(XDMFGEOMETRY * array);
%ignore XdmfGeometryInitialize(XDMFGEOMETRY * array, int * dims, int numDims, int arrayType, int * status);
%ignore XdmfGeometryInsertDataFromPointer(XDMFGEOMETRY * array, void * values, int arrayType, unsigned int startIndex, unsigned int numVals, unsigned int arrayStride, unsigned int valueStride, int * status);
%ignore XdmfGeometryInsertDataFromXdmfArray(XDMFGEOMETRY * array, XDMFARRAY * valArray, int * arrayStarts, int * valueStarts, int * arrayCounts, int * valueCounts, int * arrayStrides, int * valueStrides, int * status);
%ignore XdmfGeometryInsertHeavyDataController(XDMFGEOMETRY * array, XDMFHEAVYDATACONTROLLER * controller, int passControl);
%ignore XdmfGeometryInsertValue(XDMFGEOMETRY * array, unsigned int index, void * value, int arrayType, int * status);
%ignore XdmfGeometryIsInitialized(XDMFGEOMETRY * array);
%ignore XdmfGeometryPushBack(XDMFGEOMETRY * array, void * value, int arrayType, int * status);
%ignore XdmfGeometryRead(XDMFGEOMETRY * array, int * status);
%ignore XdmfGeometryReadController(XDMFGEOMETRY * array, int * status);
%ignore XdmfGeometryReadReference(XDMFGEOMETRY * array, int * status);
%ignore XdmfGeometryRelease(XDMFGEOMETRY * array);
%ignore XdmfGeometryRemoveHeavyDataController(XDMFGEOMETRY * array, unsigned int index);
%ignore XdmfGeometryReserve(XDMFGEOMETRY * array, int size);
%ignore XdmfGeometryResize(XDMFGEOMETRY * array, int * dims, int numDims, int arrayType, int * status);
%ignore XdmfGeometrySetReadMode(XDMFGEOMETRY * array, int readMode, int * status);
%ignore XdmfGeometrySetReference(XDMFGEOMETRY * array, XDMFARRAYREFERENCE * reference, int passControl);
%ignore XdmfGeometrySetName(XDMFGEOMETRY * array, char * name, int * status);
%ignore XdmfGeometrySetValuesInternal(XDMFGEOMETRY * array, void * pointer, unsigned int numValues, int arrayType, int transferOwnership, int * status);
%ignore XdmfGeometrySwapWithXdmfArray(XDMFGEOMETRY * array, XDMFARRAY * swapArray);
%ignore XdmfGeometrySwapWithArray(XDMFGEOMETRY * array, void ** pointer, int numValues, int arrayType, int * status);
// XdmfGeometry inherited from XdmfItem
%ignore XdmfGeometryAccept(XDMFGEOMETRY * item, XDMFVISITOR * visitor, int * status);
%ignore XdmfGeometryFree(void * item);
%ignore XdmfGeometryGetInformation(XDMFGEOMETRY * item, unsigned int index);
%ignore XdmfGeometryGetInformationByKey(XDMFGEOMETRY * item, char * key);
%ignore XdmfGeometryGetNumberInformations(XDMFGEOMETRY * item);
%ignore XdmfGeometryInsertInformation(XDMFGEOMETRY * item, XDMFINFORMATION * information, int passControl);
%ignore XdmfGeometryRemoveInformation(XDMFGEOMETRY * item, unsigned int index);
%ignore XdmfGeometryRemoveInformationByKey(XDMFGEOMETRY * item, char * key);
%ignore XdmfGeometryGetItemTag(XDMFGEOMETRY * item);

// XdmfGeometryType

%ignore XdmfGeometryTypeNoGeometryType();
%ignore XdmfGeometryTypeXYZ();
%ignore XdmfGeometryTypeXY();
%ignore XdmfGeometryTypeGetDimensions(int type, int * status);
%ignore XdmfGeometryTypeGetName(int type);

// XdmfGraph

%ignore XdmfGraphNew(unsigned int numberNodes);
%ignore XdmfGraphGetAttribute(XDMFGRAPH * graph, unsigned int index);
%ignore XdmfGraphGetAttributeByName(XDMFGRAPH * graph, char * Name);
%ignore XdmfGraphGetNumberAttributes(XDMFGRAPH * graph);
%ignore XdmfGraphInsertAttribute(XDMFGRAPH * graph, XDMFATTRIBUTE * Attribute, int passControl);
%ignore XdmfGraphRemoveAttribute(XDMFGRAPH * graph, unsigned int index);
%ignore XdmfGraphRemoveAttributeByName(XDMFGRAPH * graph, char * Name);
%ignore XdmfGraphGetNumberNodes(XDMFGRAPH * graph);
// XdmfGraph inherited from XdmfSparseMatrix
%ignore XdmfGraphNew(unsigned int numberRows, unsigned int numberColumns);
%ignore XdmfGraphGetColumnIndex(XDMFGRAPH * matrix, int * status);
%ignore XdmfGraphGetName(XDMFGRAPH * matrix);
%ignore XdmfGraphGetNumberColumns(XDMFGRAPH * matrix);
%ignore XdmfGraphGetNumberRows(XDMFGRAPH * matrix);
%ignore XdmfGraphGetRowPointer(XDMFGRAPH * matrix, int * status);
%ignore XdmfGraphGetValues(XDMFGRAPH * matrix, int * status);
%ignore XdmfGraphGetValuesString(XDMFGRAPH * matrix, int * status);
%ignore XdmfGraphSetColumnIndex(XDMFGRAPH * matrix,
                                XDMFARRAY * columnIndex,
                                int passControl,
                                int * status);
%ignore XdmfGraphSetName(XDMFGRAPH * matrix, char * name, int * status);
%ignore XdmfGraphSetRowPointer(XDMFGRAPH * matrix,
                               XDMFARRAY * rowPointer,
                               int passControl,
                               int * status);
%ignore XdmfGraphSetValues(XDMFGRAPH * matrix,
                           XDMFARRAY * values,
                           int passControl,
                           int * status);
// XdmfGraph inherited from XdmfItem
%ignore XdmfGraphAccept(XDMFGRAPH * item, XDMFVISITOR * visitor, int * status);
%ignore XdmfGraphFree(void * item);
%ignore XdmfGraphGetInformation(XDMFGRAPH * item, unsigned int index);
%ignore XdmfGraphGetInformationByKey(XDMFGRAPH * item, char * key);
%ignore XdmfGraphGetNumberInformations(XDMFGRAPH * item);
%ignore XdmfGraphInsertInformation(XDMFGRAPH * item, XDMFINFORMATION * information, int passControl);
%ignore XdmfGraphRemoveInformation(XDMFGRAPH * item, unsigned int index);
%ignore XdmfGraphRemoveInformationByKey(XDMFGRAPH * item, char * key);
%ignore XdmfGraphGetItemTag(XDMFGRAPH * item);

// XdmfGridCollection

%ignore XdmfGridCollectionNew();
%ignore XdmfGridCollectionGetType(XDMFGRIDCOLLECTION * collection, int * status);
%ignore XdmfGridCollectionSetType(XDMFGRIDCOLLECTION * collection, int type, int * status);
// XdmfGridCollection inherited from XdmfDomain
%ignore XdmfGridCollectionGetGridCollection(XDMFGRIDCOLLECTION * domain, unsigned int index);
%ignore XdmfGridCollectionGetGridCollectionByName(XDMFGRIDCOLLECTION * domain, char * Name);
%ignore XdmfGridCollectionGetNumberGridCollections(XDMFGRIDCOLLECTION * domain);
%ignore XdmfGridCollectionInsertGridCollection(XDMFGRIDCOLLECTION * domain,
                                       XDMFGRIDCOLLECTION * GridCollection,
                                       int passControl);
%ignore XdmfGridCollectionRemoveGridCollection(XDMFGRIDCOLLECTION * domain, unsigned int index);
%ignore XdmfGridCollectionRemoveGridCollectionByName(XDMFGRIDCOLLECTION * domain, char * Name);
%ignore XdmfGridCollectionGetGraph(XDMFGRIDCOLLECTION * domain, unsigned int index);
%ignore XdmfGridCollectionGetGraphByName(XDMFGRIDCOLLECTION * domain, char * Name);
%ignore XdmfGridCollectionGetNumberGraphs(XDMFGRIDCOLLECTION * domain);
%ignore XdmfGridCollectionInsertGraph(XDMFGRIDCOLLECTION * domain, XDMFGRAPH * graph, int passControl);
%ignore XdmfGridCollectionRemoveGraph(XDMFGRIDCOLLECTION * domain, unsigned int index);
%ignore XdmfGridCollectionRemoveGraphByName(XDMFGRIDCOLLECTION * domain, char * Name);
%ignore XdmfGridCollectionGetCurvilinearGrid(XDMFGRIDCOLLECTION * domain, unsigned int index);
%ignore XdmfGridCollectionGetCurvilinearGridByName(XDMFGRIDCOLLECTION * domain, char * Name);
%ignore XdmfGridCollectionGetNumberCurvilinearGrids(XDMFGRIDCOLLECTION * domain);
%ignore XdmfGridCollectionInsertCurvilinearGrid(XDMFGRIDCOLLECTION * domain,
                                        XDMFCURVILINEARGRID * CurvilinearGrid,
                                        int passControl);
%ignore XdmfGridCollectionRemoveCurvilinearGrid(XDMFGRIDCOLLECTION * domain, unsigned int index);
%ignore XdmfGridCollectionRemoveCurvilinearGridByName(XDMFGRIDCOLLECTION * domain, char * Name);
%ignore XdmfGridCollectionGetRectilinearGrid(XDMFGRIDCOLLECTION * domain, unsigned int index);
%ignore XdmfGridCollectionGetRectilinearGridByName(XDMFGRIDCOLLECTION * domain, char * Name);
%ignore XdmfGridCollectionGetNumberRectilinearGrids(XDMFGRIDCOLLECTION * domain);
%ignore XdmfGridCollectionInsertRectilinearGrid(XDMFGRIDCOLLECTION * domain,
                                        XDMFRECTILINEARGRID * RectilinearGrid,
                                        int passControl);
%ignore XdmfGridCollectionRemoveRectilinearGrid(XDMFGRIDCOLLECTION * domain, unsigned int index);
%ignore XdmfGridCollectionRemoveRectilinearGridByName(XDMFGRIDCOLLECTION * domain, char * Name);
%ignore XdmfGridCollectionGetRegularGrid(XDMFGRIDCOLLECTION * domain, unsigned int index);
%ignore XdmfGridCollectionGetRegularGridByName(XDMFGRIDCOLLECTION * domain, char * Name);
%ignore XdmfGridCollectionGetNumberRegularGrids(XDMFGRIDCOLLECTION * domain);
%ignore XdmfGridCollectionInsertRegularGrid(XDMFGRIDCOLLECTION * domain,
                                    XDMFREGULARGRID * RegularGrid,
                                    int passControl);
%ignore XdmfGridCollectionRemoveRegularGrid(XDMFGRIDCOLLECTION * domain, unsigned int index);
%ignore XdmfGridCollectionRemoveRegularGridByName(XDMFGRIDCOLLECTION * domain, char * Name);
%ignore XdmfGridCollectionGetUnstructuredGrid(XDMFGRIDCOLLECTION * domain, unsigned int index);
%ignore XdmfGridCollectionGetUnstructuredGridByName(XDMFGRIDCOLLECTION * domain, char * Name);
%ignore XdmfGridCollectionGetNumberUnstructuredGrids(XDMFGRIDCOLLECTION * domain);
%ignore XdmfGridCollectionInsertUnstructuredGrid(XDMFGRIDCOLLECTION * domain,
                                         XDMFUNSTRUCTUREDGRID * UnstructuredGrid,
                                         int passControl);
%ignore XdmfGridCollectionRemoveUnstructuredGrid(XDMFGRIDCOLLECTION * domain, unsigned int index);
%ignore XdmfGridCollectionRemoveUnstructuredGridByName(XDMFGRIDCOLLECTION * domain, char * Name);
// XdmfGridCollection inherited from XdmfGrid
%ignore XdmfGridCollectionGetAttribute(XDMFGRIDCOLLECTION * grid, unsigned int index);
%ignore XdmfGridCollectionGetAttributeByName(XDMFGRIDCOLLECTION * grid, char * Name);
%ignore XdmfGridCollectionGetNumberAttributes(XDMFGRIDCOLLECTION * grid);
%ignore XdmfGridCollectionInsertAttribute(XDMFGRIDCOLLECTION * grid, XDMFATTRIBUTE * Attribute, int passControl);
%ignore XdmfGridCollectionRemoveAttribute(XDMFGRIDCOLLECTION * grid, unsigned int index);
%ignore XdmfGridCollectionRemoveAttributeByName(XDMFGRIDCOLLECTION * grid, char * Name);
%ignore XdmfGridCollectionGetSet(XDMFGRIDCOLLECTION * grid, unsigned int index);
%ignore XdmfGridCollectionGetSetByName(XDMFGRIDCOLLECTION * grid, char * Name);
%ignore XdmfGridCollectionGetNumberSets(XDMFGRIDCOLLECTION * grid);
%ignore XdmfGridCollectionInsertSet(XDMFGRIDCOLLECTION * grid, XDMFSET * Set, int passControl);
%ignore XdmfGridCollectionRemoveSet(XDMFGRIDCOLLECTION * grid, unsigned int index);
%ignore XdmfGridCollectionRemoveSetByName(XDMFGRIDCOLLECTION * grid, char * Name);
%ignore XdmfGridCollectionGetMap(XDMFGRIDCOLLECTION * grid, unsigned int index);
%ignore XdmfGridCollectionGetMapByName(XDMFGRIDCOLLECTION * grid, char * Name);
%ignore XdmfGridCollectionGetNumberMaps(XDMFGRIDCOLLECTION * grid);
%ignore XdmfGridCollectionInsertMap(XDMFGRIDCOLLECTION * grid, XDMFMAP * Map, int passControl);
%ignore XdmfGridCollectionRemoveMap(XDMFGRIDCOLLECTION * grid, unsigned int index);
%ignore XdmfGridCollectionRemoveMapByName(XDMFGRIDCOLLECTION * grid, char * Name);
%ignore XdmfGridCollectionGetName(XDMFGRIDCOLLECTION * grid);
%ignore XdmfGridCollectionGetTime(XDMFGRIDCOLLECTION * grid);
%ignore XdmfGridCollectionSetName(XDMFGRIDCOLLECTION * grid, char * name, int * status);
%ignore XdmfGridCollectionSetTime(XDMFGRIDCOLLECTION * grid, XDMFTIME * time, int passControl);
// XdmfGridCollection inherited from XdmfItem
%ignore XdmfGridCollectionAccept(XDMFGRIDCOLLECTION * item, XDMFVISITOR * visitor, int * status);
%ignore XdmfGridCollectionFree(void * item);
%ignore XdmfGridCollectionGetInformation(XDMFGRIDCOLLECTION * item, unsigned int index);
%ignore XdmfGridCollectionGetInformationByKey(XDMFGRIDCOLLECTION * item, char * key);
%ignore XdmfGridCollectionGetNumberInformations(XDMFGRIDCOLLECTION * item);
%ignore XdmfGridCollectionInsertInformation(XDMFGRIDCOLLECTION * item, XDMFINFORMATION * information, int passControl);
%ignore XdmfGridCollectionRemoveInformation(XDMFGRIDCOLLECTION * item, unsigned int index);
%ignore XdmfGridCollectionRemoveInformationByKey(XDMFGRIDCOLLECTION * item, char * key);
%ignore XdmfGridCollectionGetItemTag(XDMFGRIDCOLLECTION * item);

// XdmfGrid

// XdmfGrid inherited from XdmfGrid
%ignore XdmfGridGetAttribute(XDMFGRID * grid, unsigned int index);
%ignore XdmfGridGetAttributeByName(XDMFGRID * grid, char * Name);
%ignore XdmfGridGetNumberAttributes(XDMFGRID * grid);
%ignore XdmfGridInsertAttribute(XDMFGRID * grid, XDMFATTRIBUTE * Attribute, int passControl);
%ignore XdmfGridRemoveAttribute(XDMFGRID * grid, unsigned int index);
%ignore XdmfGridRemoveAttributeByName(XDMFGRID * grid, char * Name);
%ignore XdmfGridGetSet(XDMFGRID * grid, unsigned int index);
%ignore XdmfGridGetSetByName(XDMFGRID * grid, char * Name);
%ignore XdmfGridGetNumberSets(XDMFGRID * grid);
%ignore XdmfGridInsertSet(XDMFGRID * grid, XDMFSET * Set, int passControl);
%ignore XdmfGridRemoveSet(XDMFGRID * grid, unsigned int index);
%ignore XdmfGridRemoveSetByName(XDMFGRID * grid, char * Name);
%ignore XdmfGridGetMap(XDMFGRID * grid, unsigned int index);
%ignore XdmfGridGetMapByName(XDMFGRID * grid, char * Name);
%ignore XdmfGridGetNumberMaps(XDMFGRID * grid);
%ignore XdmfGridInsertMap(XDMFGRID * grid, XDMFMAP * Map, int passControl);
%ignore XdmfGridRemoveMap(XDMFGRID * grid, unsigned int index);
%ignore XdmfGridRemoveMapByName(XDMFGRID * grid, char * Name);
%ignore XdmfGridGetName(XDMFGRID * grid);
%ignore XdmfGridGetTime(XDMFGRID * grid);
%ignore XdmfGridSetName(XDMFGRID * grid, char * name, int * status);
%ignore XdmfGridSetTime(XDMFGRID * grid, XDMFTIME * time, int passControl);
// XdmfGrid inherited from XdmfItem
%ignore XdmfGridAccept(XDMFGRID * item, XDMFVISITOR * visitor, int * status);
%ignore XdmfGridFree(void * item);
%ignore XdmfGridGetInformation(XDMFGRID * item, unsigned int index);
%ignore XdmfGridGetInformationByKey(XDMFGRID * item, char * key);
%ignore XdmfGridGetNumberInformations(XDMFGRID * item);
%ignore XdmfGridInsertInformation(XDMFGRID * item, XDMFINFORMATION * information, int passControl);
%ignore XdmfGridRemoveInformation(XDMFGRID * item, unsigned int index);
%ignore XdmfGridRemoveInformationByKey(XDMFGRID * item, char * key);
%ignore XdmfGridGetItemTag(XDMFGRID * item);

// XdmfMap

%ignore XdmfMapNew();
%ignore XdmfMapNewFromIdVector(int ** globalNodeIds, int * numIdsOnNode, int numIds);
%ignore XdmfMapGetName(XDMFMAP * map);
%ignore XdmfMapInsert(XDMFMAP * map, int remoteTaskId, int localNodeId, int remoteLocalNodeId);
%ignore XdmfMapIsInitialized(XDMFMAP * map);
%ignore XdmfMapRead(XDMFMAP * map, int * status);
%ignore XdmfMapRelease(XDMFMAP * map);
%ignore XdmfMapRetrieveLocalNodeIds(XDMFMAP * map, int remoteTaskId);
%ignore XdmfMapRetrieveNumberLocalNodeIds(XDMFMAP * map, int remoteTaskId);
%ignore XdmfMapRetrieveNumberRemoteTaskIds(XDMFMAP * map);
%ignore XdmfMapRetrieveNumberRemoteNodeIds(XDMFMAP * map, int remoteTaskId, int localNodeId);
%ignore XdmfMapRetrieveRemoteTaskIds(XDMFMAP * map);
%ignore XdmfMapRetrieveRemoteNodeIds(XDMFMAP * map, int remoteTaskId, int localNodeId);
%ignore XdmfMapSetHeavyDataControllers(XDMFMAP * map,
                                       XDMFHEAVYDATACONTROLLER ** remoteTaskControllers,
                                       int numRemoteTaskControllers,
                                       XDMFHEAVYDATACONTROLLER ** localNodeControllers,
                                       int numberLocalNodeControllers,
                                       XDMFHEAVYDATACONTROLLER ** remoteLocalNodeControllers,
                                       int numRemoteLocalNodeControllers,
                                       int passControl,
                                       int * status);
%ignore XdmfMapSetName(XDMFMAP * map, char * newName);
// XdmfMap inherited from XdmfItem
%ignore XdmfMapAccept(XDMFMAP * item, XDMFVISITOR * visitor, int * status);
%ignore XdmfMapFree(void * item);
%ignore XdmfMapGetInformation(XDMFMAP * item, unsigned int index);
%ignore XdmfMapGetInformationByKey(XDMFMAP * item, char * key);
%ignore XdmfMapGetNumberInformations(XDMFMAP * item);
%ignore XdmfMapInsertInformation(XDMFMAP * item, XDMFINFORMATION * information, int passControl);
%ignore XdmfMapRemoveInformation(XDMFMAP * item, unsigned int index);
%ignore XdmfMapRemoveInformationByKey(XDMFMAP * item, char * key);
%ignore XdmfMapGetItemTag(XDMFMAP * item);

// XdmfReader

%ignore XdmfReaderNew();
%ignore XdmfReaderFree(XDMFREADER * item);
// XdmfReader inherited from XdmfCoreReader
%ignore XdmfReaderRead(XDMFREADER * reader, char * filePath, int * status);

// XdmfRectilinearGrid

%ignore XdmfRectilinearGridNew(XDMFARRAY ** axesCoordinates,
                               unsigned int numCoordinates,
                               int passControl);
%ignore XdmfRectilinearGridNew2D(XDMFARRAY * xCoordinates,
                                 XDMFARRAY * yCoordinates,
                                 int passControl);
%ignore XdmfRectilinearGridNew3D(XDMFARRAY * xCoordinates,
                                 XDMFARRAY * yCoordinates,
                                 XDMFARRAY * zCoordinates,
                                 int passControl);
%ignore XdmfRectilinearGridGetCoordinatesByIndex(XDMFRECTILINEARGRID * grid,
                                                 unsigned int axisIndex,
                                                 int * status);
%ignore XdmfRectilinearGridGetCoordinates(XDMFRECTILINEARGRID * grid, int * status);
%ignore XdmfRectilinearGridGetNumberCoordinates(XDMFRECTILINEARGRID * grid, int * status);
%ignore XdmfRectilinearGridGetDimensions(XDMFRECTILINEARGRID * grid, int * status);
%ignore XdmfRectilinearGridSetCoordinates(XDMFRECTILINEARGRID * grid,
                                          XDMFARRAY ** axesCoordinates,
                                          unsigned int numCoordinates,
                                          int passControl,
                                          int * status);
%ignore XdmfRectilinearGridSetCoordinatesByIndex(XDMFRECTILINEARGRID * grid,
                                                 unsigned int index,
                                                 XDMFARRAY * coordinates,
                                                 int passControl,
                                                 int * status);
// XdmfRectilinearGrid inherited from XdmfItem
%ignore XdmfRectilinearGridAccept(XDMFRECTILINEARGRID * item, XDMFVISITOR * visitor, int * status);
%ignore XdmfRectilinearGridFree(void * item);
%ignore XdmfRectilinearGridGetInformation(XDMFRECTILINEARGRID * item, unsigned int index);
%ignore XdmfRectilinearGridGetInformationByKey(XDMFRECTILINEARGRID * item, char * key);
%ignore XdmfRectilinearGridGetNumberInformations(XDMFRECTILINEARGRID * item);
%ignore XdmfRectilinearGridInsertInformation(XDMFRECTILINEARGRID * item, XDMFINFORMATION * information, int passControl);
%ignore XdmfRectilinearGridRemoveInformation(XDMFRECTILINEARGRID * item, unsigned int index);
%ignore XdmfRectilinearGridRemoveInformationByKey(XDMFRECTILINEARGRID * item, char * key);
%ignore XdmfRectilinearGridGetItemTag(XDMFRECTILINEARGRID * item);
// XdmfRectilinearGrid inherited from XdmfGrid
%ignore XdmfRectilinearGridGetAttribute(XDMFRECTILINEARGRID * grid, unsigned int index);
%ignore XdmfRectilinearGridGetAttributeByName(XDMFRECTILINEARGRID * grid, char * Name);
%ignore XdmfRectilinearGridGetNumberAttributes(XDMFRECTILINEARGRID * grid);
%ignore XdmfRectilinearGridInsertAttribute(XDMFRECTILINEARGRID * grid, XDMFATTRIBUTE * Attribute, int passControl);
%ignore XdmfRectilinearGridRemoveAttribute(XDMFRECTILINEARGRID * grid, unsigned int index);
%ignore XdmfRectilinearGridRemoveAttributeByName(XDMFRECTILINEARGRID * grid, char * Name);
%ignore XdmfRectilinearGridGetSet(XDMFRECTILINEARGRID * grid, unsigned int index);
%ignore XdmfRectilinearGridGetSetByName(XDMFRECTILINEARGRID * grid, char * Name);
%ignore XdmfRectilinearGridGetNumberSets(XDMFRECTILINEARGRID * grid);
%ignore XdmfRectilinearGridInsertSet(XDMFRECTILINEARGRID * grid, XDMFSET * Set, int passControl);
%ignore XdmfRectilinearGridRemoveSet(XDMFRECTILINEARGRID * grid, unsigned int index);
%ignore XdmfRectilinearGridRemoveSetByName(XDMFRECTILINEARGRID * grid, char * Name);
%ignore XdmfRectilinearGridGetMap(XDMFRECTILINEARGRID * grid, unsigned int index);
%ignore XdmfRectilinearGridGetMapByName(XDMFRECTILINEARGRID * grid, char * Name);
%ignore XdmfRectilinearGridGetNumberMaps(XDMFRECTILINEARGRID * grid);
%ignore XdmfRectilinearGridInsertMap(XDMFRECTILINEARGRID * grid, XDMFMAP * Map, int passControl);
%ignore XdmfRectilinearGridRemoveMap(XDMFRECTILINEARGRID * grid, unsigned int index);
%ignore XdmfRectilinearGridRemoveMapByName(XDMFRECTILINEARGRID * grid, char * Name);
%ignore XdmfRectilinearGridGetName(XDMFRECTILINEARGRID * grid);
%ignore XdmfRectilinearGridGetTime(XDMFRECTILINEARGRID * grid);
%ignore XdmfRectilinearGridSetName(XDMFRECTILINEARGRID * grid, char * name, int * status);
%ignore XdmfRectilinearGridSetTime(XDMFRECTILINEARGRID * grid, XDMFTIME * time, int passControl);

// XdmfRegularGrid

%ignore XdmfRegularGridNew2D(double xBrickSize,
                             double yBrickSize,
                             unsigned int xNumPoints,
                             unsigned int yNumPoints,
                             double xOrigin,
                             double yOrigin);
%ignore XdmfRegularGridNew3D(double xBrickSize,
                             double yBrickSize,
                             double zBrickSize,
                             unsigned int xNumPoints,
                             unsigned int yNumPoints,
                             unsigned int zNumPoints,
                             double xOrigin,
                             double yOrigin,
                             double zOrigin);
%ignore XdmfRegularGridNew(XDMFARRAY * brickSize,
                           XDMFARRAY * numPoints,
                           XDMFARRAY * origin,
                           int passControl);
%ignore XdmfRegularGridGetBrickSize(XDMFREGULARGRID * grid, int * status);
%ignore XdmfRegularGridGetDimensions(XDMFREGULARGRID * grid, int * status);
%ignore XdmfRegularGridGetOrigin(XDMFREGULARGRID * grid, int * status);
%ignore XdmfRegularGridSetBrickSize(XDMFREGULARGRID * grid,
                                    XDMFARRAY * brickSize,
                                    int passControl,
                                    int * status);
%ignore XdmfRegularGridSetDimensions(XDMFREGULARGRID * grid,
                                     XDMFARRAY * dimensions,
                                     int passControl,
                                     int * status);
%ignore XdmfRegularGridSetOrigin(XDMFREGULARGRID * grid,
                                 XDMFARRAY * origin,
                                 int passControl,
                                 int * status);
// XdmfRegularGrid inherited from XdmfItem
%ignore XdmfRegularGridAccept(XDMFREGULARGRID * item, XDMFVISITOR * visitor, int * status);
%ignore XdmfRegularGridFree(void * item);
%ignore XdmfRegularGridGetInformation(XDMFREGULARGRID * item, unsigned int index);
%ignore XdmfRegularGridGetInformationByKey(XDMFREGULARGRID * item, char * key);
%ignore XdmfRegularGridGetNumberInformations(XDMFREGULARGRID * item);
%ignore XdmfRegularGridInsertInformation(XDMFREGULARGRID * item, XDMFINFORMATION * information, int passControl);
%ignore XdmfRegularGridRemoveInformation(XDMFREGULARGRID * item, unsigned int index);
%ignore XdmfRegularGridRemoveInformationByKey(XDMFREGULARGRID * item, char * key);
%ignore XdmfRegularGridGetItemTag(XDMFREGULARGRID * item);
// XdmfRegularGrid inherited from XdmfGrid
%ignore XdmfRegularGridGetAttribute(XDMFREGULARGRID * grid, unsigned int index);
%ignore XdmfRegularGridGetAttributeByName(XDMFREGULARGRID * grid, char * Name);
%ignore XdmfRegularGridGetNumberAttributes(XDMFREGULARGRID * grid);
%ignore XdmfRegularGridInsertAttribute(XDMFREGULARGRID * grid, XDMFATTRIBUTE * Attribute, int passControl);
%ignore XdmfRegularGridRemoveAttribute(XDMFREGULARGRID * grid, unsigned int index);
%ignore XdmfRegularGridRemoveAttributeByName(XDMFREGULARGRID * grid, char * Name);
%ignore XdmfRegularGridGetSet(XDMFREGULARGRID * grid, unsigned int index);
%ignore XdmfRegularGridGetSetByName(XDMFREGULARGRID * grid, char * Name);
%ignore XdmfRegularGridGetNumberSets(XDMFREGULARGRID * grid);
%ignore XdmfRegularGridInsertSet(XDMFREGULARGRID * grid, XDMFSET * Set, int passControl);
%ignore XdmfRegularGridRemoveSet(XDMFREGULARGRID * grid, unsigned int index);
%ignore XdmfRegularGridRemoveSetByName(XDMFREGULARGRID * grid, char * Name);
%ignore XdmfRegularGridGetMap(XDMFREGULARGRID * grid, unsigned int index);
%ignore XdmfRegularGridGetMapByName(XDMFREGULARGRID * grid, char * Name);
%ignore XdmfRegularGridGetNumberMaps(XDMFREGULARGRID * grid);
%ignore XdmfRegularGridInsertMap(XDMFREGULARGRID * grid, XDMFMAP * Map, int passControl);
%ignore XdmfRegularGridRemoveMap(XDMFREGULARGRID * grid, unsigned int index);
%ignore XdmfRegularGridRemoveMapByName(XDMFREGULARGRID * grid, char * Name);
%ignore XdmfRegularGridGetName(XDMFREGULARGRID * grid);
%ignore XdmfRegularGridGetTime(XDMFREGULARGRID * grid);
%ignore XdmfRegularGridSetName(XDMFREGULARGRID * grid, char * name, int * status);
%ignore XdmfRegularGridSetTime(XDMFREGULARGRID * grid, XDMFTIME * time, int passControl);

// XdmfSet

%ignore XdmfSetNew();
%ignore XdmfSetGetAttribute(XDMFSET * set, unsigned int index);
%ignore XdmfSetGetAttributeByName(XDMFSET * set, char * Name);
%ignore XdmfSetGetNumberAttributes(XDMFSET * set);
%ignore XdmfSetGetType(XDMFSET * set);
%ignore XdmfSetInsertAttribute(XDMFSET * set, XDMFATTRIBUTE * Attribute, int passControl);
vXdmfSetRemoveAttribute(XDMFSET * set, unsigned int index);
%ignore XdmfSetRemoveAttributeByName(XDMFSET * set, char * Name);
%ignore XdmfSetSetType(XDMFSET * set, int type, int * status);
// XdmfSet inherited from XdmfArray
%ignore XdmfSetNew();
%ignore XdmfSetClear(XDMFSET * array);
%ignore XdmfSetErase(XDMFSET * array, unsigned int index);
%ignore XdmfSetGetArrayType(XDMFSET * array, int * status);
%ignore XdmfSetGetCapacity(XDMFSET * array);
%ignore XdmfSetGetDimensions(XDMFSET * array);
%ignore XdmfSetGetDimensionsString(XDMFSET * array);
%ignore XdmfSetGetHeavyDataController(XDMFSET * array, unsigned int index);
%ignore XdmfSetGetReadMode(XDMFSET * array, int * status);
%ignore XdmfSetGetName(XDMFSET * array);
%ignore XdmfSetGetNumberDimensions(XDMFSET * array);
%ignore XdmfSetGetNumberHeavyDataControllers(XDMFSET * array);
%ignore XdmfSetGetSize(XDMFSET * array);
%ignore XdmfSetGetReference(XDMFSET * array);
%ignore XdmfSetGetValue(XDMFSET * array, unsigned int index, int arrayType, int * status);
%ignore XdmfSetGetValues(XDMFSET * array, unsigned int startIndex, int arrayType, unsigned int numValues, unsigned int arrayStride, unsigned int valueStride, int * status);
%ignore XdmfSetGetValuesInternal(XDMFSET * array);
%ignore XdmfSetGetValuesString(XDMFSET * array);
%ignore XdmfSetInitialize(XDMFSET * array, int * dims, int numDims, int arrayType, int * status);
%ignore XdmfSetInsertDataFromPointer(XDMFSET * array, void * values, int arrayType, unsigned int startIndex, unsigned int numVals, unsigned int arrayStride, unsigned int valueStride, int * status);
%ignore XdmfSetInsertDataFromXdmfArray(XDMFSET * array, XDMFARRAY * valArray, int * arrayStarts, int * valueStarts, int * arrayCounts, int * valueCounts, int * arrayStrides, int * valueStrides, int * status);
%ignore XdmfSetInsertHeavyDataController(XDMFSET * array, XDMFHEAVYDATACONTROLLER * controller, int passControl);
%ignore XdmfSetInsertValue(XDMFSET * array, unsigned int index, void * value, int arrayType, int * status);
%ignore XdmfSetIsInitialized(XDMFSET * array);
%ignore XdmfSetPushBack(XDMFSET * array, void * value, int arrayType, int * status);
%ignore XdmfSetRead(XDMFSET * array, int * status);
%ignore XdmfSetReadController(XDMFSET * array, int * status);
%ignore XdmfSetReadReference(XDMFSET * array, int * status);
%ignore XdmfSetRelease(XDMFSET * array);
%ignore XdmfSetRemoveHeavyDataController(XDMFSET * array, unsigned int index);
%ignore XdmfSetReserve(XDMFSET * array, int size);
%ignore XdmfSetResize(XDMFSET * array, int * dims, int numDims, int arrayType, int * status);
%ignore XdmfSetSetReadMode(XDMFSET * array, int readMode, int * status);
%ignore XdmfSetSetReference(XDMFSET * array, XDMFARRAYREFERENCE * reference, int passControl);
%ignore XdmfSetSetName(XDMFSET * array, char * name, int * status);
%ignore XdmfSetSetValuesInternal(XDMFSET * array, void * pointer, unsigned int numValues, int arrayType, int transferOwnership, int * status);
%ignore XdmfSetSwapWithXdmfArray(XDMFSET * array, XDMFARRAY * swapArray);
%ignore XdmfSetSwapWithArray(XDMFSET * array, void ** pointer, int numValues, int arrayType, int * status);
// XdmfSet inherited from XdmfItem
%ignore XdmfSetAccept(XDMFSET * item, XDMFVISITOR * visitor, int * status);
%ignore XdmfSetFree(void * item);
%ignore XdmfSetGetInformation(XDMFSET * item, unsigned int index);
%ignore XdmfSetGetInformationByKey(XDMFSET * item, char * key);
%ignore XdmfSetGetNumberInformations(XDMFSET * item);
%ignore XdmfSetInsertInformation(XDMFSET * item, XDMFINFORMATION * information, int passControl);
%ignore XdmfSetRemoveInformation(XDMFSET * item, unsigned int index);
%ignore XdmfSetRemoveInformationByKey(XDMFSET * item, char * key);
%ignore XdmfSetGetItemTag(XDMFSET * item);

// XdmfSetType

%ignore XdmfSetTypeNoSetType();
%ignore XdmfSetTypeNode();
%ignore XdmfSetTypeCell();
%ignore XdmfSetTypeFace();
%ignore XdmfSetTypeEdge();

// XdmfTime

%ignore XdmfTimeNew(double value);
%ignore XdmfTimeGetValue(XDMFTIME * timePointer);
%ignore XdmfTimeSetValue(XDMFTIME * timePointer, double time);
// XdmfTime inherited from XdmfItem
%ignore XdmfTimeAccept(XDMFTIME * item, XDMFVISITOR * visitor, int * status);
%ignore XdmfTimeFree(void * item);
%ignore XdmfTimeGetInformation(XDMFTIME * item, unsigned int index);
%ignore XdmfTimeGetInformationByKey(XDMFTIME * item, char * key);
%ignore XdmfTimeGetNumberInformations(XDMFTIME * item);
%ignore XdmfTimeInsertInformation(XDMFTIME * item, XDMFINFORMATION * information, int passControl);
%ignore XdmfTimeRemoveInformation(XDMFTIME * item, unsigned int index);
%ignore XdmfTimeRemoveInformationByKey(XDMFTIME * item, char * key);
%ignore XdmfTimeGetItemTag(XDMFTIME * item);

// XdmfTopology

%ignore XdmfTopologyNew();
%ignore XdmfTopologyGetNumberElements(XDMFTOPOLOGY * topology, int * status);
%ignore XdmfTopologyGetType(XDMFTOPOLOGY * topology);
%ignore XdmfTopologySetType(XDMFTOPOLOGY * topology, int type, int * status);
%ignore XdmfTopologySetPolyType(XDMFTOPOLOGY * topology, int type, int nodes, int * status);
// XdmfTopology inherited from XdmfArray
%ignore XdmfTopologyNew();
%ignore XdmfTopologyClear(XDMFTOPOLOGY * array);
%ignore XdmfTopologyErase(XDMFTOPOLOGY * array, unsigned int index);
%ignore XdmfTopologyGetArrayType(XDMFTOPOLOGY * array, int * status);
%ignore XdmfTopologyGetCapacity(XDMFTOPOLOGY * array);
%ignore XdmfTopologyGetDimensions(XDMFTOPOLOGY * array);
%ignore XdmfTopologyGetDimensionsString(XDMFTOPOLOGY * array);
%ignore XdmfTopologyGetHeavyDataController(XDMFTOPOLOGY * array, unsigned int index);
%ignore XdmfTopologyGetReadMode(XDMFTOPOLOGY * array, int * status);
%ignore XdmfTopologyGetName(XDMFTOPOLOGY * array);
%ignore XdmfTopologyGetNumberDimensions(XDMFTOPOLOGY * array);
%ignore XdmfTopologyGetNumberHeavyDataControllers(XDMFTOPOLOGY * array);
%ignore XdmfTopologyGetSize(XDMFTOPOLOGY * array);
%ignore XdmfTopologyGetReference(XDMFTOPOLOGY * array);
%ignore XdmfTopologyGetValue(XDMFTOPOLOGY * array, unsigned int index, int arrayType, int * status);
%ignore XdmfTopologyGetValues(XDMFTOPOLOGY * array, unsigned int startIndex, int arrayType, unsigned int numValues, unsigned int arrayStride, unsigned int valueStride, int * status);
%ignore XdmfTopologyGetValuesInternal(XDMFTOPOLOGY * array);
%ignore XdmfTopologyGetValuesString(XDMFTOPOLOGY * array);
%ignore XdmfTopologyInitialize(XDMFTOPOLOGY * array, int * dims, int numDims, int arrayType, int * status);
%ignore XdmfTopologyInsertDataFromPointer(XDMFTOPOLOGY * array, void * values, int arrayType, unsigned int startIndex, unsigned int numVals, unsigned int arrayStride, unsigned int valueStride, int * status);
%ignore XdmfTopologyInsertDataFromXdmfArray(XDMFTOPOLOGY * array, XDMFARRAY * valArray, int * arrayStarts, int * valueStarts, int * arrayCounts, int * valueCounts, int * arrayStrides, int * valueStrides, int * status);
%ignore XdmfTopologyInsertHeavyDataController(XDMFTOPOLOGY * array, XDMFHEAVYDATACONTROLLER * controller, int passControl);
%ignore XdmfTopologyInsertValue(XDMFTOPOLOGY * array, unsigned int index, void * value, int arrayType, int * status);
%ignore XdmfTopologyIsInitialized(XDMFTOPOLOGY * array);
%ignore XdmfTopologyPushBack(XDMFTOPOLOGY * array, void * value, int arrayType, int * status);
%ignore XdmfTopologyRead(XDMFTOPOLOGY * array, int * status);
%ignore XdmfTopologyReadController(XDMFTOPOLOGY * array, int * status);
%ignore XdmfTopologyReadReference(XDMFTOPOLOGY * array, int * status);
%ignore XdmfTopologyRelease(XDMFTOPOLOGY * array);
%ignore XdmfTopologyRemoveHeavyDataController(XDMFTOPOLOGY * array, unsigned int index);
%ignore XdmfTopologyReserve(XDMFTOPOLOGY * array, int size);
%ignore XdmfTopologyResize(XDMFTOPOLOGY * array, int * dims, int numDims, int arrayType, int * status);
%ignore XdmfTopologySetReadMode(XDMFTOPOLOGY * array, int readMode, int * status);
%ignore XdmfTopologySetReference(XDMFTOPOLOGY * array, XDMFARRAYREFERENCE * reference, int passControl);
%ignore XdmfTopologySetName(XDMFTOPOLOGY * array, char * name, int * status);
%ignore XdmfTopologySetValuesInternal(XDMFTOPOLOGY * array, void * pointer, unsigned int numValues, int arrayType, int transferOwnership, int * status);
%ignore XdmfTopologySwapWithXdmfArray(XDMFTOPOLOGY * array, XDMFARRAY * swapArray);
%ignore XdmfTopologySwapWithArray(XDMFTOPOLOGY * array, void ** pointer, int numValues, int arrayType, int * status);
// XdmfTopology inherited from XdmfItem
%ignore XdmfTopologyAccept(XDMFTOPOLOGY * item, XDMFVISITOR * visitor, int * status);
%ignore XdmfTopologyFree(void * item);
%ignore XdmfTopologyGetInformation(XDMFTOPOLOGY * item, unsigned int index);
%ignore XdmfTopologyGetInformationByKey(XDMFTOPOLOGY * item, char * key);
%ignore XdmfTopologyGetNumberInformations(XDMFTOPOLOGY * item);
%ignore XdmfTopologyInsertInformation(XDMFTOPOLOGY * item, XDMFINFORMATION * information, int passControl);
%ignore XdmfTopologyRemoveInformation(XDMFTOPOLOGY * item, unsigned int index);
%ignore XdmfTopologyRemoveInformationByKey(XDMFTOPOLOGY * item, char * key);
%ignore XdmfTopologyGetItemTag(XDMFTOPOLOGY * item);

// XdmfTopologyType

%ignore XdmfTopologyTypePolyvertex();
%ignore XdmfTopologyTypePolyline();
%ignore XdmfTopologyTypePolygon();
%ignore XdmfTopologyTypeTriangle();
%ignore XdmfTopologyTypeQuadrilateral();
%ignore XdmfTopologyTypeTetrahedron();
%ignore XdmfTopologyTypePyramid();
%ignore XdmfTopologyTypeWedge();
%ignore XdmfTopologyTypeHexahedron();
%ignore XdmfTopologyTypeEdge_3();
%ignore XdmfTopologyTypeTriangle_6();
%ignore XdmfTopologyTypeQuadrilateral_8();
%ignore XdmfTopologyTypeQuadrilateral_9();
%ignore XdmfTopologyTypeTetrahedron_10();
%ignore XdmfTopologyTypePyramid_13();
%ignore XdmfTopologyTypeWedge_15();
%ignore XdmfTopologyTypeWedge_18();
%ignore XdmfTopologyTypeHexahedron_20();
%ignore XdmfTopologyTypeHexahedron_24();
%ignore XdmfTopologyTypeHexahedron_27();
%ignore XdmfTopologyTypeHexahedron_64();
%ignore XdmfTopologyTypeHexahedron_125();
%ignore XdmfTopologyTypeHexahedron_216();
%ignore XdmfTopologyTypeHexahedron_343();
%ignore XdmfTopologyTypeHexahedron_512();
%ignore XdmfTopologyTypeHexahedron_729();
%ignore XdmfTopologyTypeHexahedron_1000();
%ignore XdmfTopologyTypeHexahedron_1331();
%ignore XdmfTopologyTypeHexahedron_Spectral_64();
%ignore XdmfTopologyTypeHexahedron_Spectral_125();
%ignore XdmfTopologyTypeHexahedron_Spectral_216();
%ignore XdmfTopologyTypeHexahedron_Spectral_343();
%ignore XdmfTopologyTypeHexahedron_Spectral_512();
%ignore XdmfTopologyTypeHexahedron_Spectral_729();
%ignore XdmfTopologyTypeHexahedron_Spectral_1000();
%ignore XdmfTopologyTypeHexahedron_Spectral_1331();
%ignore XdmfTopologyTypeMixed();
%ignore XdmfTopologyTypeGetCellType(int type);
%ignore XdmfTopologyTypeGetEdgesPerElement(int type, int * status);
%ignore XdmfTopologyTypeGetFacesPerElement(int type, int * status);
%ignore XdmfTopologyTypeGetFaceType(int type);
%ignore XdmfTopologyTypeGetID(int type);
%ignore XdmfTopologyTypeGetName(int type);
%ignore XdmfTopologyTypeGetNodesPerElement(int type);

// XdmfUnstructuredGrid

%ignore XdmfUnstructuredGridNew();
%ignore XdmfUnstructuredGridNewFromRegularGrid(XDMFREGULARGRID * regularGrid, int * status);
%ignore XdmfUnstructuredGridGetGeometry(XDMFUNSTRUCTUREDGRID * grid);
%ignore XdmfUnstructuredGridGetTopology(XDMFUNSTRUCTUREDGRID * grid);
%ignore XdmfUnstructuredGridSetGeometry(XDMFUNSTRUCTUREDGRID * grid,
                                        XDMFGEOMETRY * geometry,
                                        int passControl);
%ignore XdmfUnstructuredGridSetTopology(XDMFUNSTRUCTUREDGRID * grid,
                                        XDMFTOPOLOGY * topology,
                                        int passControl);
// XdmfUnstructuredGrid inherited from XdmfItem
%ignore XdmfUnstructuredGridAccept(XDMFUNSTRUCTUREDGRID * item, XDMFVISITOR * visitor, int * status);
%ignore XdmfUnstructuredGridFree(void * item);
%ignore XdmfUnstructuredGridGetInformation(XDMFUNSTRUCTUREDGRID * item, unsigned int index);
%ignore XdmfUnstructuredGridGetInformationByKey(XDMFUNSTRUCTUREDGRID * item, char * key);
%ignore XdmfUnstructuredGridGetNumberInformations(XDMFUNSTRUCTUREDGRID * item);
%ignore XdmfUnstructuredGridInsertInformation(XDMFUNSTRUCTUREDGRID * item, XDMFINFORMATION * information, int passControl);
%ignore XdmfUnstructuredGridRemoveInformation(XDMFUNSTRUCTUREDGRID * item, unsigned int index);
%ignore XdmfUnstructuredGridRemoveInformationByKey(XDMFUNSTRUCTUREDGRID * item, char * key);
%ignore XdmfUnstructuredGridGetItemTag(XDMFUNSTRUCTUREDGRID * item);
// XdmfUnstructuredGrid inherited from XdmfGrid
%ignore XdmfUnstructuredGridGetAttribute(XDMFUNSTRUCTUREDGRID * grid, unsigned int index);
%ignore XdmfUnstructuredGridGetAttributeByName(XDMFUNSTRUCTUREDGRID * grid, char * Name);
%ignore XdmfUnstructuredGridGetNumberAttributes(XDMFUNSTRUCTUREDGRID * grid);
%ignore XdmfUnstructuredGridInsertAttribute(XDMFUNSTRUCTUREDGRID * grid, XDMFATTRIBUTE * Attribute, int passControl);
%ignore XdmfUnstructuredGridRemoveAttribute(XDMFUNSTRUCTUREDGRID * grid, unsigned int index);
%ignore XdmfUnstructuredGridRemoveAttributeByName(XDMFUNSTRUCTUREDGRID * grid, char * Name);
%ignore XdmfUnstructuredGridGetSet(XDMFUNSTRUCTUREDGRID * grid, unsigned int index);
%ignore XdmfUnstructuredGridGetSetByName(XDMFUNSTRUCTUREDGRID * grid, char * Name);
%ignore XdmfUnstructuredGridGetNumberSets(XDMFUNSTRUCTUREDGRID * grid);
%ignore XdmfUnstructuredGridInsertSet(XDMFUNSTRUCTUREDGRID * grid, XDMFSET * Set, int passControl);
%ignore XdmfUnstructuredGridRemoveSet(XDMFUNSTRUCTUREDGRID * grid, unsigned int index);
%ignore XdmfUnstructuredGridRemoveSetByName(XDMFUNSTRUCTUREDGRID * grid, char * Name);
%ignore XdmfUnstructuredGridGetMap(XDMFUNSTRUCTUREDGRID * grid, unsigned int index);
%ignore XdmfUnstructuredGridGetMapByName(XDMFUNSTRUCTUREDGRID * grid, char * Name);
%ignore XdmfUnstructuredGridGetNumberMaps(XDMFUNSTRUCTUREDGRID * grid);
%ignore XdmfUnstructuredGridInsertMap(XDMFUNSTRUCTUREDGRID * grid, XDMFMAP * Map, int passControl);
%ignore XdmfUnstructuredGridRemoveMap(XDMFUNSTRUCTUREDGRID * grid, unsigned int index);
%ignore XdmfUnstructuredGridRemoveMapByName(XDMFUNSTRUCTUREDGRID * grid, char * Name);
%ignore XdmfUnstructuredGridGetName(XDMFUNSTRUCTUREDGRID * grid);
%ignore XdmfUnstructuredGridGetTime(XDMFUNSTRUCTUREDGRID * grid);
%ignore XdmfUnstructuredGridSetName(XDMFUNSTRUCTUREDGRID * grid, char * name, int * status);
%ignore XdmfUnstructuredGridSetTime(XDMFUNSTRUCTUREDGRID * grid, XDMFTIME * time, int passControl);

#ifdef SWIGJAVA

// Typemaps that work for Java

%typemap(out) shared_ptr<XdmfItem> {
    if(shared_ptr<XdmfAttribute> value = shared_dynamic_cast<XdmfAttribute>($1)) {
        *(shared_ptr< XdmfAttribute > **)&($result) = value ? new shared_ptr< XdmfAttribute >(value) : 0;
    }
    else if(shared_ptr<XdmfCurvilinearGrid> value = shared_dynamic_cast<XdmfCurvilinearGrid>($1)) {
        *(shared_ptr< XdmfCurvilinearGrid > **)&($result) = value ? new shared_ptr< XdmfCurvilinearGrid >(value) : 0;
    }
    else if(shared_ptr<XdmfGridCollection> value = shared_dynamic_cast<XdmfGridCollection>($1)) {
        *(shared_ptr< XdmfGridCollection > **)&($result) = value ? new shared_ptr< XdmfGridCollection >(value) : 0;
    }
    else if(shared_ptr<XdmfDomain> value = shared_dynamic_cast<XdmfDomain>($1)) {
        *(shared_ptr< XdmfDomain > **)&($result) = value ? new shared_ptr< XdmfDomain >(value) : 0;
    }
    else if(shared_ptr<XdmfGeometry> value = shared_dynamic_cast<XdmfGeometry>($1)) {
        *(shared_ptr< XdmfGeometry > **)&($result) = value ? new shared_ptr< XdmfGeometry >(value) : 0;
    }
    else if(shared_ptr<XdmfInformation> value = shared_dynamic_cast<XdmfInformation>($1)) {
        *(shared_ptr< XdmfInformation > **)&($result) = value ? new shared_ptr< XdmfInformation >(value) : 0;
    }
    else if(shared_ptr<XdmfRectilinearGrid> value = shared_dynamic_cast<XdmfRectilinearGrid>($1)) {
        *(shared_ptr< XdmfRectilinearGrid > **)&($result) = value ? new shared_ptr< XdmfRectilinearGrid >(value) : 0;
    }
    else if(shared_ptr<XdmfRegularGrid> value = shared_dynamic_cast<XdmfRegularGrid>($1)) {
        *(shared_ptr< XdmfRegularGrid > **)&($result) = value ? new shared_ptr< XdmfRegularGrid >(value) : 0;
    }
    else if(shared_ptr<XdmfSet> value = shared_dynamic_cast<XdmfSet>($1)) {
        *(shared_ptr< XdmfSet > **)&($result) = value ? new shared_ptr< XdmfSet >(value) : 0;
    }
    else if(shared_ptr<XdmfTime> value = shared_dynamic_cast<XdmfTime>($1)) {
        *(shared_ptr< XdmfTime > **)&($result) = value ? new shared_ptr< XdmfTime >(value) : 0;
    }
    else if(shared_ptr<XdmfTopology> value = shared_dynamic_cast<XdmfTopology>($1)) {
        *(shared_ptr< XdmfTopology > **)&($result) = value ? new shared_ptr< XdmfTopology >(value) : 0;
    }
    else if(shared_ptr<XdmfUnstructuredGrid> value = shared_dynamic_cast<XdmfUnstructuredGrid>($1)) {
        *(shared_ptr< XdmfUnstructuredGrid > **)&($result) = value ? new shared_ptr< XdmfUnstructuredGrid >(value) : 0;
    }
    else {
        *(shared_ptr< XdmfItem > **)&($result) = &($1);
    }
}

// Ignore multiple inheritance warning
#pragma SWIG nowarn=813

// Ignore const overloaded methods
%ignore XdmfCurvilinearGrid::getDimensions() const;
%ignore XdmfDomain::getCurvilinearGrid(const unsigned int) const;
%ignore XdmfDomain::getCurvilinearGrid(const std::string &) const;
%ignore XdmfDomain::getGridCollection(const unsigned int) const;
%ignore XdmfDomain::getGridCollection(const std::string &) const;
%ignore XdmfDomain::getRectilinearGrid(const unsigned int) const;
%ignore XdmfDomain::getRectilinearGrid(const std::string &) const;
%ignore XdmfDomain::getRegularGrid(const unsigned int) const;
%ignore XdmfDomain::getRegularGrid(const std::string &) const;
%ignore XdmfDomain::getUnstructuredGrid(const unsigned int) const;
%ignore XdmfDomain::getUnstructuredGrid(const std::string &) const;
%ignore XdmfGrid::getAttribute(const unsigned int) const;
%ignore XdmfGrid::getAttribute(const std::string &) const;
%ignore XdmfGrid::getMap() const;
%ignore XdmfGrid::getMap(unsigned int const) const;
%ignore XdmfGrid::getMap(unsigned int const &) const;
%ignore XdmfGrid::getMap(std::string const &) const;
%ignore XdmfGrid::getSet(const unsigned int) const;
%ignore XdmfGrid::getSet(const std::string &) const;
%ignore XdmfGrid::getTime() const;
%ignore XdmfRectilinearGrid::getCoordinates(const unsigned int) const;
%ignore XdmfRectilinearGrid::getCoordinates() const;
%ignore XdmfRectilinearGrid::getDimensions() const;
%ignore XdmfRegularGrid::getBrickSize() const;
%ignore XdmfRegularGrid::getDimensions() const;
%ignore XdmfRegularGrid::getOrigin() const;
%ignore XdmfSet::getAttribute(const unsigned int) const;
%ignore XdmfSet::getAttribute(const std::string &) const;

// Ignore ItemTags
%ignore XdmfAttribute::ItemTag;
%ignore XdmfCurvilinearGrid::ItemTag;
%ignore XdmfDomain::ItemTag;
%ignore XdmfGeometry::ItemTag;
%ignore XdmfGraph::ItemTag;
%ignore XdmfGrid::ItemTag;
%ignore XdmfGridCollection::ItemTag;
%ignore XdmfMap::ItemTag;
%ignore XdmfRectilinearGrid::ItemTag;
%ignore XdmfRegularGrid::ItemTag;
%ignore XdmfSet::ItemTag;
%ignore XdmfTime::ItemTag;
%ignore XdmfTopology::ItemTag;
%ignore XdmfUnstructuredGrid::ItemTag;

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("XdmfJava");
        }
        catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load for" +
                                "XdmfJava\n" + e);
            System.exit(1);
        }
    }
%}

#endif /* SWIGJAVA */

#ifdef SWIGPYTHON

%include std_set.i
%include std_map.i
%include std_vector.i

%template(XdmfMapNodeIdSet) std::set<int>;
%template(XdmfMapNodeIdMap) std::map<int, std::set<int> >;
%template(XdmfMapMap) std::map<int, std::map<int, std::set<int> > >;
%template(AttributeVector) std::vector<shared_ptr<XdmfAttribute> >;
%template(MapVector) std::vector<shared_ptr<XdmfMap> >;
%template(ArrayVector) std::vector<shared_ptr<XdmfArray> >;

#ifdef XDMF_BUILD_DSM

%pythoncode {
    import sys
    from XdmfCore import *
    if 'mpi4py' in sys.modules.keys():
      from XdmfDSM import *
}

#else

%pythoncode {
    from XdmfCore import *
}

#endif /* XDMF_BUILD_DSM */


%fragment("XdmfItemCast", "header") {
    #include <XdmfSharedPtr.hpp>
    PyObject * XdmfItemCast(shared_ptr<XdmfItem> obj) {
        PyObject * result;
        if(shared_ptr<XdmfAttribute> attribute = shared_dynamic_cast<XdmfAttribute>(obj)) {
            result = SWIG_NewPointerObj(SWIG_as_voidptr(new shared_ptr<XdmfAttribute>(attribute)), SWIGTYPE_p_std__shared_ptrT_XdmfAttribute_t, SWIG_POINTER_OWN);
        }
        else if(shared_ptr<XdmfCurvilinearGrid> gridCurvilinear = shared_dynamic_cast<XdmfCurvilinearGrid>(obj)) {
            result = SWIG_NewPointerObj(SWIG_as_voidptr(new shared_ptr<XdmfCurvilinearGrid>(gridCurvilinear)), SWIGTYPE_p_std__shared_ptrT_XdmfCurvilinearGrid_t, SWIG_POINTER_OWN);
        }
        else if(shared_ptr<XdmfGridCollection> gridCollection = shared_dynamic_cast<XdmfGridCollection>(obj)) {
            result = SWIG_NewPointerObj(SWIG_as_voidptr(new shared_ptr<XdmfGridCollection>(gridCollection)), SWIGTYPE_p_std__shared_ptrT_XdmfGridCollection_t, SWIG_POINTER_OWN);
        }
        else if(shared_ptr<XdmfDomain> domain = shared_dynamic_cast<XdmfDomain>(obj)) {
            result = SWIG_NewPointerObj(SWIG_as_voidptr(new shared_ptr<XdmfDomain>(domain)), SWIGTYPE_p_std__shared_ptrT_XdmfDomain_t, SWIG_POINTER_OWN);
        }
        else if(shared_ptr<XdmfGeometry> geometry = shared_dynamic_cast<XdmfGeometry>(obj)) {
            result = SWIG_NewPointerObj(SWIG_as_voidptr(new shared_ptr<XdmfGeometry>(geometry)), SWIGTYPE_p_std__shared_ptrT_XdmfGeometry_t, SWIG_POINTER_OWN);
        }
        else if(shared_ptr<XdmfGraph> graph = shared_dynamic_cast<XdmfGraph>(obj)) {
            result = SWIG_NewPointerObj(SWIG_as_voidptr(new shared_ptr<XdmfGraph>(graph)), SWIGTYPE_p_std__shared_ptrT_XdmfGraph_t, SWIG_POINTER_OWN);
        }
        else if(shared_ptr<XdmfInformation> information = shared_dynamic_cast<XdmfInformation>(obj)) {
            result = SWIG_NewPointerObj(SWIG_as_voidptr(new shared_ptr<XdmfInformation>(information)), SWIGTYPE_p_std__shared_ptrT_XdmfInformation_t, SWIG_POINTER_OWN);
        }
        else if(shared_ptr<XdmfMap> map = shared_dynamic_cast<XdmfMap>(obj)) {
            result = SWIG_NewPointerObj(SWIG_as_voidptr(new shared_ptr<XdmfMap>(map)), SWIGTYPE_p_std__shared_ptrT_XdmfMap_t, SWIG_POINTER_OWN);
        }
        else if(shared_ptr<XdmfRectilinearGrid> gridRectilinear = shared_dynamic_cast<XdmfRectilinearGrid>(obj)) {
            result = SWIG_NewPointerObj(SWIG_as_voidptr(new shared_ptr<XdmfRectilinearGrid>(gridRectilinear)), SWIGTYPE_p_std__shared_ptrT_XdmfRectilinearGrid_t, SWIG_POINTER_OWN);
        }
        else if(shared_ptr<XdmfRegularGrid> gridRegular = shared_dynamic_cast<XdmfRegularGrid>(obj)) {
            result = SWIG_NewPointerObj(SWIG_as_voidptr(new shared_ptr<XdmfRegularGrid>(gridRegular)), SWIGTYPE_p_std__shared_ptrT_XdmfRegularGrid_t, SWIG_POINTER_OWN);
        }
        else if(shared_ptr<XdmfSet> set = shared_dynamic_cast<XdmfSet>(obj)) {
            result = SWIG_NewPointerObj(SWIG_as_voidptr(new shared_ptr<XdmfSet>(set)), SWIGTYPE_p_std__shared_ptrT_XdmfSet_t, SWIG_POINTER_OWN);
        }
        else if(shared_ptr<XdmfTime> time = shared_dynamic_cast<XdmfTime>(obj)) {
            result = SWIG_NewPointerObj(SWIG_as_voidptr(new shared_ptr<XdmfTime>(time)), SWIGTYPE_p_std__shared_ptrT_XdmfTime_t, SWIG_POINTER_OWN);
        }
        else if(shared_ptr<XdmfTopology> topology = shared_dynamic_cast<XdmfTopology>(obj)) {
            result = SWIG_NewPointerObj(SWIG_as_voidptr(new shared_ptr<XdmfTopology>(topology)), SWIGTYPE_p_std__shared_ptrT_XdmfTopology_t, SWIG_POINTER_OWN);
        }
        else if(shared_ptr<XdmfUnstructuredGrid> gridUnstructured = shared_dynamic_cast<XdmfUnstructuredGrid>(obj)) {
            result = SWIG_NewPointerObj(SWIG_as_voidptr(new shared_ptr<XdmfUnstructuredGrid>(gridUnstructured)), SWIGTYPE_p_std__shared_ptrT_XdmfUnstructuredGrid_t, SWIG_POINTER_OWN);
        }
        else {
            result = SWIG_NewPointerObj(SWIG_as_voidptr(new shared_ptr<XdmfItem>(obj)), SWIGTYPE_p_std__shared_ptrT_XdmfItem_t, SWIG_POINTER_OWN);
        }
        return result;
    }
}

%typemap(out, fragment="XdmfItemCast") shared_ptr<XdmfItem> {
    $result = XdmfItemCast($1);
}

%typemap(out, fragment="XdmfItemCast") std::vector<shared_ptr<XdmfItem> > {
    $result = PyList_New($1.size());
    for(std::vector<shared_ptr<XdmfItem> >::size_type i = 0; i < $1.size(); ++i) {
        PyList_SetItem($result, i, XdmfItemCast($1[i]));
    } 
}

%extend XdmfAttributeCenter {
    bool __eq__(const XdmfAttributeCenter * attributeCenter) {
        return $self == attributeCenter;
    }
}

%extend XdmfAttributeType {
    bool __eq__(const XdmfAttributeType * attributeType) {
   	return $self == attributeType;
    }
}

%extend XdmfGeometryType {
    bool __eq__(const XdmfGeometryType * geometryType) {
   	return $self == geometryType;
    }
}

%extend XdmfGridCollectionType {
    bool __eq__(const XdmfGridCollectionType * gridCollectionType) {
   	return $self == gridCollectionType;
    }
}

%extend XdmfSetType {
    bool __eq__(const XdmfSetType * setType) {
   	return $self == setType;
    }
}

%extend XdmfTopologyType {
    bool __eq__(const XdmfTopologyType * topologyType) {
   	return $self == topologyType;
    }
}

#endif /* SWIGPYTHON */

// Shared Pointer Templates
%shared_ptr(XdmfAggregate)
%shared_ptr(XdmfAttribute)
%shared_ptr(XdmfAttributeCenter)
%shared_ptr(XdmfAttributeType)
%shared_ptr(XdmfCurvilinearGrid)
%shared_ptr(XdmfDomain)
%shared_ptr(XdmfGeometry)
%shared_ptr(XdmfGeometryType)
%shared_ptr(XdmfGraph)
%shared_ptr(XdmfGrid)
%shared_ptr(XdmfGridCollection)
%shared_ptr(XdmfGridCollectionType)
%shared_ptr(XdmfItemFactory)
%shared_ptr(XdmfMap)
%shared_ptr(XdmfReader)
%shared_ptr(XdmfRectilinearGrid)
%shared_ptr(XdmfRegularGrid)
%shared_ptr(XdmfSet)
%shared_ptr(XdmfSetType)
%shared_ptr(XdmfTime)
%shared_ptr(XdmfTopology)
%shared_ptr(XdmfTopologyType)
%shared_ptr(XdmfUnstructuredGrid)

%include Xdmf.hpp
%include XdmfGrid.hpp

%include XdmfAggregate.hpp
%include XdmfAttribute.hpp
%include XdmfAttributeCenter.hpp
%include XdmfAttributeType.hpp
%include XdmfCurvilinearGrid.hpp
%include XdmfDomain.hpp
%include XdmfGeometry.hpp
%include XdmfGeometryType.hpp
%include XdmfGraph.hpp
%include XdmfGridCollection.hpp
%include XdmfGridCollectionType.hpp
%include XdmfItemFactory.hpp
%include XdmfMap.hpp
%include XdmfReader.hpp
%include XdmfRectilinearGrid.hpp
%include XdmfRegularGrid.hpp
%include XdmfSet.hpp
%include XdmfSetType.hpp
%include XdmfTime.hpp
%include XdmfTopology.hpp
%include XdmfTopologyType.hpp
%include XdmfUnstructuredGrid.hpp
