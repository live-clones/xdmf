set(XdmfTests
	TestXdmfGeometry
	TestXdmfTopology
	TestXdmfVisitor
)

foreach(test ${XdmfTests})
	add_executable(${test} ${test})
	target_link_libraries(${test} Xdmf)
endforeach(test XdmfTests)