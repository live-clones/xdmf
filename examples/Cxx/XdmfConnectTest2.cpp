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
        //#initDSMWriterConnectRequired begin

        int size, id, dsmSize;
        dsmSize = 64;
        MPI_Status status;
        MPI_Comm comm = MPI_COMM_WORLD;

        MPI_Init(&argc, &argv);

        MPI_Comm_rank(comm, &id);
        MPI_Comm_size(comm, &size);


        std::string newPath = "dsm";
        std::string newSetPath = "Data";

        // Initializing objects

        shared_ptr<XdmfHDF5WriterDSM> exampleWriter = XdmfHDF5WriterDSM::New(newPath, comm);

        exampleWriter->getServerBuffer()->GetComm()->ReadDsmPortName();

        exampleWriter->getServerBuffer()->Connect();

        //#initDSMWriterConnectRequired end

        //#notify begin

        int notify = 0;

        if (size > 1) {
          if (id == 0) {
            notify = exampleWriter->waitOn(newPath, "notify");
          }
          else if (id == size - 1) {
            // The user will want to ensure that the waiton command is called first
            exampleWriter->waitRelease(newPath, "notify", 3);
          }
        }

        //#notify end

        //#buffernotify begin

        int notify = 0;

        if (size > 1) {
          if (id == 0) {
            notify = exampleWriter->getServerBuffer()->WaitOn(newPath, "notify");
          }
          else if (id == size - 1) {
            // The user will want to ensure that the waiton command is called first
            exampleWriter->getServerBuffer()->WaitRelease(newPath, "notify", 3);
          }
        }

        //#buffernotify end

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

        return 0;
}
