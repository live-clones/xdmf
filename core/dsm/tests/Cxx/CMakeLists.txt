# Include our test macros
include(AddTestsCxx)

# Add any dependencies that the cxx core tests may need
# Note: The tests already depend on their own file
ADD_TEST_CXX_DEPENDENCIES("XdmfCore")
ADD_TEST_CXX_DEPENDENCIES("XdmfDSM")

# Add any ldpath directories that the cxx tests may need
ADD_TEST_CXX_LDPATH("${CMAKE_BINARY_DIR}")

# Add any path directoreis that the Cxx tests may need
ADD_TEST_CXX_PATH("${CMAKE_BINARY_DIR}")

# Add any cxx tests here:
# Note: We don't want to use a foreach loop to test the files incase we
#       have extra arguments (ie: ADD_TEST_CXX(testname inputfile))
#       Read UseCxxTest.cmake for more information
# ---------------------------------------
IF (MPIEXEC_MAX_NUMPROCS STRGREATER 5)
  if ("${XDMF_DSM_IS_CRAY}" STREQUAL "")
    ADD_MPI_TEST_CXX(DSMLoopTest.sh DSMLoopTest)
    ADD_MPI_TEST_CXX(DSMLoopTestPaged.sh DSMLoopTestPaged)
    ADD_MPI_TEST_CXX(DSMLoopTestPagedSingleCore.sh DSMLoopTestPagedSingleCore)
    ADD_MPI_TEST_CXX(ConnectTest.sh
                     XdmfAcceptTest,XdmfConnectTest2,XdmfConnectTest)
    ADD_MPI_TEST_CXX(ConnectTestPaged.sh
                     XdmfAcceptTestPaged,XdmfConnectTest2Paged,XdmfConnectTestPaged)
  else ("${XDMF_DSM_IS_CRAY}" STREQUAL "")
    ADD_MPI_TEST_CXX(DSMLoopTestPagedSingleCore.sh DSMLoopTestPagedSingleCore)
    ADD_MPI_TEST_CXX(CrayConnectTest.sh
                     XdmfAcceptTest,XdmfConnectTest2,XdmfConnectTest)
  endif ("${XDMF_DSM_IS_CRAY}" STREQUAL "")
ENDIF(MPIEXEC_MAX_NUMPROCS STRGREATER 5)
# Add any cxx cleanup here:
# Note: We don't want to use a foreach loop to test the files incase we
#       have multiple files (ie: CLEAN_TEST_CXX(testname outputfile1 ...))
#       Read UseCxxTest.cmake for more information
# ---------------------------------------
IF (MPIEXEC_MAX_NUMPROCS STRGREATER 5)
  if ("${XDMF_DSM_IS_CRAY}" STREQUAL "")
  CLEAN_TEST_CXX(DSMLoopTest.sh)
  CLEAN_TEST_CXX(DSMLoopTestPaged.sh)
  if ("$ENV{XDMFDSM_CONFIG_FILE}" STREQUAL "")
    CLEAN_TEST_CXX(ConnectTest.sh dsmconnect.cfg)
    CLEAN_TEST_CXX(ConnectTestPaged.sh dsmconnect.cfg)
  else ("$ENV{XDMFDSM_CONFIG_FILE}" STREQUAL "")
    CLEAN_TEST_CXX(ConnectTest.sh "$ENV{XDMFDSM_CONFIG_FILE}")
    CLEAN_TEST_CXX(ConnectTestPaged.sh "$ENV{XDMFDSM_CONFIG_FILE}")
  endif ("$ENV{XDMFDSM_CONFIG_FILE}" STREQUAL "")
  else ("${XDMF_DSM_IS_CRAY}" STREQUAL "")
      CLEAN_TEST_CXX(DSMLoopTestPagedSingleCore.sh)
    if ("$ENV{XDMFDSM_CONFIG_FILE}" STREQUAL "")
      CLEAN_TEST_CXX(CrayConnectTest.sh dsmconnect.cfg)
    else ("$ENV{XDMFDSM_CONFIG_FILE}" STREQUAL "")
      CLEAN_TEST_CXX(CrayConnectTest.sh "$ENV{XDMFDSM_CONFIG_FILE}")
    endif ("$ENV{XDMFDSM_CONFIG_FILE}" STREQUAL "")
  endif ("${XDMF_DSM_IS_CRAY}" STREQUAL "")
ENDIF (MPIEXEC_MAX_NUMPROCS STRGREATER 5)
