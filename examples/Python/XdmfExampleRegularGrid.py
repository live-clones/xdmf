from Xdmf import *

if __name__ == "__main__":
	#Assuming exampleGrid is a shared pointer to an XdmfRegularGrid object

	#//initvalues begin

	newBrickX = 20.0
	newPointsX = 5
	newOriginX = 0.0
	newBrickY = 20.0
	newPointsY = 5
	newOriginY = 0.0
	newBrickZ = 20.0
	newPointsZ = 5
	newOriginZ = 0.0

	#//initvalues end

	#//initialization2 begin

	exampleGrid = XdmfRegularGrid.New(
		newBrickX, newBrickY, newPointsX, newPointsY, newOriginX, newOriginY)

	#//initialization2 end

	#//initialization3 begin

	exampleGrid = XdmfRegularGrid.New(
		newBrickX, newBrickY, newBrickZ, newPointsX, newPointsY, newPointsZ, newOriginX, newOriginY, newOriginZ)

	#//initialization3 end

	#//getBrickSize begin

	exampleBrick = exampleGrid.getBrickSize()

	#//getBrickSize end

	#//getDimensions begin

	exampleDimensions = exampleGrid.getDimensions()

	#//getDimensions end

	#//getOrigin begin

	exampleOrigin = exampleGrid.getOrigin()

	#//getOrigin end

	#//initializationvector begin

	newBrickSize = XdmfArray.New()
	newBrickSize.pushBackAsFloat64(20.0)
	newBrickSize.pushBackAsFloat64(21.0)
	newBrickSize.pushBackAsFloat64(22.0)
	newNumPoints = XdmfArray.New()
	newNumPoints.pushBackAsUInt32(5)
	newNumPoints.pushBackAsUInt32(6)
	newNumPoints.pushBackAsUInt32(7)
	newGridOrigin = XdmfArray.New()
	newGridOrigin.pushBackAsFloat64(0.0)
	newGridOrigin.pushBackAsFloat64(1.0)
	newGridOrigin.pushBackAsFloat64(2.0)

	exampleGrid = XdmfRegularGrid.New(newBrickSize, newNumPoints, newGridOrigin)

	#//initializationvector end

	#//setBrickSize begin

	exampleGrid.setBrickSize(newBrickSize)

	#//setBrickSize end

	#//setDimensions begin

	exampleGrid.setDimensions(newNumPoints)

	#//setDimensions end

	#//setOrigin begin

	exampleGrid.setOrigin(newGridOrigin)

	#//setOrigin end
