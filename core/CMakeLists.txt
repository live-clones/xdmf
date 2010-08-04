project(XdmfCore)
cmake_minimum_required(VERSION 2.6)

set(BUILD_SHARED_LIBS true)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMake)

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

set(XdmfCoreSources
	XdmfArray
	XdmfArrayType
	XdmfCoreItemFactory
	XdmfCoreReader
	XdmfHDF5Controller
	XdmfHDF5Writer
	XdmfInformation
	XdmfItem
	XdmfItemProperty
	XdmfSystemUtils
	XdmfVisitor
	XdmfWriter
)

add_library(XdmfCore ${XdmfCoreSources})
target_link_libraries(XdmfCore ${HDF5_LIBRARIES} ${LIBXML2_LIBRARIES})

option(XDMF_BUILD_TESTING OFF)
if(XDMF_BUILD_TESTING)
	enable_testing()
	include(CTest)
	add_subdirectory(tests)
endif(XDMF_BUILD_TESTING)

option(XDMF_WRAP_PYTHON OFF)
if(XDMF_WRAP_PYTHON)
	find_package(SWIG REQUIRED)
	include(${SWIG_USE_FILE})
	find_package(PythonLibs REQUIRED)
	include_directories(${PYTHON_INCLUDE_DIRS})
	set(CMAKE_SWIG_OUTDIR ${CMAKE_BINARY_DIR})
	set_source_files_properties(XdmfCore.i PROPERTIES CPLUSPLUS ON)
	swig_add_module(XdmfCore python XdmfCore.i)
	swig_link_libraries(XdmfCore XdmfCore ${PYTHON_LIBRARIES})
endif(XDMF_WRAP_PYTHON)

set(XdmfCore_INCLUDE_DIRS ${Boost_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR} ${HDF5_INCLUDE_DIR} ${LIBXML2_INCLUDE_DIR} ${PYTHON_INCLUDE_DIRS} CACHE INTERNAL "")