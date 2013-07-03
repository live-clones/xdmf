/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfDSMBuffer.hpp                                                   */
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
  Module                  : H5FDdsmBufferService.h, H5FDdsmBuffer.h

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

#ifndef XDMFDSMBUFFER_HPP_
#define XDMFDSMBUFFER_HPP_

// Forward Declarations

// Includes
#include <XdmfCore.hpp>
#include <XdmfDSMCommMPI.hpp>
#include <mpi.h>

#define XDMF_DSM_DEFAULT_TAG    0x80
#define XDMF_DSM_COMMAND_TAG    0x81
#define XDMF_DSM_SERVER_ACK_TAG 0x82
#define XDMF_DSM_CLIENT_ACK_TAG 0x83
#define XDMF_DSM_PUT_DATA_TAG   0x84
#define XDMF_DSM_GET_DATA_TAG   0x85
#define XDMF_DSM_EXCHANGE_TAG   0x86

#define XDMF_DSM_ANY_TAG        -1
#define XDMF_DSM_ANY_SOURCE     -2

#define XDMF_DSM_TYPE_UNIFORM       0
#define XDMF_DSM_TYPE_UNIFORM_RANGE 1
#define XDMF_DSM_TYPE_MIXED         2
#define XDMF_DSM_TYPE_BLOCK_CYCLIC  3
#define XDMF_DSM_TYPE_BLOCK_RANDOM  4

#define XDMF_DSM_DEFAULT_LENGTH 10000
#define XDMF_DSM_DEFAULT_BLOCK_LENGTH 1024
#define XDMF_DSM_ALIGNMENT 4096

#define XDMF_DSM_OPCODE_PUT          0x01
#define XDMF_DSM_OPCODE_GET          0x02

#define XDMF_DSM_LOCK_ACQUIRE        0x03
#define XDMF_DSM_LOCK_RELEASE        0x05

#define XDMF_DSM_OPCODE_DONE         0xFF

#define XDMF_DSM_SUCCESS  1
#define XDMF_DSM_FAIL    -1


/**
 * @brief Controls the data buffer for DSM.
 *
 * XdmfDSMBuffer takes the place of the H5FDdsmBuffer defined in H5FD.
 * It is primarily for allowing the XdmfDSM to interact with HDF5 dsm without threads.
 */
class XDMFCORE_EXPORT XdmfDSMBuffer {

public:

  XdmfDSMBuffer();
  ~XdmfDSMBuffer();

  /**
   * Find the Id of the core that the provided address resides on.
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
   * @skipline XdmfDSMBuffer
   * @skipline exampleController
   * @skipline AddressToId
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until exampleWriter
   * @skipline if
   * @skipline exampleBuffer
   * @skipline AddressToId
   *
   * @param	Address		The address to be found
   * @return			The id of the core that the address resides on
   */
  int AddressToId(int Address);

  /**
   * Broadcasts the provided comm from the specified core to all other cores. 
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfDSMNoThread.cpp
   * @skipline size
   * @until MPI_Comm_size
   * @skipline workerComm
   * @until cfree
   * @skipline exampleController
   * @until size-1
   * @skipline if
   * @until {
   * @skipline XdmfDSMBuffer
   * @skipline exampleController
   * @skipline broadcastComm
   * @until BroadcastComm
   * @skip Section
   * @skipline }
   *
   * Python:
   * Unusable in python unless an object of a cpointer type is passed via wrapped code
   *
   *
   *
   * @param	comm	The communicator to be transmitted
   * @param	root	The core that the broadcast is originating from
   */
  void BroadcastComm(int *comm, int root);

  /**
   * One iteration of the service loop.
   * It is not advised to use this function manually.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfDSMNoThread.cpp
   * @skipline size
   * @until MPI_Comm_size
   * @skipline workerComm
   * @until cfree
   * @skipline exampleController
   * @until size-1
   * @skipline if
   * @until {
   * @skipline XdmfDSMBuffer
   * @skipline exampleController
   * @skip RecieveInfo
   * @skipline if
   * @until BufferService
   * @skipline }
   * @until SendCommandHeader
   * @skipline }
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until exampleWriter
   * @skipline if
   * @skipline exampleBuffer
   * @skip ServerIds
   * @skip SendInfo
   * @skipline if
   * @skipline serviceOut
   * @skipline if
   * @skipline SendCommandHeader
   *
   * @param       returnOpcode    A variable that will hold the code at the end of the loop
   * @return                      If the iteration executed without problem returns XDMF_DSM_SUCCESS
   */
  int BufferService(int *returnOpcode = 0);

  /**
   * Starts up the service loop.
   * The loop then executes until the op code "Done" is sent to this core.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfDSMNoThread.cpp
   * @skipline size
   * @until MPI_Comm_size
   * @skipline workerComm
   * @until cfree
   * @skipline exampleController
   * @until size-1
   * @skipline if
   * @until {
   * @skipline XdmfDSMBuffer
   * @skipline exampleController
   * @skip RecieveInfo
   * @skipline if
   * @until BufferServiceLoop
   * @skipline }
   * @until SendCommandHeader
   * @skipline }
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until exampleWriter
   * @skipline if
   * @skipline exampleBuffer
   * @skip ServerIds
   * @skip SendInfo
   * @skipline if
   * @skipline BufferServiceLoop
   * @skipline if
   * @skipline SendCommandHeader
   *
   * @param       returnOpcode    A variable that will hold the code at the end of the loop
   */
  void BufferServiceLoop(int *returnOpcode = 0);

  /**
   * Configures the Buffer to match the configuration details provided.
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
   * @skipline XdmfDSMBuffer
   * @skipline exampleController
   * @skipline ConfigureUniform
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until exampleWriter
   * @skipline if
   * @skipline exampleBuffer
   * @skipline ConfigureUniform
   *
   * @param       Comm            The communicator that will be handling the communications for the DSM
   * @param       Length          The length of the data buffer on server cores
   * @param       StartId         The id that the server cores will start on, if set to -1 then it will default to 0
   * @param       EndId           The id that the server cores will end on, if set to -1 then it will be the last core in the communicator
   * @param       aBlockLength    The block size of the data buffer, 0 is no blocking
   * @param       random          Whether the assignment is random or cyclic. Default is cyclic
   */
  void ConfigureUniform(XdmfDSMCommMPI *Comm, long Length,
                        int StartId = -1, int EndId = -1,
                        long aBlockLength = 0, bool random = false);

  /**
   * Gets data from the server cores.
   * It is not advised to use this function manually.
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
   * @skipline XdmfDSMBuffer
   * @skipline exampleController
   * @skipline core0StartAddress
   * @until GetAddressRange
   * @skipline dsmData
   * @skipline if
   * @until {
   * @skipline Get
   * @until message
   * Skipline }
   * @skip Section
   * @skipline }
   *
   *
   * Python:
   * Unusable in python unless an object of a cpointer type is passed via wrapped code
   * Use the XdmfHDF5WriterDSM for this functionality
   *
   *
   *
   * @param	Address		The starting address of the data retrieved
   * @param	aLength		The length of the data to be retrieved
   * @param	Data		A pointer in which the data is to be stored after retieval
   */
  void Get(long Address, long aLength, void *Data);

  /**
   * Gets the starting address and ending address for the core of the provided Id.
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
   * @skipline XdmfDSMBuffer
   * @skipline exampleController
   * @skipline core0StartAddress
   * @until GetAddressRange
   * @skip Section
   * @skipline }
   *
   * Python:
   * Unusable in python unless an object of a cpointer type is passed via wrapped code
   *
   *
   *
   * @param	Id	The core for which the start and end address are to be found
   * @param	Start	A pointer in which the start address is to be placed
   * @param	End	A pointer in which the end address is to be placed
   */
  void GetAddressRangeForId(int Id, int *Start, int *End);

  /**
   * Gets the size of the blocks for the data buffer.
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
   * @skipline XdmfDSMBuffer
   * @skipline exampleController
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
   * @skipline exampleBuffer
   * @skipline GetBlockLength
   *
   * @return      The size of the blocks in the DSM buffer
   */
  long GetBlockLength();

  /**
   * Gets the Comm being used to facilitate the communications for the DSM
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
   * @skipline XdmfDSMBuffer
   * @skipline exampleController
   * @skipline XdmfDSMCommMPI
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until exampleWriter
   * @skipline if
   * @skipline exampleBuffer
   * @skipline exampleDSMComm
   *
   * @return      The Comm controlling the communications for the DSM
   */
  XdmfDSMCommMPI * GetComm();

  /**
   * Gets the data pointer that the buffer controls. Should be NULL on non-server cores.
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
   * @skipline XdmfDSMBuffer
   * @skipline exampleController
   * @skipline GetDataPointer
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until exampleWriter
   * @skipline if
   * @skipline exampleBuffer
   * @skipline GetDataPointer
   *
   * @return	The buffer's internal data pointer.
   */
  char * GetDataPointer();

  /**
   * Gets the DSM type currently being used.
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
   * @skipline XdmfDSMBuffer
   * @skipline exampleController
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
   * @skipline exampleBuffer
   * @skipline GetDsmType
   *
   * @return      The integer representation of the DSM type
   */
  int GetDsmType();

  /**
   * Gets the address at the end of DSM buffer for this buffer.
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
   * @skipline XdmfDSMBuffer
   * @skipline exampleController
   * @skipline GetEndAddress
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until exampleWriter
   * @skipline if
   * @skipline exampleBuffer
   * @skipline GetEndAddress
   *
   * @return      The end address of the DSM buffer
   */
  int GetEndAddress();

  /**
   * Gets the id of the last of the server cores that handle the DSM buffer.
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
   * @skipline XdmfDSMBuffer
   * @skipline exampleController
   * @skipline GetEndServerId
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until exampleWriter
   * @skipline if
   * @skipline exampleBuffer
   * @skipline GetEndServerId
   *
   * @return      The id of the last core that serves as a DSM server
   */
  int GetEndServerId();

  /**
   * Gets if the Buffer is connected to an intercomm
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
   * @skipline XdmfDSMBuffer
   * @skipline exampleController
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
   * @skipline exampleBuffer
   * @skipline GetIsConnected
   *
   * @return      Whether the buffer is connected
   */
  bool GetIsConnected();

  /**
   * Gets if the buffer is a DSM server.
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
   * @skipline XdmfDSMBuffer
   * @skipline exampleController
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
   * @skipline exampleBuffer
   * @skipline GetIsServer
   *
   * @return      If the Buffer is a DSM server
   */
  bool GetIsServer();

  /**
   * The length of the buffer per core.
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
   * @skipline XdmfDSMBuffer
   * @skipline exampleController
   * @skipline GetLength
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until exampleWriter
   * @skipline if
   * @skipline exampleBuffer
   * @skipline GetLength
   *
   * @return      The length of the data buffer per core
   */
  long GetLength();

  /**
   * Gets the address at the beginning of the DSM buffer for this buffer.
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
   * @skipline XdmfDSMBuffer
   * @skipline exampleController
   * @skipline GetStartAddress
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until exampleWriter
   * @skipline if
   * @skipline exampleBuffer
   * @skipline GetStartAddress
   *
   * @return      The beginning address of the DSM buffer
   */
  int GetStartAddress();

  /**
   * Gets the id of the first of the server cores that handle the DSM buffer.
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
   * @skipline XdmfDSMBuffer
   * @skipline exampleController
   * @skipline GetStartServerId
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until exampleWriter
   * @skipline if
   * @skipline exampleBuffer
   * @skipline GetStartServerId
   *
   * @return      The id of the first core that serves as a DSM server
   */
  int GetStartServerId();

  /**
   * The total length of the data buffer when combining the buffers in all cores.
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
   * @skipline XdmfDSMBuffer
   * @skipline exampleController
   * @skipline GetTotalLength
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until exampleWriter
   * @skipline if
   * @skipline exampleBuffer
   * @skipline GetTotalLength
   *
   * @return      The total length of the data buffer
   */
  long GetTotalLength();

  /**
   * Probes inter and intra comms until a command is found.
   * Then sets the comm that the command was found on to the provided variable
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
   * @skipline XdmfDSMBuffer
   * @skipline exampleController
   * @skip ConfigureUniform
   * @skipline id
   * @until ReceiveCommandHeader
   * @skipline }
   * @skip Section
   * @skipline }
   *
   * Python:
   * Unusable in python unless an object of a cpointer type is passed via wrapped code
   *
   *
   *
   * @param       comm    a pointer to the variable that the integer code for the comm is placed in
   */
  void ProbeCommandHeader(int *comm);

  /**
   * Puts data to the server cores.
   * It is not advised to use this function manually.
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
   * @skipline XdmfDSMBuffer
   * @skipline exampleController
   * @skipline core0StartAddress
   * @until GetAddressRange
   * @skipline dsmData
   * @until Put
   * @skipline }
   * @until message
   * Skipline }
   * @skip Section
   * @skipline }
   *
   * Python:
   * Unusable in python unless an object of a cpointer type is passed via wrapped code
   * Use the XdmfHDF5WriterDSM for this functionality
   *
   *
   *
   * @param	Address		The starting address that the data will be placed at
   * @param	aLength		The length of the data to be sent
   * @param	Data		A pointer to the data to be sent
   */
  void Put(long Address, long aLength, const void *Data);

  /**
   * Recieves an integer as an acknowledgement from the specified core.
   * It is not advised to use this function manually.
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
   * @skipline XdmfDSMBuffer
   * @skipline exampleController
   * @skip ReceiveData
   * @skipline id
   * @until ReceiveAcknowledgment
   * @skipline }
   * @skip Section
   * @skipline }
   *
   * Python:
   * Unusable in python unless an object of a cpointer type is passed via wrapped code
   *
   *
   *
   * @param       source  The core to recieve from
   * @param       data    The integer that the recieved data will be stored in
   * @param       tag     The tag associated with the acknowledgement
   * @param       comm    The comm over which the acknowldedgement will occur
   */
  void ReceiveAcknowledgment(int source, int &data, int tag, int comm);

  /**
   * Recieves command data from either a specified or unspecified source.
   * If remoteSource is not provided any source currently sending a command is used.
   * It is not advised to use this function manually.
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
   * @skipline XdmfDSMBuffer
   * @skipline exampleController
   * @skip ConfigureUniform
   * @skipline id
   * @until ReceiveCommandHeader
   * @skipline }
   * @skip Section
   * @skipline }
   *
   * Python:
   * Unusable in python unless an object of a cpointer type is passed via wrapped code
   *
   *
   *
   * @param       opcode          A pointer to the location where the code associated with the operation will be stored
   * @param       source          A pointer to the location where the index of the source core will be stored (will be the same as remoteSource if provided)
   * @param       address         A pointer to the location where the address specified by the command will be stored
   * @param       aLength         A pointer to the location where the length of the data specified by the command will be stored
   * @param       comm            The communicator over which the transmission will occur
   * @param       remoteSource    If provided, the core being recieved from
   */
  void ReceiveCommandHeader(int *opcode, int *source, int *address, int *aLength, int comm, int remoteSource = -1);

  /**
   * Recieves data from a specific core and stores it in a pointer.
   * It is not advised to use this function manually.
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
   * @skipline XdmfDSMBuffer
   * @skipline exampleController
   * @skip ReceiveCommandHeader
   * @skipline id
   * @until ReceiveData
   * @skipline }
   * @skip Section
   * @skipline }
   *
   * Python:
   * Unusable in python unless an object of a cpointer type is passed via wrapped code
   *
   *
   *
   * @param       source          The core data will be recieved from
   * @param       data            The pointer where the recieved data will be stored
   * @param       aLength         The length of the data transmitted
   * @param       tag             The communication tag to be used for the transmission
   * @param       aAddress        The location where the data will be written on the data buffer
   * @param       comm            The comunicator over which the data transfer will occur
   */
  void ReceiveData(int source, char * data, int aLength, int tag, int aAddress, int comm);

  /**
   * With the Comm with ID 0 recieve information
   * about a server from another core on the intercomm.
   * Used to recieve server data from different managers.
   * It is not advised to use this function manually.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfDSMNoThread.cpp
   * @skipline size
   * @until MPI_Comm_size
   * @skipline workerComm
   * @until cfree
   * @skipline exampleController
   * @until size-1
   * @skipline if
   * @until {
   * @skipline XdmfDSMBuffer
   * @skipline exampleController
   * @skipline readComm
   * @until cfree
   * @skipline if
   * @until workerComm
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until exampleWriter
   * @skipline if
   * @skipline exampleBuffer
   * @skipline ServerIds
   * @until (workerComm)
   *
   */
  void ReceiveInfo();

  /**
   * Sends an integer as an acknowledgement to the specified core.
   * It is not advised to use this function manually.
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
   * @skipline XdmfDSMBuffer
   * @skipline exampleController
   * @skip ReceiveData
   * @skipline id
   * @until ReceiveAcknowledgment
   * @skipline }
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until exampleWriter
   * @skipline if
   * @skipline exampleBuffer
   * @skip SendData
   * @skipline if
   * @skipline SendAcknowledgment
   *
   * @param       dest    The core to send to
   * @param       data    The integer to send
   * @param       tag     The tag associated with the acknowledgement
   * @param       comm    The comm over which the acknowldedgement will occur
   */
  void SendAcknowledgment(int dest, int data, int tag, int comm);

  /**
   * Sends command data to a specified core.
   * It is not advised to use this function manually.
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
   * @skipline XdmfDSMBuffer
   * @skipline exampleController
   * @skip ConfigureUniform
   * @skipline id
   * @until ReceiveCommandHeader
   * @skipline }
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until exampleWriter
   * @skipline if
   * @skipline exampleBuffer
   * @skip ConfigureUniform
   * @skipline if
   * @skipline SendCommandHeader
   *
   * @param       opcode          The code for the command to be sent
   * @param       dest            The core that the command is to be sent to
   * @param       address         The address to be referenced by the command
   * @param       aLength         The length of the data to be used by the command
   * @param       comm            The communicator over which the transmission will occur
   */
  void SendCommandHeader(int opcode, int dest, int address, int aLength, int comm);

  /**
   * Sends data from a pointer to a specified core.
   * It is not advised to use this function manually.
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
   * @skipline XdmfDSMBuffer
   * @skipline exampleController
   * @skip ReceiveCommandHeader
   * @skipline id
   * @until ReceiveData
   * @skipline }
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until exampleWriter
   * @skipline if
   * @skipline exampleBuffer
   * @skip SendCommandHeader
   * @skipline if
   * @skipline SendData
   *
   * @param       dest            The core that the data will be sent to
   * @param       data            A pointer to the location of the data being sent
   * @param       aLength         The length of the data being sent
   * @param       tag             The communication tag to be used for the transmission
   * @param       aAddress        The address on the recieveing core's buffer that the data is to be placed in
   * @param       comm            The communicator over which the data transfer will take place
   */
  void SendData(int dest, char * data, int aLength, int tag, int aAddress, int comm);

  /**
   * Ends the service loop server cores associated with this buffer
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfDSMNoThread.cpp
   * @skipline size
   * @until MPI_Comm_size
   * @skipline exampleWriter
   * @skipline closeFile
   * @skipline if
   * @until {
   * @skipline XdmfDSMBuffer
   * @skipline SendDone
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until exampleWriter
   * @skipline Section
   * @skipline if
   * @skipline GetServerBuffer
   * @skipline SendDone
   *
   */
  void SendDone();

  /**
   * From the Comm with ID 0 send out information
   * about the server to another core on the intercomm.
   * Used to send server data to different managers.
   * It is not advised to use this function manually.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfDSMNoThread.cpp
   * @skipline size
   * @until MPI_Comm_size
   * @skipline workerComm
   * @until cfree
   * @skipline exampleController
   * @until size-1
   * @skipline if
   * @until {
   * @skipline XdmfDSMBuffer
   * @skipline exampleController
   * @skipline readComm
   * @until cfree
   * @skipline if
   * @until workerComm
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until exampleWriter
   * @skipline if
   * @skipline exampleBuffer
   * @skipline ServerIds
   * @until (workerComm)
   *
   */
  void SendInfo();

  /**
   * Sets the DSM type to the provided type.
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
   * @skipline XdmfDSMBuffer
   * @skipline exampleController
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
   * @skipline exampleBuffer
   * @skipline SetDsmType
   *
   * @param       newDsmType      The Dsm type that the buffer will be changed to
   */
  void SetDsmType(int newDsmType);

  /**
   * Sets the size of the blocks used in the data buffer.
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
   * @skipline XdmfDSMBuffer
   * @skipline exampleController
   * @skipline SetBlockLength
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until exampleWriter
   * @skipline if
   * @skipline exampleBuffer
   * @skipline SetBlockLength
   *
   * @param       newBlock        The new block size to be used
   */
  void SetBlockLength(long newBlock);

  /**
   * Sets the Comm to be used to facilitate the communications for the DSM
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
   * @skipline XdmfDSMBuffer
   * @skipline exampleController
   * @skipline XdmfDSMCommMPI
   * @skipline SetComm
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until exampleWriter
   * @skipline if
   * @skipline exampleBuffer
   * @skipline exampleDSMComm
   * @skipline SetComm
   *
   * @param       newComm         The communicator that is to be used by the DSM
   */
  void SetComm(XdmfDSMCommMPI * newComm);

  /**
   * Sets the Buffer's connection status. Used if the XdmfDSMCommMPI is connected or disconnected manually.
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
   * @skipline XdmfDSMBuffer
   * @skipline exampleController
   * @skipline SetIsConnected
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until exampleWriter
   * @skipline if
   * @skipline exampleBuffer
   * @skipline GetIsConnected
   * @skipline SetIsConnected
   *
   * @param       newStatus       The new connection status
   */
  void SetIsConnected(bool newStatus);

  /**
   * Sets whether the buffer is a DSM server.
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
   * @skipline XdmfDSMBuffer
   * @skipline exampleController
   * @skipline SetIsServer
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until exampleWriter
   * @skipline if
   * @skipline exampleBuffer
   * @skipline GetIsServer
   * @skipline SetIsServer
   *
   * @param       newIsServer     Whether the buffer is to be a DSM server or not
   */
  void SetIsServer(bool newIsServer);

protected:


private:

  void SetLength(long aLength);

  class			CommandMsg;
  class			InfoMsg;

  bool			IsServer;

  int			EndAddress;
  int			StartAddress;

  int			StartServerId;
  int			EndServerId;

  unsigned int		Length;
  unsigned int		TotalLength;
  unsigned int		BlockLength;

  XdmfDSMCommMPI	*Comm;

  char			*DataPointer;

  int			DsmType;

  int			CommChannel;
  bool			IsConnected;
};

#endif /* XDMFDSMBUFFER_HPP_ */

