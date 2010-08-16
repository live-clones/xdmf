project(XdmfCore)
cmake_minimum_required(VERSION 2.6)

set(BUILD_SHARED_LIBS true)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 
SET(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib)
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
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

option(XDMF_WRAP_JAVA "Wrap Xdmf with Java" ON)
if(XDMF_WRAP_JAVA)
	find_package(SWIG REQUIRED)
	include(${SWIG_USE_FILE})
	find_package(Java REQUIRED)
	find_package(JNI REQUIRED)
	set(CMAKE_SWIG_FLAGS -v -make_default -package mil.army.arl.xdmf)
	set(XDMF_CORE_JAVA_PACKAGE_DIR mil/army/arl/xdmf)
	set(XDMF_CORE_JAVA_DIR ${CMAKE_CURRENT_BINARY_DIR}/${XDMF_CORE_JAVA_PACKAGE_DIR})
	file(MAKE_DIRECTORY ${XDMF_CORE_JAVA_DIR})
	set(CMAKE_SWIG_OUTDIR ${XDMF_CORE_JAVA_DIR})
	include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${JAVA_INCLUDE_PATH} ${JAVA_INCLUDE_PATH2})
	set_source_files_properties(XdmfCore.i PROPERTIES CPLUSPLUS ON)
	swig_add_module(XdmfCoreJava java XdmfCore.i)
	swig_link_libraries(XdmfCoreJava XdmfCore)
	set(XDMF_CORE_JAVA_JAR ${CMAKE_BINARY_DIR}/XdmfCore.jar CACHE INTERNAL "")
	add_custom_command(TARGET XdmfCoreJava
		POST_BUILD
		COMMAND ${JAVA_COMPILE} ARGS "${CMAKE_CURRENT_BINARY_DIR}/mil/army/arl/xdmf/*.java"
		COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_CURRENT_BINARY_DIR} ${JAVA_ARCHIVE} -cvf ${XDMF_CORE_JAVA_JAR} "mil/army/arl/xdmf/*.class")
endif(XDMF_WRAP_JAVA)

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
	install(FILES ${CMAKE_BINARY_DIR}/XdmfCore.py DESTINATION lib/python)
	install(TARGETS ${SWIG_MODULE_XdmfCore_REAL_NAME} DESTINATION lib/python)
endif(XDMF_WRAP_PYTHON)

file(GLOB XdmfCoreHeaders *.hpp)
install(FILES ${XdmfCoreHeaders} DESTINATION include)
install(TARGETS XdmfCore LIBRARY DESTINATION lib)

set(XdmfCore_INCLUDE_DIRS ${Boost_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR} ${HDF5_INCLUDE_DIR} ${LIBXML2_INCLUDE_DIR} ${PYTHON_INCLUDE_DIRS} CACHE INTERNAL "")
