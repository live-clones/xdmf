PROJECT(Xdmf)

INCLUDE_DIRECTORIES(
  ${Xdmf_SOURCE_DIR}/Ice
  ${Xdmf_BINARY_DIR}/Ice
  ${Xdmf_SOURCE_DIR}/libsrc
  ${Xdmf_BINARY_DIR}/libsrc
  ${Xdmf_SOURCE_DIR}/vtk
  ${Xdmf_BINARY_DIR}/vtk
  )
SUBDIRS(Ice)
SUBDIRS(libsrc)
SUBDIRS(vtk)
