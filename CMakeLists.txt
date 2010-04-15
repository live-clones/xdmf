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

option(XDMF_WRAP_PYTHON ON)
if(XDMF_WRAP_PYTHON)
	find_package(PythonLibs REQUIRED)
	if(PYTHONLIBS_FOUND)
		include_directories(${PYTHON_INCLUDE_DIRS})
	endif(PYTHONLIBS_FOUND)
endif(XDMF_WRAP_PYTHON)

set(XdmfSources
	XdmfArray
	XdmfAttribute
	XdmfAttributeCenter
	XdmfAttributeType
	XdmfDataItem
  XdmfDomain
	XdmfGeometry
	XdmfGeometryType
	XdmfGrid
	XdmfItem
	XdmfObject
	XdmfTopology
	XdmfTopologyType
	XdmfVisitor
)

add_library(Xdmf ${XdmfSources})
target_link_libraries(Xdmf ${HDF5_LIBRARIES} ${LIBXML2_LIBRARIES})

if(XDMF_WRAP_PYTHON)
	add_library(_Xdmf MODULE XdmfPython)
	set_target_properties(_Xdmf PROPERTIES PREFIX "")
	target_link_libraries(_Xdmf Xdmf)
	if(WIN32 OR APPLE)
		target_link_libraries(_Xdmf ${PYTHON_LIBRARY})
	endif(WIN32 OR APPLE)
endif(XDMF_WRAP_PYTHON)

configure_file(Xdmf.py Xdmf.py COPYONLY)

option(BUILD_DOCUMENTATION OFF)
if(BUILD_DOCUMENTATION)
	add_subdirectory(doc)
endif(BUILD_DOCUMENTATION)

option(BUILD_TESTING OFF)
if(BUILD_TESTING)
	enable_testing()
	include(CTest)
	add_subdirectory(tests)
endif(BUILD_TESTING)
