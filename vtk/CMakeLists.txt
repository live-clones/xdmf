SET( Xdmf_SRCS
vtkMyXdmfReader
vtkXdmfDataSetWriter
vtkXdmfReader
vtkXdmfDataArray
)

# if vtk was found include the use vtk file
IF (VTK_FOUND)
  IF(VTK_BUILD_SHARED_LIBS)
    SET(LIBTYPE SHARED)
  ELSE(VTK_BUILD_SHARED_LIBS)
    SET(LIBTYPE STATIC)
  ENDIF(VTK_BUILD_SHARED_LIBS)
  ADD_LIBRARY(vtkXdmf ${LIBTYPE} ${Xdmf_SRCS})
  TARGET_LINK_LIBRARIES(vtkXdmf vtkIO Xdmf)

  # Create Tcl wrappers if VTK is wrapped in Tcl.
  IF(VTK_WRAP_TCL)
    VTK_WRAP_TCL(vtk${PROJECT_NAME}TCL XdmfTCL_SRCS ${Xdmf_SRCS})
    ADD_LIBRARY(vtk${PROJECT_NAME}TCL ${LIBTYPE} ${XdmfTCL_SRCS} ${Xdmf_SRCS})
    TARGET_LINK_LIBRARIES(vtk${PROJECT_NAME}TCL vtkIOTCL Xdmf)
  ENDIF(VTK_WRAP_TCL)

  # Create Python wrappers if VTK is wrapped in Python.
  IF(VTK_WRAP_PYTHON)
    VTK_WRAP_PYTHON(vtk${PROJECT_NAME}Python XdmfPython_SRCS ${Xdmf_SRCS})
    ADD_LIBRARY(vtk${PROJECT_NAME}Python SHARED ${XdmfPython_SRCS} ${Xdmf_SRCS})
    TARGET_LINK_LIBRARIES(vtk${PROJECT_NAME}Python vtkIOPython Xdmf)
  ENDIF(VTK_WRAP_PYTHON)

  ADD_EXECUTABLE(VTKTest VTKTest.cxx)
  TARGET_LINK_LIBRARIES(VTKTest vtkXdmfTCL)
  ADD_TEST(VTKTest ${EXECUTABLE_OUTPUT_PATH}/VTKTest)
ENDIF (VTK_FOUND)
