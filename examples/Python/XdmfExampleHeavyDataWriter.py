from Xdmf import *

if __name__ == "__main__":
        #//initialization begin

        newPath = "Your file path goes here"
        replaceOrig = True
        exampleWriter = XdmfHDF5Writer.New(newPath, replaceOrig)

        #//initialization end

        #//openFile begin

        exampleWriter.openFile()

        #//openFile end

        #//closeFile begin

        exampleWriter.closeFile()

        #//closeFile end

        #//getFilePath begin

        examplePath = exampleWriter.getFilePath()

        #//getFilePath end

        #//getMode begin

        exampleMode = XdmfHeavyDataWriter.Default
        if exampleWriter.getMode() == exampleMode:
                #Do whatever is to be done if the mode is default

        #//getMode end

        #//getReleaseData begin

        testRelease = exampleWriter.getReleaseData()

        #//getReleaseData end

        #//setFileSizeLimit begin

        newFileSizeLimit = 10

        exampleWriter.setFileSizeLimit(newFileSizeLimit)

        #//setFileSizeLimit end

        #//getFileSizeLimit begin

        exampleLimit = exampleWriter.getFileSizeLimit()

        #//getFileSizeLimit end

        #//getFileOverhead begin

        exampleOverhead = exampleWriter.getFileOverhead()

        #//getFileOverhead end

        #//setAllowSetSplitting begin

        newAllow = True
        #false is default

        exampleWriter.setAllowSetSplitting(newAllow)

        #//setAllowSetSplitting end

        #//getAllowSetSplitting begin

        exampleAllowSplitting = exampleWriter.getAllowSetSplitting()

        #//getAllowSetSplitting end

        #//setFileIndex begin

        newFileIndex = 0
        #0 resets to no appended index

        exampleWriter.setFileIndex(newFileIndex)

        #//setFileIndex end

        #//getFileIndex begin

        exampleIndex = exampleWriter.getFileIndex()

        #//getFileIndex end

        #//setMode begin

        exampleWriter.setMode(XdmfHeavyDataWriter.Default)

        #//setMode end

        #//setReleaseData begin

        exampleWriter.setReleaseData(True)
        #Sets the writer to release data after writing

        #//setReleaseData end

        #//visit begin

        exampleArray = XdmfArray.New()
        exampleArray.pushBackAsInt32(1)
        exampleArray.pushBackAsInt32(2)
        exampleArray.pushBackAsInt32(3)
        exampleArray.pushBackAsInt32(4)
        exampleArray.pushBackAsInt32(5)
        exampleWriter.visit(exampleArray, exampleWriter)

        #//visit end
