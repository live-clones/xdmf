from Xdmf import *

if __name__ == "__main__":
	#Using XdmfArrayType.Int32() as an example
	propertyMap = StringMap()
	XdmfArrayType.Int32().getProperties(propertyMap)
