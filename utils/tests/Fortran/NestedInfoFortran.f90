!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!
!!     AUTHOR: Andrew Burns (andrew.j.burns2@us.army.mil)
!!
!!     Use the Xdmf Fortran Bindings to write out several links to 
!!     data structures in different files.
!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


PROGRAM XdmfFortranExample
 
  Implicit none
  INCLUDE 'Xdmf.f'

  

  INTEGER*8 obj
  character*256 filename
  INTEGER tempID;

  filename = 'nested_output.xmf'//CHAR(0)

  CALL XDMFINIT(obj, filename)

  tempID = XDMFADDINFORMATION(obj, 'XIncludes'//CHAR(0), '3'//CHAR(0))
  tempID = XDMFADDINFORMATION(obj, "testoutput.xmf"//CHAR(0), "xpointer(//Xdmf/Domain/Grid[1])"//CHAR(0))
  tempID = XDMFADDINFORMATION(obj, "testoutput2.xmf"//CHAR(0), "xpointer(//Xdmf/Domain/Grid[1])"//CHAR(0))
  tempID = XDMFADDINFORMATION(obj, "editedtestoutput.xmf"//CHAR(0), "xpointer(//Xdmf/Domain/Grid[1])"//CHAR(0))
  CALL XDMFINSERTINFORMATIONINTOINFORMATION(obj, 0, 1, .TRUE.)
  CALL XDMFINSERTINFORMATIONINTOINFORMATION(obj, 0, 1, .TRUE.)
  CALL XDMFINSERTINFORMATIONINTOINFORMATION(obj, 0, 1, .TRUE.)
!! The Information will be added to the Grid Collection
  CALL XDMFADDGRIDCOLLECTION(obj, "MultiFile Reference"//CHAR(0), &
                             XDMF_GRID_COLLECTION_TYPE_TEMPORAL)
  CALL XDMFCLOSEGRIDCOLLECTION(obj, .TRUE.)
  CALL XDMFWRITE(obj, filename, 10, .TRUE.)
  CALL XDMFCLOSE(obj)


END PROGRAM XdmfFortranExample
