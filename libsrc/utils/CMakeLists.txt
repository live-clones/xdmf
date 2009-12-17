SET( XdmfUtilSource
  XdmfFortran
  XdmfDiff
  )

SET( XdmfSTLConverterSource
    XdmfSTLConverter
    )

#SET(swig_extra_args ${swig_extra_args} "-DXDMF_BUILD_UTILS")
   	
ADD_LIBRARY(XdmfUtils ${LIBTYPE} ${XdmfUtilSource})
TARGET_LINK_LIBRARIES(XdmfUtils Xdmf)
ADD_EXECUTABLE(XdmfDiff XdmfDiffUtil)
TARGET_LINK_LIBRARIES(XdmfDiff XdmfUtils)

INSTALL(TARGETS XdmfDiff XdmfUtils
    RUNTIME DESTINATION ${XDMF_INSTALL_BIN_DIR_CM24} COMPONENT Runtime
    LIBRARY DESTINATION ${XDMF_INSTALL_LIB_DIR_CM24} COMPONENT Runtime
    ARCHIVE DESTINATION ${XDMF_INSTALL_LIB_DIR_CM24} COMPONENT Development)
