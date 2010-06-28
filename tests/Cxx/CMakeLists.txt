set(XdmfCoreCxxTests
	TestXdmfArray
	TestXdmfArrayWriteRead
	TestXdmfDataItem
	TestXdmfHDF5Writer
)

set(XdmfCxxTests
	TestXdmfAttribute
	TestXdmfGeometry
	TestXdmfGrid
	TestXdmfGridCollection
	TestXdmfReader
	TestXdmfSet
	TestXdmfTopology
	TestXdmfVisitorValueCounter
	TestXdmfWriter
	TestXdmfWriterHDF5ThenXML
)

foreach(test ${XdmfCoreCxxTests})
  add_executable(${test} ${test})
  target_link_libraries(${test} XdmfCore)
  add_test(${test} ${test})
endforeach(test ${XdmfCoreCxxTests})

foreach(test ${XdmfCxxTests})
	add_executable(${test} ${test})
	target_link_libraries(${test} Xdmf)
	add_test(${test} ${test})
endforeach(test ${XdmfCxxTests})
