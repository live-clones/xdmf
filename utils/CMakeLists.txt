include_directories(${CMAKE_CURRENT_SOURCE_DIR})

option(XDMF_BUILD_EXODUS_IO OFF)
option(XDMF_BUILD_PARTITIONER OFF)
option(XDMF_BUILD_FORTRAN OFF)
option(XDMF_BUILD_DSM OFF)


set(XdmfUtilsSources
  XdmfDiff
  XdmfTopologyConverter
  XdmfUtils)
set(XdmfUtilsLinkLibraries Xdmf)

if(XDMF_BUILD_FORTRAN)
  set(XdmfUtilsSources ${XdmfUtilsSources} XdmfFortran)
endif(XDMF_BUILD_FORTRAN)

if (XDMF_BUILD_DSM)
  add_definitions(-DXDMF_BUILD_DSM)
endif (XDMF_BUILD_DSM)

if(XDMF_BUILD_EXODUS_IO)
  set(XDMF_SWIG_FLAGS ${XDMF_SWIG_FLAGS} -DXDMF_BUILD_EXODUS_IO)
  find_package(Exodus REQUIRED)
  if(EXODUS_FOUND)
    include_directories(${EXODUS_INCLUDE_DIR} ${NetCDF_INCLUDE_DIR})
  endif(EXODUS_FOUND)
  set(XdmfUtilsSources ${XdmfUtilsSources} XdmfExodusReader XdmfExodusWriter)
  set(XdmfUtilsExecutables ${XdmfUtilsExecutables} XdmfExodusConverter)
  set(XdmfUtilsLinkLibraries
    ${XdmfUtilsLinkLibraries}
    ${EXODUS_LIBRARIES}
    ${NetCDF_LIBRARIES})
endif(XDMF_BUILD_EXODUS_IO)

if(XDMF_BUILD_PARTITIONER)
  set(XDMF_SWIG_FLAGS ${XDMF_SWIG_FLAGS} -DXDMF_BUILD_PARTITIONER)
  find_package(Metis REQUIRED)
  if(METIS_FOUND)
    include_directories(${METIS_INCLUDE_DIR})
  endif(METIS_FOUND)
  set(XdmfUtilsExecutables ${XdmfUtilsExecutables} XdmfPartitioner)
  set(XdmfUtilsSources ${XdmfUtilsSources} XdmfPartitioner)
  set(XdmfUtilsLinkLibraries ${XdmfUtilsLinkLibraries} ${METIS_LIBRARIES})
endif(XDMF_BUILD_PARTITIONER)

add_library(XdmfUtils ${XdmfUtilsSources})
target_link_libraries(XdmfUtils ${XdmfUtilsLinkLibraries})

if(WIN32)
  set_target_properties(${executable} PROPERTIES
      DEFINE_SYMBOL XdmfUtils_EXPORTS
	  PREFIX ../
	  IMPORT_PREFIX ../
	  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
	  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
	  ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif(WIN32)

foreach(executable ${XdmfUtilsExecutables})
  add_executable(${executable} ${executable})
  set_target_properties(${executable} PROPERTIES COMPILE_FLAGS -DBUILD_EXE)
  target_link_libraries(${executable} XdmfUtils)

  if(WIN32)
	  set_target_properties(${executable} PROPERTIES
	      DEFINE_SYMBOL XdmfUtils_EXPORTS)
      if(NOT MSVC10)
    	  set_target_properties(${executable} PROPERTIES
	    	  PREFIX ../
		      IMPORT_PREFIX ../
		      RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
		      LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
		      ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
      endif(NOT MSVC10)
  endif(WIN32)

endforeach(executable ${XdmfUtilsExecutables})

if(BUILD_TESTING)
  add_subdirectory(tests)
endif(BUILD_TESTING)

if(XDMF_WRAP_JAVA)
  XDMF_SWIG_JAVA(XdmfUtils Compiled_Xdmf_Jar)
endif(XDMF_WRAP_JAVA)


if(XDMF_WRAP_PYTHON)
  XDMF_SWIG_PYTHON(XdmfUtils Xdmf)
endif(XDMF_WRAP_PYTHON)

foreach(source ${XdmfUtilsSources})
  set(XdmfUtilsHeaders ${XdmfUtilsHeaders} ${source}.hpp)
endforeach(source ${XdmfUtilsSources})

if(XDMF_BUILD_FORTRAN)
  set(XdmfUtilsHeaders ${XdmfUtilsHeaders} Xdmf.f)
endif(XDMF_BUILD_FORTRAN)

install(FILES ${XdmfUtilsHeaders} DESTINATION include)
install(TARGETS XdmfUtils 
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)
install(TARGETS ${XdmfUtilsExecutables} 
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)
