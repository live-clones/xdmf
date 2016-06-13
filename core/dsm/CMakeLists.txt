project(XdmfDSM)
cmake_minimum_required(VERSION 2.6)
if (POLICY CMP0015)
  cmake_policy(SET CMP0015 NEW)
endif (POLICY CMP0015)

set(XdmfDSMLinkLibraries XdmfCore)
set(XdmfDSMLinkLibraryDir )

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

option(XDMF_BUILD_DSM_THREADS ON)

mark_as_advanced(Boost_INCLUDE_DIR)
include_directories(${Boost_INCLUDE_DIRS})
mark_as_advanced(FORCE Boost_INCLUDE_DIR)

#mark_as_advanced(CLEAR HDF5_C_INCLUDE_DIR)
#mark_as_advanced(CLEAR HDF5_hdf5_LIBRARY_DEBUG)
#mark_as_advanced(CLEAR HDF5_hdf5_LIBRARY_RELEASE)
#find_package(HDF5 REQUIRED)
if(HDF5_FOUND)
  mark_as_advanced(FORCE HDF5_C_INCLUDE_DIR)
  mark_as_advanced(FORCE HDF5_hdf5_LIBRARY_DEBUG)
  mark_as_advanced(FORCE HDF5_hdf5_LIBRARY_RELEASE)
  include_directories(${HDF5_INCLUDE_DIRS})
  include_directories(${HDF5_C_INCLUDE_DIR})
  set(HDF5_LIBRARIES ${HDF5_hdf5_LIBRARY_RELEASE})
  get_filename_component(HDF5_LIBRARY_DIR "${HDF5_hdf5_LIBRARY_RELEASE}" PATH)
  set(XdmfDSMLinkLibraryDir ${XdmfDSMLinkLibraryDir} ${HDF5_LIBRARY_DIR})
  # FIXME: Would like to get this info from HDF5 so we don't have conflicting
  # MPI versions
  if(HDF5_IS_PARALLEL)
    # If MPI variables are already populated don't search again
    if (NOT MPI_FOUND)
    find_package(MPI REQUIRED)
    endif (NOT MPI_FOUND)
    if(MPI_FOUND)
      include_directories(${MPI_INCLUDE_PATH})
      set(XdmfDSMLinkLibraries ${XdmfDSMLinkLibraries} ${MPI_LIBRARY} ${MPI_EXTRA_LIBRARY})
    endif(MPI_FOUND)
  else(HDF5_IS_PARALLEL)
    message(SEND_ERROR
      "Error: XdmfDSM requires a parallel enabled build of HDF5.")
  endif(HDF5_IS_PARALLEL)
  set(XdmfDSMLinkLibraries ${XdmfDSMLinkLibraries} ${HDF5_LIBRARIES})
endif(HDF5_FOUND)


find_package(LibXml2 REQUIRED)
if(LIBXML2_FOUND)
  include_directories(${LIBXML2_INCLUDE_DIR})
  set(XdmfDSMLinkLibraries ${XdmfDSMLinkLibraries} ${LIBXML2_LIBRARIES})
endif(LIBXML2_FOUND)

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
endif (XDMF_BUILD_DSM_THREADS)

if (XDMF_BUILD_DSM_THREADS)
  find_package(H5FDdsm REQUIRED NO_MODULE)
  if(H5FDdsm_FOUND)
    include_directories(${H5FDdsm_INCLUDE_DIR})
    get_property(h5fddsm_lib_location TARGET H5FDdsm PROPERTY LOCATION)
    set(H5FDdsm_LIBRARIES ${h5fddsm_lib_location})
    set(XdmfDSMLinkLibraries ${XdmfDSMLinkLibraries} ${H5FDdsm_LIBRARIES})
  else(H5FDdsm_FOUND)
    message(SEND_ERROR
      "Cannot find HDF5 DSM!  Please set H5FDdsm_DIR or disable XDMF_BUILD_DSM_THREADS and configure again.")
  endif(H5FDdsm_FOUND)
endif (XDMF_BUILD_DSM_THREADS)

# Set a variable if cray is being used
STRING(REGEX MATCH "aprun" IS_CRAY "${MPIEXEC}")

set(XDMF_DSM_IS_CRAY "${IS_CRAY}")
set(XDMF_DSM_IS_CRAY "${XDMF_DSM_IS_CRAY}" PARENT_SCOPE)

if (XDMF_DSM_IS_CRAY)
  add_definitions(-DXDMF_DSM_IS_CRAY)
endif (XDMF_DSM_IS_CRAY)

add_library(XdmfDSMObjects OBJECT ${XdmfDSMSources})
add_library(XdmfDSM $<TARGET_OBJECTS:XdmfDSMObjects>)
if (BUILD_SHARED_LIBS)
  add_library(XdmfDSM_Static STATIC $<TARGET_OBJECTS:XdmfDSMObjects>)
  if (UNIX)
    set_target_properties(XdmfDSM_Static PROPERTIES
      OUTPUT_NAME "XdmfDSM")
  endif (UNIX)
endif (BUILD_SHARED_LIBS)
#add_library(XdmfDSM ${XdmfDSMSources})
#if (BUILD_SHARED_LIBS)
#  add_library(XdmfDSM_Static STATIC ${XdmfDSMSources})
#  if (UNIX)
#    set_target_properties(XdmfDSM_Static PROPERTIES
#      OUTPUT_NAME "XdmfDSM")
#  endif (UNIX)
#endif (BUILD_SHARED_LIBS)
link_directories(${XDMF_LIBRARY_DIRS} ${XdmfDSMLinkLibraryDir})
target_link_libraries(XdmfDSM ${XdmfDSMLinkLibraries})
if (BUILD_SHARED_LIBS)
  target_link_libraries(XdmfDSM_Static ${XdmfDSMLinkLibraries})
endif (BUILD_SHARED_LIBS)

if(WIN32)
  add_definitions(-D_HDF5USEDLL_ -D_HDF5USEHLDLL_)
  set_target_properties(XdmfDSM PROPERTIES
      DEFINE_SYMBOL XdmfDSM_EXPORTS)
  if (BUILD_SHARED_LIBS)
    set_target_properties(XdmfDSM_Static PROPERTIES
        DEFINE_SYMBOL XdmfDSM_EXPORTS)
  endif (BUILD_SHARED_LIBS)
  if(NOT MSVC10)
        set_target_properties(XdmfDSM PROPERTIES
      PREFIX ../
      IMPORT_PREFIX ../
      RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
      LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
      ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
  if (BUILD_SHARED_LIBS)
      set_target_properties(XdmfDSM_Static PROPERTIES
      PREFIX ../
      IMPORT_PREFIX ../
      RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
      LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
      ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
  endif (BUILD_SHARED_LIBS)
  endif(NOT MSVC10)
endif(WIN32)

if(XDMF_WRAP_JAVA)
  XDMF_SWIG_JAVA(XdmfDSM Compiled_XdmfCore_Jar)
endif(XDMF_WRAP_JAVA)

if(XDMF_WRAP_PYTHON)
  if (NOT BUILD_SHARED_LIBS)
    message(FATAL_ERROR "Python Wrappers do not function"
                        " properly without shared libraries")
  endif (NOT BUILD_SHARED_LIBS)
  XDMF_SWIG_PYTHON(XdmfDSM XdmfCore)
endif(XDMF_WRAP_PYTHON)


set(XDMF_LIBRARY_DIRS ${XDMF_LIBRARY_DIRS} PARENT_SCOPE)

if(WIN32)
    set(XDMFDSM_LIBRARY ${CMAKE_INSTALL_PREFIX}/lib/XdmfDSM.lib PARENT_SCOPE)
endif(WIN32)

if(UNIX)
    if (BUILD_SHARED_LIBS)
      set(XDMFDSM_LIBRARY ${CMAKE_INSTALL_PREFIX}/lib/libXdmfDSM.so
          PARENT_SCOPE)
    else (BUILD_SHARED_LIBS)
      set(XDMFDSM_LIBRARY ${CMAKE_INSTALL_PREFIX}/lib/libXdmfDSM.a
          PARENT_SCOPE)
    endif (BUILD_SHARED_LIBS)
endif(UNIX)

if(APPLE)
    set(XDMFDSM_LIBRARY ${CMAKE_INSTALL_PREFIX}/lib/libXdmfDSM.dylib
        PARENT_SCOPE)
endif(APPLE)

file(GLOB_RECURSE
  XdmfDSMHeaders
  "*.hpp" "*.tpp" "*.i"
  "../../CMake/VersionSuite/*.hpp")
install(FILES ${XdmfDSMHeaders} DESTINATION include)
install(TARGETS XdmfDSM
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)
if (BUILD_SHARED_LIBS)
  install(TARGETS XdmfDSM_Static
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)
endif (BUILD_SHARED_LIBS)

if(XDMF_BUILD_DSM_THREADS)
set(XdmfDSM_INCLUDE_DIRS
  ${Boost_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${HDF5_INCLUDE_DIR}
  ${H5FDdsm_INCLUDE_DIR}
  ${LIBXML2_INCLUDE_DIR}
  ${PYTHON_INCLUDE_DIRS}
  CACHE INTERNAL "")
else(XDMF_BUILD_DSM_THREADS)
set(XdmfDSM_INCLUDE_DIRS
  ${Boost_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${HDF5_INCLUDE_DIR}
  ${LIBXML2_INCLUDE_DIR}
  ${PYTHON_INCLUDE_DIRS}
  CACHE INTERNAL "")
endif(XDMF_BUILD_DSM_THREADS)

include_directories(${XdmfCore_INCLUDE_DIRS} ${XdmfDSM_INCLUDE_DIRS})

if(BUILD_TESTING)
  add_subdirectory(tests)
endif(BUILD_TESTING)
