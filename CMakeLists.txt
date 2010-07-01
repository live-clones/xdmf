project(Xdmf)
cmake_minimum_required(VERSION 2.6)

set(BUILD_SHARED_LIBS true)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMake ${CMAKE_CURRENT_SOURCE_DIR}/core/CMake)

add_subdirectory(core)
include_directories(${XdmfCore_INCLUDE_DIRS})

set(XdmfSources
	XdmfAttribute
	XdmfAttributeCenter
	XdmfAttributeType
	XdmfDomain
	XdmfGeometry
	XdmfGeometryType
	XdmfGrid
	XdmfGridCollection
	XdmfGridCollectionType
	XdmfItemFactory
	XdmfReader
	XdmfSet
	XdmfSetType
	XdmfTopology
	XdmfTopologyType
)

add_library(Xdmf ${XdmfSources})
target_link_libraries(Xdmf XdmfCore)

option(XDMF_BUILD_DOCUMENTATION OFF)
if(XDMF_BUILD_DOCUMENTATION)
	add_subdirectory(doc)
endif(XDMF_BUILD_DOCUMENTATION)

if(XDMF_BUILD_TESTING)
	enable_testing()
	add_subdirectory(tests)
endif(XDMF_BUILD_TESTING)

if(XDMF_WRAP_PYTHON)
	find_package(SWIG REQUIRED)
	include(${SWIG_USE_FILE})
	set(CMAKE_SWIG_OUTDIR ${CMAKE_BINARY_DIR})
	set_source_files_properties(Xdmf.i PROPERTIES CPLUSPLUS ON)
	swig_add_module(Xdmf python Xdmf.i)
	swig_link_libraries(Xdmf Xdmf ${PYTHON_LIBRARIES})
endif(XDMF_WRAP_PYTHON)

option(XDMF_BUILD_UTILS OFF)
if(XDMF_BUILD_UTILS)
	add_subdirectory(utils)
endif(XDMF_BUILD_UTILS)


