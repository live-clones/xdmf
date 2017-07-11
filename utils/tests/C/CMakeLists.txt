# Include our test macros
include(AddTestsC)

# Add any dependencies that the c core tests may need
# Note: The tests already depend on their own file
ADD_TEST_C_DEPENDENCIES("Xdmf")
ADD_TEST_C_DEPENDENCIES("XdmfUtils")

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
if (XDMF_BUILD_EXODUS_IO)
  ADD_TEST_C(CTestXdmfExodusIO)
endif (XDMF_BUILD_EXODUS_IO)
ADD_TEST_C(CTestXdmfDiff)
ADD_TEST_C(CTestXdmfGeometryConverter)
ADD_TEST_C(CTestXdmfTopologyConverter)

# Add any cxx cleanup here:
# Note: We don't want to use a foreach loop to test the files incase we
#       have multiple files (ie: CLEAN_TEST_C(testname outputfile1 ...))
#       Read UseCxxTest.cmake for more information
# ---------------------------------------
if (XDMF_BUILD_EXODUS_IO)
  CLEAN_TEST_C(CTestXdmfExodusIO
    TestXdmfExodusIO.exo)
endif (XDMF_BUILD_EXODUS_IO)
CLEAN_TEST_C(CTestXdmfDiff)
CLEAN_TEST_C(CTestXdmfGeometryConverter)
CLEAN_TEST_C(CTestXdmfTopologyConverter)
