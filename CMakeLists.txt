PROJECT(Xdmf)

INCLUDE_DIRECTORIES(${Xdmf_SOURCE_DIR}/Ice/libsrc)
INCLUDE_DIRECTORIES(${Xdmf_SOURCE_DIR}/libsrc
                    ${Xdmf_BINARY_DIR}/libsrc
                    ${Xdmf_BINARY_DIR}/Ice/libsrc)

SET (LIBRARY_OUTPUT_PATH ${Xdmf_BINARY_DIR}/bin/ CACHE PATH "Single output directory for building all libraries.")
SET (EXECUTABLE_OUTPUT_PATH ${Xdmf_BINARY_DIR}/bin/ CACHE PATH "Single output directory for building all executables.")

INCLUDE (${CMAKE_ROOT}/Modules/CheckLibraryExists.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/CheckIncludeFile.cmake)

# Build IceConfig.h
MESSAGE(STATUS "Configure IceConfig.h")
SUBDIRS(Ice/libsrc)

OPTION(XDMF_HAS_NDGM "XDMF has Network Distributed Global Memory (NDGM)" OFF)
IF(XDMF_HAS_NDGM)
	FIND_PATH(NDGM_DIR BuildNDGM.cmake ${Xdmf_SOURCE_DIR}/../Ndgm "Root of Source for NDGM")
	IF(NDGM_DIR)
		MESSAGE(STATUS "Process NDGM")
#		INCLUDE(${NDGM_DIR}/BuildNDGM.cmake)
		SUBDIRS(NDGM)
	ENDIF(NDGM_DIR)
	ADD_DEFINITIONS(-DHAVE_NDGM)
ENDIF(XDMF_HAS_NDGM)

OPTION(XDMF_SYSTEM_HDF5 "Use sytstem HDF5" ON)

IF(XDMF_SYSTEM_HDF5)
  FIND_LIBRARY(HDF5_LIBRARY
	hdf5
	/usr/lib
	/opt/lib
	/usr/local/lib)
  FIND_PATH(HDF5_INCLUDE_PATH
	H5Ipublic.h
	/usr/include
	/opt/include
	/usr/local/include)
  MESSAGE(STATUS "Using system HDF5")
  INCLUDE_DIRECTORIES(${HDF5_INCLUDE_PATH})
  SET(HDF5_LIBRARY ${HDF5_LIBRARY})
ELSE(XDMF_SYSTEM_HDF5)
  INCLUDE_DIRECTORIES(${Xdmf_SOURCE_DIR}/Utilities/hdf5
                      ${Xdmf_BINARY_DIR}/Utilities/hdf5)
  SUBDIRS(Utilities)
  SET(HDF5_LIBRARY vtkhdf5)
ENDIF(XDMF_SYSTEM_HDF5)

FIND_PATH(EXPAT_INCLUDE_DIR expat.h
  /usr/local/include
  /usr/include
)

FIND_LIBRARY(EXPAT_LIBRARY expat
  /usr/lib
  /usr/local/lib
)

INCLUDE_DIRECTORIES(${EXPAT_INCLUDE_DIR})

SUBDIRS(libsrc binsrc)

OPTION(XDMF_BUILD_VTK "Build VTK reader" OFF)
IF(XDMF_BUILD_VTK)
  SUBDIRS(vtk)
ENDIF(XDMF_BUILD_VTK)

# Add the testing directories
OPTION(XDMF_BUILD_TESTING "Test the project" OFF)
IF(XDMF_BUILD_TESTING)
  ENABLE_TESTING()
  INCLUDE (${CMAKE_ROOT}/Modules/Dart.cmake)
ENDIF(XDMF_BUILD_TESTING)
