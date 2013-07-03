/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfDSMCommMPI.hpp                                                  */
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

#ifndef XDMFDSMCOMMMPI_HPP_
#define XDMFDSMCOMMMPI_HPP_

// Forward Declarations

#define XDMF_DSM_COMM_MPI       0x11

#define XDMF_DSM_INTRA_COMM  0x00
#define XDMF_DSM_INTER_COMM  0x01
#define XDMF_DSM_ANY_COMM    0x02

#define XDMF_DSM_SERVER_ID          XDMF_DSM_INTRA_COMM
#define XDMF_DSM_CLIENT_ID          XDMF_DSM_INTER_COMM
#define XDMF_DSM_NUM_CONNECTION_IDS 0x02

// Includes
#include <XdmfCore.hpp>

#include <mpi.h>

/**
 * @brief Holds communicators for interacting with H5FD dsm.
 *
 * XdmfDSMCommMPI takes the place of the H5FDdsmComm defined in H5FD.
 * It provides more access to the the intra and inter communicators.
 * It is primarily for allowing the XdmfDSM to interact with HDF5 dsm without threads.
 */
class XDMFCORE_EXPORT XdmfDSMCommMPI {

public:

  XdmfDSMCommMPI();
  ~XdmfDSMCommMPI();

  /**
   * Accepts connections to the port currently named by DsmMasterHostName. Called on server side, accepts from core 0.
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
   * @skipline connectingGroup
   * @until false
   * @skipline }
   * @skipline (!connectingGroup
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
   * @skipline exampleBuffer
   * @skipline exampleDSMComm
   * @skipline connectingGroup
   * @until ClosePort
   *
   */
  void Accept();

  /**
   * Closes the port currently named by DsmMasterHostName.
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
   * @skipline connectingGroup
   * @until False
   * @skipline }
   * @skipline (!connectingGroup
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
   * @skipline exampleBuffer
   * @skipline exampleDSMComm
   * @skipline connectingGroup
   * @until ClosePort
   *
   */
  void ClosePort();

  /**
   * If core ID is 0 then attempts to connect to the port currently named by DsmMasterHostName
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
   * @skipline connectingGroup
   * @until false
   * @skipline }
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
   * @skipline exampleBuffer
   * @skipline exampleDSMComm
   * @skipline connectingGroup
   * @until False
   * @skipline (connectingGroup
   * @until Disconnect
   *
   * @return      Whether the connection was successful or not
   */
  int Connect();

  /**
   * Disconnects the intercomm if not in static intercomm mode. Then sets the intercomm to MPI_COMM_NULL.
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
   * @skipline connectingGroup
   * @until false
   * @skipline }
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
   * @skipline exampleBuffer
   * @skipline exampleDSMComm
   * @skipline connectingGroup
   * @until False
   * @skipline (connectingGroup
   * @until Disconnect
   *
   */
  void Disconnect();

  /**
   * Sets the IntraComm to the provided comm by duplicating it.
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
   * @skipline DupComm
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
   * @skipline GetIntraComm
   * @skipline DupComm
   *
   * @param	comm	The comm to be used as the IntraComm
   */
  void DupComm(MPI_Comm comm);

  /**
   * Sets the intercomm to the communicator provided by duplicating it.
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
   * @skipline DupInterComm
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until exampleWriter
   * @skipline Section
   * @skipline exampleDSMComm
   * @skipline GetInterComm
   * @skipline DupInterComm
   *
   * @param       comm    The comm to be used as the intercomm
   */
  void DupInterComm(MPI_Comm comm);

  /**
   * Gets the port name that will be connected to when Connect/Accept is called.
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
   * @skipline connectingGroup
   * @until false
   * @skipline }
   * @skipline (!connectingGroup
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
   * @skipline exampleBuffer
   * @skipline exampleDSMComm
   * @skipline connectingGroup
   * @until ClosePort
   *
   * @return      a pointer to the character string that specifies the port
   */
  char * GetDsmMasterHostName();

  /**
   * Gets the Id with regards to the IntraComm.
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
   * @skipline GetId
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
   * @skipline GetId
   *
   * @return	The Id of the core with regards to the IntraComm
   */
  int GetId();

  /**
   * Gets the communicator that is currently functioning as the intercomm.
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
   * @skipline GetInterComm
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until exampleWriter
   * @skipline Section
   * @skipline exampleDSMComm
   * @skipline GetInterComm
   *
   * @return      The communicator currently serving as the intercomm.
   */
  MPI_Comm GetInterComm();

  /**
   * Gets the type of the InterComm.
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
   * @skipline exampleBuffer
   * @skipline exampleDSMComm
   * @skipline GetInterCommType
   *
   * @return	An integer representation of the InterComm's type
   */
  int GetInterCommType();

  /**
   * Gets the number of cores contained in the InterComm.
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
   * @skipline GetInterSize
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
   * @skipline GetInterSize
   *
   * @return	The number of cores in the InterComm
   */
  int GetInterSize();

  /**
   * Gets the communicator that is currently functioning as the intracomm.
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
   * @skipline GetIntraComm
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
   * @skipline GetIntraComm
   *
   * @return      The communicator currently serving as the intracomm.
   */
  MPI_Comm GetIntraComm();

  /**
   * Gets the number of cores contained in the IntraComm.
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
   * @skipline GetIntraSize
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
   * @skipline GetIntraSize
   *
   * @return	The number of cores in the IntraComm
   */
  int GetIntraSize();

  /**
   * Initializes the Intracomm rank and size to the associated variables in the internal structure
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
   * @skipline Init
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
   * @skipline Init
   *
   */
  void Init();

  /**
   * Opens a port and stores the port name in DsmMasterHostName.
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
   * @skipline connectingGroup
   * @until false
   * @skipline }
   * @skipline (!connectingGroup
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
   * @skipline exampleBuffer
   * @skipline exampleDSMComm
   * @skipline connectingGroup
   * @until ClosePort
   *
   */
  void OpenPort();

  /**
   * Sets the port name that will be connected to when Connect/Accept is called.
   * Data is copied, so the provided string is not modified.
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
   * @skipline connectingGroup
   * @until false
   * @skipline }
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
   * @skipline exampleBuffer
   * @skipline exampleDSMComm
   * @skipline connectingGroup
   * @until False
   * @skipline (connectingGroup
   * @until Disconnect
   *
   * @param       hostName        a pointer to the character string that specifies the port
   */
  void SetDsmMasterHostName(const char *hostName);

protected:


private:
  MPI_Comm	IntraComm;
  int		Id;
  int		IntraSize;
  MPI_Comm	InterComm;
  int		InterSize;
  int		InterCommType;
  char 		DsmMasterHostName[MPI_MAX_PORT_NAME];
};

#endif /* XDMFDSMCOMMMPI_HPP_ */

