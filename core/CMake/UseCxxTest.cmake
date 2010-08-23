 # We should have one place that points to the cxx source directory and the cxx
 # binary directory
 SET(cxx_source_dir ${CMAKE_CURRENT_SOURCE_DIR})
 SET(cxx_binary_dir ${CMAKE_CURRENT_BINARY_DIR})
 GET_PROPERTY(cxx_dependencies GLOBAL PROPERTY CXX_TEST_DEPENDENCIES)

 # CXX Test Macro
 # Author: Brian Panneton
 # Description: This macro builds and add the cxx test in one shot.
 # Parameters:         
 #		executable      = executable name
 #             	${ARGN}         = any arguments for the executable
 MACRO(ADD_CXX_TEST executable)
       	IF(EXISTS ${ARGN})
               SET(arguments "${ARGN}")
       	ELSE(EXISTS ${ARGN}) # Sanity Check
               SET(arguments "")
       	ENDIF(EXISTS ${ARGN})

	IF(EXISTS ${cxx_source_dir}/${executable}.cpp)
		ADD_EXECUTABLE(${executable} ${cxx_source_dir}/${executable}.cpp)
      	ENDIF(EXISTS ${cxx_source_dir}/${executable}.cpp)

	IF(EXISTS ${cxx_source_dir}/${executable}.cxx)
		ADD_EXECUTABLE(${executable} ${cxx_source_dir}/${executable}.cxx)
       	ENDIF(EXISTS ${cxx_source_dir}/${executable}.cxx)
	
	TARGET_LINK_LIBRARIES(${executable} ${cxx_dependencies})
 
        STRING(REGEX MATCH "core" is_core "${cxx_binary_dir}")
        IF(EXISTS ${is_core})
                SET(is_core "Core")
        ENDIF(EXISTS ${is_core})

      	ADD_TEST(Cxx${is_core}_${executable} ${executable} ${arguments})
 ENDMACRO(ADD_CXX_TEST executable)

