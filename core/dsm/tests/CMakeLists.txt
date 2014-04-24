add_subdirectory(C)
add_subdirectory(Cxx)

if(XDMF_WRAP_PYTHON)
  add_subdirectory(Python)
endif(XDMF_WRAP_PYTHON)
