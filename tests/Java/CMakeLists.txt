INCLUDE(AddTestsJava)

# Add any dependencies that the java tests may need
# Note: The tests already depend on their own file
#	JAVA_TEST_DEPENDENCIES is also set in core
ADD_TEST_JAVA_DEPENDENCIES("${XDMF_JAVA_JAR}")

# Add any classpath directories that the java tests may need
# Note: JAVA_TEST_CLASSPATH is also set in core
ADD_TEST_JAVA_CLASSPATH("${XDMF_JAVA_JAR}")

# Add any ldpath directories that the java tests may need
# Note: JAVA_TEST_LDPATH is also set in core
ADD_TEST_JAVA_LDPATH("")

# Add any java tests here:
# Note:	We don't want to use a foreach loop to test the files incase we
#	have extra arguments (ie: ADD_TEST_JAVA(testname inputfile))
#	Read UseJavaTest.cmake for more information
# ------------------------
ADD_TEST_JAVA(TestXdmfJava)
ADD_TEST_JAVA(TestXdmfEquals)

# Add any java cleanup here:
# Note: We don't want to use a foreach loop to test the files incase we
#	have multiple files (ie: CLEAN_TEST_JAVA(testname outputfile1 ...))
#	Read UseCxxTest.cmake for more information
# ---------------------------------------
CLEAN_TEST_JAVA(TestXdmfJava
	outputJavaTest.xmf
)
CLEAN_TEST_JAVA(TestXdmfEquals)

# Add a custom target for all java tests 
# Note: ${TARGETS} is set in ADD_TEST_JAVA
#IF(EXISTS Java_Core_Test_target)
#	SET(Java_Core_Test_target Java_Core_Test_target)
#ENDIF(EXISTS Java_Core_Test_target)

#GET_PROPERTY(targets GLOBAL PROPERTY JAVA_TEST_TARGETS)
#ADD_CUSTOM_TARGET(Java_ALLTEST ALL DEPENDS ${Java_Core_Test_target} XdmfJava ${targets}) 
CREATE_TARGET_TEST_JAVA()
