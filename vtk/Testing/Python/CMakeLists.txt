FIND_PROGRAM(PYTHON_EXECUTABLE 
  NAMES python vtkpython
  PATHS ${VTK_LIBRARY_DIRS})

IF(PYTHON_EXECUTABLE)
  ADD_TEST(TestVTKPython ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/TestVTKXdmf.py)

  ADD_TEST(VTKPythonMultiGrid ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/Shapes.py ${Xdmf_SOURCE_DIR}/Data/Data/Shapes/Shapes.xmf)

  IF(VTK_USE_DISPLAY)
    FOREACH(test 
        IronImage
        IronRect
        IronStruct
        )
      ADD_TEST(${test}Magic 
        ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/IronMagic.py
        "${Xdmf_SOURCE_DIR}/Data" IronMagic)
      ADD_TEST(${test}StrideMagic 
        ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/IronMagic.py
        "${Xdmf_SOURCE_DIR}/Data" IronMagicStride "2 3 5")
    ENDFOREACH(test)
  ENDIF(VTK_USE_DISPLAY)

ELSE(PYTHON_EXECUTABLE)
  MESSAGE(FATAL_ERROR "Python executable is not set")
ENDIF(PYTHON_EXECUTABLE)
