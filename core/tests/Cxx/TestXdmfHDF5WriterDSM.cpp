#include <H5FDdsm.h>
#include <H5FDdsmManager.h>
#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfHDF5WriterDSM.hpp"

int main(int argc, char *argv[])
{
  int rank, size, provided, dsmSize = 16;
  MPI_Comm comm = MPI_COMM_WORLD;

  MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &size);

  if (rank == 0) {
    if (provided != MPI_THREAD_MULTIPLE) {
      std::cout << "# MPI_THREAD_MULTIPLE not set, you may need to recompile your "
	<< "MPI distribution with threads enabled" << std::endl;
    }
    else {
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
  if (rank == 0) {
    std::cout << "DSM server memory size is : "
              << (int)remoteMB << " MB" << std::endl;
    std::cout << "DSM server process count  : "
              <<  (int)numServers << std::endl;
  }

  // Create Array
  // Array should be distributed among processes
  shared_ptr<XdmfArray> array = XdmfArray::New();
  array->initialize<int>(0);
  array->pushBack(0);
  array->pushBack(1);
  array->pushBack(2);

  // Create DSM Writer and write to DSM space.
  shared_ptr<XdmfHDF5WriterDSM> writer =
    XdmfHDF5WriterDSM::New("dsm", dsmBuffer);
  array->accept(writer);

  // Read data
  shared_ptr<XdmfArray> readArray = XdmfArray::New();
  readArray->setHeavyDataController(array->getHeavyDataController());
  readArray->read();
  assert(readArray->getSize() == 3);
  assert(readArray->getArrayType() == XdmfArrayType::Int32());

  for(unsigned int i=0; i<readArray->getSize(); ++i) {
    assert(array->getValue<int>(i) == readArray->getValue<int>(i));
  }

  // Wait for everyone to have finished reading
  MPI_Barrier(comm);
  delete dsmManager;

  MPI_Finalize();

  return 0;
}
