include(UsePythonTest)

set(XdmfPythonTests
	TestXdmfArray.py
	TestXdmfVisitor.py
)

foreach(test ${XdmfPythonTests})
  set_source_files_properties(${test} PROPERTIES PYTHONPATH "${CMAKE_BINARY_DIR}")
	add_python_test(${test} ${test})
endforeach(test XdmfPythonTests)
