from Xdmf import *

if __name__ == "__main__":

	writer = XdmfHDF5Writer.New("hdf5WriterPythonTest.h5")

	grid = XdmfUnstructuredGrid.New()
	grid.setName("test")
	points = [0.1, 0.1, 1.1, 1.1, 0.1, 1.1, 3.1, 0.1, 2.1, 0.1, 1.1, 1.1, 1.1, 1.1, 1.1, 3.1, 2.1, 2.1,
			0.1, 0.1, -1.1, 1.1, 0.1, -1.1, 3.1, 0.1, -2.1, 0.1, 1.1, -1.1, 1.1, 1.1, -1.1, 3.1, 2.1, -2.1]

	grid.getGeometry().setType(XdmfGeometryType.XYZ())
	grid.getGeometry().insertAsFloat64(0, points)

	connectivity = [0, 1, 7, 6, 3, 4, 10, 9, 1, 2, 8, 7, 4, 5, 11, 10]
	grid.getTopology().setType(XdmfTopologyType.Hexahedron())
	grid.getTopology().insertAsInt32(0, connectivity)

	nodalAttribute = XdmfAttribute.New()
	nodalAttribute.setName("Nodal Attribute")
	nodalAttribute.setType(XdmfAttributeType.Scalar())
	nodalAttribute.setCenter(XdmfAttributeCenter.Node())
	nodeVals = [100, 200, 300, 300, 400, 500, 300, 400, 500, 500, 600, 700]
	nodalAttribute.insertAsFloat64(0, nodeVals)

	cellAttribute = XdmfAttribute.New()
	cellAttribute.setName("Cell Attribute")
	cellAttribute.setType(XdmfAttributeType.Scalar())
	cellAttribute.setCenter(XdmfAttributeCenter.Cell())
	cellVals = [100, 200]
	cellAttribute.insertAsFloat64(0, cellVals)
	
	grid.insert(nodalAttribute)
	grid.insert(cellAttribute)

	domain = XdmfDomain.New()
	domain.insert(grid)
	domain.accept(writer)
