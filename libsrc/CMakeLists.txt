# We Need
# 	HDF5 Include
# 	HDF5 Libs
# 	Expat Includes
# 	Expat Libs
# We'd like
#	NDGM
# 	Python Include
# 	PythonConfig Include
# 	Python Libs
# 	TCL Include
# 	TCL Libs

SOURCE_FILES( XdmfFormatSource
	XdmfArray
	XdmfCharArray
	XdmfDOM
	XdmfDataDesc
	XdmfExpression
	XdmfExprLex
	XdmfExprYacc
	XdmfFormat
	XdmfFormatMulti
	XdmfFormatHDF
	XdmfFormatXML
	XdmfLlist
	XdmfHeavyData
	XdmfHDF
	XdmfHDFSupport
	XdmfLightData
	XdmfObject
	XdmfParameter
	XdmfXNode
	XdmfTransform
	XdmfTree
	XdmfXMLParser
)

SOURCE_FILES( XdmfModelSource
	XdmfAttribute
	XdmfGeometry
	XdmfGrid
	XdmfTopology
)

SOURCE_FILES( XdmfNdgmSource
	H5FDndgm
	XdmfNDGM
	XdmfRuntime
)

SOURCE_FILES( XdmfPythonSource
	XdmfPython
)
SOURCE_FILES( XdmfTclSource
	XdmfTcl
)

SET(ARCH_TO_BUILD Linux CACHE STRING "Host Arcitecture : Linux IRIXN32 IRIX64 AIX CYGWIN")
# Allow the user to customize their build with some local options
#
INCLUDE (${VTK_BINARY_DIR}/Xdmf/LocalUserOptions.cmake OPTIONAL) 
INCLUDE (${VTK_SOURCE_DIR}/Xdmf/LocalUserOptions.cmake OPTIONAL) 
ADD_DEFINITIONS(-D${ARCH_TO_BUILD})
# If we alread have ICE everything is in
# one place
OPTION(ICE_INSTALLED "ICE Support Already Installed" ON)
IF(ICE_INSTALLED)
	FIND_PATH(ICE_ROOT ice
		/unsupported/Ice
		/ha/cta/unsupported/Ice
		/vis/clarke/Linux
		/cygdrive/f/cygwin/home/Administrator/IceDevel02
		/usr/local
		)
	SET(ICE_VERSION 03.03 CACHE STRING "Current Version of Ice")
	INCLUDE_DIRECTORIES(${ICE_ROOT}/Release${ICE_VERSION}/${ARCH_TO_BUILD}/include)
	INCLUDE_DIRECTORIES(${ICE_ROOT}/Release${ICE_VERSION}/include)
	INCLUDE_DIRECTORIES(${ICE_ROOT}/Support${ICE_VERSION}/${ARCH_TO_BUILD}/include)
	INCLUDE_DIRECTORIES(${ICE_ROOT}/Support${ICE_VERSION}/include)
	LINK_DIRECTORIES(${ICE_ROOT}/Release${ICE_VERSION}/${ARCH_TO_BUILD}/lib)
	LINK_DIRECTORIES(${ICE_ROOT}/Support${ICE_VERSION}/${ARCH_TO_BUILD}/lib)
ELSE(ICE_INSTALLED)
	FIND_PATH(HDF5_INCLUDE_PATH hdf5.h
		/usr/local/include
		/usr/include
		)
	FIND_PATH(HDF5_CONFIG_INCLUDE_PATH H5pubconf.h
		/usr/local/include
		/usr/include
		)
	FIND_LIBRARY(HDF5_LIBRARY hdf5 PATHS
		/usr/local/lib
		/usr/lib
		)
	FIND_PATH(EXPAT_INCLUDE_PATH expat.h
		/usr/local/include
		/usr/include
		)
	FIND_LIBRARY(EXPAT_LIBRARY expat PATHS
		/usr/local/lib
		/usr/lib
		)
	FIND_PATH(ICE_INCLUDE_PATH ice.h
		/usr/local/include
		/usr/include
		)
	FIND_PATH(ICE_CONFIG_INCLUDE_PATH IceConfig.h
		/usr/local/include
		/usr/include
		)
	INCLUDE_DIRECTORIES(${ICE_INCLUDE_PATH})
	INCLUDE_DIRECTORIES(${ICE_CONFIG_INCLUDE_PATH})
	INCLUDE_DIRECTORIES(${HDF5_INCLUDE_PATH})
	INCLUDE_DIRECTORIES(${HDF5_CONFIG_INCLUDE_PATH})
	INCLUDE_DIRECTORIES(${EXPAT_INCLUDE_PATH})
	LINK_DIRECTORIES(${HDF5_LIBRARY})
	LINK_DIRECTORIES(${EXPAT_LIBRARY})
ENDIF(ICE_INSTALLED)


# if we are wrapping into Python then add the library and extra 
# source files
#
OPTION(XDMF_BUILD_PYTHON "Wrap classes into the Python interpreted language")
IF (XDMF_BUILD_PYTHON)
  INCLUDE (${CMAKE_ROOT}/Modules/FindPythonLibs.cmake)
  FIND_PATH(PYTHON_INCLUDE_PATH Python.h
	${ICE_ROOT}/Support${ICE_VERSION}/${ARCH_TO_BUILD}/include/python2.2
	${PYTHON_INCLUDE_PATH}
	${PYTHON_INCLUDE_PATH}/..
	)
  INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})
  FIND_PATH(PYCONFIG_INCLUDE_PATH pyconfig.h
	${ICE_ROOT}/Support${ICE_VERSION}/${ARCH_TO_BUILD}/include/python2.2
	${PYTHON_INCLUDE_PATH}
	${PYTHON_INCLUDE_PATH}/..
	)
  INCLUDE_DIRECTORIES(${PYCONFIG_INCLUDE_PATH})
  ADD_LIBRARY(XdmfPython SHARED XdmfPythonSource)
  TARGET_LINK_LIBRARIES (XdmfPython 
			 Xdmf
                         ${PYTHON_LIBRARY})
  IF(ICE_INSTALLED)
	INSTALL_TARGETS(${ICE_ROOT}/Release${ICE_VERSION}/${ARCH_TO_BUILD}/lib XdmfPython)
  ENDIF(ICE_INSTALLED)
ENDIF (XDMF_BUILD_PYTHON)
# if we are wrapping into Tcl then add the library and extra 
# source files
#
OPTION(XDMF_BUILD_TCL "Wrap classes into the Tcl interpreted language")
IF (XDMF_BUILD_TCL)
  INCLUDE (${CMAKE_ROOT}/Modules/FindTCL.cmake)
  INCLUDE_DIRECTORIES(${TCL_INCLUDE_PATH})
  ADD_LIBRARY(XdmfTcl SHARED XdmfTclSource)
  TARGET_LINK_LIBRARIES (XdmfTcl 
			 Xdmf
                         ${TCL_LIBRARY})
  IF(ICE_INSTALLED)
	INSTALL_TARGETS(${ICE_ROOT}/Release${ICE_VERSION}/${ARCH_TO_BUILD}/lib XdmfTcl)
  ENDIF(ICE_INSTALLED)
ENDIF (XDMF_BUILD_TCL)
# if we are wrapping into Java then add the library and extra 
# source files
#
OPTION(XDMF_BUILD_JAVA "Wrap classes into the Java interpreted language")
IF (XDMF_BUILD_JAVA)
  INCLUDE (${CMAKE_ROOT}/Modules/FindJNI.cmake)
  INCLUDE_DIRECTORIES(${JAVA_INCLUDE_PATH})
  ADD_LIBRARY(XdmfJava SHARED XdmfJavaSource)
  TARGET_LINK_LIBRARIES (XdmfJava 
			 Xdmf
                         ${JAVA_LIBRARY})
  IF(ICE_INSTALLED)
	INSTALL_TARGETS(${ICE_ROOT}/Release${ICE_VERSION}/${ARCH_TO_BUILD}/lib XdmfJava)
  ENDIF(ICE_INSTALLED)
ENDIF (XDMF_BUILD_JAVA)
OPTION(XDMF_BUILD_SHARED_LIBS "Build Shared XDMF Library")
OPTION(XDMF_HAVE_NDGM "Build NDGM Support into XDMF")
IF(XDMF_HAVE_NDGM)
	ADD_DEFINITIONS(-DHAVE_NDGM)
	IF(XDMF_BUILD_SHARED_LIBS)
		ADD_LIBRARY(Xdmf SHARED XdmfFormatSource XdmfModelSource XdmfNdgmSource)
	ELSE(XDMF_BUILD_SHARED_LIBS)
		ADD_LIBRARY(Xdmf STATIC XdmfFormatSource XdmfModelSource XdmfNdgmSource)
	ENDIF(XDMF_BUILD_SHARED_LIBS)
	LINK_LIBRARIES(ndgm)
ELSE(XDMF_HAVE_NDGM)
	IF(XDMF_BUILD_SHARED_LIBS)
		ADD_LIBRARY(Xdmf SHARED XdmfFormatSource XdmfModelSource )
	ELSE(XDMF_BUILD_SHARED_LIBS)
		ADD_LIBRARY(Xdmf STATIC XdmfFormatSource XdmfModelSource )
	ENDIF(XDMF_BUILD_SHARED_LIBS)
ENDIF(XDMF_HAVE_NDGM)
LINK_LIBRARIES(hdf5 expat z)

IF(ICE_INSTALLED)
	INSTALL_TARGETS(${ICE_ROOT}/Release${ICE_VERSION}/${ARCH_TO_BUILD}/lib Xdmf)
	IF(XDMF_BUILD_PYTHON)
		INSTALL_TARGETS(${ICE_ROOT}/Release${ICE_VERSION}/${ARCH_TO_BUILD}/lib XdmfPython)
		MESSAGE("!!! You MUST Rename ${ICE_ROOT}/Release${ICE_VERSION}/${ARCH_TO_BUILD}/lib/libXdmfPython.[dll so] to _Xdmf.[dll so] by hand !!!")
	ENDIF(XDMF_BUILD_PYTHON)
	IF(XDMF_BUILD_TCL)
		INSTALL_TARGETS(${ICE_ROOT}/Release${ICE_VERSION}/${ARCH_TO_BUILD}/lib XdmfTcl)
	ENDIF(XDMF_BUILD_TCL)
	IF(XDMF_BUILD_JAVA)
		INSTALL_TARGETS(${ICE_ROOT}/Release${ICE_VERSION}/${ARCH_TO_BUILD}/lib XdmfJava)
	ENDIF(XDMF_BUILD_JAVA)
	INSTALL_FILES(${ICE_ROOT}/Release${ICE_VERSION}/include/Xdmf .h XdmfFormatSource XdmfModelSource XdmfNdgmSource)
ENDIF(ICE_INSTALLED)
