from Xdmf import *

if __name__ == "__main__":
	newPath = "Your file path goes here"
	replaceOrig = True
	exampleWriter = XdmfHDF5Writer.New(newPath, replaceOrig)

	#Assume that exampleWriter is a shared poinnter to a XdmfHDF5Writer.

	exampleWriter.openFile()

	exampleWriter.closeFile()

	examplePath = exampleWriter.getFilePath()

	exampleMode = XdmfHeavyDataWriter.Default
	if exampleWriter.getMode() == exampleMode:
		#Do whatever is to be done if the mode is default

	testRelease = exampleWriter.getReleaseData()

	exampleWriter.openFile()

	exampleWriter.setMode(XdmfHeavyDataWriter.Default)

	exampleWriter.setReleaseData(True)
	#Sets the writer to release data after writing

	exampleArray = XdmfArray.New()
	exampleArray.pushBackAsInt32(1)
	exampleArray.pushBackAsInt32(2)
	exampleArray.pushBackAsInt32(3)
	exampleArray.pushBackAsInt32(4)
	exampleArray.pushBackAsInt32(5)
	exampleWriter.visit(exampleArray, exampleWriter)
