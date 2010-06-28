project(Xdmf2)
cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake ${CMAKE_MODULE_PATH})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(BUILD_SHARED_LIBS true)

find_package(Boost REQUIRED)
if(Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIRS})
endif(Boost_FOUND)

find_package(HDF5 REQUIRED)
if(HDF5_FOUND)
	include_directories(${HDF5_INCLUDE_DIR})
endif(HDF5_FOUND)

find_package(LibXml2 REQUIRED)
if(LIBXML2_FOUND)
	include_directories(${LIBXML2_INCLUDE_DIR})
endif(LIBXML2_FOUND)

option(XDMF_WRAP_PYTHON OFF)
if(XDMF_WRAP_PYTHON)
	find_package(SWIG REQUIRED)
	include(${SWIG_USE_FILE})
	find_package(PythonLibs REQUIRED)
	include_directories(${PYTHON_INCLUDE_DIRS})
	set_source_files_properties(Xdmf.i PROPERTIES CPLUSPLUS ON)
	swig_add_module(Xdmf python Xdmf.i)
	swig_link_libraries(Xdmf Xdmf ${PYTHON_LIBRARIES})
endif(XDMF_WRAP_PYTHON)

set(XdmfCoreSources
	XdmfArray
	XdmfArrayType
	XdmfCoreItemFactory
	XdmfCoreReader
	XdmfDataItem
	XdmfHDF5Controller
	XdmfHDF5Writer
	XdmfItem
	XdmfItemProperty
	XdmfObject
	XdmfVisitor
	XdmfWriter
)

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

add_library(XdmfCore ${XdmfCoreSources})
target_link_libraries(XdmfCore ${HDF5_LIBRARIES} ${LIBXML2_LIBRARIES})
add_library(Xdmf ${XdmfSources})
target_link_libraries(Xdmf XdmfCore)

option(XDMF_BUILD_DOCUMENTATION OFF)
if(XDMF_BUILD_DOCUMENTATION)
	add_subdirectory(doc)
endif(XDMF_BUILD_DOCUMENTATION)

option(XDMF_BUILD_TESTING OFF)
if(XDMF_BUILD_TESTING)
	enable_testing()
	include(CTest)
	add_subdirectory(tests)
endif(XDMF_BUILD_TESTING)

option(XDMF_BUILD_UTILS OFF)
if(XDMF_BUILD_UTILS)
  add_subdirectory(utils)
endif(XDMF_BUILD_UTILS)
