project(XdmfCore)
cmake_minimum_required(VERSION 2.6)

include(SetUpVersion)
if(VERSION_CONTROL_AUTOUPDATE)
        VersionCreate("Xdmf" "2" "XDMFCORE_EXPORT" "XdmfCore.hpp")
endif(VERSION_CONTROL_AUTOUPDATE)

set(BUILD_SHARED_LIBS true)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 
SET(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib)
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

option(XDMF_BUILD_DSM OFF)

find_package(Boost REQUIRED)
if(Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIRS})
    set(XDMF_BOOST_INCLUDE_DIRS ${Boost_INCLUDE_DIRS})
    get_filename_component(Boost_ROOT "${Boost_INCLUDE_DIRS}" REALPATH)
    set(XDMF_BOOST_ROOT_DIR ${Boost_ROOT})
endif(Boost_FOUND)

find_package(HDF5 REQUIRED)
if(HDF5_FOUND)
	include_directories(${HDF5_INCLUDE_DIR})
	if(HDF5_IS_PARALLEL)
		find_package(MPI REQUIRED)
		if(MPI_FOUND)
			include_directories(${MPI_INCLUDE_PATH})
		endif(MPI_FOUND)
	endif(HDF5_IS_PARALLEL)
    get_filename_component(HDF5_ROOT "${HDF5_INCLUDE_DIR}/../" REALPATH)
    set(HDF5_BINARY_DIRS ${HDF5_ROOT}/bin ${HDF5_ROOT}/dll)
    set(XDMF_HDF5_ROOT_DIR ${HDF5_ROOT})
    set(XDMF_HDF5_INCLUDE_DIR ${HDF5_INCLUDE_DIR})
    set(XDMF_HDF5_LIBRARIES ${HDF5_LIBRARIES})
    set(XDMF_LIBRARIES ${XDMF_LIBRARIES} ${HDF5_LIBRARIES})
    set(XDMF_HDF5_BINARY_DIRS ${HDF5_BINARY_DIRS})
    set(XDMF_BINARIES ${XDMF_BINARIES} ${HDF5_BINARY_DIRS})
endif(HDF5_FOUND)

if(XDMF_BUILD_DSM)
	find_path(HDF5DSM_DIR include/H5FDdsm.h)
	if(HDF5DSM_DIR AND EXISTS ${HDF5DSM_DIR}/include/H5FDdsm.h)
	  include_directories(${HDF5DSM_DIR}/include)
	  find_library(HDF5DSM_LIBRARIES H5FDdsm ${HDF5DSM_DIR}/lib)
	else(HDF5DSM_DIR AND EXISTS ${HDF5DSM_DIR}/include/H5FDdsm.h)
	  message(SEND_ERROR "Cannot find HDF5DSM!  Please set HDF5DSM_DIR and configure again.")
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
    set(XDMF_LIBXML2_ROOT_DIR ${LIBXML2_ROOT})
    set(XDMF_LIBXML2_INCLUDE_DIR ${LIBXML2_INCLUDE_DIR})
    set(XDMF_LIBXML2_LIBRARIES ${LIBXML2_LIBRARIES})
    set(XDMF_LIBRARIES ${XDMF_LIBRARIES} ${LIBXML2_LIBRARIES})
    set(XDMF_LIBXML2_BINARY_DIRS ${LIBXML2_BINARY_DIRS})
    set(XDMF_BINARIES ${XDMF_BINARIES} ${LIBXML2_BINARY_DIRS})
endif(LIBXML2_FOUND)

set(XdmfCoreSources
	XdmfArray
	XdmfArrayType
	XdmfCoreItemFactory
	XdmfCoreReader
	XdmfHDF5Controller
	XdmfHDF5Writer
	XdmfHeavyDataController
	XdmfHeavyDataWriter
	XdmfInformation
	XdmfItem
	XdmfItemProperty
	XdmfSystemUtils
    XdmfVersion
	XdmfVisitor
	XdmfWriter
)

if(XDMF_BUILD_DSM)
	set(XdmfCoreSources 
		${XdmfCoreSources}
		XdmfHDF5ControllerDSM 
		XdmfHDF5WriterDSM)
endif(XDMF_BUILD_DSM)

add_library(XdmfCore ${XdmfCoreSources})
target_link_libraries(XdmfCore ${HDF5_LIBRARIES} ${HDF5DSM_LIBRARIES} ${LIBXML2_LIBRARIES})

if(WIN32)
    add_definitions(-D_HDF5USEDLL_ -D_HDF5USEHLDLL_)
    set_target_properties(XdmfCore PROPERTIES 
            DEFINE_SYMBOL XdmfCore_EXPORTS
            PREFIX ../
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
            LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
            ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif(WIN32)

if(XDMF_WRAP_JAVA)
	XDMF_SWIG_JAVA(XdmfCore)
endif(XDMF_WRAP_JAVA)

if(XDMF_WRAP_PYTHON)
	XDMF_SWIG_PYTHON(XdmfCore)
endif(XDMF_WRAP_PYTHON)

if(WIN32)
    set(XDMFCORE_LIBRARY_CONFIG ${CMAKE_INSTALL_PREFIX}/lib/XdmfCore.lib)
else(WIN32)
    set(XDMFCORE_LIBRARY_CONFIG ${CMAKE_INSTALL_PREFIX}/lib/libXdmfCore.so)
endif(WIN32)

file(GLOB_RECURSE XdmfCoreHeaders "*.hpp" "*.tpp" "*.i" "../CMake/VersionSuite/*.hpp")
file(GLOB LokiHeaders loki/*.h)
install(FILES ${XdmfCoreHeaders} DESTINATION include)
install(FILES ${LokiHeaders} DESTINATION include/loki)
install(TARGETS XdmfCore 
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

set(XdmfCore_INCLUDE_DIRS ${Boost_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR} ${HDF5_INCLUDE_DIR} ${LIBXML2_INCLUDE_DIR} ${PYTHON_INCLUDE_DIRS} CACHE INTERNAL "")

if(BUILD_TESTING)
	add_subdirectory(tests)
endif(BUILD_TESTING)

# Since these are not set as of yet, we don't want to remove them from the
# config file
set(XDMF_LIBRARIES ${XDMF_LIBRARIES} ${CMAKE_INSTALL_PREFIX}/lib)
set(XDMF_BINARIES ${XDMF_BINARIES} ${CMAKE_INSTALL_PREFIX}/bin)
set(XDMF_LIBRARY_CONFIG "@XDMF_LIBRARY_CONFIG@")
set(XDMF_JAVA_JAR_CONFIG "@XDMF_JAVA_JAR_CONFIG@")
set(XDMF_PYTHON_CONFIG "@XDMF_PYTHON_CONFIG@")
configure_file(${CMAKE_SOURCE_DIR}/XdmfConfig.cmake.in
                ${CMAKE_BINARY_DIR}/XdmfConfig.cmake.in @ONLY)

