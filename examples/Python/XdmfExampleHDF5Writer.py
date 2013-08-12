from Xdmf import *

if __name__ == "__main__":
	#//initialization begin

	newPath = "Your file path goes here"
	replaceOrig = True
	exampleWriter = XdmfHDF5Writer.New(newPath, replaceOrig)

	#//initialization end

	#//setChunkSize begin

        newChunk = 10
	#creates blocks in sets of 10 slots

        exampleWriter.setChunkSize(newChunk)

	#//setChunkSize end

	#//getChunkSize begin

        exampleChunk = exampleWriter.getChunkSize()

	#//getChunkSize end
