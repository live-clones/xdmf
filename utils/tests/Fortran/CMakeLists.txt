# Add any dependencies that the cxx tests may need
# Note: The tests already depend on their own file
SET_PROPERTY(GLOBAL APPEND PROPERTY FORTRAN_TEST_DEPENDENCIES
  "XdmfUtils")

# Include XdmfTestDataGenerator from non-util tests
include_directories(${CMAKE_SOURCE_DIR}/tests/Fortran/)

# Include our test macros
include(AddTestsFortran)

# Add any cxx tests here:
# Note: We don't want to use a foreach loop to test the files incase we
#       have extra arguments (id: ADD_TEST_CXX(testname inputfile))
#       Read UseCxxTest.cmake for more information
# ---------------------------------------
if ("${XDMF_DSM_IS_CRAY}" STREQUAL "")
  if (MPIEXEC_MAX_NUMPROCS STRGREATER 5)
    EXECUTE_PROCESS(
      COMMAND ${MPIEXEC} --version
      OUTPUT_VARIABLE MPI_TYPE_OUTPUT
      ERROR_VARIABLE MPI_TYPE_ERROR
    )
    STRING(REGEX MATCH "Open" IS_OPENMPI "${MPI_TYPE_ERROR}")
    IF ("${IS_OPENMPI}" STREQUAL "")
      ADD_MPI_TEST_FORTRAN(ConnectDSMFortran.sh AcceptDSMFortran,ConnectDSMFortran2,ConnectDSMFortran)
    ENDIF ("${IS_OPENMPI}" STREQUAL "")
  endif(MPIEXEC_MAX_NUMPROCS STRGREATER 5)
endif ("${XDMF_DSM_IS_CRAY}" STREQUAL "")
SET_SOURCE_FILES_PROPERTIES(BinaryFortran.f90 PROPERTIES COMPILE_FLAGS -fno-range-check)
ADD_TEST_FORTRAN(OutputTestXdmfFortran)
ADD_TEST_FORTRAN(EditTestXdmfFortran)
ADD_TEST_FORTRAN(TestXdmfFortran)
ADD_TEST_FORTRAN(NestedInfoFortran)
#if GCC
STRING(REGEX MATCH "gfortran" IS_GCC "${CMAKE_Fortran_COMPILER}")
IF ("${IS_GCC}" STREQUAL "gfortran")
  SET_SOURCE_FILES_PROPERTIES(FixedOutputTestXdmfFortran.f90 PROPERTIES COMPILE_FLAGS "-ffixed-form")
ENDIF ("${IS_GCC}" STREQUAL "gfortran")
#if Intel
STRING(REGEX MATCH "ifort" IS_INTEL "${CMAKE_Fortran_COMPILER}")
IF ("${IS_INTEL}" STREQUAL "ifort")
  SET_SOURCE_FILES_PROPERTIES(FixedOutputTestXdmfFortran.f90 PROPERTIES COMPILE_FLAGS "-fixed")
ENDIF ("${IS_INTEL}" STREQUAL "ifort")
ADD_TEST_FORTRAN(FixedOutputTestXdmfFortran)
ADD_TEST_FORTRAN(FunctionTestXdmfFortran)
ADD_TEST_FORTRAN(SubsetTestXdmfFortran)

# Add any cxx cleanup here:
# Note: We don't want to use a foreach loop to test the files incase we
#       have multiple files (ie: CLEAN_TEST_CXX(testname outputfile1 ...))
#       Read UseCxxTest.cmake for more information
# ---------------------------------------
CLEAN_TEST_FORTRAN(OutputTestXdmfFortran)
CLEAN_TEST_FORTRAN(TestXdmfFortran
  my_output.h5
  my_output.xmf)
CLEAN_TEST_FORTRAN(NestedInfoFortran
  nested_output.xmf
  nested_output.h5)
CLEAN_TEST_FORTRAN(EditTestXdmfFortran
  edited_output.xmf
  edited_output.h5)
CLEAN_TEST_FORTRAN(FixedOutputTestXdmfFortran)
CLEAN_TEST_FORTRAN(FunctionTestXdmfFortran
  function_output.h5
  function_output.xmf)
CLEAN_TEST_FORTRAN(SubsetTestXdmfFortran
  subset_output.h5
  subset_output.xmf)
