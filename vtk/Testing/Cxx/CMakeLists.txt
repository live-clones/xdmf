ADD_EXECUTABLE(XdmfTestVTKIO XdmfTestVTKIO.cxx)
TARGET_LINK_LIBRARIES(XdmfTestVTKIO
  vtkIOCore
  vtkCommonCore
  vtkXdmf
  vtkFiltersParallel
  vtkInteractionStyle)

ADD_TEST(XdmfTestVTKIO ${EXECUTABLE_OUTPUT_PATH}/XdmfTestVTKIO)

