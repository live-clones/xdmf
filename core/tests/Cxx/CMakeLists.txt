# Include our test macros
include(AddTestsCxx)

# Add any dependencies that the cxx core tests may need
# Note: The tests already depend on their own file
ADD_TEST_CXX_DEPENDENCIES("XdmfCore")

# Add any ldpath directories that the cxx tests may need
ADD_TEST_CXX_LDPATH("${CMAKE_BINARY_DIR}")
ADD_TEST_CXX_LDPATH("${XDMF_LIBRARY_DIRS}")

# Add any path directoreis that the Cxx tests may need
ADD_TEST_CXX_PATH("${CMAKE_BINARY_DIR}")
ADD_TEST_CXX_PATH("${XDMF_BINARIES}")

# Add any cxx tests here:
# Note: We don't want to use a foreach loop to test the files incase we
#       have extra arguments (ie: ADD_TEST_CXX(testname inputfile))
#       Read UseCxxTest.cmake for more information
# ---------------------------------------
ADD_TEST_CXX(TestXdmfArray)
ADD_TEST_CXX(TestXdmfArrayInsert)
ADD_TEST_CXX(TestXdmfArrayMultidimensional)
ADD_TEST_CXX(TestXdmfArrayMultiDimensionalInsert)
ADD_TEST_CXX(TestXdmfArrayWriteRead)
ADD_TEST_CXX(TestXdmfArrayWriteReadHyperSlabs)
ADD_TEST_CXX(TestXdmfError)
ADD_TEST_CXX(TestXdmfHDF5Controller)
ADD_TEST_CXX(TestXdmfHDF5Writer)
ADD_TEST_CXX(TestXdmfHDF5WriterTree)
ADD_TEST_CXX(TestXdmfInformation)
ADD_TEST_CXX(TestXdmfSparseMatrix)
ADD_TEST_CXX(TestXdmfVersion)

# Add any cxx cleanup here:
# Note: We don't want to use a foreach loop to test the files incase we
#       have multiple files (ie: CLEAN_TEST_CXX(testname outputfile1 ...))
#       Read UseCxxTest.cmake for more information
# ---------------------------------------
CLEAN_TEST_CXX(TestXdmfArray)
CLEAN_TEST_CXX(TestXdmfArrayInsert)
CLEAN_TEST_CXX(TestXdmfArrayMultidimensional)
CLEAN_TEST_CXX(TestXdmfArrayMultiDimensionalInsert)
CLEAN_TEST_CXX(TestXdmfArrayWriteRead
  test.h5)
CLEAN_TEST_CXX(TestXdmfArrayWriteRead
  testHyperslab.h5)
CLEAN_TEST_CXX(TestXdmfError)
CLEAN_TEST_CXX(TestXdmfHDF5Controller)
CLEAN_TEST_CXX(TestXdmfHDF5Writer
  hdf5WriterTest.h5)
CLEAN_TEST_CXX(TestXdmfHDF5WriterTree
  hdf5WriterTestTree.h5)
CLEAN_TEST_CXX(TestXdmfInformation)
CLEAN_TEST_CXX(TestXdmfSparseMatrix
  TestXdmfSparseMatrix.xmf)
CLEAN_TEST_CXX(TestXdmfVersion)
