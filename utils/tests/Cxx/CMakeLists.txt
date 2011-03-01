# Add any dependencies that the cxx tests may need
# Note: The tests already depend on their own file
SET_PROPERTY(GLOBAL APPEND PROPERTY CXX_TEST_DEPENDENCIES
  "XdmfUtils")

# Include XdmfTestDataGenerator from non-util tests
include_directories(${CMAKE_SOURCE_DIR}/tests/Cxx/)

# Include XdmfTestDataGenerator from non-util tests
include_directories(${CMAKE_SOURCE_DIR}/tests/Cxx/)

# Include our test macros
include(AddTestsCxx)

# Add any cxx tests here:
# Note: We don't want to use a foreach loop to test the files incase we
#       have extra arguments (id: ADD_TEST_CXX(testname inputfile))
#       Read UseCxxTest.cmake for more information
# ---------------------------------------
ADD_TEST_CXX(TestXdmfTopologyConverter)

if(XDMF_BUILD_EXODUS_IO)
  ADD_TEST_CXX(TestXdmfExodusIO)
endif(XDMF_BUILD_EXODUS_IO)

# Add any cxx cleanup here:
# Note: We don't want to use a foreach loop to test the files incase we
#       have multiple files (ie: CLEAN_TEST_CXX(testname outputfile1 ...))
#       Read UseCxxTest.cmake for more information
# ---------------------------------------
CLEAN_TEST_CXX(TestXdmfTopologyConverter)
if(XDMF_BUILD_EXODUS_IO)
  CLEAN_TEST_CXX(TestXdmfExodusIO
    TestXdmfExodusIO.exo)
endif(XDMF_BUILD_EXODUS_IO)
