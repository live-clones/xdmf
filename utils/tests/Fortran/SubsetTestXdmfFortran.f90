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
  INTEGER myConnections(8,2)
  REAL*8 myCellAttribute(2), myNodeAttribute(3,4), mySmallerNode(3,2), myTime
  INTEGER nodeAttributeId, nodeSmallAttributeId, cellAttributeId, testSetID, tempID, variableID(3)

  filename = 'subset_output.xmf'//CHAR(0)
  
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

!! Create an InformationArrays to serve as a variable
  variableID(1) = XDMFADDINFORMATION(obj, 'Variable A'//CHAR(0), 'Initializes Variable A'//CHAR(0))
  CALL XDMFADDINFORMATIONARRAY(obj, 0, 'Variable Definition A'//CHAR(0), myPoints, 36, XDMF_ARRAY_TYPE_FLOAT64)

  CALL XDMFSETINFORMATIONARRAYASVARIABLE(obj, 'A'//CHAR(0), 0, 0)

  variableID(2) = XDMFADDINFORMATION(obj, 'Variable B'//CHAR(0), 'Initializes Variable B'//CHAR(0))
  CALL XDMFADDINFORMATIONARRAY(obj, 1, 'Variable Definition B'//CHAR(0), myConnections, 16, XDMF_ARRAY_TYPE_INT32)

  CALL XDMFSETINFORMATIONARRAYASVARIABLE(obj, 'B'//CHAR(0), 1, 0)

  variableID(3) = XDMFADDINFORMATION(obj, 'Variable C'//CHAR(0), 'Initializes Variable C'//CHAR(0))
  CALL XDMFADDINFORMATIONARRAY(obj, 2, 'Variable Definition C'//CHAR(0), myNodeAttribute, 12, XDMF_ARRAY_TYPE_FLOAT64)

  CALL XDMFSETINFORMATIONARRAYASVARIABLE(obj, 'C'//CHAR(0), 2, 0)

  CALL XdmfClearInformations(obj)

  CALL XDMFADDPREVIOUSINFORMATION(obj, variableID(2))

  CALL XDMFSETINFORMATIONARRAYASSUBSETREFERENCE(obj, 0, 0)

  CALL XdmfClearInformations(obj)

  tempID = XDMFSETSUBSETASTOPOLOGY(obj, 0, 1, 12, XDMF_TOPOLOGY_TYPE_HEXAHEDRON, 0)

  CALL XDMFADDPREVIOUSINFORMATION(obj, variableID(1))

  CALL XDMFSETINFORMATIONARRAYASSUBSETREFERENCE(obj, 0, 0)

  CALL XdmfClearInformations(obj)

  tempID = XDMFSETSUBSETASGEOMETRY(obj, 0, 1, 36, XDMF_GEOMETRY_TYPE_XYZ)

  CALL XDMFADDPREVIOUSINFORMATION(obj, variableID(3))

  CALL XDMFSETINFORMATIONARRAYASSUBSETREFERENCE(obj, 0, 0)

  CALL XdmfClearInformations(obj)

  testSetID = XDMFADDSUBSETASSET(obj, 0, 1, 12, 'TestSet'//CHAR(0), XDMF_SET_TYPE_NODE)
  tempID =  XDMFADDINFORMATION(obj, 'Attrib1'//CHAR(0), 'This is Attribute 1'//CHAR(0))
  nodeAttributeId = XDMFADDSUBSETASATTRIBUTE(obj, 0, 1, 12, 'NodeValues'//CHAR(0), &
       XDMF_ATTRIBUTE_CENTER_NODE, XDMF_ATTRIBUTE_TYPE_SCALAR)
  CALL XDMFADDPREVIOUSINFORMATION(obj, variableID(1))

  CALL XDMFSETINFORMATIONARRAYASSUBSETREFERENCE(obj, 0, 0)

  CALL XdmfClearInformations(obj)

  PRINT *, 'Node Attribute ID: ', nodeAttributeId
  tempID = XDMFADDINFORMATION(obj, 'Grid1'//CHAR(0), 'This is Grid 1'//CHAR(0))
  tempID = XDMFADDINFORMATION(obj, 'SubInformation'//CHAR(0), 'This is an information inside an information'//CHAR(0))
  CALL XDMFADDSUBSETASINFORMATIONARRAY(obj, 0, 1, 36, 1, 'Array'//CHAR(0))
  CALL XDMFINSERTINFORMATIONINTOINFORMATION(obj, 0, 1, .TRUE.)
  CALL XDMFADDPREVIOUSINFORMATION(obj, variableID(1))
  CALL XDMFADDPREVIOUSINFORMATION(obj, variableID(2))
  CALL XDMFADDPREVIOUSINFORMATION(obj, variableID(3))
  CALL XDMFADDGRID(obj, 'TestGrid'//CHAR(0), .FALSE.)
  CALL XDMFCLOSEGRIDCOLLECTION(obj, .TRUE.)
  CALL XDMFWRITE(obj, filename, 10, .TRUE.)
  CALL XDMFCLOSE(obj)


END PROGRAM XdmfFortranExample
