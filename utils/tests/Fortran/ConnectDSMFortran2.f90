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
  INTEGER id, commsize, buffersize, statusval(MPI_STATUS_SIZE)
  INTEGER intraID, intraSize
  INTEGER interID, interSize
  INTEGER ierr
  INTEGER writeloopcontrol, i
  REAL*8 dataArray(4)

  INTEGER interComm, intraComm

  filename = 'nested_output.xmf'//CHAR(0)

  buffersize = 16/commsize

  if (buffersize < 1) then
    buffersize = 1
  endif

  writeloopcontrol = 0

  CALL MPI_INIT (ierr)

  CALL XDMFINIT(obj, filename)

  CALL MPI_COMM_RANK(MPI_COMM_WORLD, id, ierr)
  CALL MPI_COMM_SIZE(MPI_COMM_WORLD, commsize, ierr)

  CALL XDMFCONNECTDSM(obj, "dsm"//CHAR(0), MPI_COMM_WORLD)

  CALL XDMFGETDSMINTRACOMM(obj, intraComm)

  CALL MPI_COMM_RANK(intraComm, intraID, ierr)
  CALL MPI_COMM_SIZE(intraComm, intraSize, ierr)

  CALL MPI_BARRIER(intraComm, ierr)

  CALL XDMFGETDSMINTERCOMM(obj, interComm)

  CALL MPI_COMM_RANK(interComm, interID, ierr)
  CALL MPI_COMM_SIZE(interComm, interSize, ierr)

!! Work is done here

  do while (writeloopcontrol < 3)

    if (id == 0) then
      CALL MPI_RECV(writeloopcontrol, 1, MPI_INT, interID - 1, 0, interComm, statusval, ierr)
    endif

    CALL XDMFREADFROMDSM(obj, "Data"//CHAR(0), XDMF_ARRAY_TYPE_FLOAT64, dataArray, id*4, 1, 4, 4*commsize)

    i = 1

    do while (i <= 4)
      dataArray(i) = dataArray(i) * 2
      i = i + 1
    enddo

    CALL XDMFWRITETODSM(obj, "Data"//CHAR(0), XDMF_ARRAY_TYPE_FLOAT64, dataArray, id*4, 1, 4, 4*commsize)

    if (id == 0) then
      CALL MPI_SEND(writeloopcontrol, 1, MPI_INT, interID - 1, 0, interComm, ierr)
    endif

    writeloopcontrol = writeloopcontrol + 1

  enddo

!! Wait for other processes to finish before closing

  CALL MPI_BARRIER(interComm, ierr)

  CALL XDMFCLOSE(obj)

  CALL MPI_FINALIZE(ierr)

END PROGRAM XdmfFortranExample
