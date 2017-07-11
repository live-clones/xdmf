# Include our test macros
include(AddTestsC)

# Add any dependencies that the cxx core tests may need
# Note: The tests already depend on their own file
ADD_TEST_C_DEPENDENCIES("Xdmf")

# Add any ldpath directories that the cxx tests may need
ADD_TEST_C_LDPATH("${CMAKE_BINARY_DIR}")

# Add any path directoreis that the Cxx tests may need
ADD_TEST_C_PATH("${CMAKE_BINARY_DIR}")
ADD_TEST_C_PATH("${XDMF_BINARIES}")

# Add any cxx tests here:
# Note: We don't want to use a foreach loop to test the files incase we
#       have extra arguments (ie: ADD_TEST_C(testname inputfile))
#       Read UseCxxTest.cmake for more information
# ---------------------------------------
ADD_TEST_C(CTestXdmfWriter)
ADD_TEST_C(CTestXdmfAggregate)
ADD_TEST_C(CTestXdmfAttribute)
ADD_TEST_C(CTestXdmfBinaryController)
ADD_TEST_C(CTestXdmfDomain)
if (TIFF_FOUND)
  ADD_TEST_C(CTestXdmfTIFFReadWriteCompressed)
endif ()
ADD_TEST_C(CTestXdmfTopology)
ADD_TEST_C(CTestXdmfGeometry)
ADD_TEST_C(CTestXdmfGraph)
ADD_TEST_C(CTestXdmfGridCollection)
ADD_TEST_C(CTestXdmfGridController)
ADD_TEST_C(CTestXdmfCurvilinearGrid)
ADD_TEST_C(CTestXdmfRectilinearGrid)
ADD_TEST_C(CTestXdmfRegularGrid)
ADD_TEST_C(CTestXdmfUnstructuredGrid)
ADD_TEST_C(CTestXdmfMap)
ADD_TEST_C(CTestXdmfSet)

# Add any cxx cleanup here:
# Note: We don't want to use a foreach loop to test the files incase we
#       have multiple files (ie: CLEAN_TEST_C(testname outputfile1 ...))
#       Read UseCxxTest.cmake for more information
# ---------------------------------------
CLEAN_TEST_C(CTestXdmfWriter
  testfile.xmf
  testfile.h5
  secondtestfile.xmf
  secondtestfile.h5
  subsetfile.xmf
  subsetfile.h5
  functionfile.xmf
  functionfile.h5)
CLEAN_TEST_C(CTestXdmfAggregate,
  aggregate.xmf
  aggregate.h5)
CLEAN_TEST_C(CTestXdmfAttribute
  attributefile.xmf
  attributefile.h5)
CLEAN_TEST_C(CTestXdmfBinaryController
  binary.bin)
CLEAN_TEST_C(CTestXdmfDomain
  domainfile.xmf
  domainfile.h5)
if (TIFF_FOUND)
  CLEAN_TEST_C(CTestXdmfTIFFReadWriteCompressed
    compressedstripoutput.tif)
endif ()
CLEAN_TEST_C(CTestXdmfTopology
  topofile.xmf
  topofile.h5)
CLEAN_TEST_C(CTestXdmfGeometry
  geofile.xmf
  geofile.h5)
CLEAN_TEST_C(CTestXdmfGraph
  matrixfile.xmf
  matrixfile.h5)
CLEAN_TEST_C(CTestXdmfGridCollection
  colfile.xmf
  colfile.h5)
CLEAN_TEST_C(CTestXdmfGridController
  gridControllerReference.xmf
  gridController.xmf)
CLEAN_TEST_C(CTestXdmfCurvilinearGrid
  curvfile.xmf
  curvfile.h5)
CLEAN_TEST_C(CTestXdmfRectilinearGrid
  rectfile.xmf
  rectfile.h5)
CLEAN_TEST_C(CTestXdmfRegularGrid
  regfile.xmf
  regfile.h5)
CLEAN_TEST_C(CTestXdmfUnstructuredGrid
  unfile.xmf
  unfile.h5)
CLEAN_TEST_C(CTestXdmfMap
  mapfile.xmf
  mapfile.h5)
CLEAN_TEST_C(CTestXdmfSet
  setfile.xmf
  setfile.h5)
