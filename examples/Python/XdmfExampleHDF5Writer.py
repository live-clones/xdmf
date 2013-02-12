from Xdmf import *

if __name__ == "__main__":
	newPath = "Your file path goes here"
	replaceOrig = True
	exampleWriter = XdmfHDF5Writer.New(newPath, replaceOrig)

	newFileSizeLimit = 10
	exampleWriter.setFileSizeLimit(newFileSizeLimit)

	exampleLimit = exampleWriter.getFileSizeLimit()
