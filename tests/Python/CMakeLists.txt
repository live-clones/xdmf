# Add any dependencies that the python tests may need
# Note: The tests already depend on their own file
#	PYTHON_TEST_DEPENDENCIES is also set in core
SET_PROPERTY(GLOBAL APPEND PROPERTY PYTHON_TEST_DEPENDENCIES
	""
)

# Add any pythonpath directories that the python tests may need
# Note: PYTHON_TEST_PYTHONPATH is also set in core
GET_PROPERTY(python_pythonpath GLOBAL PROPERTY PYTHON_TEST_PYTHONPATH)
SET_PROPERTY(GLOBAL PROPERTY PYTHON_TEST_PYTHONPATH
	"${python_pythonpath}:"
)

# Include our test macros
include(UsePythonTest)

# Add any python tests here:
# Note: We don't want to use a foreach loop to test the files incase we
#	have extra arguments (ie: ADD_PYTHON_TEST(testname inputfile))
#	Read UsePythonTest.cmake for more information
# -----------------------
ADD_PYTHON_TEST(TestXdmfHDF5Writer)
ADD_PYTHON_TEST(TestXdmfWriter)

# Add any python cleanup here:
# Note: We don't want to use a foreach loop to test the files incase we
#	have multiple files (ie: CLEAN_PYTHON_TEST(testname outputfile1 ...))
#	Read UseCxxTest.cmake for more information
# ---------------------------------------
CLEAN_PYTHON_TEST(TestXdmfHDF5Writer
	hdf5WriterPythonTest.h5
)
CLEAN_PYTHON_TEST(TestXdmfWriter
	output.h5
	output.xmf
)

# Add a custom target for all python tests
# Note: ${TARGETS} is set in ADD_PYTHON_TEST
IF(EXISTS Python_Core_Test_target)
	SET(Python_Core_Test_target Python_Core_Test_target)
ENDIF(EXISTS Python_Core_Test_target)

GET_PROPERTY(targets GLOBAL PROPERTY PYTHON_TEST_TARGETS)
ADD_CUSTOM_TARGET(Python_ALLTEST ALL DEPENDS ${Python_Core_Test_target} ${targets})
