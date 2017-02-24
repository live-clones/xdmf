set(XDMF_SUBSECTION "Core")

add_subdirectory(C)
add_subdirectory(Cxx)

if(XDMF_WRAP_PYTHON)
  add_subdirectory(Python)
endif(XDMF_WRAP_PYTHON)

if(XDMF_WRAP_JAVA)
  add_subdirectory(Java)
endif(XDMF_WRAP_JAVA)
