/*
XdmfDSMPython.cpp:
swig -v -c++ -python -o XdmfDSMPython.cpp XdmfDSM.i
*/


%module XdmfDSM
%{

    #include <mpi.h>

    #include <XdmfArray.hpp>
    #include <XdmfArrayReference.hpp>
    #include <XdmfArrayType.hpp>
    #include <XdmfCore.hpp>
    #include <XdmfCoreItemFactory.hpp>
    #include <XdmfCoreReader.hpp>
    #include <XdmfDSM.hpp>
    #include <XdmfDSMBuffer.hpp>
    #include <XdmfDSMCommMPI.hpp>
    #include <XdmfDSMItemFactory.hpp>
    #include <XdmfError.hpp>
    #include <XdmfFunction.hpp>
    #include <XdmfHeavyDataController.hpp>
    #include <XdmfHeavyDataWriter.hpp>
    #include <XdmfHDF5Controller.hpp>
    #include <XdmfHDF5Writer.hpp>
    #include <XdmfHDF5ControllerDSM.hpp>
    #include <XdmfHDF5WriterDSM.hpp>
    #include <XdmfInformation.hpp>
    #include <XdmfItem.hpp>
    #include <XdmfItemProperty.hpp>
    #include <XdmfSharedPtr.hpp>
    #include <XdmfSparseMatrix.hpp>
    #include <XdmfSubset.hpp>
    #include <XdmfTIFFController.hpp>
    #include <XdmfSystemUtils.hpp>
    #include <XdmfVersion.hpp>
    #include <XdmfVisitor.hpp>
    #include <XdmfWriter.hpp>

    #include <ProjectVersion.hpp>
%}

%import XdmfCore.i

// Ignore C Wrappers

// XdmfHDF5ControllerDSM

%ignore XdmfHDF5ControllerDSMNewFromServerBuffer(char * hdf5FilePath,
                                                 char * dataSetPath,
                                                 int type,
                                                 unsigned int * start,
                                                 unsigned int * stride,
                                                 unsigned int * dimensions,
                                                 unsigned int * dataspaceDimensions,
                                                 unsigned int numDims,
                                                 void * dsmBuffer,
                                                 int * status);
%ignore XdmfHDF5ControllerDSMNew(char * hdf5FilePath,
                                 char * dataSetPath,
                                 int type,
                                 unsigned int * start,
                                 unsigned int * stride,
                                 unsigned int * dimensions,
                                 unsigned int * dataspaceDimensions,
                                 unsigned int numDims,
                                 MPI_Comm comm,
                                 unsigned int bufferSize,
                                 int startCoreIndex,
                                 int endCoreIndex,
                                 char * applicationName,
                                 int * status);
%ignore XdmfHDF5ControllerDSMNewPaged(char * hdf5FilePath,
                                      char * dataSetPath,
                                      int type,
                                      unsigned int * start,
                                      unsigned int * stride,
                                      unsigned int * dimensions,
                                      unsigned int * dataspaceDimensions,
                                      unsigned int numDims,
                                      MPI_Comm comm,
                                      unsigned int bufferSize,
                                      unsigned int blockSize,
                                      int startCoreIndex,
                                      int endCoreIndex,
                                      char * applicationName,
                                      int * status);
%ignore XdmfHDF5ControllerDSMGetServerBuffer(XDMFHDF5CONTROLLERDSM * controller);
%ignore XdmfHDF5ControllerDSMGetServerMode(XDMFHDF5CONTROLLERDSM * controller);
%ignore XdmfHDF5ControllerDSMGetWorkerComm(XDMFHDF5CONTROLLERDSM * controller);
%ignore XdmfHDF5ControllerDSMSetServerBuffer(XDMFHDF5CONTROLLERDSM * controller, XDMFDSMBUFFER * newBuffer);
%ignore XdmfHDF5ControllerDSMSetServerMode(XDMFHDF5CONTROLLERDSM * controller, int newMode);
%ignore XdmfHDF5ControllerDSMSetWorkerComm(XDMFHDF5CONTROLLERDSM * controller, MPI_Comm comm, int * status);
%ignore XdmfHDF5ControllerDSMStopDSM(XDMFHDF5CONTROLLERDSM * controller, int * status);
%ignore XdmfHDF5ControllerDSMRestartDSM(XDMFHDF5CONTROLLERDSM * controller, int * status);
// XdmfHDF5ControlerDSM inherited from XdmfHDF5Controler
%ignore XdmfHDF5ControlerDSMGetDataSetPath(XDMFHDF5CONTROLLERDSM * controller);
// XdmfHDF5ControlerDSM inherited from XdmfHeavyDataController
%ignore XdmfHDF5ControlerDSMFree(XDMFHDF5CONTROLLERDSM * item);
%ignore XdmfHDF5ControlerDSMGetDataspaceDimensions(XDMFHDF5CONTROLLERDSM * controller);
%ignore XdmfHDF5ControlerDSMGetDimensions(XDMFHDF5CONTROLLERDSM * controller);
%ignore XdmfHDF5ControlerDSMGetFilePath(XDMFHDF5CONTROLLERDSM * controller);
%ignore XdmfHDF5ControlerDSMGetName(XDMFHDF5CONTROLLERDSM * controller);
%ignore XdmfHDF5ControlerDSMGetNumberDimensions(XDMFHDF5CONTROLLERDSM * controller);
%ignore XdmfHDF5ControlerDSMGetSize(XDMFHDF5CONTROLLERDSM * controller);
%ignore XdmfHDF5ControlerDSMGetStart(XDMFHDF5CONTROLLERDSM * controller);
%ignore XdmfHDF5ControlerDSMGetStride(XDMFHDF5CONTROLLERDSM * controller);
%ignore XdmfHDF5ControlerDSMSetArrayOffset(XDMFHDF5CONTROLLERDSM * controller, unsigned int newOffset);
%ignore XdmfHDF5ControlerDSMGetArrayOffset(XDMFHDF5CONTROLLERDSM * controller);
%ignore XdmfHDF5ControlerDSMGetType(XDMFHDF5CONTROLLERDSM * controller, int * status);
%ignore XdmfHDF5ControlerDSMRead(XDMFHDF5CONTROLLERDSM * controller, void * array, int * status);

// XdmfHDF5WriterDSM

%ignore XdmfHDF5WriterDSMNewFromServerBuffer(char * filePath,
                                             void * dsmBuffer,
                                             int * status);
%ignore XdmfHDF5WriterDSMNew(char * filePath,
                             MPI_Comm comm,
                             unsigned int bufferSize,
                             int startCoreIndex,
                             int endCoreIndex,
                             int * status);
%ignore XdmfHDF5WriterDSMNewPaged(char * filePath,
                                  MPI_Comm comm,
                                  unsigned int bufferSize,
                                  unsigned int blockSize,
                                  int startCoreIndex,
                                  int endCoreIndex,
                                  int * status);
%ignore XdmfHDF5WriterDSMNewConnectRequired(char * filePath,
                                            MPI_Comm comm,
                                            char * applicationName,
                                            int * status);
%ignore XdmfHDF5WriterDSMGetDataSetSize(XDMFHDF5WRITERDSM * writer, char * fileName, char * dataSetName);
%ignore XdmfHDF5WriterDSMGetServerBuffer(XDMFHDF5WRITERDSM * writer);
%ignore XdmfHDF5WriterDSMGetServerMode(XDMFHDF5WRITERDSM * writer);
%ignore XdmfHDF5WriterDSMGetWorkerComm(XDMFHDF5WRITERDSM * writer);
%ignore XdmfHDF5WriterDSMSetServerBuffer(XDMFHDF5WRITERDSM * writer, XDMFDSMBUFFER * newBuffer);
%ignore XdmfHDF5WriterDSMSetServerMode(XDMFHDF5WRITERDSM * writer, int newMode);
%ignore XdmfHDF5WriterDSMSetWorkerComm(XDMFHDF5WRITERDSM * writer, MPI_Comm comm, int * status);
%ignore XdmfHDF5WriterDSMStopDSM(XDMFHDF5WRITERDSM * writer, int * status);
%ignore XdmfHDF5WriterDSMRestartDSM(XDMFHDF5WRITERDSM * writer, int * status);
%ignore XdmfHDF5WriterDSMWaitRelease(XDMFHDF5WRITERDSM * writer, char * fileName, char * datasetName, int code);
%ignore XdmfHDF5WriterDSMWaitOn(XDMFHDF5WRITERDSM * writer, char * fileName, char * datasetName);
// XdmfHDF5WriterDSM inherited from XdmfHDF5Writer
%ignore XdmfHDF5WriterCloseFile(XDMFHDF5WRITER * writer, int * status);
%ignore XdmfHDF5WriterGetChunkSize(XDMFHDF5WRITER * writer, int * status);
%ignore XdmfHDF5WriterOpenFile(XDMFHDF5WRITER * writer, int * status);
%ignore XdmfHDF5WriterSetChunkSize(XDMFHDF5WRITER * writer, unsigned int chunkSize, int * status);
// XdmfHDF5WriterDSM inherited from XdmfHeavyDataWriter
%ignore XdmfHDF5WriterDSMFree(XDMFHDF5WRITERDSM * item);
%ignore XdmfHDF5WriterDSMGetAllowSetSplitting(XDMFHDF5WRITERDSM * writer);
%ignore XdmfHDF5WriterDSMGetFileIndex(XDMFHDF5WRITERDSM * writer);
%ignore XdmfHDF5WriterDSMGetFileOverhead(XDMFHDF5WRITERDSM * writer);
%ignore XdmfHDF5WriterDSMGetFilePath(XDMFHDF5WRITERDSM * writer);
%ignore XdmfHDF5WriterDSMGetFileSizeLimit(XDMFHDF5WRITERDSM * writer);
%ignore XdmfHDF5WriterDSMGetMode(XDMFHDF5WRITERDSM * writer);
%ignore XdmfHDF5WriterDSMGetReleaseData(XDMFHDF5WRITERDSM * writer);
%ignore XdmfHDF5WriterDSMSetAllowSetSplitting(XDMFHDF5WRITERDSM * writer, int newAllow);
%ignore XdmfHDF5WriterDSMSetFileIndex(XDMFHDF5WRITERDSM * writer, int newIndex);
%ignore XdmfHDF5WriterDSMSetFileSizeLimit(XDMFHDF5WRITERDSM * writer, int newSize);
%ignore XdmfHDF5WriterDSMSetMode(XDMFHDF5WRITERDSM * writer, int mode, int * status);
%ignore XdmfHDF5WriterDSMSetReleaseData(XDMFHDF5WRITERDSM * writer, int releaseData);

// XdmfDSMCommMPI

%ignore XdmfDSMCommMPINew();
%ignore XdmfDSMCommMPIFree(XDMFDSMCOMMMPI * item);
%ignore XdmfDSMCommMPIAccept(XDMFDSMCOMMMPI * dsmComm, unsigned int numConnections, int * status);
%ignore XdmfDSMCommMPIClosePort(XDMFDSMCOMMMPI * dsmComm, int * status);
%ignore XdmfDSMCommMPIConnect(XDMFDSMCOMMMPI * dsmComm, int * status);
%ignore XdmfDSMCommMPIDisconnect(XDMFDSMCOMMMPI * dsmComm, int * status);
%ignore XdmfDSMCommMPIDupComm(XDMFDSMCOMMMPI * dsmComm, MPI_Comm comm, int * status);
%ignore XdmfDSMCommMPIDupInterComm(XDMFDSMCOMMMPI * dsmComm, MPI_Comm comm, int * status);
%ignore XdmfDSMCommMPIGetApplicationName(XDMFDSMCOMMMPI * dsmComm);
%ignore XdmfDSMCommMPIGetDsmFileName(XDMFDSMCOMMMPI * dsmComm);
%ignore XdmfDSMCommMPIGetDsmPortName(XDMFDSMCOMMMPI * dsmComm);
%ignore XdmfDSMCommMPIGetDsmProcessStructure(XDMFDSMCOMMMPI * dsmComm,
                                             char ** names,
                                             unsigned int * coreCount,
                                             int * numApplications);
%ignore XdmfDSMCommMPIGetId(XDMFDSMCOMMMPI * dsmComm);
%ignore XdmfDSMCommMPIGetInterComm(XDMFDSMCOMMMPI * dsmComm);
%ignore XdmfDSMCommMPIGetInterCommType(XDMFDSMCOMMMPI * dsmComm);
%ignore XdmfDSMCommMPIGetInterId(XDMFDSMCOMMMPI * dsmComm);
%ignore XdmfDSMCommMPIGetInterSize(XDMFDSMCOMMMPI * dsmComm);
%ignore XdmfDSMCommMPIGetIntraComm(XDMFDSMCOMMMPI * dsmComm);
%ignore XdmfDSMCommMPIGetIntraSize(XDMFDSMCOMMMPI * dsmComm);
%ignore XdmfDSMCommMPIGetUseEnvFileName(XDMFDSMCOMMMPI * dsmComm);
%ignore XdmfDSMCommMPIInit(XDMFDSMCOMMMPI * dsmComm, int * status);
%ignore XdmfDSMCommMPIOpenPort(XDMFDSMCOMMMPI * dsmComm, int * status);
%ignore XdmfDSMCommMPIReadDsmPortName(XDMFDSMCOMMMPI * dsmComm);
%ignore XdmfDSMCommMPISetApplicationName(XDMFDSMCOMMMPI * dsmComm, char * newName);
%ignore XdmfDSMCommMPISetDsmFileName(XDMFDSMCOMMMPI * dsmComm, char * filename);
%ignore XdmfDSMCommMPISetDsmPortName(XDMFDSMCOMMMPI * dsmComm, char * hostName);
%ignore XdmfDSMCommMPISetUseEnvFileName(XDMFDSMCOMMMPI * dsmComm, int status);

// XdmfDSMBuffer

%ignore XdmfDSMBufferNew();
%ignore XdmfDSMBufferFree(XDMFDSMBUFFER * item);
%ignore XdmfDSMBufferAddressToId(XDMFDSMBUFFER * buffer, int Address, int * status);
%ignore XdmfDSMBufferBroadcastComm(XDMFDSMBUFFER * buffer, int *comm, int root, int * status);
%ignore XdmfDSMBufferBufferService(XDMFDSMBUFFER * buffer, int *returnOpcode, int * status);
%ignore XdmfDSMBufferBufferServiceLoop(XDMFDSMBUFFER * buffer, int *returnOpcode, int * status);
%ignore XdmfDSMBufferConfigureUniform(XDMFDSMBUFFER * buffer,
                                      XDMFDSMCOMMMPI * Comm,
                                      long Length,
                                      int StartId,
                                      int EndId,
                                      long aBlockLength,
                                      int random,
                                      int * status);
%ignore XdmfDSMBufferConnect(XDMFDSMBUFFER * buffer, int persist, int * status);
%ignore XdmfDSMBufferCreate(XDMFDSMBUFFER * buffer, int comm, int startId, int endId, int * status);
%ignore XdmfDSMBufferDisconnect(XDMFDSMBUFFER * buffer, int * status);
%ignore XdmfDSMBufferGet(XDMFDSMBUFFER * buffer, long Address, long aLength, void * Data, int * status);
%ignore XdmfDSMBufferGetAddressRangeForId(XDMFDSMBUFFER * buffer, int Id, int * Start, int * End, int * status);
%ignore XdmfDSMBufferGetBlockLength(XDMFDSMBUFFER * buffer);
%ignore XdmfDSMBufferGetComm(XDMFDSMBUFFER * buffer);
%ignore XdmfDSMBufferGetDataPointer(XDMFDSMBUFFER * buffer);
%ignore XdmfDSMBufferGetDsmType(XDMFDSMBUFFER * buffer);
%ignore XdmfDSMBufferGetEndAddress(XDMFDSMBUFFER * buffer);
%ignore XdmfDSMBufferGetEndServerId(XDMFDSMBUFFER * buffer);
%ignore XdmfDSMBufferGetInterCommType(XDMFDSMBUFFER * buffer);
%ignore XdmfDSMBufferGetIsConnected(XDMFDSMBUFFER * buffer);
%ignore XdmfDSMBufferGetIsServer(XDMFDSMBUFFER * buffer);
%ignore XdmfDSMBufferGetLength(XDMFDSMBUFFER * buffer);
%ignore XdmfDSMBufferGetLocalBufferSizeMBytes(XDMFDSMBUFFER * buffer);
%ignore XdmfDSMBufferGetResizeFactor(XDMFDSMBUFFER * buffer);
%ignore XdmfDSMBufferGetStartAddress(XDMFDSMBUFFER * buffer);
%ignore XdmfDSMBufferGetStartServerId(XDMFDSMBUFFER * buffer);
%ignore XdmfDSMBufferGetTotalLength(XDMFDSMBUFFER * buffer);
%ignore XdmfDSMBufferProbeCommandHeader(XDMFDSMBUFFER * buffer, int * comm, int * status);
%ignore XdmfDSMBufferPut(XDMFDSMBUFFER * buffer, long Address, long aLength, void * Data, int * status);
%ignore XdmfDSMBufferReceiveAcknowledgment(XDMFDSMBUFFER * buffer,
                                           int source,
                                           int * data,
                                           int tag,
                                           int comm,
                                           int * status);
%ignore XdmfDSMBufferReceiveCommandHeader(XDMFDSMBUFFER * buffer,
                                          int * opcode,
                                          int * source,
                                          int * address,
                                          int * aLength,
                                          int comm,
                                          int remoteSource,
                                          int * status);
%ignore XdmfDSMBufferReceiveData(XDMFDSMBUFFER * buffer,
                                 int source,
                                 char * data,
                                 int aLength,
                                 int tag,
                                 int aAddress,
                                 int comm,
                                 int * status);
%ignore XdmfDSMBufferReceiveInfo(XDMFDSMBUFFER * buffer,
                                 int * status);
%ignore XdmfDSMBufferSendAccept(XDMFDSMBUFFER * buffer, unsigned int numConnects);
%ignore XdmfDSMBufferSendAcknowledgment(XDMFDSMBUFFER * buffer,
                                        int dest,
                                        int data,
                                        int tag,
                                        int comm,
                                        int * status);
%ignore XdmfDSMBufferSendCommandHeader(XDMFDSMBUFFER * buffer,
                                       int opcode,
                                       int dest,
                                       int address,
                                       int aLength,
                                       int comm,
                                       int * status);
%ignore XdmfDSMBufferSendData(XDMFDSMBUFFER * buffer,
                              int dest,
                              char * data,
                              int aLength,
                              int tag,
                              int aAddress,
                              int comm,
                              int * status);
%ignore XdmfDSMBufferSendDone(XDMFDSMBUFFER * buffer, int * status);
%ignore XdmfDSMBufferSendInfo(XDMFDSMBUFFER * buffer, int * status);
%ignore XdmfDSMBufferSetBlockLength(XDMFDSMBUFFER * buffer, long newBlock);
%ignore XdmfDSMBufferSetComm(XDMFDSMBUFFER * buffer, XDMFDSMCOMMMPI * newComm);
%ignore XdmfDSMBufferSetDsmType(XDMFDSMBUFFER * buffer, int newDsmType);
%ignore XdmfDSMBufferSetInterCommType(XDMFDSMBUFFER * buffer, int newType);
%ignore XdmfDSMBufferSetIsConnected(XDMFDSMBUFFER * buffer, int newStatus);
%ignore XdmfDSMBufferSetIsServer(XDMFDSMBUFFER * buffer, int newIsServer);
%ignore XdmfDSMBufferSetLocalBufferSizeMBytes(XDMFDSMBUFFER * buffer, unsigned int newSize);
%ignore XdmfDSMBufferSetResizeFactor(XDMFDSMBUFFER * buffer, double newFactor);
%ignore XdmfDSMBufferWaitRelease(XDMFDSMBUFFER * buffer, char * filename, char * datasetname, int code);
%ignore XdmfDSMBufferWaitOn(XDMFDSMBUFFER * buffer, char * filename, char * datasetname);

#ifdef SWIGJAVA

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("XdmfDSMJava");
        }
        catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load for" +
                               "XdmfDSMJava\n" + e);
            System.exit(1);
        }
    }
%}


#endif /* SWIGJAVA */

#ifdef SWIGPYTHON

%pythoncode {
    from XdmfCore import *
}

%include mpi4py/mpi4py.i

%mpi4py_typemap(Comm, MPI_Comm);

%include <typemaps.i>
%apply int & INOUT {int & data };

%extend XdmfDSMBuffer {
    bool __eq__(const XdmfDSMBuffer * item) {
        return $self == item;
    }
};

%extend XdmfDSMCommMPI {
    bool __eq__(const XdmfDSMCommMPI * item) {
        return $self == item;
    }
};

%extend XdmfHeavyDataController {
    static shared_ptr<XdmfHDF5ControllerDSM> XdmfHDF5ControllerDSMCast(PyObject * obj)
    {
      void * resultPointer = 0;
      swig_type_info * returnType = SWIG_TypeQuery("_p_boost__shared_ptrT_XdmfHeavyDataController_t");
      SWIG_ConvertPtr(obj, &resultPointer, returnType, 0);
      shared_ptr<XdmfHeavyDataController> * returnControllerPointer =
        reinterpret_cast<shared_ptr<XdmfHeavyDataController> *>(resultPointer);
      shared_ptr<XdmfHeavyDataController> returnController = returnControllerPointer[0];
      if (shared_ptr<XdmfHDF5ControllerDSM> returnHDF5Controller = shared_dynamic_cast<XdmfHDF5ControllerDSM>(returnController)) {
        return returnHDF5Controller;
      }
      else {
        XdmfError::message(XdmfError::FATAL, "Error: Attempting to cast a non HDF5 Controller to HDF5");
        return shared_ptr<XdmfHDF5ControllerDSM>();
      }
    }
};

#endif /* SWIGPYTHON */



// Shared Pointer Templates
%shared_ptr(XdmfHDF5ControllerDSM)
%shared_ptr(XdmfHDF5WriterDSM)
%shared_ptr(XdmfDSMItemFactory)

%include XdmfDSM.hpp
%include XdmfHDF5ControllerDSM.hpp
%include XdmfHDF5WriterDSM.hpp
%include XdmfDSMBuffer.hpp
%include XdmfDSMCommMPI.hpp
%include XdmfDSMItemFactory.hpp
