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

XdmfSerialTest( XdmfHDFRoundtrip TestXdmfHDF.cxx )

if(XDMF_BUILD_MPI)
    XdmfMPITest( XdmfHDFMPI 4 XdmfHDFMPI.cxx )
endif(XDMF_BUILD_MPI)

