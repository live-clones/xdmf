/*
XdmfCorePython.cpp:
swig -v -c++ -python -o XdmfCorePython.cpp XdmfCore.i
*/

#ifdef XDMF_BUILD_TIFF

%module XdmfCore
%{
    #include <XdmfArray.hpp>
    #include <XdmfArrayReference.hpp>
    #include <XdmfArrayType.hpp>
    #include <XdmfCore.hpp>
    #include <XdmfCoreItemFactory.hpp>
    #include <XdmfCoreReader.hpp>
    #include <XdmfError.hpp>
    #include <XdmfFunction.hpp>
    #include <XdmfHeavyDataController.hpp>
    #include <XdmfHeavyDataWriter.hpp>
    #include <XdmfHDF5Controller.hpp>
    #include <XdmfHDF5Writer.hpp>
    #include <XdmfInformation.hpp>
    #include <XdmfItem.hpp>
    #include <XdmfItemProperty.hpp>
    #include <XdmfSharedPtr.hpp>
    #include <XdmfSparseMatrix.hpp>
    #include <XdmfSubset.hpp>
    #include <XdmfSystemUtils.hpp>
    #include <XdmfTIFFController.hpp>
    #include <XdmfVersion.hpp>
    #include <XdmfVisitor.hpp>
    #include <XdmfWriter.hpp>

    #include <ProjectVersion.hpp>
%}

#else

%module XdmfCore
%{
    #include <XdmfArray.hpp>
    #include <XdmfArrayReference.hpp>
    #include <XdmfArrayType.hpp>
    #include <XdmfCore.hpp>
    #include <XdmfCoreItemFactory.hpp>
    #include <XdmfCoreReader.hpp>
    #include <XdmfError.hpp>
    #include <XdmfFunction.hpp>
    #include <XdmfHeavyDataController.hpp>
    #include <XdmfHeavyDataWriter.hpp>
    #include <XdmfHDF5Controller.hpp>
    #include <XdmfHDF5Writer.hpp>
    #include <XdmfInformation.hpp>
    #include <XdmfItem.hpp>
    #include <XdmfItemProperty.hpp>
    #include <XdmfSharedPtr.hpp>
    #include <XdmfSparseMatrix.hpp>
    #include <XdmfSubset.hpp>
    #include <XdmfSystemUtils.hpp>
    #include <XdmfVersion.hpp>
    #include <XdmfVisitor.hpp>
    #include <XdmfWriter.hpp>

    #include <ProjectVersion.hpp>
%}

#endif

// Ignoring C Wrappers

// XdmfItem

%ignore XdmfItemAccept(XDMFITEM * item, XDMFVISITOR * visitor, int * status);
%ignore XdmfItemFree(void * item);
%ignore XdmfItemGetInformation(XDMFITEM * item, unsigned int index);
%ignore XdmfItemGetInformationByKey(XDMFITEM * item, char * key);
%ignore XdmfItemGetNumberInformations(XDMFITEM * item);
%ignore XdmfItemInsertInformation(XDMFITEM * item, XDMFINFORMATION * information, int passControl);
%ignore XdmfItemRemoveInformation(XDMFITEM * item, unsigned int index);
%ignore XdmfItemRemoveInformationByKey(XDMFITEM * item, char * key);
%ignore XdmfItemGetItemTag(XDMFITEM * item);

// XdmfArray

%ignore XdmfArrayNew();
%ignore XdmfArrayClear(XDMFARRAY * array);
%ignore XdmfArrayErase(XDMFARRAY * array, unsigned int index);
%ignore XdmfArrayGetArrayType(XDMFARRAY * array, int * status);
%ignore XdmfArrayGetCapacity(XDMFARRAY * array);
%ignore XdmfArrayGetDimensions(XDMFARRAY * array);
%ignore XdmfArrayGetDimensionsString(XDMFARRAY * array);
%ignore XdmfArrayGetHeavyDataController(XDMFARRAY * array, unsigned int index);
%ignore XdmfArrayGetReadMode(XDMFARRAY * array, int * status);
%ignore XdmfArrayGetName(XDMFARRAY * array);
%ignore XdmfArrayGetNumberDimensions(XDMFARRAY * array);
%ignore XdmfArrayGetNumberHeavyDataControllers(XDMFARRAY * array);
%ignore XdmfArrayGetSize(XDMFARRAY * array);
%ignore XdmfArrayGetReference(XDMFARRAY * array);
%ignore XdmfArrayGetValue(XDMFARRAY * array, unsigned int index, int arrayType, int * status);
%ignore XdmfArrayGetValues(XDMFARRAY * array, unsigned int startIndex, int arrayType, unsigned int numValues, unsigned int arrayStride, unsigned int valueStride, int * status);
%ignore XdmfArrayGetValuesInternal(XDMFARRAY * array);
%ignore XdmfArrayGetValuesString(XDMFARRAY * array);
%ignore XdmfArrayInitialize(XDMFARRAY * array, int * dims, int numDims, int arrayType, int * status);
%ignore XdmfArrayInsertDataFromPointer(XDMFARRAY * array, void * values, int arrayType, unsigned int startIndex, unsigned int numVals, unsigned int arrayStride, unsigned int valueStride, int * status);
%ignore XdmfArrayInsertDataFromXdmfArray(XDMFARRAY * array, XDMFARRAY * valArray, int * arrayStarts, int * valueStarts, int * arrayCounts, int * valueCounts, int * arrayStrides, int * valueStrides, int * status);
%ignore XdmfArrayInsertHeavyDataController(XDMFARRAY * array, XDMFHEAVYDATACONTROLLER * controller, int passControl);
%ignore XdmfArrayInsertValue(XDMFARRAY * array, unsigned int index, void * value, int arrayType, int * status);
%ignore XdmfArrayIsInitialized(XDMFARRAY * array);
%ignore XdmfArrayPushBack(XDMFARRAY * array, void * value, int arrayType, int * status);
%ignore XdmfArrayRead(XDMFARRAY * array, int * status);
%ignore XdmfArrayReadController(XDMFARRAY * array, int * status);
%ignore XdmfArrayReadReference(XDMFARRAY * array, int * status);
%ignore XdmfArrayRelease(XDMFARRAY * array);
%ignore XdmfArrayRemoveHeavyDataController(XDMFARRAY * array, unsigned int index);
%ignore XdmfArrayReserve(XDMFARRAY * array, int size);
%ignore XdmfArrayResize(XDMFARRAY * array, int * dims, int numDims, int arrayType, int * status);
%ignore XdmfArraySetReadMode(XDMFARRAY * array, int readMode, int * status);
%ignore XdmfArraySetReference(XDMFARRAY * array, XDMFARRAYREFERENCE * reference, int passControl);
%ignore XdmfArraySetName(XDMFARRAY * array, char * name, int * status);
%ignore XdmfArraySetValuesInternal(XDMFARRAY * array, void * pointer, unsigned int numValues, int arrayType, int transferOwnership, int * status);
%ignore XdmfArraySwapWithXdmfArray(XDMFARRAY * array, XDMFARRAY * swapArray);
%ignore XdmfArraySwapWithArray(XDMFARRAY * array, void ** pointer, int numValues, int arrayType, int * status);
// XdmfArray inherited from XdmfItem
%ignore XdmfArrayAccept(XDMFARRAY * item, XDMFVISITOR * visitor, int * status);
%ignore XdmfArrayFree(void * item);
%ignore XdmfArrayGetInformation(XDMFARRAY * item, unsigned int index);
%ignore XdmfArrayGetInformationByKey(XDMFARRAY * item, char * key);
%ignore XdmfArrayGetNumberInformations(XDMFARRAY * item);
%ignore XdmfArrayInsertInformation(XDMFARRAY * item, XDMFINFORMATION * information, int passControl);
%ignore XdmfArrayRemoveInformation(XDMFARRAY * item, unsigned int index);
%ignore XdmfArrayRemoveInformationByKey(XDMFARRAY * item, char * key);
%ignore XdmfArrayGetItemTag(XDMFARRAY * item);

// XdmfArrayReference

%ignore XdmfArrayReferenceGetConstructedType(XDMFARRAYREFERENCE * arrayReference);
%ignore XdmfArrayReferenceRead(XDMFARRAYREFERENCE * arrayReference, int * status);
%ignore XdmfArrayReferenceSetConstructedProperties(XDMFARRAYREFERENCE * arrayReference, void * referenceobject);
%ignore XdmfArrayReferenceSetConstructedType(XDMFARRAYREFERENCE * arrayReference, char * newType);
// XdmfArrayReference inherited from XdmfItem
%ignore XdmfArrayReferenceAccept(XDMFARRAYREFERENCE * item, XDMFVISITOR * visitor, int * status);
%ignore XdmfArrayReferenceFree(void * item);
%ignore XdmfArrayReferenceGetInformation(XDMFARRAYREFERENCE * item, unsigned int index);
%ignore XdmfArrayReferenceGetInformationByKey(XDMFARRAYREFERENCE * item, char * key);
%ignore XdmfArrayReferenceGetNumberInformations(XDMFARRAYREFERENCE * item);
%ignore XdmfArrayReferenceInsertInformation(XDMFARRAYREFERENCE * item, XDMFINFORMATION * information, int passControl);
%ignore XdmfArrayReferenceRemoveInformation(XDMFARRAYREFERENCE * item, unsigned int index);
%ignore XdmfArrayReferenceRemoveInformationByKey(XDMFARRAYREFERENCE * item, char * key);
%ignore XdmfArrayReferenceGetItemTag(XDMFARRAYREFERENCE * item);

// XdmfArrayType

%ignore XdmfArrayTypeInt8();
%ignore XdmfArrayTypeInt16();
%ignore XdmfArrayTypeInt32();
%ignore XdmfArrayTypeInt64();
%ignore XdmfArrayTypeFloat32();
%ignore XdmfArrayTypeFloat64();
%ignore XdmfArrayTypeUInt8();
%ignore XdmfArrayTypeUInt16();
%ignore XdmfArrayTypeUInt32();
%ignore XdmfArrayTypeUInt64();
%ignore XdmfArrayTypeComparePrecision(int type1, int type2, oint * status);
%ignore XdmfArrayTypeGetElementSize(int type, int * status);
%ignore XdmfArrayTypeGetIsSigned(int type, int * status);
%ignore XdmfArrayTypeGetName(int type, int * status);

// XdmfCoreReader

%ignore XdmfCoreReaderRead(XDMFCOREREADER * reader, char * filePath, int * status);

// XdmfError

%ignore XdmfErrorSetCErrorsAreFatal(int status);
%ignore XdmfErrorSetLevelLimit(int level, int * status);
%ignore XdmfErrorSetSuppressionLevel(int level, int * status);
%ignore XdmfErrorGetCErrorsAreFatal();
%ignore XdmfErrorGetLevelLimit();
%ignore XdmfErrorGetSuppressionLevel();

// XdmfFunction

%ignore XdmfFunctionNew();
%ignore XdmfFunctionNewInit(char * newExpression,  char ** keys, XDMFARRAY ** values, int numVariables);
%ignore XdmfFunctionAddFunction(char * name, XDMFARRAY *(*functionref)(XDMFARRAY **, unsigned int), int * status);
%ignore XdmfFunctionAddOperation(char newoperator, XDMFARRAY *(*operationref)(XDMFARRAY *, XDMFARRAY *), int priority, int * status);
%ignore XdmfFunctionAverage(XDMFARRAY ** values, int numValues);
%ignore XdmfFunctionChunk(XDMFARRAY * val1, XDMFARRAY * val2, int * status);
%ignore XdmfFunctionEvaluateExpression(char * expression, char ** keys, XDMFARRAY ** values, int numVariables, int * status);
%ignore XdmfFunctionEvaluateOperation(XDMFARRAY * val1, XDMFARRAY * val2, char operation, int * status);
%ignore XdmfFunctionEvaluateFunction(XDMFARRAY ** valueVector, int numValues, char * functionName, int * status);
%ignore XdmfFunctionGetExpression(XDMFFUNCTION * function);
%ignore XdmfFunctionGetNumberVariables(XDMFFUNCTION * function);
%ignore XdmfFunctionGetOperationPriority(char operation);
%ignore XdmfFunctionGetSupportedOperations();
%ignore XdmfFunctionGetSupportedFunctions();
%ignore XdmfFunctionGetNumberSupportedFunctions();
%ignore XdmfFunctionGetValidDigitChars();
%ignore XdmfFunctionGetValidVariableChars();
%ignore XdmfFunctionGetVariable(XDMFFUNCTION * function, char * key);
%ignore XdmfFunctionGetVariableList(XDMFFUNCTION * function);
%ignore XdmfFunctionInterlace(XDMFARRAY * val1, XDMFARRAY * val2, int * status);
%ignore XdmfFunctionInsertVariable(XDMFFUNCTION * function, char * key, XDMFARRAY * value, int passControl);
%ignore XdmfFunctionRemoveVariable(XDMFFUNCTION * function, char * key);
%ignore XdmfFunctionSetExpression(XDMFFUNCTION * function, char * newExpression, int * status);
%ignore XdmfFunctionSum(XDMFARRAY ** values, int numValues);
// XdmfFunction inherited from XdmfItem
%ignore XdmfFunctionAccept(XDMFFUNCTION * item, XDMFVISITOR * visitor, int * status);
%ignore XdmfFunctionFree(void * item);
%ignore XdmfFunctionGetInformation(XDMFFUNCTION * item, unsigned int index);
%ignore XdmfFunctionGetInformationByKey(XDMFFUNCTION * item, char * key);
%ignore XdmfFunctionGetNumberInformations(XDMFFUNCTION * item);
%ignore XdmfFunctionInsertInformation(XDMFFUNCTION * item, XDMFINFORMATION * information, int passControl);
%ignore XdmfFunctionRemoveInformation(XDMFFUNCTION * item, unsigned int index);
%ignore XdmfFunctionRemoveInformationByKey(XDMFFUNCTION * item, char * key);
%ignore XdmfFunctionGetItemTag(XDMFFUNCTION * item);
// XdmfFunction inherited from XdmfArrayReference
%ignore XdmfFunctionGetConstructedType(XDMFFUNCTION * arrayReference);
%ignore XdmfFunctionRead(XDMFFUNCTION * arrayReference, int * status);
%ignore XdmfFunctionSetConstructedProperties(XDMFFUNCTION * arrayReference, void * referenceobject);
%ignore XdmfFunctionSetConstructedType(XDMFFUNCTION * arrayReference, char * newType);

// XdmfHDF5Controller

%ignore XdmfHDF5ControllerNew(char * hdf5FilePath, char * dataSetPath, int type, unsigned int * start, unsigned int * stride, unsigned int * dimensions, unsigned int * dataspaceDimensions, unsigned int numDims, int * status);
%ignore XdmfHDF5ControllerGetDataSetPath(XDMFHDF5CONTROLLER * controller);
// XdmfHDF5Controller inherited from XdmfHeavyDataController
%ignore XdmfHDF5ControllerFree(XDMFHDF5CONTROLLER * item);
%ignore XdmfHDF5ControllerGetDataspaceDimensions(XDMFHDF5CONTROLLER * controller);
%ignore XdmfHDF5ControllerGetDimensions(XDMFHDF5CONTROLLER * controller);
%ignore XdmfHDF5ControllerGetFilePath(XDMFHDF5CONTROLLER * controller);
%ignore XdmfHDF5ControllerGetName(XDMFHDF5CONTROLLER * controller);
%ignore XdmfHDF5ControllerGetNumberDimensions(XDMFHDF5CONTROLLER * controller);
%ignore XdmfHDF5ControllerGetSize(XDMFHDF5CONTROLLER * controller);
%ignore XdmfHDF5ControllerGetStart(XDMFHDF5CONTROLLER * controller);
%ignore XdmfHDF5ControllerGetStride(XDMFHDF5CONTROLLER * controller);
%ignore XdmfHDF5ControllerSetArrayOffset(XDMFHDF5CONTROLLER * controller, unsigned int newOffset);
%ignore XdmfHDF5ControllerGetArrayOffset(XDMFHDF5CONTROLLER * controller);
%ignore XdmfHDF5ControllerGetType(XDMFHDF5CONTROLLER * controller, int * status);
%ignore XdmfHDF5ControllerRead(XDMFHDF5CONTROLLER * controller, void * array, int * status);

// XdmfHDF5Writer

%ignore XdmfHDF5WriterNew(char * fileName, int clobberFile);
%ignore XdmfHDF5WriterCloseFile(XDMFHDF5WRITER * writer, int * status);
%ignore XdmfHDF5WriterGetChunkSize(XDMFHDF5WRITER * writer, int * status);
%ignore XdmfHDF5WriterOpenFile(XDMFHDF5WRITER * writer, int * status);
%ignore XdmfHDF5WriterSetChunkSize(XDMFHDF5WRITER * writer, unsigned int chunkSize, int * status);
// XdmfHDF5Writer inherited from XdmfHeavyDataWriter
%ignore XdmfHDF5WriterFree(XDMFHDF5WRITER * item);
%ignore XdmfHDF5WriterGetAllowSetSplitting(XDMFHDF5WRITER * writer);
%ignore XdmfHDF5WriterGetFileIndex(XDMFHDF5WRITER * writer);
%ignore XdmfHDF5WriterGetFileOverhead(XDMFHDF5WRITER * writer);
%ignore XdmfHDF5WriterGetFilePath(XDMFHDF5WRITER * writer);
%ignore XdmfHDF5WriterGetFileSizeLimit(XDMFHDF5WRITER * writer);
%ignore XdmfHDF5WriterGetMode(XDMFHDF5WRITER * writer);
%ignore XdmfHDF5WriterGetReleaseData(XDMFHDF5WRITER * writer);
%ignore XdmfHDF5WriterSetAllowSetSplitting(XDMFHDF5WRITER * writer, int newAllow);
%ignore XdmfHDF5WriterSetFileIndex(XDMFHDF5WRITER * writer, int newIndex);
%ignore XdmfHDF5WriterSetFileSizeLimit(XDMFHDF5WRITER * writer, int newSize);
%ignore XdmfHDF5WriterSetMode(XDMFHDF5WRITER * writer, int mode, int * status);
%ignore XdmfHDF5WriterSetReleaseData(XDMFHDF5WRITER * writer, int releaseData);

// XdmfHeavyDataController

%ignore XdmfHeavyDataControllerFree(XDMFHEAVYDATACONTROLLER * item);
%ignore XdmfHeavyDataControllerGetDataspaceDimensions(XDMFHEAVYDATACONTROLLER * controller);
%ignore XdmfHeavyDataControllerGetDimensions(XDMFHEAVYDATACONTROLLER * controller);
%ignore XdmfHeavyDataControllerGetFilePath(XDMFHEAVYDATACONTROLLER * controller);
%ignore XdmfHeavyDataControllerGetName(XDMFHEAVYDATACONTROLLER * controller);
%ignore XdmfHeavyDataControllerGetNumberDimensions(XDMFHEAVYDATACONTROLLER * controller);
%ignore XdmfHeavyDataControllerGetSize(XDMFHEAVYDATACONTROLLER * controller);
%ignore XdmfHeavyDataControllerGetStart(XDMFHEAVYDATACONTROLLER * controller);
%ignore XdmfHeavyDataControllerGetStride(XDMFHEAVYDATACONTROLLER * controller);
%ignore XdmfHeavyDataControllerSetArrayOffset(XDMFHEAVYDATACONTROLLER * controller, unsigned int newOffset);
%ignore XdmfHeavyDataControllerGetArrayOffset(XDMFHEAVYDATACONTROLLER * controller);
%ignore XdmfHeavyDataControllerGetType(XDMFHEAVYDATACONTROLLER * controller, int * status);
%ignore XdmfHeavyDataControllerRead(XDMFHEAVYDATACONTROLLER * controller, void * array, int * status);

// XdmfHeavyDataWriter

%ignore XdmfHeavyDataWriterFree(XDMFHEAVYDATAWRITER * item);
%ignore XdmfHeavyDataWriterGetAllowSetSplitting(XDMFHEAVYDATAWRITER * writer);
%ignore XdmfHeavyDataWriterGetFileIndex(XDMFHEAVYDATAWRITER * writer);
%ignore XdmfHeavyDataWriterGetFileOverhead(XDMFHEAVYDATAWRITER * writer);
%ignore XdmfHeavyDataWriterGetFilePath(XDMFHEAVYDATAWRITER * writer);
%ignore XdmfHeavyDataWriterGetFileSizeLimit(XDMFHEAVYDATAWRITER * writer);
%ignore XdmfHeavyDataWriterGetMode(XDMFHEAVYDATAWRITER * writer);
%ignore XdmfHeavyDataWriterGetReleaseData(XDMFHEAVYDATAWRITER * writer);
%ignore XdmfHeavyDataWriterSetAllowSetSplitting(XDMFHEAVYDATAWRITER * writer, int newAllow);
%ignore XdmfHeavyDataWriterSetFileIndex(XDMFHEAVYDATAWRITER * writer, int newIndex);
%ignore XdmfHeavyDataWriterSetFileSizeLimit(XDMFHEAVYDATAWRITER * writer, int newSize);
%ignore XdmfHeavyDataWriterSetMode(XDMFHEAVYDATAWRITER * writer, int mode, int * status);
%ignore XdmfHeavyDataWriterSetReleaseData(XDMFHEAVYDATAWRITER * writer, int releaseData);

// XdmfInformation

%ignore XdmfInformationNew(char * key, char * value);
%ignore XdmfInformationGetArray(XDMFINFORMATION * information, unsigned int index);
%ignore XdmfInformationGetArrayByName(XDMFINFORMATION * information, char * name);
%ignore XdmfInformationGetKey(XDMFINFORMATION * information);
%ignore XdmfInformationGetNumberArrays(XDMFINFORMATION * information);
%ignore XdmfInformationGetValue(XDMFINFORMATION * information);
%ignore XdmfInformationInsertArray(XDMFINFORMATION * information, XDMFARRAY * array, int transferOwnership);
%ignore XdmfInformationRemoveArray(XDMFINFORMATION * information, unsigned int index);
%ignore XdmfInformationRemoveArrayByName(XDMFINFORMATION * information, char * name);
%ignore XdmfInformationSetKey(XDMFINFORMATION * information, char * key, int * status);
%ignore XdmfInformationSetValue(XDMFINFORMATION * information, char * value, int * status);
// XdmfInformation inherited from XdmfItem
%ignore XdmfInformationAccept(XDMFINFORMATION * item, XDMFVISITOR * visitor, int * status);
%ignore XdmfInformationFree(void * item);
%ignore XdmfInformationGetInformation(XDMFINFORMATION * item, unsigned int index);
%ignore XdmfInformationGetInformationByKey(XDMFINFORMATION * item, char * key);
%ignore XdmfInformationGetNumberInformations(XDMFINFORMATION * item);
%ignore XdmfInformationInsertInformation(XDMFINFORMATION * item, XDMFINFORMATION * information, int passControl);
%ignore XdmfInformationRemoveInformation(XDMFINFORMATION * item, unsigned int index);
%ignore XdmfInformationRemoveInformationByKey(XDMFINFORMATION * item, char * key);
%ignore XdmfInformationGetItemTag(XDMFINFORMATION * item);

// XdmfSparseMatrix

%ignore XdmfSparseMatrixNew(unsigned int numberRows, unsigned int numberColumns);
%ignore XdmfSparseMatrixGetColumnIndex(XDMFSPARSEMATRIX * matrix, int * status);
%ignore XdmfSparseMatrixGetName(XDMFSPARSEMATRIX * matrix);
%ignore XdmfSparseMatrixGetNumberColumns(XDMFSPARSEMATRIX * matrix);
%ignore XdmfSparseMatrixGetNumberRows(XDMFSPARSEMATRIX * matrix);
%ignore XdmfSparseMatrixGetRowPointer(XDMFSPARSEMATRIX * matrix, int * status);
%ignore XdmfSparseMatrixGetValues(XDMFSPARSEMATRIX * matrix, int * status);
%ignore XdmfSparseMatrixGetValuesString(XDMFSPARSEMATRIX * matrix, int * status);
%ignore XdmfSparseMatrixSetColumnIndex(XDMFSPARSEMATRIX * matrix,
                                       XDMFARRAY * columnIndex,
                                       int passControl,
                                       int * status);
%ignore XdmfSparseMatrixSetName(XDMFSPARSEMATRIX * matrix, char * name, int * status);
%ignore XdmfSparseMatrixSetRowPointer(XDMFSPARSEMATRIX * matrix,
                                      XDMFARRAY * rowPointer,
                                      int passControl,
                                      int * status);
%ignore XdmfSparseMatrixSetValues(XDMFSPARSEMATRIX * matrix,
                                  XDMFARRAY * values,
                                  int passControl,
                                  int * status);
// XdmfSparseMatrix inherited from XdmfItem
%ignore XdmfSparseMatrixAccept(XDMFSPARSEMATRIX * item, XDMFVISITOR * visitor, int * status);
%ignore XdmfSparseMatrixFree(void * item);
%ignore XdmfSparseMatrixGetInformation(XDMFSPARSEMATRIX * item, unsigned int index);
%ignore XdmfSparseMatrixGetInformationByKey(XDMFSPARSEMATRIX * item, char * key);
%ignore XdmfSparseMatrixGetNumberInformations(XDMFSPARSEMATRIX * item);
%ignore XdmfSparseMatrixInsertInformation(XDMFSPARSEMATRIX * item, XDMFINFORMATION * information, int passControl);
%ignore XdmfSparseMatrixRemoveInformation(XDMFSPARSEMATRIX * item, unsigned int index);
%ignore XdmfSparseMatrixRemoveInformationByKey(XDMFSPARSEMATRIX * item, char * key);
%ignore XdmfSparseMatrixGetItemTag(XDMFSPARSEMATRIX * item);

// XdmfSubset

%ignore XdmfSubsetNew(void * referenceArray,
                      unsigned int * start,
                      unsigned int * stride,
                      unsigned int * dimensions,
                      unsigned int numDims,
                      int passControl,
                      int * status);
%ignore XdmfSubsetGetDimensions(XDMFSUBSET * subset);
%ignore XdmfSubsetGetNumberDimensions(XDMFSUBSET * subset);
%ignore XdmfSubsetGetReferenceArray(XDMFSUBSET * subset);
%ignore XdmfSubsetGetSize(XDMFSUBSET * subset);
%ignore XdmfSubsetGetStart(XDMFSUBSET * subset);
%ignore XdmfSubsetGetStride(XDMFSUBSET * subset);
%ignore XdmfSubsetSetDimensions(XDMFSUBSET * subset,
                                unsigned int * newDimensions,
                                unsigned int numDims,
                                int * status);
%ignore XdmfSubsetSetReferenceArray(XDMFSUBSET * subset,
                                    XDMFARRAY * referenceArray,
                                    int passControl);
%ignore XdmfSubsetSetStart(XDMFSUBSET * subset,
                           unsigned int * newStarts,
                           unsigned int numDims,
                           int * status);
%ignore XdmfSubsetSetStride(XDMFSUBSET * subset,
                            unsigned int * newStrides,
                            unsigned int numDims,
                            int * status);
// XdmfSubset inherited from XdmfItem
%ignore XdmfSubsetAccept(XDMFSUBSET * item, XDMFVISITOR * visitor, int * status);
%ignore XdmfSubsetFree(void * item);
%ignore XdmfSubsetGetInformation(XDMFSUBSET * item, unsigned int index);
%ignore XdmfSubsetGetInformationByKey(XDMFSUBSET * item, char * key);
%ignore XdmfSubsetGetNumberInformations(XDMFSUBSET * item);
%ignore XdmfSubsetInsertInformation(XDMFSUBSET * item, XDMFINFORMATION * information, int passControl);
%ignore XdmfSubsetRemoveInformation(XDMFSUBSET * item, unsigned int index);
%ignore XdmfSubsetRemoveInformationByKey(XDMFSUBSET * item, char * key);
%ignore XdmfSubsetGetItemTag(XDMFSUBSET * item);
// XdmfSubset inherited from XdmfArrayReference
%ignore XdmfSubsetGetConstructedType(XDMFSUBSET * arrayReference);
%ignore XdmfSubsetRead(XDMFSUBSET * arrayReference, int * status);
%ignore XdmfSubsetSetConstructedProperties(XDMFSUBSET * arrayReference, void * referenceobject);
%ignore XdmfSubsetSetConstructedType(XDMFSUBSET * arrayReference, char * newType);

// XdmfWriter

%ignore XdmfWriterNew(char * fileName);
%ignore XdmfWriterNewSpecifyHeavyDataWriter(char * fileName, XDMFHEAVYDATAWRITER * heavyDataWriter);
%ignore XdmfWriterFree(XDMFWRITER * item);
%ignore XdmfWriterGetFilePath(XDMFWRITER * writer, int * status);
%ignore XdmfWriterGetHeavyDataWriter(XDMFWRITER * writer, int * status);
%ignore XdmfWriterGetLightDataLimit(XDMFWRITER * writer, int * status);
%ignore XdmfWriterGetMode(XDMFWRITER * writer, int * status);
%ignore XdmfWriterGetWriteXPaths(XDMFWRITER * writer, int * status);
%ignore XdmfWriterGetXPathParse(XDMFWRITER * writer, int * status);
%ignore XdmfWriterSetHeavyDataWriter(XDMFWRITER * writer,
                                     XDMFHEAVYDATAWRITER * heavyDataWriter,
                                     int transferOwnership,
                                     int * status);
%ignore XdmfWriterSetLightDataLimit(XDMFWRITER * writer, unsigned int numValues, int * status);
%ignore XdmfWriterSetMode(XDMFWRITER * writer, int mode, int * status);
%ignore XdmfWriterSetWriteXPaths(XDMFWRITER * writer, int writeXPaths, int * status);
%ignore XdmfWriterSetXPathParse(XDMFWRITER * writer, int xPathParse, int * status);

#ifdef SWIGJAVA

// Ignore const overloaded methods
%ignore XdmfArray::getHeavyDataController() const;
%ignore XdmfArray::getValuesInternal() const;
%ignore XdmfItem::getInformation(const unsigned int) const;
%ignore XdmfItem::getInformation(const std::string &) const;
%ignore XdmfWriter::getHeavyDataWriter() const;
%ignore XdmfInformation::getArray(unsigned int const) const;
%ignore XdmfInformation::getArray(std::string const &) const;

// Ignore ItemTags
%ignore XdmfArray::ItemTag;
%ignore XdmfInformation::ItemTag;

// Define equality operators
%extend XdmfItem {

    bool equals(boost::shared_ptr<XdmfItem> item) {
        if (item == NULL) {
            return false;
        }
        return self == item.get();
    }

    bool IsEqual(boost::shared_ptr<XdmfItem> item) {
        if (item == NULL) {
            return false;
        }
        return self == item.get();
    }
};

%extend XdmfItemProperty {

    bool equals(boost::shared_ptr<XdmfItemProperty> itemProperty) {
        if (itemProperty == NULL) {
            return false;
        }
        return self == itemProperty.get();
    }

    bool IsEqual(boost::shared_ptr<XdmfItemProperty> itemProperty) {
        if (itemProperty == NULL) {
            return false;
        }
        return self == itemProperty.get();
    }

};

%typemap(javacode) XdmfArray %{
    public void insertValuesAsInt8(int index, char[] values) {
        for(int i = 0; i < values.length; i++)
            this.insertValueAsInt8(index+i, values[i]);
    }

    public void insertValuesAsInt16(int index, short[] values) {
        for(int i = 0; i < values.length; i++)
            this.insertValueAsInt16(index+i, values[i]);
    }

    public void insertValuesAsInt32(int index, int[] values) {
        for(int i = 0; i < values.length; i++)
            this.insertValueAsInt32(index+i, values[i]);
    }

    public void insertValuesAsFloat32(int index, float[] values) {
        for(int i = 0; i < values.length; i++)
            this.insertValueAsFloat32(index+i, values[i]);
    }

    public void insertValuesAsFloat64(int index, double[] values) {
        for(int i = 0; i < values.length; i++)
            this.insertValueAsFloat64(index+i, values[i]);
    }
%}

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("XdmfCoreJava");
        }
        catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load for" +
                               "XdmfCoreJava\n" + e);
            System.exit(1);
        }
    }
%}

#endif /* SWIGJAVA */

#ifdef SWIGPYTHON

%ignore XdmfArray::insert(const unsigned int startIndex, const T * const valuesPointer, const unsigned int numValues, const unsigned int arrayStride = 1, const unsigned int valuesStride = 1);
%ignore XdmfArray::pushBack(const T & value);

/*This converts XdmfErrors to Python RuntimeErrors*/
%exception
{
        try
        {
                $action
        }
        catch (XdmfError e)
        {
                PyErr_SetString(PyExc_RuntimeError, e.what());
                return NULL;
        }
}

/*This causes it to avoid throwing a warning about overloaded functions.
  We are doing this intentionally so suppressing the warning makes sense.*/
#pragma SWIG nowarn=509

%extend XdmfItem {
    bool __eq__(const XdmfItem * item) {
        return $self == item;
    }
};

%extend XdmfItemProperty {
    bool __eq__(const XdmfItemProperty * item) {
        return $self == item;
    }
};

%extend XdmfVisitor {
    bool __eq__(const XdmfVisitor * item) {
        return $self == item;
    }
};

%extend XdmfHeavyDataWriter {
    bool __eq__(const XdmfHeavyDataWriter * item) {
        return $self == item;
    }
};

%extend XdmfHeavyDataController {
    bool __eq__(const XdmfHeavyDataController * item) {
        return $self == item;
    }

    static shared_ptr<XdmfHDF5Controller> XdmfHDF5ControllerCast(PyObject * obj)
    {
      void * resultPointer = 0;
      swig_type_info * returnType = SWIG_TypeQuery("_p_boost__shared_ptrT_XdmfHeavyDataController_t");
      SWIG_ConvertPtr(obj, &resultPointer, returnType, 0);
      shared_ptr<XdmfHeavyDataController> * returnControllerPointer =
        reinterpret_cast<shared_ptr<XdmfHeavyDataController> *>(resultPointer);
      shared_ptr<XdmfHeavyDataController> returnController = returnControllerPointer[0];
      if (shared_ptr<XdmfHDF5Controller> returnHDF5Controller = shared_dynamic_cast<XdmfHDF5Controller>(returnController)) {
        return returnHDF5Controller;
      }
      else {
        XdmfError::message(XdmfError::FATAL, "Error: Attempting to cast a non HDF5 Controller to HDF5");
        return shared_ptr<XdmfHDF5Controller>();
      }
    }
};

%extend XdmfCoreReader {
    bool __eq__(const XdmfCoreReader * item) {
        return $self == item;
    }
};

%extend XdmfWriter {
    bool __eq__(const XdmfWriter * item) {
        return $self == item;
    }
};

// Provide accessors from python lists to XdmfArrays
%extend XdmfArray {

    PyObject * getBuffer() {
        void *vp = $self->getValuesInternal();
        Py_ssize_t sz =
            $self->getSize() * $self->getArrayType()->getElementSize();
        PyObject * c = PyBuffer_FromMemory(vp, sz);
        return(c);
    }

    %pythoncode {
        def getNumpyArray(self):
            h5ctl = self.getHeavyDataController()
            if h5ctl == None :
                try :
                    from numpy import frombuffer as ___frombuffer
                except :
                    return None
                buf = self.getBuffer()
                aType = self.getArrayType()
                if aType == XdmfArrayType.Int8() :
                    return(___frombuffer(buf, 'int8'))
                if aType == XdmfArrayType.Int16() :
                    return(___frombuffer(buf, 'int16'))
                if aType == XdmfArrayType.Int32() :
                    return(___frombuffer(buf, 'int32'))
                if aType == XdmfArrayType.Int64() :
                    return(___frombuffer(buf, 'int64'))
                if aType == XdmfArrayType.Float32() :
                    return(___frombuffer(buf, 'float32'))
                if aType == XdmfArrayType.Float64() :
                    return(___frombuffer(buf, 'float64'))
                if aType == XdmfArrayType.UInt8() :
                    return(___frombuffer(buf, 'uint8'))
                if aType == XdmfArrayType.UInt16() :
                    return(___frombuffer(buf, 'uint16'))
                if aType == XdmfArrayType.UInt32() :
                    return(___frombuffer(buf, 'uint32'))
                if aType == XdmfArrayType.UInt64() :
                    return(___frombuffer(buf, 'uint64'))
                return None
            else :
                if  h5ctl.getName() == "HDF":
                  controller = XdmfHeavyDataController.XdmfHDF5ControllerCast(h5ctl)
                  h5FileName = controller.getFilePath()
                  h5DataSetName = controller.getDataSetPath()
                  if (h5FileName == None) | (h5DataSetName == None) :
                      return None
                  try :
                      from h5py import File as ___File
                      from numpy import array as ___array
                      f = ___File(h5FileName, 'r')
                      if h5DataSetName in f.keys() :
                          return(___array(f[h5DataSetName]))
                  except :
                      pass
                  return None
                else:
                  return None

    };

    void getValues(unsigned int startIndex, PyObject * list, unsigned int numValues = 1, unsigned int arrayStride = 1, unsigned int valuesStride = 1)
    {
      Py_ssize_t size = PyList_Size(list);
      PyObject * placeholderVal;
      if (valuesStride * numValues > size)
      {
        if ($self->XdmfArray::getArrayType() == XdmfArrayType::Int8())
        {
          placeholderVal = PyLong_FromLong((char)0);
        }
        else if ($self->XdmfArray::getArrayType() == XdmfArrayType::Int16())
        {
          placeholderVal = PyLong_FromLong((short)0);
        }
        else if ($self->XdmfArray::getArrayType() == XdmfArrayType::Int32())
        {
          placeholderVal = PyLong_FromLong((int)0);
        }
        else if ($self->XdmfArray::getArrayType() == XdmfArrayType::Int64())
        {
          placeholderVal = PyLong_FromLong((long)0);
        }
        else if ($self->XdmfArray::getArrayType() == XdmfArrayType::Float32())
        {
          placeholderVal = PyFloat_FromDouble((float)0);
        }
        else if ($self->XdmfArray::getArrayType() == XdmfArrayType::Float64())
        {
          placeholderVal = PyFloat_FromDouble((double)0);
        }
        else if ($self->XdmfArray::getArrayType() == XdmfArrayType::UInt8())
        {
          placeholderVal = PyLong_FromUnsignedLong((unsigned char)0);
        }
        else if ($self->XdmfArray::getArrayType() == XdmfArrayType::UInt16())
        {
          placeholderVal = PyLong_FromUnsignedLong((unsigned short)0);
        }
        else if ($self->XdmfArray::getArrayType() == XdmfArrayType::UInt32())
        {
          placeholderVal = PyLong_FromUnsignedLong((unsigned int)0);
        }
        else if ($self->XdmfArray::getArrayType() == XdmfArrayType::UInt64())
        {
          placeholderVal = PyLong_FromUnsignedLong((uint64_t)0);
        }
        else if ($self->XdmfArray::getArrayType() == XdmfArrayType::String())
        {
          placeholderVal = PyString_FromString("");
        }
      }
      for (unsigned int i = 0; i < numValues; ++i)
      {
        unsigned int index = startIndex + (i * arrayStride);
        unsigned int insertIndex = i * valuesStride;
        PyObject * insertedVal;
        if ($self->XdmfArray::getArrayType() == XdmfArrayType::Int8())
        {
          insertedVal = PyLong_FromLong($self->XdmfArray::getValue<char>(index));
        }
        else if ($self->XdmfArray::getArrayType() == XdmfArrayType::Int16())
        {
          insertedVal = PyLong_FromLong($self->XdmfArray::getValue<short>(index));
        }
        else if ($self->XdmfArray::getArrayType() == XdmfArrayType::Int32())
        {
          insertedVal = PyLong_FromLong($self->XdmfArray::getValue<int>(index));
        }
        else if ($self->XdmfArray::getArrayType() == XdmfArrayType::Int64())
        {
          insertedVal = PyLong_FromLong($self->XdmfArray::getValue<long>(index));
        }
        else if ($self->XdmfArray::getArrayType() == XdmfArrayType::Float32())
        {
          insertedVal = PyFloat_FromDouble($self->XdmfArray::getValue<float>(index));
        }
        else if ($self->XdmfArray::getArrayType() == XdmfArrayType::Float64())
        {
          insertedVal = PyFloat_FromDouble($self->XdmfArray::getValue<double>(index));
        }
        else if ($self->XdmfArray::getArrayType() == XdmfArrayType::UInt8())
        {
          insertedVal = PyLong_FromUnsignedLong($self->XdmfArray::getValue<unsigned char>(index));
        }
        else if ($self->XdmfArray::getArrayType() == XdmfArrayType::UInt16())
        {
          insertedVal = PyLong_FromUnsignedLong($self->XdmfArray::getValue<unsigned short>(index));
        }
        else if ($self->XdmfArray::getArrayType() == XdmfArrayType::UInt32())
        {
          insertedVal = PyLong_FromUnsignedLong($self->XdmfArray::getValue<unsigned int>(index));
        }
        else if ($self->XdmfArray::getArrayType() == XdmfArrayType::UInt64())
        {
          insertedVal = PyLong_FromUnsignedLong($self->XdmfArray::getValue<uint64_t>(index));
        }
        else if ($self->XdmfArray::getArrayType() == XdmfArrayType::String())
        {
          insertedVal = PyString_FromString($self->XdmfArray::getValue<std::string>(index).c_str());
        }
        if (insertIndex < size)
        {
          PyList_SetItem(list, insertIndex, insertedVal);
        }
        else
        {
          for (unsigned int padding = size; padding < insertIndex; ++padding)
          {
            PyList_Append(list, placeholderVal);
            ++size;
          }
          PyList_Append(list, insertedVal);
          ++size;
        }
      }
    }

    void insertAsInt8(int startIndex, PyObject * list, int listStartIndex = 0, int numValues = -1, int arrayStride = 1, int listStride = 1) {
        Py_ssize_t size;
        if (numValues <= 0) {
          size = PyList_Size(list);
        }
        else {
          size = numValues;
        }
        for(Py_ssize_t i = 0; i < size; ++i) {
            if (listStartIndex + (i * listStride) >= PyList_Size(list)) {
              $self->insert(i+startIndex, (char) 0);
            }
            else {
              $self->insert((i * arrayStride) + startIndex, (char)PyLong_AsLong(PyList_GetItem(list, listStartIndex + (i * listStride))));
            }
        }
    }

    void insertAsInt16(int startIndex, PyObject * list, int listStartIndex = 0, int numValues = -1, int arrayStride = 1, int listStride = 1) {
        Py_ssize_t size;
        if (numValues <= 0) {
          size = PyList_Size(list);
        }
        else {
          size = numValues;
        }
        for(Py_ssize_t i = 0; i < size; ++i) {
            if (listStartIndex + (i * listStride) >= PyList_Size(list)) {
              $self->insert(i+startIndex, (short) 0);
            }
            else {
              $self->insert((i * arrayStride) + startIndex, (short)PyLong_AsLong(PyList_GetItem(list, listStartIndex + (i * listStride))));
            }
        }
    }

    void insertAsInt32(int startIndex, PyObject * list, int listStartIndex = 0, int numValues = -1, int arrayStride = 1, int listStride = 1) {
        Py_ssize_t size;
        if (numValues <= 0) {
          size = PyList_Size(list);
        }
        else {
          size = numValues;
        }
        for(Py_ssize_t i = 0; i < size; ++i) {
            if (listStartIndex + (i * listStride) >= PyList_Size(list)) {
              $self->insert(i+startIndex, (int) 0);
            }
            else {
              $self->insert((i * arrayStride) + startIndex, (int)PyLong_AsLong(PyList_GetItem(list, listStartIndex + (i * listStride))));
            }
        }
    }

    void insertAsInt64(int startIndex, PyObject * list, int listStartIndex = 0, int numValues = -1, int arrayStride = 1, int listStride = 1) {
        Py_ssize_t size;
        if (numValues <= 0) {
          size = PyList_Size(list);
        }
        else {
          size = numValues;
        }
        for(Py_ssize_t i = 0; i < size; ++i) {
            if (listStartIndex + (i * listStride) >= PyList_Size(list)) {
              $self->insert(i+startIndex, (long) 0);
            }
            else {
              $self->insert((i * arrayStride) + startIndex, PyLong_AsLong(PyList_GetItem(list, listStartIndex + (i * listStride))));
            }
        }
    }

    void insertAsFloat32(int startIndex, PyObject * list, int listStartIndex = 0, int numValues = -1, int arrayStride = 1, int listStride = 1) {
        Py_ssize_t size;
        if (numValues <= 0) {
          size = PyList_Size(list);
        }
        else {
          size = numValues;
        }
        for(Py_ssize_t i = 0; i < size; ++i) {
            if (listStartIndex + (i * listStride) >= PyList_Size(list)) {
              $self->insert(i+startIndex, (float) 0);
            }
            else {
              $self->insert((i * arrayStride) + startIndex, (float)PyFloat_AsDouble(PyList_GetItem(list, listStartIndex + (i * listStride))));
            }
        }
    }

    void insertAsFloat64(int startIndex, PyObject * list, int listStartIndex = 0, int numValues = -1, int arrayStride = 1, int listStride = 1) {
        Py_ssize_t size;
        if (numValues <= 0) {
          size = PyList_Size(list);
        }
        else {
          size = numValues;
        }
        for(Py_ssize_t i = 0; i < size; ++i) {
            if (listStartIndex + (i * listStride) >= PyList_Size(list)) {
              $self->insert(i+startIndex, (double) 0);
            }
            else {
              $self->insert((i * arrayStride) + startIndex, PyFloat_AsDouble(PyList_GetItem(list, listStartIndex + (i * listStride))));
            }
        }
    }

    void insertAsUInt8(int startIndex, PyObject * list, int listStartIndex = 0, int numValues = -1, int arrayStride = 1, int listStride = 1) {
        Py_ssize_t size;
        if (numValues <= 0) {
          size = PyList_Size(list);
        }
        else {
          size = numValues;
        }
        for(Py_ssize_t i = 0; i < size; ++i) {
            if (listStartIndex + (i * listStride) >= PyList_Size(list)) {
              $self->insert(i+startIndex, (unsigned char) 0);
            }
            else {
              $self->insert((i * arrayStride) + startIndex, (unsigned char)(PyLong_AsUnsignedLong(PyList_GetItem(list, listStartIndex + (i * listStride)))));
            }
        }
    }

    void insertAsUInt16(int startIndex, PyObject * list, int listStartIndex = 0, int numValues = -1, int arrayStride = 1, int listStride = 1) {
        Py_ssize_t size;
        if (numValues <= 0) {
          size = PyList_Size(list);
        }
        else {
          size = numValues;
        }
        for(Py_ssize_t i = 0; i < size; ++i) {
            if (listStartIndex + (i * listStride) >= PyList_Size(list)) {
              $self->insert(i+startIndex, (unsigned short) 0);
            }
            else {
              $self->insert((i * arrayStride) + startIndex, (unsigned short)(PyLong_AsUnsignedLong(PyList_GetItem(list, listStartIndex + (i * listStride)))));
            }
        }
    }

    void insertAsUInt32(int startIndex, PyObject * list, int listStartIndex = 0, int numValues = -1, int arrayStride = 1, int listStride = 1) {
        Py_ssize_t size;
        if (numValues <= 0) {
          size = PyList_Size(list);
        }
        else {
          size = numValues;
        }
        for(Py_ssize_t i = 0; i < size; ++i) {
            if (listStartIndex + (i * listStride) >= PyList_Size(list)) {
              $self->insert(i+startIndex, (unsigned int) 0);
            }
            else {
              $self->insert((i * arrayStride) + startIndex, (unsigned int)(PyLong_AsUnsignedLong(PyList_GetItem(list, listStartIndex + (i * listStride)))));
            }
        }
    }

    void insertAsUInt64(int startIndex, PyObject * list, int listStartIndex = 0, int numValues = -1, int arrayStride = 1, int listStride = 1) {
        Py_ssize_t size;
        if (numValues <= 0) {
          size = PyList_Size(list);
        }
        else {
          size = numValues;
        }
        for(Py_ssize_t i = 0; i < size; ++i) {
            if (listStartIndex + (i * listStride) >= PyList_Size(list)) {
              $self->insert(i+startIndex, (uint64_t) 0);
            }
            else {
              $self->insert((i * arrayStride) + startIndex, (uint64_t)(PyLong_AsUnsignedLong(PyList_GetItem(list, listStartIndex + (i * listStride)))));
            }
        }
    }

    void insertAsString(int startIndex, PyObject * list, int listStartIndex = 0, int numValues = -1, int arrayStride = 1, int listStride = 1) {
        Py_ssize_t size;
        if (numValues <= 0) {
          size = PyList_Size(list);
        }
        else {
          size = numValues;
        }
        for(Py_ssize_t i = 0; i < size; ++i) {
            if (listStartIndex + (i * listStride) >= PyList_Size(list)) {
              $self->insert<std::string>(i+startIndex, "");
            }
            else {
              $self->insert<std::string>((i * arrayStride) + startIndex, PyString_AsString(PyList_GetItem(list, listStartIndex + (i * listStride))));
            }
        }
    }

    static shared_ptr<XdmfArray> XdmfArrayPtr(PyObject * obj)
    {
      void * resultPointer = 0;
      swig_type_info * returnType = SWIG_TypeQuery("_p_boost__shared_ptrT_XdmfArray_t");
      SWIG_ConvertPtr(obj, &resultPointer, returnType, 0);
      shared_ptr<XdmfArray> * returnArrayPointer = reinterpret_cast<shared_ptr<XdmfArray> *>(resultPointer);
      shared_ptr<XdmfArray> returnArray = returnArrayPointer[0];
      return returnArray;
    }

    void
    pushBackAsInt8(const char & value)
    {
      $self->XdmfArray::pushBack(value);
    }

    void
    pushBackAsInt16(const short & value)
    {
      $self->XdmfArray::pushBack(value);
    }

    void
    pushBackAsInt32(const int & value)
    {
      $self->XdmfArray::pushBack(value);
    }

    void
    pushBackAsInt64(const long & value)
    {
      $self->XdmfArray::pushBack(value);
    }

    void
    pushBackAsUInt8(const unsigned char & value)
    {
      $self->XdmfArray::pushBack(value);
    }

    void
    pushBackAsUInt16(const unsigned short & value)
    {
      $self->XdmfArray::pushBack(value);
    }

    void
    pushBackAsUInt32(const unsigned int & value)
    {
      $self->XdmfArray::pushBack(value);
    }

    void
    pushBackAsUInt64(const uint64_t & value)
    {
      $self->XdmfArray::pushBack(value);
    }

    void
    pushBackAsFloat32(const float & value)
    {
      $self->XdmfArray::pushBack(value);
    }

    void
    pushBackAsFloat64(const double & value)
    {
      $self->XdmfArray::pushBack(value);
    }

    void
    pushBackAsString(const std::string & value)
    {
      $self->XdmfArray::pushBack(value);
    }

    void
    insertValueAsInt8(const unsigned int startIndex, const char * const valuesPointer, const unsigned int numValues = 1, const unsigned int arrayStride = 1, const unsigned int valuesStride = 1)
    {
      $self->XdmfArray::insert(startIndex, valuesPointer, numValues, arrayStride, valuesStride);
    }

    void
    insertValueAsInt8(const unsigned int startIndex, const char value)
    {
      $self->XdmfArray::insert(startIndex, value);
    }

    void
    insertValueAsInt16(const unsigned int startIndex, const short * const valuesPointer, const unsigned int numValues = 1, const unsigned int arrayStride = 1, const unsigned int valuesStride = 1)
    {
      $self->XdmfArray::insert(startIndex, valuesPointer, numValues, arrayStride, valuesStride);
    }

    void
    insertValueAsInt16(const unsigned int startIndex, const short value)
    {
      $self->XdmfArray::insert(startIndex, value);
    }

    void
    insertValueAsInt32(const unsigned int startIndex, const int * const valuesPointer, const unsigned int numValues = 1, const unsigned int arrayStride = 1, const unsigned int valuesStride = 1)
    {
      $self->XdmfArray::insert(startIndex, valuesPointer, numValues, arrayStride, valuesStride);
    }

    void
    insertValueAsInt32(const unsigned int startIndex, const int value)
    {
      $self->XdmfArray::insert(startIndex, value);
    }

    void
    insertValueAsInt64(const unsigned int startIndex, const long * const valuesPointer, const unsigned int numValues = 1, const unsigned int arrayStride = 1, const unsigned int valuesStride = 1)
    {
      $self->XdmfArray::insert(startIndex, valuesPointer, numValues, arrayStride, valuesStride);
    }

    void
    insertValueAsInt64(const unsigned int startIndex, const long value)
    {
      $self->XdmfArray::insert(startIndex, value);
    }

    void
    insertValueAsUInt8(const unsigned int startIndex, const unsigned char * const valuesPointer, const unsigned int numValues = 1, const unsigned int arrayStride = 1, const unsigned int valuesStride = 1)
    {
      $self->XdmfArray::insert(startIndex, valuesPointer, numValues, arrayStride, valuesStride);
    }

    void
    insertValueAsUInt8(const unsigned int startIndex, const unsigned char value)
    {
      $self->XdmfArray::insert(startIndex, value);
    }

    void
    insertValueAsUInt16(const unsigned int startIndex, const unsigned short * const valuesPointer, const unsigned int numValues = 1, const unsigned int arrayStride = 1, const unsigned int valuesStride = 1)
    {
      $self->XdmfArray::insert(startIndex, valuesPointer, numValues, arrayStride, valuesStride);
    }

    void
    insertValueAsUInt16(const unsigned int startIndex, const unsigned short value)
    {
      $self->XdmfArray::insert(startIndex, value);
    }

    void
    insertValueAsUInt32(const unsigned int startIndex, const unsigned int * const valuesPointer, const unsigned int numValues = 1, const unsigned int arrayStride = 1, const unsigned int valuesStride = 1)
    {
      $self->XdmfArray::insert(startIndex, (unsigned int *)(valuesPointer), numValues, arrayStride, valuesStride);
    }

    void
    insertValueAsUInt32(const unsigned int startIndex, const unsigned int value)
    {
      $self->XdmfArray::insert(startIndex, value);
    }

    void
    insertValueAsUInt64(const unsigned int startIndex, const uint64_t * const valuesPointer, const unsigned int numValues = 1, const unsigned int arrayStride = 1, const unsigned int valuesStride = 1)
    {
      $self->XdmfArray::insert(startIndex, (uint64_t *)(valuesPointer), numValues, arrayStride, valuesStride);
    }

    void
    insertValueAsUInt64(const unsigned int startIndex, const uint64_t value)
    {
      $self->XdmfArray::insert(startIndex, value);
    }

    void
    insertValueAsFloat32(const unsigned int startIndex, const float * const valuesPointer, const unsigned int numValues = 1, const unsigned int arrayStride = 1, const unsigned int valuesStride = 1)
    {
      $self->XdmfArray::insert(startIndex, (float *)(&valuesPointer), numValues, arrayStride, valuesStride);
    }

    void
    insertValueAsFloat32(const unsigned int startIndex, const float value)
    {
      $self->XdmfArray::insert(startIndex, value);
    }

    void
    insertValueAsFloat64(const unsigned int startIndex, const double * const valuesPointer, const unsigned int numValues = 1, const unsigned int arrayStride = 1, const unsigned int valuesStride = 1)
    {
      $self->XdmfArray::insert(startIndex, (double *)(valuesPointer), numValues, arrayStride, valuesStride);
    }

    void
    insertValueAsFloat64(const unsigned int startIndex, const double value)
    {
      $self->XdmfArray::insert(startIndex, value);
    }

    void
    insertValueAsString(const unsigned int startIndex, const char ** const valuesPointer, const unsigned int numValues = 1, const unsigned int arrayStride = 1, const unsigned int valuesStride = 1)
    {
      std::string * tempPointer = new std::string[numValues]();
      for (unsigned int i = 0; i < numValues; ++i)
      {
        tempPointer[i] = std::string(valuesPointer[i]);
      }
      $self->XdmfArray::insert(startIndex, (std::string *)(tempPointer), numValues, arrayStride, valuesStride);
    }

    void
    insertValueAsString(const unsigned int startIndex, const char * value)
    {
      $self->XdmfArray::insert(startIndex, std::string(value));
    }
};

/*This causes it to avoid throwing a warning for redefining fuctions that are defined for XdmfArray.
  I do this because doing so was intentional.*/
#pragma SWIG nowarn=302
/*Warning 325 is due to having nested classes in XdmfFunction that are not accessible when wrapped.
  As of right now, this is acceptable behavior. So, the warning is suppressed*/
#pragma SWIG nowarn=325

%extend XdmfFunction {

%{
    /*trying to transfer python functions*/
    /*note, accessing private members is impossible from swig.*/


    /*Class to wrap python functions to be compatible with the XdmfFunction code.
      This version has an execute that takes a vector of XdmfArrays as parameters,
      so it is used for functions.*/
    class PythonFunction : public XdmfFunction::XdmfFunctionInternal {
      public:
        static shared_ptr<PythonFunction>
        New(PyObject * functionref)
        {
          shared_ptr<PythonFunction> p (new PythonFunction(functionref));
          return p;
        }

        ~PythonFunction()
        {
        }

        virtual shared_ptr<XdmfArray> execute(std::vector<shared_ptr<XdmfArray> > valueVector)
        {
          swig_type_info * paramType = SWIG_TypeQuery("_p_std__vectorT_boost__shared_ptrT_XdmfArray_t_std__allocatorT_boost__shared_ptrT_XdmfArray_t_t_t");
          PyObject * pyVector = SWIG_NewPointerObj(static_cast<void*>(& valueVector), paramType, SWIG_POINTER_NEW);
          PyObject * args = PyTuple_New(1);
          /* In this case you could also cast a pointer to the vector
             into the PyObject * type, but that doesn't work for all types*/
          PyTuple_SetItem(args, 0, pyVector);
          PyObject * resultObject = PyObject_CallObject(mInternalFunction, args);
          void * resultPointer = 0;
          swig_type_info * returnType = SWIG_TypeQuery("_p_boost__shared_ptrT_XdmfArray_t");
          SWIG_ConvertPtr(resultObject, &resultPointer, returnType, 0);
          shared_ptr<XdmfArray> * returnArrayPointer = reinterpret_cast<shared_ptr<XdmfArray> *>(resultPointer);
          shared_ptr<XdmfArray> returnArray = returnArrayPointer[0];
          return returnArray;
        }
      private:
        PythonFunction(PyObject * functionref)
        {
          if (PyCallable_Check(functionref) == 1) {
            mInternalFunction = functionref;
          }
          else {
            XdmfError::message(XdmfError::FATAL,
                               "Error: Function is not callable");
          }
        }

        PyObject * mInternalFunction;
    };

    /*Class to wrap python functions to be compatible with the XdmfFunction Operation code.
      This version has an execute that takes two XdmfArrays as parameters,
      so it is used for binary operators.*/
    class PythonOperation : public XdmfFunction::XdmfOperationInternal {
      public:
        static shared_ptr<PythonOperation>
        New(PyObject * operationref)
        {
          shared_ptr<PythonOperation> p (new PythonOperation(operationref));
          return p;
        }

        ~PythonOperation()
        {
        }

        virtual shared_ptr<XdmfArray> execute(shared_ptr<XdmfArray> val1, shared_ptr<XdmfArray> val2)
        {
          swig_type_info * paramType = SWIG_TypeQuery("_p_boost__shared_ptrT_XdmfArray_t");
          PyObject * pyVal1 = SWIG_NewPointerObj(static_cast<void*>(& val1), paramType, SWIG_POINTER_NEW);
          PyObject * pyVal2 = SWIG_NewPointerObj(static_cast<void*>(& val2), paramType, SWIG_POINTER_NEW);
          PyObject * args = PyTuple_New(2);
          PyTuple_SetItem(args, 0, pyVal1);
          PyTuple_SetItem(args, 1, pyVal2);
          PyObject * resultObject = PyObject_CallObject(mInternalOperation, args);
          void * resultPointer = 0;
          swig_type_info * returnType = SWIG_TypeQuery("_p_boost__shared_ptrT_XdmfArray_t");
          SWIG_ConvertPtr(resultObject, &resultPointer, returnType, 0);
          shared_ptr<XdmfArray> * returnArrayPointer = reinterpret_cast<shared_ptr<XdmfArray> *>(resultPointer);
          shared_ptr<XdmfArray> returnArray = returnArrayPointer[0];
          return returnArray;
        }
      private:
        PythonOperation(PyObject * operationref)
        {
          if (PyCallable_Check(operationref) == 1) {
            mInternalOperation = operationref;
          }
          else {
            XdmfError::message(XdmfError::FATAL,
                               "Error: Operation is not callable");
          }
        }

        PyObject * mInternalOperation;
    };
%}

    static int addFunction(std::string newName, PyObject * functionref)
    {
      shared_ptr<PythonFunction> newFunction = PythonFunction::New(functionref);
      return XdmfFunction::addFunction(newName, newFunction);
    }

    static int addOperation(char newName, PyObject * calcref, int priority)
    {
      shared_ptr<PythonOperation> newOperation = PythonOperation::New(calcref);
      return XdmfFunction::addOperation(newName, newOperation, priority);
    }
};

#endif /* SWIGPYTHON */

%include boost_shared_ptr.i

%inline
%{
    #include <boost/shared_ptr.hpp>
    using namespace boost;
%}

%include std_string.i
%include std_vector.i
%include std_map.i

%shared_ptr(Loki::BaseVisitor)
%shared_ptr(Loki::BaseVisitable<void>)
%shared_ptr(Loki::Visitor<XdmfItem>)
%shared_ptr(Loki::Visitor<XdmfArray>)

%include loki/Visitor.h

// Shared Pointer Templates
%shared_ptr(XdmfArray)
%shared_ptr(XdmfArrayReference)
%shared_ptr(XdmfArrayType)
%shared_ptr(XdmfCoreItemFactory)
%shared_ptr(XdmfCoreReader)
%shared_ptr(XdmfFunction)
%shared_ptr(XdmfHDF5Controller)
%shared_ptr(XdmfHDF5Writer)
%shared_ptr(XdmfHeavyDataController)
%shared_ptr(XdmfHeavyDataWriter)
%shared_ptr(XdmfInformation)
%shared_ptr(XdmfItem)
%shared_ptr(XdmfItemProperty)
%shared_ptr(XdmfSparseMatrix)
%shared_ptr(XdmfSubset)
#ifdef XDMF_BUILD_TIFF
  %shared_ptr(XdmfTIFFController)
#endif
%shared_ptr(XdmfVisitor)
%shared_ptr(XdmfWriter)

%shared_ptr(std::vector<int>)

// Abstract Base Classes
%template(BaseVisitable) Loki::BaseVisitable<void>;
%template() Loki::Visitor<XdmfArray>;
%template() Loki::Visitor<XdmfItem>;

%include XdmfCore.hpp
%include XdmfError.hpp
%include XdmfItem.hpp
%include XdmfItemProperty.hpp
%include XdmfSparseMatrix.hpp
%include XdmfSystemUtils.hpp
%include XdmfVisitor.hpp
%include XdmfArrayReference.hpp
%include XdmfFunction.hpp
%include XdmfHeavyDataController.hpp
%include XdmfHeavyDataWriter.hpp
%include XdmfSubset.hpp

%include XdmfCoreItemFactory.hpp
%include XdmfCoreReader.hpp
%include XdmfInformation.hpp
%include XdmfHDF5Controller.hpp
%include XdmfHDF5Writer.hpp
#ifdef XDMF_BUILD_TIFF
  %include XdmfTIFFController.hpp
#endif
%include XdmfWriter.hpp

%include CMake/VersionSuite/ProjectVersion.hpp
%include XdmfVersion.hpp

%include XdmfArray.hpp
%include XdmfArrayType.hpp

#ifdef SWIGPYTHON

%pythoncode {
    XdmfVersion = _XdmfCore.cvar.XdmfVersion
};

#endif /* SWIGPYTHON */

%template(getValueAsInt8) XdmfArray::getValue<char>;
%template(getValueAsInt16) XdmfArray::getValue<short>;
%template(getValueAsInt32) XdmfArray::getValue<int>;
%template(getValueAsInt64) XdmfArray::getValue<long>;
%template(getValueAsFloat32) XdmfArray::getValue<float>;
%template(getValueAsFloat64) XdmfArray::getValue<double>;
%template(getValueAsUInt8) XdmfArray::getValue<unsigned char>;
%template(getValueAsUInt16) XdmfArray::getValue<unsigned short>;
%template(getValueAsUInt32) XdmfArray::getValue<unsigned int>;
%template(getValueAsUInt64) XdmfArray::getValue<uint64_t>;
%template(getValueAsString) XdmfArray::getValue<std::string>;

%template(initializeAsInt8) XdmfArray::initialize<char>;
%template(initializeAsInt16) XdmfArray::initialize<short>;
%template(initializeAsInt32) XdmfArray::initialize<int>;
%template(initializeAsInt64) XdmfArray::initialize<long>;
%template(initializeAsFloat32) XdmfArray::initialize<float>;
%template(initializeAsFloat64) XdmfArray::initialize<double>;
%template(initializeAsUInt8) XdmfArray::initialize<unsigned char>;
%template(initializeAsUInt16) XdmfArray::initialize<unsigned short>;
%template(initializeAsUInt32) XdmfArray::initialize<unsigned int>;
%template(initializeAsUInt64) XdmfArray::initialize<uint64_t>;
%template(initializeAsString) XdmfArray::initialize<std::string>;

%template(insertValueAsInt8) XdmfArray::insert<char>;
%template(insertValueAsInt16) XdmfArray::insert<short>;
%template(insertValueAsInt32) XdmfArray::insert<int>;
%template(insertValueAsInt64) XdmfArray::insert<long>;
%template(insertValueAsFloat32) XdmfArray::insert<float>;
%template(insertValueAsFloat64) XdmfArray::insert<double>;
%template(insertValueAsUInt8) XdmfArray::insert<unsigned char>;
%template(insertValueAsUInt16) XdmfArray::insert<unsigned short>;
%template(insertValueAsUInt32) XdmfArray::insert<unsigned int>;
%template(insertValueAsUInt64) XdmfArray::insert<uint64_t>;
%template(insertValueAsString) XdmfArray::insert<std::string>;

%template(pushBackAsInt8) XdmfArray::pushBack<char>;
%template(pushBackAsInt16) XdmfArray::pushBack<short>;
%template(pushBackAsInt32) XdmfArray::pushBack<int>;
%template(pushBackAsInt64) XdmfArray::pushBack<long>;
%template(pushBackAsFloat32) XdmfArray::pushBack<float>;
%template(pushBackAsFloat64) XdmfArray::pushBack<double>;
%template(pushBackAsUInt8) XdmfArray::pushBack<unsigned char>;
%template(pushBackAsUInt16) XdmfArray::pushBack<unsigned short>;
%template(pushBackAsUInt32) XdmfArray::pushBack<unsigned int>;
%template(pushBackAsUInt64) XdmfArray::pushBack<uint64_t>;
%template(pushBackAsString) XdmfArray::pushBack<std::string>;

%template(resizeAsInt8) XdmfArray::resize<char>;
%template(resizeAsInt16) XdmfArray::resize<short>;
%template(resizeAsInt32) XdmfArray::resize<int>;
%template(resizeAsInt64) XdmfArray::resize<long>;
%template(resizeAsFloat32) XdmfArray::resize<float>;
%template(resizeAsFloat64) XdmfArray::resize<double>;
%template(resizeAsUInt8) XdmfArray::resize<unsigned char>;
%template(resizeAsUInt16) XdmfArray::resize<unsigned short>;
%template(resizeAsUInt32) XdmfArray::resize<unsigned int>;
%template(resizeAsUInt64) XdmfArray::resize<uint64_t>;
%template(resizeAsString) XdmfArray::resize<std::string>;

%template(UInt8Vector) std::vector<unsigned char>;
%template(UInt16Vector) std::vector<unsigned short>;
%template(UInt32Vector) std::vector<unsigned int>;
%template(UInt64Vector) std::vector<uint64_t>;
%template(Int8Vector) std::vector<char>;
%template(Int16Vector) std::vector<short>;
%template(Int32Vector) std::vector<int>;
%template(Int64Vector) std::vector<long>;
%template(Float32Vector) std::vector<float>;
%template(Float64Vector) std::vector<double>;
%template(StringVector) std::vector<std::string>;
%template(ItemVector) std::vector<boost::shared_ptr<XdmfItem> >;
%template(HeavyControllerVector) std::vector<boost::shared_ptr<XdmfHeavyDataController> >;
%template(ArrayMap) std::map<std::string, boost::shared_ptr<XdmfArray> >;
%template(StringMap) std::map<std::string, std::string>;
%template(DSMStructreVector) std::vector<std::pair<std::string, unsigned int> >;
%template(DSMApplicationPair) std::pair<std::string, unsigned int>;
