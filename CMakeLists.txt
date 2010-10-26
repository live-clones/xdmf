project(Xdmf)
cmake_minimum_required(VERSION 2.6)

set(BUILD_SHARED_LIBS true)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 
SET(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib)
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_MODULE_PATH 
	${CMAKE_SOURCE_DIR}/CMake
	${CMAKE_SOURCE_DIR}/CMake/TestingSuite)

# Should we build with testing
option(XDMF_BUILD_TESTING OFF)
if(XDMF_BUILD_TESTING)
	enable_testing()
	include(CTest)
endif(XDMF_BUILD_TESTING)

# Should we build with documentation
option(XDMF_BUILD_DOCUMENTATION OFF)
if(XDMF_BUILD_DOCUMENTATION)
	add_subdirectory(doc)
endif(XDMF_BUILD_DOCUMENTATION)

# Wrapper Setup
option(XDMF_WRAP_PYTHON OFF)
option(XDMF_WRAP_JAVA OFF)

# If we are wrapping either, we need swig
if(XDMF_WRAP_PYTHON OR XDMF_WRAP_JAVA)
	find_package(SWIG REQUIRED)
	include(${SWIG_USE_FILE})
endif(XDMF_WRAP_PYTHON OR XDMF_WRAP_JAVA)

# If we are wrapping python, let's include it in the top level
if(XDMF_WRAP_PYTHON)
	find_package(PythonInterp REQUIRED)
	find_package(PythonLibs REQUIRED)
	include_directories(${PYTHON_INCLUDE_DIRS})

	# A macro to swig and create the python files
	# Since we essentually do the same thing inside and outside core, it
	# would be easier as a macro
	# Parameters:
	#	python_name = the name of the intended target to be wrapped
	#	${ARGN} = any dependencies needed by the target
	# Output:
	#	Target Name = the output target name will have ${python_name} as its name
	MACRO(XDMF_SWIG_PYTHON python_name)
		set(CMAKE_SWIG_OUTDIR ${CMAKE_BINARY_DIR})
		set(CMAKE_SWIG_FLAGS "")
		set_source_files_properties(${python_name}.i PROPERTIES CPLUSPLUS ON)
		swig_add_module(${python_name} python ${python_name}.i)
		swig_link_libraries(${python_name} ${python_name} ${PYTHON_LIBRARIES})
		set_property(DIRECTORY APPEND PROPERTY
			ADDITIONAL_MAKE_CLEAN_FILES
			${CMAKE_BINARY_DIR}/${python_name}.pyc
		)	
		if("${ARGN}" STRGREATER "")
			add_dependencies(${python_name} "${ARGN}")
		endif("${ARGN}" STRGREATER "")
		install(FILES ${CMAKE_BINARY_DIR}/Xdmf.py DESTINATION lib/python)
		install(TARGETS ${SWIG_MODULE_Xdmf_REAL_NAME} DESTINATION lib/python)
	ENDMACRO(XDMF_SWIG_PYTHON)
endif(XDMF_WRAP_PYTHON)

# If we are wrapping java, let's include it in the top level
if(XDMF_WRAP_JAVA)
	find_package(Java REQUIRED)
	find_package(JNI REQUIRED)
	include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${JAVA_INCLUDE_PATH} ${JAVA_INCLUDE_PATH2})

	# A macro to swig and create the jar files
	# Since we essentually do the same thing inside and outside core, it
	# would be easier as a macro
	# Parameters:
	#	java_name = the name of the intended target to be wrapped
	#	${ARGN} = any dependencies needed by the target
	# Output:
	#	Target Name	= the output target name will have ${java_name}Java as its name
	#	Target Jar	= the output target jar will have ${java_name}Java.jar as its name
	MACRO(XDMF_SWIG_JAVA java_name)
		set(XDMF_JAVA_PACKAGE_DIR mil/army/arl/xdmf)
		set(XDMF_JAVA_DIR ${CMAKE_CURRENT_BINARY_DIR}/${XDMF_JAVA_PACKAGE_DIR})
		set(CMAKE_SWIG_OUTDIR ${XDMF_JAVA_DIR})
		set(CMAKE_SWIG_FLAGS -v -make_default -package mil.army.arl.xdmf)
		set_source_files_properties(${java_name}.i PROPERTIES CPLUSPLUS ON)
		swig_add_module(${java_name}Java java ${java_name}.i)
		swig_link_libraries(${java_name}Java ${java_name})	
		add_dependencies(${java_name}Java ${java_name}JavaDir)
		if("${ARGN}" STRGREATER "")
			add_dependencies(${java_name}Java "${ARGN}")
		endif("${ARGN}" STRGREATER "")
		add_custom_command(TARGET ${java_name}Java
			POST_BUILD 	
			COMMAND ${JAVA_COMPILE}
			ARGS 	 -cp "'${CMAKE_BINARY_DIR}/*'"
					 "${XDMF_JAVA_DIR}/*.java"
			COMMAND ${JAVA_ARCHIVE}
			ARGS	-cvf ${CMAKE_BINARY_DIR}/${java_name}.jar
					"${XDMF_JAVA_PACKAGE_DIR}/*.class"
		)
		add_custom_target(
			${java_name}JavaDir ALL
			DEPENDS ${XDMF_JAVA_DIR}
		)	
		add_custom_command(
			OUTPUT ${XDMF_JAVA_DIR}
			COMMAND ${CMAKE_COMMAND} -E make_directory
				${XDMF_JAVA_DIR}
		)
		set_property(DIRECTORY APPEND PROPERTY
			ADDITIONAL_MAKE_CLEAN_FILES
			${CMAKE_BINARY_DIR}/${java_name}.jar
			${XDMF_JAVA_DIR}
		)
	ENDMACRO(XDMF_SWIG_JAVA)
endif(XDMF_WRAP_JAVA)

add_subdirectory(core)
include_directories(${XdmfCore_INCLUDE_DIRS})

option(XDMF_BUILD_CORE_ONLY OFF)
if(NOT XDMF_BUILD_CORE_ONLY)
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
		XdmfGridCurvilinear
		XdmfGridRectilinear
		XdmfGridRegular
		XdmfGridUnstructured
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
	
	if(XDMF_WRAP_JAVA)
		XDMF_SWIG_JAVA(Xdmf XdmfCoreJava)
		set(XDMF_JAVA_JAR ${CMAKE_BINARY_DIR}/Xdmf.jar CACHE INTERNAL "")
	endif(XDMF_WRAP_JAVA)
	
	if(XDMF_WRAP_PYTHON)
		XDMF_SWIG_PYTHON(Xdmf XdmfCore)
	endif(XDMF_WRAP_PYTHON)
	
	if(XDMF_BUILD_TESTING)
		include_directories(tests/Cxx)
		add_subdirectory(tests)
	endif(XDMF_BUILD_TESTING)
	
	option(XDMF_BUILD_UTILS OFF)
	if(XDMF_BUILD_UTILS)
		add_subdirectory(utils)
	endif(XDMF_BUILD_UTILS)
		
	file(GLOB XdmfHeaders *.hpp)
	install(FILES ${XdmfHeaders} DESTINATION include)
	install(TARGETS Xdmf LIBRARY DESTINATION lib)
endif(NOT XDMF_BUILD_CORE_ONLY)
