include(AddTestsPython)

# Add any dependencies that the python tests may need
# Note: The tests already depend on their own file
ADD_TEST_PYTHON_DEPENDENCIES("")

# Add any pythonpath directories that the python tests may need
ADD_TEST_PYTHON_PYTHONPATH("${CMAKE_BINARY_DIR}")

# Add any python tests here:
# Note: we don't want to use a foreach loop to test the files incase we
#	have extra arguments (ie: ADD_TEST_PYTHON(testname inputfile))
#	Read UsePythonTest.cmake for more information
# ---------------------
ADD_TEST_PYTHON(TestXdmfArray)
ADD_TEST_PYTHON(TestXdmfEquals)

# Add any python cleanup here:
# Note: We don't want to use a foreach loop to test the files incase we
#	have multiple files (ie: CLEAN_TEST_PYTHON(testname outputfile1 ...))
#	Read UseCxxTest.cmake for more information
# ---------------------------------------
CLEAN_TEST_PYTHON(TestXdmfArray)
CLEAN_TEST_PYTHON(TestXdmfEquals)

# Add a custom target for all python tests
# Note: ${TARGETS} is set in ADD_TEST_PYTHON
#GET_PROPERTY(targets GLOBAL PROPERTY PYTHON_TEST_TARGETS)
#ADD_CUSTOM_TARGET(Python_Core_Test_target ALL DEPENDS ${targets})
CREATE_TARGET_TEST_PYTHON()
# We need to reset the targets since we created a core target
#SET_PROPERTY(GLOBAL PROPERTY PYTHON_TEST_TARGETS "")
