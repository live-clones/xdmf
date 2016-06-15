#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfDomain.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfTopology.hpp"
#include "XdmfUnstructuredGrid.hpp"
#include "XdmfReader.hpp"
#include "XdmfWriter.hpp"
#include "XdmfHDF5WriterDSM.hpp"
#include "XdmfHDF5ControllerDSM.hpp"

int main(int argc, char *argv[])
{
        int size, id, dsmSize;
        dsmSize = 64;//The total size of the DSM being created
        MPI_Comm comm = MPI_COMM_WORLD;

        MPI_Init(&argc, &argv);

        MPI_Comm_rank(comm, &id);
        MPI_Comm_size(comm, &size);


        std::vector<unsigned int> outputVector;

        shared_ptr<XdmfArray> testArray = XdmfArray::New();
        testArray->initialize<int>(0);

        shared_ptr<XdmfArray> testArray2 = XdmfArray::New();
        testArray2->initialize<int>(0);

        std::string newPath = "dsm";
        std::string newSetPath = "dataspace";
        std::string secondSetPath = "data2";

        // Change this to determine the number of cores used as servers
        unsigned int numServersCores = 2;
        // Change this to determine the size of the arrays generated when initializing
        unsigned int writeArraySize = 4;

        std::vector<unsigned int> writeStartVector;
        std::vector<unsigned int> writeStrideVector;
        std::vector<unsigned int> writeCountVector;
        std::vector<unsigned int> writeDataSizeVector;

        std::vector<unsigned int> readStartVector;
        std::vector<unsigned int> readStrideVector;
        std::vector<unsigned int> readCountVector;
        std::vector<unsigned int> readDataSizeVector;

        std::vector<unsigned int> readOutputCountVector;

        shared_ptr<XdmfArray> readArray = XdmfArray::New();
        readArray->initialize<int>(0);

        shared_ptr<XdmfArray> readArray2 = XdmfArray::New();
        readArray2->initialize<int>(0);

        shared_ptr<XdmfHDF5ControllerDSM> readController;
        shared_ptr<XdmfHDF5ControllerDSM> readOutputController;
        shared_ptr<XdmfHDF5ControllerDSM> writeController;

        shared_ptr<XdmfHDF5ControllerDSM> readController2;
        shared_ptr<XdmfHDF5ControllerDSM> readOutputController2;
        shared_ptr<XdmfHDF5ControllerDSM> writeController2;

        MPI_Comm workerComm;

        MPI_Group workers, dsmgroup;

        MPI_Comm_group(comm, &dsmgroup);
        int * ServerIds = (int *)calloc((numServersCores), sizeof(int));
        unsigned int index = 0;
        for(int i=size-numServersCores ; i <= size-1 ; ++i)
        {
                ServerIds[index++] = i;
        }

        MPI_Group_excl(dsmgroup, index, ServerIds, &workers);
        int testval = MPI_Comm_create(comm, workers, &workerComm);
        cfree(ServerIds);

        shared_ptr<XdmfHDF5WriterDSM> exampleWriter = XdmfHDF5WriterDSM::New(newPath, comm, dsmSize/numServersCores, 1, 1);

        exampleWriter->setMode(XdmfHeavyDataWriter::Hyperslab);
        exampleWriter->setReleaseData(true);

        shared_ptr<XdmfWriter> fileWriter = XdmfWriter::New("dsmfile.xmf", exampleWriter);

        shared_ptr<XdmfReader> fileReader = XdmfReader::New();

        //split out sub-comm for the worker cores
        //server cores will not progress to this point until after the servers are done running

        if (id != 1)
        {

          shared_ptr<XdmfDomain> domain = XdmfDomain::New();

          shared_ptr<XdmfUnstructuredGrid> ungrid = XdmfUnstructuredGrid::New();

          domain->insert(ungrid);

          shared_ptr<XdmfGeometry> geo = XdmfGeometry::New();

          shared_ptr<XdmfTopology> topo = XdmfTopology::New();

          shared_ptr<XdmfAttribute> array = XdmfAttribute::New();

          ungrid->setTopology(topo);
          ungrid->setGeometry(geo);

          ungrid->insert(array);
          
          for (unsigned int i = 0; i < 1000; ++i)
          {
            geo->pushBack(i);
            topo->pushBack(i);
            array->pushBack(i);
          }

          if (id == 0)
          {
            exampleWriter->getServerBuffer()->GetComm()->OpenPort();
          }

          domain->accept(exampleWriter);
          if (id == 0)
          {
            domain->accept(fileWriter);
          }

          exampleWriter->getServerBuffer()->GetComm()->Barrier(XDMF_DSM_INTRA_COMM);

/*
          if (id == 2)
          {
            shared_ptr<XdmfArray> readArray = shared_dynamic_cast<XdmfArray>(fileReader->read("dsmfile.xmf"));
          }
*/

        }

        if (id == 0)
        {
           exampleWriter->getServerBuffer()->SendAccept(1);
        }

        MPI_Barrier(exampleWriter->getServerBuffer()->GetComm()->GetInterComm());

        if (id == 0)
        {
           exampleWriter->getServerBuffer()->GetComm()->ClosePort();
        }

        MPI_Barrier(comm);

        MPI_Finalize();

        return 0;
}
