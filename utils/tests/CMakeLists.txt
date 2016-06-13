add_subdirectory(C)
add_subdirectory(Cxx)
if(XDMF_WRAP_PYTHON)
  add_subdirectory(Python)
endif(XDMF_WRAP_PYTHON)
# Fortran detection doesn't work properly for CMake
# When BUILD_SHARED_LIBS is off
if(XDMF_BUILD_FORTRAN AND BUILD_SHARED_LIBS)
  add_subdirectory(Fortran)
endif(XDMF_BUILD_FORTRAN AND BUILD_SHARED_LIBS)
