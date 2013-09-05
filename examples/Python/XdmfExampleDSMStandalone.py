from Xdmf import *
from mpi4py.MPI import * #the act of importing this automatically calls Init

if __name__ == "__main__":
        #//initMPI begin

        comm = COMM_WORLD

        dsmSize = 64

        id = comm.Get_rank()
        size = comm.Get_size()

        coreSize = int(dsmSize/size)

        #//initMPI end

        exampleArray = XdmfArray.New()
        exampleArray.initializeAsInt32(0)

        #Writing form all cores

        #//writevectorinit begin

        newPath = "dsm"
        newSetPath = "data"

        writeStarts = UInt32Vector()
        writeCounts = UInt32Vector()
        writeStrides = UInt32Vector()
        writeDataSize = UInt32Vector()

        writeStarts.push_back(id*4)
        writeCounts.push_back(4)
        writeStrides.push_back(1)
        writeDataSize.push_back(size*4)

        #//writevectorinit end

        for i in range (1, 5):
                exampleArray.pushBackAsInt32(i*id)


        #writing from one core
        #if id == 0:
        #       for i in range (1, (size*4)+1):
        #               exampleArray.pushBackAsInt32(i)
        #       writeStarts.push_back(0)
        #       writeCounts.push_back(size*4)
        #       writeStrides.push_back(1)
        #       writeDataSize.push_back(size*4)
        #else:
        #       writeStarts.push_back(0)
        #       writeCounts.push_back(0)
        #       writeStrides.push_back(1)
        #       writeDataSize.push_back(size*4)

        #writing with arrays of different sizes
        #startposition = 0
        #for i in range(0, id):
        #       startposition = startposition + i + 1
        #totaldata = 0
        #for i in range(0, size):
        #       totaldata = totaldata + i + 1
        #for i in range(0, id+1):
        #       exampleArray.pushBackAsInt32(id)
        #writeStarts.push_back(startposition)
        #writeCounts.push_back(id+1)
        #writeStrides.push_back(1)
        #writeDataSize.push_back(totaldata)

        #//initwritergenerate begin

        #Python has to do it this way because H5FD isn't wrapped to Python
        #If this is used the manager must be deleted before the program ends.
        exampleWriter = XdmfHDF5WriterDSM.New(newPath, comm, coreSize)

        #//initwritergenerate end

        #//initcontrollerwithbuffer begin

        writeController = XdmfHDF5ControllerDSM.New(
                newPath,
                newSetPath,
                XdmfArrayType.Int32(),
                writeStarts,
                writeStrides,
                writeCounts,
                writeDataSize,
                exampleWriter.getBuffer())

        #//initcontrollerwithbuffer end

        #//setManagercontroller begin

        writeController.setManager(exampleWriter.getManager())

        #//setManagercontroller end

        #//setBuffercontroller begin

        writeController.setBuffer(exampleWriter.getBuffer())

        #//setBuffercontroller end

        exampleWriter.deleteManager()

        #//initcontrollergenerate begin

        #Python has to do it this way because H5FD isn't wrapped to Python
        #If this is used the manager must be deleted before the program ends.
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

        #//initcontrollergenerate end

        #//initwriterwithbuffer begin

        exampleWriter = XdmfHDF5WriterDSM.New(newPath, writeController.getBuffer())

        #//initwriterwithbuffer end

        #//setManagerwriter begin

        exampleWriter.setManager(writeController.getManager())

        #//setManagerwriter end

        #//setBufferwriter begin

        exampleWriter.setBuffer(writeController.getBuffer())

        #//setBufferwriter end

        exampleWriter.setMode(XdmfHeavyDataWriter.Hyperslab);

        exampleArray.insert(writeController);

        for i in range (0, size):
                comm.Barrier()
                if i == id:
                        print "Core # " + str(id)
                        print "Controller Stats"
                        print "datasetpath = " + exampleArray.getHeavyDataController(0).getDataSetPath()
                        print "filepath = " + exampleArray.getHeavyDataController(0).getFilePath()
                        print "Dataspace Dimensions"
                        outputVector = exampleArray.getHeavyDataController(0).getDataspaceDimensions()
                        j=0
                        for item in outputVector:
                                print "[" + str(j) + "] = " + str(item)
                                j = j + 1
                        print "Controller Dimensions"
                        outputVector = exampleArray.getHeavyDataController(0).getDimensions()
                        j=0
                        for item in outputVector:
                                print "[" + str(j) + "] = " + str(item)
                                j = j + 1
                        print "Controller Size = " + str(exampleArray.getHeavyDataController(0).getSize())
                        print "Controller Starts"
                        outputVector = exampleArray.getHeavyDataController(0).getStart()
                        j=0
                        for item in outputVector:
                                print "[" + str(j) + "] = " + str(item)
                                j = j + 1
                        print "Controller strides"
                        outputVector = exampleArray.getHeavyDataController(0).getStride()
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
        #       readStarts.push_back(0)
        #       readCounts.push_back(4*size)
        #       readStrides.push_back(1)
        #       readDataSize.push_back(size*4)
        #else:
        #       readStarts.push_back(0)
        #       readCounts.push_back(0)
        #       readStrides.push_back(1)
        #       readDataSize.push_back(size*4)

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

        readArray.insert(readController)

        print "testing read"
        readArray.read()
        print "done testing read"

        for i in range (0, size):
                comm.Barrier()
                if i == id:
                        print "Core # " + str(id)
                        print "Controller Stats"
                        print "datasetpath = " + readArray.getHeavyDataController(0).getDataSetPath()
                        print "filepath = " + readArray.getHeavyDataController(0).getFilePath()
                        print "Dataspace Dimensions"
                        outputVector = readArray.getHeavyDataController(0).getDataspaceDimensions()
                        j=0
                        for item in outputVector:
                                print "[" + str(j) + "] = " + str(item)
                                j = j + 1
                        print "Controller Dimensions"
                        outputVector = readArray.getHeavyDataController(0).getDimensions()
                        j=0
                        for item in outputVector:
                                print "[" + str(j) + "] = " + str(item)
                                j = j + 1
                        print "Controller Size = " + str(readArray.getHeavyDataController(0).getSize())
                        print "Controller Starts"
                        outputVector = readArray.getHeavyDataController(0).getStart()
                        j=0
                        for item in outputVector:
                                print "[" + str(j) + "] = " + str(item)
                                j = j + 1
                        print "Controller strides"
                        outputVector = readArray.getHeavyDataController(0).getStride()
                        j=0
                        for item in outputVector:
                                print "[" + str(j) + "] = " + str(item)
                                j = j + 1
                        for j in range (0, readArray.getSize()):
                                print "Core #" + str(id) +" exampleArray[" + str(j) + "] = " + str(readArray.getValueAsInt32(j))

        #//deleteManagerwriter begin

        exampleWriter.deleteManager()

        #//deleteManagerwriter end

        '''
        #//deleteManagercontroller

        writeController.deleteManager()

        #//deleteManagercontroller
        '''

        #finalize is automatically called on program exit
