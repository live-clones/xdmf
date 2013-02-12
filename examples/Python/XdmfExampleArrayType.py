from Xdmf import *

if __name__ == "__main__":
	dataSize = XdmfArrayType.Int8().getElementSize()

	dataName = XdmfArrayType.Int8().getName()

	exampleArray = XdmfArray.New()

	exampleType = exampleArray.getType()

	if exampleType == XdmfArrayType.Int8():
		#do whatever is to be done if the type is Int8
