set(XdmfCoreCxxTests
	TestXdmfArray
	TestXdmfArrayWriteRead
	TestXdmfHDF5Controller
	TestXdmfHDF5Writer
	TestXdmfInformation
)

foreach(test ${XdmfCoreCxxTests})
  add_executable(${test} ${test})
  target_link_libraries(${test} XdmfCore)
  add_test(${test} ${test})
endforeach(test ${XdmfCoreCxxTests})
