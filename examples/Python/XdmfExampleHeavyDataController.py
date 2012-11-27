from Xdmf import *

if __name__ == "__main__":
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
        exampleController = XdmfHDF5Controller.New(
                newPath,
                newSetPath,
                readType,
                readStarts,
                readStrides,
                readCounts)

	#Assuming that exampleController is a shared pointer to an XdmfHDF5Controller object
	#Since XdmfHeavyDataController is an abstract class

	examplePath = exampleController.getDataSetPath()

	exampleDimensions = exampleController.getDimensions()

	examplePath = exampleController.getFilePath()

	exampleName = exampleController.getName()

	exampleSize = exampleController.getSize()

	exampleType = exampleController.getType()

	exampleArray = XdmfArray.New()
	exampleController.read(exampleArray)
	#exampleArray now holds the data that exampleController holds.
