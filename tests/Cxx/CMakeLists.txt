# Include our test macros
include(AddTestsCxx)

# Add any dependencies that the cxx tests may need
# Note: The tests already depend on their own file
ADD_TEST_CXX_DEPENDENCIES("Xdmf")

# Add any cxx tests here:
# Note: We don't want to use a foreach loop to test the files incase we
#	have extra arguments (id: ADD_TEST_CXX(testname inputfile))
#	Read UseCxxTest.cmake for more information
# ---------------------------------------
ADD_TEST_CXX(TestXdmfAttribute)
ADD_TEST_CXX(TestXdmfGeometry)
ADD_TEST_CXX(TestXdmfGridCollection)
ADD_TEST_CXX(TestXdmfGridCurvilinear)
ADD_TEST_CXX(TestXdmfGridRegular)
ADD_TEST_CXX(TestXdmfGridRectilinear)
ADD_TEST_CXX(TestXdmfGridUnstructured)
ADD_TEST_CXX(TestXdmfReader)
ADD_TEST_CXX(TestXdmfSet)
ADD_TEST_CXX(TestXdmfTime)
ADD_TEST_CXX(TestXdmfTopology)
ADD_TEST_CXX(TestXdmfVisitorValueCounter)
ADD_TEST_CXX(TestXdmfWriter)
ADD_TEST_CXX(TestXdmfWriterHDF5ThenXML)
ADD_TEST_CXX(TestXdmfXPath)

# Add any cxx cleanup here:
# Note: We don't want to use a foreach loop to test the files incase we
#	have multiple files (ie: CLEAN_TEST_CXX(testname outputfile1 ...))
#	Read UseCxxTest.cmake for more information
# ---------------------------------------
CLEAN_TEST_CXX(TestXdmfAttribute)
CLEAN_TEST_CXX(TestXdmfGeometry)
CLEAN_TEST_CXX(TestXdmfGridCurvilinear
	TestXdmfGridCurvilinear1.xmf
	TestXdmfGridCurvilinear2.xmf
)
CLEAN_TEST_CXX(TestXdmfGridCollection
	TestXdmfGridCollection1.xmf
	TestXdmfGridCollection2.xmf
	TestXdmfGridCollectionHDF1.h5
	TestXdmfGridCollectionHDF1.xmf
	TestXdmfGridCollectionHDF2.xmf
)
CLEAN_TEST_CXX(TestXdmfGridRegular
	TestXdmfGridRegular1.xmf
	TestXdmfGridRegular2.xmf
)
CLEAN_TEST_CXX(TestXdmfGridRectilinear
	TestXdmfGridRectilinear1.xmf
	TestXdmfGridRectilinear2.xmf
)
CLEAN_TEST_CXX(TestXdmfGridUnstructured)
CLEAN_TEST_CXX(TestXdmfReader
	TestXdmfReader1.h5
	TestXdmfReader1.xmf
	TestXdmfReader2.xmf
)
CLEAN_TEST_CXX(TestXdmfSet)
CLEAN_TEST_CXX(TestXdmfTime)
CLEAN_TEST_CXX(TestXdmfTopology)
CLEAN_TEST_CXX(TestXdmfVisitorValueCounter)
CLEAN_TEST_CXX(TestXdmfWriter
	output.h5
	output.xmf)
CLEAN_TEST_CXX(TestXdmfWriterHDF5ThenXML)
CLEAN_TEST_CXX(TestXdmfXPath
	XdmfXPath1.xmf
	XdmfXPath2.xmf
)
