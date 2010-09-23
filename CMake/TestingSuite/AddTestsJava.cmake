INCLUDE(TestingSetup)

# Variables that are set externally
SET(java_binary_dir ${CMAKE_CURRENT_BINARY_DIR})
SET(java_source_dir ${CMAKE_CURRENT_SOURCE_DIR})

# Java Add Dependencies Macro
# Author: Brian Panneton
# Description: This macro adds the java test dependencies.
#        Note: The tests already depend on their own file
# Parameters:         
#              dependencies         = any dependencies needed for java tests
MACRO(ADD_TEST_JAVA_DEPENDENCIES dependencies)
	IF(NOT ("${dependencies}" STREQUAL ""))
		SET_PROPERTY(GLOBAL APPEND PROPERTY JAVA_TEST_DEPENDENCIES 
        		"${dependencies}"
		)
	ENDIF(NOT ("${dependencies}" STREQUAL ""))
ENDMACRO(ADD_TEST_JAVA_DEPENDENCIES dependencies)

# Java Add Classpath Macro
# Author: Brian Panneton
# Description: This macro adds the java test classpaths.
# Parameters:         
#              cp         = any classpaths needed for java tests
MACRO(ADD_TEST_JAVA_CLASSPATH cp)
        GET_PROPERTY(classpath GLOBAL PROPERTY JAVA_TEST_CLASSPATH)
        IF(NOT ("${cp}" STREQUAL ""))
                SET_PROPERTY(GLOBAL PROPERTY JAVA_TEST_CLASSPATH 
                        "${classpath}:${cp}" 
                )
        ENDIF(NOT ("${cp}" STREQUAL "")) 
ENDMACRO(ADD_TEST_JAVA_CLASSPATH cp)

# Java Add LDPath  Macro
# Author: Brian Panneton
# Description: This macro adds the java test ldpaths.
# Parameters:         
#               ld        = any ldpaths needed for java tests
MACRO(ADD_TEST_JAVA_LDPATH ld)
	GET_PROPERTY(ldpath GLOBAL PROPERTY JAVA_TEST_LDPATH)
	IF("${ld}" STRGREATER "")
		SET_PROPERTY(GLOBAL PROPERTY JAVA_TEST_LDPATH 
        		"${ldpath}:${ld}" 
		)
	ENDIF("${ld}" STRGREATER "")  
ENDMACRO(ADD_TEST_JAVA_LDPATH ld)

# Add Java Test Macro
# Author: Brian Panneton
# Description:	This macro builds and adds the java test in one shot. There is
#		no need to build a test separately, because there isn't a case 
#		that you don't want to run it.
# Parameters: 
#		executable 	= executable name 
#		${ARGN}		= any arguments for the executable
#
MACRO(ADD_TEST_JAVA executable)

	PARSE_TEST_ARGS("${ARGN}")	

	GET_PROPERTY(java_dependencies GLOBAL PROPERTY JAVA_TEST_DEPENDENCIES)
	GET_PROPERTY(java_classpath GLOBAL PROPERTY JAVA_TEST_CLASSPATH)
	GET_PROPERTY(java_ldpath GLOBAL PROPERTY JAVA_TEST_LDPATH)
	
	ADD_CUSTOM_COMMAND(
		OUTPUT ${java_binary_dir}/${executable}.class
		WORKING_DIRECTORY ${java_binary_dir}
		DEPENDS	${java_source_dir}/${executable}.java
			${java_dependencies}
		COMMAND ${JAVA_COMPILE}
		ARGS	-cp	'${java_classpath}'
			-d	'${java_binary_dir}'
			${java_source_dir}/${executable}.java
	)
	
	SET_PROPERTY(GLOBAL APPEND PROPERTY JAVA_TEST_TARGETS "${java_binary_dir}/${executable}.class")
	
	SET_CORE("${java_binary_dir}")
	ADD_TEST(Java${is_core}_${executable}${dup} ${CMAKE_COMMAND}
        	-D EXECUTABLE=${executable}
        	-D ARGUMENTS=${arguments}
        	-D CLASSPATH=${java_classpath}
        	-D LDPATH=${java_ldpath} 
        	-P ${java_binary_dir}/TestDriverJava.cmake
	) 

ENDMACRO(ADD_TEST_JAVA executable)

# Java Clean Macro
# Author: Brian Panneton
# Description: This macro sets up the java test for a make clean.
# Parameters:         
#              executable      = executable name
#              ${ARGN}         = files that the executable created
MACRO(CLEAN_TEST_JAVA executable)
       set_property(DIRECTORY APPEND PROPERTY 
                ADDITIONAL_MAKE_CLEAN_FILES ${ARGN}
       )
ENDMACRO(CLEAN_TEST_JAVA executable)

# Java Create Target Macro
# Author: Brian Panneton
# Description: This macro sets up the python test target
# Parameters:   none
MACRO(CREATE_TARGET_TEST_JAVA)
        IF(EXISTS JavaCore_ALLTEST)
                SET(JavaCore_ALLTEST JavaCore_ALLTEST)
        ENDIF(EXISTS JavaCore_ALLTEST)

	SET_CORE("${java_binary_dir}")
	GET_PROPERTY(targets GLOBAL PROPERTY JAVA_TEST_TARGETS)
        ADD_CUSTOM_TARGET(Java${is_core}_ALLTEST ALL DEPENDS 
                ${JavaCore_ALLTEST} ${targets})

	IF(NOT ("${is_core}" STREQUAL ""))
                SET_PROPERTY(GLOBAL PROPERTY JAVA_TEST_TARGETS "")
        ENDIF(NOT ("${is_core}" STREQUAL ""))

ENDMACRO(CREATE_TARGET_TEST_JAVA)


# Configure the java 'driver' file
CONFIGURE_FILE(${TESTING_SUITE_DIR}/TestingSuite/TestDriverJava.cmake.in ${java_binary_dir}/TestDriverJava.cmake @ONLY)