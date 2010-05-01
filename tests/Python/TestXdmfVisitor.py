from Xdmf import *

if __name__ == "__main__":

	visitor = XdmfVisitor.New()
	visitor.setLightDataLimit(10)

	grid = XdmfGrid.New()
	grid.setName("test")
	points = [0.1, 0.1, 1.1, 1.1, 0.1, 1.1, 3.1, 0.1, 2.1, 0.1, 1.1, 1.1, 1.1, 1.1, 1.1, 3.1, 2.1, 2.1,
			0.1, 0.1, -1.1, 1.1, 0.1, -1.1, 3.1, 0.1, -2.1, 0.1, 1.1, -1.1, 1.1, 1.1, -1.1, 3.1, 2.1, -2.1]

	grid.getGeometry().setGeometryType(XdmfGeometryType.XYZ())
	grid.getGeometry().getArray().copyValuesAsDouble(0, points)

	connectivity = [0, 1, 7, 6, 3, 4, 10, 9, 1, 2, 8, 7, 4, 5, 11, 10]
	grid.getTopology().setTopologyType(XdmfTopologyType.Hexahedron())
	grid.getTopology().getArray().copyValuesAsInt(0, connectivity)

	nodalAttribute = XdmfAttribute.New()
	nodalAttribute.setName("Nodal Attribute")
	nodalAttribute.setAttributeType(XdmfAttributeType.Scalar())
	nodalAttribute.setAttributeCenter(XdmfAttributeCenter.Node())
	nodeVals = [100, 200, 300, 300, 400, 500, 300, 400, 500, 500, 600, 700]
	nodalAttribute.getArray().copyValuesAsDouble(0, nodeVals)

	cellAttribute = XdmfAttribute.New()
	cellAttribute.setName("Cell Attribute")
	cellAttribute.setAttributeType(XdmfAttributeType.Scalar())
	cellAttribute.setAttributeCenter(XdmfAttributeCenter.Cell())
	cellVals = [100, 200]
	cellAttribute.getArray().copyValuesAsDouble(0, cellVals)
	
	grid.insert(nodalAttribute)
	grid.insert(cellAttribute)

	domain = XdmfDomain.New()
	domain.insert(grid)
	domain.accept(visitor)
