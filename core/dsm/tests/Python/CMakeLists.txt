include(AddTestsPython)

# Add any dependencies that the python tests may need
# Note: The tests already depend on their own file
ADD_TEST_PYTHON_DEPENDENCIES("XdmfCore")
ADD_TEST_PYTHON_DEPENDENCIES("XdmfDSM")

# Add any pythonpath directories that the python tests may need
ADD_TEST_PYTHON_PYTHONPATH("${CMAKE_BINARY_DIR}")
ADD_TEST_PYTHON_PYTHONPATH("${PYTHON_INCLUDE_MPI4PY_DIR}/../..")

# Add any ldpath directories that the python tests may need
ADD_TEST_PYTHON_LDPATH("${CMAKE_BINARY_DIR}")
ADD_TEST_PYTHON_LDPATH("${XDMF_LIBRARY_DIRS}")

# Add any path directories that the python tests may need
ADD_TEST_PYTHON_PATH("${CMAKE_BINARY_DIR}")
ADD_TEST_PYTHON_PATH("${HDF5_BINARY_DIRS}")
ADD_TEST_PYTHON_PATH("${LIBXML2_BINARY_DIRS}")

# Add any python tests here:
# Note: We don't want to use a foreach loop to test the files incase we
#       have extra arguments (ie: ADD_TEST_PYTHON(testname inputfile))
#       Read UsePYTHONTest.cmake for more information
# ---------------------------------------
if (MPIEXEC_MAX_NUMPROCS STRGREATER 5)
  ADD_MPI_TEST_PYTHON(PythonDSM.sh XdmfExampleDsmTest)
  ADD_MPI_TEST_PYTHON(PythonConnect.sh XdmfExampleAcceptTest,XdmfExampleConnectTest2,XdmfExampleConnectTest)
endif(MPIEXEC_MAX_NUMPROCS STRGREATER 5)

# Add any python cleanup here:
# Note: We don't want to use a foreach loop to test the files incase we
#       have multiple files (ie: CLEAN_TEST_PYTHON(testname outputfile1 ...))
#       Read UsePYTHONTest.cmake for more information
# ---------------------------------------
if (MPIEXEC_MAX_NUMPROCS STRGREATER 5)
  CLEAN_TEST_PYTHON(PythonDSM.sh)
  CLEAN_TEST_PYTHON(PythonConnect.sh dsmconnect.cfg)
endif (MPIEXEC_MAX_NUMPROCS STRGREATER 5)
