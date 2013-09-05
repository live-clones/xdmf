from Xdmf import *
from mpi4py.MPI import *

if __name__ == "__main__":
        #//initMPI begin

        dsmSize = 64
        comm = COMM_WORLD

        id = comm.Get_rank()
        size = comm.Get_size()

        newPath = "dsm"
        newSetPath = "Data"

        # Initializing objects

        testComm = XdmfDSMCommMPI()
        testComm.DupComm(comm)
        testComm.Init()
        testBuffer = XdmfDSMBuffer()
        testBuffer.SetIsServer(False)
        testBuffer.SetComm(testComm)
        testBuffer.SetIsConnected(True)

        exampleWriter = XdmfHDF5WriterDSM.New(newPath, testBuffer)

        #//initMPI end

        #//ReadDsmPortName begin

        exampleWriter.getServerBuffer().GetComm().ReadDsmPortName()

        #//ReadDsmPortName end

        #//GetDsmPortName begin

        portName = exampleWriter.getServerBuffer().GetComm().GetDsmPortName()

        #//GetDsmPortName end

        #//SetDsmPortName begin

        exampleWriter.getServerBuffer().GetComm().SetDsmPortName(portName)

        #//SetDsmPortName end

        #//Connect begin

        exampleWriter.getServerManager().Connect()

        #//Connect end

        '''

        #//manualConnect begin

        try:
                status = exampleWriter.getServerBuffer().GetComm().Connect()
                if status == MPI_SUCCESS:
                exampleWriter.getServerBuffer().SetIsConnected(true)
                try:
                        exampleWriter.getServerBuffer().ReceiveInfo()
                except RuntimeError as e:
                        # ReceiveInfo failed
                        print e.args[0]
        except RuntimeError as e:
                # Connection failed
                print e.args[0]

        #//manualConnect end

        '''

        exampleWriter.getServerBuffer().GetComm().GetIntraComm().Barrier()

        writeArray = XdmfArray.New()

        for i in range(1, 6):
                writeArray.pushBackAsInt32(i*(id+1))

        writeStartVector = UInt32Vector()
        writeStrideVector = UInt32Vector()
        writeCountVector = UInt32Vector()
        writeDataSizeVector = UInt32Vector()

        writeStartVector.push_back(id*5)
        writeStrideVector.push_back(1)
        writeCountVector.push_back(5)
        writeDataSizeVector.push_back(5*size)

        writeController = XdmfHDF5ControllerDSM.New(
                newPath,
                newSetPath,
                XdmfArrayType.Int32(),
                writeStartVector,
                writeStrideVector,
                writeCountVector,
                writeDataSizeVector,
                exampleWriter.getServerBuffer());

        readStartVector = UInt32Vector()
        readStrideVector = UInt32Vector()
        readCountVector = UInt32Vector()
        readDataSizeVector = UInt32Vector()

        readStartVector.push_back(5*id);
        readStrideVector.push_back(1);
        readCountVector.push_back(5);
        readDataSizeVector.push_back(5*size);

        readController = XdmfHDF5ControllerDSM.New(
                newPath,
                newSetPath,
                XdmfArrayType.Int32(),
                readStartVector,
                readStrideVector,
                readCountVector,
                readDataSizeVector,
                exampleWriter.getServerBuffer());

        exampleWriter.setMode(XdmfHeavyDataWriter.Hyperslab);

        # Done initializing

        for i in range(0, size):
                exampleWriter.getServerBuffer().GetComm().GetIntraComm().Barrier()
                if i == id:
                        outputstream = "Array on core " + str(exampleWriter.getServerBuffer().GetComm().GetInterId()) + " contains:\n"
                        for j in range(0, writeArray.getSize()):
                                outputstream  = outputstream + "[" + str(j) + "]" + str(writeArray.getValueAsInt32(j)) + "\n"
                        print outputstream

        loopamount = 4

        for numloops in range(0, loopamount):
                if writeArray.getNumberHeavyDataControllers() > 0:
                        writeArray.removeHeavyDataController(0);
                writeArray.insert(writeController)


                if id == size - 1:
                        print "\n\n"

                writeArray.accept(exampleWriter)

                if id == size - 1:
                        sentData = 1
                        exampleWriter.getServerBuffer().SendAcknowledgment(exampleWriter.getServerBuffer().GetComm().GetInterId() + 1, sentData, XDMF_DSM_EXCHANGE_TAG, XDMF_DSM_INTER_COMM)
                        exampleWriter.getServerBuffer().ReceiveAcknowledgment(exampleWriter.getServerBuffer().GetComm().GetInterId() + 1, sentData, XDMF_DSM_EXCHANGE_TAG, XDMF_DSM_INTER_COMM)

                exampleWriter.getServerBuffer().GetComm().GetIntraComm().Barrier()

                writeArray.removeHeavyDataController(0)
                writeArray.insert(readController)
                writeArray.release()
                writeArray.read()

                for i in range (0, size):
                        exampleWriter.getServerBuffer().GetComm().GetIntraComm().Barrier()
                        if i == id:
                                outputstream = "Array on core " + str(exampleWriter.getServerBuffer().GetComm().GetInterId()) + " contains:\n"
                                for j in range(0, writeArray.getSize()):
                                        tempVal = writeArray.getValueAsInt32(j)
                                        tempVal = tempVal * 2
                                        writeArray.insertAsInt32(j, [tempVal])
                                        outputstream = outputstream + "[" + str(j) + "]" + str(writeArray.getValueAsInt32(j)) + "\n"
                                print outputstream

        if id == size - 1:
                sentData = 1
                exampleWriter.getServerBuffer().SendAcknowledgment(exampleWriter.getServerBuffer().GetComm().GetInterId() + 1, sentData, XDMF_DSM_EXCHANGE_TAG, XDMF_DSM_INTER_COMM)

        exampleWriter.getServerBuffer().GetComm().GetIntraComm().Barrier()

        exampleWriter.getServerBuffer().GetComm().GetInterComm().Barrier()

        #//Disconnectmanager begin

        exampleWriter.getServerManager().Disconnect()

        #//Disconnectmanager end

        #//Disconnectcomm begin

        exampleWriter.getServerBuffer().GetComm().Disconnect()

        #//Disconnectcomm end
