option(XDMF_BUILD_PARTITIONER OFF)

set(XdmfUtilsExecutables)
set(XdmfUtilsSources
	XdmfTopologyConverter
)
set(XdmfUtilsLinkLibraries Xdmf)

if(XDMF_BUILD_PARTITIONER)
	find_package(Metis REQUIRED)
	if(METIS_FOUND)
		include_directories(${METIS_INCLUDE_DIR})
	endif(METIS_FOUND)
	set(XdmfUtilsExecutables ${XdmfUtilsExecutables} XdmfPartitioner)
	set(XdmfUtilsSources ${XdmfUtilsSources} XdmfPartitioner)
	set(XdmfUtilsLinkLibraries ${XdmfUtilsLinkLibraries} ${METIS_LIBRARIES})
endif(XDMF_BUILD_PARTITIONER)

add_library(XdmfUtils ${XdmfUtilsSources})
target_link_libraries(XdmfUtils ${XdmfUtilsLinkLibraries})

foreach(executable ${XdmfUtilsExecutables})
	add_executable(${executable} ${executable})
	set_target_properties(${executable} PROPERTIES COMPILE_FLAGS -DBUILD_EXE)
	target_link_libraries(${executable} XdmfUtils)
endforeach(executable ${XdmfUtilsExecutables})
