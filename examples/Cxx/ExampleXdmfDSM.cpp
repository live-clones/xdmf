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
	MPI_Status status;
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

	/*This write algorithm isn't supported yet, the writers all write to the same location instead of propagating for each core
	// Create Array
	// Array should be distributed among processes
	shared_ptr<XdmfArray> array = XdmfArray::New();
	array->initialize<int>(0);
	array->pushBack(id);
	array->pushBack(id*2);
	array->pushBack(id*3);
	array->pushBack(id*4);

	std::cout << "from core #" << id << " generating these values:" << id << " " << id*2 << " " << id*3 << " " << id*4 << std::endl;

	std::cout << "setting up DSM writer" << std::endl;
	// Create DSM Writer and write to DSM space.
	shared_ptr<XdmfHDF5WriterDSM> writer = XdmfHDF5WriterDSM::New("dsm", dsmBuffer);
        std::cout << "DSM writer set up" << std::endl;
	MPI_Barrier(comm);
	array->accept(writer);
	std::cout << "DSM writer used" << std::endl;

	//ensure all writing finishes
	MPI_Barrier(comm);

	if (id == 0)
	{
		std::cout << "getting output" << std::endl;
		// Read data (Working on getting this to produce meaningful results.)
		std::vector<unsigned int> startVector;
		startVector.push_back(array->getSize() * id);
		std::cout << "starting index" << startVector[0] << std::endl;
		std::vector<unsigned int> strideVector;
		strideVector.push_back(1);
		std::vector<unsigned int> countVector;
		countVector.push_back(array->getSize());
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
			std::cout << "core #" << id <<" readArray[" << i << "] = " << readArray->getValue<int>(i) << std::endl;
		}
	}
	*/


	//variables holding data for building the reading controllers
	std::string readFilePath = "";
	std::string readSetPath = "";
	shared_ptr<const XdmfArrayType> readType = XdmfArrayType::Int32();
	int totalValues = 0;
	//Array is generated and written to DSM
	shared_ptr<XdmfArray> array = XdmfArray::New();

	array->initialize<int>(0);
	for (int i = 0; i<(size*2); i++)
	{
		array->pushBack(i);
	}

	shared_ptr<XdmfHDF5WriterDSM> spreadWriter = XdmfHDF5WriterDSM::New("dsmspread" , dsmBuffer);


	array->accept(spreadWriter);


	readFilePath = array->getHeavyDataController()->getFilePath();
	readSetPath = array->getHeavyDataController()->getDataSetPath();
	readType = array->getHeavyDataController()->getType();
	totalValues = array->getSize();
	MPI_Barrier(comm);

	//all cores read part of the written buffer

	std::vector<unsigned int> startVector;
	startVector.push_back((totalValues/size)*((size-1)-id));
	std::vector<unsigned int> strideVector;
	strideVector.push_back(1);
	std::vector<unsigned int> countVector;
	countVector.push_back((totalValues/size));
	shared_ptr<XdmfArray> readArray = XdmfArray::New();
	shared_ptr<XdmfHDF5ControllerDSM> readController = XdmfHDF5ControllerDSM::New(
		readFilePath,
		readSetPath,
		readType,
		startVector,
		strideVector,
		countVector,
		dsmBuffer);
	readArray->setHeavyDataController(readController);
	readArray->read();

	for (int j = 0; j<size; j++)
	{
		MPI_Barrier(comm);
		if (j==id)
		{
			std::cout << "core #" << id << " starting index" << startVector[0] << std::endl;
			std::cout << "core #" << id << " printing output" << std::endl;
			for(unsigned int i=0; i<readArray->getSize(); ++i)
			{
				std::cout << "core #" << id <<" readArray[" << i << "] = " << readArray->getValue<int>(i) << std::endl;
			}
		}
	}



	// Wait for everyone to have finished reading
	MPI_Barrier(comm);
	std::cout << "Finished from Core #" << id << std::endl;

	MPI_Barrier(comm);
	delete dsmManager;

	MPI_Finalize();

	return 0;
}
