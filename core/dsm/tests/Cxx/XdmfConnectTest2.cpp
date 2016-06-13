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

/*
        //since the start and end ids are larger than the size there are no buffers alloted
        //thus, no blockage occurs
        XdmfDSMCommMPI * testComm = new XdmfDSMCommMPI();
        testComm->DupComm(comm);
        testComm->Init();
        testComm->SetApplicationName("Connect 2");
        XdmfDSMBuffer * testBuffer = new XdmfDSMBuffer();
        testBuffer->SetIsServer(false);
        testBuffer->SetComm(testComm);
        testBuffer->SetIsConnected(true);
*/

        shared_ptr<XdmfHDF5WriterDSM> exampleWriter = XdmfHDF5WriterDSM::New(newPath, comm, "Connect 2");

        MPI_Comm_rank(exampleWriter->getServerBuffer()->GetComm()->GetIntraComm(), &id);
        MPI_Comm_size(exampleWriter->getServerBuffer()->GetComm()->GetIntraComm(), &size);

        std::vector<unsigned int> readStartVector;
        std::vector<unsigned int> readStrideVector;
        std::vector<unsigned int> readCountVector;
        std::vector<unsigned int> readDataSizeVector;

        readStartVector.push_back(5*id);
        readStrideVector.push_back(1);
        readCountVector.push_back(5);
        readDataSizeVector.push_back(5*size);

        shared_ptr<XdmfArray> readArray = XdmfArray::New();

        readArray->initialize<int>(0);
        readArray->reserve(5);

        shared_ptr<XdmfHDF5ControllerDSM> readController = XdmfHDF5ControllerDSM::New(
                newPath,
                newSetPath,
                XdmfArrayType::Int32(),
                readStartVector,
                readStrideVector,
                readCountVector,
                readDataSizeVector,
                exampleWriter->getServerBuffer());

        #ifdef  _WIN32
                Sleep(1000)
        #else
                sleep(10);
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

        readController->getServerBuffer()->GetComm()->ReadDsmPortName();

        readController->getServerBuffer()->Connect();

//        shared_ptr<XdmfHDF5WriterDSM> exampleWriter = XdmfHDF5WriterDSM::New(newPath, testBuffer);

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

        exampleWriter->setMode(XdmfHeavyDataWriter::Hyperslab);

        // Done initialization

        MPI_Barrier(readController->getServerBuffer()->GetComm()->GetIntraComm());

        // Let waits be set up
        #ifdef  _WIN32
                Sleep(100)
        #else
                sleep(1);
        #endif

        // Test Notify
        exampleWriter->waitRelease(newPath, "notify", 3);

        // Testing the structure of the DSM
        if (id == 0)
        {
          std::vector<unsigned int> structuresizes;
          structuresizes.push_back(1);
          structuresizes.push_back(1);
          structuresizes.push_back(2);
          structuresizes.push_back(2);
          std::vector<std::string> structurenames;
          structurenames.push_back("Accept");
          structurenames.push_back("Server");
          structurenames.push_back("Connect 1");
          structurenames.push_back("Connect 2");
          std::vector<std::pair<std::string, unsigned int> > teststructure = exampleWriter->getServerBuffer()->GetComm()->GetDsmProcessStructure();
          printf("DSM Structure:\n");
          for (unsigned int i = 0; i < teststructure.size(); ++i)
          {
            std::cout << "(" << teststructure[i].first << ", " << teststructure[i].second << ")" << "=="
            << "(" << structurenames[i] << ", " << structuresizes[i] << ")" << std::endl;
            assert(teststructure[i].second == structuresizes[i]);
            assert(teststructure[i].first.compare(structurenames[i]) == 0);
          }
        }

        MPI_Barrier(exampleWriter->getServerBuffer()->GetComm()->GetIntraComm());

        for (unsigned int numloops = 0; numloops < 4; ++numloops)
        {
                if (id == 0)
                {
                        int receiveData = 0;
                        readController->getServerBuffer()->ReceiveAcknowledgment(readController->getServerBuffer()->GetComm()->GetInterId() - 1, receiveData, XDMF_DSM_EXCHANGE_TAG, XDMF_DSM_INTER_COMM);
                }

                MPI_Barrier(readController->getServerBuffer()->GetComm()->GetIntraComm());

                if (readArray->getNumberHeavyDataControllers() > 0)
                {
                        readArray->removeHeavyDataController(0);
                }
                readArray->insert(readController);
                readArray->read();

                for (int i = 0; i < size; ++i)
                {
                        MPI_Barrier(readController->getServerBuffer()->GetComm()->GetIntraComm());
                        if (i == id)
                        {
                                std::stringstream outputstream;
                                outputstream << "Array on core " << exampleWriter->getServerBuffer()->GetComm()->GetInterId() << " contains:" << std::endl;
                                for (unsigned int j = 0; j < readArray->getSize(); ++j)
                                {
                                        int tempVal = readArray->getValue<int>(j);
                                        tempVal = tempVal * 3;
                                        readArray->insert(j, tempVal);
                                        outputstream << "[" << j << "]" << readArray->getValue<int>(j) << std::endl;
                                }
                                std::cout << outputstream.str();
                        }
                }

                MPI_Barrier(readController->getServerBuffer()->GetComm()->GetIntraComm());
                if (id == 0)
                {
                        std::cout << std::endl << std::endl;
                }

                readArray->removeHeavyDataController(0);
                readArray->insert(writeController);

                readArray->accept(exampleWriter);

                if (id == 0)
                {
                        int receiveData = 0;
                        readController->getServerBuffer()->SendAcknowledgment(readController->getServerBuffer()->GetComm()->GetInterId() - 1, receiveData, XDMF_DSM_EXCHANGE_TAG, XDMF_DSM_INTER_COMM);
                }
        }

        //this last acknowledgment is to end the loop.

        if (id == 0)
        {
                int receiveData = 0;
                readController->getServerBuffer()->ReceiveAcknowledgment(readController->getServerBuffer()->GetComm()->GetInterId() - 1, receiveData, XDMF_DSM_EXCHANGE_TAG, XDMF_DSM_INTER_COMM);
        }

        MPI_Barrier(readController->getServerBuffer()->GetComm()->GetIntraComm());

        // Do work stuff here

        if (id == 0)
        {
                readController->stopDSM();
        }

        MPI_Barrier(readController->getServerBuffer()->GetComm()->GetInterComm());

        MPI_Finalize();

#endif

        return 0;
}
