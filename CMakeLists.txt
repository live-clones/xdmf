OPTION(XDMF_BUILD_SHARED_LIBS "Build Shared XDMF Library" ${ICE_BUILD_SHARED_LIBS})
OPTION(XDMF_HAVE_NDGM "Build NDGM Support into XDMF" ${ICE_HAVE_NDGM})
OPTION(XDMF_BUILD_PYTHON "Wrap classes into the Python interpreted language" ${ICE_BUILD_PYTHON})
FIND_LIBRARY(XDMF_Z_LIBRARY z
	/usr/lib
	/usr/local/lib
	)
LINK_DIRECTORIES(${XDMF_Z_LIBRARY})
SUBDIRS(libsrc binsrc vtk)
