#include <mpi.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <XdmfArray.hpp>
#include <XdmfArrayType.hpp>
#include <XdmfHDF5WriterDSM.hpp>
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
        int numServersCores = size - 1;
        int numConnections = 2;

//        shared_ptr<XdmfHDF5WriterDSM> exampleWriter = XdmfHDF5WriterDSM::New(newPath, comm, dsmSize/numServersCores, size-numServersCores, size-1);
        shared_ptr<XdmfHDF5WriterDSM> exampleWriter = XdmfHDF5WriterDSM::New(newPath, comm, dsmSize/numServersCores, 1, 1, "Accept");

        if (id == 0)
        {
                //#initMPI end

                //#GetDsmFileName begin

                std::string connectionFileName = exampleWriter->getServerBuffer()->GetComm()->GetDsmFileName();

                //#GetDsmFileName end

                //#SetDsmFileName begin

                exampleWriter->getServerBuffer()->GetComm()->SetDsmFileName(connectionFileName);

                //#SetDsmFileName end

                //#OpenPort begin

                exampleWriter->getServerBuffer()->GetComm()->OpenPort();

                //#OpenPort end

                //#SendAccept begin

                exampleWriter->getServerBuffer()->SendAccept(numConnections);

                //#SendAccept end

                /*

                //#manualAccept begin

                // Notify the server cores to accept connections
                for (int i = exampleWriter->getServerBuffer()->StartServerId; i <= exampleWriter->getServerBuffer()->EndServerId; ++i)
                {
                        if (i != exampleWriter->getServerBuffer()->Comm->GetId())
                        {
                                exampleWriter->getServerBuffer()->SendCommandHeader(XDMF_DSM_ACCEPT, i, 0, 0, XDMF_DSM_INTER_COMM);
                                exampleWriter->getServerBuffer()->SendAcknowledgment(i, numConnections, XDMF_DSM_EXCHANGE_TAG, XDMF_DSM_INTER_COMM);
                        }
                }
                // Accept connections
                exampleWriter->getServerBuffer()->Comm->Accept(numConnections);
                // Distribute current DSM status
                exampleWriter->getServerBuffer()->SendInfo();

                //#manualAccept end

                */

                //#finishwork begin

                MPI_Barrier(exampleWriter->getServerBuffer()->GetComm()->GetIntraComm());
        }

        MPI_Barrier(exampleWriter->getServerBuffer()->GetComm()->GetInterComm());

        //#finishwork end

        //#ClosePort begin

	if (id == 0)
	{
	        exampleWriter->getServerBuffer()->GetComm()->ClosePort();
	}

        //#ClosePort end

        //#finalizeMPI begin

        MPI_Finalize();

        //#finalizeMPI end

#else

        std::cout << "Interprogram DSM does not work properly with OpenMPI" << std::endl;

#endif

        return 0;
}
