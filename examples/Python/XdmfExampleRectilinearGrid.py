from Xdmf import *

if __name__ == "__main__":
        #Assuming that exampleGrid is a shared pointer to an XdmfRectilinearGrid

        #//initvalues

        pointsXArray = XdmfArray.New()
        pointsXArray.pushBackAsInt32(5)
        pointsXArray.pushBackAsInt32(6)
        pointsXArray.pushBackAsInt32(7)
        pointsXArray.pushBackAsInt32(8)
        pointsXArray.pushBackAsInt32(9)
        pointsXArray.pushBackAsInt32(10)
        pointsYArray = XdmfArray.New()
        pointsYArray.pushBackAsInt32(3)
        pointsYArray.pushBackAsInt32(6)
        pointsYArray.pushBackAsInt32(4)
        pointsYArray.pushBackAsInt32(8)
        pointsYArray.pushBackAsInt32(7)
        pointsYArray.pushBackAsInt32(10)
        pointsZArray = XdmfArray.New()
        pointsZArray.pushBackAsInt32(3)
        pointsZArray.pushBackAsInt32(9)
        pointsZArray.pushBackAsInt32(4)
        pointsZArray.pushBackAsInt32(5)
        pointsZArray.pushBackAsInt32(7)
        pointsZArray.pushBackAsInt32(2)

        #//initvalues

        #//initialization2

        exampleGrid = XdmfRectilinearGrid.New(pointsXArray, pointsYArray)

        #//initialization2

        #//initialization3

        exampleGrid = XdmfRectilinearGrid.New(pointsXArray, pointsYArray, pointsZArray)

        #//initialization3

        #//setCoordinatessingle

        exampleGrid.setCoordinates(0, pointsXArray)

        #//setCoordinatessingle

        #//initializevector

        pointsCollector = ArrayVector()
        pointsCollector.push_back(pointsXArray)
        pointsCollector.push_back(pointsYArray)
        pointsCollector.push_back(pointsZArray)
        exampleGrid = XdmfRectilinearGrid.New(pointsCollector)

        #//initializevector

        #//getCoordinatessingle

        readPointsX = exampleGrid.getCoordinates(0)
        readPointsY = exampleGrid.getCoordinates(1)

        #//getCoordinatessingle

        #//getCoordinatesvector

        exampleCoordinates = exampleGrid.getCoordinates()

        #//getCoordinatesvector

        #//getDimensions

        exampleDimensions = exampleGrid.getDimensions()

        #//getDimensions

        #//setCoordinatesvector

        exampleGrid.setCoordinates(pointsCollector)

        #//setCoordinatesvector
