SET( Xdmf_SRCS
vtkMyXdmfReader
vtkXdmfDataArray
)

# include module to find vtk
INCLUDE (${CMAKE_ROOT}/Modules/FindVTK.cmake)
# if vtk was found include the use vtk file
IF (VTK_FOUND)
  INCLUDE(${VTK_USE_FILE})

  ADD_LIBRARY(vtkXdmf SHARED ${Xdmf_SRCS})
  TARGET_LINK_LIBRARIES(vtkXdmf vtkIO Xdmf)

  # Create Tcl wrappers if VTK is wrapped in Tcl.
  IF(VTK_WRAP_TCL)
    VTK_WRAP_TCL(vtk${PROJECT_NAME}TCL XdmfTCL_SRCS ${Xdmf_SRCS})
    ADD_LIBRARY(vtk${PROJECT_NAME}TCL SHARED ${XdmfTCL_SRCS} ${Xdmf_SRCS})
    TARGET_LINK_LIBRARIES(vtk${PROJECT_NAME}TCL vtkIOTCL Xdmf)
  ENDIF(VTK_WRAP_TCL)

  ADD_EXECUTABLE(VTKTest VTKTest.cxx)
  TARGET_LINK_LIBRARIES(VTKTest vtkXdmfTCL)
ENDIF (VTK_FOUND)