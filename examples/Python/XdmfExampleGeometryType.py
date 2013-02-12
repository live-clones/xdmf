from Xdmf import *

if __name__ == "__main__":
	exampleDimensions = XdmfGeometryType.XYZ().getDimensions()
	exampleName = XdmfGeometryType.XYZ().getName()

	exampleGeometry = XdmfGeometry.New()

	exampleType = exampleGeometry.getType()

	if exampleType == XdmfGeometryType.XYZ():
		#do whatever is to be done if the geometry is XYZ
