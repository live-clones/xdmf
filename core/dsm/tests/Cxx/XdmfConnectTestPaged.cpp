#include <mpi.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <XdmfArray.hpp>
#include <XdmfArrayType.hpp>
#include <XdmfHDF5WriterDSM.hpp>
#include <XdmfHDF5ControllerDSM.hpp>
#include <XdmfDSMBuffer.hpp>
#include <XdmfDSMCommMPI.hpp>

int main(int argc, char *argv[])
{
// This test does not work properly with openmpi
// due to an issue with the openmpi code
#ifndef OPEN_MPI
        //#initMPI begin

        int size, id, dsmSize;
        dsmSize = 64;
        MPI_Status status;
        MPI_Comm comm = MPI_COMM_WORLD;

        MPI_Init(&argc, &argv);

        MPI_Comm_rank(comm, &id);
        MPI_Comm_size(comm, &size);

        XdmfDSMCommMPI::SetUseEnvFileName(true);

        std::string newPath = "dsm";
        std::string newSetPath = "Data";

        // Initializing objects

        shared_ptr<XdmfHDF5WriterDSM> exampleWriter = XdmfHDF5WriterDSM::New(newPath, comm);

        exampleWriter->getServerBuffer()->GetComm()->SetApplicationName("Connect 1");

/*
        XdmfDSMCommMPI * testComm = new XdmfDSMCommMPI();
        testComm->DupComm(comm);
        testComm->Init();
        XdmfDSMBuffer * testBuffer = new XdmfDSMBuffer();
        testBuffer->SetIsServer(false);
        testBuffer->SetComm(testComm);
        testBuffer->SetIsConnected(true);

        shared_ptr<XdmfHDF5WriterDSM> exampleWriter = XdmfHDF5WriterDSM::New(newPath, testBuffer);
*/

        //#initMPI end

        #ifdef  _WIN32
                Sleep(500)
        #else
                sleep(5);
        #endif

	char * configFileName = strdup(exampleWriter->getServerBuffer()->GetComm()->GetDsmFileName().c_str());

	std::ifstream testStream;
        testStream.open(configFileName);

	while (!testStream.good()) { // Wait for the config file to be generated
		testStream.close();
		#ifdef  _WIN32
			Sleep(500)
		#else
			sleep(5);
		#endif
		testStream.open(configFileName);
	}

        //#ReadDsmPortName begin

        exampleWriter->getServerBuffer()->GetComm()->ReadDsmPortName();

        //#ReadDsmPortName end

        //#GetDsmPortName begin

        char * portName = exampleWriter->getServerBuffer()->GetComm()->GetDsmPortName();

        //#GetDsmPortName end

        //#SetDsmPortName begin

        exampleWriter->getServerBuffer()->GetComm()->SetDsmPortName(portName);

        //#SetDsmPortName end

        //#Connect begin

        exampleWriter->getServerBuffer()->Connect();

        //#Connect end

        exampleWriter->getServerBuffer()->SetIsConnected(true);

        /*

        //#manualConnect begin

        try
        {
                status = exampleWriter->getServerBuffer()->GetComm()->Connect();
        }
        catch (XdmfError e)
        {
                // Connection failed
                std::cout << e.what() << std::endl;
                return 0;
        }
        if (status == MPI_SUCCESS)
        {
                exampleWriter->getServerBuffer()->SetIsConnected(true);
                try
                {
                        exampleWriter->getServerBuffer()->ReceiveInfo();
                }
                catch (XdmfError e)
                {
                        //ReceiveInfo failed
                        std::cout << e.what() << std::endl;
                        return 0;
                }
        }

        //#manualConnect end

        */

        MPI_Barrier(exampleWriter->getServerBuffer()->GetComm()->GetIntraComm());

        // Testing the structure of the DSM
        if (id == 0)
        {
          std::vector<unsigned int> structuresizes;
          structuresizes.push_back(1);
          structuresizes.push_back(1);
          structuresizes.push_back(2);
          structuresizes.push_back(2);
          std::vector<std::string> structurenames;
          structurenames.push_back("Application");
          structurenames.push_back("Server");
          structurenames.push_back("Connect 1");
          structurenames.push_back("Connect 2");
          std::vector<std::pair<std::string, unsigned int> > teststructure = exampleWriter->getServerBuffer()->GetComm()->GetDsmProcessStructure();
          printf("DSM Structure:\n");
          for (unsigned int i = 0; i < teststructure.size(); ++i)
          {
            std::cout << "(" << teststructure[i].first << ", " << teststructure[i].second << ")" << std::endl;
            assert(teststructure[i].second == structuresizes[i]);
            assert(teststructure[i].first.compare(structurenames[i]) == 0);
          }
        }
        
        MPI_Barrier(exampleWriter->getServerBuffer()->GetComm()->GetIntraComm());

        shared_ptr<XdmfArray> writeArray = XdmfArray::New();

        for (int i = 1; i <= 5; ++i)
        {
                writeArray->pushBack(i*(id+1));
        }

        std::vector<unsigned int> writeStartVector;
        std::vector<unsigned int> writeStrideVector;
        std::vector<unsigned int> writeCountVector;
        std::vector<unsigned int> writeDataSizeVector;

        writeStartVector.push_back(id*5);
        writeStrideVector.push_back(1);
        writeCountVector.push_back(5);
        writeDataSizeVector.push_back(5*size);

        shared_ptr<XdmfHDF5ControllerDSM> writeController = XdmfHDF5ControllerDSM::New(
                newPath,
                newSetPath,
                XdmfArrayType::Int32(),
                writeStartVector,
                writeStrideVector,
                writeCountVector,
                writeDataSizeVector,
                exampleWriter->getServerBuffer());

        std::vector<unsigned int> readStartVector;
        std::vector<unsigned int> readStrideVector;
        std::vector<unsigned int> readCountVector;
        std::vector<unsigned int> readDataSizeVector;

        readStartVector.push_back(5*id);
        readStrideVector.push_back(1);
        readCountVector.push_back(5);
        readDataSizeVector.push_back(5*size);

        shared_ptr<XdmfHDF5ControllerDSM> readController = XdmfHDF5ControllerDSM::New(
                newPath,
                newSetPath,
                XdmfArrayType::Int32(),
                readStartVector,
                readStrideVector,
                readCountVector,
                readDataSizeVector,
                exampleWriter->getServerBuffer());

        exampleWriter->setMode(XdmfHeavyDataWriter::Hyperslab);

        // Test Notify
        int notify = exampleWriter->waitOn(newPath, "notify");

        printf("notification value = %d\n", notify);

        assert(notify == 3);

        // Done initializing

        for (int i = 0; i < size; ++i)
        {
                MPI_Barrier(exampleWriter->getServerBuffer()->GetComm()->GetIntraComm());
                if (i == id)
                {
                        std::stringstream outputstream;
                        outputstream << "Array on core " << exampleWriter->getServerBuffer()->GetComm()->GetInterId() << " contains:" << std::endl;
                        for (unsigned int j = 0; j < writeArray->getSize(); ++j)
                        {
                                outputstream << "[" << j << "]" << writeArray->getValue<int>(j) << std::endl;
                        }
                        std::cout << outputstream.str();
                }
        }

        for (unsigned int numloops = 0; numloops < 4; ++numloops)
        {
                if (writeArray->getNumberHeavyDataControllers() > 0)
                {
                        writeArray->removeHeavyDataController(0);
                }
                writeArray->insert(writeController);
        

                if (id == size - 1)
                {
                        std::cout << std::endl << std::endl;
                }

                writeArray->accept(exampleWriter);

                if (id == size - 1)
                {
                        int sentData = 1;
                        exampleWriter->getServerBuffer()->SendAcknowledgment(exampleWriter->getServerBuffer()->GetComm()->GetInterId() + 1, sentData, XDMF_DSM_EXCHANGE_TAG, XDMF_DSM_INTER_COMM);
                        exampleWriter->getServerBuffer()->ReceiveAcknowledgment(exampleWriter->getServerBuffer()->GetComm()->GetInterId() + 1, sentData, XDMF_DSM_EXCHANGE_TAG, XDMF_DSM_INTER_COMM);
                }

                MPI_Barrier(exampleWriter->getServerBuffer()->GetComm()->GetIntraComm());

                writeArray->removeHeavyDataController(0);
                writeArray->insert(readController);
                writeArray->release();
                writeArray->read();

                for (int i = 0; i < size; ++i)
                {
                        MPI_Barrier(exampleWriter->getServerBuffer()->GetComm()->GetIntraComm());
                        if (i == id)
                        {
                                std::stringstream outputstream;
                                outputstream << "Array on core " << exampleWriter->getServerBuffer()->GetComm()->GetInterId() << " contains:" << std::endl;
                                for (unsigned int j = 0; j < writeArray->getSize(); ++j)
                                {
                                        int tempVal = writeArray->getValue<int>(j);
                                        tempVal = tempVal * 2;
                                        writeArray->insert(j, tempVal);
                                        outputstream << "[" << j << "]" << writeArray->getValue<int>(j) << std::endl;
                                }
                                std::cout << outputstream.str();
                        }
                }
        }

        if (id == size - 1)
        {
                int sentData = 1;
                exampleWriter->getServerBuffer()->SendAcknowledgment(exampleWriter->getServerBuffer()->GetComm()->GetInterId() + 1, sentData, XDMF_DSM_EXCHANGE_TAG, XDMF_DSM_INTER_COMM);
        }

        MPI_Barrier(exampleWriter->getServerBuffer()->GetComm()->GetIntraComm());

        MPI_Barrier(exampleWriter->getServerBuffer()->GetComm()->GetInterComm());

        //#Disconnectmanager begin

        exampleWriter->getServerBuffer()->Disconnect();

        //#Disconnectmanager end

        //#Disconnectcomm begin

        exampleWriter->getServerBuffer()->GetComm()->Disconnect();

        //#Disconnectcomm end

        //#finalizeMPI begin

        MPI_Finalize();

        //#finalizeMPI end

#endif

        return 0;
}
