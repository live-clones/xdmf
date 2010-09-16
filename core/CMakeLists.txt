project(XdmfCore)
cmake_minimum_required(VERSION 2.6)

set(BUILD_SHARED_LIBS true)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 
SET(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib)
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

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

if(XDMF_WRAP_JAVA)
	XDMF_SWIG_JAVA(XdmfCore)
	set(XDMF_CORE_JAVA_JAR ${CMAKE_BINARY_DIR}/XdmfCore.jar CACHE INTERNAL "")
endif(XDMF_WRAP_JAVA)

if(XDMF_WRAP_PYTHON)
	XDMF_SWIG_PYTHON(XdmfCore)
endif(XDMF_WRAP_PYTHON)

file(GLOB XdmfCoreHeaders *.hpp)
install(FILES ${XdmfCoreHeaders} DESTINATION include)
install(TARGETS XdmfCore LIBRARY DESTINATION lib)

set(XdmfCore_INCLUDE_DIRS ${Boost_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR} ${HDF5_INCLUDE_DIR} ${LIBXML2_INCLUDE_DIR} ${PYTHON_INCLUDE_DIRS} CACHE INTERNAL "")

if(BUILD_TESTING)
	add_subdirectory(tests)
endif(BUILD_TESTING)
