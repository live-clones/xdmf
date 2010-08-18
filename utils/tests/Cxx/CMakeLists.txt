set(XdmfUtilsCxxTests
	TestXdmfTopologyConverter
)

if(XDMF_BUILD_EXODUS_IO)
	set(XdmfUtilsCxxTests ${XdmfUtilsCxxTests} 	TestXdmfExodusIO)
endif(XDMF_BUILD_EXODUS_IO)

foreach(test ${XdmfUtilsCxxTests})
	add_executable(${test} ${test})
	target_link_libraries(${test} XdmfUtils)
	add_test(${test} ${test})
endforeach(test ${XdmfUtilsCxxTests})
