# We cannot call ${PYTHON_EXECUTABLE} or ${PYTHON_LIBRARIES} because
# they do not get propagated down to this subdirectory
SET(PYTHON_EXECUTABLE python)

# Variables that are set externally
SET(python_configure_files ${CMAKE_SOURCE_DIR}/core/CMake)
SET(python_binary_dir ${CMAKE_CURRENT_BINARY_DIR})
SET(python_source_dir ${CMAKE_CURRENT_SOURCE_DIR})
GET_PROPERTY(python_dependencies GLOBAL PROPERTY PYTHON_TEST_DEPENDENCIES)
GET_PROPERTY(python_pythonpath GLOBAL PROPERTY PYTHON_TEST_PYTHONPATH)

# Add Python Test Macro
# Author: Brian Panneton
# Description:	This macro compiles and adds the python test in one shot. There is
#		no need to build a test separately, because there isn't a case 
#		that you don't want to run it.
# Parameters: 
#		executable 	= executable name 
#		${ARGN}		= any arguments for the executable
#

MACRO(ADD_PYTHON_TEST executable)
	
	IF(EXISTS ${ARGN})
		SET(arguments "${ARGN}")
	ELSE(EXISTS ${ARGN})
		SET(arguments "")
	ENDIF(EXISTS ${ARGN})

	ADD_CUSTOM_COMMAND(
		OUTPUT ${python_binary_dir}/${executable}.pyc
		WORKING_DIRECTORY ${python_binary_dir} 
		COMMAND ${CMAKE_COMMAND}
		ARGS 	-E copy
			${python_source_dir}/${executable}.py
			${python_binary_dir}/${executable}.py
		COMMAND ${PYTHON_EXECUTABLE} 
		ARGS	-mpy_compile
			${python_binary_dir}/${executable}.py
		DEPENDS ${python_source_dir}/${executable}.py
			${python_dependencies}
	)
	
	SET_PROPERTY(GLOBAL APPEND PROPERTY PYTHON_TEST_TARGETS "${python_binary_dir}/${executable}.pyc")

	STRING(REGEX MATCH "core" is_core "${python_binary_dir}")
	IF(EXISTS ${is_core})
		SET(is_core "Core")
	ENDIF(EXISTS ${is_core})
	
	ADD_TEST(Python${is_core}_${executable} ${CMAKE_COMMAND}
        	-D EXECUTABLE=${executable}
        	-D ARGUMENTS=${arguments}
		-D PYTHONPATH=${python_pythonpath}
        	-P ${python_binary_dir}/PythonTestDriver.cmake
	) 

ENDMACRO(ADD_PYTHON_TEST)

# Python Clean Macro
# Author: Brian Panneton
# Description: This macro sets up the python test for a make clean.
# Parameters:         
#              executable      = executable name
#              ${ARGN}         = files that the executable created
MACRO(CLEAN_PYTHON_TEST executable)
	set_property(DIRECTORY APPEND PROPERTY
		ADDITIONAL_MAKE_CLEAN_FILES ${ARGN}
		${executable}.py
	)
ENDMACRO(CLEAN_PYTHON_TEST executable)

# Configure the python 'driver' file
CONFIGURE_FILE(${python_configure_files}/PythonTestDriver.cmake.in ${python_binary_dir}/PythonTestDriver.cmake @ONLY)

