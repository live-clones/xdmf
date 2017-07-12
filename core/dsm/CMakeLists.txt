project(XdmfDSM)

set(XdmfDSMLinkLibraries XdmfCore)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${Boost_INCLUDE_DIRS})

set(XdmfDSMSources
  XdmfHDF5ControllerDSM
  XdmfHDF5WriterDSM
  XdmfDSMCommMPI
  XdmfDSMBuffer
  XdmfDSMDescription
  XdmfDSMItemFactory
  XdmfDSMDriver)

if (XDMF_BUILD_DSM_THREADS)
  add_definitions(-DXDMF_BUILD_DSM_THREADS)
endif ()

if (XDMF_BUILD_DSM_THREADS)
  find_package(H5FDdsm REQUIRED NO_MODULE)
  if(H5FDdsm_FOUND)
    include_directories(${H5FDdsm_INCLUDE_DIR})
    get_property(h5fddsm_lib_location TARGET H5FDdsm PROPERTY LOCATION)
    set(H5FDdsm_LIBRARIES ${h5fddsm_lib_location})
    set(XdmfDSMLinkLibraries ${XdmfDSMLinkLibraries} ${H5FDdsm_LIBRARIES})
  else()
    message(SEND_ERROR
      "Cannot find HDF5 DSM!  Please set H5FDdsm_DIR or disable XDMF_BUILD_DSM_THREADS and configure again.")
  endif()
endif ()

# Set a variable if cray is being used
STRING(REGEX MATCH "aprun" IS_CRAY "${MPIEXEC}")

set(XDMF_DSM_IS_CRAY "${IS_CRAY}")
set(XDMF_DSM_IS_CRAY "${XDMF_DSM_IS_CRAY}" PARENT_SCOPE)

if (XDMF_DSM_IS_CRAY)
  add_definitions(-DXDMF_DSM_IS_CRAY)
endif (XDMF_DSM_IS_CRAY)

add_library(XdmfDSM ${XdmfDSMSources})
SET_TARGET_PROPERTIES(
  XdmfDSM PROPERTIES
  VERSION ${XDMF_VERSION}
  SOVERSION ${XDMF_MAJOR_VERSION}
  )
target_link_libraries(XdmfDSM ${XdmfDSMLinkLibraries})

if(WIN32)
  add_definitions(-D_HDF5USEDLL_ -D_HDF5USEHLDLL_)
  set_target_properties(XdmfDSM PROPERTIES
      DEFINE_SYMBOL XdmfDSM_EXPORTS)
endif()

if(XDMF_WRAP_JAVA)
  XDMF_SWIG_JAVA(XdmfDSM Compiled_XdmfCore_Jar)
endif()

if(XDMF_WRAP_PYTHON)
  if (NOT BUILD_SHARED_LIBS)
    message(FATAL_ERROR "Python Wrappers do not function"
                        " properly without shared libraries")
  endif (NOT BUILD_SHARED_LIBS)
  XDMF_SWIG_PYTHON(XdmfDSM XdmfCore)
endif()


file(GLOB_RECURSE
  XdmfDSMHeaders
  "*.hpp" "*.tpp" "*.i"
  "../../CMake/VersionSuite/*.hpp")
install(FILES ${XdmfDSMHeaders} DESTINATION include)
install(TARGETS XdmfDSM
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)

if(XDMF_BUILD_DSM_THREADS)
set(XdmfDSM_INCLUDE_DIRS
  ${Boost_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${HDF5_INCLUDE_DIR}
  ${H5FDdsm_INCLUDE_DIR}
  ${LIBXML2_INCLUDE_DIR}
  ${PYTHON_INCLUDE_DIRS}
  CACHE INTERNAL "")
else()
set(XdmfDSM_INCLUDE_DIRS
  ${Boost_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${HDF5_INCLUDE_DIR}
  ${LIBXML2_INCLUDE_DIR}
  ${PYTHON_INCLUDE_DIRS}
  CACHE INTERNAL "")
endif()

include_directories(${XdmfCore_INCLUDE_DIRS} ${XdmfDSM_INCLUDE_DIRS})

if(XDMF_BUILD_TESTING)
  add_subdirectory(tests)
endif()
