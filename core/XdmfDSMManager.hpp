/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfDSMManager.hpp                                                  */
/*                                                                           */
/*  Author:                                                                  */
/*     Andrew Burns                                                          */
/*     andrew.j.burns2@us.army.mil                                           */
/*     US Army Research Laboratory                                           */
/*     Aberdeen Proving Ground, MD                                           */
/*                                                                           */
/*     Copyright @ 2013 US Army Research Laboratory                          */
/*     All Rights Reserved                                                   */
/*     See Copyright.txt for details                                         */
/*                                                                           */
/*     This software is distributed WITHOUT ANY WARRANTY; without            */
/*     even the implied warranty of MERCHANTABILITY or FITNESS               */
/*     FOR A PARTICULAR PURPOSE.  See the above copyright notice             */
/*     for more information.                                                 */
/*                                                                           */
/*****************************************************************************/

/*=========================================================================
  This code is derived from an earlier work and is distributed
  with permission from, and thanks to ...
=========================================================================*/

/*=========================================================================

  Project                 : H5FDdsm
  Module                  : H5FDdsmManager.h

  Authors:
     John Biddiscombe     Jerome Soumagne
     biddisco@cscs.ch     soumagne@cscs.ch

  Copyright (C) CSCS - Swiss National Supercomputing Centre.
  You may use modify and and distribute this code freely providing
  1) This copyright notice appears on all copies of source code
  2) An acknowledgment appears with any substantial usage of the code
  3) If this code is contributed to any other open source project, it
  must not be reformatted such that the indentation, bracketing or
  overall style is modified significantly.

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

  This work has received funding from the European Community's Seventh
  Framework Programme (FP7/2007-2013) under grant agreement 225967 âxtMuSEâOB
=========================================================================*/

#ifndef XDMFDSMMANAGER_HPP_
#define XDMFDSMMANAGER_HPP_

// Forward Declarations

// Includes
#include <XdmfDSMBuffer.hpp>
#include <XdmfDSMCommMPI.hpp>
#include <XdmfCore.hpp>
#include <mpi.h>


/**
 * @brief Holds communicators for interacting with H5FD dsm.
 *
 * XdmfDSMManager takes the place of the H5FDdsmManager defined in H5FD.
 * It is primarily for allowing the XdmfDSM to interact with HDF5 dsm without threads.
 */
class XDMFCORE_EXPORT XdmfDSMManager {

public:

  XdmfDSMManager();
  ~XdmfDSMManager();

  /**
   * Attempts to connect the buffer to the port that is currently set.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfDSMNoThread.cpp
   * @skipline size
   * @until MPI_Comm_size
   * @skipline exampleController
   * @until size-1
   * @skipline if
   * @until {
   * @skipline XdmfDSMManager
   * @skipline exampleController
   * @skipline XdmfDSMBuffer
   * @skipline exampleController
   * @skipline XdmfDSMCommMPI
   * @skipline connectingGroup
   * @until false
   * @skipline }
   * @skip (connectingGroup
   * @until }
   * @skipline (connectingGroup
   * @until }
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until exampleWriter
   * @skipline if
   * @skipline exampleManager
   * @skipline exampleBuffer
   * @skipline exampleDSMComm
   * @skipline connectingGroup
   * @until False
   * @skip Disconnect
   * @skipline (connectingGroup
   * @until Disconnect
   *
   * @param       persist         Whether to try to connect repeatedly
   */
  void Connect(bool persist = false);

  /**
   * Creates an internal buffer based on the information provided to the Manager already.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfDSMNoThread.cpp
   * @skipline size
   * @until MPI_Comm_size
   * @skipline exampleWriter
   * @skipline if
   * @until {
   * @skipline exampleManager
   * @skipline exampleWriter
   * @skipline Create
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until exampleWriter
   * @skipline if
   * @skipline exampleManager
   * @skipline Create
   *
   * @param       startId         The index of the first server node
   * @param       endId           The index of the last server node
   */
  void Create(int startId = -1, int endId = -1);

  /**
   * Disposes of the current DSM data buffer.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfDSMNoThread.cpp
   * @skipline size
   * @until MPI_Comm_size
   * @skipline exampleWriter
   * @skipline if
   * @until {
   * @skipline exampleManager
   * @skipline exampleWriter
   * @skipline Destroy
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until exampleWriter
   * @skipline if
   * @skipline exampleManager
   * @skipline Destroy
   *
   */
  void Destroy();

  /**
   * Disconnects the buffer from the port it was connected to.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfDSMNoThread.cpp
   * @skipline size
   * @until MPI_Comm_size
   * @skipline exampleController
   * @until size-1
   * @skipline if
   * @until {
   * @skipline XdmfDSMManager
   * @skipline exampleController
   * @skipline XdmfDSMBuffer
   * @skipline exampleController
   * @skipline XdmfDSMCommMPI
   * @skipline connectingGroup
   * @until false
   * @skipline }
   * @skip (connectingGroup
   * @until }
   * @skipline (connectingGroup
   * @until }
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until exampleWriter
   * @skipline if
   * @skipline exampleManager
   * @skipline exampleBuffer
   * @skipline exampleDSMComm
   * @skipline connectingGroup
   * @until False
   * @skip Disconnect
   * @skipline (connectingGroup
   * @until Disconnect
   *
   */
  void Disconnect();

  /**
   * gets the block length for the DSM data buffer.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfDSMNoThread.cpp
   * @skipline size
   * @until MPI_Comm_size
   * @skipline exampleWriter
   * @skipline if
   * @until {
   * @skipline exampleManager
   * @skipline exampleWriter
   * @skipline GetBlockLength
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until exampleWriter
   * @skipline if
   * @skipline exampleManager
   * @skipline exampleBlockLength
   *
   * @return      The size of the blocks currently being used
   */
  long GetBlockLength();

  /**
   * Gets the manager's internal buffer.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfDSMNoThread.cpp
   * @skipline size
   * @until MPI_Comm_size
   * @skipline exampleWriter
   * @skipline if
   * @until {
   * @skipline exampleManager
   * @skipline exampleWriter
   * @skipline exampleBuffer
   * @skipline GetDSMBuffer
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until exampleWriter
   * @skipline if
   * @skipline exampleManager
   * @skip exampleBuffer
   * @skipline GetDSMBuffer
   *
   * @return      The buffer that the manager is using as its internal buffer
   */
  XdmfDSMBuffer * GetDsmBuffer();

  /**
   * Gets the current type of DSM that the Manager is using.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfDSMNoThread.cpp
   * @skipline size
   * @until MPI_Comm_size
   * @skipline exampleWriter
   * @skipline if
   * @until {
   * @skipline exampleManager
   * @skipline exampleWriter
   * @skipline GetDsmType
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until exampleWriter
   * @skipline if
   * @skipline exampleManager
   * @skipline exampleType
   *
   * @return      They type of DSM being used
   */
  int GetDsmType();

  /**
   * Gets the type of intercomm that the manager is currently using.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfDSMNoThread.cpp
   * @skipline size
   * @until MPI_Comm_size
   * @skipline exampleWriter
   * @skipline if
   * @until {
   * @skipline exampleManager
   * @skipline exampleWriter
   * @skipline GetInterCommType
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until exampleWriter
   * @skipline if
   * @skipline exampleManager
   * @skipline exampleCommType
   *
   * @return      They type of intercomm currently being used
   */
  int GetInterCommType();

  /**
   * Gets if the Buffer is connected via intercomm.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfDSMNoThread.cpp
   * @skipline size
   * @until MPI_Comm_size
   * @skipline exampleWriter
   * @skipline if
   * @until {
   * @skipline exampleManager
   * @skipline exampleWriter
   * @skipline GetIsConnected
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until exampleWriter
   * @skipline if
   * @skipline exampleManager
   * @skipline GetIsConnected
   *
   * @return      Whether the Buffer is connected
   */
  bool GetIsConnected();

  /**
   * Gets whether the Manager is managing a server or not.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfDSMNoThread.cpp
   * @skipline size
   * @until MPI_Comm_size
   * @skipline exampleWriter
   * @skipline if
   * @until {
   * @skipline exampleManager
   * @skipline exampleWriter
   * @skipline GetIsServer
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until exampleWriter
   * @skipline if
   * @skipline exampleManager
   * @skipline exampleIsServer
   *
   * @return      Whether the Manager is a server or not
   */
  bool GetIsServer();

  /**
   * Gets the maximum size of the local buffer on server cores.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfDSMNoThread.cpp
   * @skipline size
   * @until MPI_Comm_size
   * @skipline exampleWriter
   * @skipline if
   * @until {
   * @skipline exampleManager
   * @skipline exampleWriter
   * @skipline MBytes
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until exampleWriter
   * @skipline if
   * @skipline exampleManager
   * @skipline exampleBufferSize
   *
   * @return      the maximum size of the data buffer on server cores
   */
  unsigned int GetLocalBufferSizeMBytes();

  /**
   * Gets the MpiComm that the manager is currently using.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfDSMNoThread.cpp
   * @skipline size
   * @until MPI_Comm_size
   * @skipline exampleWriter
   * @skipline if
   * @until {
   * @skipline exampleManager
   * @skipline exampleWriter
   * @skipline exampleManagerComm
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until exampleWriter
   * @skipline if
   * @skipline exampleManager
   * @skipline exampleManagerComm
   *
   * @return      The MpiComm that the manager is currently using
   */
  MPI_Comm GetMpiComm();

  /**
   * Gets the id of the core with regards to the MpiComm
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfDSMNoThread.cpp
   * @skipline size
   * @until MPI_Comm_size
   * @skipline exampleWriter
   * @skipline if
   * @until {
   * @skipline exampleManager
   * @skipline exampleWriter
   * @skipline intraId
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until exampleWriter
   * @skipline if
   * @skipline exampleManager
   * @skipline intraId
   *
   * @return      The Id of the core calling this function
   */
  int GetUpdatePiece();

  /**
   * Gets the total number of cores on the MpiComm
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfDSMNoThread.cpp
   * @skipline size
   * @until MPI_Comm_size
   * @skipline exampleWriter
   * @skipline if
   * @until {
   * @skipline exampleManager
   * @skipline exampleWriter
   * @skipline intraSize
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until exampleWriter
   * @skipline if
   * @skipline exampleManager
   * @skipline intraSize
   *
   * @return      The total number of cores over the MpiComm
   */
  int GetUpdateNumPieces();

  /**
   * Sets the block length for the DSM data buffer.
   * Memory will be alloted in a multiple of the block size.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfDSMNoThread.cpp
   * @skipline size
   * @until MPI_Comm_size
   * @skipline exampleWriter
   * @skipline if
   * @until {
   * @skipline exampleManager
   * @skipline exampleWriter
   * @skipline GetBlockLength
   * @skipline Set
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until exampleWriter
   * @skipline if
   * @skipline exampleManager
   * @skipline exampleBlockLength
   * @skipline SetBlockLength
   *
   * @param       newSize         The size to the blocks to be used
   */
  void SetBlockLength(long newSize);

  /**
   * Sets the manager's internal buffer to the buffer provided.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfDSMNoThread.cpp
   * @skipline size
   * @until MPI_Comm_size
   * @skipline exampleWriter
   * @skipline if
   * @until {
   * @skipline exampleManager
   * @skipline exampleWriter
   * @skipline exampleBuffer
   * @skipline getServerBuffer
   * @skipline SetDSMBuffer
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until exampleWriter
   * @skipline if
   * @skipline exampleManager
   * @skipline exampleBuffer
   * @skipline SetDSMBuffer
   *
   * @param       newBuffer       The buffer that the Manager is to use as its internal buffer
   */
  void SetDsmBuffer(XdmfDSMBuffer * newBuffer);

  /**
   * Sets the DSM type that the Manager will set up when create is called
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfDSMNoThread.cpp
   * @skipline size
   * @until MPI_Comm_size
   * @skipline exampleWriter
   * @skipline if
   * @until {
   * @skipline exampleManager
   * @skipline exampleWriter
   * @skipline SetDsmType
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until exampleWriter
   * @skipline if
   * @skipline exampleManager
   * @skipline SetDsmType
   *
   * @param       newType         The Type of DSM that the manager should generate
   */
  void SetDsmType(int newType);

  /**
   * Sets whether this Manager is managing a server or not.
   * If false it will attempt to use the intercomm for data manipulation.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfDSMNoThread.cpp
   * @skipline size
   * @until MPI_Comm_size
   * @skipline exampleWriter
   * @skipline if
   * @until {
   * @skipline exampleManager
   * @skipline exampleWriter
   * @skipline GetIsServer
   * @skipline Set
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until exampleWriter
   * @skipline if
   * @skipline exampleManager
   * @skipline exampleIsServer
   * @skipline SetIsServer
   *
   * @param       newStatus       Whether the Manager is to be a server or not
   */
  void SetIsServer(bool newStatus);

  /**
   * Sets the type of intercomm that the Manager will use.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfDSMNoThread.cpp
   * @skipline size
   * @until MPI_Comm_size
   * @skipline exampleWriter
   * @skipline if
   * @until {
   * @skipline exampleManager
   * @skipline exampleWriter
   * @skipline SetInterCommType
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until exampleWriter
   * @skipline if
   * @skipline exampleManager
   * @skipline SetInterCommType
   *
   * @param       newType         The type of intercomm to be generated for now on
   */
  void SetInterCommType(int newType);

  /**
   * Sets the maximum size of the local buffer when generating data buffers for server cores.
   * When using blocked mode it generates a buffer that is a multiple of the block size
   * that is less than or equal to this number.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfDSMNoThread.cpp
   * @skipline size
   * @until MPI_Comm_size
   * @skipline exampleWriter
   * @skipline if
   * @until {
   * @skipline exampleManager
   * @skipline exampleWriter
   * @skipline MBytes
   * @skipline Set
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until exampleWriter
   * @skipline if
   * @skipline exampleManager
   * @skipline exampleBufferSize
   * @skipline SetLocalBufferSize
   *
   * @param       newSize         The new maximum size of the data buffer on the server cores
   */
  void SetLocalBufferSizeMBytes(unsigned int newSize);

  /**
   * Sets the MpiComm to the provided communicator and updates UpdatePiece and UpdateNumPieces
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfDSMNoThread.cpp
   * @skipline size
   * @until MPI_Comm_size
   * @skipline exampleWriter
   * @skipline if
   * @until {
   * @skipline exampleManager
   * @skipline exampleWriter
   * @skipline exampleManagerComm
   * @skipline SetMpiComm
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until exampleWriter
   * @skipline if
   * @skipline exampleManager
   * @skipline exampleManagerComm
   * @skipline SetMpiComm
   *
   * @param       comm    The comm to be set as the MpiComm for this manager
   */
  void SetMpiComm(MPI_Comm comm);

protected:


private:

  int    UpdatePiece;
  int    UpdateNumPieces;
  unsigned int   LocalBufferSizeMBytes;
    
  MPI_Comm MpiComm;

  XdmfDSMBuffer *DsmBuffer;
  XdmfDSMCommMPI       *DsmComm;

  bool  IsServer;
  int    DsmType;
  long   BlockLength;
  int    InterCommType;
};

#endif /* XDMFDSMMANAGER_HPP_ */
