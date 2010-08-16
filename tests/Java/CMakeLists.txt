set(XdmfJavaTests
        TestXdmfJava
)

include (UseJavaTest)

set(JAVA_TEST_DEPENDENCIES ${XDMF_CORE_JAVA_JAR} ${XDMF_JAVA_JAR})
foreach (test ${XdmfJavaTests})
    set_source_files_properties(${test}.java PROPERTIES CLASSPATH "${XDMF_CORE_JAVA_JAR}:${XDMF_JAVA_JAR}")
    set_source_files_properties(${test}.java PROPERTIES LDPATH "${CMAKE_BINARY_DIR}")
    build_java_test(${test} ${test}.java ${JAVA_TEST_DEPENDENCIES})
    add_java_test(${test} ${test}.java)
endforeach (test ${XdmfJavaTests})
