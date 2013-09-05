# Include our test macros
include(AddTestsCxx)

# Add any dependencies that the cxx core tests may need
# Note: The tests already depend on their own file
ADD_TEST_CXX_DEPENDENCIES("XdmfDSM")

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
if(XDMF_BUILD_DSM_THREADS)
  ADD_TEST_CXX(TestXdmfHDF5WriterDSM)
endif(XDMF_BUILD_DSM_THREADS)

# Add any cxx cleanup here:
# Note: We don't want to use a foreach loop to test the files incase we
#       have multiple files (ie: CLEAN_TEST_CXX(testname outputfile1 ...))
#       Read UseCxxTest.cmake for more information
# ---------------------------------------
if(XDMF_BUILD_DSM_THREADS)
  CLEAN_TEST_CXX(TestXdmfHDF5WriterDSM)
endif(XDMF_BUILD_DSM_THREADS)
