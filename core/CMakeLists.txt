project(XdmfCore)
cmake_minimum_required(VERSION 2.6)

include(SetUpVersion)
if(VERSION_CONTROL_AUTOUPDATE)
        VersionMajorSet("2")
        VersionCalculate()
        VersionWrite("Xdmf")
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
endif(Boost_FOUND)

find_package(HDF5 REQUIRED)
if(HDF5_FOUND)
	include_directories(${HDF5_INCLUDE_DIR})	
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

if(XDMF_WRAP_JAVA)
	XDMF_SWIG_JAVA(XdmfCore)
endif(XDMF_WRAP_JAVA)

if(XDMF_WRAP_PYTHON)
	XDMF_SWIG_PYTHON(XdmfCore)
endif(XDMF_WRAP_PYTHON)

file(GLOB XdmfCoreHeaders *.hpp)
file(GLOB LokiHeaders loki/*.h)
install(FILES ${XdmfCoreHeaders} DESTINATION include)
install(FILES ${LokiHeaders} DESTINATION include/loki)
install(TARGETS XdmfCore LIBRARY DESTINATION lib)

set(XdmfCore_INCLUDE_DIRS ${Boost_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR} ${HDF5_INCLUDE_DIR} ${LIBXML2_INCLUDE_DIR} ${PYTHON_INCLUDE_DIRS} CACHE INTERNAL "")

if(BUILD_TESTING)
	add_subdirectory(tests)
endif(BUILD_TESTING)
