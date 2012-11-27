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
	int size, id, providedThreading, dsmSize;
	dsmSize = 16;
	MPI_Comm comm = MPI_COMM_WORLD;

	MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &providedThreading);

	MPI_Comm_rank(comm, &id);
	MPI_Comm_size(comm, &size);

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

	// Create DSM
	H5FDdsmManager * dsmManager = new H5FDdsmManager();
	dsmManager->SetCommunicator(comm);
	dsmManager->SetLocalBufferSizeMBytes(dsmSize / size);
	dsmManager->CreateDSM();

	H5FDdsmBuffer * dsmBuffer = dsmManager->GetDSMHandle();

	H5FD_dsm_set_mode(H5FD_DSM_MANUAL_SERVER_UPDATE, dsmBuffer);

	// Get info from remote server
	double remoteMB = dsmBuffer->GetTotalLength() / (1024.0 * 1024.0);
	double numServers = dsmBuffer->GetEndServerId() + 1;
	if (id == 0)
	{
		std::cout << "DSM server memory size is : "
			<< (int)remoteMB << " MB" << std::endl;
		std::cout << "DSM server process count  : "
			<< (int)numServers << std::endl;
	}

	// Create Array
	// Array should be distributed among processes
	shared_ptr<XdmfArray> array = XdmfArray::New();
	array->initialize<int>(0);
	array->pushBack(id+id*0);
	array->pushBack(id+id*1);
	array->pushBack(id+id*2);

	// Create DSM Writer and write to DSM space.
	shared_ptr<XdmfHDF5WriterDSM> writer = XdmfHDF5WriterDSM::New("dsm", dsmBuffer);
	array->accept(writer);

	//ensure all writing finishes
	MPI_Barrier(comm);

	if (id == 0)
	{
		std::cout << "getting output" << std::endl;
		// Read data
		std::vector<unsigned int> startVector;
		startVector.push_back(0);
		std::vector<unsigned int> strideVector;
		strideVector.push_back(0);
		std::vector<unsigned int> countVector;
		countVector.push_back(array->getSize()*size);
		shared_ptr<XdmfArray> readArray = XdmfArray::New();
		shared_ptr<XdmfHDF5ControllerDSM> readController = XdmfHDF5ControllerDSM::New(
			array->getHeavyDataController()->getFilePath(),
			array->getHeavyDataController()->getDataSetPath(),
			array->getHeavyDataController()->getType(),
			startVector,
			strideVector,
			countVector,
			dsmBuffer);
		readArray->setHeavyDataController(readController);
		readArray->read();

		std::cout << "printing output" << std::endl;
		for(unsigned int i=0; i<readArray->getSize(); ++i)
		{
			std::cout << "readArray[" << i << "] = " << readArray->getValue<int>(i) << std::endl;
		}
	}

	// Wait for everyone to have finished reading

	std::cout << "Finished from Core #" << id << std::endl;

	MPI_Barrier(comm);
	delete dsmManager;

	MPI_Finalize();

	return 0;
}
