from Xdmf import *
from mpi4py.MPI import *

if __name__ == "__main__":
	#The total size of the DSM being created
	dsmSize = 64;
	comm = COMM_WORLD;

	id = comm.Get_rank()
	size = comm.Get_size()

	testArray = XdmfArray.New()
	testArray.initializeAsInt32(0)
	newPath = "dsm"
	newSetPath = "data"

	numServersCores = 4

	for i in range(1,5):
		testArray.pushBackAsInt32(i*(id+1))


	writeStartVector = UInt32Vector()
	writeStartVector.push_back(id*4)
	#writeStartVector.push_back(id);
	writeStrideVector = UInt32Vector()
	writeStrideVector.push_back(1)
	#writeStrideVector.push_back(size-3);
	writeCountVector = UInt32Vector()
	writeCountVector.push_back(4)
	writeDataSizeVector = UInt32Vector()
	writeDataSizeVector.push_back(4*(size-numServersCores))


	ServerIds = []

	for i in range(size-numServersCores, size):
		ServerIds.append(i)

	workers = comm.Get_group().Excl(ServerIds)

	workerComm = comm.Create(workers)

	exampleWriter = XdmfHDF5WriterDSM.New(newPath, comm, dsmSize/numServersCores, size-numServersCores, size-1)

	'''
	exampleController = XdmfHDF5ControllerDSM.New(
		newPath,
		newSetPath,
		XdmfArrayType.Int32(),
		writeStartVector,
		writeStrideVector,
		writeCountVector,
		writeDataSizeVector,
		comm,
		dsmSize/numServersCores,
		size-numServersCores,
		size-1)
	'''

	

        # Split out sub-comm for the worker cores
        # Server cores will not progress to this point until after the servers are done running


        if (id < size - numServersCores):

		# This section is to demonstrate the functionality of the XdmfDSM classes

		exampleWriter.setServerMode(True)
		exampleServerMode = exampleWriter.getServerMode()

		exampleWorkerComm = exampleWriter.getWorkerComm()
		exampleWriter.setWorkerComm(exampleWorkerComm)

		'''
		exampleWorkerComm = exampleController.getWorkerComm()
		exampleController.setWorkerComm(exampleWorkerComm)
		'''

		writeController = XdmfHDF5ControllerDSM.New(
			newPath,
			newSetPath,
			XdmfArrayType.Int32(),
			writeStartVector,
			writeStrideVector,
			writeCountVector,
			writeDataSizeVector,
			exampleWriter.getServerBuffer());

		exampleWriter2 = XdmfHDF5WriterDSM.New(newPath, exampleWriter.getServerBuffer());

		writeController.setServerMode(True)
		exampleControllerServerMode = writeController.getServerMode()

		exampleManager = exampleWriter.getServerManager()
		exampleWriter.setManager(exampleManager)
		
		'''
		exampleManager = exampleController.getServerManager()
		exampleController.setManager(exampleManager)
		'''

		intraId = exampleManager.GetUpdatePiece()
		intraSize = exampleManager.GetUpdateNumPieces()

		exampleManagerComm = exampleManager.GetMpiComm()
		exampleManager.SetMpiComm(exampleManagerComm)

		exampleBufferSize = exampleManager.GetLocalBufferSizeMBytes()
		exampleManager.SetLocalBufferSizeMBytes(exampleBufferSize)

		exampleIsServer = exampleManager.GetIsServer()
		exampleManager.SetIsServer(exampleIsServer)

		exampleType = exampleManager.GetDsmType()
		exampleManager.SetDsmType(XDMF_DSM_TYPE_UNIFORM)

		exampleBlockLength = exampleManager.GetBlockLength()
		exampleManager.SetBlockLength(exampleBlockLength)

		exampleCommType = exampleManager.GetInterCommType()
		exampleManager.SetInterCommType(XDMF_DSM_COMM_MPI)

		exampleManagerConnectionStatus = exampleManager.GetIsConnected()

		'''
		exampleManager.Destroy()
		exampleManager.Create(size - numServerCores, size - 1)
		'''

		exampleBuffer = exampleWriter.getServerBuffer()
		exampleWriter.setBuffer(exampleBuffer)

		'''
		exampleBuffer = exampleController.getServerBuffer()
		exampleController.setBuffer(exampleBuffer)
		'''

                exampleManager.SetDsmBuffer(exampleBuffer)
                exampleBuffer = exampleManager.GetDsmBuffer()

                exampleIsConnected = exampleBuffer.GetIsConnected()
                exampleBuffer.SetIsConnected(exampleIsConnected)

                exampleDataPointer = exampleBuffer.GetDataPointer()

                exampleDSMType = exampleBuffer.GetDsmType()
                exampleBuffer.SetDsmType(XDMF_DSM_TYPE_UNIFORM)

                exampleBufferIsServer = exampleBuffer.GetIsServer()
                exampleBuffer.SetIsServer(exampleIsServer)

                exampleBufferStart = exampleBuffer.GetStartAddress()
                exampleBufferEnd = exampleBuffer.GetEndAddress()

                exampleServerStart = exampleBuffer.GetStartServerId()
                exampleServerEnd = exampleBuffer.GetEndServerId()

                exampleBufferLength = exampleBuffer.GetLength()
                exampleTotalBufferLength = exampleBuffer.GetTotalLength()

                exampleBufferBlockLength = exampleBuffer.GetBlockLength()
                exampleBuffer.SetBlockLength(exampleBufferBlockLength)

		'''
                exampleBuffer.ConfigureUniform(exampleBuffer.GetComm(), dsmSize/numServersCores, size - numServersCores, size - 1)
		'''
		sendingCore = -1

                if (id == sendingCore):
                        exampleBuffer.SendCommandHeader(XDMF_DSM_OPCODE_DONE, 1, 0, 0, XDMF_DSM_INTER_COMM)

                if (id == sendingCore):
                        sentData = "datastring"
                        exampleBuffer.SendData(1, sentData, 0, XDMF_DSM_PUT_DATA_TAG, 0, XDMF_DSM_INTER_COMM)

                if (id == sendingCore):
                        sentData = 1
                        exampleBuffer.SendAcknowledgment(1, sentData, XDMF_DSM_PUT_DATA_TAG, XDMF_DSM_INTER_COMM)

		ServerIds = []

		for i in range(0, (size - numServersCores) / 2):
			ServerIds.append(i)

		readingCores = workerComm.Get_group().Excl(ServerIds)

		readComm = workerComm.Create(readingCores)

		writingCores = workerComm.Get_group().Incl(ServerIds)

		writeComm = workerComm.Create(writingCores)

		exampleBuffer.GetComm().DupComm(workerComm)

		if (id == 0):
			exampleBuffer.BufferServiceLoop()
			serviceOut = exampleBuffer.BufferService()

                if (id == 1):
			exampleBuffer.SendCommandHeader(XDMF_DSM_OPCODE_DONE, 0, 0, 0, XDMF_DSM_INTER_COMM)
			exampleBuffer.SendCommandHeader(XDMF_DSM_OPCODE_DONE, 0, 0, 0, XDMF_DSM_INTER_COMM)


                correspondingId = exampleBuffer.AddressToId(500)

                exampleDSMComm = exampleBuffer.GetComm()
                exampleBuffer.SetComm(exampleDSMComm)

                exampleIntraID = exampleDSMComm.GetId()
                exampleIntraSize = exampleDSMComm.GetIntraSize()
                exampleInterSize = exampleDSMComm.GetInterSize()

                exampleInterCommType = exampleDSMComm.GetInterCommType()

                exampleDSMComm.Init()


                exampleIntraComm = exampleDSMComm.GetIntraComm()
		exampleDSMComm.DupComm(exampleIntraComm.Dup())

		print type(exampleDSMComm.GetDsmPortName())
		testName = "test"
		exampleDSMComm.SetDsmPortName(testName)
		print exampleDSMComm.GetDsmPortName()

		'''

                connectingGroup = True
                if (id < 5):
                        connectingGroup = True;
                else:
                        connectingGroup = False;

		portString = ""

                if (!connectingGroup):
                        exampleDSMComm.OpenPort()
                        portString = exampleDSMComm.GetDsmPortName()
                        // Send the port string to the connecting group
                        exampleDSMComm.Accept()
                        // When done with connection
                        exampleDSMComm.ClosePort()

                if (connectingGroup):
                        // Recieve string from Master group
                        exampleDSMComm.SetDsmPortName(portString)
                        exampleDSMComm.Connect()
                        // When done with connection
                        exampleDSMComm.Disconnect()

                if (connectingGroup):
                        // Recieve string from Master group
                        exampleDSMComm.SetDsmPortName(portString);
                        exampleManager.Connect();
                        // When done with connection
                        exampleManager.Disconnect();

		'''

                # This is the end of the Demonstration

		exampleWriter.setMode(XdmfHeavyDataWriter.Hyperslab)

		testArray.insert(writeController)

		for i in range(0, size-numServersCores):
			workerComm.Barrier()
			if (i == id):
				print "Core # " + str(id)
				print "Controller stats" 
				print "datasetpath = " + testArray.getHeavyDataController(0).getDataSetPath() 
				print "filepath = " + testArray.getHeavyDataController(0).getFilePath()
				outputVector = testArray.getHeavyDataController(0).getDataspaceDimensions()
				print "Data space dimensions" 
				for j in range(0, outputVector.size()):
					print "[" + str(j) + "] =" + str(outputVector[j])
				print "Controller Dimensions"
				outputVector = testArray.getHeavyDataController(0).getDimensions()
				for j in range(0, outputVector.size()):
					print "[" + str(j) + "] =" + str(outputVector[j])
				print "Controller size"  + str(testArray.getHeavyDataController(0).getSize())
				print "Controller starts"
				outputVector = testArray.getHeavyDataController(0).getStart()
				for j in range(0, outputVector.size()):
					print "[" + str(j) + "] =" + str(outputVector[j])
				print "Controller strides"
				outputVector = testArray.getHeavyDataController(0).getStride()
				for j in range(0, outputVector.size()):
					print "[" + str(j) + "] =" + str(outputVector[j])
				for j in range(0, testArray.getSize()):
					print "core #" + str(id) + " testArray[" + str(j) + "] = " + str(testArray.getValueAsInt32(j))
		testArray.accept(exampleWriter)

		readStartVector = UInt32Vector()
		readStartVector.push_back(4*(size - id - 1 - numServersCores))
		readStrideVector = UInt32Vector()
		readStrideVector.push_back(1)
		readCountVector = UInt32Vector()
		readCountVector.push_back(4)
		readDataSizeVector = UInt32Vector()
		readDataSizeVector.push_back(4*(size-numServersCores))

		readArray = XdmfArray.New()

		readArray.initializeAsInt32(0)
		readArray.reserve(testArray.getSize())

		readController = XdmfHDF5ControllerDSM.New(
			newPath,
			newSetPath,
			XdmfArrayType.Int32(),
			readStartVector,
			readStrideVector,
			readCountVector,
			readDataSizeVector,
			exampleWriter.getServerBuffer())

		readArray.insert(readController)

		if (id == 0):
			print "\n\n\n"

		print "testing read"
		readArray.read()


                for i in range (0, size):
                        workerComm.Barrier()
                        if (i == id):
                                print "Core # " + str(id)
                                print "Controller stats"
                                print "datasetpath = " + readArray.getHeavyDataController(0).getDataSetPath()
                                print "filepath = " + readArray.getHeavyDataController(0).getFilePath()
                                outputVector = readArray.getHeavyDataController(0).getDataspaceDimensions()
                                print "Data space dimensions"
                                for j in range(0, outputVector.size()):
                                        print "[" + str(j) + "] =" + str(outputVector[j])
                                print "Controller Dimensions"
                                outputVector = readArray.getHeavyDataController(0).getDimensions()
                                for j in range(0, outputVector.size()):
                                        print "[" + str(j) + "] =" + str(outputVector[j])
                                print "Controller size" + str(readArray.getHeavyDataController(0).getSize())
                                print "Controller starts"
                                outputVector = readArray.getHeavyDataController(0).getStart()
                                for j in range(0, outputVector.size()):
                                        print "[" + str(j) + "] =" + str(outputVector[j])
                                print "Controller strides"
                                outputVector = readArray.getHeavyDataController(0).getStride()
                                for j in range(0, outputVector.size()):
                                        print "[" + str(j) + "] =" + str(outputVector[j])
                                for j in range (0, readArray.getSize()):
                                        print "core #" + str(id) + " readArray[" + str(j) + "] = " + str(readArray.getValueAsInt32(j))

		workerComm.Barrier()

		# End of Work Section

	if (id == 0):
		exampleWriter.stopDSM()
		'''
		exampleController.stopDSM()
		closeBuffer = exampleWriter.getServerBuffer()
                closeBuffer.SendDone()
		'''

	exampleDSMComm = exampleWriter.getServerBuffer().GetComm()

	exampleInterComm = exampleDSMComm.GetInterComm()
        exampleDSMComm.DupInterComm(exampleInterComm.Dup())

	if (id >= size - numServersCores):
		exampleWriter.getServerBuffer().SendInfo()
	else:
		exampleWriter.getServerBuffer().ReceiveInfo()

	'''
	exampleWriter.restartDSM()
	exampleController.restartDSM()
	'''

	exampleWriter.deleteManager()
	'''
	exampleController.deleteManager()
	'''
