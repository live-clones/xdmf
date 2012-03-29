!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!
!!     AUTHOR: Kenneth Leiter (kenneth.leiter@arl.army.mil)
!!
!!     Use the Xdmf Fortran Bindings to write out a simple mesh consisting of
!!     two hexahedrons.  Link against the XdmfUtils library to compile.
!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

PROGRAM XdmfFortranExample
  
  IMPLICIT NONE
  INCLUDE 'Xdmf.f'

  INTEGER*8 obj
  character*256 filename
  REAL*4 myPoints(3,3,4)
  INTEGER myConnections(8,2)
  REAL*8 myCellAttribute(2), myNodeAttribute(3,4), myTime

  filename = 'my_output'//CHAR(0)
  
  myPoints(1,1,1) = 0
  myPoints(2,1,1) = 0
  myPoints(3,1,1) = 1
  myPoints(1,2,1) = 1
  myPoints(2,2,1) = 0
  myPoints(3,2,1) = 1
  myPoints(1,3,1) = 3
  myPoints(2,3,1) = 0
  myPoints(3,3,1) = 2
  myPoints(1,1,2) = 0
  myPoints(2,1,2) = 1
  myPoints(3,1,2) = 1
  myPoints(1,2,2) = 1
  myPoints(2,2,2) = 1
  myPoints(3,2,2) = 1
  myPoints(1,3,2) = 3
  myPoints(2,3,2) = 2
  myPoints(3,3,2) = 2
  myPoints(1,1,3) = 0
  myPoints(2,1,3) = 0
  myPoints(3,1,3) = -1
  myPoints(1,2,3) = 1
  myPoints(2,2,3) = 0
  myPoints(3,2,3) = -1
  myPoints(1,3,3) = 3
  myPoints(2,3,3) = 0
  myPoints(3,3,3) = -2
  myPoints(1,1,4) = 0
  myPoints(2,1,4) = 1
  myPoints(3,1,4) = -1
  myPoints(1,2,4) = 1
  myPoints(2,2,4) = 1
  myPoints(3,2,4) = -1
  myPoints(1,3,4) = 3
  myPoints(2,3,4) = 2
  myPoints(3,3,4) = -2
  
  myConnections(1,1) = 0
  myConnections(2,1) = 1
  myConnections(3,1) = 7
  myConnections(4,1) = 6
  myConnections(5,1) = 3
  myConnections(6,1) = 4
  myConnections(7,1) = 10
  myConnections(8,1) = 9
  myConnections(1,2) = 1
  myConnections(2,2) = 2
  myConnections(3,2) = 8
  myConnections(4,2) = 7
  myConnections(5,2) = 4
  myConnections(6,2) = 5
  myConnections(7,2) = 11
  myConnections(8,2) = 10
  
  myNodeAttribute(1,1) = 100
  myNodeAttribute(1,2) = 300
  myNodeAttribute(1,3) = 300
  myNodeAttribute(1,4) = 500
  myNodeAttribute(2,1) = 200
  myNodeAttribute(2,2) = 400
  myNodeAttribute(2,3) = 400
  myNodeAttribute(2,4) = 600
  myNodeAttribute(3,1) = 300
  myNodeAttribute(3,2) = 500
  myNodeAttribute(3,3) = 500
  myNodeAttribute(3,4) = 700
  
  myCellAttribute(1) = 100
  myCellAttribute(2) = 200
  
  myTime = 1.0

  CALL XDMFINIT(obj, filename)
  CALL XDMFADDGRIDCOLLECTION(obj, "Temporal"//CHAR(0), &
       XDMF_GRID_COLLECTION_TYPE_TEMPORAL)
  CALL XDMFSETTIME(obj, myTime)
  CALL XDMFSETTOPOLOGY(obj, XDMF_TOPOLOGY_TYPE_HEXAHEDRON, 16, &
       XDMF_ARRAY_TYPE_INT32, myConnections)
  CALL XDMFSETGEOMETRY(obj, XDMF_GEOMETRY_TYPE_XYZ, 36, &
       XDMF_ARRAY_TYPE_FLOAT32, myPoints)
  CALL XDMFADDATTRIBUTE(obj, 'NodeValues'//CHAR(0), &
       XDMF_ATTRIBUTE_CENTER_NODE, XDMF_ATTRIBUTE_TYPE_SCALAR, 12, &
       XDMF_ARRAY_TYPE_FLOAT64, myNodeAttribute)
  CALL XDMFADDATTRIBUTE(obj, 'CellValues'//CHAR(0), &
       XDMF_ATTRIBUTE_CENTER_CELL, XDMF_ATTRIBUTE_TYPE_SCALAR, 2, &
       XDMF_ARRAY_TYPE_FLOAT64, myCellAttribute)
  CALL XDMFADDINFORMATION(obj, 'Key'//CHAR(0), 'Value'//CHAR(0))
  CALL XDMFADDGRID(obj, 'TestGrid'//CHAR(0))

  myTime = 2.0

  CALL XDMFSETTIME(obj, myTime)
  CALL XDMFADDGRID(obj, 'Identical'//CHAR(0))
  CALL XDMFCLOSEGRIDCOLLECTION(obj)
  CALL XDMFWRITE(obj, filename)
  CALL XDMFCLOSE(obj)

END PROGRAM XdmfFortranExample
       
