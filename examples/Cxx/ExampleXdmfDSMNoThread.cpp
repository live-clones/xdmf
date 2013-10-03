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
        //#initMPI begin

        int size, id, dsmSize;
        dsmSize = 64;//The total size of the DSM being created
        MPI_Status status;
        MPI_Comm comm = MPI_COMM_WORLD;

        MPI_Init(&argc, &argv);

        MPI_Comm_rank(comm, &id);
        MPI_Comm_size(comm, &size);

        //#initMPI end


        std::vector<unsigned int> outputVector;

        shared_ptr<XdmfArray> testArray = XdmfArray::New();

        for (unsigned int i = 1; i <= 4; ++i)
        {
                testArray->pushBack(i*(id+1));
        }

        //#initwritevector begin

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

        //#initwritervector end

        //#commsplit begin

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

        //#commsplit end

        //#initwritergenerate begin

        shared_ptr<XdmfHDF5WriterDSM> exampleWriter = XdmfHDF5WriterDSM::New(newPath, comm, dsmSize/numServersCores, size-numServersCores, size-1);

        //#initwritergenerate end
/*

        //#initcontrollergenerate begin
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

        //#initcontrollergenerate end
*/

        // Split out sub-comm for the worker cores
        // Server cores will not progress to this point until after the servers are done running

        //#startworksection begin

        if (id < size - numServersCores)
        {

                //#startworksection end

                // This section is to demonstrate the functionality of the XdmfDSM classes

                //#setServerModewriter begin

                exampleWriter->setServerMode(true);

                //#setServerModewriter end

                //#getServerModewriter begin

                bool exampleServerMode = exampleWriter->getServerMode();

                //#getServerModewriter end

                //#getWorkerCommwriter begin

                MPI_Comm exampleWorkerComm = exampleWriter->getWorkerComm();

                //#getWorkerCommwriter end

                //#setWorkerCommwriter begin

                exampleWriter->setWorkerComm(exampleWorkerComm);

                //#setWorkerCommwriter end

/*
                //#getWorkerCommcontroller begin

                MPI_Comm exampleWorkerComm = exampleController->getWorkerComm();

                //#getWorkerCommcontroller end

                //#setWorkerCommcontroller begin

                exampleController->setWorkerComm(exampleWorkerComm);

                //#setWorkerCommcontroller end

                //#setServerModecontroller begin

                exampleController->setServerMode(true);

                //#setServerModecontroller end

                //#getServerModecontroller begin

                bool exampleControllerServerMode = exampleController->getServerMode();

                //#getServerModecontroller end
*/

                //#initcontrollerwithbuffer begin

                shared_ptr<XdmfHDF5ControllerDSM> writeController = XdmfHDF5ControllerDSM::New(
                        newPath,
                        newSetPath,
                        XdmfArrayType::Int32(),
                        writeStartVector,
                        writeStrideVector,
                        writeCountVector,
                        writeDataSizeVector,
                        exampleWriter->getServerBuffer());

                //#initcontrollerwithbuffer end

                //#initwriterwithbuffer begin

                shared_ptr<XdmfHDF5WriterDSM> exampleWriter2 = XdmfHDF5WriterDSM::New(newPath, exampleWriter->getServerBuffer());

                //#initwriterwithbuffer end

                writeController->setServerMode(true);
                bool exampleControllerServerMode = writeController->getServerMode();

                //#declaremanager begin

                XdmfDSMManager * exampleManager;

                //#declaremanager end

                //#getServerManagerwriter begin

                exampleManager = exampleWriter->getServerManager();

                //#getServerManagerwriter end

                //#setManagerwriter begin

                exampleWriter->setManager(exampleManager);

                //#setManagerwriter end

/*
                //#getServerManagercontroller begin

                exampleManager = exampleController->getServerManager();

                //#getServerManagercontroller end

                //#setManagercontroller begin

                exampleController->setManager(exampleManager);

                //#setManagercontroller end
*/

                //#GetUpdatePiece begin

                int intraId = exampleManager->GetUpdatePiece();

                //#GetUpdatePiece end

                //#GetUpdateNumPieces begin

                int intraSize = exampleManager->GetUpdateNumPieces();

                //#getUpdateNumPieces end

                //#GetMpiComm begin

                MPI_Comm exampleManagerComm = exampleManager->GetMpiComm();

                //#GetMpiComm end

                //#SetMpiComm begin

                exampleManager->SetMpiComm(exampleManagerComm);

                //#SetMpiComm end

                //#GetLocalBufferSizeMBytes begin

                int exampleBufferSize = exampleManager->GetLocalBufferSizeMBytes();

                //#GetLocalBufferSizeMBytes end

                //#SetLocalBufferSizeMBytes begin

                exampleManager->SetLocalBufferSizeMBytes(exampleBufferSize);

                //#SetLocalBufferSizeMBytes end

                //#GetIsServer begin

                bool exampleIsServer = exampleManager->GetIsServer();

                //#GetIsServer end

                //#SetIsServer begin

                exampleManager->SetIsServer(exampleIsServer);

                //#SetIsServer end

                //#GetDsmType begin

                int exampleType = exampleManager->GetDsmType();

                //#GetDsmType end

                //#SetDsmType begin

                exampleManager->SetDsmType(XDMF_DSM_TYPE_UNIFORM);

                //#SetDsmType end

                //#GetBlockLength begin

                long exampleBlockLength = exampleManager->GetBlockLength();

                //#GetBlockLength end

                //#SetBlockLength begin

                exampleManager->SetBlockLength(exampleBlockLength);

                //#SetBlockLength end

                //#GetInterCommType begin

                int exampleCommType = exampleManager->GetInterCommType();

                //#GetInterCommType end

                //#SetInterCommType begin

                exampleManager->SetInterCommType(XDMF_DSM_COMM_MPI);

                //#SetInterCommType end

                //#GetIsConnected begin

                bool exampleManagerConnectionStatus = exampleManager->GetIsConnected();

                //#GetIsConnected end

        /*
                //#Create begin

                exampleManager->Create(size - numServerCores, size - 1);

                //#Create end

                //#Destroy begin

                exampleManager->Destroy();

                //#Destroy end
        */

                //#declarebuffer begin

                XdmfDSMBuffer * exampleBuffer;

                //#delcarebuffer end

                //#getServerBufferwriter begin

                exampleBuffer = exampleWriter->getServerBuffer();

                //#getServerBufferwriter end

                //#setBufferwriter begin

                exampleWriter->setBuffer(exampleBuffer);

                //#setBufferwriter end

/*
                //#getServerBuffercontroller begin

                exampleBuffer = exampleController->getServerBuffer();

                //#getServerBuffercontroller end

                //#setBuffercontroller begin

                exampleController->setBuffer(exampleBuffer);

                //#setBuffercontroller end
*/

                //#GetDsmBuffer begin

                exampleBuffer = exampleManager->GetDsmBuffer();

                //#GetDsmBuffer end

                //#SetDsmBuffer begin

                exampleManager->SetDsmBuffer(exampleBuffer);

                //#SetDsmBuffer end

                //#GetIsConnectedbuffer begin

                bool exampleIsConnected = exampleBuffer->GetIsConnected();

                //#GetIsConnectedbuffer end

                //#SetIsConnectedbuffer begin

                exampleBuffer->SetIsConnected(exampleIsConnected);

                //#SetIsConnectedbuffer end

                //#GetDataPointer begin

                char * exampleDataPointer = exampleBuffer->GetDataPointer();

                //#GetDataPointer end

                //#GetDSMTypebuffer begin

                int exampleDSMType = exampleBuffer->GetDsmType();

                //#GetDsmTypebuffer end

                //#SetDsmTypebuffer begin

                exampleBuffer->SetDsmType(XDMF_DSM_TYPE_UNIFORM);

                //#SetDsmTypebuffer end

                //#GetIsServerbuffer begin

                bool exampleBufferIsServer = exampleBuffer->GetIsServer();

                //#GetIsServerbuffer end

                //#SetIsServerbuffer begin

                exampleBuffer->SetIsServer(exampleIsServer);

                //#SetIsServerbuffer end

                //#GetStartAddress begin

                int exampleBufferStart = exampleBuffer->GetStartAddress();

                //#GetStartAddress end

                //#GetEndAddress begin

                int exampleBufferEnd = exampleBuffer->GetEndAddress();

                //#GetEndAddress end

                //#GetStartServerId begin

                int exampleServerStart = exampleBuffer->GetStartServerId();

                //#GetStartServerId end

                //#GetEndServerId begin

                int exampleServerEnd = exampleBuffer->GetEndServerId();

                //#GetEndServerId end

                for (int i = 0; i<size - numServersCores; ++i)
                {
                        if (i == id)
                        {
                                std::cout << "starting id = " << exampleServerStart << " and ending id = " << exampleServerEnd << " from core " << id << std::endl;
                        }
                }

                //#GetLength begin

                long exampleBufferLength = exampleBuffer->GetLength();

                //#GetLength end

                //#GetTotalLength begin

                long exampleTotalBufferLength = exampleBuffer->GetTotalLength();

                //#GetTotalLength end

                //#GetBlockLengthbuffer begin

                long exampleBufferBlockLength = exampleBuffer->GetBlockLength();

                //#GetBlockLengthbuffer end

                //#SetBlockLengthbuffer begin

                exampleBuffer->SetBlockLength(exampleBufferBlockLength);

                //#SetBlockLengthbuffer end

                /*
                //#ConfigureUniform begin

                exampleBuffer->ConfigureUniform(exampleBuffer->GetComm(), dsmSize/numServersCores, size - numServersCores, size - 1);

                //#ConfigureUniform end
                */

                //#CommandHeader begin

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

                //#CommandHeader end

                //#SendRecvData begin

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

                //#SendRecvData end

                //#SendRecvAcknowledgement begin

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

                //#SendRecvAcknowledgement end

                //#BroadcastComm begin

                int broadcastComm = XDMF_DSM_INTER_COMM;
                exampleBuffer->BroadcastComm(&broadcastComm, 0);

                //#BroadcastComm end

                //#BufferService begin

                if (id == 0)
                {
                        int returnCode;
                        int serviceOut = exampleBuffer->BufferService(&returnCode);
                }
                if (id == 1)
                {
                        exampleBuffer->SendCommandHeader(XDMF_DSM_OPCODE_DONE, 0, 0, 0, XDMF_DSM_INTER_COMM);
                }

                //#BufferService end

                //#BufferServiceLoop begin

                if (id == 0)
                {
                        int returnCode;
                        exampleBuffer->BufferServiceLoop(&returnCode);
                }
                if (id == 1)
                {
                        exampleBuffer->SendCommandHeader(XDMF_DSM_OPCODE_DONE, 0, 0, 0, XDMF_DSM_INTER_COMM);
                }

                //#BufferServiceloop end

                //#GetAddressRangeForId begin

                int core0StartAddress = 0;
                int core0EndAddress = 0;
                exampleBuffer->GetAddressRangeForId(0, &core0StartAddress, &core0EndAddress);

                //#GetAddressRangeForId end

                //#AddressToId begin

                int correspondingId = exampleBuffer->AddressToId(500);

                //#AddressToId end

                //#PutGet begin

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

                //#PutGet end

                //#GetComm begin

                XdmfDSMCommMPI * exampleDSMComm = exampleBuffer->GetComm();

                //#GetComm end 

                //#SetComm begin

                exampleBuffer->SetComm(exampleDSMComm);

                //#SetComm end

                //#GetId begin

                int exampleIntraID = exampleDSMComm->GetId();

                //#GetId end

                //#GetIntraSize begin

                int exampleIntraSize = exampleDSMComm->GetIntraSize();

                //#GetIntraSize end

                //#GetInterId begin

                int exampleInterId = exampleDSMComm->GetInterId();

                //#GetInterId end

                //#GetInterSize begin

                int exampleInterSize = exampleDSMComm->GetInterSize();

                //#GetInterSize end

                //#GetInterCommType begin

                int exampleInterCommType = exampleDSMComm->GetInterCommType();

                //#GetInterCommType end

                //#initcomm begin

                exampleDSMComm->Init();

                //#initcomm end

                //#GetIntraComm begin

                MPI_Comm exampleIntraComm = exampleDSMComm->GetIntraComm();

                //#GetIntraComm end

                //#DupComm begin

                exampleDSMComm->DupComm(workerComm);

                //#DupComm end

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
                        portString = exampleDSMComm->GetDsmPortName();
                        // Send the port string to the connecting group
                        exampleDSMComm->Accept();

                        // When done with connection;
                        exampleDSMComm->ClosePort();
                }
        
                if (connectingGroup)
                {
                        // Recieve string from Master group
                        exampleDSMComm->SetDsmPortName(portString);
                        exampleDSMComm->Connect();

                        // When done with connection
                        exampleDSMComm->Disconnect();
                }

                if (connectingGroup)
                {
                        // Recieve string from Master group
                        exampleDSMComm->SetDsmPortName(portString);
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
                        exampleWriter->getServerBuffer());

                readArray->insert(readController);

                if (id == 0)
                {
                        std::cout << "\n\n\n";
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

                //#endworksection begin

        }

        //#endworksection end

        exampleWriter->closeFile();

        //#stopDSMwriter begin

        if (id == 0)
        {
                exampleWriter->stopDSM();
        }

        //#stopDSMwriter end

        /*
        //#stopDSMcontroller begin

        if (id == 0)
        {
                exampleController->stopDSM();
        }

        //#stopDSMcontroller end
        */

        /*
        //#SendDone begin

        if (id == 0)
        {
                XdmfDSMBuffer closeBuffer = exampleWriter->getServerBuffer();
                closeBuffer->SendDone();
        }

        //#SendDone end
        */

        //#GetInterComm begin

        XdmfDSMCommMPI * exampleDSMComm = exampleWriter->getServerBuffer()->GetComm();
        MPI_Comm exampleInterComm = exampleDSMComm->GetInterComm();

        //#GetInterComm end

        //#DupInterComm begin

        exampleDSMComm->DupInterComm(comm);

        //#DupInterComm end

        //#SendRecvInfo begin

        if (id >= size - numServersCores)
        {
                exampleWriter->getServerBuffer()->SendInfo();
        }
        else
        {
                exampleWriter->getServerBuffer()->ReceiveInfo();
        }

        //#SendRecvInfo begin

        /*
        //#restartDSMwriter begin

        exampleWriter->restartDSM();

        //#restartDSMwriter end

        //#restartDSMcontroller begin

        exampleController->restartDSM();

        //#restartDSMcontroller end
        */

        MPI_Barrier(comm);

        //#finalizeMPI begin

        //the dsmManager must be deleted or else there will be a segfault
        exampleWriter->deleteManager();

        MPI_Finalize();

        //#finalizeMPI end

        return 0;
}
