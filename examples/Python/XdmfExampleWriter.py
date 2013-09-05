from Xdmf import *

if __name__ == "__main__":
        #//initialization begin

        outFile = "output file name goes here"
        exampleWriter = XdmfWriter.New(outFile)

        #//initialization end

        #//heavyinitialization begin

        outFile = "output file name goes here"
        heavyFile = "heavy file name goes here"
        replaceFile = True
        exampleHeavyWriter = XdmfHDF5Writer.New(heavyFile, replaceFile)
        exampleWriter = XdmfWriter.New(outFile, exampleHeavyWriter)

        #//heavyinitialization end

        #//getFilePath begin

        examplePath = exampleWriter.getFilePath()

        #//getFilePath end

        #//getHeavyDataWriter begin

        exampleHeavyWriter = exampleWriter.getHeavyDataWriter()

        #//getHeavyDataWriter end

        #//setHeavyDataWriter begin

        exampleWriter.setHeavyDataWriter(exampleHeavyWriter)

        #//setHeavyDataWriter end

        #//getLightDataLimit begin

        exampleLimit = exampleWriter.getLightDataLimit()

        #//getLightDataLimit end

        #//getMode begin

        testMode = XdmfWriter.Default
        if exampleWriter.getMode() == testMode:
                #Do whatever is to be done if the mode is default

        #//getMode end

        #//getWriteXPaths begin

        exampleTestPaths = exampleWriter.getWriteXPaths()

        #//getWriteXPaths end

        #//getXPathParse begin

        exampleXPathParse = exampleWriter.getXPathParse()

        #//getXPathParse end

        #//setLightDataLimit begin

        newLimit = 20;
        exampleWriter.setLightDataLimit(newLimit)
        #The writer will now place any data with a number of values over 20 into heavy data

        #//setLightDataLimit end

        #//setMode begin

        exampleWriter.setMode(XdmfWriter.Default)

        #//setMode end

        #//setWriteXPaths begin

        exampleWriter.setWriteXPaths(True)

        #//setWriteXPaths end

        #//setXPathParse begin

        exampleWriter.setXPathParse(True)

        #//setXPathParse end

        #//visitarray begin

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

        #//visitarray end

        #//visititem begin

        #Using XdmfDomain here, but any XdmfItem would work
        exampleDomain = XdmfDomain.New()
        outFile = "output file name goes here"
        exampleWriter = XdmfWriter.New(outFile)
        exampleWriter.visit(exampleDomain, exampleWriter)

        #//visititem end
