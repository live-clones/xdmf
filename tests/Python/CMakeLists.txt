include(UsePythonTest)

set(XdmfPythonTests
	TestXdmfArray.py
	TestXdmfHDF5Writer.py
	TestXdmfWriter.py
)

foreach(test ${XdmfPythonTests})
  set_source_files_properties(${test} PROPERTIES PYTHONPATH "${CMAKE_BINARY_DIR}")
	add_python_test(${test} ${test})
endforeach(test XdmfPythonTests)
