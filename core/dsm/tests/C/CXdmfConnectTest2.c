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
        int size, id, dsmSize;
        dsmSize = 64;
        MPI_Status status;
        MPI_Comm comm = MPI_COMM_WORLD;

	int xdmfStatus = 0;

	/* Iterators */

	int i = 0;
	unsigned int j = 0;

        MPI_Init(&argc, &argv);

        MPI_Comm_rank(comm, &id);
        MPI_Comm_size(comm, &size);

        char * newPath = "dsm";
        char * newSetPath = "Data";

        /* Initializing objects */

        /* 
         * Since the start and end ids are larger than the size there are no buffers alloted
         * thus, no blockage occurs
         */
        void * testComm = XdmfDSMCommMPINew();
        XdmfDSMCommMPIDupComm(testComm, comm, &xdmfStatus);
        XdmfDSMCommMPIInit(testComm, &xdmfStatus);
        void * testBuffer = XdmfDSMBufferNew();
        XdmfDSMBufferSetIsServer(testBuffer, 0);
        XdmfDSMBufferSetComm(testBuffer, testComm);
        XdmfDSMBufferSetIsConnected(testBuffer, 1);

        unsigned int readStartVector[1];
        unsigned int readStrideVector[1];
        unsigned int readCountVector[1];
        unsigned int readDataSizeVector[1];

        readStartVector[0] = 5*id;
        readStrideVector[0] = 1;
        readCountVector[0] = 5;
        readDataSizeVector[0] = 5*size;

        void * readArray = XdmfArrayNew();

	int arraySize[1] = {5};

	XdmfArrayInitialize(readArray, arraySize, 1,  XDMF_ARRAY_TYPE_INT32, &xdmfStatus);

        XDMFHDF5CONTROLLERDSM * readController = XdmfHDF5ControllerDSMNewFromServerBuffer(
                newPath,
                newSetPath,
                XDMF_ARRAY_TYPE_INT32,
                readStartVector,
                readStrideVector,
                readCountVector,
                readDataSizeVector,
		1,
                testBuffer,
		&xdmfStatus);

        #ifdef  _WIN32
                Sleep(1000)
        #else
                sleep(10);
        #endif

        char * checkFileName = XdmfDSMCommMPIGetDsmFileName(testComm);

        FILE * checkFile = fopen(checkFileName, "r");

        while (checkFile == NULL) {
		printf("file not found\n");
                #ifdef  _WIN32
                        Sleep(500)
                #else
                        sleep(5);
                #endif
        }

	printf("done check\n");

	fclose(checkFile);

        XdmfDSMCommMPIReadDsmPortName(XdmfDSMBufferGetComm(XdmfHDF5ControllerDSMGetServerBuffer(readController)));

        XdmfDSMBufferConnect(XdmfHDF5ControllerDSMGetServerBuffer(readController), 0, &xdmfStatus);

        XDMFHDF5WRITERDSM * exampleWriter = XdmfHDF5WriterDSMNewFromServerBuffer(newPath, testBuffer, &xdmfStatus);

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

        XdmfHDF5WriterDSMSetMode(exampleWriter, XDMF_HEAVY_WRITER_MODE_HYPERSLAB, &xdmfStatus);

        /* Done initialization */

        MPI_Barrier(XdmfDSMCommMPIGetIntraComm(XdmfDSMBufferGetComm(XdmfHDF5ControllerDSMGetServerBuffer(readController))));

	unsigned int numloops = 0;

        for (numloops = 0; numloops < 4; ++numloops)
        {
                if (id == 0)
                {
                        int receiveData = 0;
                        XdmfDSMBufferReceiveAcknowledgment(XdmfHDF5ControllerDSMGetServerBuffer(readController),
                                                           XdmfDSMCommMPIGetInterId(XdmfDSMBufferGetComm(XdmfHDF5ControllerDSMGetServerBuffer(readController))) - 1,
                                                           &receiveData,
                                                           XDMF_DSM_EXCHANGE_TAG,
                                                           XDMF_DSM_INTER_COMM,
                                                           &xdmfStatus);
                }

                MPI_Barrier(XdmfDSMCommMPIGetIntraComm(XdmfDSMBufferGetComm(XdmfHDF5ControllerDSMGetServerBuffer(readController))));

                if (XdmfArrayGetNumberHeavyDataControllers(readArray) > 0)
                {
                        XdmfArrayRemoveHeavyDataController(readArray, 0);
                }
                XdmfArrayInsertHeavyDataController(readArray, (XDMFHEAVYDATACONTROLLER *)readController, 0);
                XdmfArrayRead(readArray, &xdmfStatus);

                for (i = 0; i < size; ++i)
                {
                        MPI_Barrier(XdmfDSMCommMPIGetIntraComm(XdmfDSMBufferGetComm(XdmfHDF5ControllerDSMGetServerBuffer(readController))));
                        if (i == id)
                        {
                                printf("Array on core %d contains:\n", XdmfDSMCommMPIGetInterId(XdmfDSMBufferGetComm(XdmfHDF5WriterDSMGetServerBuffer(exampleWriter))));
                                for (j = 0; j < XdmfArrayGetSize(readArray); ++j)
                                {
                                        int tempVal = ((int *)XdmfArrayGetValue(readArray, j, XDMF_ARRAY_TYPE_INT32, &xdmfStatus))[0];
                                        tempVal = tempVal * 3;
                                        XdmfArrayInsertValue(readArray, j, &tempVal, XDMF_ARRAY_TYPE_INT32, &xdmfStatus);
                                        printf("[%d] = %d\n", j, ((int *)XdmfArrayGetValue(readArray, j, XDMF_ARRAY_TYPE_INT32, &xdmfStatus))[0]);
                                }
                        }
                }

                MPI_Barrier(XdmfDSMCommMPIGetIntraComm(XdmfDSMBufferGetComm(XdmfHDF5ControllerDSMGetServerBuffer(readController))));

                if (id == 0)
                {
                        printf("\n\n");
                }

                XdmfArrayRemoveHeavyDataController(readArray, 0);
                XdmfArrayInsertHeavyDataController(readArray, (XDMFHEAVYDATACONTROLLER *)writeController, 0);

                XdmfArrayAccept(readArray, (XDMFVISITOR *)exampleWriter, &xdmfStatus);

                if (id == 0)
                {
                        int receiveData = 0;
                        XdmfDSMBufferSendAcknowledgment(XdmfHDF5ControllerDSMGetServerBuffer(readController),
                                                        XdmfDSMCommMPIGetInterId(XdmfDSMBufferGetComm(XdmfHDF5ControllerDSMGetServerBuffer(readController))) - 1,
                                                        receiveData,
                                                        XDMF_DSM_EXCHANGE_TAG,
                                                        XDMF_DSM_INTER_COMM,
                                                        &xdmfStatus);
                }
        }

        /* This last acknowledgment is to end the loop. */

        if (id == 0)
        {
                int receiveData = 0;
                XdmfDSMBufferReceiveAcknowledgment(XdmfHDF5ControllerDSMGetServerBuffer(readController),
                                                   XdmfDSMCommMPIGetInterId(XdmfDSMBufferGetComm(XdmfHDF5ControllerDSMGetServerBuffer(readController))) - 1,
                                                   &receiveData,
                                                   XDMF_DSM_EXCHANGE_TAG,
                                                   XDMF_DSM_INTER_COMM,
                                                   &xdmfStatus);
        }

        MPI_Barrier(XdmfDSMCommMPIGetIntraComm(XdmfDSMBufferGetComm(XdmfHDF5ControllerDSMGetServerBuffer(readController))));

        /* Do work stuff here */

        if (id == 0)
        {
                XdmfHDF5ControllerDSMStopDSM(readController, &xdmfStatus);
        }

        MPI_Barrier(XdmfDSMCommMPIGetInterComm(XdmfDSMBufferGetComm(XdmfHDF5ControllerDSMGetServerBuffer(readController))));

        MPI_Finalize();

#endif

        return 0;
}
