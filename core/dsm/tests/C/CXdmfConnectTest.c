#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <XdmfArray.hpp>
#include <XdmfArrayType.hpp>
#include <XdmfHDF5WriterDSM.hpp>
#include <XdmfHDF5ControllerDSM.hpp>
#include <XdmfDSMBuffer.hpp>
#include <XdmfDSMCommMPI.hpp>

int main(int argc, char *argv[])
{
/* This test does not work properly with openmpi*/
/* due to an issue with the openmpi code*/
#ifndef OPEN_MPI
        /* initMPI begin */

        int size, id, dsmSize;
        dsmSize = 64;
        MPI_Status status;
	int xdmfStatus = 0;
        MPI_Comm comm = MPI_COMM_WORLD;

        MPI_Init(&argc, &argv);

        MPI_Comm_rank(comm, &id);
        MPI_Comm_size(comm, &size);



        char * newPath = "dsm";
        char * newSetPath = "Data";

        /* Initializing objects */

        void * testComm = XdmfDSMCommMPINew();
        XdmfDSMCommMPIDupComm(testComm, comm, &xdmfStatus);
        XdmfDSMCommMPIInit(testComm, &xdmfStatus);
        void * testBuffer = XdmfDSMBufferNew();
        XdmfDSMBufferSetIsServer(testBuffer, 0);
        XdmfDSMBufferSetComm(testBuffer, testComm);
        XdmfDSMBufferSetIsConnected(testBuffer, 1);

        XDMFHDF5WRITERDSM * exampleWriter = XdmfHDF5WriterDSMNewFromServerBuffer(newPath, testBuffer, &xdmfStatus);

        #ifdef  _WIN32
                Sleep(500)
        #else
                sleep(5);
        #endif	

        char * checkFileName = XdmfDSMCommMPIGetDsmFileName(testComm);

	FILE * checkFile = fopen(checkFileName, "r");

	while (checkFile == NULL) {
		printf("file not found\n");
		#ifdef  _WIN32
                        Sleep(500);
                #else
                        sleep(5);
                #endif
	}

	printf("done check\n");

	fclose(checkFile);

        XdmfDSMCommMPIReadDsmPortName(XdmfDSMBufferGetComm(XdmfHDF5WriterDSMGetServerBuffer(exampleWriter)));

        char * portName = XdmfDSMCommMPIGetDsmPortName(XdmfDSMBufferGetComm(XdmfHDF5WriterDSMGetServerBuffer(exampleWriter)));

        XdmfDSMCommMPISetDsmPortName(XdmfDSMBufferGetComm(XdmfHDF5WriterDSMGetServerBuffer(exampleWriter)), portName);

        XdmfDSMBufferConnect(XdmfHDF5WriterDSMGetServerBuffer(exampleWriter), 0, &xdmfStatus);

        MPI_Barrier(XdmfDSMCommMPIGetIntraComm(XdmfDSMBufferGetComm(XdmfHDF5WriterDSMGetServerBuffer(exampleWriter))));

        void * writeArray = XdmfArrayNew();

	/* Iterators */

	int i = 0;
	unsigned int j = 0;

        for (i = 1; i <= 5; ++i)
        {
		int value = i*(id+1);
                XdmfArrayPushBack(writeArray, &value, XDMF_ARRAY_TYPE_INT32, &xdmfStatus);
        }

        unsigned int writeStartVector[1];
        unsigned int writeStrideVector[1];
        unsigned int writeCountVector[1];
        unsigned int writeDataSizeVector[1];

        writeStartVector[0] = id*5;
        writeStrideVector[0] = 1;
        writeCountVector[0] = 5;
        writeDataSizeVector[0] = 5*size;

        XDMFHDF5CONTROLLERDSM * writeController = XdmfHDF5ControllerDSMNewFromServerBuffer(
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

        unsigned int readStartVector[1];
        unsigned int readStrideVector[1];
        unsigned int readCountVector[1];
        unsigned int readDataSizeVector[1];

        readStartVector[0] = 5*id;
        readStrideVector[0] = 1;
        readCountVector[0] = 5;
        readDataSizeVector[0] = 5*size;

        XDMFHDF5CONTROLLERDSM * readController = XdmfHDF5ControllerDSMNewFromServerBuffer(
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

        XdmfHDF5WriterDSMSetMode(exampleWriter, XDMF_HEAVY_WRITER_MODE_HYPERSLAB, &xdmfStatus);

        /* Done initializing */

        for (i = 0; i < size; ++i)
        {
                MPI_Barrier(XdmfDSMCommMPIGetIntraComm(XdmfDSMBufferGetComm(XdmfHDF5WriterDSMGetServerBuffer(exampleWriter))));
                if (i == id)
                {
                        printf("Array on core %d contains:\n", XdmfDSMCommMPIGetInterId(XdmfDSMBufferGetComm(XdmfHDF5WriterDSMGetServerBuffer(exampleWriter))));
                        for (j = 0; j < XdmfArrayGetSize(writeArray); ++j)
                        {
                                printf("[%d] = %d\n", j, ((int *)XdmfArrayGetValue(writeArray, j, XDMF_ARRAY_TYPE_INT32, &xdmfStatus))[0]);
                        }
                }
        }
	unsigned int numloops = 0;
        for (numloops = 0; numloops < 4; ++numloops)
        {

                if (XdmfArrayGetNumberHeavyDataControllers(writeArray) > 0)
                {
                        XdmfArrayRemoveHeavyDataController(writeArray, 0);
                }
                XdmfArrayInsertHeavyDataController(writeArray, (XDMFHEAVYDATACONTROLLER *)writeController, 0);

                if (id == size - 1)
                {
                        printf("\n\n");
                }
                XdmfArrayAccept(writeArray, (XDMFVISITOR *)exampleWriter, &xdmfStatus);

                if (id == size - 1)
                {
                        int sentData = 1;
                        XdmfDSMBufferSendAcknowledgment(XdmfHDF5WriterDSMGetServerBuffer(exampleWriter),
                                                        XdmfDSMCommMPIGetInterId(XdmfDSMBufferGetComm(XdmfHDF5WriterDSMGetServerBuffer(exampleWriter))) + 1,
                                                        sentData,
                                                        XDMF_DSM_EXCHANGE_TAG,
                                                        XDMF_DSM_INTER_COMM,
                                                        &xdmfStatus);
			XdmfDSMBufferReceiveAcknowledgment(XdmfHDF5WriterDSMGetServerBuffer(exampleWriter),
                                                           XdmfDSMCommMPIGetInterId(XdmfDSMBufferGetComm(XdmfHDF5WriterDSMGetServerBuffer(exampleWriter))) + 1,
                                                           &sentData,
                                                           XDMF_DSM_EXCHANGE_TAG,
                                                           XDMF_DSM_INTER_COMM,
                                                           &xdmfStatus);
                }

                MPI_Barrier(XdmfDSMCommMPIGetIntraComm(XdmfDSMBufferGetComm(XdmfHDF5WriterDSMGetServerBuffer(exampleWriter))));

                XdmfArrayRemoveHeavyDataController(writeArray, 0);
                XdmfArrayInsertHeavyDataController(writeArray, (XDMFHEAVYDATACONTROLLER *)readController, 0);
                XdmfArrayRelease(writeArray);
                XdmfArrayRead(writeArray, &xdmfStatus);

                for (i = 0; i < size; ++i)
                {
                        MPI_Barrier(XdmfDSMCommMPIGetIntraComm(XdmfDSMBufferGetComm(XdmfHDF5WriterDSMGetServerBuffer(exampleWriter))));
                        if (i == id)
                        {
                                printf("Array on core %d contains:\n", XdmfDSMCommMPIGetInterId(XdmfDSMBufferGetComm(XdmfHDF5WriterDSMGetServerBuffer(exampleWriter))));
                                for (j = 0; j < XdmfArrayGetSize(writeArray); ++j)
                                {
                                        int tempVal = ((int *)XdmfArrayGetValue(writeArray, j, XDMF_ARRAY_TYPE_INT32, &xdmfStatus))[0];
                                        tempVal = tempVal * 2;
                                        XdmfArrayInsertValue(writeArray, j, &tempVal, XDMF_ARRAY_TYPE_INT32, &xdmfStatus);
                                        printf("[%d] = %d\n", j, ((int *)XdmfArrayGetValue(writeArray, j, XDMF_ARRAY_TYPE_INT32, &xdmfStatus))[0]);
                                }
                        }
                }
        }

        if (id == size - 1)
        {
                int sentData = 1;
                XdmfDSMBufferSendAcknowledgment(XdmfHDF5WriterDSMGetServerBuffer(exampleWriter),
                                                XdmfDSMCommMPIGetInterId(XdmfDSMBufferGetComm(XdmfHDF5WriterDSMGetServerBuffer(exampleWriter))) + 1,
                                                sentData,
                                                XDMF_DSM_EXCHANGE_TAG,
                                                XDMF_DSM_INTER_COMM,
                                                &xdmfStatus);
        }

        MPI_Barrier(XdmfDSMCommMPIGetIntraComm(XdmfDSMBufferGetComm(XdmfHDF5WriterDSMGetServerBuffer(exampleWriter))));

        MPI_Barrier(XdmfDSMCommMPIGetInterComm(XdmfDSMBufferGetComm(XdmfHDF5WriterDSMGetServerBuffer(exampleWriter))));

        XdmfDSMBufferDisconnect(XdmfHDF5WriterDSMGetServerBuffer(exampleWriter), &xdmfStatus);

        XdmfDSMCommMPIDisconnect(XdmfDSMBufferGetComm(XdmfHDF5WriterDSMGetServerBuffer(exampleWriter)), &xdmfStatus);

        MPI_Finalize();

#endif

        return 0;
}
