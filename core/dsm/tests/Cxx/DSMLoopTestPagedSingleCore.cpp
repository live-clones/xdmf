#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfHDF5WriterDSM.hpp"
#include "XdmfHDF5ControllerDSM.hpp"

int main(int argc, char *argv[])
{
        int size, id, dsmSize;
        dsmSize = 64;//The total size of the DSM being created
        MPI_Comm comm = MPI_COMM_WORLD;

        MPI_Init(&argc, &argv);

        MPI_Comm_rank(comm, &id);
        MPI_Comm_size(comm, &size);


        std::vector<unsigned int> outputVector;

        shared_ptr<XdmfArray> testArray = XdmfArray::New();
        testArray->initialize<int>(0);

        shared_ptr<XdmfArray> testArray2 = XdmfArray::New();
        testArray2->initialize<int>(0);

        std::string newPath = "dsm";
        std::stringstream setPathAggregate;
        setPathAggregate << "dataspace" << id;
        std::string newSetPath = setPathAggregate.str();
        std::string secondSetPath = "data2";

        // Change this to determine the number of cores used as servers
        unsigned int numServersCores = 2;
        // Change this to determine the size of the arrays generated when initializing
        unsigned int writeArraySize = 4;

        std::vector<unsigned int> writeStartVector;
        std::vector<unsigned int> writeStrideVector;
        std::vector<unsigned int> writeCountVector;
        std::vector<unsigned int> writeDataSizeVector;

        std::vector<unsigned int> readStartVector;
        std::vector<unsigned int> readStrideVector;
        std::vector<unsigned int> readCountVector;
        std::vector<unsigned int> readDataSizeVector;

        std::vector<unsigned int> readOutputCountVector;

        shared_ptr<XdmfArray> readArray = XdmfArray::New();
        readArray->initialize<int>(0);

        shared_ptr<XdmfArray> readArray2 = XdmfArray::New();
        readArray2->initialize<int>(0);

        shared_ptr<XdmfHDF5ControllerDSM> readController;
        shared_ptr<XdmfHDF5ControllerDSM> readOutputController;
        shared_ptr<XdmfHDF5ControllerDSM> writeController;

        shared_ptr<XdmfHDF5ControllerDSM> readController2;
        shared_ptr<XdmfHDF5ControllerDSM> readOutputController2;
        shared_ptr<XdmfHDF5ControllerDSM> writeController2;

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

        shared_ptr<XdmfHDF5WriterDSM> exampleWriter = XdmfHDF5WriterDSM::New(newPath, comm, dsmSize/numServersCores, 1024, 1, size-numServersCores, size-1);

        exampleWriter->setMode(XdmfHeavyDataWriter::Hyperslab);

        //split out sub-comm for the worker cores
        //server cores will not progress to this point until after the servers are done running

        if (id < size - (int)numServersCores)
        {
                // Split the comm even further

                // This Comm is for the individual cores
                MPI_Comm writeComm;

                MPI_Group readingCores, writingCores;

                MPI_Comm_group(workerComm, &workers);
                int * ServerIds = (int *)calloc(1, sizeof(int));
                ServerIds[0] = id;

                MPI_Group_incl(workers, 1, ServerIds, &writingCores);
                testval = MPI_Comm_create(workerComm, writingCores, &writeComm);
                cfree(ServerIds);

                std::cout << "initializing" << std::endl;

                writeStartVector.push_back(0);
                writeStrideVector.push_back(1);
                writeCountVector.push_back(1);
                writeDataSizeVector.push_back(1);

                writeController = XdmfHDF5ControllerDSM::New(
                  newPath,
                  newSetPath,
                  XdmfArrayType::Int32(),
                  writeStartVector,
                  writeStrideVector,
                  writeCountVector,
                  writeDataSizeVector,
                  exampleWriter->getServerBuffer());

                testArray->pushBack(id);

                testArray->insert(writeController);

                exampleWriter->setWorkerComm(writeComm);

                printf("writing to dsm\n");

                testArray->accept(exampleWriter);

                printf("done writing to dsm\n");

                MPI_Barrier(workerComm);

                exampleWriter->setWorkerComm(workerComm);

                printf("reading size\n");

                int readsize = exampleWriter->getDataSetSize(newPath, newSetPath);
                printf("size of dataset %s:%s = %d\n", newPath.c_str(), newSetPath.c_str(), readsize);
                assert(readsize == 1);

//                exampleWriter->setWorkerComm(workerComm);
        }

        if (id == 0)
        {
                exampleWriter->stopDSM();
        }

        MPI_Barrier(comm);

        printf("finalizing\n");

        MPI_Finalize();

        return 0;
}
