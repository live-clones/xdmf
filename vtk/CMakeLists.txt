# Build vtkXdmf*RenderWindowInteractor.h
IF(WIN32)
	SET( Xdmf_SRCS
		vtkXdmfReader
		vtkXdmfDataSetWriter
		vtkXdmfDataArray
		vtkXdmfWin32RenderWindowInteractor
		)
ELSE(WIN32)
	SET( Xdmf_SRCS
		vtkXdmfReader
		vtkXdmfDataSetWriter
		vtkXdmfDataArray
		vtkXdmfXRenderWindowInteractor
		)
ENDIF(WIN32)

INSTALL_FILES(/include/XdmfVtk .h ${Xdmf_SRCS})

# if vtk was found include the use vtk file
IF (VTK_FOUND)
  IF(VTK_BUILD_SHARED_LIBS)
    SET(LIBTYPE SHARED)
  ELSE(VTK_BUILD_SHARED_LIBS)
    SET(LIBTYPE STATIC)
  ENDIF(VTK_BUILD_SHARED_LIBS)
  ADD_LIBRARY(vtkXdmf ${LIBTYPE} ${Xdmf_SRCS})
  INSTALL_TARGETS(/lib vtkXdmf)
  TARGET_LINK_LIBRARIES(vtkXdmf vtkIO Xdmf vtkRendering)

  # Create Tcl wrappers if VTK is wrapped in Tcl.
  IF(VTK_WRAP_TCL)
    VTK_WRAP_TCL2(vtk${PROJECT_NAME}TCL XdmfTCL_SRCS ${Xdmf_SRCS})
    ADD_LIBRARY(vtk${PROJECT_NAME}TCL ${LIBTYPE} ${XdmfTCL_SRCS} ${Xdmf_SRCS})
    INSTALL_TARGETS(/lib vtk${PROJECT_NAME}TCL)
    TARGET_LINK_LIBRARIES(vtk${PROJECT_NAME}TCL vtkIOTCL vtkRenderingTCL Xdmf)
  ENDIF(VTK_WRAP_TCL)

  # Create Python wrappers if VTK is wrapped in Python.
  IF(VTK_WRAP_PYTHON)
    VTK_WRAP_PYTHON2(vtk${PROJECT_NAME}Python XdmfPython_SRCS ${Xdmf_SRCS})
    ADD_LIBRARY(vtk${PROJECT_NAME}Python SHARED ${XdmfPython_SRCS} ${Xdmf_SRCS})
    INSTALL_TARGETS(/lib vtk${PROJECT_NAME}Python)
    TARGET_LINK_LIBRARIES(vtk${PROJECT_NAME}Python vtkIOPython vtkRenderingPython Xdmf)
  ENDIF(VTK_WRAP_PYTHON)
  IF(XDMF_BUILD_TESTING)
    SUBDIRS(Testing)
  ENDIF(XDMF_BUILD_TESTING)
ELSE(VTK_FOUND)
MESSAGE(STATUS "Can't find VTK ... no vtkXdmf interface can be built")
ENDIF (VTK_FOUND)
