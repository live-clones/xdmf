
set(_Executable_Output_Path ${EXECUTABLE_OUTPUT_PATH})
IF(WIN32)
  SET(EXECUTABLE_OUTPUT_PATH ${EXECUTABLE_OUTPUT_PATH}/${CMAKE_BUILD_TYPE})
ENDIF(WIN32)

macro( XdmfSerialTest testname )
    add_executable( ${testname} ${ARGN} )
    target_link_libraries( ${testname} Xdmf )
    add_test( ${testname} ${_Executable_Output_Path}/${testname} )
endmacro()

macro( XdmfMPITest testname num_procs )
    find_package( MPI )
    if( MPI_FOUND )
        include_directories( MPI_INCLUDE_PATH )
        add_executable( ${testname} ${ARGN} )
        target_link_libraries( ${testname} Xdmf ${MPI_LIBRARIES} )
        add_test( ${testname} 
            ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} ${num_procs}
            ${MPI_EXEC_PREFLAGS}
            ${_Executable_Output_Path}/${testname}
            ${MPI_EXEC_POSTFLAGS} )
    endif( MPI_FOUND )
endmacro( XdmfMPITest )

macro( XdmfUtilsTest testname )
    add_executable (${testname} ${ARGN} )
    target_link_libraries( ${testname} XdmfUtils )
    add_test( ${testname} ${_Executable_Output_Path}/${testname} )
endmacro()

XdmfSerialTest( XdmfTestHDFRoundtrip TestXdmfHDF.cxx )
#XdmfSerialTest( XdmfTestArray TestXdmfArray.cc)

if(XDMF_BUILD_MPI)
    XdmfMPITest( XdmfTestHDFMPI 4 XdmfHDFMPI.cxx )
endif(XDMF_BUILD_MPI)

if(XDMF_WRAP_PYTHON)
  if(PYTHON_EXECUTABLE)
    CONFIGURE_FILE(PythonTestDriver.cmake.in PythonTestDriver.cmake @ONLY)
    SET(PYTHON_TESTS
        TestXdmfArrayPython
        TestXdmfWriteGridsToFilePython
       )
    FOREACH(PYTHON_TEST ${PYTHON_TESTS})
      CONFIGURE_FILE(${PYTHON_TEST}.py ${PYTHON_TEST}.py COPYONLY)
      # Run all python tests by calling a cmake script that sets correct
      # environment variables --- python script to run and appropraite arguments
      # are set in ARGV and passed to the cmake script
      SET(ARGV ${CMAKE_CURRENT_BINARY_DIR}/${PYTHON_TEST}.py)
      ADD_TEST("${PYTHON_TEST}" ${CMAKE_COMMAND} "-D" ARGV=${ARGV} "-P" ${CMAKE_CURRENT_BINARY_DIR}/PythonTestDriver.cmake) 
    ENDFOREACH(PYTHON_TEST)
  endif(PYTHON_EXECUTABLE)
endif(XDMF_WRAP_PYTHON)
