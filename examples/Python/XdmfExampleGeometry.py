from Xdmf import *

if __name__ == "__main__":
	exampleGeometry = XdmfGeometry.New()

	exampleGeometry.setType(XdmfGeometryType.XYZ())

	exampleType = exampleGeometry.getType()

	numPoints = exampleGeometry.getNumberPoints()
