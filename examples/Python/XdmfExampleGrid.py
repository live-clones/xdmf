from Xdmf import *

if __name__ == "__main__":
        #//initialization begin

        exampleGrid = XdmfUnstructuredGrid.New()

        #Using an unstructured grid since XdmfGrid is an abstract class

        #//initialization end

        #//setName begin

        newName = "New Name"
        exampleGrid.setName(newName)

        #//setName end

        #//setTime begin

        newTime = XdmfTime.New(20.0)
        exampleGrid.setTime(newTime)

        #//setTIme end

        #//getTime begin

        exampleTime = exampleGrid.getTime()

        #//getTime end

        #//getName begin

        exampleName = exampleGrid.getName()

        #//getName end

        #//getGeometry begin

        exampleGeometry = exampleGrid.getGeometry()

        #//getGeometry end

        #//getTopology begin

        exampleTopology = exampleGrid.getTopology()

        #//getTopology end
