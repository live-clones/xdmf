INCLUDE(AddTestsJava)

#Add any dependencies that the java core tests may need
# Note: The tests already depend on their own file
ADD_TEST_JAVA_DEPENDENCIES(Compiled_XdmfCore_Jar)

# Add any classpath directories that the java tests may need
# Note: JAVA_TEST_CLASSPATH is also set in core
ADD_TEST_JAVA_CLASSPATH(".:${XdmfCore_JAVA_JAR}")

# Add any ldpath directories that the java tests may need
# Note: JAVA_TEST_LDPATH is also set in core
ADD_TEST_JAVA_LDPATH("${CMAKE_BINARY_DIR}")

# Add any java tests here:
# Note: We don't want to use a foreach loop to test the files incase we
#	have extra arguments (ie: ADD_TEST_JAVA(testname inputfile))
#	Read UseJavaTest.cmake for more information
# ------------------------
ADD_TEST_JAVA(TestXdmfEquals)

# Add any java cleanup here:
# Note: We don't want to use a foreach loop to test the files incase we
#	have multiple files (ie: CLEAN_TEST_JAVA(testname outputfile1 ...))
#	Read UseCxxTest.cmake for more information
# ---------------------------------------
CLEAN_TEST_JAVA(TestXdmfEquals)

# Add a custom target for all java tests 
# Note: ${TARGETS} is set in ADD_TEST_JAVA
#GET_PROPERTY(targets GLOBAL PROPERTY JAVA_TEST_TARGETS)
#ADD_CUSTOM_TARGET(Java_Core_Test_target ALL DEPENDS XdmfCoreJava ${targets})
CREATE_TARGET_TEST_JAVA()
