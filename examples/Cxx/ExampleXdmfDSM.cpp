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

        int size, id, providedThreading, dsmSize;
        dsmSize = 64;//The total size of the DSM being created
        MPI_Status status;
        MPI_Comm comm = MPI_COMM_WORLD;

        MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &providedThreading);

        MPI_Comm_rank(comm, &id);
        MPI_Comm_size(comm, &size);

        //#initMPI end

        if (id == 0)
        {
                if (providedThreading != MPI_THREAD_MULTIPLE)
                {
                        std::cout << "# MPI_THREAD_MULTIPLE not set, you may need to recompile your "
                                << "MPI distribution with threads enabled" << std::endl;
                }
                else
                {
                        std::cout << "# MPI_THREAD_MULTIPLE is OK" << std::endl;
                }
        }

        //#generateBuffer begin

        // Create DSM
        H5FDdsmManager * dsmManager = new H5FDdsmManager();
        dsmManager->SetMpiComm(comm);
        dsmManager->SetLocalBufferSizeMBytes(dsmSize / size);
        dsmManager->SetIsStandAlone(H5FD_DSM_TRUE);
        dsmManager->Create();

        H5FD_dsm_set_manager(dsmManager);

        H5FD_dsm_set_options(H5FD_DSM_LOCK_ASYNCHRONOUS);

        //Generate DSM buffer
        H5FDdsmBuffer * dsmBuffer = dsmManager->GetDsmBuffer();

        //#generateBuffer end

        // Get info from remote server
        double remoteMB = dsmBuffer->GetTotalLength() / (1024.0 * 1024.0);
        double numServers = dsmBuffer->GetEndServerId() + 1;
        if (id == 0)
        {
                std::cout << "DSM server memory size is : "
                        << (int)remoteMB << " MB" << std::endl;
                std::cout << "DSM server process count  : "
                        << (int)numServers << std::endl;
                std::cout << "Memory allocated per core : " << (int)(dsmSize / size) << "MB" << std::endl;
        }

        std::cout << "setting up DSM writer" << std::endl;
        // Create DSM Writer and write to DSM space.

        //#initializewriterfrombuffer begin

        // Virtual file path
        std::string newPath = "dsm";
        shared_ptr<XdmfHDF5WriterDSM> writer = XdmfHDF5WriterDSM::New(newPath, dsmBuffer);
        writer->setMode(XdmfHeavyDataWriter::Hyperslab);

        //#initializewriterfrombuffer end

        //#createwritecontrollervectors begin

        // Virtual set path
        std::string newSetPath = "data";

        // Holds the starting index for each dimension
        std::vector<unsigned int> writeStartVector;
        // Holds the distance between written values for each dimension
        std::vector<unsigned int> writeStrideVector;
        // Holds the total number of values for each dimension
        std::vector<unsigned int> writeCountVector;
        //holds the maximum DSM size for each dimension
        std::vector<unsigned int> writeDataSizeVector;

        shared_ptr<XdmfHDF5ControllerDSM> readController;

        //#createwritecontrollervectors end

        std::vector<unsigned int> outputVector;

        /*
        //writes must happen from all cores
        //write empty arrays from cores other than main
        shared_ptr<XdmfArray> array = XdmfArray::New();
        array->initialize<int>(0);//this is required to write
        if (id == 0)
        {
                for (unsigned int i = 0; i < 4*size; ++i)
                {
                        array->pushBack(i);
                }


                writeStartVector.push_back(0);
                writeCountVector.push_back(4*size);
                writeDataSizeVector.push_back(4*size);
                writeStrideVector.push_back(1);

                readController = XdmfHDF5ControllerDSM::New(
                        newPath,
                        newSetPath,
                        XdmfArrayType::Int32(),
                        writeStartVector,
                        writeStrideVector,
                        writeCountVector,
                        writeDataSizeVector,
                        dsmBuffer);
                array->insert(readController);

                for (unsigned int i = 0; i<size -1; ++i)
                {
                        MPI_Barrier(comm);
                }

                std::cout << "Core # " << id << std::endl;
                std::cout << "Controller stats" << std::endl;
                std::cout << "datasetpath = " << readController->getDataSetPath() << std::endl;
                std::cout << "filepath = " << readController->getFilePath() << std::endl;
                outputVector = readController->getDataspaceDimensions();
                std::cout << "Data space dimensions" << std::endl;
                for (int j=0; j<outputVector.size(); ++j)
                {
                        std::cout << "[" << j << "] =" << outputVector[j] << std::endl;
                }
                std::cout << "Controller Dimensions" << std::endl;
                outputVector = readController->getDimensions();
                for (int j=0; j<outputVector.size(); ++j)
                {
                        std::cout << "[" << j << "] =" << outputVector[j] << std::endl;
                }
                std::cout << "Controller size" << readController->getSize() << std::endl;
                std::cout << "Controller starts" << std::endl;
                outputVector = readController->getStart();
                for (int j=0; j<outputVector.size(); ++j)
                {
                        std::cout << "[" << j << "] =" << outputVector[j] << std::endl;
                }
                std::cout << "Controller strides" << std::endl;
                outputVector = readController->getStride();
                for (int j=0; j<outputVector.size(); ++j)
                {
                        std::cout << "[" << j << "] =" << outputVector[j] << "\n" << std::endl;
                }

                std::cout << "DSM writer set up" << std::endl;
                array->accept(writer);//if this hangs it's because writes need to be called on all cores
                std::cout << "DSM writer used" << id << std::endl;
        }
        else
        {
                writeStartVector.push_back(0);
                writeCountVector.push_back(0);
                writeDataSizeVector.push_back(4*size);
                writeStrideVector.push_back(1);

                readController = XdmfHDF5ControllerDSM::New(
                        newPath,
                        newSetPath,
                        XdmfArrayType::Int32(),
                        writeStartVector,
                        writeStrideVector,
                        writeCountVector,
                        writeDataSizeVector,
                        dsmBuffer);
                array->insert(readController);

                for (unsigned int i =1; i<size; ++i)
                {
                         if (id == i)
                        {
                                std::cout << "Core # " << id << std::endl;
                                std::cout << "Controller stats" << std::endl;
                                std::cout << "datasetpath = " << readController->getDataSetPath() << std::endl;
                                std::cout << "filepath = " << readController->getFilePath() << std::endl;
                                outputVector = readController->getDataspaceDimensions();
                                std::cout << "Data space dimensions" << std::endl;
                                for (int j=0; j<outputVector.size(); ++j)
                                {
                                        std::cout << "[" << j << "] =" << outputVector[j] << std::endl;
                                }
                                std::cout << "Controller Dimensions" << std::endl;
                                outputVector = readController->getDimensions();
                                for (int j=0; j<outputVector.size(); ++j)
                                {
                                        std::cout << "[" << j << "] =" << outputVector[j] << std::endl;
                                }
                                std::cout << "Controller size" << readController->getSize() << std::endl;
                                std::cout << "Controller starts" << std::endl;
                                outputVector = readController->getStart();
                                for (int j=0; j<outputVector.size(); ++j)
                                {
                                        std::cout << "[" << j << "] =" << outputVector[j] << std::endl;
                                }
                                std::cout << "Controller strides" << std::endl;
                                outputVector = readController->getStride();
                                for (int j=0; j<outputVector.size(); ++j)
                                {
                                        std::cout << "[" << j << "] =" << outputVector[j] << "\n" << std::endl;
                                }
                        }

                        MPI_Barrier(comm);
                }

                std::cout << "DSM writer set up" << std::endl;
                array->accept(writer);//if this hangs it's because writes need to be called on all cores
                std::cout << "DSM writer used" << id << std::endl;
        }*/

        
        //multicore write with arrays of varying sizes
        // Create Array
        // Array should be distributed among processes
        shared_ptr<XdmfArray> array = XdmfArray::New();
        array->initialize<int>(0);
        for(unsigned int i = 0; i <= id; ++i)
        {
                array->pushBack(id);
        }

        //#initializereadcontroller begin

        int startindex = 0;
        for (unsigned int i = 0; i <= id; ++i)
        {
                startindex += i;
        }

        int datacount = 0;
        for (unsigned int i = 0; i < size; ++i)
        {
                datacount = datacount + i + 1;
        }

        writeStartVector.push_back(startindex);
        writeStrideVector.push_back(1);
        writeCountVector.push_back(array->getSize());
        writeDataSizeVector.push_back(datacount);

        readController = XdmfHDF5ControllerDSM::New(
                newPath,
                newSetPath,
                XdmfArrayType::Int32(),
                writeStartVector,
                writeStrideVector,
                writeCountVector,
                writeDataSizeVector,
                dsmBuffer);

        //#initializereadController end

        array->insert(readController);
        for (unsigned int i = 0; i < size; ++i)
        {
                MPI_Barrier(comm);
                if (id == i)
                {
                        std::cout << "Core # " << id << std::endl;
                        std::cout << "Controller stats" << std::endl;
                        std::cout << "datasetpath = " << readController->getDataSetPath() << std::endl;
                        std::cout << "filepath = " << readController->getFilePath() << std::endl;
                        outputVector = readController->getDataspaceDimensions();
                        std::cout << "Data space dimensions" << std::endl;
                        for (int j=0; j<outputVector.size(); ++j)
                        {
                                std::cout << "[" << j << "] =" << outputVector[j] << std::endl;
                        }
                        std::cout << "Controller Dimensions" << std::endl;
                        outputVector = readController->getDimensions();
                        for (int j=0; j<outputVector.size(); ++j)
                        {
                                std::cout << "[" << j << "] =" << outputVector[j] << std::endl;
                        }
                        std::cout << "Controller size" << readController->getSize() << std::endl;
                        std::cout << "Controller starts" << std::endl;
                        outputVector = readController->getStart();
                        for (int j=0; j<outputVector.size(); ++j)
                        {
                                std::cout << "[" << j << "] =" << outputVector[j] << std::endl;
                        }
                        std::cout << "Controller strides" << std::endl;
                        outputVector = readController->getStride();
                        for (int j=0; j<outputVector.size(); ++j)
                        {
                                std::cout << "[" << j << "] =" << outputVector[j] << "\n" << std::endl;
                        }
                }
        }


        std::cout << "DSM writer set up" << std::endl;
        MPI_Barrier(comm);
        array->accept(writer);
        std::cout << "DSM writer used" << id << std::endl;
        


        /*
        //for writing from multiple cores
        // Create Array
        // Array should be distributed among processes
        shared_ptr<XdmfArray> array = XdmfArray::New();
        array->initialize<int>(0);
        for (unsigned int i = 1; i<=4; ++i)
        {
                array->pushBack(id*i);
        }

        std::cout << "from core #" << id << " generating these values: " << id << " " << id*2 << " " << id*3 << " " << id*4 << std::endl;

        writeStartVector.push_back(array->getSize()*id);
        writeCountVector.push_back(array->getSize());
        writeDataSizeVector.push_back(array->getSize()*size);
        writeStrideVector.push_back(1);

        readController = XdmfHDF5ControllerDSM::New(
                newPath,
                newSetPath,
                XdmfArrayType::Int32(),
                writeStartVector,
                writeStrideVector,
                writeCountVector,
                writeDataSizeVector,
                dsmBuffer);
        array->insert(readController);
        for (unsigned int i = 0; i < size; ++i)
        {
                MPI_Barrier(comm);
                if (id == i)
                {
                        std::cout << "Core # " << id << std::endl;
                        std::cout << "Controller stats" << std::endl;
                        std::cout << "datasetpath = " << readController->getDataSetPath() << std::endl;
                        std::cout << "filepath = " << readController->getFilePath() << std::endl;
                        outputVector = readController->getDataspaceDimensions();
                        std::cout << "Data space dimensions" << std::endl;
                        for (int j=0; j<outputVector.size(); ++j)
                        {
                                std::cout << "[" << j << "] =" << outputVector[j] << std::endl;
                        }
                        std::cout << "Controller Dimensions" << std::endl;
                        outputVector = readController->getDimensions();
                        for (int j=0; j<outputVector.size(); ++j)
                        {
                                std::cout << "[" << j << "] =" << outputVector[j] << std::endl;
                        }
                        std::cout << "Controller size" << readController->getSize() << std::endl;
                        std::cout << "Controller starts" << std::endl;
                        outputVector = readController->getStart();
                        for (int j=0; j<outputVector.size(); ++j)
                        {
                                std::cout << "[" << j << "] =" << outputVector[j] << std::endl;
                        }
                        std::cout << "Controller strides" << std::endl;
                        outputVector = readController->getStride();
                        for (int j=0; j<outputVector.size(); ++j)
                        {
                                std::cout << "[" << j << "] =" << outputVector[j] << "\n" << std::endl;
                        }
                }
        }


        std::cout << "DSM writer set up" << std::endl;
        MPI_Barrier(comm);
        array->accept(writer);
        std::cout << "DSM writer used" << id << std::endl;*/

        //ensure all writes are called
        MPI_Barrier(comm);
        std::cout << "core #" << id << "finished waiting" << std::endl;

        //#createreadcontrollervectors begin

        //holds the starting index for each dimension
        std::vector<unsigned int> startVector;
        //holds the distance between read values for each dimension
        std::vector<unsigned int> strideVector;
        //holds the total number of values for each dimension
        std::vector<unsigned int> countVector;
        //holds the maximum DSM size for each dimension
        std::vector<unsigned int> datasizeVector;

        //#createreadcontrollervectors end

        /*
        //single core read must be called across all cores
        if (id == 0)
        {
                std::cout << "getting output" << std::endl;
                // Read data
                startVector.push_back(0);
                strideVector.push_back(1);
                countVector.push_back(4*size);
                datasizeVector.push_back(4*size);
                std::cout << "starting index" << startVector[0] << std::endl;
                std::cout << "stride = " << strideVector[0] << std::endl;
                std::cout << "number of values = " << countVector[0] << std::endl;
                std::cout << "Size of block = " << datasizeVector[0] << std::endl;
                readController = XdmfHDF5ControllerDSM::New(
                        array->getHeavyDataController(0)->getFilePath(),
                        array->getHeavyDataController(0)->getDataSetPath(),
                        array->getHeavyDataController(0)->getType(),
                        startVector,
                        strideVector,
                        countVector,
                        datasizeVector,
                        dsmBuffer);
                std::cout << "done making reader" << std::endl;
                shared_ptr<XdmfArray> readArray = XdmfArray::New();
                readArray->initialize<int>(0);
                readArray->insert(readController);

                std::cout << "reader set" << std::endl;
                readArray->read();//if it hangs here, read requires all cores to read at the same time
                std::cout << "done reading" << std::endl;
                std::cout << "printing output" << std::endl;

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
        else
        {
                std::cout << "getting output" << std::endl;
                // Read data
                startVector.push_back(0);
                std::cout << "starting index" << startVector[0] << std::endl;
                strideVector.push_back(1);
                std::cout << "stride = " << strideVector[0] << std::endl;
                countVector.push_back(0);
                std::cout << "number of values = " << countVector[0] << std::endl;
                datasizeVector.push_back(4*size);
                std::cout << "Size of block = " << datasizeVector[0] << std::endl;
                readController = XdmfHDF5ControllerDSM::New(
                array->getHeavyDataController(0)->getFilePath(),
                array->getHeavyDataController(0)->getDataSetPath(),
                array->getHeavyDataController(0)->getType(),
                startVector,
                strideVector,
                countVector,
                datasizeVector,
                dsmBuffer);
                std::cout << "done making reader" << std::endl;
                shared_ptr<XdmfArray> readArray = XdmfArray::New();
                readArray->initialize<int>(0);
                readArray->insert(readController);

                std::cout << "reader set" << std::endl;
                readArray->read();//if it hangs here, read requires all cores to read at the same time
                std::cout << "done reading" << std::endl;
                std::cout << "printing output" << std::endl;

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
        }*/


        
        //mutlicore read with varying sizes
        std::cout << "getting output" << std::endl;
        // Read data (Working on getting this to produce meaningful results.)
        startVector.push_back(datacount - (startindex + array->getSize()));
        std::cout << "starting index" << startVector[0] << std::endl;
        strideVector.push_back(1);
        std::cout << "stride = " << strideVector[0] << std::endl;
        countVector.push_back(array->getSize());
        std::cout << "number of values = " << countVector[0] << std::endl;
        datasizeVector.push_back(datacount);
        std::cout << "Size of block = " << datasizeVector[0] << std::endl;
        readController = XdmfHDF5ControllerDSM::New(
                array->getHeavyDataController(0)->getFilePath(),
                array->getHeavyDataController(0)->getDataSetPath(),
                array->getHeavyDataController(0)->getType(),
                startVector,
                strideVector,
                countVector,
                datasizeVector,
                dsmBuffer);
        std::cout << "done making reader" << std::endl;
        shared_ptr<XdmfArray> readArray = XdmfArray::New();
        readArray->reserve(array->getSize()*size);
        readArray->insert(readController);

        std::cout << "reader set" << std::endl;
        readArray->read();
        std::cout << "done reading" << std::endl;
        std::cout << "printing output" << std::endl;

        for (unsigned int i = 0; i<size; ++i)
        {
                MPI_Barrier(comm);
                if (i == id)
                {
                        std::cout << "Core # " << id << std::endl;
                        std::cout << "Controller stats" << std::endl;
                        std::cout << "datasetpath = " << array->getHeavyDataController(0)->getDataSetPath() << std::endl;
                        std::cout << "datasetpath = " << array->getHeavyDataController(0)->getFilePath() << std::endl;
                        outputVector = array->getHeavyDataController(0)->getDataspaceDimensions();
                        std::cout << "Data space dimensions" << std::endl;
                        for (int j=0; j<outputVector.size(); ++j)
                        {
                                std::cout << "[" << j << "] =" << outputVector[j] << std::endl;
                        }
                        std::cout << "Controller Dimensions" << std::endl;
                        outputVector = array->getHeavyDataController(0)->getDimensions();
                        for (int j=0; j<outputVector.size(); ++j)
                        {
                                std::cout << "[" << j << "] =" << outputVector[j] << std::endl;
                        }
                        std::cout << "Controller size" << array->getHeavyDataController(0)->getSize() << std::endl;
                        std::cout << "Controller starts" << std::endl;
                        outputVector = readController->getStart();
                        for (int j=0; j<outputVector.size(); ++j)
                        {
                                std::cout << "[" << j << "] =" << outputVector[j] << std::endl;
                        }
                        std::cout << "Controller strides" << std::endl;
                        outputVector = array->getHeavyDataController(0)->getStride();
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


        /*
        //For the multicore read
        std::cout << "getting output" << std::endl;
        // Read data (Working on getting this to produce meaningful results.)
        startVector.push_back((size - id -1)*4);
        std::cout << "starting index" << startVector[0] << std::endl;
        strideVector.push_back(1);
        std::cout << "stride = " << strideVector[0] << std::endl;
        countVector.push_back(4);
        std::cout << "number of values = " << countVector[0] << std::endl;
        datasizeVector.push_back(4*size);
        std::cout << "Size of block = " << datasizeVector[0] << std::endl;
        readController = XdmfHDF5ControllerDSM::New(
                array->getHeavyDataController(0)->getFilePath(),
                array->getHeavyDataController(0)->getDataSetPath(),
                array->getHeavyDataController(0)->getType(),
                startVector,
                strideVector,
                countVector,
                datasizeVector,
                dsmBuffer);
        std::cout << "done making reader" << std::endl;
        shared_ptr<XdmfArray> readArray = XdmfArray::New();
        readArray->reserve(array->getSize()*size);
        readArray->insert(readController);

        std::cout << "reader set" << std::endl;
        readArray->read();
        std::cout << "done reading" << std::endl;
        std::cout << "printing output" << std::endl;

        for (unsigned int i = 0; i<size; ++i)
        {
                MPI_Barrier(comm);
                if (i == id)
                {
                        std::cout << "Core # " << id << std::endl;
                        std::cout << "Controller stats" << std::endl;
                        std::cout << "datasetpath = " << array->getHeavyDataController(0)->getDataSetPath() << std::endl;
                        std::cout << "filepath = " << array->getHeavyDataController(0)->getFilePath() << std::endl;
                        outputVector = array->getHeavyDataController(0)->getDataspaceDimensions();
                        std::cout << "Data space dimensions" << std::endl;
                        for (int j=0; j<outputVector.size(); ++j)
                        {
                                std::cout << "[" << j << "] =" << outputVector[j] << std::endl;
                        }
                        std::cout << "Controller Dimensions" << std::endl;
                        outputVector = array->getHeavyDataController(0)->getDimensions();
                        for (int j=0; j<outputVector.size(); ++j)
                        {
                                std::cout << "[" << j << "] =" << outputVector[j] << std::endl;
                        }
                        std::cout << "Controller size" << array->getHeavyDataController(0)->getSize() << std::endl;
                        std::cout << "Controller starts" << std::endl;
                        outputVector = readController->getStart();
                        for (int j=0; j<outputVector.size(); ++j)
                        {
                                std::cout << "[" << j << "] =" << outputVector[j] << std::endl;
                        }
                        std::cout << "Controller strides" << std::endl;
                        outputVector = array->getHeavyDataController(0)->getStride();
                        for (int j=0; j<outputVector.size(); ++j)
                        {
                                std::cout << "[" << j << "] =" << outputVector[j] << "\n" << std::endl;
                        }

                        for(unsigned int i=0; i<readArray->getSize(); ++i)
                        {
                                std::cout << "core #" << id <<" readArray[" << i << "] = " << readArray->getValue<int>(i) << std::endl;
                        }
                }
        }*/


        MPI_Barrier(comm);

        //#finalizeMPI
        delete dsmManager;

        MPI_Finalize();

        //#finalizeMPI

        return 0;
}
