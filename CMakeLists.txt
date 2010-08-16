project(Xdmf)
cmake_minimum_required(VERSION 2.6)

set(BUILD_SHARED_LIBS true)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 
SET(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib)
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMake ${CMAKE_CURRENT_SOURCE_DIR}/core/CMake)

add_subdirectory(core)
include_directories(${XdmfCore_INCLUDE_DIRS})
include_directories(tests/Cxx)

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
	XdmfMap
	XdmfReader
	XdmfSet
	XdmfSetType
	XdmfTime
	XdmfTopology
	XdmfTopologyType
)

add_library(Xdmf ${XdmfSources})
target_link_libraries(Xdmf XdmfCore)

option(XDMF_BUILD_DOCUMENTATION OFF)
if(XDMF_BUILD_DOCUMENTATION)
	add_subdirectory(doc)
endif(XDMF_BUILD_DOCUMENTATION)

if(XDMF_WRAP_PYTHON)
	find_package(SWIG REQUIRED)
	include(${SWIG_USE_FILE})
	find_package(PythonLibs REQUIRED)
	include_directories(${PYTHON_INCLUDE_DIRS})
	set(CMAKE_SWIG_OUTDIR ${CMAKE_BINARY_DIR})
	set_source_files_properties(Xdmf.i PROPERTIES CPLUSPLUS ON)
	swig_add_module(Xdmf python Xdmf.i)
	swig_link_libraries(Xdmf Xdmf ${PYTHON_LIBRARIES})
	install(FILES ${CMAKE_BINARY_DIR}/Xdmf.py DESTINATION lib/python)
	install(TARGETS ${SWIG_MODULE_Xdmf_REAL_NAME} DESTINATION lib/python)
endif(XDMF_WRAP_PYTHON)

if(XDMF_WRAP_JAVA)
	find_package(SWIG REQUIRED)
	include(${SWIG_USE_FILE})
	set(CMAKE_SWIG_OUTDIR ${CMAKE_BINARY_DIR})
        set_source_files_properties(Xdmf.i PROPERTIES CPLUSPLUS ON)
        find_package(Java REQUIRED)
        find_package(JNI REQUIRED)
        set(CMAKE_SWIG_FLAGS -v -make_default -package mil.army.arl.xdmf)
        set(XDMF_JAVA_PACKAGE_DIR mil/army/arl/xdmf)
        set(XDMF_JAVA_DIR ${CMAKE_CURRENT_BINARY_DIR}/${XDMF_JAVA_PACKAGE_DIR})
        file(MAKE_DIRECTORY ${XDMF_JAVA_DIR})
        set(CMAKE_SWIG_OUTDIR ${XDMF_JAVA_DIR})
        include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${JAVA_INCLUDE_PATH} ${JAVA_INCLUDE_PATH2})
        swig_add_module(XdmfJava java Xdmf.i)
        swig_link_libraries(XdmfJava Xdmf)
        add_dependencies(XdmfJava XdmfCoreJava)
        set(XDMF_JAVA_JAR ${CMAKE_BINARY_DIR}/Xdmf.jar CACHE INTERNAL "")
        add_custom_command(TARGET XdmfJava
                           POST_BUILD
                           DEPENDS ${XDMF_CORE_JAVA_JAR}
                           COMMAND ${JAVA_COMPILE} ARGS -cp ${XDMF_CORE_JAVA_JAR} "${CMAKE_CURRENT_BINARY_DIR}/mil/army/arl/xdmf/*.java"
                           COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_CURRENT_BINARY_DIR} ${JAVA_ARCHIVE} -cvf ${XDMF_JAVA_JAR} "mil/army/arl/xdmf/*.class")
endif(XDMF_WRAP_JAVA)

option(XDMF_BUILD_UTILS OFF)
if(XDMF_BUILD_UTILS)
	add_subdirectory(utils)
endif(XDMF_BUILD_UTILS)

file(GLOB XdmfHeaders *.hpp)
install(FILES ${XdmfHeaders} DESTINATION include)
install(TARGETS Xdmf LIBRARY DESTINATION lib)

if(XDMF_BUILD_TESTING)
	enable_testing()
	add_subdirectory(tests)
endif(XDMF_BUILD_TESTING)

