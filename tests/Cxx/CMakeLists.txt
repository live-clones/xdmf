set(XdmfCxxTests
	TestXdmfArray
	TestXdmfArrayWriteRead
	TestXdmfAttribute
	TestXdmfDataItem
	TestXdmfGeometry
	TestXdmfGrid
	TestXdmfHDF5Writer
	TestXdmfReader
	TestXdmfSet
	TestXdmfTopology
	TestXdmfVisitorValueCounter
	TestXdmfWriter
	TestXdmfWriterHDF5ThenXML
)

foreach(test ${XdmfCxxTests})
	add_executable(${test} ${test})
	target_link_libraries(${test} Xdmf)
	add_test(${test} ${test})
endforeach(test XdmfCxxTests)
