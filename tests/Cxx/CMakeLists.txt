set(XdmfCxxTests
	TestXdmfAttribute
	TestXdmfArray
	TestXdmfDataItem
	TestXdmfGeometry
	TestXdmfGrid
	TestXdmfTopology
	TestXdmfVisitor
	TestXdmfVisitorValueCounter
)

foreach(test ${XdmfCxxTests})
	add_executable(${test} ${test})
	target_link_libraries(${test} Xdmf)
	add_test(${test} ${test})
endforeach(test XdmfCxxTests)
