# Build vtkXdmfRenderWindowInteractor.h
IF(WIN32)
 SET(XDMF_WINDOWINTERACTOR "vtkWin32RenderWindowInteractor")
ELSE(WIN32)
 SET(XDMF_WINDOWINTERACTOR "vtkXRenderWindowInteractor")
ENDIF(WIN32)
CONFIGURE_FILE(
	${Xdmf_SOURCE_DIR}/vtk/vtkXdmfRenderWindowInteractor.h.in
	${Xdmf_BINARY_DIR}/vtk/vtkXdmfRenderWindowInteractor.h
	)
CONFIGURE_FILE(
 	${Xdmf_SOURCE_DIR}/vtk/vtkXdmfRenderWindowInteractor.cxx.in
 	${Xdmf_BINARY_DIR}/vtk/vtkXdmfRenderWindowInteractor.cxx
 	)

SET( Xdmf_SRCS
vtkXdmfReader
vtkXdmfDataSetWriter
vtkXdmfDataArray
vtkXdmfRenderWindowInteractor
)

SET_SOURCE_FILES_PROPERTIES(
	vtkXdmfRenderWindowInteractor.cxx
	vtkXdmfRenderWindowInteractor.h
	GENERATED)

	

# if vtk was found include the use vtk file
IF (VTK_FOUND)
  IF(VTK_BUILD_SHARED_LIBS)
    SET(LIBTYPE SHARED)
  ELSE(VTK_BUILD_SHARED_LIBS)
    SET(LIBTYPE STATIC)
  ENDIF(VTK_BUILD_SHARED_LIBS)
  ADD_LIBRARY(vtkXdmf ${LIBTYPE} ${Xdmf_SRCS})
  TARGET_LINK_LIBRARIES(vtkXdmf vtkIO Xdmf vtkRendering)

  # Create Tcl wrappers if VTK is wrapped in Tcl.
  IF(VTK_WRAP_TCL)
    VTK_WRAP_TCL(vtk${PROJECT_NAME}TCL XdmfTCL_SRCS ${Xdmf_SRCS})
    ADD_LIBRARY(vtk${PROJECT_NAME}TCL ${LIBTYPE} ${XdmfTCL_SRCS} ${Xdmf_SRCS})
    TARGET_LINK_LIBRARIES(vtk${PROJECT_NAME}TCL vtkIOTCL vtkRenderingTCL Xdmf)
  ENDIF(VTK_WRAP_TCL)

  # Create Python wrappers if VTK is wrapped in Python.
  IF(VTK_WRAP_PYTHON)
    VTK_WRAP_PYTHON(vtk${PROJECT_NAME}Python XdmfPython_SRCS ${Xdmf_SRCS})
    ADD_LIBRARY(vtk${PROJECT_NAME}Python SHARED ${XdmfPython_SRCS} ${Xdmf_SRCS})
    TARGET_LINK_LIBRARIES(vtk${PROJECT_NAME}Python vtkIOPython vtkRenderingPython Xdmf)
  ENDIF(VTK_WRAP_PYTHON)
  IF(XDMF_BUILD_TESTING)
    SUBDIRS(Testing)
  ENDIF(XDMF_BUILD_TESTING)
ELSE(VTK_FOUND)
MESSAGE(STATUS "Can't find VTK ... no vtkXdmf interface can be built")
ENDIF (VTK_FOUND)
