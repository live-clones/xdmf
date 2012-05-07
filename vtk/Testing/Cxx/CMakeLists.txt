ADD_EXECUTABLE(XdmfTestVTKIO XdmfTestVTKIO.cxx)

set(_libs 
  vtkIOCore
  vtkCommonCore
  vtkXdmf
  vtkInteractionStyle)

if(PARAVIEW_USE_MPI)
  list(APPEND _libs vtkFiltersParallel)
endif()

target_link_libraries(XdmfTestVTKIO ${_libs})

ADD_TEST(XdmfTestVTKIO ${EXECUTABLE_OUTPUT_PATH}/XdmfTestVTKIO)

