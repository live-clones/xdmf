# Add any dependencies that the python tests may need
# Note: The tests already depend on their own file
SET_PROPERTY(GLOBAL APPEND PROPERTY PYTHON_TEST_DEPENDENCIES
	
)

# Add any pythonpath directories that the python tests may need
GET_PROPERTY(python_pythonpath GLOBAL PROPERTY PYTHON_TEST_PYTHONPATH)
SET_PROPERTY(GLOBAL PROPERTY PYTHON_TEST_PYTHONPATH
	"${CMAKE_BINARY_DIR}:"
)

# Include our test macros
include(UsePythonTest)

# Add any python tests here:
# Note: we don't want to use a foreach loop to test the files incase we
#	have extra arguments (ie: ADD_PYTHON_TEST(testname inputfile))
#	Read UsePythonTest.cmake for more information
# ---------------------
ADD_PYTHON_TEST(TestXdmfArray)

# Add any python cleanup here:
# Note: We don't want to use a foreach loop to test the files incase we
#       have multiple files (ie: CLEAN_PYTHON_TEST(testname outputfile1 ...))
#       Read UseCxxTest.cmake for more information
# ---------------------------------------
CLEAN_PYTHON_TEST(TestXdmfArray)

# Add a custom target for all python tests
# Note: ${TARGETS} is set in ADD_PYTHON_TEST
GET_PROPERTY(targets GLOBAL PROPERTY PYTHON_TEST_TARGETS)
ADD_CUSTOM_TARGET(Python_Core_Test_target ALL DEPENDS ${targets})

# We need to reset the targests since we created a core target
SET_PROPERTY(GLOBAL PROPERTY PYTHON_TEST_TARGETS "")
