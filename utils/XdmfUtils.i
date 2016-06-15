/*
XdmfUtilsPython.cpp:
swig -v -c++ -python -o XdmfUtilsPython.cpp XdmfUtils.i
*/

#ifdef XDMF_BUILD_DSM

%module XdmfUtils
%{
    // MPI includes
    #include <mpi.h>

    // XdmfCore Includes
    #include <XdmfArray.hpp>
    #include <XdmfArrayReference.hpp>
    #include <XdmfArrayType.hpp>
    #include <XdmfCoreItemFactory.hpp>
    #include <XdmfCoreReader.hpp>
    #include <XdmfError.hpp>
    #include <XdmfFunction.hpp>
    #include <XdmfDSMBuffer.hpp>
    #include <XdmfDSMCommMPI.hpp>
    #include <XdmfDSMItemFactory.hpp>
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

    // XdmfUtils Includes
    #include <XdmfUtils.hpp>
    #include <XdmfDiff.hpp>
    #include <XdmfExodusReader.hpp>
    #include <XdmfExodusWriter.hpp>
    #include <XdmfPartitioner.hpp>
    #include <XdmfGeometryConverter.hpp>
    #include <XdmfTopologyConverter.hpp>
%}

#else

%module XdmfUtils
%{
    // XdmfCore Includes
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

    // XdmfUtils Includes
    #include <XdmfUtils.hpp>
    #include <XdmfDiff.hpp>
    #include <XdmfExodusReader.hpp>
    #include <XdmfExodusWriter.hpp>
    #include <XdmfPartitioner.hpp>
    #include <XdmfGeometryConverter.hpp>
    #include <XdmfTopologyConverter.hpp>
%}

#endif

%import Xdmf.i

// Ignoring C Wrappers

// XdmfDiff

%ignore XdmfDiffNew();
%ignore XdmfDiffCompare(XDMFDIFF * diff, XDMFITEM * item1, XDMFITEM * item2);
%ignore XdmfDiffGetAbsoluteTolerance(XDMFDIFF * diff);
%ignore XdmfDiffSetAbsoluteTolerance(XDMFDIFF * diff, double tolerance);
%ignore XdmfDiffFree(XDMFDIFF * diff);

// XdmfExodusReader

%ignore XdmfExodusReaderNew();
%ignore XdmfExodusReaderRead(XDMFEXODUSREADER * reader, char * fileName, XDMFHEAVYDATAWRITER * heavyDataWriter);
%ignore XdmfExodusReaderFree(XDMFEXODUSREADER * reader);

// XdmfExodusWriter

%ignore XdmfExodusWriterNew();
%ignore XdmfExodusWriterWriteGrid(XDMFEXODUSWRITER * writer,
                                  char * filePath,
                                  XDMFUNSTRUCTUREDGRID * grid,
                                  int * status);
%ignore XdmfExodusWriterWriteGridCollection(XDMFEXODUSWRITER * writer,
                                            char * filePath,
                                            XDMFGRIDCOLLECTION * grid,
                                            int * status);
%ignore XdmfExodusWriterFree(XDMFEXODUSWRITER * writer);

// XdmfGeometryConverter

%ignore XdmfGeometryConverterNew();
%ignore XdmfGeometryConverterConvertToCartesian(XDMFGEOMETRYCONVERTER * converter,
                                                XDMFGEOMETRY * geometryToConvert);
%ignore XdmfGeometryConverterConvertToSpherical(XDMFGEOMETRYCONVERTER * converter,
                                                XDMFGEOMETRY * geometryToConvert);
%ignore XdmfGeometryConverterConvertToCartesianOverwrite(XDMFGEOMETRYCONVERTER * converter,
                                                         XDMFGEOMETRY * geometryToConvert);
%ignore XdmfGeometryConverterConvertToSphericalOverwrite(XDMFGEOMETRYCONVERTER * converter,
                                                         XDMFGEOMETRY * geometryToConvert);
%ignore XdmfGeometryConverterZeroOrigin(XDMFGEOMETRYCONVERTER * converter,
                                        XDMFGEOMETRY * geometryToConvert);
%ignore XdmfGeometryConverterZeroOriginOverwrite(XDMFGEOMETRYCONVERTER * converter,
                                                 XDMFGEOMETRY * geometryToConvert);
%ignore XdmfGeometryConverterFree(XDMFGEOMETRYCONVERTER * converter);

// XdmfPartitioner

%ignore XdmfPartitionerNew();
%ignore XdmfPartitionerIgnore(XDMFPARTITIONER * partitioner,
                              XDMFSET * set);
%ignore XdmfPartitionerPartitionGraph(XDMFPARTITIONER * partitioner,
                                      XDMFGRAPH * graphToPartition,
                                      unsigned int numberOfPartitions);
%ignore XdmfPartitionerPartitionUnstructuredGrid(XDMFPARTITIONER * partitioner,
                                                 XDMFUNSTRUCTUREDGRID * gridToPartition,
                                                 unsigned int numberOfPartitions,
                                                 int metisScheme,
                                                 XDMFHEAVYDATAWRITER * heavyDataWriter);
%ignore XdmfPartitionerUnpartition(XDMFPARTITIONER * partitioner,
                                   XDMFGRIDCOLLECTION * gridToUnPartition);
%ignore XdmfPartitionerFree(XDMFPARTITIONER * partitioner);

// XdmfTopologyConverter

%ignore XdmfTopologyConverterNew();
%ignore XdmfTopologyConverterConvert(XDMFTOPOLOGYCONVERTER * converter,
                                     XDMFUNSTRUCTUREDGRID * gridToConvert,
                                     int topologytype,
                                     XDMFHEAVYDATAWRITER * heavyDataWriter);
%ignore XdmfTopologyConverterGetExternalFaces(XDMFTOPOLOGYCONVERTER * converter,
                                              XDMFTOPOLOGY * convertedTopology);
%ignore XdmfTopologyConverterFree(XDMFTOPOLOGYCONVERTER * converter);

#ifdef SWIGPYTHON

%pythoncode {
    from Xdmf import *
}

#endif /* SWIGPYTHON */

// Shared Pointer Templates
%shared_ptr(XdmfDiff)
#ifdef XDMF_BUILD_EXODUS_IO
    %shared_ptr(XdmfExodusReader)
    %shared_ptr(XdmfExodusWriter)
#endif
#ifdef XDMF_BUILD_PARTITIONER
    %shared_ptr(XdmfPartitioner)
#endif
%shared_ptr(XdmfGeometryConverter)
%shared_ptr(XdmfTopologyConverter)

%include XdmfUtils.hpp
%include XdmfDiff.hpp
#ifdef XDMF_BUILD_EXODUS_IO
    %include XdmfExodusReader.hpp
    %include XdmfExodusWriter.hpp
#endif
#ifdef XDMF_BUILD_PARTITIONER
    %include XdmfPartitioner.hpp
#endif
%include XdmfGeometryConverter.hpp
%include XdmfTopologyConverter.hpp
