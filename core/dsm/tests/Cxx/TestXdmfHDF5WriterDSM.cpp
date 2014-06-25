#include <H5FDdsm.h>
#include <H5FDdsmManager.h>
#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfHDF5ControllerDSM.hpp"
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
  dsmManager->SetMpiComm(comm);
  dsmManager->SetLocalBufferSizeMBytes(dsmSize / size);
  dsmManager->SetLocalBufferSizeMBytes(dsmSize / size);
  dsmManager->SetIsStandAlone(H5FD_DSM_TRUE);
  dsmManager->Create();
  H5FD_dsm_set_manager(dsmManager);

  H5FD_dsm_set_options(H5FD_DSM_LOCK_ASYNCHRONOUS);

  H5FDdsmBuffer * dsmBuffer = dsmManager->GetDsmBuffer();

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
  array->pushBack(rank*3);
  array->pushBack(rank*3 + 1);
  array->pushBack(rank*3 + 2);
  shared_ptr<XdmfHDF5ControllerDSM> controller = 
    XdmfHDF5ControllerDSM::New("dsm.h5",
                               "data",
                               XdmfArrayType::Int32(),
                               std::vector<unsigned int>(1, rank*3),
                               std::vector<unsigned int>(1, 1),
                               std::vector<unsigned int>(1, 3),
                               std::vector<unsigned int>(1, size*3),
                               dsmBuffer);
  array->setHeavyDataController(controller);

  // Create DSM Writer and write to DSM space.
  shared_ptr<XdmfHDF5WriterDSM> writer =
    XdmfHDF5WriterDSM::New("dsm.h5", dsmBuffer);
  writer->setMode(XdmfHeavyDataWriter::Hyperslab);
  array->accept(writer);

  //H5FD_dsm_dump();

  // Release data and read back in to check if we wrote correctly
  array->release();
  array->read();
  std::cout << array->getSize() << "?=" << 3 << std::endl;
  assert(array->getSize() == 3);
  std::cout << array->getValue<int>(0) << "?=" << 3*rank << std::endl;
  assert(array->getValue<int>(0) == 3*rank);
  std::cout << array->getValue<int>(1) << "?=" << ((3*rank) + 1) << std::endl;
  assert(array->getValue<int>(1) == 3*rank + 1);
  std::cout << array->getValue<int>(2) << "?=" << ((3*rank) + 2) << std::endl;
  assert(array->getValue<int>(2) == 3*rank + 2);
  
  MPI_Barrier(comm);
  
  // Adjust controller to read entire dataset onto each processor
  shared_ptr<XdmfHDF5ControllerDSM> fullController = 
    XdmfHDF5ControllerDSM::New("dsm.h5",
                               "data",
                               XdmfArrayType::Int32(),
                               std::vector<unsigned int>(1, 0),
                               std::vector<unsigned int>(1, 1),
                               std::vector<unsigned int>(1, size*3),
                               std::vector<unsigned int>(1, size*3),
                               dsmBuffer);
  array->setHeavyDataController(fullController);
  array->release();
  array->read();
  std::cout << array->getSize() << "?=" << (size * 3) << std::endl;
  assert(array->getSize() == size*3);
  for(int i=0; i<size*3; ++i) {
    std::cout << array->getValue<int>(i) << "?=" << i << std::endl;
    assert(array->getValue<int>(i) == i);
  }

  // Wait for everyone to have finished reading
  MPI_Barrier(comm);
  delete dsmManager;

  MPI_Finalize();

  return 0;
}
