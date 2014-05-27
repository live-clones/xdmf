!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!
!!     AUTHOR: Andrew Burns (andrew.j.burns2@us.army.mil)
!!
!!     A test of the XdmfDSM capabilities using the fortran interface.
!!     This part of the program sets up the server to be used by the DSM.
!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


PROGRAM XdmfFortranExample
 
  Implicit none
  INCLUDE 'Xdmf.f'
  INCLUDE 'mpif.h'

  

  INTEGER*8 obj
  character*256 filename
  INTEGER id, commsize, buffersize
  INTEGER intraID, intraSize
  INTEGER ierr

  INTEGER interComm

  filename = 'nested_output.xmf'//CHAR(0)

  if (buffersize < 1) then
    buffersize = 1
  endif

  CALL MPI_INIT (ierr)

  CALL XDMFINIT(obj, filename)

  CALL MPI_COMM_RANK(MPI_COMM_WORLD, id, ierr)
  CALL MPI_COMM_SIZE(MPI_COMM_WORLD, commsize, ierr)

  buffersize = 16/commsize

!! Cores 1 to size-1 are blocked by the initialization of the server.
!! When the server finishes they are released and allowed to progress.
  CALL XDMFINITDSMSERVER(obj, "dsm"//CHAR(0), MPI_COMM_WORLD, bufferSize, 1, commsize-1)

  if (id == 0) then
    CALL XDMFACCEPTDSM(obj, 2)
  endif

  CALL XDMFGETDSMINTERCOMM(obj, interComm)

!! Wait for other processes to finish before closing

  CALL MPI_BARRIER(interComm, ierr)

  if (id == 0) then
    CALL XDMFCLOSEDSMPORT(obj)
  endif

  CALL XDMFCLOSE(obj)

  CALL MPI_FINALIZE(ierr)

END PROGRAM XdmfFortranExample
