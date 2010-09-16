# Add any dependencies that the cxx tests may need
# Note: The tests already depend on their own file
SET_PROPERTY(GLOBAL APPEND PROPERTY CXX_TEST_DEPENDENCIES
	"Xdmf"
)

# Include our test macros
include (UseCxxTest)

# Add any cxx tests here:
# Note: We don't want to use a foreach loop to test the files incase we
#	have extra arguments (id: ADD_CXX_TEST(testname inputfile))
#	Read UseCxxTest.cmake for more information
# ---------------------------------------
ADD_CXX_TEST(TestXdmfAttribute)
ADD_CXX_TEST(TestXdmfGeometry)
ADD_CXX_TEST(TestXdmfGrid)
ADD_CXX_TEST(TestXdmfGridCollection)
ADD_CXX_TEST(TestXdmfGridRegular)
ADD_CXX_TEST(TestXdmfReader)
ADD_CXX_TEST(TestXdmfSet)
ADD_CXX_TEST(TestXdmfTime)
ADD_CXX_TEST(TestXdmfTopology)
ADD_CXX_TEST(TestXdmfVisitorValueCounter)
ADD_CXX_TEST(TestXdmfWriter)
ADD_CXX_TEST(TestXdmfWriterHDF5ThenXML)
ADD_CXX_TEST(TestXdmfXPath)

# Add any cxx cleanup here:
# Note: We don't want to use a foreach loop to test the files incase we
#	have multiple files (ie: CLEAN_CXX_TEST(testname outputfile1 ...))
#	Read UseCxxTest.cmake for more information
# ---------------------------------------
CLEAN_CXX_TEST(TestXdmfAttribute)
CLEAN_CXX_TEST(TestXdmfGeometry)
CLEAN_CXX_TEST(TestXdmfGrid)
CLEAN_CXX_TEST(TestXdmfGridCollection
	TestXdmfGridCollection1.xmf
	TestXdmfGridCollection2.xmf
	TestXdmfGridCollectionHDF1.h5
	TestXdmfGridCollectionHDF1.xmf
	TestXdmfGridCollectionHDF2.xmf
)
CLEAN_CXX_TEST(TestXdmfGridRegular
	TestXdmfGridRegular1.xmf
)
CLEAN_CXX_TEST(TestXdmfReader
	TestXdmfReader1.h5
	TestXdmfReader1.xmf
	TestXdmfReader2.xmf
)
CLEAN_CXX_TEST(TestXdmfSet)
CLEAN_CXX_TEST(TestXdmfTime)
CLEAN_CXX_TEST(TestXdmfTopology)
CLEAN_CXX_TEST(TestXdmfVisitorValueCounter)
CLEAN_CXX_TEST(TestXdmfWriter
	output.h5
	output.xmf)
CLEAN_CXX_TEST(TestXdmfWriterHDF5ThenXML)
CLEAN_CXX_TEST(TestXdmfXPath
	XdmfXPath1.xmf
	XdmfXPath2.xmf
)