ADD_EXECUTABLE(TestNewWriter TestNewWriter.cxx)
TARGET_LINK_LIBRARIES(TestNewWriter vtkIO vtkCommon vtkFiltering vtkGraphics vtkXdmf)

ADD_TEST(NewWriter ${EXECUTABLE_OUTPUT_PATH}/TestNewWriter)

