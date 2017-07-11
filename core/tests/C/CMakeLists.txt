# Include our test macros
include(AddTestsC)

# Add any dependencies that the cxx core tests may need
# Note: The tests already depend on their own file
ADD_TEST_C_DEPENDENCIES("XdmfCore")

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
ADD_TEST_C(CTestXdmfArray)
ADD_TEST_C(CTestXdmfArrayType)
ADD_TEST_C(CTestXdmfError)
ADD_TEST_C(CTestXdmfFunction)
ADD_TEST_C(CTestXdmfHDF5Controller)
ADD_TEST_C(CTestXdmfInformation)
ADD_TEST_C(CTestXdmfSparseMatrix)
ADD_TEST_C(CTestXdmfSubset)

# Add any cxx cleanup here:
# Note: We don't want to use a foreach loop to test the files incase we
#       have multiple files (ie: CLEAN_TEST_C(testname outputfile1 ...))
#       Read UseCxxTest.cmake for more information
# ---------------------------------------
CLEAN_TEST_C(CTestXdmfArray)
CLEAN_TEST_C(CTestXdmfArrayType)
CLEAN_TEST_C(CTestXdmfError)
CLEAN_TEST_C(CTestXdmfFunction
  functionfile.xmf
  functionfile.h5)
CLEAN_TEST_C(CTestXdmfHDF5Controller
  testhdf5.h5)
CLEAN_TEST_C(CTestXdmfInformation)
CLEAN_TEST_C(CTestXdmfSparseMatrix)
CLEAN_TEST_C(CTestXdmfSubset
  subsetfile.xmf
  subsetfile.h5)
