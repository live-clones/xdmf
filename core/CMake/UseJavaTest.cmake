# Add a Java test 
# One cannot simply do:
# SET(ENV{LD_LIBRARY_PATH} ${LIBRARY_OUTPUT_PATH})
# SET(my_test foo)
# ADD_TEST(JAVA_TEST_1  java ${my_test})
# Since cmake is only transmitting the ADD_TEST line to ctest thus you are loosing
# the env var. The only way to store the env var is to physically write in the cmake script
# whatever LD_LIBRARY_PATH you want and then add the test as 'cmake -P java_test.cmake'
# 
# Usage:
# set (JAVA_TEST_DEPENDENCIES ${XDMF_CORE_JAVA_JAR} ${NETDMF_JAVA_JAR})
# set_source_files_properties(${test}.java PROPERTIES CLASSPATH "${XDMF_CORE_JAVA_JAR}:${NETDMF_JAVA_JAR}")
# set_source_files_properties(${test}.java PROPERTIES LDPATH "${CMAKE_BINARY_DIR}:${XDMF_LIBRARY_DIRS}")
# build_java_test(${test} ${test}.java ${JAVA_TEST_DEPENDENCIES})
# add_java_test(${test} ${test}.java)
#

# Modified by Zacarias Ojeda <zojeda@gmail.com>

MACRO(BUILD_JAVA_TEST TESTNAME FILENAME DEPS)
  GET_SOURCE_FILE_PROPERTY(src_file ${FILENAME} LOCATION)
  GET_SOURCE_FILE_PROPERTY(classpath ${FILENAME} CLASSPATH)
  GET_FILENAME_COMPONENT(FILENAME_BASE ${src_file} NAME_WE)

  ADD_CUSTOM_COMMAND(
       OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${FILENAME_BASE}.class
       WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
       COMMAND ${JAVA_COMPILE}
       ARGS ${src_file} -d ${CMAKE_CURRENT_BINARY_DIR} -cp ${classpath} ${wo_semicolumn}
       DEPENDS ${FILENAME} ${DEPS}
  )
  ADD_CUSTOM_TARGET("${FILENAME_BASE}_target" ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${FILENAME_BASE}.class)
ENDMACRO(BUILD_JAVA_TEST TESTNAME FILENAME)



MACRO(ADD_JAVA_TEST TESTNAME FILENAME)
  GET_SOURCE_FILE_PROPERTY(loc ${FILENAME} LOCATION)
  GET_SOURCE_FILE_PROPERTY(classpath ${FILENAME} CLASSPATH)
  GET_SOURCE_FILE_PROPERTY(ldpath ${FILENAME} LDPATH)
  GET_SOURCE_FILE_PROPERTY(args ${FILENAME} ARGS)
  GET_FILENAME_COMPONENT(loc2 ${loc} NAME_WE)

  IF (NOT args)
    SET(args "")
  ENDIF (NOT args)

  STRING(REGEX REPLACE ";" " " wo_semicolumn "${ARGN}")
  FILE(WRITE ${CMAKE_CURRENT_BINARY_DIR}/${TESTNAME}.cmake
"
  SET(ENV{LD_LIBRARY_PATH} ${ldpath}:$ENV{LD_LIBRARY_PATH})
  
  MESSAGE("Running Compiled File" )

  EXECUTE_PROCESS(
  	COMMAND ${JAVA_RUNTIME} -cp .:${classpath} ${loc2} ${args}
  	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  	RESULT_VARIABLE my_result
  	OUTPUT_VARIABLE my_output
  	ERROR_VARIABLE  my_error
  )
  
  MESSAGE(\"result is \${my_result}\")
  MESSAGE(\"output is \${my_output}\")
  MESSAGE(\"error is \${my_error}\")

  if(my_result)
    MESSAGE(SEND_ERROR "\${result}")
  endif(my_result)
"
)
  ADD_TEST(${TESTNAME} ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/${TESTNAME}.cmake)
ENDMACRO(ADD_JAVA_TEST)
