set(XdmfTests
	TestXdmfAttribute
	TestXdmfGeometry
	TestXdmfGrid
	TestXdmfTopology
	TestXdmfVisitor
)

foreach(test ${XdmfTests})
	add_executable(${test} ${test})
	target_link_libraries(${test} Xdmf)
	add_test(${test} ${test})
endforeach(test XdmfTests)
