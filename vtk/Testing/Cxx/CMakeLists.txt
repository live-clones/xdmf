ADD_EXECUTABLE(XdmfTestVTKIO XdmfTestVTKIO.cxx)
TARGET_LINK_LIBRARIES(XdmfTestVTKIO vtkIO vtkCommon vtkFiltering vtkGraphics vtkXdmf)

ADD_TEST(XdmfTestVTKIO ${EXECUTABLE_OUTPUT_PATH}/XdmfTestVTKIO)

