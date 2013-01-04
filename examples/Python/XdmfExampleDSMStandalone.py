from Xdmf import *
from mpi4py.MPI import * #the act of importing this automatically calls Init

if __name__ == "__main__":
	comm = COMM_WORLD

	dsmSize = 64

	id = comm.Get_rank()
	size = comm.Get_size()

	exampleArray = XdmfArray.New()
	exampleArray.initializeAsInt32(0)
	newPath = "dsm"
	newSetPath = "data"

	writeStarts = UInt32Vector()
	writeCounts = UInt32Vector()
	writeStrides = UInt32Vector()
	writeDataSize = UInt32Vector()

	#Writing form all cores
	for i in range (1, 5):
		exampleArray.pushBackAsInt32(i*id)
	writeStarts.push_back(id*4)
	writeCounts.push_back(4)
	writeStrides.push_back(1)
	writeDataSize.push_back(size*4)

	#writing from one core
	#if id == 0:
	#	for i in range (1, (size*4)+1):
	#		exampleArray.pushBackAsInt32(i)
	#	writeStarts.push_back(0)
	#	writeCounts.push_back(size*4)
	#	writeStrides.push_back(1)
	#	writeDataSize.push_back(size*4)
	#else:
	#	writeStarts.push_back(0)
	#	writeCounts.push_back(0)
	#	writeStrides.push_back(1)
	#	writeDataSize.push_back(size*4)

	#writing with arrays of different sizes
	#startposition = 0
	#for i in range(0, id):
	#	startposition = startposition + i + 1
	#totaldata = 0
	#for i in range(0, size):
	#	totaldata = totaldata + i + 1
	#for i in range(0, id+1):
	#	exampleArray.pushBackAsInt32(id)
	#writeStarts.push_back(startposition)
	#writeCounts.push_back(id+1)
	#writeStrides.push_back(1)
	#writeDataSize.push_back(totaldata)


	coreSize = int(dsmSize/size)

	#Python has to do it this way because H5FD isn't wrapped to Python
	#If this is used the manager must be deleted before the program ends.
	exampleWriter = XdmfHDF5WriterDSM.New(newPath, comm, coreSize)

	writeController = XdmfHDF5ControllerDSM.New(
		newPath,
		newSetPath,
		XdmfArrayType.Int32(),
		writeStarts,
		writeStrides,
		writeCounts,
		writeDataSize,
		exampleWriter.getBuffer())

	writeController.setManager(exampleWriter.getManager())
	writeController.setBuffer(exampleWriter.getBuffer())

	exampleWriter.deleteManager()

	writeController = XdmfHDF5ControllerDSM.New(
                newPath,
                newSetPath,
                XdmfArrayType.Int32(),
                writeStarts,
                writeStrides,
                writeCounts,
                writeDataSize,
                comm,
		coreSize)

	exampleWriter = XdmfHDF5WriterDSM.New(newPath, writeController.getBuffer())
	exampleWriter.setManager(writeController.getManager())
	exampleWriter.setBuffer(writeController.getBuffer())

	exampleWriter.setMode(XdmfHeavyDataWriter.Hyperslab);

	exampleArray.setHeavyDataController(writeController);

	for i in range (0, size):
		comm.Barrier()
		if i == id:
			print "Core # " + str(id)
			print "Controller Stats"
			print "datasetpath = " + exampleArray.getHeavyDataController().getDataSetPath()
			print "filepath = " + exampleArray.getHeavyDataController().getFilePath()
			print "Dataspace Dimensions"
			outputVector = exampleArray.getHeavyDataController().getDataspaceDimensions()
			j=0
			for item in outputVector:
				print "[" + str(j) + "] = " + str(item)
				j = j + 1
			print "Controller Dimensions"
			outputVector = exampleArray.getHeavyDataController().getDimensions()
			j=0
			for item in outputVector:
				print "[" + str(j) + "] = " + str(item)
				j = j + 1
			print "Controller Size = " + str(exampleArray.getHeavyDataController().getSize())
			print "Controller Starts"
			outputVector = exampleArray.getHeavyDataController().getStart()
			j=0
			for item in outputVector:
				print "[" + str(j) + "] = " + str(item)
				j = j + 1
			print "Controller strides"
			outputVector = exampleArray.getHeavyDataController().getStride()
			j=0
			for item in outputVector:
				print "[" + str(j) + "] = " + str(item)
				j = j + 1
			for j in range (0, exampleArray.getSize()):
				print "Core #" + str(id) +" exampleArray[" + str(j) + "] = " + str(exampleArray.getValueAsInt32(j))

	exampleArray.accept(exampleWriter)

	readStarts = UInt32Vector()
	readCounts = UInt32Vector()
	readStrides = UInt32Vector()
	readDataSize = UInt32Vector()

	#read equal size arrays for all cores
	readStarts.push_back((size - id - 1) * 4)
	readCounts.push_back(4)
	readStrides.push_back(1)
	readDataSize.push_back(size*4)

	#read to one core
	#if (id == 0):
	#	readStarts.push_back(0)
	#	readCounts.push_back(4*size)
	#	readStrides.push_back(1)
	#	readDataSize.push_back(size*4)
	#else:
	#	readStarts.push_back(0)
	#	readCounts.push_back(0)
	#	readStrides.push_back(1)
	#	readDataSize.push_back(size*4)

	#for arrays of different sizes
	#readStarts.push_back(totaldata - startposition - 1 - id)
	#readCounts.push_back(id+1)
	#readStrides.push_back(1)
	#readDataSize.push_back(totaldata)

	readArray = XdmfArray.New()

	readArray.initializeAsInt32(0)
	readArray.reserve(exampleArray.getSize())

	readController = XdmfHDF5ControllerDSM.New(
		newPath,
		newSetPath,
		XdmfArrayType.Int32(),
		readStarts,
		readStrides,
		readCounts,
		readDataSize,
		exampleWriter.getBuffer())

	readArray.setHeavyDataController(readController)

        print "testing read"
        readArray.read()
	print "done testing read"

	for i in range (0, size):
		comm.Barrier()
		if i == id:
			print "Core # " + str(id)
			print "Controller Stats"
			print "datasetpath = " + readArray.getHeavyDataController().getDataSetPath()
			print "filepath = " + readArray.getHeavyDataController().getFilePath()
			print "Dataspace Dimensions"
			outputVector = readArray.getHeavyDataController().getDataspaceDimensions()
			j=0
			for item in outputVector:
				print "[" + str(j) + "] = " + str(item)
				j = j + 1
			print "Controller Dimensions"
			outputVector = readArray.getHeavyDataController().getDimensions()
			j=0
			for item in outputVector:
				print "[" + str(j) + "] = " + str(item)
				j = j + 1
			print "Controller Size = " + str(readArray.getHeavyDataController().getSize())
			print "Controller Starts"
			outputVector = readArray.getHeavyDataController().getStart()
			j=0
			for item in outputVector:
				print "[" + str(j) + "] = " + str(item)
				j = j + 1
			print "Controller strides"
			outputVector = readArray.getHeavyDataController().getStride()
			j=0
			for item in outputVector:
				print "[" + str(j) + "] = " + str(item)
				j = j + 1
			for j in range (0, readArray.getSize()):
				print "Core #" + str(id) +" exampleArray[" + str(j) + "] = " + str(readArray.getValueAsInt32(j))

	#if the New method using the Comm and data size is used, the manager has to be deleted or there will be a segfault
	exampleWriter.deleteManager()
	'''
	writeController.deleteManager()
	'''

	#finalize is automatically called on program exit
