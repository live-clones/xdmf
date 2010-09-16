# Include our test macros
include(AddTestsCxx)

# Add any dependencies that the cxx core tests may need
# Note: The tests already depend on their own file
ADD_TEST_CXX_DEPENDENCIES("XdmfCore")

# Add any cxx tests here:
# Note: We don't want to use a foreach loop to test the files incase we
#	have extra arguments (ie: ADD_TEST_CXX(testname inputfile))
#	Read UseCxxTest.cmake for more information
# ---------------------------------------
ADD_TEST_CXX(TestXdmfArray)
ADD_TEST_CXX(TestXdmfArrayWriteRead)
ADD_TEST_CXX(TestXdmfHDF5Controller)
ADD_TEST_CXX(TestXdmfHDF5Writer)
ADD_TEST_CXX(TestXdmfInformation)

# Add any cxx cleanup here:
# Note: We don't want to use a foreach loop to test the files incase we
#	have multiple files (ie: CLEAN_TEST_CXX(testname outputfile1 ...))
#	Read UseCxxTest.cmake for more information
# ---------------------------------------
CLEAN_TEST_CXX(TestXdmfArray)
CLEAN_TEST_CXX(TestXdmfArrayWriteRead 
	test.h5)
CLEAN_TEST_CXX(TestXdmfHDF5Controller)
CLEAN_TEST_CXX(TestXdmfHDF5Writer
	hdf5WriterTest.h5)
CLEAN_TEST_CXX(TestXdmfInformation)
