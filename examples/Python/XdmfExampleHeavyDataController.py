from Xdmf import *

if __name__ == "__main__":
        #//initialization begin

        newPath = "File path to hdf5 file goes here"
        newSetPath = "path to the set goes here"
        readType = XdmfArrayType.Int32()
        readStarts = UInt32Vector()
        #Three dimensions, all starting at index 0
        readStarts.push_back(0)
        readStarts.push_back(0)
        readStarts.push_back(0)
        readStrides = UInt32Vector()
        #Three dimensions, no skipping between reads
        readStrides.push_back(1)
        readStrides.push_back(1)
        readStrides.push_back(1)
        readCounts = UInt32Vector()
        #Three dimensions, reading 10 values from each
        readCounts.push_back(10)
        readCounts.push_back(10)
        readCounts.push_back(10)
        readDataSize = UInt32Vector()
        #Three dimensions, 10 values in each
        readDataSize.push_back(10)
        readDataSize.push_back(10)
        readDataSize.push_back(10)
        exampleController = XdmfHDF5Controller.New(
                newPath,
                newSetPath,
                readType,
                readStarts,
                readStrides,
                readCounts,
                readDataSize)

        #Using an XdmfHDF5Controller since XdmfHeavyDataController is an abstract class

        #//initialization end

        #//getDataSetPath begin

        examplePath = exampleController.getDataSetPath()

        #//getDataSetPath end

        #//getDimensions begin

        exampleDimensions = exampleController.getDimensions()

        #//getDimensions end

        #//getDataspaceDimensions begin

        exampleDataspaceDimensions = exampleController.getDataspaceDimensions()

        #//getDataspaceDimensions end

        #//getFilePath begin

        examplePath = exampleController.getFilePath()

        #//getFilePath end

        #//getName begin

        exampleName = exampleController.getName()

        #//getName end

        #//getSize begin

        exampleSize = exampleController.getSize()

        #//getSize end

        #//getType begin

        exampleType = exampleController.getType()

        #//getType end

        #//read begin

        exampleArray = XdmfArray.New()
        exampleController.read(exampleArray)
        #exampleArray now holds the data that exampleController holds.

        #//read end

        #//getStart begin

        exampleStart = exampleController.getStart()

        #//getStart end

        #//getStride begin

        exampleStride = exampleController.getStride()

        #//getStride end

        #//setArrayOffset begin

        newArrayOffset = 5#default is 0

        exampleController.setArrayOffset(newArrayOffset)

        #//setArrayOffset end

        #//getArrayOffset begin

        exampleOffset = exampleController.getArrayOffset()

        #//getArrayOffset end
