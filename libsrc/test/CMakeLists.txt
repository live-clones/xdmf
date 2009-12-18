macro( XdmfSerialTest testname )
    add_executable( ${testname} ${ARGN} )
    target_link_libraries( ${testname} Xdmf )
    add_test( ${testname} ${EXECUTABLE_OUTPUT_PATH}/${testname} )
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
            ${EXECUTABLE_OUTPUT_PATH}/${testname}
            ${MPI_EXEC_POSTFLAGS} )
    endif( MPI_FOUND )
endmacro( XdmfMPITest )

macro( XdmfUtilsTest testname )
    add_executable (${testname} ${ARGN} )
    target_link_libraries( ${testname} XdmfUtils )
    add_test( ${testname} ${EXECUTABLE_OUTPUT_PATH}/${testname} )
endmacro()

XdmfSerialTest( XdmfTestHDFRoundtrip TestXdmfHDF.cxx )
#XdmfSerialTest( XdmfTestArray TestXdmfArray.cc)

if(XDMF_BUILD_MPI)
    XdmfMPITest( XdmfTestHDFMPI 4 XdmfHDFMPI.cxx )
endif(XDMF_BUILD_MPI)

if(XDMF_WRAP_PYTHON)
  if(PYTHON_EXECUTABLE)
    SET(PYTHON_TESTS
        TestXdmfArrayPython
	TestXdmfWriteGridsToFilePython	
       )
    FOREACH(PYTHON_TEST ${PYTHON_TESTS})
      SET(XDMF_CURRENT_PYTHON_TEST "${CMAKE_CURRENT_SOURCE_DIR}/${PYTHON_TEST}.py")
      CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/RunPythonTest.cmake.in" "${EXECUTABLE_OUTPUT_PATH}/${PYTHON_TEST}.cmake" @ONLY)
      ADD_TEST("${PYTHON_TEST}" ${CMAKE_COMMAND} "-P" "${EXECUTABLE_OUTPUT_PATH}/${PYTHON_TEST}.cmake") 
    ENDFOREACH(PYTHON_TEST)
  endif(PYTHON_EXECUTABLE)
endif(XDMF_WRAP_PYTHON)
