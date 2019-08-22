print("Accept")

from mpi4py.MPI import *
from Xdmf import *

if __name__ == "__main__":
        if get_vendor()[0] == 'Open MPI':
          print("Interprogram DSM does not work properly with OpenMPI")
        else:
          #//initMPI begin

          dsmSize = 64
          comm = COMM_WORLD

          id = comm.Get_rank()
          size = comm.Get_size()

          newPath = "dsm";
          numServersCores = size - 1;
          numConnections = 2

          exampleWriter = XdmfHDF5WriterDSM.New(newPath, comm, dsmSize/numServersCores, 1, 1, "Accept");

          if id == 0:
                  #//initMPI end

                  #//GetDsmFileName begin

                  connectionFileName = exampleWriter.getServerBuffer().GetComm().GetDsmFileName()

                  #//GetDsmFileName end

                  #//SetDsmFileName begin

                  exampleWriter.getServerBuffer().GetComm().SetDsmFileName(connectionFileName)

                  #//SetDsmFileName end

                  #//OpenPort begin

                  exampleWriter.getServerBuffer().GetComm().OpenPort()

                  #//OpenPort end

                  #//SendAccept begin

                  exampleWriter.getServerBuffer().SendAccept(numConnections)

                  #//SendAccept end

                  '''

                  #//manualAccept begin

                  # Notify the server cores to accept connections
                  for i in range(exampleWriter.getServerBuffer().StartServerId, exampleWriter.getServerBuffer().EndServerId+1):
                          if i != exampleWriter.getServerBuffer().Comm.GetId():
                                  exampleWriter.getServerBuffer().SendCommandHeader(XDMF_DSM_ACCEPT, i, 0, 0, XDMF_DSM_INTER_COMM)
                                  exampleWriter.getServerBuffer().SendAcknowledgment(i, numConnections, XDMF_DSM_EXCHANGE_TAG, XDMF_DSM_INTER_COMM)
                  # Accept connections
                  exampleWriter.getServerBuffer().Comm.Accept(numConnections)
                  # Distribute current DSM status
                  exampleWriter.getServerBuffer().SendInfo()

                  #//manualAccept end

                  '''

                  #//finishwork begin

                  exampleWriter.getServerBuffer().GetComm().GetIntraComm().Barrier()

          exampleWriter.getServerBuffer().GetComm().GetInterComm().Barrier()

          #//finishwork end

          #//ClosePort begin

	  if id == 0:
	          exampleWriter.getServerBuffer().GetComm().ClosePort()

          #//ClosePort end
