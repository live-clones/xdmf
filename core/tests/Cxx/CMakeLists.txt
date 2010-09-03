# Add any dependencies that the cxx core tests may need
# Note: The tests already depend on their own file
SET_PROPERTY(GLOBAL APPEND PROPERTY CXX_TEST_DEPENDENCIES
	"XdmfCore"
)

# Include our test macros
include (UseCxxTest)

# Add any cxx tests here:
# Note: We don't want to use a foreach loop to test the files incase we
# 	have extra arguments (ie: ADD_CXX_TEST(testname inputfile))
#	Read UseCxxTest.cmake for more information
# ---------------------------------------
ADD_CXX_TEST(TestXdmfArray)
ADD_CXX_TEST(TestXdmfArrayWriteRead)
ADD_CXX_TEST(TestXdmfHDF5Controller)
ADD_CXX_TEST(TestXdmfHDF5Writer)
ADD_CXX_TEST(TestXdmfInformation)

# Add any cxx cleanup here:
# Note: We don't want to use a foreach loop to test the files incase we
#       have multiple files (ie: CLEAN_CXX_TEST(testname outputfile1 ...))
#       Read UseCxxTest.cmake for more information
# ---------------------------------------
CLEAN_CXX_TEST(TestXdmfArray)
CLEAN_CXX_TEST(TestXdmfArrayWriteRead 
	test.h5)
CLEAN_CXX_TEST(TestXdmfHDF5Controller)
CLEAN_CXX_TEST(TestXdmfHDF5Writer
	hdf5WriterTest.h5)
CLEAN_CXX_TEST(TestXdmfInformation)
