from Xdmf import *

if __name__ == "__main__":
	outFile = "output file name goes here"
	exampleWriter = XdmfWriter.New(outFile)

	heavyFile = "heavy file name goes here"
	replaceFile = True
	exampleHeavyWriter = XdmfHDF5Writer.New(heavyFile, replaceFile)
	exampleWriter = XdmfWriter.New(outFile, exampleHeavyWriter)

	#Assume that exampleWriter is a shared pointer to a XdmfWriter object

	examplePath = exampleWriter.getFilePath()

	exampleHeavyWriter = exampleWriter.getHeavyDataWriter()

	exampleWriter.setHeavyDataWriter(exampleHeavyWriter)

	exampleLimit = exampleWriter.getLightDataLimit()

	testMode = XdmfWriter.Default
	#Assume that exampleWriter is a shared pointer to a XdmfWriter object
	if exampleWriter.getMode() == testMode:
		#Do whatever is to be done if the mode is default

	exampleTestPaths = exampleWriter.getWriteXPaths()

	exampleXPathParse = exampleWriter.getXPathParse()

	newLimit = 20;
	#Assume that exampleWriter is a shared pointer to a XdmfWriter object
	exampleWriter.setLightDataLimit(newLimit)
	#The writer will now place any data with a number of values over 20 into heavy data

	exampleWriter.setMode(XdmfWriter.Default)

	exampleWriter.setWriteXPaths(True)

	exampleWriter.setXPathParse(True)

	#Using XdmfAttribute here, but any XdmfArray would work
	exampleAttribute = XdmfAttribute.New()
	exampleAttribute.setCenter(XdmfAttributeCenter.Node())
	exampleAttribute.setType(XdmfAttributeType.Scalar())
	exampleAttribute.pushBackAsInt32(1)
	exampleAttribute.pushBackAsInt32(2)
	exampleAttribute.pushBackAsInt32(3)
	exampleAttribute.pushBackAsInt32(4)
	exampleAttribute.pushBackAsInt32(5)
	exampleAttribute.pushBackAsInt32(6)
	outFile = "output file name goes here"
	exampleWriter = XdmfWriter.New(outFile)
	exampleWriter.visit(exampleAttribute, exampleWriter)

	#Using XdmfDomain here, but any XdmfItem would work
	exampleDomain = XdmfDomain.New()
	outFile = "output file name goes here"
	exampleWriter = XdmfWriter.New(outFile)
	exampleWriter.visit(exampleDomain, exampleWriter)
