#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <XdmfArray.hpp>
#include <XdmfArrayType.hpp>
#include <XdmfHDF5WriterDSM.hpp>


int main(int argc, char *argv[])
{
/* This test does not work properly with openmpi*/
/* due to an issue with the openmpi code*/
#ifndef OPEN_MPI
        int size, id, dsmSize;
	int xdmfStatus = 0;
        dsmSize = 64;
        MPI_Status status;
        MPI_Comm comm = MPI_COMM_WORLD;

        MPI_Init(&argc, &argv);

        MPI_Comm_rank(comm, &id);
        MPI_Comm_size(comm, &size);

        char * newPath = "dsm";
        int numServersCores = size - 1;
        int numConnections = 2;


        XDMFHDF5WRITERDSM * exampleWriter = XdmfHDF5WriterDSMNew(newPath, comm, dsmSize/numServersCores, size-numServersCores, size-1, "Accept", &xdmfStatus);

        if (id == 0)
        {
                XdmfDSMCommMPIOpenPort(XdmfDSMBufferGetComm(XdmfHDF5WriterDSMGetServerBuffer(exampleWriter)), &xdmfStatus);

                XdmfDSMBufferSendAccept(XdmfHDF5WriterDSMGetServerBuffer(exampleWriter), numConnections);

                MPI_Barrier(XdmfDSMCommMPIGetIntraComm(XdmfDSMBufferGetComm(XdmfHDF5WriterDSMGetServerBuffer(exampleWriter))));
        }

        MPI_Barrier(XdmfDSMCommMPIGetInterComm(XdmfDSMBufferGetComm(XdmfHDF5WriterDSMGetServerBuffer(exampleWriter))));

	if (id == 0)
	{
	        XdmfDSMCommMPIClosePort(XdmfDSMBufferGetComm(XdmfHDF5WriterDSMGetServerBuffer(exampleWriter)), &xdmfStatus);
	}

        MPI_Finalize();
#else

        printf("Interprogram DSM does not work properly with OpenMPI\n");

#endif

        return 0;
}
