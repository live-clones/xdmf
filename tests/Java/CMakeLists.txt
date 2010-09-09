# Add any dependencies that the java tests may need
# Note: The tests already depend on their own file
#	JAVA_TEST_DEPENDENCIES is also set in core
SET_PROPERTY(GLOBAL APPEND PROPERTY JAVA_TEST_DEPENDENCIES 
	${XDMF_JAVA_JAR}
)

# Add any classpath directories that the java tests may need
# Note: JAVA_TEST_CLASSPATH is also set in core
GET_PROPERTY(java_classpath GLOBAL PROPERTY JAVA_TEST_CLASSPATH)
SET_PROPERTY(GLOBAL PROPERTY JAVA_TEST_CLASSPATH 
	"${java_classpath}:${XDMF_JAVA_JAR}"
)

# Add any ldpath directories that the java tests may need
# Note: JAVA_TEST_LDPATH is also set in core
GET_PROPERTY(java_ldpath GLOBAL PROPERTY JAVA_TEST_LDPATH)
SET_PROPERTY(GLOBAL PROPERTY JAVA_TEST_LDPATH 
	"${java_ldpath}:"
)

# Include our test macros
include (UseJavaTest)

# Add any java tests here:
# Note:	We don't want to use a foreach loop to test the files incase we
#	have extra arguments (ie: ADD_JAVA_TEST(testname inputfile))
#	Read UseJavaTest.cmake for more information
# ------------------------
ADD_JAVA_TEST(TestXdmfJava)

# Add any java cleanup here:
# Note: We don't want to use a foreach loop to test the files incase we
#	have multiple files (ie: CLEAN_JAVA_TEST(testname outputfile1 ...))
#	Read UseCxxTest.cmake for more information
# ---------------------------------------
CLEAN_JAVA_TEST(TestXdmfJava
	outputJavaTest.xmf
)

# Add a custom target for all java tests 
# Note: ${TARGETS} is set in ADD_JAVA_TEST
IF(EXISTS Java_Core_Test_target)
	SET(Java_Core_Test_target Java_Core_Test_target)
ENDIF(EXISTS Java_Core_Test_target)

GET_PROPERTY(targets GLOBAL PROPERTY JAVA_TEST_TARGETS)
ADD_CUSTOM_TARGET(Java_ALLTEST ALL DEPENDS ${Java_Core_Test_target} XdmfJava ${targets}) 
