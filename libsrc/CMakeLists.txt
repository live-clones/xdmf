SET( XdmfFormatSource
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

SET( XdmfModelSource
	XdmfAttribute
	XdmfGeometry
	XdmfGrid
	XdmfTopology
)

SET( XdmfNdgmSource
	H5FDndgm
	XdmfNDGM
	XdmfRuntime
)

SET( XdmfPythonSource
	XdmfPython
)
SET( XdmfTclSource
	XdmfTcl
)

INCLUDE (${CMAKE_ROOT}/Modules/CheckTypeSize.cmake)
CHECK_TYPE_SIZE(__int64        SIZEOF___INT64)
CHECK_TYPE_SIZE("long long"    SIZEOF_LONG_LONG)

OPTION(XDMF_BUILD_SHARED_LIBS "Build Shared XDMF Library" ON)

IF(CMAKE_COMPILER_IS_GNUCXX)
  OPTION(XDMF_USE_ANSI_STDLIB "Use the ANSI standard iostream library" ON)
ELSE(CMAKE_COMPILER_IS_GNUCXX)
  IF(CMAKE_BUILD_TOOL MATCHES "devenv")
    OPTION(XDMF_USE_ANSI_STDLIB "Use the ANSI standard iostream library" ON)
  ELSE(CMAKE_BUILD_TOOL MATCHES "devenv")
    OPTION(XDMF_USE_ANSI_STDLIB "Use the ANSI standard iostream library" OFF)
  ENDIF(CMAKE_BUILD_TOOL MATCHES "devenv")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

CONFIGURE_FILE(${Xdmf_SOURCE_DIR}/libsrc/XdmfConfig.h.in
               ${Xdmf_BINARY_DIR}/libsrc/XdmfConfig.h)

IF(XDMF_HAS_NDGM)
  SET(XdmfSources ${XdmfFormatSource} ${XdmfModelSource} ${XdmfNdgmSource})
ELSE(XDMF_HAS_NDGM)
  SET(XdmfSources ${XdmfFormatSource} ${XdmfModelSource})
ENDIF(XDMF_HAS_NDGM)

IF(XDMF_BUILD_SHARED_LIBS)
  ADD_LIBRARY(Xdmf SHARED ${XdmfSources})
ELSE(XDMF_BUILD_SHARED_LIBS)
  ADD_LIBRARY(Xdmf STATIC ${XdmfSources})
ENDIF(XDMF_BUILD_SHARED_LIBS)

TARGET_LINK_LIBRARIES(Xdmf ${HDF5_LIBRARY} ${EXPAT_LIBRARY})

OPTION(XDMF_WRAP_PYTHON "Build a Python wrapping interface" ON)
IF(XDMF_WRAP_PYTHON)
  IF(NOT XDMF_BUILD_SHARED_LIBS)
    MESSAGE("Python wrapping only works on shared libraries")
  ENDIF(NOT XDMF_BUILD_SHARED_LIBS)
  INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})
  INCLUDE (${CMAKE_ROOT}/Modules/FindPythonLibs.cmake)
  ADD_LIBRARY(_Xdmf SHARED ${XdmfSources} ${XdmfPythonSource})
  TARGET_LINK_LIBRARIES(_Xdmf ${PYTHON_LIBRARY} ${HDF5_LIBRARY} ${EXPAT_LIBRARY})
  SET_TARGET_PROPERTIES(_Xdmf PROPERTIES PREFIX "")
ENDIF(XDMF_WRAP_PYTHON)

OPTION(XDMF_WRAP_TCL "Build a TCL wrapping interface" ON)
IF(XDMF_WRAP_TCL)
  IF(NOT XDMF_BUILD_SHARED_LIBS)
    MESSAGE("Tcl wrapping only works on shared libraries")
  ENDIF(NOT XDMF_BUILD_SHARED_LIBS)
  INCLUDE_DIRECTORIES(${TCL_INCLUDE_PATH})
  INCLUDE (${CMAKE_ROOT}/Modules/FindTCL.cmake)
  ADD_LIBRARY(XdmfTCL SHARED ${XdmfSources} ${XdmfTclSource})
  TARGET_LINK_LIBRARIES(XdmfTCL ${TCL_LIBRARY} ${HDF5_LIBRARY} ${EXPAT_LIBRARY})
  ADD_EXECUTABLE(DummyTCL dummy.cxx)
  TARGET_LINK_LIBRARIES(DummyTCL XdmfTCL)
ENDIF(XDMF_WRAP_TCL)
