project(Xdmf2)
cmake_minimum_required(VERSION 2.6)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(BUILD_SHARED_LIBS true)

find_package(Boost REQUIRED)
if(Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIRS})
	message(${Boost_INCLUDE_DIRS})
endif(Boost_FOUND)

option(XDMF_WRAP_PYTHON ON)
if(XDMF_WRAP_PYTHON)
	find_package(PythonLibs REQUIRED)
	if(PYTHONLIBS_FOUND)
		include_directories(${PYTHON_INCLUDE_DIRS})
	endif(PYTHONLIBS_FOUND)
endif(XDMF_WRAP_PYTHON)

set(XdmfSources
	XdmfAttribute
	XdmfAttributeCenter
	XdmfAttributeType
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

if(XDMF_WRAP_PYTHON)
	add_library(_Xdmf MODULE XdmfPython)
	set_target_properties(_Xdmf PROPERTIES PREFIX "")
	target_link_libraries(_Xdmf Xdmf)
	if(WIN32 OR APPLE)
		target_link_libraries(_Xdmf ${PYTHON_LIBRARY})
	endif(WIN32 OR APPLE)
endif(XDMF_WRAP_PYTHON)

configure_file(Xdmf.py Xdmf.py COPYONLY)

option(BUILD_TESTING OFF)
if(BUILD_TESTING)
	enable_testing()
	add_subdirectory(tests)
	add_executable(XdmfTest test.cpp)
	target_link_libraries(XdmfTest Xdmf)
endif(BUILD_TESTING)
