!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!
!!     AUTHOR: Kenneth Leiter (kenneth.leiter@arl.army.mil)
!!
!!     Use the Xdmf Fortran Bindings to write out a simple mesh consisting of
!!     two hexahedrons.  Link against the XdmfFortran library to compile.
!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

       PROGRAM XdmfFortranExample
       
       !DEC$ ATTRIBUTES DLLIMPORT, ALIAS:'_XdmfInit'::XdmfInit
       !DEC$ ATTRIBUTES DLLIMPORT, ALIAS:'_XdmfSetGridTopology'::XdmfSetGridTopology
       !DEC$ ATTRIBUTES DLLIMPORT, ALIAS:'_XdmfSetGridGeometry'::XdmfSetGridGeometry
       !DEC$ ATTRIBUTES DLLIMPORT, ALIAS:'_XdmfSetGridAttribute'::XdmfSetGridAttribute
       !DEC$ ATTRIBUTES DLLIMPORT, ALIAS:'_XdmfAddGridAttribute'::XdmfAddGridAttribute
       !DEC$ ATTRIBUTES DLLIMPORT, ALIAS:'_XdmfWriteGrid'::XdmfWriteGrid
       !DEC$ ATTRIBUTES DLLIMPORT, ALIAS:'_XdmfWriteToFile'::XdmfWriteToFile
       !DEC$ ATTRIBUTES DLLIMPORT, ALIAS:'_XdmfSerialize'::XdmfSerialize
       !DEC$ ATTRIBUTES DLLIMPORT, ALIAS:'_XdmfClose'::XdmfClose
       
       INTEGER*8 obj
       character*256 filename
       REAL*4 myPoints(3,5)
       INTEGER myConnections(3,3)
       REAL*8  myNodeAttribute(5)
       
       filename = 'my_output'//CHAR(0)
       
       myPoints(1,1) = 0
       myPoints(2,1) = 0
       myPoints(3,1) = 0

       myPoints(1,2) = 1
       myPoints(2,2) = 2
       myPoints(3,2) = 0

       myPoints(1,3) = 2
       myPoints(2,3) = 0
       myPoints(3,3) = 0

       myPoints(1,4) = 3
       myPoints(2,4) = 2
       myPoints(3,4) = 0

       myPoints(1,5) = 4
       myPoints(2,5) = 0
       myPoints(3,5) = 0

       myConnections(1,1) = 0
       myConnections(2,1) = 1
       myConnections(3,1) = 2

       myConnections(1,2) = 2
       myConnections(2,2) = 1
       myConnections(3,2) = 3

       myConnections(1,3) = 2
       myConnections(2,3) = 3
       myConnections(3,3) = 4

       myNodeAttribute(1) = 100
       myNodeAttribute(2) = 300
       myNodeAttribute(3) = 300
       myNodeAttribute(4) = 500
       myNodeAttribute(5) = 200

       CALL XDMFINIT(obj, filename)
       CALL XDMFSETGRIDTOPOLOGY(obj, 'Triangle'//CHAR(0), 3, myConnections)
       CALL XDMFSETGRIDGEOMETRY(obj, 'XYZ'//CHAR(0), 'XDMF_FLOAT32_TYPE'//CHAR(0), 5, myPoints)
       CALL XDMFADDGRIDATTRIBUTE(obj, 'NodeValues'//CHAR(0),'XDMF_FLOAT64_TYPE'//CHAR(0), 'NODE'//CHAR(0), &
       'SCALAR'//CHAR(0), 5, myNodeAttribute)
       CALL XDMFWRITEGRID(obj, 'TestGrid'//CHAR(0))
       CALL XDMFWRITETOFILE(obj)
       CALL XDMFSERIALIZE(obj)
       CALL XDMFCLOSE(obj)
       END

