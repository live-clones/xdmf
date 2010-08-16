# List of global variables needed by tests
SET(defines 
	JAVA_TEST_DEPENDENCIES
	JAVA_TEST_CLASSPATH
	JAVA_TEST_LDPATH
	JAVA_TEST_TARGETS
	PYTHON_TEST_DEPENDENCIES
	CXX_TEST_DEPENDENCIES
)

# Make sure they are defined
FOREACH(def IN LISTS defines)
	GET_PROPERTY(is_defined GLOBAL PROPERTY ${def} DEFINED)
	IF(NOT is_defined)
		DEFINE_PROPERTY(GLOBAL PROPERTY ${def}
			BRIEF_DOCS "${def}"
			FULL_DOCS "${def} No Documentation"
		)
	ENDIF()
ENDFOREACH()

