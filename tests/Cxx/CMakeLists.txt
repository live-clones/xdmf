# Include our test macros
include(AddTestsCxx)

# Add any dependencies that the cxx tests may need
# Note: The tests already depend on their own file
ADD_TEST_CXX_DEPENDENCIES("Xdmf")

# Add any cxx tests here:
# Note: We don't want to use a foreach loop to test the files incase we
#	have extra arguments (id: ADD_TEST_CXX(testname inputfile))
#	Read UseCxxTest.cmake for more information
# ---------------------------------------
if ("${XDMF_DSM_IS_CRAY}" STREQUAL "")
  IF (MPIEXEC_MAX_NUMPROCS GREATER 5)
    ADD_MPI_TEST_CXX(DSMFileTest.sh XdmfFileAcceptTest,XdmfFileConnectTest)
  ENDIF(MPIEXEC_MAX_NUMPROCS GREATER 5)
endif ("${XDMF_DSM_IS_CRAY}" STREQUAL "")
ADD_TEST_CXX(TestXdmfAggregate)
ADD_TEST_CXX(TestXdmfAttribute)
ADD_TEST_CXX(TestXdmfBinaryController)
ADD_TEST_CXX(TestXdmfCurvilinearGrid)
ADD_TEST_CXX(TestXdmfFunction)
ADD_TEST_CXX(TestXdmfGeometry)
ADD_TEST_CXX(TestXdmfGraph)
ADD_TEST_CXX(TestXdmfGridCollection)
ADD_TEST_CXX(TestXdmfGridController)
ADD_TEST_CXX(TestXdmfGridTemplate)
ADD_TEST_CXX(TestXdmfHDF5Hyperslab)
ADD_TEST_CXX(TestXdmfHDF5Visit)
ADD_TEST_CXX(TestXdmfMap)
ADD_TEST_CXX(TestXdmfMultiOpen)
ADD_TEST_CXX(TestXdmfMultiXPath)
ADD_TEST_CXX(TestXdmfReader)
ADD_TEST_CXX(TestXdmfRegularGrid)
ADD_TEST_CXX(TestXdmfRectilinearGrid)
ADD_TEST_CXX(XdmfPostFixCalc)
ADD_TEST_CXX(TestXdmfSet)
ADD_TEST_CXX(TestXdmfSubset)
ADD_TEST_CXX(TestXdmfTemplate)
ADD_TEST_CXX(TestXdmfTime)
if (TIFF_FOUND)
  ADD_TEST_CXX(TestXdmfTIFFReadWriteCompressed)
endif (TIFF_FOUND)
ADD_TEST_CXX(TestXdmfTopology)
ADD_TEST_CXX(TestXdmfTopologyMixed)
ADD_TEST_CXX(TestXdmfUnstructuredGrid)
ADD_TEST_CXX(TestXdmfVisitorValueCounter)
ADD_TEST_CXX(TestXdmfWriter)
ADD_TEST_CXX(TestXdmfWriterHDF5ThenXML)
ADD_TEST_CXX(TestXdmfXPath)
ADD_TEST_CXX(TestXdmfXPointerReference)
#removed due to long execution time
#ADD_TEST_CXX(HugeWriteArray)

# Add any cxx cleanup here:
# Note: We don't want to use a foreach loop to test the files incase we
#       have multiple files (ie: CLEAN_TEST_CXX(testname outputfile1 ...))
#       Read UseCxxTest.cmake for more information
# ---------------------------------------
if ("${XDMF_DSM_IS_CRAY}" STREQUAL "")
  IF (MPIEXEC_MAX_NUMPROCS STRGREATER 5)
    CLEAN_TEST_CXX(DSMFileTest.sh dsmfile.xmf dsmconnect.cfg)
  ENDIF (MPIEXEC_MAX_NUMPROCS STRGREATER 5)
endif ("${XDMF_DSM_IS_CRAY}" STREQUAL "")
CLEAN_TEST_CXX(TestXdmfAggregate
  aggregate.xmf
  aggregate.h5)
CLEAN_TEST_CXX(TestXdmfAttribute)
CLEAN_TEST_CXX(TestXdmfBinaryController
  TestXdmfBinary.xmf
  testBinary.bin)
CLEAN_TEST_CXX(TestXdmfCurvilinearGrid
  TestXdmfCurvilinearGrid1.xmf
  TestXdmfCurvilinearGrid2.xmf)
CLEAN_TEST_CXX(TestXdmfFunction
  function.xmf
  function.h5)
CLEAN_TEST_CXX(TestXdmfGeometry)
CLEAN_TEST_CXX(TestXdmfGraph
  TestXdmfGraph.xmf)
CLEAN_TEST_CXX(TestXdmfGridCollection
  TestXdmfGridCollection1.xmf
  TestXdmfGridCollection2.xmf
  TestXdmfGridCollectionHDF1.h5
  TestXdmfGridCollectionHDF1.xmf
  TestXdmfGridCollectionHDF2.xmf)
CLEAN_TEST_CXX(TestXdmfGridTemplate
  gridtemplate.xmf
  gridtemplate.h5
  gridtemplate2.xmf
  gridtemplate2.h5)
CLEAN_TEST_CXX(TestXdmfHDF5Hyperslab
  TestXdmfHDF5Hyperslab.xmf
  TestXdmfHDF5Hyperslab.h5
  TestXdmfHDF5Hyperslab2.xmf)
CLEAN_TEST_CXX(TestXdmfHDF5Visit
  TestXdmfHDF5Visit.xmf
  TestXdmfHDF5Visit.h5)
CLEAN_TEST_CXX(TestXdmfMap
  TestXdmfMap1.xmf
  TestXdmfMap2.xmf
  TestXdmfMapHDF1.xmf
  TestXdmfMapHDF1.h5
  TestXdmfMapHDF2.xmf)
CLEAN_TEST_CXX(TestXdmfMultiOpen
  setfile.h5
  attributefile.h5)
CLEAN_TEST_CXX(TestXdmfMultiXPath
  nestedOuter.xmf
  nestedInner.xmf)
CLEAN_TEST_CXX(TestXdmfReader
  TestXdmfReader1.h5
  TestXdmfReader1.xmf
  TestXdmfReader2.xmf)
CLEAN_TEST_CXX(TestXdmfRectilinearGrid
  TestXdmfRectilinearGrid1.xmf
  TestXdmfRectilinearGrid2.xmf)
CLEAN_TEST_CXX(TestXdmfRegularGrid
  TestXdmfRegularGrid1.xmf
  TestXdmfRegularGrid2.xmf)
CLEAN_TEST_CXX(XdmfPostFixCalc)
CLEAN_TEST_CXX(TestXdmfSet)
CLEAN_TEST_CXX(TestXdmfSubset
  subset.xmf
  subset.h5)
CLEAN_TEST_CXX(TestXdmfTemplate
  template.xmf
  template.h5
  template2.xmf
  template2.h5)
CLEAN_TEST_CXX(TestXdmfTime)
if (TIFF_FOUND)
  CLEAN_TEST_CXX(TestXdmfTIFFReadWriteCompressed
    compressedtiffoutput.xmf
    compressedoutput.tif
    compressedstripoutput.tif
    compresseddirectories.tif)
endif (TIFF_FOUND)
CLEAN_TEST_CXX(TestXdmfTopology)
CLEAN_TEST_CXX(TestXdmfTopologyMixed
  TestXdmfTopologyMixed1.xmf
  TestXdmfTopologyMixed2.xmf)
CLEAN_TEST_CXX(TestXdmfUnstructuredGrid)
CLEAN_TEST_CXX(TestXdmfVisitorValueCounter)
CLEAN_TEST_CXX(TestXdmfWriter
  output.h5
  output.xmf)
CLEAN_TEST_CXX(TestXdmfWriterHDF5ThenXML)
CLEAN_TEST_CXX(TestXdmfXPath
  XdmfXPath1.xmf
  XdmfXPath2.xmf)
CLEAN_TEST_CXX(TestXdmfXPointerReference
  duplicateXpointer.xmf
  duplicateXpointer.h5
  duplicateXpointer2.xmf
  duplicateXpointer2.h5)
#removed due to long execution time
#CLEAN_TEST_CXX(HugeWriteArray
#  arraydata1.h5
#  arraydata2.h5
#  arraydata3.h5
#  arraydata.h5
#  arraydata.xmf)
