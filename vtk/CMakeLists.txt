# Build vtkXdmf*RenderWindowInteractor.h
IF(WIN32)
	SET(XDMF_WINDOWINTERACTOR vtkWin32RenderWindowInteractor)
	CONFIGURE_FILE(${Xdmf_SOURCE_DIR}/vtk/vtkXdmfRenderWindowInteractor.h.in
		${Xdmf_BINARY_DIR}/vtk/vtkXdmfRenderWindowInteractor.h
		@ONLY IMMEDIATE)
	CONFIGURE_FILE(${Xdmf_SOURCE_DIR}/vtk/vtkXdmfRenderWindowInteractor.cxx.in
		${Xdmf_BINARY_DIR}/vtk/vtkXdmfRenderWindowInteractor.cxx
		@ONLY IMMEDIATE)
ELSE(WIN32)
	INCLUDE_DIRECTORIES(${X11_X11_INCLUDE_PATH})
	SET(XDMF_WINDOWINTERACTOR vtkXRenderWindowInteractor)
	CONFIGURE_FILE(${Xdmf_SOURCE_DIR}/vtk/vtkXdmfRenderWindowInteractor.h.in
		${Xdmf_BINARY_DIR}/vtk/vtkXdmfRenderWindowInteractor.h
		@ONLY IMMEDIATE)
	CONFIGURE_FILE(${Xdmf_SOURCE_DIR}/vtk/vtkXdmfRenderWindowInteractor.cxx.in
		${Xdmf_BINARY_DIR}/vtk/vtkXdmfRenderWindowInteractor.cxx
		@ONLY IMMEDIATE)
ENDIF(WIN32)

SET( Xdmf_SRCS
	vtkXdmfReader.cxx
  vtkXdmfWriter.cxx
	vtkXdmfDataArray.cxx
	vtkXdmfOpenGLCamera.cxx
  #${Xdmf_BINARY_DIR}/vtk/vtkXdmfRenderWindowInteractor.cxx
  #vtkXdmfDataSetWriter.cxx
  )

IF (NOT APPLE)
	SET(Xdmf_SRCS ${Xdmf_SRCS}
		${Xdmf_BINARY_DIR}/vtk/vtkXdmfRenderWindowInteractor.cxx
		)
ENDIF (NOT APPLE)

SET_SOURCE_FILES_PROPERTIES(
  vtkXdmfDataArray
  WRAP_EXCLUDE
  )

SET(VTK_BINARY_DIR "${Xdmf_BINARY_DIR}/vtk")
INCLUDE(${VTK_SOURCE_DIR}/CMake/vtkExportKit.cmake)
VTK_EXPORT_KIT("Xdmf" "XDMF" "${Xdmf_SRCS}")

# if vtk was found include the use vtk file
IF (VTK_FOUND)
  IF(VTK_BUILD_SHARED_LIBS OR BUILD_SHARED_LIBS)
    SET(LIBTYPE SHARED)
  ELSE(VTK_BUILD_SHARED_LIBS OR BUILD_SHARED_LIBS)
    SET(LIBTYPE STATIC)
  ENDIF(VTK_BUILD_SHARED_LIBS OR BUILD_SHARED_LIBS)
  ADD_LIBRARY(vtkXdmf ${LIBTYPE} ${Xdmf_SRCS})
  TARGET_LINK_LIBRARIES(vtkXdmf vtkIO Xdmf vtkRendering)

  # Create Tcl wrappers if VTK is wrapped in Tcl.
  IF(VTK_WRAP_TCL)
    VTK_WRAP_TCL2(vtk${PROJECT_NAME}TCL XdmfTCL_SRCS ${Xdmf_SRCS})
    ADD_LIBRARY(vtk${PROJECT_NAME}TCL ${LIBTYPE} ${XdmfTCL_SRCS} ${Xdmf_SRCS})
    TARGET_LINK_LIBRARIES(vtk${PROJECT_NAME}TCL vtkIOTCL vtkRenderingTCL Xdmf)
    ADD_DEPENDENCIES(vtk${PROJECT_NAME}TCL vtk${PROJECT_NAME})
  ENDIF(VTK_WRAP_TCL)

  # Create Python wrappers if VTK is wrapped in Python.
  IF(VTK_WRAP_PYTHON)
    VTK_WRAP_PYTHON2(vtk${PROJECT_NAME}Python XdmfPython_SRCS ${Xdmf_SRCS})
    ADD_LIBRARY(vtk${PROJECT_NAME}Python SHARED ${XdmfPython_SRCS} ${Xdmf_SRCS})
    TARGET_LINK_LIBRARIES(vtk${PROJECT_NAME}Python vtkIOPython vtkRenderingPython Xdmf)
    ADD_DEPENDENCIES(vtk${PROJECT_NAME}Python vtk${PROJECT_NAME})
  ENDIF(VTK_WRAP_PYTHON)
  IF(XDMF_BUILD_TESTING)
    SUBDIRS(Testing)
  ENDIF(XDMF_BUILD_TESTING)
ELSE(VTK_FOUND)
  MESSAGE(STATUS "Can't find VTK ... no vtkXdmf interface can be built")
ENDIF (VTK_FOUND)

IF(NOT XDMF_NOT_INSTALL_INCLUDE_FILES)
  INSTALL_FILES(/include/XdmfVtk .h ${Xdmf_SRCS})
ENDIF(NOT XDMF_NOT_INSTALL_INCLUDE_FILES)
IF (VTK_FOUND AND BUILD_SHARED_LIBS)
  INSTALL_TARGETS(/lib vtkXdmf)
  IF(VTK_WRAP_TCL)
    INSTALL_TARGETS(/lib vtk${PROJECT_NAME}TCL)
  ENDIF(VTK_WRAP_TCL)
  IF(VTK_WRAP_PYTHON)
    INSTALL_TARGETS(/lib vtk${PROJECT_NAME}Python)
  ENDIF(VTK_WRAP_PYTHON)
ENDIF (VTK_FOUND AND BUILD_SHARED_LIBS)

