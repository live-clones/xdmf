from Xdmf import *

if __name__ == "__main__":
	exampleGeometry = XdmfGeometry.New()

	#Assuming that exampleGeometry is a shared pointer to a XdmfGeometry object

	exampleGeometry.setType(XdmfGeometryType.XYZ())

	exampleType = exampleGeometry.getType()

	numPoints = exampleGeometry.getNumberPoints()
