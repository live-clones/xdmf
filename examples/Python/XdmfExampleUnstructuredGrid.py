from Xdmf import *

if __name__ == "__main__":
	exampleGrid = XdmfUnstructuredGrid.New()

	newBrickX = 0.0
	newBrickY = 0.0
	newPointsX = 5
	newPointsY = 5
	newOriginX = 20.0
	newOriginY = 20.0
	baseGrid = XdmfRegularGrid.New(newBrickX, newBrickY, newPointsX, newPointsY, newOriginX, newOriginY)
	regGeneratedGrid = XdmfUnstructuredGrid.New(baseGrid)

	newGeometry = XdmfGeometry.New()
	newGeometry.setType(XdmfGeometryType.XYZ())
	newGeometry.pushBackAsInt32(1)
	newGeometry.pushBackAsInt32(2)
	newGeometry.pushBackAsInt32(3)
	newGeometry.pushBackAsInt32(4)
	newGeometry.pushBackAsInt32(5)
	newGeometry.pushBackAsInt32(6)
	newGeometry.pushBackAsInt32(7)
	newGeometry.pushBackAsInt32(8)
	newGeometry.pushBackAsInt32(9)
	exampleGrid.setGeometry(newGeometry)

	newTopology = XdmfTopology.New()
	newTopology.setType(XdmfTopologyType.Triangle())
	newTopology.pushBackAsInt32(1)
	newTopology.pushBackAsInt32(2)
	newTopology.pushBackAsInt32(3)
	newTopology.pushBackAsInt32(4)
	newTopology.pushBackAsInt32(5)
	newTopology.pushBackAsInt32(6)
	newTopology.pushBackAsInt32(7)
	newTopology.pushBackAsInt32(8)
	newTopology.pushBackAsInt32(9)
	exampleGrid.setTopology(newTopology)

	exampleGeometry = exampleGrid.getGeometry()
	exampleTopology = exampleGrid.getTopology()
