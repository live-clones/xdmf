#include <mpi.h>
#include <H5FDdsm.h>
#include <H5FDdsmManager.h>
#include <iostream>
#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfHDF5WriterDSM.hpp"
#include "XdmfHDF5ControllerDSM.hpp"

int main(int argc, char *argv[])
{
        //#initMPI begin

        int size, id, dsmSize;
        dsmSize = 64;//The total size of the DSM being created
        MPI_Status status;
        MPI_Comm comm = MPI_COMM_WORLD;

        MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &providedThreading);

        MPI_Comm_rank(comm, &id);
        MPI_Comm_size(comm, &size);

        //#initMPI end

        std::vector<unsigned int> outputVector;

        shared_ptr<XdmfArray> testArray = XdmfArray::New();

        for (unsigned int i = 1; i <= 4; ++i)
        {
                testArray->pushBack(i*id);
        }

        //#writevectorinit begin

        std::string newPath = "dsm";
        std::string newSetPath = "data";

        std::vector<unsigned int> writeStartVector;
        writeStartVector.push_back(id*4);
        std::vector<unsigned int> writeStrideVector;
        writeStrideVector.push_back(1);
        std::vector<unsigned int> writeCountVector;
        writeCountVector.push_back(4);
        std::vector<unsigned int> writeDataSizeVector;
        writeDataSizeVector.push_back(4*size);

        //#writevectorinit end

        /*
        //#initwritergenerate begin

        shared_ptr<XdmfHDF5WriterDSM> exampleWriter = XdmfHDF5WriterDSM::New(newPath, comm, dsmSize/size);

        //#initwritergenerate end
        */

        //#initcontrollergenerate begin

        shared_ptr<XdmfHDF5ControllerDSM> writeController = XdmfHDF5ControllerDSM::New(
                newPath,
                newSetPath,
                XdmfArrayType::Int32(),
                writeStartVector,
                writeStrideVector,
                writeCountVector,
                writeDataSizeVector,
                comm,
                dsmSize/size);

        //#initcontrollergenerate end

        /*
        //#initcontrollerwithbuffer begin

        shared_ptr<XdmfHDF5ControllerDSM> writeController = XdmfHDF5ControllerDSM::New(
                newPath,
                newSetPath,
                XdmfArrayType::Int32(),
                writeStartVector,
                writeStrideVector,
                writeCountVector,
                writeDataSizeVector,
                exampleWriter->getBuffer());

        //#initcontrollerwithbuffer end

        //#setManagercontroller begin

        writeController->setManager(exampleWriter->getManager());

        //#setManagercontroller end

        //#setBuffercontroller begin

        writeController->setBuffer(exampleWriter->getBuffer());
        //In this context setting the buffer is redundant
        //However, if multiple buffers exist, this can be used to change between them

        //#setBuffercontroller end
        */

        //#initwriterwithbuffer begin

        shared_ptr<XdmfHDF5WriterDSM> exampleWriter = XdmfHDF5WriterDSM::New(newPath, writeController->getBuffer());

        //#initwriterwithbuffer end

        //#setManagerwriter begin

        exampleWriter->setManager(writeController->getManager());

        //#setManagerwriter end

        //#setBufferwriter begin

        exampleWriter->setBuffer(writeController->getBuffer());
        //In this context setting the buffer is redundant
        //However, if multiple buffers exist, this can be used to change between them

        //#setBufferwriter end

        exampleWriter->setMode(XdmfHeavyDataWriter::Hyperslab);

        testArray->insert(writeController);

        for (unsigned int i = 0; i<size; ++i)
        {
                MPI_Barrier(comm);
                if (i == id)
                {
                        std::cout << "Core # " << id << std::endl;
                        std::cout << "Controller stats" << std::endl;
                        std::cout << "datasetpath = " << testArray->getHeavyDataController(0)->getDataSetPath() << std::endl;
                        std::cout << "filepath = " << testArray->getHeavyDataController(0)->getFilePath() << std::endl;
                        outputVector = testArray->getHeavyDataController(0)->getDataspaceDimensions();
                        std::cout << "Data space dimensions" << std::endl;
                        for (int j=0; j<outputVector.size(); ++j)
                        {
                                std::cout << "[" << j << "] =" << outputVector[j] << std::endl;
                        }
                        std::cout << "Controller Dimensions" << std::endl;
                        outputVector = testArray->getHeavyDataController(0)->getDimensions();
                        for (int j=0; j<outputVector.size(); ++j)
                        {
                                std::cout << "[" << j << "] =" << outputVector[j] << std::endl;
                        }
                        std::cout << "Controller size" << testArray->getHeavyDataController(0)->getSize() << std::endl;
                        std::cout << "Controller starts" << std::endl;
                        outputVector = testArray->getHeavyDataController(0)->getStart();
                        for (int j=0; j<outputVector.size(); ++j)
                        {
                                std::cout << "[" << j << "] =" << outputVector[j] << std::endl;
                        }
                        std::cout << "Controller strides" << std::endl;
                        outputVector = testArray->getHeavyDataController(0)->getStride();
                        for (int j=0; j<outputVector.size(); ++j)
                        {
                                std::cout << "[" << j << "] =" << outputVector[j] << "\n" << std::endl;
                        }
                        for(unsigned int i=0; i<testArray->getSize(); ++i)
                        {
                                std::cout << "core #" << id <<" testArray[" << i << "] = " << testArray->getValue<int>(i) << std::endl;
                        }
                }
        }

        testArray->accept(exampleWriter);

        std::vector<unsigned int> readStartVector;
        readStartVector.push_back(4*(size - id - 1));
        std::vector<unsigned int> readStrideVector;
        readStrideVector.push_back(1);
        std::vector<unsigned int> readCountVector;
        readCountVector.push_back(4);
        std::vector<unsigned int> readDataSizeVector;
        readDataSizeVector.push_back(4*size);

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
                exampleWriter->getBuffer());

        readArray->insert(readController);

        std::cout << "testing read" << std::endl;
        readArray->read();
        std::cout << "done testing read" << std::endl;
        

        for (unsigned int i = 0; i<size; ++i)
        {
                MPI_Barrier(comm);
                if (i == id)
                {
                        std::cout << "Core # " << id << std::endl;
                        std::cout << "Controller stats" << std::endl;
                        std::cout << "datasetpath = " << readArray->getHeavyDataController(0)->getDataSetPath() << std::endl;
                        std::cout << "filepath = " << readArray->getHeavyDataController(0)->getFilePath() << std::endl;
                        outputVector = readArray->getHeavyDataController(0)->getDataspaceDimensions();
                        std::cout << "Data space dimensions" << std::endl;
                        for (int j=0; j<outputVector.size(); ++j)
                        {
                                std::cout << "[" << j << "] =" << outputVector[j] << std::endl;
                        }
                        std::cout << "Controller Dimensions" << std::endl;
                        outputVector = readArray->getHeavyDataController(0)->getDimensions();
                        for (int j=0; j<outputVector.size(); ++j)
                        {
                                std::cout << "[" << j << "] =" << outputVector[j] << std::endl;
                        }
                        std::cout << "Controller size" << readArray->getHeavyDataController(0)->getSize() << std::endl;
                        std::cout << "Controller starts" << std::endl;
                        outputVector = readArray->getHeavyDataController(0)->getStart();
                        for (int j=0; j<outputVector.size(); ++j)
                        {
                                std::cout << "[" << j << "] =" << outputVector[j] << std::endl;
                        }
                        std::cout << "Controller strides" << std::endl;
                        outputVector = readArray->getHeavyDataController(0)->getStride();
                        for (int j=0; j<outputVector.size(); ++j)
                        {
                                std::cout << "[" << j << "] =" << outputVector[j] << "\n" << std::endl;
                        }
                        for(unsigned int i=0; i<readArray->getSize(); ++i)
                        {
                                std::cout << "core #" << id <<" readArray[" << i << "] = " << readArray->getValue<int>(i) << std::endl;
                        }
                }
        }


        MPI_Barrier(comm);

        /*
        //#managerdeletionwriter begin

        //the dsmManager must be deleted or else there will be a segfault

        delete exampleWriter->getManager();

        //#managerdeletionwriter end

        //#deleteManagerwriter begin

        //the dsmManager must be deleted or else there will be a segfault

        examplewriter->deleteManager();

        //#deleteManagerwriter end
        */

        //#managerdeletioncontroller begin

        //the dsmManager must be deleted or else there will be a segfault

        delete writeController->getManager();

        //#managerdeletioncontroller end

        /*don't call delete twice on the same manager or else an error will occur
        //#deleteManagercontroller begin

        //the dsmManager must be deleted or else there will be a segfault

        writeController->deleteManager();

        //#deleteManagercontroller end
        */

        MPI_Finalize();

        return 0;
}
