INCLUDE(TestDefines)

# Variables that are set externally
SET(java_configure_files ${CMAKE_SOURCE_DIR}/core/CMake)
SET(java_binary_dir ${CMAKE_CURRENT_BINARY_DIR})
SET(java_source_dir ${CMAKE_CURRENT_SOURCE_DIR})
GET_PROPERTY(java_dependencies GLOBAL PROPERTY JAVA_TEST_DEPENDENCIES)
GET_PROPERTY(java_classpath GLOBAL PROPERTY JAVA_TEST_CLASSPATH)
GET_PROPERTY(java_ldpath GLOBAL PROPERTY JAVA_TEST_LDPATH)

# Add Java Test Macro
# Author: Brian Panneton
# Description:	This macro builds and adds the java test in one shot. There is
#		no need to build a test separately, because there isn't a case 
#		that you don't want to run it.
# Parameters: 
#		executable 	= executable name 
#		${ARGN}		= any arguments for the executable
#

MACRO(ADD_JAVA_TEST executable)
	
	IF(EXISTS ${ARGN})
		SET(arguments "${ARGN}")
	ELSE(EXISTS ${ARGN})
		SET(arguments "")
	ENDIF(EXISTS ${ARGN})

	ADD_CUSTOM_COMMAND(
		OUTPUT ${java_binary_dir}/${executable}.class
		WORKING_DIRECTORY ${java_binary_dir}
		DEPENDS	${java_source_dir}/${executable}.java
			${java_dependencies}
		COMMAND ${JAVA_COMPILE}
		ARGS	-cp	${java_classpath}
			-d	${java_binary_dir}
			${java_source_dir}/${executable}.java
	)
	
	SET_PROPERTY(GLOBAL APPEND PROPERTY JAVA_TEST_TARGETS "${java_binary_dir}/${executable}.class")

        STRING(REGEX MATCH "core" is_core "${java_binary_dir}")
        IF(EXISTS ${is_core})
                SET(is_core "Core")
        ENDIF(EXISTS ${is_core})
	

	ADD_TEST(Java${is_core}_${executable} ${CMAKE_COMMAND}
        	-D EXECUTABLE=${executable}
        	-D ARGUMENTS=${arguments}
        	-D CLASSPATH=${java_classpath}
        	-D LDPATH=${java_ldpath} 
        	-P ${java_binary_dir}/JavaTestDriver.cmake
	) 

ENDMACRO(ADD_JAVA_TEST)

# Java Clean Macro
# Author: Brian Panneton
# Description: This macro sets up the java test for a make clean.
# Parameters:         
#              executable      = executable name
#              ${ARGN}         = files that the executable created
MACRO(CLEAN_JAVA_TEST executable)
       set_property(DIRECTORY APPEND PROPERTY 
                ADDITIONAL_MAKE_CLEAN_FILES ${ARGN}
       )
ENDMACRO(CLEAN_JAVA_TEST executable)


# Configure the java 'driver' file
CONFIGURE_FILE(${java_configure_files}/JavaTestDriver.cmake.in ${java_binary_dir}/JavaTestDriver.cmake @ONLY)
