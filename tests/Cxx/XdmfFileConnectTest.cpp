#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>
#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfDomain.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfTopology.hpp"
#include "XdmfUnstructuredGrid.hpp"
#include "XdmfReader.hpp"
#include "XdmfWriter.hpp"
#include "XdmfDSMDriver.hpp"
#include "XdmfDSMBuffer.hpp"
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

        // The read should cause a connect to the dsm of the other process.
        #ifdef  _WIN32
                Sleep(500)
        #else
                sleep(5);
        #endif

        std::string configFileName = "dsmfile.xmf";

        std::ifstream testStream;
        const char * configFileNamePtr = configFileName.c_str();
        testStream.open(configFileNamePtr);

        while (!testStream.good()) { // Wait for the config file to be generated
          testStream.close();
          #ifdef  _WIN32
            Sleep(500)
          #else
            sleep(5);
          #endif
          testStream.open(configFileNamePtr);
        }

        XdmfDSMBuffer * dsmBuffer = new XdmfDSMBuffer();
        dsmBuffer->SetComm(new XdmfDSMCommMPI());
        dsmBuffer->GetComm()->DupComm(MPI_COMM_WORLD);
        dsmBuffer->GetComm()->DupInterComm(MPI_COMM_WORLD);

        dsmBuffer->GetComm()->Init();

        xdmf_dsm_set_manager(dsmBuffer);

        shared_ptr<XdmfReader> fileReader = XdmfReader::New();

        assert(fileReader);

        shared_ptr<XdmfDomain> readDomain = shared_dynamic_cast<XdmfDomain>(fileReader->read("dsmfile.xmf"));

        shared_ptr<XdmfUnstructuredGrid> readUnGrid = readDomain->getUnstructuredGrid(0);

        assert(readUnGrid);

        shared_ptr<XdmfGeometry> readGeo = readUnGrid->getGeometry();

        assert(readGeo);

        readGeo->read();

        for (unsigned int i = 0; i < readGeo->getSize(); ++i)
        {
          assert(readGeo->getValue<unsigned int>(i) == i);
        }

        shared_ptr<XdmfTopology> readTopo = readUnGrid->getTopology();

        assert(readTopo);

        readTopo->read();

        for (unsigned int i = 0; i < readTopo->getSize(); ++i)
        {
          assert(readTopo->getValue<unsigned int>(i) == i);
        }

        shared_ptr<XdmfAttribute> readAttr = readUnGrid->getAttribute(0);

        assert(readAttr);

        readAttr->read();

        for (unsigned int i = 0; i < readAttr->getSize(); ++i)
        {
          assert(readAttr->getValue<unsigned int>(i) == i);
        }

        if (id == 0)
        {
          shared_dynamic_cast<XdmfHDF5ControllerDSM>(readAttr->getHeavyDataController(0))->stopDSM();
        }

        MPI_Barrier(shared_dynamic_cast<XdmfHDF5ControllerDSM>(readAttr->getHeavyDataController(0))->getServerBuffer()->GetComm()->GetInterComm());

        MPI_Barrier(comm);

        MPI_Finalize();

        return 0;
}
