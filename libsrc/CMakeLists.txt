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

OPTION(XDMF_REGENERATE_WRAPPERS
  "Regenerate XdmfPython.cxx XdmfTcl.cxx and XdmfJava.cxx" OFF)
MARK_AS_ADVANCED(XDMF_REGENERATE_WRAPPERS)
IF(XDMF_REGENERATE_WRAPPERS)
  FIND_PROGRAM(SWIG_EXECUTABLE 
    NAMES swig-1.3 swig
    PATHS /usr/bin
    DOC "Swig executable for generating wrapped files")
  MARK_AS_ADVANCED(SWIG_EXECUTABLE)
  IF(SWIG_EXECUTABLE)
    SET(XDMF_SWIG_INCLUDES 
      -I${Xdmf_SOURCE_DIR} 
      -I${Xdmf_SOURCE_DIR}/libsrc 
      -I${Xdmf_BINARY_DIR}/libsrc 
      -I${Xdmf_SOURCE_DIR}/Ice/libsrc
      -I${Xdmf_BINARY_DIR}/Ice/libsrc
      )
    GET_FILENAME_COMPONENT(SWIG_EXE_PATH ${SWIG_EXECUTABLE}
      PATH)
    SET(SWIG_LIB_PATH "${SWIG_EXE_PATH}/../../Swig/Lib")
    MESSAGE("SWIG_LIB_PATH ${SWIG_LIB_PATH}")
    IF(EXISTS "${SWIG_LIB_PATH}")
      SET(XDMF_SWIG_INCLUDES
        ${XDMF_SWIG_INCLUDES}
        -I${SWIG_LIB_PATH}
        -I${SWIG_LIB_PATH}/tcl
        -I${SWIG_LIB_PATH}/python
        )
    ENDIF(EXISTS "${SWIG_LIB_PATH}")
    FOREACH(LANGUAGE Tcl Python Java)
      STRING(TOLOWER ${LANGUAGE} lang)
      SET(swigtarget ${Xdmf_BINARY_DIR}/libsrc/Xdmf${LANGUAGE}.cxx)
      IF(LANGUAGE MATCHES "Tcl")
        SET(swig_extra_args -prefix Xdmf -namespace)
      ENDIF(LANGUAGE MATCHES "Tcl")
      ADD_CUSTOM_COMMAND(
        OUTPUT ${swigtarget}
        DEPENDS ${Xdmf_SOURCE_DIR}/libsrc/Xdmf.i
        COMMAND ${SWIG_EXECUTABLE}
        ARGS -v -c++ -make_default -includeall "-${lang}" ${swig_extra_args} ${XDMF_SWIG_INCLUDES} -o "${swigtarget}" Xdmf.i)

    ENDFOREACH(LANGUAGE)
    ADD_CUSTOM_TARGET(RerunSwig DEPENDS 
      ${Xdmf_BINARY_DIR}/libsrc/XdmfTcl.cxx
      ${Xdmf_BINARY_DIR}/libsrc/XdmfPython.cxx
      ${Xdmf_BINARY_DIR}/libsrc/XdmfJava.cxx
      )
  ENDIF(SWIG_EXECUTABLE)
ENDIF(XDMF_REGENERATE_WRAPPERS)

OPTION(XDMF_REGENERATE_YACCLEX
  "Regenerate XdmfExprYacc.cxx and XdmfExprLex.cxx" OFF)
MARK_AS_ADVANCED(XDMF_REGENERATE_YACCLEX)
IF(XDMF_REGENERATE_YACCLEX)
  FIND_PROGRAM(YACC_EXECUTABLE
    NAMES yacc bison
    PATHS /usr/bin
    DOC "Yacc or Bison executable")
  FIND_PROGRAM(FLEX_EXECUTABLE
    NAMES flex
    PATHS /usr/bin
    DOC "Flex executable")
  MARK_AS_ADVANCED(YACC_EXECUTABLE FLEX_EXECUTABLE)
  IF(YACC_EXECUTABLE)
    SET(BISON_FLAGS)
    IF(YACC_EXECUTABLE MATCHES "bison")
      SET(BISON_FLAGS "-y")
    ENDIF(YACC_EXECUTABLE MATCHES "bison")
    ADD_CUSTOM_COMMAND(
      OUTPUT ${Xdmf_BINARY_DIR}/libsrc/XdmfExprYacc.tab.c
      DEPENDS ${Xdmf_SOURCE_DIR}/libsrc/XdmfExpr.y
      COMMAND ${YACC_EXECUTABLE}
      ARGS ${BISON_FLAGS} -d -b ${Xdmf_BINARY_DIR}/libsrc/XdmfExprYacc
      -p dice_yy
      ${Xdmf_SOURCE_DIR}/libsrc/XdmfExpr.y
      )
    ADD_CUSTOM_COMMAND(
      OUTPUT ${Xdmf_BINARY_DIR}/libsrc/XdmfExprYacc.tab.h
      DEPENDS ${Xdmf_SOURCE_DIR}/libsrc/XdmfExpr.y
      COMMAND ${YACC_EXECUTABLE}
      ARGS ${BISON_FLAGS} -d -b ${Xdmf_BINARY_DIR}/libsrc/XdmfExprYacc
      -p dice_yy
      ${Xdmf_SOURCE_DIR}/libsrc/XdmfExpr.y
      )
    ADD_CUSTOM_COMMAND(
      OUTPUT ${Xdmf_BINARY_DIR}/libsrc/XdmfExprYacc.cxx
      DEPENDS ${Xdmf_BINARY_DIR}/libsrc/XdmfExprYacc.tab.c
      COMMAND ${CMAKE_COMMAND}
      ARGS -E copy ${Xdmf_BINARY_DIR}/libsrc/XdmfExprYacc.tab.c
      ${Xdmf_BINARY_DIR}/libsrc/XdmfExprYacc.cxx
      )
    ADD_CUSTOM_TARGET(RerunYacc DEPENDS
      ${Xdmf_BINARY_DIR}/libsrc/XdmfExprYacc.cxx)
  ENDIF(YACC_EXECUTABLE)
  IF(FLEX_EXECUTABLE)
    ADD_CUSTOM_COMMAND(
      OUTPUT ${Xdmf_BINARY_DIR}/libsrc/XdmfExprLex.cxx
      DEPENDS ${Xdmf_SOURCE_DIR}/libsrc/XdmfExpr.l
      COMMAND ${FLEX_EXECUTABLE}
      ARGS -Pdice_yy -o${Xdmf_BINARY_DIR}/libsrc/XdmfExprLex.cxx 
      ${Xdmf_SOURCE_DIR}/libsrc/XdmfExpr.l)
    ADD_CUSTOM_TARGET(RerunLex DEPENDS
      ${Xdmf_BINARY_DIR}/libsrc/XdmfExprLex.cxx)
  ENDIF(FLEX_EXECUTABLE)

ENDIF(XDMF_REGENERATE_YACCLEX)

INCLUDE (${CMAKE_ROOT}/Modules/CheckTypeSize.cmake)
IF(WIN32)
  CHECK_TYPE_SIZE(__int64        SIZEOF___INT64)
ENDIF(WIN32)
CHECK_TYPE_SIZE("long long"    SIZEOF_LONG_LONG)

INCLUDE (${CMAKE_ROOT}/Modules/CheckIncludeFile.cmake)
CHECK_INCLUDE_FILE(malloc.h XDMF_HAVE_MALLOC_H)

CONFIGURE_FILE(${Xdmf_SOURCE_DIR}/libsrc/XdmfConfig.h.in
  ${Xdmf_BINARY_DIR}/libsrc/XdmfConfig.h)

IF(NOT WIN32)
  ADD_DEFINITIONS(-D_HPUX_SOURCE)
ENDIF(NOT WIN32)

SET(XdmfSources ${XdmfFormatSource} ${XdmfExtraSource} ${XdmfModelSource})

IF(XDMF_HAS_NDGM)
  SET(XdmfSources ${XdmfSources} ${XdmfNdgmSource})
ENDIF(XDMF_HAS_NDGM)

IF(BUILD_SHARED_LIBS)
  ADD_LIBRARY(Xdmf SHARED ${XdmfSources})
ELSE(BUILD_SHARED_LIBS)
  ADD_LIBRARY(Xdmf STATIC ${XdmfSources})
ENDIF(BUILD_SHARED_LIBS)

IF(XDMF_HAS_NDGM)
  TARGET_LINK_LIBRARIES(Xdmf ${NDGM_LIBRARY} ${HDF5_LIBRARY} ${XDMF_MPI_LIBRARIES} ${XDMF_EXPAT_LIBRARIES} ${XDMF_ZLIB_LIBRARIES})
ELSE(XDMF_HAS_NDGM)
  TARGET_LINK_LIBRARIES(Xdmf ${HDF5_LIBRARY} ${XDMF_MPI_LIBRARIES} ${XDMF_EXPAT_LIBRARIES} ${XDMF_ZLIB_LIBRARIES})
ENDIF(XDMF_HAS_NDGM)

OPTION(XDMF_WRAP_PYTHON "Build a Python wrapping interface" OFF)
IF(XDMF_WRAP_PYTHON OR VTK_WRAP_PYTHON)
  FIND_PACKAGE(PythonExecutable)
  FIND_PACKAGE(PythonLibs)
ENDIF(XDMF_WRAP_PYTHON OR VTK_WRAP_PYTHON)

IF(XDMF_WRAP_PYTHON)
  IF(NOT BUILD_SHARED_LIBS)
    MESSAGE("Python wrapping only works on shared libraries")
  ENDIF(NOT BUILD_SHARED_LIBS)
  INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})
  ADD_LIBRARY(_Xdmf SHARED ${XdmfPythonSource})
  IF(XDMF_HAS_NDGM)
    TARGET_LINK_LIBRARIES(_Xdmf ${NDGM_LIBRARY})
  ENDIF(XDMF_HAS_NDGM)
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

IF(NOT XDMF_NOT_INSTALL_INCLUDE_FILES)
  INSTALL_FILES(/include/Xdmf .h
    XdmfConfig
    Xdmf
    XdmfArrayCopyMacro
    XdmfDice
    XdmfExport
    XdmfExpr
    ${XdmfFormatSource}
    ${XdmfModelSource})
  IF(XDMF_HAS_NDGM)
    INSTALL_FILES(/include/Xdmf .h
      ${XdmfNdgmSource})
  ENDIF(XDMF_HAS_NDGM)
ENDIF(NOT XDMF_NOT_INSTALL_INCLUDE_FILES)

IF(BUILD_SHARED_LIBS)
  INSTALL_TARGETS(/lib Xdmf)
  IF(XDMF_WRAP_PYTHON)
    INSTALL_TARGETS(/lib _Xdmf)
  ENDIF(XDMF_WRAP_PYTHON)
  IF(XDMF_WRAP_TCL)
    INSTALL_TARGETS(/lib XdmfTCL)
  ENDIF(XDMF_WRAP_TCL)
ENDIF(BUILD_SHARED_LIBS)

