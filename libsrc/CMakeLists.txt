SET( XdmfFormatSource
  XdmfArray
  XdmfCharArray
  XdmfDOM
  XdmfDataDesc
  XdmfExpression
  XdmfFormat
  XdmfFormatMulti
  XdmfFormatHDF
  XdmfFormatXML
  XdmfHeavyData
  XdmfHDF
  XdmfHDFSupport
  XdmfLightData
  XdmfObject
  XdmfParameter
  XdmfXNode
  XdmfTransform
  XdmfXMLParser
  )

SET( XdmfExtraSource
  XdmfTree
  XdmfLlist
  XdmfExprLex
  XdmfExprYacc
  )

SET( XdmfModelSource
  XdmfAttribute
  XdmfGeometry
  XdmfGrid
  XdmfTopology
  XdmfNDGM
  XdmfRuntime
  )

SET( XdmfNdgmSource
  H5FDndgm
  )

SET( XdmfPythonSource
  XdmfPython
  )
SET( XdmfTclSource
  XdmfTcl
  )

INCLUDE (${CMAKE_ROOT}/Modules/CheckTypeSize.cmake)
IF(WIN32)
  CHECK_TYPE_SIZE(__int64        SIZEOF___INT64)
ENDIF(WIN32)
CHECK_TYPE_SIZE("long long"    SIZEOF_LONG_LONG)

OPTION(BUILD_SHARED_LIBS "Build Shared XDMF Library" ON)

CONFIGURE_FILE(${Xdmf_SOURCE_DIR}/libsrc/XdmfConfig.h.in
  ${Xdmf_BINARY_DIR}/libsrc/XdmfConfig.h)

IF(NOT WIN32)
  ADD_DEFINITIONS(-D_HPUX_SOURCE)
ENDIF(NOT WIN32)

IF(XDMF_HAS_NDGM)
  SET(XdmfSources ${XdmfFormatSource} ${XdmfExtraSource} ${XdmfModelSource} ${XdmfNdgmSource})
  INSTALL_FILES(/include/Xdmf .h
    XdmfConfig
    Xdmf
    XdmfArrayCopyMacro
    XdmfDice
    XdmfExport
    XdmfExpr
    ${XdmfFormatSource}
    ${XdmfModelSource}
    ${XdmfNdgmSource})
ELSE(XDMF_HAS_NDGM)
  SET(XdmfSources ${XdmfFormatSource} ${XdmfExtraSource} ${XdmfModelSource})
  INSTALL_FILES(/include/Xdmf .h
    XdmfConfig
    Xdmf
    XdmfArrayCopyMacro
    XdmfDice
    XdmfExport
    XdmfExpr
    ${XdmfFormatSource}
    ${XdmfModelSource})
ENDIF(XDMF_HAS_NDGM)

IF(BUILD_SHARED_LIBS)
  ADD_LIBRARY(Xdmf SHARED ${XdmfSources})
ELSE(BUILD_SHARED_LIBS)
  ADD_LIBRARY(Xdmf STATIC ${XdmfSources})
ENDIF(BUILD_SHARED_LIBS)
INSTALL_TARGETS(/lib Xdmf)

IF(XDMF_HAS_NDGM)
  TARGET_LINK_LIBRARIES(Xdmf ${NDGM_LIBRARY} ${HDF5_LIBRARY} ${XDMF_EXPAT_LIBRARIES} ${XDMF_ZLIB_LIBRARIES})
ELSE(XDMF_HAS_NDGM)
  TARGET_LINK_LIBRARIES(Xdmf ${HDF5_LIBRARY} ${XDMF_EXPAT_LIBRARIES} ${XDMF_ZLIB_LIBRARIES})
ENDIF(XDMF_HAS_NDGM)

OPTION(XDMF_WRAP_PYTHON "Build a Python wrapping interface" OFF)
IF(XDMF_WRAP_PYTHON)
  FIND_PACKAGE(PythonExecutable)
  IF(NOT BUILD_SHARED_LIBS)
    MESSAGE("Python wrapping only works on shared libraries")
  ENDIF(NOT BUILD_SHARED_LIBS)
  INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})
  INCLUDE (${CMAKE_ROOT}/Modules/FindPythonLibs.cmake)
  ADD_LIBRARY(_Xdmf SHARED ${XdmfPythonSource})
  IF(XDMF_HAS_NDGM)
    TARGET_LINK_LIBRARIES(_Xdmf ${NDGM_LIBRARY})
  ENDIF(XDMF_HAS_NDGM)
  INSTALL_TARGETS(/lib _Xdmf)
  TARGET_LINK_LIBRARIES(_Xdmf Xdmf ${HDF5_LIBRARY} ${EXPAT_LIBRARY} ${XDMF_ZLIB_LIBRARIES})
  IF(WIN32)
    TARGET_LINK_LIBRARIES(_Xdmf ${PYTHON_LIBRARY})
  ENDIF(WIN32)
  SET_TARGET_PROPERTIES(_Xdmf PROPERTIES PREFIX "")
ENDIF(XDMF_WRAP_PYTHON)

OPTION(XDMF_WRAP_TCL "Build a TCL wrapping interface" OF)
IF(XDMF_WRAP_TCL)
  IF(NOT BUILD_SHARED_LIBS)
    MESSAGE("Tcl wrapping only works on shared libraries")
  ENDIF(NOT BUILD_SHARED_LIBS)
  INCLUDE_DIRECTORIES(${TCL_INCLUDE_PATH})
  INCLUDE (${CMAKE_ROOT}/Modules/FindTCL.cmake)
  ADD_LIBRARY(XdmfTCL SHARED ${XdmfTclSource})
  INSTALL_TARGETS(/lib XdmfTCL)
  IF(XDMF_HAS_NDGM)
    TARGET_LINK_LIBRARIES(XdmfTCL ${NDGM_LIBRARY})
  ENDIF(XDMF_HAS_NDGM)
  TARGET_LINK_LIBRARIES(XdmfTCL Xdmf ${TCL_LIBRARY} ${HDF5_LIBRARY} ${EXPAT_LIBRARY} ${XDMF_ZLIB_LIBRARIES})
  ADD_EXECUTABLE(DummyTCL dummy.cxx)
  TARGET_LINK_LIBRARIES(DummyTCL XdmfTCL)
ENDIF(XDMF_WRAP_TCL)

IF(XDMF_BUILD_TESTING)
  SUBDIRS(Testing)
ENDIF(XDMF_BUILD_TESTING)
