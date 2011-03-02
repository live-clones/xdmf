include(AddTestsPython)

# Add any dependencies that the python tests may need
# Note: The tests already depend on their own file
ADD_TEST_PYTHON_DEPENDENCIES("")

# Add any pythonpath directories that the python tests may need
ADD_TEST_PYTHON_PYTHONPATH("${CMAKE_BINARY_DIR}")

# Add any ldpath directories that the python tests may need
ADD_TEST_PYTHON_LDPATH("${CMAKE_BINARY_DIR}")
ADD_TEST_PYTHON_LDPATH("${HDF5_LIBRARIES}")
ADD_TEST_PYTHON_LDPATH("${LIBXML2_LIBRARIES}")

# Add any path directories that the python tests may need
ADD_TEST_PYTHON_PATH("${CMAKE_BINARY_DIR}")
ADD_TEST_PYTHON_PATH("${HDF5_BINARY_DIRS}")
ADD_TEST_PYTHON_PATH("${LIBXML2_BINARY_DIRS}")

# Add any python tests here:
# Note: we don't want to use a foreach loop to test the files incase we
#       have extra arguments (ie: ADD_TEST_PYTHON(testname inputfile))
#       Read UsePythonTest.cmake for more information
# ---------------------
ADD_TEST_PYTHON(TestXdmfArray)
ADD_TEST_PYTHON(TestXdmfArrayMultidimensional)
ADD_TEST_PYTHON(TestXdmfEquals)
ADD_TEST_PYTHON(TestXdmfVersion)

# Add any python cleanup here:
# Note: We don't want to use a foreach loop to test the files incase we
#       have multiple files (ie: CLEAN_TEST_PYTHON(testname outputfile1 ...))
#       Read UseCxxTest.cmake for more information
# ---------------------------------------
CLEAN_TEST_PYTHON(TestXdmfArray)
CLEAN_TEST_PYTHON(TestXdmfArrayMultidimensional)
CLEAN_TEST_PYTHON(TestXdmfEquals)
CLEAN_TEST_PYTHON(TestXdmfVersion)

# Add a custom target for all python tests
CREATE_TARGET_TEST_PYTHON()
