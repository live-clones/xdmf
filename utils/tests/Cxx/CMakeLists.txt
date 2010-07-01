set(XdmfUtilsCxxTests
	TestXdmfTopologyConverter
)

foreach(test ${XdmfUtilsCxxTests})
	add_executable(${test} ${test})
	target_link_libraries(${test} XdmfUtils)
	add_test(${test} ${test})
endforeach(test ${XdmfUtilsCxxTests})
