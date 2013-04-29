from Xdmf import *

if __name__ == "__main__":
	newPath = "Your file path goes here"
	replaceOrig = True
	exampleWriter = XdmfHDF5Writer.New(newPath, replaceOrig)

	newFileSizeLimit = 10
	exampleWriter.setFileSizeLimit(newFileSizeLimit)

	exampleLimit = exampleWriter.getFileSizeLimit()

        newAllow = True#false is default

        exampleWriter.setAllowSetSplitting(newAllow)

        exampleAllowSplitting = exampleWriter.getAllowSetSplitting()

        newFileIndex = 0#0 resets to no appended index

        exampleWriter.setFileIndex(newFileIndex)

        exampleIndex = exampleWriter.getFileIndex()

        newChunk = 10#creates blocks in sets of 10 slots

        exampleWriter.setChunkSize(newChunk)

        exampleChunk = exampleWriter.getChunkSize()
