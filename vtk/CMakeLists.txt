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

SOURCE_FILES( Xdmf_SRCS
vtkXdmfReaderHelper
vtkXdmfReader
vtkXdmfDataArray
vtkXdmfDataSetWriter
vtkXdmfRenderWindowInteractor
vtkXdmfUtil
)

# CYGWIN ?
IF(CMAKE_COMPILER_IS_GNUCXX)
 IF(WIN32)
  IF(UNIX)
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwin32")
   SET(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -mwin32")
  ENDIF(UNIX)
 ENDIF(WIN32)
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

ADD_LIBRARY(vtkXdmf Xdmf_SRCS)
SET(ARCH_TO_BUILD Linux CACHE STRING "Host Arcitecture : Linux IRIXN32 IRIX64 AIX CYGWIN")
ADD_DEFINITIONS(-D${ARCH_TO_BUILD})
# If we alread have ICE everything is in
# one place
OPTION(ICE_INSTALLED "ICE Already Installed" ON)
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
	INCLUDE_DIRECTORIES(${ICE_ROOT}/Support${ICE_VERSION}/${ARCH_TO_BUILD}/include/Vtk)
	INCLUDE_DIRECTORIES(${ICE_ROOT}/Support${ICE_VERSION}/${ARCH_TO_BUILD}/include/Vtk/Common)
	INCLUDE_DIRECTORIES(${ICE_ROOT}/Support${ICE_VERSION}/${ARCH_TO_BUILD}/include/Vtk/IO)
	INCLUDE_DIRECTORIES(${ICE_ROOT}/Support${ICE_VERSION}/${ARCH_TO_BUILD}/include/Vtk/Graphics)
	INCLUDE_DIRECTORIES(${ICE_ROOT}/Support${ICE_VERSION}/${ARCH_TO_BUILD}/include/Vtk/Rendering)
	INCLUDE_DIRECTORIES(${ICE_ROOT}/Support${ICE_VERSION}/include)
	LINK_DIRECTORIES(${ICE_ROOT}/Release${ICE_VERSION}/${ARCH_TO_BUILD}/lib)
	LINK_DIRECTORIES(${ICE_ROOT}/Support${ICE_VERSION}/${ARCH_TO_BUILD}/lib)
	LINK_DIRECTORIES(${ICE_ROOT}/Support${ICE_VERSION}/${ARCH_TO_BUILD}/lib/vtk)
ELSE(ICE_INSTALLED)
ENDIF(ICE_INSTALLED)
FIND_PATH(XDMF_INCLUDE_PATH Xdmf/Xdmf.h
	/usr/local
	)
FIND_LIBRARY(XDMF_LIBRARY Xdmf
	/usr/local/lib
	)
INCLUDE_DIRECTORIES(${XDMF_INCLUDE_PATH})
LINK_LIBRARIES ( vtkpng vtkzlib vtkjpeg Xdmf hdf5 expat z )
IF(XDMF_HAVE_NDGM)
	LINK_DIRECTORIES(${ICE_BINARY_DIR}/Ndgm/libsrc)
	LINK_LIBRARIES(ndgm)
ENDIF(XDMF_HAVE_NDGM)

# Allow the user to customize their build with some local options
#
INCLUDE (${VTK_BINARY_DIR}/Xdmf/LocalUserOptions.cmake OPTIONAL) 
INCLUDE (${VTK_SOURCE_DIR}/Xdmf/LocalUserOptions.cmake OPTIONAL) 

# if we are wrapping into Tcl then add the library and extra 
# source files
#
WRAP_EXCLUDE_FILES(vtkXdmfReaderHelper )

IF (VTK_WRAP_TCL)
  VTK_WRAP_TCL(vtkXdmfTCL XdmfTCL_SRCS ${Xdmf_SRCS})
  ADD_LIBRARY(vtkXdmfTCL ${XdmfTCL_SRCS})
  TARGET_LINK_LIBRARIES (vtkXdmfTCL 
                         vtkFilteringTCL 
			 vtkIOTCL
                         vtkCommonTCL  
                         ${TCL_LIBRARY})
  INSTALL_TARGETS(/lib/vtk vtkXdmfTCL)
ENDIF (VTK_WRAP_TCL)

# if we are wrapping into Python then add the library and extra 
# source files
#
OPTION(XDMF_WRAP_PYTHON "wrap classes into the Python interpreted language")
IF (XDMF_WRAP_PYTHON)
  INCLUDE (${CMAKE_ROOT}/Modules/FindPythonLibs.cmake)
  UTILITY_SOURCE(VTK_WRAP_PYTHON_EXE vtkWrapPython Wrapping vtkWrapPython.c)
  MARK_AS_ADVANCED(VTK_WRAP_PYTHON_EXE)
  VTK_WRAP_PYTHON(vtkXdmfPython XdmfPython_SRCS ${Xdmf_SRCS})
  INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})
  FIND_PATH(PYCONFIG_INCLUDE_PATH pyconfig.h
	${PYTHON_INCLUDE_PATH}
	${PYTHON_INCLUDE_PATH}/..
	${ICE_ROOT}/Support${ICE_VERSION}/${ARCH_TO_BUILD}/include/python2.2
	)
  INCLUDE_DIRECTORIES(${PYCONFIG_INCLUDE_PATH})
  IF (APPLE)
    ADD_LIBRARY(vtkXdmfPython MODULE ${XdmfPython_SRCS})
  ELSE (APPLE)
    ADD_LIBRARY(vtkXdmfPython SHARED ${XdmfPython_SRCS})
    TARGET_LINK_LIBRARIES (vtkXdmfPython 
			   vtkRenderingPython
			   vtkGraphicsPython
                           vtkFilteringPython 
			   vtkIOPython
                           vtkCommonPython
			   )
  ENDIF (APPLE)
  TARGET_LINK_LIBRARIES (vtkXdmfPython 
                         ${PYTHON_LIBRARY})
  INSTALL_TARGETS(/lib/vtk vtkXdmfPython)
ENDIF (XDMF_WRAP_PYTHON)

# if we are wrapping into Java then add the library and extra 
# source files
#
IF (VTK_WRAP_JAVA)
  VTK_WRAP_JAVA(vtkXdmfJava XdmfJava_SRCS ${Xdmf_SRCS})
  ADD_LIBRARY(vtkXdmfJava SHARED ${XdmfJava_SRCS})
  TARGET_LINK_LIBRARIES (vtkXdmfJava 
                         vtkCommonJava 
                         vtkFilteringJava)
  INSTALL_TARGETS(/lib/vtk vtkXdmfJava)
ENDIF (VTK_WRAP_JAVA)

LINK_LIBRARIES (
vtkCommon 
vtkFiltering
vtkGraphics
vtkIO
vtkXdmf
)

IF(WIN32)
  IF(UNIX)
    IF(NOT BUILD_SHARED_LIBS)
      ADD_DEFINITIONS(-DPNG_STATIC)
    ENDIF(NOT BUILD_SHARED_LIBS)
  ENDIF(UNIX)
ENDIF(WIN32)


IF(WIN32)
  IF(NOT BUILD_SHARED_LIBS)
    ADD_DEFINITIONS(-DJPEGSTATIC)
  ENDIF(NOT BUILD_SHARED_LIBS)
ENDIF(WIN32)

INSTALL_TARGETS(/lib/vtk vtkXdmf)
INSTALL_FILES(/include/vtk .h Xdmf_SRCS)
