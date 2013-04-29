!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!
!!     AUTHOR: Kenneth Leiter (kenneth.leiter@arl.army.mil)
!!
!!     Use the Xdmf Fortran Bindings to write out a simple mesh consisting of
!!     two hexahedrons.  Link against the XdmfUtils library to compile.
!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


PROGRAM XdmfFortranExample
 
  Implicit none
  INCLUDE 'Xdmf.f'

  

  INTEGER*8 obj
  character*256 filename
  REAL*4 myPoints(3,3,4)
  INTEGER myConnections(8,2), myDimensions(3)
  REAL*8 myCellAttribute(2), myNodeAttribute(3,4), mySmallerNode(3,2), myTime, myOrigin(3), myBrick(3)
  INTEGER nodeAttributeId, nodeSmallAttributeId, cellAttributeId, testSetID, testMapID, tempID

  filename = 'my_output.xmf'//CHAR(0)
  
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

  myDimensions(1) = 12
  myDimensions(2) = 12
  myDimensions(3) = 12

  myOrigin(1) = 0
  myOrigin(2) = 0
  myOrigin(3) = 0

  myBrick(1) = 12
  myBrick(2) = 12
  myBrick(3) = 12

  myTime = 1.0

  CALL XDMFINIT(obj, filename)

  CALL XDMFSETMAXFILESIZE(obj, 1)
  CALL XDMFSETALLOWSETSPLITTING(obj, .TRUE.)
  CALL XDMFINITHDF5(obj, 'my_output.h5'//CHAR(0), .TRUE.)

  tempID = XDMFADDINFORMATION(obj, 'GridCollection1'//CHAR(0), 'This is Grid collection 1'//CHAR(0))
  CALL XDMFADDGRIDCOLLECTION(obj, "Temporal"//CHAR(0), &
       XDMF_GRID_COLLECTION_TYPE_TEMPORAL)
  CALL XDMFADDMAP(obj, "TestMap"//CHAR(0))
  CALL XDMFADDREMOTENODEID(obj, 0, 1, 2, 3)
  CALL XDMFADDREMOTENODEID(obj, 0, 1, 2, 4)
  CALL XDMFADDREMOTENODEID(obj, 0, 1, 3, 3)
  CALL XDMFADDREMOTENODEID(obj, 0, 1, 3, 5)
  testMapID = XDMFSTOREMAP(obj, 0)
  CALL XDMFADDREMOTENODEID(obj, 0, 1, 3, 8)
  CALL XDMFSETTIME(obj, myTime)
!! Unstructured Only
  tempID = XDMFSETTOPOLOGY(obj, XDMF_TOPOLOGY_TYPE_HEXAHEDRON, 16, &
       XDMF_ARRAY_TYPE_INT32, myConnections, 0)
!! /Unstructured Only
!! Curvilinear and Rectilinear Only
  tempID = XDMFSETDIMENSIONS(obj, 3, XDMF_ARRAY_TYPE_INT32, myDimensions)
!! /Curvilinear and Rectilinear Only
!! Unstructured and Curvilinear Only
  tempID = XDMFSETGEOMETRY(obj, XDMF_GEOMETRY_TYPE_XYZ, 36, &
       XDMF_ARRAY_TYPE_FLOAT32, myPoints)
!! /Unstructured and Curvilinear Only
!! Rectilinear Only
  tempID = XDMFADDCOORDINATE(obj, "XCoordinates"//CHAR(0), 12, XDMF_ARRAY_TYPE_FLOAT32, myPoints(1,1,1))
  tempID = XDMFADDCOORDINATE(obj, "YCoordinates"//CHAR(0), 12, XDMF_ARRAY_TYPE_FLOAT32, myPoints(1,2,2))
  tempID = XDMFADDCOORDINATE(obj, "ZCoordinates"//CHAR(0), 12, XDMF_ARRAY_TYPE_FLOAT32, myPoints(1,3,3))
!! /Rectilinear Only
!! Regular Only
  tempID = XDMFSETORIGIN(obj, 3, XDMF_ARRAY_TYPE_FLOAT64, myOrigin)
  tempID = XDMFSETBRICK(obj, 3, XDMF_ARRAY_TYPE_FLOAT64, myBrick)
!! /Regular Only
  testSetID = XDMFADDSET(obj, 'TestSet'//CHAR(0), XDMF_SET_TYPE_NODE, myNodeAttribute,  12, XDMF_ARRAY_TYPE_FLOAT64)
  tempID =  XDMFADDINFORMATION(obj, 'Attrib1'//CHAR(0), 'This is Attribute 1'//CHAR(0))
  nodeAttributeId = XDMFADDATTRIBUTE(obj, 'NodeValues'//CHAR(0), &
       XDMF_ATTRIBUTE_CENTER_NODE, XDMF_ATTRIBUTE_TYPE_SCALAR, 12, &
       XDMF_ARRAY_TYPE_FLOAT64, myNodeAttribute)
  CALL XDMFRETRIEVEATTRIBUTEVALUES(obj, 0, mySmallerNode, XDMF_ARRAY_TYPE_FLOAT64, 6, 0, 1, 1)
  tempID =  XDMFADDINFORMATION(obj, 'Attrib2'//CHAR(0), 'This is Attribute 2'//CHAR(0))
  cellAttributeId = XDMFADDATTRIBUTE(obj, 'CellValues'//CHAR(0), &
       XDMF_ATTRIBUTE_CENTER_CELL, XDMF_ATTRIBUTE_TYPE_SCALAR, 2, &
       XDMF_ARRAY_TYPE_FLOAT64, myCellAttribute)
  nodeSmallAttributeId = XDMFADDATTRIBUTE(obj, 'SmallNodeValues'//CHAR(0), &
       XDMF_ATTRIBUTE_CENTER_NODE, XDMF_ATTRIBUTE_TYPE_SCALAR, 6, &
       XDMF_ARRAY_TYPE_FLOAT64, mySmallerNode)  
  tempID = XDMFADDINFORMATION(obj, 'Grid1'//CHAR(0), 'This is Grid 1'//CHAR(0))
  CALL XDMFADDGRID(obj, 'TestGrid'//CHAR(0), .FALSE.)
  myTime = 2.0

  CALL XDMFSETTIME(obj, myTime)
  CALL XDMFADDPREVIOUSATTRIBUTE(obj, cellAttributeId)
  CALL XDMFADDPREVIOUSMAP(obj, testMapID)
  CALL XDMFADDPREVIOUSSET(obj, testSetID)
  CALL XDMFADDPREVIOUSATTRIBUTE(obj, nodeAttributeId)
  CALL XDMFADDGRID(obj, 'Identical'//CHAR(0), .FALSE.)
  CALL XDMFCLOSEGRIDCOLLECTION(obj, .TRUE.)
!!  CALL XDMFWRITEHDF5(obj, 'my_output.h5'//CHAR(0), .TRUE.)
  CALL XDMFWRITE(obj, filename, 10, .TRUE.)
  CALL XDMFCLOSE(obj)


END PROGRAM XdmfFortranExample
