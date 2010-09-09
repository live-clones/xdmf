#Add any dependencies that the java core tests may need
# Note: The tests already depend on their own file
SET_PROPERTY(GLOBAL APPEND PROPERTY JAVA_TEST_DEPENDENCIES 
	"${XDMF_CORE_JAVA_JAR}"
)
# Add any classpath directories that the java tests may need
# Note: JAVA_TEST_CLASSPATH is also set in core
GET_PROPERTY(java_classpath GLOBAL PROPERTY JAVA_TEST_CLASSPATH)
SET_PROPERTY(GLOBAL PROPERTY JAVA_TEST_CLASSPATH 
	".:${XDMF_CORE_JAVA_JAR}"
)
# Add any ldpath directories that the java tests may need
# Note: JAVA_TEST_LDPATH is also set in core
GET_PROPERTY(java_ldpath GLOBAL PROPERTY JAVA_TEST_LDPATH)
SET_PROPERTY(GLOBAL PROPERTY JAVA_TEST_LDPATH 
	"${CMAKE_BINARY_DIR}"
)

# Include our test macros
include (UseJavaTest)

# Add any java tests here:
# Note: We don't want to use a foreach loop to test the files incase we
#	have extra arguments (ie: ADD_JAVA_TEST(testname inputfile))
#	Read UseJavaTest.cmake for more information
# ------------------------
# none

# Add any java cleanup here:
# Note: We don't want to use a foreach loop to test the files incase we
#	have multiple files (ie: CLEAN_JAVA_TEST(testname outputfile1 ...))
#	Read UseCxxTest.cmake for more information
# ---------------------------------------
# none

# Add a custom target for all java tests 
# Note: ${TARGETS} is set in ADD_JAVA_TEST
GET_PROPERTY(targets GLOBAL PROPERTY JAVA_TEST_TARGETS)
ADD_CUSTOM_TARGET(Java_Core_Test_target ALL DEPENDS XdmfCoreJava ${targets})
