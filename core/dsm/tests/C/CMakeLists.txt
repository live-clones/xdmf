# Include our test macros
include(AddTestsC)

# Add any dependencies that the cxx core tests may need
# Note: The tests already depend on their own file
ADD_TEST_C_DEPENDENCIES("XdmfCore")
ADD_TEST_C_DEPENDENCIES("XdmfDSM")

# Add any ldpath directories that the cxx tests may need
ADD_TEST_C_LDPATH("${CMAKE_BINARY_DIR}")

# Add any path directoreis that the Cxx tests may need
ADD_TEST_C_PATH("${CMAKE_BINARY_DIR}")

# Add any cxx tests here:
# Note: We don't want to use a foreach loop to test the files incase we
#       have extra arguments (ie: ADD_TEST_CXX(testname inputfile))
#       Read UseCxxTest.cmake for more information
# ---------------------------------------
if ("${XDMF_DSM_IS_CRAY}" STREQUAL "")
  if (MPIEXEC_MAX_NUMPROCS STRGREATER 5)
    ADD_MPI_TEST_C(CDSMLoopTest.sh CDSMLoopTest)
    ADD_MPI_TEST_C(CConnectTest.sh CXdmfAcceptTest,CXdmfConnectTest2,CXdmfConnectTest)
  endif(MPIEXEC_MAX_NUMPROCS STRGREATER 5)
endif ("${XDMF_DSM_IS_CRAY}" STREQUAL "")
# Add any cxx cleanup here:
# Note: We don't want to use a foreach loop to test the files incase we
#       have multiple files (ie: CLEAN_TEST_CXX(testname outputfile1 ...))
#       Read UseCxxTest.cmake for more information
# ---------------------------------------
if ("${XDMF_DSM_IS_CRAY}" STREQUAL "")
  if(XDMF_BUILD_DSM_THREADS)
    CLEAN_TEST_C(CConnectTest.sh dsmconnect.cfg)
  endif(XDMF_BUILD_DSM_THREADS)
endif ("${XDMF_DSM_IS_CRAY}" STREQUAL "")
