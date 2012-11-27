from Xdmf import *

if __name__ == "__main__":
	newBrickX = 20.0
	newPointsX = 5
	newOriginX = 0.0
	newBrickY = 20.0
	newPointsY = 5
	newOriginY = 0.0
	newBrickZ = 20.0
	newPointsZ = 5
	newOriginZ = 0.0

	exampleGrid = XdmfRegularGrid.New(
		newBrickX, newBrickY, newPointsX, newPointsY, newOriginX, newOriginY)

	exampleGrid = XdmfRegularGrid.New(
		newBrickX, newBrickY, newBrickZ, newPointsX, newPointsY, newPointsZ, newOriginX, newOriginY, newOriginZ)


	exampleBrick = exampleGrid.getBrickSize()
	exampleDimensions = exampleGrid.getDimensions()
	exampleOrigin = exampleGrid.getOrigin()


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

	exampleGrid.setBrickSize(newBrickSize)
	exampleGrid.setDimensions(newNumPoints)
	exampleGrid.setOrigin(newGridOrigin)
