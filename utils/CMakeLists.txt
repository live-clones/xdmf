option(BUILD_PARTITIONER OFF)

set(XdmfUtilsExectutables)
set(XdmfUtilsSources)
set(XdmfUtilsLinkLibraries Xdmf)

if(BUILD_PARTITIONER)
	find_package(Metis REQUIRED)
	if(METIS_FOUND)
		include_directories(${METIS_INCLUDE_DIR})
	endif(METIS_FOUND)
	set(XdmfUtilsExectutables ${XdmfUtilsExectutables} XdmfPartitioner)
	set(XdmfUtilsSources ${XdmfUtilsSources} XdmfPartitioner)
	set(XdmfUtilsLinkLibraries ${XdmfUtilsLinkLibraries} ${METIS_LIBRARIES})
endif(BUILD_PARTITIONER)

add_library(XdmfUtils ${XdmfUtilsSources})
target_link_libraries(XdmfUtils ${XdmfUtilsLinkLibraries})

foreach(executable ${XdmfUtilsExectutables})
	add_executable(${executable} ${executable})
	set_target_properties(${executable} PROPERTIES COMPILE_FLAGS -DBUILD_EXE)
	target_link_libraries(${executable} XdmfUtils)
endforeach(executable ${XdmfUtilsExectutables})