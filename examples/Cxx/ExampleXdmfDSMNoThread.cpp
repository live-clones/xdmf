#include <mpi.h>
#include <stdlib.h>
#include <iostream>
#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfHDF5WriterDSM.hpp"
#include "XdmfHDF5ControllerDSM.hpp"
#include "XdmfError.hpp"

int main(int argc, char *argv[])
{
	int size, id, dsmSize;
	dsmSize = 64;//The total size of the DSM being created
	MPI_Status status;
	MPI_Comm comm = MPI_COMM_WORLD;

	MPI_Init(&argc, &argv);

	MPI_Comm_rank(comm, &id);
	MPI_Comm_size(comm, &size);


	std::vector<unsigned int> outputVector;

	shared_ptr<XdmfArray> testArray = XdmfArray::New();

	for (unsigned int i = 1; i <= 4; ++i)
	{
		testArray->pushBack(i*(id+1));
	}

	std::string newPath = "dsm";
	std::string newSetPath = "data";

	int numServersCores = 4;

	std::vector<unsigned int> writeStartVector;
	writeStartVector.push_back(id*4);
	//writeStartVector.push_back(id);
	std::vector<unsigned int> writeStrideVector;
	writeStrideVector.push_back(1);
	//writeStrideVector.push_back(size-3);
	std::vector<unsigned int> writeCountVector;
	writeCountVector.push_back(4);
	std::vector<unsigned int> writeDataSizeVector;
	writeDataSizeVector.push_back(4*(size-numServersCores));

        MPI_Comm workerComm;

        MPI_Group workers, dsmgroup;

        MPI_Comm_group(comm, &dsmgroup);
        int * ServerIds = (int *)calloc((numServersCores), sizeof(int));
        unsigned int index = 0;
        for(int i=size-numServersCores ; i <= size-1 ; ++i)
        {
                ServerIds[index++] = i;
        }

        MPI_Group_excl(dsmgroup, index, ServerIds, &workers);
        int testval = MPI_Comm_create(comm, workers, &workerComm);
        cfree(ServerIds);


	shared_ptr<XdmfHDF5WriterDSM> exampleWriter = XdmfHDF5WriterDSM::New(newPath, comm, dsmSize/numServersCores, size-numServersCores, size-1);
/*
	shared_ptr<XdmfHDF5ControllerDSM> exampleController = XdmfHDF5ControllerDSM::New(
		newPath,
		newSetPath,
		XdmfArrayType::Int32(),
		writeStartVector,
		writeStrideVector,
		writeCountVector,
		writeDataSizeVector,
		comm,
		dsmSize/numServersCores,
		size-numServersCores,
		size-1);
*/

	// Split out sub-comm for the worker cores
	// Server cores will not progress to this point until after the servers are done running

	if (id < size - numServersCores)
	{
		// This section is to demonstrate the functionality of the XdmfDSM classes

		exampleWriter->setServerMode(true);
		bool exampleServerMode = exampleWriter->getServerMode();

		/*
		MPI_Comm exampleServerComm = exampleWriter->getServerComm();
		exampleWriter->setServerComm(exampleServerComm);
		*/

		MPI_Comm exampleWorkerComm = exampleWriter->getWorkerComm();
		exampleWriter->setWorkerComm(exampleWorkerComm);

/*
		MPI_Comm exampleServerComm = exampleController->getServerComm();
		exampleController->setServerComm(exampleServerComm);

		MPI_Comm exampleWorkerComm = exampleController->getWorkerComm();
		exampleController->setWorkerComm(exampleWorkerComm);
*/

		shared_ptr<XdmfHDF5ControllerDSM> writeController = XdmfHDF5ControllerDSM::New(
			newPath,
			newSetPath,
			XdmfArrayType::Int32(),
			writeStartVector,
			writeStrideVector,
			writeCountVector,
			writeDataSizeVector,
			exampleWriter->getServerBuffer(),
	                size-numServersCores,
			size-1);

		shared_ptr<XdmfHDF5WriterDSM> exampleWriter2 = XdmfHDF5WriterDSM::New(newPath, exampleWriter->getServerBuffer(), size-numServersCores, size-1);

		writeController->setServerMode(true);
		bool exampleControllerServerMode = writeController->getServerMode();

		XdmfDSMManager * exampleManager;

		exampleManager = exampleWriter->getServerManager();
		exampleWriter->setManager(exampleManager);
/*
		exampleManager = exampleController->getServerManager();
		exampleController->setManager(exampleManager);
*/

		int intraId = exampleManager->GetUpdatePiece();
		int intraSize = exampleManager->GetUpdateNumPieces();

		MPI_Comm exampleManagerComm = exampleManager->GetMpiComm();
		exampleManager->SetMpiComm(exampleManagerComm);

		int exampleBufferSize = exampleManager->GetLocalBufferSizeMBytes();
		exampleManager->SetLocalBufferSizeMBytes(exampleBufferSize);

		bool exampleIsServer = exampleManager->GetIsServer();
		exampleManager->SetIsServer(exampleIsServer);

		int exampleType = exampleManager->GetDsmType();
		exampleManager->SetDsmType(XDMF_DSM_TYPE_UNIFORM);

		long exampleBlockLength = exampleManager->GetBlockLength();
		exampleManager->SetBlockLength(exampleBlockLength);

		int exampleCommType = exampleManager->GetInterCommType();
		exampleManager->SetInterCommType(XDMF_DSM_COMM_MPI);

		bool exampleManagerConnectionStatus = exampleManager->GetIsConnected();

	/*
		exampleManager->Destroy();
		exampleManager->Create(size - numServerCores, size - 1);
	*/

		XdmfDSMBuffer * exampleBuffer;

		exampleBuffer = exampleWriter->getServerBuffer();
		exampleWriter->setBuffer(exampleBuffer);
/*
		exampleBuffer = exampleController->getServerBuffer();
		exampleController->setBuffer(exampleBuffer);
*/
		exampleManager->SetDsmBuffer(exampleBuffer);
		exampleBuffer = exampleManager->GetDsmBuffer();

		bool exampleIsConnected = exampleBuffer->GetIsConnected();
		exampleBuffer->SetIsConnected(exampleIsConnected);

		char * exampleDataPointer = exampleBuffer->GetDataPointer();

		int exampleDSMType = exampleBuffer->GetDsmType();
		exampleBuffer->SetDsmType(XDMF_DSM_TYPE_UNIFORM);

		bool exampleBufferIsServer = exampleBuffer->GetIsServer();
		exampleBuffer->SetIsServer(exampleIsServer);

		int exampleBufferStart = exampleBuffer->GetStartAddress();
		int exampleBufferEnd = exampleBuffer->GetEndAddress();

		int exampleServerStart = exampleBuffer->GetStartServerId();
		int exampleServerEnd = exampleBuffer->GetEndServerId();

		long exampleBufferLength = exampleBuffer->GetLength();
		long exampleTotalBufferLength = exampleBuffer->GetTotalLength();

		long exampleBufferBlockLength = exampleBuffer->GetBlockLength();
		exampleBuffer->SetBlockLength(exampleBufferBlockLength);

		/*
		exampleBuffer->ConfigureUniform(exampleBuffer->GetComm(), dsmSize/numServersCores, size - numServersCores, size - 1);
		*/

		if (id == 0)
		{
			exampleBuffer->SendCommandHeader(XDMF_DSM_LOCK_ACQUIRE, 1, 0, 0, XDMF_DSM_INTRA_COMM);
		}

		if (id == 1)
		{
			int probeComm;
			exampleBuffer->ProbeCommandHeader(&probeComm);

			if (probeComm == XDMF_DSM_INTER_COMM)
			{
				std::cout << "InterComm" << std::endl;
			}
			else
			{
				std::cout << "IntraComm" << std::endl;
			}			
			int length;
			int address;
			int opcode;
			int source;
			exampleBuffer->ReceiveCommandHeader(&opcode, &source, &length, &address, XDMF_DSM_INTRA_COMM, 0);
		}


		if (id == 0)
		{
			char * sentData = "datastring";
			exampleBuffer->SendData(1, sentData, 0, XDMF_DSM_PUT_DATA_TAG, 0, XDMF_DSM_INTER_COMM);
		}

		if (id == 1)
		{
			int length;
			int address;
			char * recvData;
			exampleBuffer->ReceiveData(0, recvData, length, XDMF_DSM_PUT_DATA_TAG, address, XDMF_DSM_INTER_COMM);
		}

		if (id == 0)
	        {
	                int sentData = 1;
	                exampleBuffer->SendAcknowledgment(1, sentData, XDMF_DSM_PUT_DATA_TAG, XDMF_DSM_INTER_COMM);
	        }

	        if (id == 1)
	        {
	                int recvData;
	                exampleBuffer->ReceiveAcknowledgment(0, recvData, XDMF_DSM_PUT_DATA_TAG, XDMF_DSM_INTER_COMM);
	        }

                MPI_Comm readComm, writeComm;

                MPI_Group readingCores, writingCores;

                MPI_Comm_group(workerComm, &workers);
                int * ServerIds = (int *)calloc(((size - numServersCores) / 2), sizeof(int));
                unsigned int index = 0;
                for(int i=0 ; i < (int)((size - numServersCores) / 2) ; ++i)
                {
                        ServerIds[index++] = i;
                }

                MPI_Group_excl(workers, index, ServerIds, &writingCores);
                testval = MPI_Comm_create(workerComm, writingCores, &writeComm);
                MPI_Group_incl(workers, index, ServerIds, &readingCores);
                testval = MPI_Comm_create(workerComm, readingCores, &readComm);
                cfree(ServerIds);


		if (id < (int)((size - numServersCores) / 2))
		{
			exampleBuffer->GetComm()->DupComm(readComm);
			exampleBuffer->ReceiveInfo();
		}
		else
		{
			exampleBuffer->GetComm()->DupComm(writeComm);
			exampleBuffer->SendInfo();
		}

		exampleBuffer->GetComm()->DupComm(workerComm);

		int broadcastComm = XDMF_DSM_INTER_COMM;
		exampleBuffer->BroadcastComm(&broadcastComm, 0);

		if (id == 0)
		{
			int returnCode;
			exampleBuffer->BufferServiceLoop(&returnCode);
			int serviceOut = exampleBuffer->BufferService(&returnCode);
		}
		
		if (id == 1)
		{
			exampleBuffer->SendCommandHeader(XDMF_DSM_OPCODE_DONE, 0, 0, 0, XDMF_DSM_INTER_COMM);
			exampleBuffer->SendCommandHeader(XDMF_DSM_OPCODE_DONE, 0, 0, 0, XDMF_DSM_INTER_COMM);
		}

		int core0StartAddress = 0;
		int core0EndAddress = 0;
		exampleBuffer->GetAddressRangeForId(0, &core0StartAddress, &core0EndAddress);

		int correspondingId = exampleBuffer->AddressToId(500);

		int dsmData = 5;
		if (sizeof(int)/sizeof(char) + core0StartAddress < core0EndAddress)
		{
			exampleBuffer->Put(0, sizeof(int)/sizeof(char), &dsmData);
			exampleBuffer->Get(0, sizeof(int)/sizeof(char), &dsmData);
		}
		else
		{
			// Error occured
			XdmfError::message(XdmfError::FATAL, "Address out of range");
		}

		XdmfDSMCommMPI * exampleDSMComm = exampleBuffer->GetComm();
		exampleBuffer->SetComm(exampleDSMComm);

		int exampleIntraID = exampleDSMComm->GetId();
		int exampleIntraSize = exampleDSMComm->GetIntraSize();
		int exampleInterSize = exampleDSMComm->GetInterSize();

		int exampleInterCommType = exampleDSMComm->GetInterCommType();

		exampleDSMComm->Init();

		MPI_Comm exampleIntraComm = exampleDSMComm->GetIntraComm();
		exampleDSMComm->DupComm(workerComm);

	/*
		bool connectingGroup;
		char * portString;
		if (id < 5)
		{
			connectingGroup = true;
		}
		else
		{
			connectingGroup = false;
		}

		if (!connectingGroup)
		{
			exampleDSMComm->OpenPort();
			portString = exampleDSMComm->GetDsmMasterHostName();
			// Send the port string to the connecting group
			exampleDSMComm->Accept();

			// When done with connection;
			exampleDSMComm->ClosePort();
		}
	
		if (connectingGroup)
		{
			// Recieve string from Master group
			exampleDSMComm->SetDsmMasterHostName(portString);
			exampleDSMComm->Connect();

			// When done with connection
			exampleDSMComm->Disconnect();
		}

		if (connectingGroup)
		{
			// Recieve string from Master group
			exampleDSMComm->SetDsmMasterHostName(portString);
			exampleManager->Connect();

			// When done with connection
			exampleManager->Disconnect();
		}

	*/


		// This is the end of the Demonstration

		exampleWriter->setMode(XdmfHeavyDataWriter::Hyperslab);

		testArray->insert(writeController);

		for (unsigned int i = 0; i<size-numServersCores; ++i)
		{
			MPI_Barrier(workerComm);
			if (i == id)
			{
				std::cout << "Core # " << id << std::endl;
				std::cout << "Controller stats" << std::endl;
				std::cout << "datasetpath = " << testArray->getHeavyDataController(0)->getDataSetPath() << std::endl;
				std::cout << "filepath = " << testArray->getHeavyDataController(0)->getFilePath() << std::endl;
				outputVector = testArray->getHeavyDataController(0)->getDataspaceDimensions();
				std::cout << "Data space dimensions" << std::endl;
				for (unsigned int j=0; j<outputVector.size(); ++j)
				{
					std::cout << "[" << j << "] =" << outputVector[j] << std::endl;
				}
				std::cout << "Controller Dimensions" << std::endl;
				outputVector = testArray->getHeavyDataController(0)->getDimensions();
				for (unsigned int j=0; j<outputVector.size(); ++j)
				{
					std::cout << "[" << j << "] =" << outputVector[j] << std::endl;
				}
				std::cout << "Controller size" << testArray->getHeavyDataController(0)->getSize() << std::endl;
				std::cout << "Controller starts" << std::endl;
				outputVector = testArray->getHeavyDataController(0)->getStart();
				for (unsigned int j=0; j<outputVector.size(); ++j)
				{
					std::cout << "[" << j << "] =" << outputVector[j] << std::endl;
				}
				std::cout << "Controller strides" << std::endl;
				outputVector = testArray->getHeavyDataController(0)->getStride();
				for (unsigned int j=0; j<outputVector.size(); ++j)
				{
					std::cout << "[" << j << "] =" << outputVector[j] << "\n" << std::endl;
				}
				for(unsigned int j=0; j<testArray->getSize(); ++j)
				{
					std::cout << "core #" << id <<" testArray[" << j << "] = " << testArray->getValue<int>(j) << std::endl;
				}
			}
		}
		testArray->accept(exampleWriter);
	
		std::vector<unsigned int> readStartVector;
		readStartVector.push_back(4*(size - id - 1 - numServersCores));
		std::vector<unsigned int> readStrideVector;
		readStrideVector.push_back(1);
		std::vector<unsigned int> readCountVector;
		readCountVector.push_back(4);
		std::vector<unsigned int> readDataSizeVector;
		readDataSizeVector.push_back(4*(size-numServersCores));

		shared_ptr<XdmfArray> readArray = XdmfArray::New();

		readArray->initialize<int>(0);
		readArray->reserve(testArray->getSize());

		shared_ptr<XdmfHDF5ControllerDSM> readController = XdmfHDF5ControllerDSM::New(
			newPath,
			newSetPath,
			XdmfArrayType::Int32(),
			readStartVector,
			readStrideVector,
			readCountVector,
			readDataSizeVector,
			exampleWriter->getServerBuffer(),
       	        	size-numServersCores,
       		        size-1);

		readArray->insert(readController);

		if (id == 0)
		{
			printf("\n\n\n");
		}

		std::cout << "testing read" << std::endl;
		readArray->read();
	

	        for (unsigned int i = 0; i<size; ++i)
		{
			MPI_Barrier(workerComm);
			if (i == id)
			{
				std::cout << "Core # " << id << std::endl;
				std::cout << "Controller stats" << std::endl;
				std::cout << "datasetpath = " << readArray->getHeavyDataController(0)->getDataSetPath() << std::endl;
				std::cout << "filepath = " << readArray->getHeavyDataController(0)->getFilePath() << std::endl;
				outputVector = readArray->getHeavyDataController(0)->getDataspaceDimensions();
				std::cout << "Data space dimensions" << std::endl;
				for (unsigned int j=0; j<outputVector.size(); ++j)
				{
					std::cout << "[" << j << "] =" << outputVector[j] << std::endl;
				}
				std::cout << "Controller Dimensions" << std::endl;
				outputVector = readArray->getHeavyDataController(0)->getDimensions();
				for (unsigned int j=0; j<outputVector.size(); ++j)
				{
					std::cout << "[" << j << "] =" << outputVector[j] << std::endl;
				}
				std::cout << "Controller size" << readArray->getHeavyDataController(0)->getSize() << std::endl;
				std::cout << "Controller starts" << std::endl;
				outputVector = readArray->getHeavyDataController(0)->getStart();
				for (unsigned int j=0; j<outputVector.size(); ++j)
				{
					std::cout << "[" << j << "] =" << outputVector[j] << std::endl;
				}
				std::cout << "Controller strides" << std::endl;
				outputVector = readArray->getHeavyDataController(0)->getStride();
				for (unsigned int j=0; j<outputVector.size(); ++j)
				{
					std::cout << "[" << j << "] =" << outputVector[j] << "\n" << std::endl;
				}
				for(unsigned int j=0; j<readArray->getSize(); ++j)
				{
					std::cout << "core #" << id <<" readArray[" << j << "] = " << readArray->getValue<int>(j) << std::endl;
				}
			}
		}

		MPI_Barrier(workerComm);

		// End of Work Section
	}

	exampleWriter->closeFile();

	if (id == 0)
        {
		exampleWriter->stopDSM();
	/*
		exampleController->stopDSM();
		XdmfDSMBuffer closeBuffer = exampleWriter->getServerBuffer();
		closeBuffer->SendDone();
	*/
        }

	XdmfDSMCommMPI * exampleDSMComm = exampleWriter->getServerBuffer()->GetComm();
	MPI_Comm exampleInterComm = exampleDSMComm->GetInterComm();
	exampleDSMComm->DupInterComm(comm);

	/*
	exampleWriter->restartDSM();
	exampleController->restartDSM();
	*/

	MPI_Barrier(comm);

	//the dsmManager must be deleted or else there will be a segfault
	exampleWriter->deleteManager();

	MPI_Finalize();

	return 0;
}
