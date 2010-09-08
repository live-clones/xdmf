# Add any dependencies that the cxx tests may need
# Note: The tests already depend on their own file
SET_PROPERTY(GLOBAL APPEND PROPERTY CXX_TEST_DEPENDENCIES
	"XdmfUtils"
)

# Include our test macros
include (UseCxxTest)

# Add any cxx tests here:
# Note: We don't want to use a foreach loop to test the files incase we
#	have extra arguments (id: ADD_CXX_TEST(testname inputfile))
#	Read UseCxxTest.cmake for more information
# ---------------------------------------
ADD_CXX_TEST(TestXdmfTopologyConverter)

if(XDMF_BUILD_EXODUS_IO)
	ADD_CXX_TEST(TestXdmfExodusIO)
endif(XDMF_BUILD_EXODUS_IO)

# Add any cxx cleanup here:
# Note: We don't want to use a foreach loop to test the files incase we
#	have multiple files (ie: CLEAN_CXX_TEST(testname outputfile1 ...))
#	Read UseCxxTest.cmake for more information
# ---------------------------------------
CLEAN_CXX_TEST(TestXdmfTopologyConverter)
if(XDMF_BUILD_EXODUS_IO)
	CLEAN_CXX_TEST(TestXdmfExodusIO
		TestXdmfExodusIO.exo
	)
endif(XDMF_BUILD_EXODUS_IO)