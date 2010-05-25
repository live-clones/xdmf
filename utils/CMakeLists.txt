option(BUILD_PARTITIONER OFF)

set(XdmfUtilsSources)
set(XdmfUtilsLibraries)

if(BUILD_PARTITIONER)
  find_package(Metis REQUIRED)
  if(METIS_FOUND)
    include_directories(${METIS_INCLUDE_DIR})
  endif(METIS_FOUND)
  set(XdmfUtilsSources ${XdmfUtilsSources} XdmfPartitioner)
  set(XdmfUtilsLibraries ${XdmfUtilsLibraries} ${METIS_LIBRARY})
endif(BUILD_PARTITIONER)

add_library(XdmfUtils ${XdmfUtilsSources})
target_link_libraries(XdmfUtils Xdmf)
