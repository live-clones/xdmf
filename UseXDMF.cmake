#
# This module is provided as XDMF_USE_FILE by XDMFConfig.cmake.  It can
# be INCLUDEd in a project to load the needed compiler and linker
# settings to use XDMF.
#

# Load the compiler settings used for XDMF.
IF(XDMF_BUILD_SETTINGS_FILE)
  INCLUDE(${CMAKE_ROOT}/Modules/CMakeImportBuildSettings.cmake)
  CMAKE_IMPORT_BUILD_SETTINGS(${XDMF_BUILD_SETTINGS_FILE})
ENDIF(XDMF_BUILD_SETTINGS_FILE)

# Add include directories needed to use XDMF.
INCLUDE_DIRECTORIES(${XDMF_INCLUDE_DIRS})

# Add link directories needed to use XDMF.
LINK_DIRECTORIES(${XDMF_LIBRARY_DIRS})
