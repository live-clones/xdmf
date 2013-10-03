from XdmfCore import *

if __name__ == "__main__":

	writtenArray = XdmfArray.New()
	dimensionVector = UInt32Vector()
	dimensionVector.push_back(20)
	writtenArray.initializeAsInt32(dimensionVector)
	for i in range(20):
		writtenArray.pushBackAsInt32(i + 1)

	readArray = XdmfArray.New()
	readDimensionVector = UInt32Vector()
	readDimensionVector.push_back(6)
	readDimensionVector.push_back(4)
	readArray.initializeAsInt32(readDimensionVector)

	writeStarts = UInt32Vector()
	writeStarts.push_back(0)
	writeStarts.push_back(0)
	writeStrides = UInt32Vector()
	writeStrides.push_back(2)
	writeStrides.push_back(2)
	writeDim = UInt32Vector()
	writeDim.push_back(3)
	writeDim.push_back(2)
	readStarts = UInt32Vector()
	readStarts.push_back(0)
	readStarts.push_back(0)
	readStrides = UInt32Vector()
	readStrides.push_back(2)
	readStrides.push_back(2)
	readDim = UInt32Vector()
	readDim.push_back(3)
	readDim.push_back(2)

	try:
		readArray.insert(readStarts, writtenArray, writeStarts, writeDim, readDim, readStrides, writeStrides)
	except RuntimeError as e:
		#this is simpler than it is in C++
		print e.args[0]


	try:
		raise RuntimeError(XdmfError(XdmfError.FATAL, "testError"))
        except RuntimeError as e:
		print e.args[0].what()
