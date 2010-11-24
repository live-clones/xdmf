 INCLUDE(TestingSetup)

 # We should have one place that points to the cxx source directory and the cxx
 # binary directory
 SET(cxx_source_dir ${CMAKE_CURRENT_SOURCE_DIR})
 SET(cxx_binary_dir ${CMAKE_CURRENT_BINARY_DIR})

 # CXX Add Dependencies Macro
 # Author: Brian Panneton
 # Description: This macro adds the cxx test dependencies.
 # 	  Note: The tests already depend on their own file
 # Parameters:         
 #              dependencies         = any dependencies needed for cxx tests
 MACRO(ADD_TEST_CXX_DEPENDENCIES dependencies)
	IF(NOT ("${dependencies}" STREQUAL ""))
		SET_PROPERTY(GLOBAL APPEND PROPERTY CXX_TEST_DEPENDENCIES
        		 "${dependencies}"
 		)
	ENDIF(NOT ("${dependencies}" STREQUAL ""))
 ENDMACRO(ADD_TEST_CXX_DEPENDENCIES dependencies)

 # CXX Test Macro
 # Author: Brian Panneton
 # Description: This macro builds and add the cxx test in one shot.
 # Parameters:         
 #		executable      = executable name
 #             	${ARGN}         = any arguments for the executable
 MACRO(ADD_TEST_CXX executable)
	
	PARSE_TEST_ARGS("${ARGN}")
	
	IF(EXISTS ${cxx_source_dir}/${executable}.cpp)
		ADD_EXECUTABLE(${executable}${dup} ${cxx_source_dir}/${executable}.cpp)
   	ENDIF(EXISTS ${cxx_source_dir}/${executable}.cpp)

	IF(EXISTS ${cxx_source_dir}/${executable}.cxx)
		ADD_EXECUTABLE(${executable}${dup} ${cxx_source_dir}/${executable}.cxx)
   	ENDIF(EXISTS ${cxx_source_dir}/${executable}.cxx)
	
	GET_PROPERTY(cxx_dependencies GLOBAL PROPERTY CXX_TEST_DEPENDENCIES)
	TARGET_LINK_LIBRARIES(${executable}${dup} ${cxx_dependencies})
 
	SET_CORE("${cxx_binary_dir}")
   	ADD_TEST(Cxx${is_core}_${executable}${dup} ${executable}${dup} ${arguments})
 ENDMACRO(ADD_TEST_CXX executable)

 # CXX Clean Macro
 # Author: Brian Panneton
 # Description: This macro sets up the cxx test for a make clean.
 # Parameters:         
 #		executable      = executable name
 #             	${ARGN}         = files that the executable created
 MACRO(CLEAN_TEST_CXX executable)
	set_property(DIRECTORY APPEND PROPERTY 
		ADDITIONAL_MAKE_CLEAN_FILES ${ARGN} 
	)
 ENDMACRO(CLEAN_TEST_CXX executable)
