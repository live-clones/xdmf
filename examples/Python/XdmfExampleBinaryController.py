from Xdmf import *

if __name__ == "__main__":
        #//initialization begin

        newPath = "File path to binary file goes here"
        newSeek = 0
        newEndian = XdmfBinaryController.NATIVE
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
        #three dimensins, each with 20 maximum values
        readDataSize.push_back(20)
        readDataSize.push_back(20)
        readDataSize.push_back(20)
        exampleController = XdmfBinaryController.New(
                newPath,
                readType,
                newSeek,
                newEndian,
                readStarts,
                readStrides,
                readCounts,
                readDataSize)

        #//initialization end

        #//initializationsimplified begin

        newPath = "File path to binary file goes here"
        newSeek = 0
        newEndian = XdmfBinaryController.NATIVE
        readType = XdmfArrayType.Int32()
        readCounts = UInt32Vector()
        #Three dimensions, reading 10 values from each
        readCounts.push_back(10)
        readCounts.push_back(10)
        readCounts.push_back(10)
        exampleController = XdmfBinaryController.New(
                newPath,
                readType,
                newSeek,
                newEndian,
                readCounts)

        #//initializationsimplified end

        #//getEndian begin

        readEndian = exampleController.getEndian()

        #//getEndian end

        #//getSeek begin

        readSeek = exampleController.getSeek()

        #//getSeek end
