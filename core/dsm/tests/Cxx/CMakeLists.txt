# Include our test macros
include(AddTestsCxx)

# Add any dependencies that the cxx core tests may need
# Note: The tests already depend on their own file
ADD_TEST_CXX_DEPENDENCIES("XdmfCore")
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
IF(XDMF_BUILD_DSM_THREADS)
  ADD_TEST_CXX(TestXdmfHDF5WriterDSM)
ENDIF(XDMF_BUILD_DSM_THREADS)
IF (MPIEXEC_MAX_NUMPROCS STRGREATER 5)
  ADD_MPI_TEST_CXX(DSMLoopTest.sh DSMLoopTest)
  get_filename_component(MPI_BIN_DIRECTORY ${MPI_CXX_COMPILER} PATH)
  EXECUTE_PROCESS(
    COMMAND ${MPI_BIN_DIRECTORY}/mpirun --version
    OUTPUT_VARIABLE MPI_TYPE_OUTPUT
    ERROR_VARIABLE MPI_TYPE_ERROR
  )
  STRING(REGEX MATCH "Open MPI" IS_OPENMPI "${MPI_TYPE_ERROR}")
  IF ("${IS_OPENMPI}" STREQUAL "")
    ADD_MPI_TEST_CXX(ConnectTest.sh XdmfAcceptTest,XdmfConnectTest2,XdmfConnectTest)
  ENDIF ("${IS_OPENMPI}" STREQUAL "")
ENDIF(MPIEXEC_MAX_NUMPROCS STRGREATER 5)

# Add any cxx cleanup here:
# Note: We don't want to use a foreach loop to test the files incase we
#       have multiple files (ie: CLEAN_TEST_CXX(testname outputfile1 ...))
#       Read UseCxxTest.cmake for more information
# ---------------------------------------
IF(XDMF_BUILD_DSM_THREADS)
  CLEAN_TEST_CXX(TestXdmfHDF5WriterDSM)
ENDIF(XDMF_BUILD_DSM_THREADS)
CLEAN_TEST_CXX(DSMLoopTest.sh)
IF (MPIEXEC_MAX_NUMPROCS STRGREATER 5)
  CLEAN_TEST_CXX(ConnectTest.sh dsmconnect.cfg)
ENDIF (MPIEXEC_MAX_NUMPROCS STRGREATER 5)
