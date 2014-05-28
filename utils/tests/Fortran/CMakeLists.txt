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
if (MPIEXEC_MAX_NUMPROCS STRGREATER 5)
  get_filename_component(MPI_BIN_DIRECTORY ${MPI_C_COMPILER} PATH)
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
ADD_TEST_FORTRAN(OutputTestXdmfFortran)
ADD_TEST_FORTRAN(EditTestXdmfFortran)
ADD_TEST_FORTRAN(TestXdmfFortran)
ADD_TEST_FORTRAN(NestedInfoFortran)
SET_SOURCE_FILES_PROPERTIES(FixedOutputTestXdmfFortran.f90 PROPERTIES COMPILE_FLAGS -ffixed-form)
ADD_TEST_FORTRAN(FixedOutputTestXdmfFortran -ffixed-form)

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
