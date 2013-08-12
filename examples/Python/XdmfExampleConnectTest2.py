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

	readStartVector = UInt32Vector()
	readStrideVector = UInt32Vector()
	readCountVector = UInt32Vector()
	readDataSizeVector = UInt32Vector()

	readStartVector.push_back(5*id)
	readStrideVector.push_back(1)
	readCountVector.push_back(5)
	readDataSizeVector.push_back(5*size)

	readArray = XdmfArray.New()

	readArray.initializeAsInt32(0)
	readArray.reserve(5)

	readController = XdmfHDF5ControllerDSM.New(
		newPath,
		newSetPath,
		XdmfArrayType.Int32(),
		readStartVector,
		readStrideVector,
		readCountVector,
		readDataSizeVector,
		testBuffer);

	readController.getServerBuffer().GetComm().ReadDsmPortName();

	readController.getServerManager().Connect();

	exampleWriter = XdmfHDF5WriterDSM.New(newPath, testBuffer);

	writeStartVector = UInt32Vector();
	writeStrideVector = UInt32Vector();
	writeCountVector = UInt32Vector();
	writeDataSizeVector = UInt32Vector();

	writeStartVector.push_back(id*5);
	writeStrideVector.push_back(1);
	writeCountVector.push_back(5);
	writeDataSizeVector.push_back(5*size);

	writeController = XdmfHDF5ControllerDSM.New(
		newPath,
		newSetPath,
		XdmfArrayType.Int32(),
		writeStartVector,
		writeStrideVector,
		writeCountVector,
		writeDataSizeVector,
		exampleWriter.getServerBuffer());

	exampleWriter.setMode(XdmfHeavyDataWriter.Hyperslab);

	# Done initialization

	readController.getServerBuffer().GetComm().GetIntraComm().Barrier()

	loopamount = 4

	print str(range(0, loopamount)) + "'s type = " + str(type(range(0, loopamount)))

        for numloops in range(0, loopamount):
                if id == 0:
                        receiveData = 0
                        readController.getServerBuffer().ReceiveAcknowledgment(readController.getServerBuffer().GetComm().GetInterId() - 1, receiveData, XDMF_DSM_EXCHANGE_TAG, XDMF_DSM_INTER_COMM)

                readController.getServerBuffer().GetComm().GetIntraComm().Barrier()

                if readArray.getNumberHeavyDataControllers() > 0:
                        readArray.removeHeavyDataController(0)
                readArray.insert(readController)
                readArray.read()

                for i in range(0, size):
                        readController.getServerBuffer().GetComm().GetIntraComm().Barrier()
                        if i == id:
				outputstream = " "
                                outputstream = outputstream + "Array on core " + str(exampleWriter.getServerBuffer().GetComm().GetInterId()) + " contains:\n"
				for j in range(0, readArray.getSize()):
                                        tempVal = readArray.getValueAsInt32(j)
                                        tempVal = tempVal * 3
                                        readArray.insertAsInt32(j, [tempVal])
                                        outputstream = outputstream + "[" + str(j) + "]" + str(readArray.getValueAsInt32(j))+ "\n"
                                print outputstream

                readController.getServerBuffer().GetComm().GetIntraComm().Barrier()
                if id == 0:
                        print "\n\n"

                readArray.removeHeavyDataController(0)
                readArray.insert(writeController)

                readArray.accept(exampleWriter)

                if id == 0:
                        receiveData = 0
                        readController.getServerBuffer().SendAcknowledgment(readController.getServerBuffer().GetComm().GetInterId() - 1, receiveData, XDMF_DSM_EXCHANGE_TAG, XDMF_DSM_INTER_COMM)


        # This last acknowledgment is to end the loop.

        if id == 0:
                receiveData = 0;
                readController.getServerBuffer().ReceiveAcknowledgment(readController.getServerBuffer().GetComm().GetInterId() - 1, receiveData, XDMF_DSM_EXCHANGE_TAG, XDMF_DSM_INTER_COMM)

        readController.getServerBuffer().GetComm().GetIntraComm().Barrier()

        if id == 0:
                readController.stopDSM()

        readController.getServerBuffer().GetComm().GetInterComm().Barrier()

	readController.getServerManager().Disconnect()
