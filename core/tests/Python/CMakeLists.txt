include(UsePythonTest)

set(XdmfCorePythonTests
	TestXdmfArray.py
)

foreach(test ${XdmfCorePythonTests})
	set_source_files_properties(${test} PROPERTIES PYTHONPATH "${CMAKE_BINARY_DIR}")
	add_python_test(${test} ${test})
endforeach(test ${XdmfCorePythonTests})
