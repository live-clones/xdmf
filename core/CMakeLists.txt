project(XdmfCore)
cmake_minimum_required(VERSION 2.6)

include(SetUpVersion)
if(VERSION_CONTROL_AUTOUPDATE OR 
    NOT EXISTS ${CMAKE_BINARY_DIR}/XdmfVersion.hpp)
  VersionCreate("Xdmf" "2" "XDMFCORE_EXPORT" "XdmfCore.hpp")
endif(VERSION_CONTROL_AUTOUPDATE OR 
    NOT EXISTS ${CMAKE_BINARY_DIR}/XdmfVersion.hpp)

set(BUILD_SHARED_LIBS true)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_BINARY_DIR})
set(CMAKE_SKIP_BUILD_RPATH  FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

option(XDMF_BUILD_DSM OFF)

mark_as_advanced(CLEAR Boost_INCLUDE_DIR)
find_package(Boost REQUIRED)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  mark_as_advanced(FORCE Boost_INCLUDE_DIR)
endif(Boost_FOUND)

mark_as_advanced(CLEAR HDF5_C_INCLUDE_DIR)
mark_as_advanced(CLEAR HDF5_hdf5_LIBRARY_DEBUG)
mark_as_advanced(CLEAR HDF5_hdf5_LIBRARY_RELEASE)
find_package(HDF5 REQUIRED)
if(HDF5_FOUND)
  include_directories(${HDF5_INCLUDE_DIRS})
  mark_as_advanced(FORCE HDF5_C_INCLUDE_DIR)
  mark_as_advanced(FORCE HDF5_hdf5_LIBRARY_DEBUG)
  mark_as_advanced(FORCE HDF5_hdf5_LIBRARY_RELEASE)
  if(HDF5_IS_PARALLEL)
    find_package(MPI REQUIRED)
    if(MPI_FOUND)
      include_directories(${MPI_INCLUDE_PATH})
      link_directories(${MPI_INCLUDE_PATH}/../lib)
    endif(MPI_FOUND)
  endif(HDF5_IS_PARALLEL)
  get_filename_component(HDF5_ROOT "${HDF5_INCLUDE_DIR}/../" REALPATH)
  set(HDF5_BINARY_DIRS ${HDF5_ROOT}/bin ${HDF5_ROOT}/dll)
  set(XDMF_LIBRARIES ${XDMF_LIBRARIES} ${HDF5_LIBRARIES})
  set(XDMF_HDF5_BINARY_DIRS ${HDF5_BINARY_DIRS} PARENT_SCOPE)
  set(XDMF_BINARIES ${XDMF_BINARIES} ${HDF5_BINARY_DIRS})
endif(HDF5_FOUND)

if(XDMF_BUILD_DSM)
  find_path(HDF5DSM_DIR include/H5FDdsm.h)
  if(HDF5DSM_DIR AND EXISTS ${HDF5DSM_DIR}/include/H5FDdsm.h)
    include_directories(${HDF5DSM_DIR}/include)
    find_library(HDF5DSM_LIBRARIES H5FDdsm ${HDF5DSM_DIR}/lib)
  else(HDF5DSM_DIR AND EXISTS ${HDF5DSM_DIR}/include/H5FDdsm.h)
    message(SEND_ERROR
      "Cannot find HDF5DSM!  Please set HDF5DSM_DIR and configure again.")
  endif(HDF5DSM_DIR AND EXISTS ${HDF5DSM_DIR}/include/H5FDdsm.h)

  find_package(MPI REQUIRED)
  if(MPI_FOUND)
    include_directories(${MPI_INCLUDE_PATH})
  endif(MPI_FOUND)
endif(XDMF_BUILD_DSM)

find_package(LibXml2 REQUIRED)
if(LIBXML2_FOUND)
  include_directories(${LIBXML2_INCLUDE_DIR})
  get_filename_component(LIBXML2_ROOT "${LIBXML2_INCLUDE_DIR}/../" REALPATH)
  set(LIBXML2_BINARY_DIRS ${LIBXML2_ROOT}/bin)
  set(LIBXML2_BINARY_DIRS ${LIBXML2_BINARY_DIRS} PARENT_SCOPE)
  set(XDMF_LIBRARIES ${XDMF_LIBRARIES} ${LIBXML2_LIBRARIES})
  set(XDMF_BINARIES ${XDMF_BINARIES} ${LIBXML2_BINARY_DIRS})
endif(LIBXML2_FOUND)

set(XdmfCoreSources
  XdmfArray
  XdmfArrayType
  XdmfCoreItemFactory
  XdmfCoreReader
  XdmfError
  XdmfHDF5Controller
  XdmfHDF5Writer
  XdmfHeavyDataController
  XdmfHeavyDataWriter
  XdmfInformation
  XdmfItem
  XdmfItemProperty
  XdmfSystemUtils
  ${CMAKE_BINARY_DIR}/XdmfVersion
  XdmfVisitor
  XdmfWriter)

if(XDMF_BUILD_DSM)
  set(XdmfCoreSources
    ${XdmfCoreSources}
    XdmfHDF5ControllerDSM
    XdmfHDF5WriterDSM)
endif(XDMF_BUILD_DSM)

add_library(XdmfCore ${XdmfCoreSources})
target_link_libraries(XdmfCore
  ${HDF5_hdf5_LIBRARY}
  ${HDF5DSM_LIBRARIES}
  ${LIBXML2_LIBRARIES})

if(WIN32)
  add_definitions(-D_HDF5USEDLL_ -D_HDF5USEHLDLL_)
  set_target_properties(XdmfCore PROPERTIES 
      DEFINE_SYMBOL XdmfCore_EXPORTS)
  if(NOT MSVC10)
	set_target_properties(XdmfCore PROPERTIES
      PREFIX ../
      IMPORT_PREFIX ../
      RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
      LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
      ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
  endif(NOT MSVC10)
endif(WIN32)

if(XDMF_WRAP_JAVA)
  XDMF_SWIG_JAVA(XdmfCore)
endif(XDMF_WRAP_JAVA)

if(XDMF_WRAP_PYTHON)
  XDMF_SWIG_PYTHON(XdmfCore)
endif(XDMF_WRAP_PYTHON)

if(WIN32)
    set(XDMFCORE_LIBRARY ${CMAKE_INSTALL_PREFIX}/lib/XdmfCore.lib PARENT_SCOPE)
endif(WIN32)

if(UNIX)
    set(XDMFCORE_LIBRARY ${CMAKE_INSTALL_PREFIX}/lib/libXdmfCore.so
        PARENT_SCOPE)
endif(UNIX)

if(APPLE)
    set(XDMFCORE_LIBRARY ${CMAKE_INSTALL_PREFIX}/lib/libXdmfCore.dylib
        PARENT_SCOPE)
endif(APPLE)

file(GLOB_RECURSE
  XdmfCoreHeaders
  "*.hpp" "*.tpp" "*.i"
  "../CMake/VersionSuite/*.hpp")
file(GLOB LokiHeaders loki/*.h)
install(FILES ${XdmfCoreHeaders} DESTINATION include)
install(FILES ${LokiHeaders} DESTINATION include/loki)
install(TARGETS XdmfCore
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)

set(XdmfCore_INCLUDE_DIRS
  ${Boost_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${HDF5_INCLUDE_DIR}
  ${LIBXML2_INCLUDE_DIR}
  ${PYTHON_INCLUDE_DIRS}
  CACHE INTERNAL "")

set(XDMF_LIBRARIES ${XDMF_LIBRARIES} ${CMAKE_INSTALL_PREFIX}/lib)
set(XDMF_BINARIES ${XDMF_BINARIES} ${CMAKE_INSTALL_PREFIX}/bin)

if(BUILD_TESTING)
  add_subdirectory(tests)
endif(BUILD_TESTING)

set(XDMF_LIBRARIES ${XDMF_LIBRARIES} PARENT_SCOPE)
set(XDMF_BINARIES ${XDMF_BINARIES} PARENT_SCOPE)

