FIND_PROGRAM(PYTHON_EXECUTABLE 
  NAMES python vtkpython
  PATHS ${VTK_LIBRARY_DIRS})

IF(PYTHON_EXECUTABLE)
  ADD_TEST(TestVTKPython ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/TestVTKXdmf.py)

  ADD_TEST(VTKPythonMultiGrid ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/Shapes.py ${Xdmf_SOURCE_DIR}/Data/Data/Shapes/Shapes.xmf)

  IF(VTK_USE_DISPLAY)
    ADD_TEST(IronImageMagic ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/IronMagic.py
      "${Xdmf_SOURCE_DIR}/Data/Data/Iron Protein.xmf" "${Xdmf_SOURCE_DIR}/Data/Baseline" IronMagic)
    ADD_TEST(IronImageStrideMagic ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/IronMagic.py
      "${Xdmf_SOURCE_DIR}/Data/Data/Iron Protein.xmf" "${Xdmf_SOURCE_DIR}/Data/Baseline" IronMagicStride "2 3 5")
    ADD_TEST(IronRectMagic ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/IronMagic.py
      "${Xdmf_SOURCE_DIR}/Data/Data/IronRectProtein.xmf" "${Xdmf_SOURCE_DIR}/Data/Baseline" IronMagic)
    ADD_TEST(IronRectStrideMagic ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/IronMagic.py
      "${Xdmf_SOURCE_DIR}/Data/Data/IronRectProtein.xmf" "${Xdmf_SOURCE_DIR}/Data/Baseline" IronMagicStride "2 3 5")
    ADD_TEST(IronStructMagic ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/IronMagic.py
      "${Xdmf_SOURCE_DIR}/Data/Data/IronStructProtein.xmf" "${Xdmf_SOURCE_DIR}/Data/Baseline" IronMagic)
    ADD_TEST(IronStructStrideMagic ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/IronMagic.py
      "${Xdmf_SOURCE_DIR}/Data/Data/IronStructProtein.xmf" "${Xdmf_SOURCE_DIR}/Data/Baseline" IronMagicStride "2 3 5")
  ENDIF(VTK_USE_DISPLAY)

ELSE(PYTHON_EXECUTABLE)
  MESSAGE(FATAL_ERROR "Python executable is not set")
ENDIF(PYTHON_EXECUTABLE)
