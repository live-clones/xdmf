from Xdmf import *

if __name__ == "__main__":
        #Assuming that exampleGrid is a shared pointer to an XdmfCurvilinearGrid object

        #//initialization begin

        newPointsX = 5
        newPointsY = 5
        newPointsZ = 5

        #//initialization end

        #//constructor2 begin

        exampleGrid = XdmfCurvilinearGrid.New(newPointsX, newPointsY)

        #//constructor2 end

        #//constructor3 begin

        exampleGrid = XdmfCurvilinearGrid.New(newPointsX, newPointsY, newPointsZ)

        #//constructor3 end

        #//constructorvector begin

        newPoints = XdmfArray.New()
        newPoints.pushBackAsInt32(5)
        newPoints.pushBackAsInt32(5)
        newPoints.pushBackAsInt32(5)
        exampleGrid = XdmfCurvilinearGrid.New(newPoints)

        #//constructorvector end

        #//setGeometry begin

        newPoints = XdmfArray.New()
        newPoints.pushBackAsInt32(5)
        newPoints.pushBackAsInt32(5)
        newPoints.pushBackAsInt32(5)
        newGeometry = XdmfGeometry.New()
        newGeometry.setType(XdmfGeometryType.XYZ())
        # Start index is 0, 3 values are passed, stride for both arrays is 1
        newGeometry.insert(0, newPoints, 0, 3, 1, 1)
        exampleGrid.setGeometry(newGeometry)

        #//setGeometry end

        #//setDimensions begin

        newPoints = XdmfArray.New()
        newPoints.pushBackAsInt32(5)
        newPoints.pushBackAsInt32(5)
        newPoints.pushBackAsInt32(5)
        exampleGrid.setDimensions(newPoints)

        #//setDimensions end

        #//getGeometry begin

        exampleGeometry = exampleGrid.getGeometry()

        #//getGeometry end

        #//getDimensions begin

        exampleDimensions = exampleGrid.getDimensions()

        #//getDimensions end
