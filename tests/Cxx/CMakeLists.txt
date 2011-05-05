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
ADD_TEST_CXX(TestXdmfCurvilinearGrid)
ADD_TEST_CXX(TestXdmfGeometry)
ADD_TEST_CXX(TestXdmfGridCollection)
ADD_TEST_CXX(TestXdmfHDF5Hyperslab)
ADD_TEST_CXX(TestXdmfReader)
ADD_TEST_CXX(TestXdmfRegularGrid)
ADD_TEST_CXX(TestXdmfRectilinearGrid)
ADD_TEST_CXX(TestXdmfSet)
ADD_TEST_CXX(TestXdmfTime)
ADD_TEST_CXX(TestXdmfTopology)
ADD_TEST_CXX(TestXdmfTopologyMixed)
ADD_TEST_CXX(TestXdmfUnstructuredGrid)
ADD_TEST_CXX(TestXdmfVisitorValueCounter)
ADD_TEST_CXX(TestXdmfWriter)
ADD_TEST_CXX(TestXdmfWriterHDF5ThenXML)
ADD_TEST_CXX(TestXdmfXPath)

# Add any cxx cleanup here:
# Note: We don't want to use a foreach loop to test the files incase we
#       have multiple files (ie: CLEAN_TEST_CXX(testname outputfile1 ...))
#       Read UseCxxTest.cmake for more information
# ---------------------------------------
CLEAN_TEST_CXX(TestXdmfAttribute)
CLEAN_TEST_CXX(TestXdmfCurvilinearGrid
  TestXdmfCurvilinearGrid1.xmf
  TestXdmfCurvilinearGrid2.xmf)
CLEAN_TEST_CXX(TestXdmfGeometry)
CLEAN_TEST_CXX(TestXdmfGridCollection
  TestXdmfGridCollection1.xmf
  TestXdmfGridCollection2.xmf
  TestXdmfGridCollectionHDF1.h5
  TestXdmfGridCollectionHDF1.xmf
  TestXdmfGridCollectionHDF2.xmf)
CLEAN_TEST_CXX(TestXdmfHDF5Hyperslab
  TestXdmfHDF5Hyperslab.xmf
  TestXdmfHDF5Hyperslab.h5
  TestXdmfHDF5Hyperslab2.xmf)
CLEAN_TEST_CXX(TestXdmfReader
  TestXdmfReader1.h5
  TestXdmfReader1.xmf
  TestXdmfReader2.xmf)
CLEAN_TEST_CXX(TestXdmfRectilinearGrid
  TestXdmfRectilinearGrid1.xmf
  TestXdmfRectilinearGrid2.xmf)
CLEAN_TEST_CXX(TestXdmfRegularGrid
  TestXdmfRegularGrid1.xmf
  TestXdmfRegularGrid2.xmf)
CLEAN_TEST_CXX(TestXdmfSet)
CLEAN_TEST_CXX(TestXdmfTime)
CLEAN_TEST_CXX(TestXdmfTopology)
CLEAN_TEST_CXX(TestXdmfTopologyMixed
  TestXdmfTopologyMixed1.xmf
  TestXdmfTopologyMixed2.xmf)
CLEAN_TEST_CXX(TestXdmfUnstructuredGrid)
CLEAN_TEST_CXX(TestXdmfVisitorValueCounter)
CLEAN_TEST_CXX(TestXdmfWriter
  output.h5
  output.xmf)
CLEAN_TEST_CXX(TestXdmfWriterHDF5ThenXML)
CLEAN_TEST_CXX(TestXdmfXPath
  XdmfXPath1.xmf
  XdmfXPath2.xmf)