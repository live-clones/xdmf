# Add any dependencies that the cxx core tests may need
# Note: The tests already depend on their own file
SET_PROPERTY(GLOBAL APPEND PROPERTY CXX_TEST_DEPENDENCIES
        "Xdmf"
)

# Include our test macros
include (UseCxxTest)

# Add any cxx tests here:
# Note: We don't want to use a foreach loop to test the files incase we
#       have extra arguments (id: ADD_CXX_TEST(testname inputfile))
#       Read UseCxxTest.cmake for more information
# ---------------------------------------
ADD_CXX_TEST(TestXdmfAttribute)
ADD_CXX_TEST(TestXdmfGeometry)
ADD_CXX_TEST(TestXdmfGrid)
ADD_CXX_TEST(TestXdmfGridCollection)
ADD_CXX_TEST(TestXdmfReader)
ADD_CXX_TEST(TestXdmfSet)
ADD_CXX_TEST(TestXdmfTime)
ADD_CXX_TEST(TestXdmfTopology)
ADD_CXX_TEST(TestXdmfVisitorValueCounter)
ADD_CXX_TEST(TestXdmfWriter)
ADD_CXX_TEST(TestXdmfWriterHDF5ThenXML)
ADD_CXX_TEST(TestXdmfXPath)
