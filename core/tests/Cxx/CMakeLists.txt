set(XdmfCoreCxxTests
	TestXdmfArray
	TestXdmfArrayWriteRead
	TestXdmfHDF5Writer
)

foreach(test ${XdmfCoreCxxTests})
  add_executable(${test} ${test})
  target_link_libraries(${test} XdmfCore)
  add_test(${test} ${test})
endforeach(test ${XdmfCoreCxxTests})
