#include <H5FDdsmManager.h>
#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfHDF5WriterDSM.hpp"

int main(int argc, char * argv[])
{
	MPI_Init(&argc, &argv);

	int rank, size;

	MPI_Comm comm = MPI_COMM_WORLD;
	MPI_Comm_rank(comm, &rank);
	MPI_Comm_size(comm, &size);

	// Create DSM Server
	H5FDdsmManager * dsmServer = new H5FDdsmManager();
	dsmServer->SetCommunicator(comm);
	dsmServer->SetLocalBufferSizeMBytes(16);
	dsmServer->SetDsmCommType(H5FD_DSM_COMM_SOCKET);
	dsmServer->SetDsmIsServer(1);
	dsmServer->SetServerHostName("default");
	dsmServer->SetServerPort(22000);
	dsmServer->CreateDSM();
	dsmServer->PublishDSM();

	// Create DSM Client
	H5FDdsmManager * dsmClient = new H5FDdsmManager();
	dsmClient->SetGlobalDebug(0);
	dsmClient->SetCommunicator(comm);
	dsmClient->SetDsmIsServer(0);
	dsmClient->ReadDSMConfigFile();
	dsmClient->CreateDSM();

	// Connect to Server
	dsmClient->ConnectDSM();

	H5FDdsmBuffer * dsmBuffer = dsmClient->GetDSMHandle();

	H5FD_dsm_set_mode(H5FD_DSM_MANUAL_SERVER_UPDATE, dsmBuffer);

	// Get info from remote server
	double remoteMB = dsmBuffer->GetTotalLength() / (1024.0 * 1024.0);
	double numServers = dsmBuffer->GetEndServerId() + 1;
	if (rank == 0)
	{
		std::cout << "DSM server memory size is : " << (int)remoteMB << " MB" << std::endl;
		std::cout << "DSM server process count  : " <<  (int)numServers << std::endl;
	}

	// Create Array
	boost::shared_ptr<XdmfArray> array = XdmfArray::New();
	array->initialize<int>(0);
	array->pushBack(0);
	array->pushBack(1);
	array->pushBack(2);

	// Create DSM Writer and write to DSM space.
	boost::shared_ptr<XdmfHDF5WriterDSM> writer = XdmfHDF5WriterDSM::New("dsm", dsmBuffer);
	array->accept(writer);

	// Read data
	boost::shared_ptr<XdmfArray> readArray = XdmfArray::New();
	readArray->setHeavyDataController(array->getHeavyDataController());
	readArray->read();
	assert(readArray->getSize() == 3);
	assert(readArray->getArrayType() == XdmfArrayType::Int32());

	for(unsigned int i=0; i<readArray->getSize(); ++i)
	{
		assert(array->getValue<int>(i) == readArray->getValue<int>(i));
	}

	// Return to Server
	H5FD_dsm_server_update(dsmBuffer);

	// Closes ports or MPI communicators
	dsmServer->UnpublishDSM();

	delete dsmClient;
	delete dsmServer;

	MPI_Finalize();

	return 0;
}
