set(XdmfCxxTests
	TestXdmfAttribute
	TestXdmfGeometry
	TestXdmfGrid
	TestXdmfGridCollection
	TestXdmfReader
	TestXdmfSet
	TestXdmfTime
	TestXdmfTopology
	TestXdmfVisitorValueCounter
	TestXdmfWriter
	TestXdmfWriterHDF5ThenXML
	TestXdmfXPath
)

foreach(test ${XdmfCxxTests})
	add_executable(${test} ${test})
	target_link_libraries(${test} Xdmf)
	add_test(${test} ${test})
endforeach(test ${XdmfCxxTests})
