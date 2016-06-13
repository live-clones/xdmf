#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfHDF5WriterDSM.hpp"
#include "XdmfHDF5ControllerDSM.hpp"

int main(int argc, char *argv[])
{
        int size, id, dsmSize;
        dsmSize = 64;//The total size of the DSM being created
        MPI_Status status;
        MPI_Comm comm = MPI_COMM_WORLD;

	int xdmfStatus = 0;

        MPI_Init(&argc, &argv);

        MPI_Comm_rank(comm, &id);
        MPI_Comm_size(comm, &size);

        // Change this to determine the number of cores used as servers
        int numServersCores = 2;
        // Change this to determine the size of the arrays generated when initializing
        int writeArraySize = 4;

        void *  testArray = XdmfArrayNew();
        XdmfArrayInitialize(testArray, &writeArraySize, 1,  XDMF_ARRAY_TYPE_INT32, &xdmfStatus);

        char * newPath = "dsm";
        char * newSetPath = "data";

        unsigned int writeStartVector[1];
        unsigned int writeStrideVector[1];
        unsigned int writeCountVector[1];
        unsigned int writeDataSizeVector[1];

        unsigned int readStartVector[1];
        unsigned int readStrideVector[1];
        unsigned int readCountVector[1];
        unsigned int readDataSizeVector[1];

        unsigned int readOutputCountVector[1];

        void * readArray = XdmfArrayNew();
        XdmfArrayInitialize(readArray, &writeArraySize, 1,  XDMF_ARRAY_TYPE_INT32, &xdmfStatus);

        XDMFHDF5CONTROLLERDSM * readController;
        XDMFHDF5CONTROLLERDSM * readOutputController;
        XDMFHDF5CONTROLLERDSM * writeController;

        MPI_Comm workerComm;

        MPI_Group workers, dsmgroup;

        MPI_Comm_group(comm, &dsmgroup);
        int * ServerIds = (int *)calloc((numServersCores), sizeof(int));
        unsigned int index = 0;
	int i;
        for(i=size-numServersCores ; i <= size-1 ; ++i)
        {
                ServerIds[index++] = i;
        }

        MPI_Group_excl(dsmgroup, index, ServerIds, &workers);
        int testval = MPI_Comm_create(comm, workers, &workerComm);
        cfree(ServerIds);

        XDMFHDF5WRITERDSM * exampleWriter = XdmfHDF5WriterDSMNew(newPath, comm, dsmSize/numServersCores, size-numServersCores, size-1, "Application", &xdmfStatus);

        XdmfHDF5WriterDSMSetMode(exampleWriter, XDMF_HEAVY_WRITER_MODE_HYPERSLAB, &xdmfStatus);

        //split out sub-comm for the worker cores
        //server cores will not progress to this point until after the servers are done running
        if (id < size - numServersCores)
        {
                // Split the comm even further

                MPI_Comm readComm, writeComm;

                MPI_Group readingCores, writingCores;

                MPI_Comm_group(workerComm, &workers);
                int * ServerIds = (int *)calloc(((size - numServersCores) / 2), sizeof(int));
                unsigned int index = 0;
                for(i=0 ; i < (int)((size - numServersCores) / 2) ; ++i)
                {
                        ServerIds[index++] = i;
                }

                MPI_Group_excl(workers, index, ServerIds, &readingCores);
                testval = MPI_Comm_create(workerComm, readingCores, &readComm);
                MPI_Group_incl(workers, index, ServerIds, &writingCores);
                testval = MPI_Comm_create(workerComm, writingCores, &writeComm);
                cfree(ServerIds);

                printf("initializing\n");

                // Initialize values
                // Writer is first
                if (id < (int)((size - numServersCores) / 2))
                {
                        unsigned int j;
                        for (j = 1; j <= writeArraySize; ++j)
                        {
                                int value = j*(id+1);
                                XdmfArrayInsertValue(testArray, j-1, &value, XDMF_ARRAY_TYPE_INT32, &xdmfStatus);
                        }
                        writeStartVector[0] = id*writeArraySize;
                        writeStrideVector[0] = 1;
                        writeCountVector[0] = writeArraySize;
                        writeDataSizeVector[0] = writeArraySize*(int)((size-numServersCores) / 2);
                        writeController = XdmfHDF5ControllerDSMNewFromServerBuffer(
                                newPath,
                                newSetPath,
                                XDMF_ARRAY_TYPE_INT32,
                                writeStartVector,
                                writeStrideVector,
                                writeCountVector,
                                writeDataSizeVector,
				1,
                                XdmfHDF5WriterDSMGetServerBuffer(exampleWriter),
                                &xdmfStatus);

                        XdmfArrayInsertHeavyDataController(testArray, (XDMFHEAVYDATACONTROLLER *)writeController, 0);

                        readStartVector[0] = 0;
                        readStrideVector[0] = 1;
                        readCountVector[0] = 0;
                        readDataSizeVector[0] = writeArraySize*(int)((size-numServersCores) / 2);

                        readController = XdmfHDF5ControllerDSMNewFromServerBuffer(
                                newPath,
                                newSetPath,
                                XDMF_ARRAY_TYPE_INT32,
                                readStartVector,
                                readStrideVector,
                                readCountVector,
                                readDataSizeVector,
				1,
                                XdmfHDF5WriterDSMGetServerBuffer(exampleWriter),
				&xdmfStatus);
                        XdmfArrayInsertHeavyDataController(readArray, (XDMFHEAVYDATACONTROLLER *)readController, 0);

                        for (j = 0; j<size-numServersCores; ++j)
                        {
                                MPI_Barrier(writeComm);
                                if (j == id)
                                {
                                        unsigned int k;
                                        for(k=0; k< XdmfArrayGetSize(testArray); ++k)
                                        {
						int value = ((int *)XdmfArrayGetValue(testArray, k,  XDMF_ARRAY_TYPE_INT32, &xdmfStatus))[0];
						printf("core # %d testArray[%d] = %d\n", id, k, value);
                                        }
                                }
                        }
                }
                else
                {
                        // Reader is second
                        readStartVector[0] = 0;
                        readStrideVector[0] = 1;
                        readCountVector[0] = writeArraySize*(int)((size-numServersCores) / 2);
                        readDataSizeVector[0] = writeArraySize*(int)((size-numServersCores) / 2);

                        readController = XdmfHDF5ControllerDSMNewFromServerBuffer(
                                newPath,
                                newSetPath,
                                XDMF_ARRAY_TYPE_INT32,
                                readStartVector,
                                readStrideVector,
                                readCountVector,
                                readDataSizeVector,
				1,
                                XdmfHDF5WriterDSMGetServerBuffer(exampleWriter),
				&xdmfStatus);

                        XdmfArrayInsertHeavyDataController(readArray, (XDMFHEAVYDATACONTROLLER *)readController, 0);

                        readOutputCountVector[0] = 0;

                        readOutputController = XdmfHDF5ControllerDSMNewFromServerBuffer(
                                newPath,
                                newSetPath,
                                XDMF_ARRAY_TYPE_INT32,
                                readStartVector,
                                readStrideVector,
                                readOutputCountVector,
                                readDataSizeVector,
				1,
                                XdmfHDF5WriterDSMGetServerBuffer(exampleWriter),
				&xdmfStatus);

                        writeStartVector[0] = 0;
                        writeStrideVector[0] = 1;
                        writeCountVector[0] = 0;
                        writeDataSizeVector[0] = writeArraySize*(int)((size-numServersCores) / 2);
                        writeController = XdmfHDF5ControllerDSMNewFromServerBuffer(
                                newPath,
                                newSetPath,
                                XDMF_ARRAY_TYPE_INT32,
                                writeStartVector,
                                writeStrideVector,
                                writeCountVector,
                                writeDataSizeVector,
                                1,
                                XdmfHDF5WriterDSMGetServerBuffer(exampleWriter),
				&xdmfStatus);

                        XdmfArrayInsertHeavyDataController(testArray, (XDMFHEAVYDATACONTROLLER *)writeController, 0);
                }

                XdmfArrayAccept(testArray, (XDMFVISITOR *)exampleWriter, &xdmfStatus);

                MPI_Barrier(workerComm);

		unsigned int iteration;
                // This is the loop that manipulates the data
                for (iteration = 0; iteration < 10; ++iteration)
                {
                        printf("loop iteration %d on core %d\n", iteration, id);

                        // Read in to the first set of cores
                        if (id >= (int)((size - numServersCores) / 2))
                        {
                                // Controllers are accessed like this since the writer removes them and creates its own.
				XdmfHDF5ControllerDSMSetWorkerComm((XDMFHDF5CONTROLLERDSM *)XdmfArrayGetHeavyDataController(readArray, 0), readComm, &xdmfStatus);
                                printf("read on core %d\n", id);
                                XdmfArrayRead(readArray, &xdmfStatus);
                                MPI_Barrier(readComm);

                                if (id == (int)((size - numServersCores) / 2))
                                {
					unsigned int j;
                                        for(j=0; j<XdmfArrayGetSize(readArray); ++j)
                                        {
                                                int tempVal = ((int *)XdmfArrayGetValue(readArray, j, XDMF_ARRAY_TYPE_INT32, &xdmfStatus))[0];
                                                tempVal = tempVal * 2;
                                                XdmfArrayInsertValue(readArray, j, &tempVal, XDMF_ARRAY_TYPE_INT32, &xdmfStatus);
                                                // Pull the value from the array in order to ensure the change has happened
                                                printf("core #%d readArray[%d] = %d\n", id, j, ((int *)XdmfArrayGetValue(readArray, j, XDMF_ARRAY_TYPE_INT32, &xdmfStatus))[0]);
                                        }
                                }
                        }
                        XdmfHDF5ControllerDSMSetWorkerComm((XDMFHDF5CONTROLLERDSM *)XdmfArrayGetHeavyDataController(readArray, 0), workerComm, &xdmfStatus);

                        if (id > (int)((size - numServersCores) / 2))
                        {
                                // Only the first read core should write out data
                                XdmfArrayRemoveHeavyDataController(readArray, 0);
				XdmfArrayInsertHeavyDataController(readArray, (XDMFHEAVYDATACONTROLLER *)readOutputController, 0);
                        }

                        XdmfArrayAccept(readArray, (XDMFVISITOR *)exampleWriter, &xdmfStatus);

                        if (id > (int)((size - numServersCores) / 2))
                        {
                                // Only the first read core should write out data
                                XdmfArrayRemoveHeavyDataController(readArray, 0);
                                XdmfArrayInsertHeavyDataController(readArray, (XDMFHEAVYDATACONTROLLER *)readController, 0);
                        }

                        MPI_Barrier(workerComm);

                        printf("on writing cores\n");

                        if (id < (int)((size - numServersCores) / 2))
                        {
                                XdmfHDF5ControllerDSMSetWorkerComm((XDMFHDF5CONTROLLERDSM *)XdmfArrayGetHeavyDataController(testArray, 0), writeComm, &xdmfStatus);
                                XdmfArrayRead(testArray, &xdmfStatus);
                                MPI_Barrier(writeComm);
				unsigned int i;
                                for (i = 0; i<size; ++i)
                                {
                                        MPI_Barrier(writeComm);
                                        if (i == id)
                                        {
                                                unsigned int j;
                                                for(j=0; j<XdmfArrayGetSize(testArray); ++j)
                                                {
                                                        int tempVal = ((int *)XdmfArrayGetValue(testArray, j, XDMF_ARRAY_TYPE_INT32, &xdmfStatus))[0];
                                                        tempVal = tempVal * 3;
                                                        XdmfArrayInsertValue(testArray, j, &tempVal, XDMF_ARRAY_TYPE_INT32, &xdmfStatus);
                                                        // Pull the value from the array in order to ensure the change has happened
                                                        printf("core #%d testArray[%d] = %d\n", id, j, ((int *)XdmfArrayGetValue(testArray, j, XDMF_ARRAY_TYPE_INT32, &xdmfStatus))[0]);
                                                }
                                        }
                                }
                        }
                        XdmfHDF5ControllerDSMSetWorkerComm(writeController, workerComm, &xdmfStatus);
                        XdmfArrayAccept(testArray, (XDMFVISITOR *)exampleWriter, &xdmfStatus);
                }
        }

        if (id == 0)
        {
                XdmfHDF5WriterDSMStopDSM(exampleWriter, &xdmfStatus);
        }

        MPI_Barrier(comm);

        //the dsmManager must be deleted or else there will be a segfault
//        XdmfHDF5WriterDSMDeleteManager(exampleWriter);

        MPI_Finalize();

        return 0;
}
