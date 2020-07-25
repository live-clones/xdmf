from mpi4py.MPI import *
from Xdmf import *

if __name__ == "__main__":
        #//initMPI begin

        #The total size of the DSM being created
        dsmSize = 64
        comm = COMM_WORLD

        id = comm.Get_rank()
        size = comm.Get_size()

        #//initMPI end

        testArray = XdmfArray.New()
        testArray.initializeAsInt32(0)

        for i in range(1,5):
                testArray.pushBackAsInt32(i*(id+1))

        #//initwritevector begin

        newPath = "dsm"
        newSetPath = "data"

        numServersCores = 2

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

        #//initwritevector end

        #//splitcomm begin

        ServerIds = []

        for i in range(size-numServersCores, size):
                ServerIds.append(i)

        workers = comm.Get_group().Excl(ServerIds)

        workerComm = comm.Create(workers)

        #//splitcomm end

        #//initwritergenerate begin

        exampleWriter = XdmfHDF5WriterDSM.New(newPath, comm, dsmSize/numServersCores, size-numServersCores, size-1)

        #//initwritergenerate end

        '''
        #//initcontrollergenerate begin

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

        #//initcontrollergenerate end
        '''

        

        # Split out sub-comm for the worker cores
        # Server cores will not progress to this point until after the servers are done running

        #//startworksection begin

        if (id < size - numServersCores):

                #//startworksection end

                # This section is to demonstrate the functionality of the XdmfDSM classes

                #//setServerModewriter begin

                exampleWriter.setServerMode(True)

                #//setServerModewriter end

                #//getServerModewriter begin

                exampleServerMode = exampleWriter.getServerMode()

                #//getServerModewriter end

                #//getWorkerCommwriter begin

                exampleWorkerComm = exampleWriter.getWorkerComm()

                #//getWorkerCommwriter end

                #//setWorkerCommwriter begin

                exampleWriter.setWorkerComm(exampleWorkerComm)

                #//setWorkerCommwriter end

                '''
                #//getWorkerCommcontroller begin

                exampleWorkerComm = exampleController.getWorkerComm()

                #//getWorkerCommcontroller end

                #//setWorkerCommcontroller begin

                exampleController.setWorkerComm(exampleWorkerComm)

                #//setWorkerCommcontroller end

                #//setServerModecontroller begin

                exampleController.setServerMode(True)

                #//setServerModecontroller end

                #//getServerModecontroller begin

                exampleControllerServerMode = exampleController.getServerMode()

                #//getServerModecontroller end
                '''

                #//initcontrollerfrombuffer begin

                writeController = XdmfHDF5ControllerDSM.New(
                        newPath,
                        newSetPath,
                        XdmfArrayType.Int32(),
                        writeStartVector,
                        writeStrideVector,
                        writeCountVector,
                        writeDataSizeVector,
                        exampleWriter.getServerBuffer());

                #//initcontrollerfrombuffer end

                #//initwriterfrombuffer begin

                exampleWriter2 = XdmfHDF5WriterDSM.New(newPath, exampleWriter.getServerBuffer());

                #//initwriterfrombuffer end

                writeController.setServerMode(True)
                exampleControllerServerMode = writeController.getServerMode()

                #//getServerBufferwriter begin

                exampleBuffer = exampleWriter.getServerBuffer()

                #//getServerBufferwriter end

                #//setBufferwriter begin

                exampleWriter.setBuffer(exampleBuffer)

                #//setBufferwriter end

                '''
                #//getServerBuffercontroller begin

                exampleBuffer = exampleController.getServerBuffer()

                #//getServerBuffercontroller end

                #//setBuffercontroller begin

                exampleController.setBuffer(exampleBuffer)

                #//setBuffercontroller end
                '''

                #//GetIsConnectedbuffer begin

                exampleIsConnected = exampleBuffer.GetIsConnected()

                #//GetIsConnectedbuffer end

                #//SetIsConnectedbuffer begin

                exampleBuffer.SetIsConnected(exampleIsConnected)

                #//SetIsConnectedbuffer end

                #//GetDataPointer begin

                exampleDataPointer = exampleBuffer.GetDataPointer()

                #//GetDataPointer end

                #//GetDsmTypebuffer begin

                exampleDSMType = exampleBuffer.GetDsmType()

                #//GetDsmTypebuffer end

                #//SetDsmTypebuffer begin

                exampleBuffer.SetDsmType(XDMF_DSM_TYPE_UNIFORM)

                #//SetDsmTypebuffer end

                #//GetIsServerbuffer begin

                exampleBufferIsServer = exampleBuffer.GetIsServer()

                #//GetIsServerbuffer end

                #//SetIsServerbuffer begin

                exampleBuffer.SetIsServer(exampleBufferIsServer)

                #//SetIsServerbuffer end

                #//GetStartAddress begin

                exampleBufferStart = exampleBuffer.GetStartAddress()

                #//GetStartAddress end

                #//GetEndAddress begin

                exampleBufferEnd = exampleBuffer.GetEndAddress()

                #//GetEndAddress end

                #//GetStartServerId begin

                exampleServerStart = exampleBuffer.GetStartServerId()

                #//GetStartServerId end

                #//GetEndServerid begin

                exampleServerEnd = exampleBuffer.GetEndServerId()

                #//GetEndServerId end

                #//GetLength begin

                exampleBufferLength = exampleBuffer.GetLength()

                #//GetLength end

                #//GetTotalLength begin

                exampleTotalBufferLength = exampleBuffer.GetTotalLength()

                #//GetTotalLength end

                #//GetBlockLengthbuffer begin

                exampleBufferBlockLength = exampleBuffer.GetBlockLength()

                #//GetBlockLengthbuffer end

                #//SetBlockLengthbuffer begin

                exampleBuffer.SetBlockLength(exampleBufferBlockLength)

                #//SetBlockLengthbuffer end

                '''
                #//ConfigureUniform begin

                exampleBuffer.ConfigureUniform(exampleBuffer.GetComm(), dsmSize/numServersCores, size - numServersCores, size - 1)

                #//ConfigureUniform end
                '''
                sendingCore = -1

                #//SendCommandHeader begin

                if (id == sendingCore):
                        exampleBuffer.SendCommandHeader(XDMF_DSM_OPCODE_DONE, 1, 0, 0, XDMF_DSM_INTER_COMM)

                #//SendCommandHeader end

                #//SendData begin

                if (id == sendingCore):
                        sentData = "datastring"
                        exampleBuffer.SendData(1, sentData, 0, XDMF_DSM_PUT_DATA_TAG, 0, XDMF_DSM_INTER_COMM)

                #//SendData end

                #//SendAcknowledgment begin

                if (id == 1):
                        sentData = 1
                        exampleBuffer.SendAcknowledgment(0, sentData, XDMF_DSM_PUT_DATA_TAG, XDMF_DSM_INTER_COMM)

                #//SendAcknowledgment end

                #//ReceiveAcknowledgement begin

                if (id == 0):
                        recvData = 0
                        exampleBuffer.ReceiveAcknowledgment(1, recvData, XDMF_DSM_PUT_DATA_TAG, XDMF_DSM_INTER_COMM)

                #//ReceiveAcknowledgement end



                ServerIds = []

                for i in range(0, (size - numServersCores) / 2):
                        ServerIds.append(i)

                readingCores = workerComm.Get_group().Excl(ServerIds)

                readComm = workerComm.Create(readingCores)

                writingCores = workerComm.Get_group().Incl(ServerIds)

                writeComm = workerComm.Create(writingCores)

                exampleBuffer.GetComm().DupComm(workerComm)

                #//BufferService begin

                if (id == 0):
                        serviceOut = exampleBuffer.BufferService()

                if (id == 1):
                        exampleBuffer.SendCommandHeader(XDMF_DSM_OPCODE_DONE, 0, 0, 0, XDMF_DSM_INTER_COMM)

                #//BufferService end

                #//BufferServiceLoop begin

                if (id == 0):
                        exampleBuffer.BufferServiceLoop()

                if (id == 1):
                        exampleBuffer.SendCommandHeader(XDMF_DSM_OPCODE_DONE, 0, 0, 0, XDMF_DSM_INTER_COMM)

                #//BufferServiceLoop end

                #//GetComm begin

                exampleDSMComm = exampleBuffer.GetComm()

                #//GetComm end

                #//SetComm begin

                exampleBuffer.SetComm(exampleDSMComm)

                #//SetComm end

                #//GetId begin

                exampleIntraID = exampleDSMComm.GetId()

                #//GetId end

                #//GetIntraSize begin

                exampleIntraSize = exampleDSMComm.GetIntraSize()

                #//GetIntraSize end

                #//GetInterId begin

                exampleInterID = exampleDSMComm.GetInterId()

                #//GetInterId end

                #//GetInterSize begin

                exampleInterSize = exampleDSMComm.GetInterSize()

                #//GetInterSize end

                #//GetInterCommType begin

                exampleInterCommType = exampleDSMComm.GetInterCommType()

                #//GetInterCommType end

                #//initcomm begin

                exampleDSMComm.Init()

                #//initcomm end

                #//GetIntraComm begin

                exampleIntraComm = exampleDSMComm.GetIntraComm()

                #//GetIntraComm end

                #//DupComm begin

                exampleDSMComm.DupComm(exampleIntraComm.Dup())

                #//DupComm end



                print(type(exampleDSMComm.GetDsmPortName()))
                testName = "test"
                exampleDSMComm.SetDsmPortName(testName)
                print(exampleDSMComm.GetDsmPortName())

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

                '''

                # This is the end of the Demonstration

                exampleWriter.setMode(XdmfHeavyDataWriter.Hyperslab)

                testArray.insert(writeController)

                for i in range(0, size-numServersCores):
                        workerComm.Barrier()
                        if (i == id):
                                print("Core # " + str(id))
                                print("Controller stats" )
                                print("datasetpath = " + XdmfHeavyDataController.XdmfHDF5ControllerCast(testArray.getHeavyDataController(0)).getDataSetPath() )
                                print("filepath = " + testArray.getHeavyDataController(0).getFilePath())
                                outputVector = XdmfHeavyDataController.XdmfHDF5ControllerCast(testArray.getHeavyDataController(0)).getDataspaceDimensions()
                                print("Data space dimensions" )
                                for j in range(0, outputVector.size()):
                                        print("[" + str(j) + "] =" + str(outputVector[j]))
                                print("Controller Dimensions")
                                outputVector = testArray.getHeavyDataController(0).getDimensions()
                                for j in range(0, outputVector.size()):
                                        print("[" + str(j) + "] =" + str(outputVector[j]))
                                print("Controller size"  + str(testArray.getHeavyDataController(0).getSize()))
                                print("Controller starts")
                                outputVector = XdmfHeavyDataController.XdmfHDF5ControllerCast(testArray.getHeavyDataController(0)).getStart()
                                for j in range(0, outputVector.size()):
                                        print("[" + str(j) + "] =" + str(outputVector[j]))
                                print("Controller strides")
                                outputVector = XdmfHeavyDataController.XdmfHDF5ControllerCast(testArray.getHeavyDataController(0)).getStride()
                                for j in range(0, outputVector.size()):
                                        print("[" + str(j) + "] =" + str(outputVector[j]))
                                for j in range(0, testArray.getSize()):
                                        print("core #" + str(id) + " testArray[" + str(j) + "] = " + str(testArray.getValueAsInt32(j)))
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
                        print("\n\n\n")

                print("testing read")
                readArray.read()


                for i in range (0, size):
                        workerComm.Barrier()
                        if (i == id):
                                print("Core # " + str(id))
                                print("Controller stats")
                                print("datasetpath = " + XdmfHeavyDataController.XdmfHDF5ControllerCast(readArray.getHeavyDataController(0)).getDataSetPath())
                                print("filepath = " + readArray.getHeavyDataController(0).getFilePath())
                                outputVector = XdmfHeavyDataController.XdmfHDF5ControllerCast(readArray.getHeavyDataController(0)).getDataspaceDimensions()
                                print("Data space dimensions")
                                for j in range(0, outputVector.size()):
                                        print("[" + str(j) + "] =" + str(outputVector[j]))
                                print("Controller Dimensions")
                                outputVector = readArray.getHeavyDataController(0).getDimensions()
                                for j in range(0, outputVector.size()):
                                        print("[" + str(j) + "] =" + str(outputVector[j]))
                                print("Controller size" + str(readArray.getHeavyDataController(0).getSize()))
                                print("Controller starts")
                                outputVector = XdmfHeavyDataController.XdmfHDF5ControllerCast(readArray.getHeavyDataController(0)).getStart()
                                for j in range(0, outputVector.size()):
                                        print("[" + str(j) + "] =" + str(outputVector[j]))
                                print("Controller strides")
                                outputVector = XdmfHeavyDataController.XdmfHDF5ControllerCast(readArray.getHeavyDataController(0)).getStride()
                                for j in range(0, outputVector.size()):
                                        print("[" + str(j) + "] =" + str(outputVector[j]))
                                for j in range (0, readArray.getSize()):
                                        print("core #" + str(id) + " readArray[" + str(j) + "] = " + str(readArray.getValueAsInt32(j)))

                workerComm.Barrier()

                # End of Work Section

        #//stopDSMwriter begin

        if (id == 0):
                exampleWriter.stopDSM()

        #//stopDSMwriter end

        '''
        #//stopDSMcontroller begin


        if (id == 0):
                exampleController.stopDSM()

        #//stopDSMcontroller end

        #//sendDone begin

        if (id == 0):
                closeBuffer = exampleWriter.getServerBuffer()
                closeBuffer.SendDone()

        #//sendDone end

        '''
        #//GetInterComm begin

        exampleDSMComm = exampleWriter.getServerBuffer().GetComm()

        exampleInterComm = exampleDSMComm.GetInterComm()

        #//GetInterComm end

        #//DupInterComm begin

        exampleDSMComm.DupInterComm(exampleInterComm.Dup())

        #//DupInterComm end

        #//SendRecvInfo begin

        if (id >= size - numServersCores):
                exampleWriter.getServerBuffer().SendInfo()
        else:
                exampleWriter.getServerBuffer().ReceiveInfo()

        #//SendRecvInfo end

        '''
        #//restartDSMwriter begin

        exampleWriter.restartDSM()

        #//restartDSMwriter end

        #//restartDSMcontroller begin

        exampleController.restartDSM()

        #//restartDSMcontroller end
        '''

        #//finalizeMPI

#        exampleWriter.deleteManager()

        #//finalizeMPI

        '''
        exampleController.deleteManager()
        '''
