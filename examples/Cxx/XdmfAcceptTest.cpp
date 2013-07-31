#include <mpi.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <XdmfArray.hpp>
#include <XdmfArrayType.hpp>
#include <XdmfHDF5WriterDSM.hpp>


int main(int argc, char *argv[])
{
	int size, id, dsmSize;
	dsmSize = 64;
	MPI_Status status;
	MPI_Comm comm = MPI_COMM_WORLD;

	MPI_Init(&argc, &argv);

	MPI_Comm_rank(comm, &id);
	MPI_Comm_size(comm, &size);


	std::string newPath = "dsm";
	int numServersCores = 3;


	shared_ptr<XdmfHDF5WriterDSM> exampleWriter = XdmfHDF5WriterDSM::New(newPath, comm, dsmSize/numServersCores, size-numServersCores, size-1);

	if (id < size-numServersCores)
	{
		exampleWriter->getServerBuffer()->GetComm()->OpenPort();
		exampleWriter->getServerBuffer()->SendAccept(2);

		MPI_Barrier(exampleWriter->getServerBuffer()->GetComm()->GetIntraComm());
	}

	MPI_Barrier(exampleWriter->getServerBuffer()->GetComm()->GetInterComm());

	exampleWriter->getServerBuffer()->GetComm()->ClosePort();

	MPI_Finalize();

	return 0;
}
