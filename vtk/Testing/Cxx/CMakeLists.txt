CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/TestingConstants.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/TestingConstants.h @ONLY)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

ADD_EXECUTABLE(GridTest GridTest.cxx)
TARGET_LINK_LIBRARIES(GridTest vtkXdmf)

ADD_EXECUTABLE(Structured Structured.cxx)
TARGET_LINK_LIBRARIES(Structured vtkXdmf)

ADD_EXECUTABLE(VTKTest VTKTest.cxx)
TARGET_LINK_LIBRARIES(VTKTest vtkXdmf)

IF(VTK_WRAP_TCL)
  TARGET_LINK_LIBRARIES(VTKTest vtkXdmfTCL)
ENDIF(VTK_WRAP_TCL)
IF(VTK_WRAP_PYTHON)
  TARGET_LINK_LIBRARIES(VTKTest vtkXdmfPython ${PYTHON_LIBRARY})
ENDIF(VTK_WRAP_PYTHON)
ADD_TEST(VTKTest ${EXECUTABLE_OUTPUT_PATH}/VTKTest ${Xdmf_SOURCE_DIR}/Data/Data/Test1.xmf)

ADD_EXECUTABLE(SystemInformation SystemInformation.cxx)
ADD_TEST(SystemInformation ${EXECUTABLE_OUTPUT_PATH}/SystemInformation)
TARGET_LINK_LIBRARIES(SystemInformation vtkXdmf)

# Configure a header needed by SystemInformation.
CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/SystemInformation.h.in"
               "${CMAKE_CURRENT_BINARY_DIR}/SystemInformation.h"
               @ONLY IMMEDIATE)

ADD_EXECUTABLE(WriterTest WriterTest.cxx)
TARGET_LINK_LIBRARIES(WriterTest vtkXdmf)

ADD_EXECUTABLE(TestWriters TestWriters.cxx)
TARGET_LINK_LIBRARIES(TestWriters vtkXdmf)
ADD_TEST(TestVTKWriters ${EXECUTABLE_OUTPUT_PATH}/TestWriters ${Xdmf_BINARY_DIR}/vtk/Testing/Cxx/TestVTKWriters.xmf)

