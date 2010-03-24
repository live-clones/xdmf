SET(srcs
balance.c
bucketsort.c
ccgraph.c
coarsen.c
compress.c
debug.c
estmem.c
fm.c
fortran.c
frename.c
graph.c
initpart.c
kmetis.c
kvmetis.c
kwayfm.c
kwayrefine.c
kwayvolfm.c
kwayvolrefine.c
match.c
mbalance.c
mbalance2.c
mcoarsen.c
memory.c
mesh.c
meshpart.c
mfm.c
mfm2.c
mincover.c
minitpart.c
minitpart2.c
mkmetis.c
mkwayfmh.c
mkwayrefine.c
mmatch.c
mmd.c
mpmetis.c
mrefine.c
mrefine2.c
mutil.c
myqsort.c
ometis.c
parmetis.c
pmetis.c
pqueue.c
refine.c
separator.c
sfm.c
srefine.c
stat.c
subdomains.c
timing.c
util.c
)

ADD_LIBRARY(metis SHARED ${srcs})

IF(NOT VTK_INSTALL_BIN_DIR_CM24)
  SET(VTK_INSTALL_BIN_DIR_CM24 ${CMAKE_INSTALL_PREFIX}/bin)
ENDIF(NOT VTK_INSTALL_BIN_DIR_CM24)
IF(NOT VTK_INSTALL_LIB_DIR_CM24)
  SET(VTK_INSTALL_LIB_DIR_CM24 ${CMAKE_INSTALL_PREFIX}/lib)
ENDIF(NOT VTK_INSTALL_LIB_DIR_CM24)
IF(NOT VTK_INSTALL_INCLUDE_DIR_CM24)
  SET(VTK_INSTALL_INCLUDE_DIR_CM24 ${CMAKE_INSTALL_PREFIX}/include)
ENDIF(NOT VTK_INSTALL_INCLUDE_DIR_CM24)

IF(NOT VTK_INSTALL_NO_LIBRARIES)
  INSTALL(TARGETS metis
    RUNTIME DESTINATION ${VTK_INSTALL_BIN_DIR_CM24} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${VTK_INSTALL_LIB_DIR_CM24} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${VTK_INSTALL_LIB_DIR_CM24} COMPONENT Development)
ENDIF(NOT VTK_INSTALL_NO_LIBRARIES)
IF(NOT VTK_INSTALL_NO_DEVELOPMENT)
  INSTALL(FILES
    defs.h macros.h metis.h proto.h rename.h struct.h
    DESTINATION ${VTK_INSTALL_INCLUDE_DIR_CM24}/metis/
    COMPONENT Development)
ENDIF(NOT VTK_INSTALL_NO_DEVELOPMENT)
