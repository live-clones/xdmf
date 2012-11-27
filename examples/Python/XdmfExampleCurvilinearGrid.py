from Xdmf import *

if __name__ == "__main__":
	newPointsX = 5
	newPointsY = 5
	newPointsZ = 5
	exampleGrid = XdmfCurvilinearGrid.New(newPointsX, newPointsY)
	exampleGrid = XdmfCurvilinearGrid.New(newPointsX, newPointsY, newPointsZ)
	newPoints = XdmfArray.New()
	newPoints.pushBackAsInt32(5)
	newPoints.pushBackAsInt32(5)
	newPoints.pushBackAsInt32(5)
	exampleGrid = XdmfCurvilinearGrid.New(newPoints)

	#setGeometry
	newPoints = XdmfArray.New()
	newPoints.pushBackAsInt32(5)
	newPoints.pushBackAsInt32(5)
	newPoints.pushBackAsInt32(5)
	newGeometry = XdmfGeometry.New()
	newGeometry.setType(XdmfGeometryType.XYZ())
	newGeometry.insert(0, newPoints, 0, 3, 1, 1)#Start index is 0, 3 values are passed, stride for both arrays is 1
	exampleGrid.setGeometry(newGeometry)

	#setDimensions
	newPoints = XdmfArray.New()
	newPoints.pushBackAsInt32(5)
	newPoints.pushBackAsInt32(5)
	newPoints.pushBackAsInt32(5)
	exampleGrid.setDimensions(newPoints)

	exampleGeometry = exampleGrid.getGeometry()
	exampleDimensions = exampleGrid.getDimensions()
