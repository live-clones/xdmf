include(AddTestsPython)

# Add any dependencies that the python tests may need
# Note: The tests already depend on their own file
#	PYTHON_TEST_DEPENDENCIES is also set in core
ADD_TEST_PYTHON_DEPENDENCIES("")

# Add any pythonpath directories that the python tests may need
# Note: PYTHON_TEST_PYTHONPATH is also set in core
ADD_TEST_PYTHON_PYTHONPATH("${PYTHON_INCLUDE_MPI4PY_DIR}/../..")

# Add any python tests here:
# Note: We don't want to use a foreach loop to test the files incase we
#	have extra arguments (ie: ADD_TEST_PYTHON(testname inputfile))
#	Read UsePythonTest.cmake for more information
# -----------------------
ADD_TEST_PYTHON(XdmfTestDiff)
ADD_TEST_PYTHON(XdmfTestGeometryConverter)
ADD_TEST_PYTHON(XdmfTestTopologyConverter)

# Add any python cleanup here:
# Note: We don't want to use a foreach loop to test the files incase we
#	have multiple files (ie: CLEAN_TEST_PYTHON(testname outputfile1 ...))
#	Read UseCxxTest.cmake for more information
# ---------------------------------------
CLEAN_TEST_PYTHON(XdmfTestDiff)
CLEAN_TEST_PYTHON(XdmfTestGeometryConverter)
CLEAN_TEST_PYTHON(XdmfTestTopologyConverter)

# Add a custom target for all python tests
CREATE_TARGET_TEST_PYTHON()
