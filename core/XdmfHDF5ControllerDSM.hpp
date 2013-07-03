/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfHDF5ControllerDSM.cpp                                           */
/*                                                                           */
/*  Author:                                                                  */
/*     Kenneth Leiter                                                        */
/*     kenneth.leiter@arl.army.mil                                           */
/*     US Army Research Laboratory                                           */
/*     Aberdeen Proving Ground, MD                                           */
/*                                                                           */
/*     Copyright @ 2011 US Army Research Laboratory                          */
/*     All Rights Reserved                                                   */
/*     See Copyright.txt for details                                         */
/*                                                                           */
/*     This software is distributed WITHOUT ANY WARRANTY; without            */
/*     even the implied warranty of MERCHANTABILITY or FITNESS               */
/*     FOR A PARTICULAR PURPOSE.  See the above copyright notice             */
/*     for more information.                                                 */
/*                                                                           */
/*****************************************************************************/

#ifndef XDMFHDF5CONTROLLERDSM_HPP_
#define XDMFHDF5CONTROLLERDSM_HPP_

// Forward Declarations
class H5FDdsmBuffer;
class H5FDdsmManager;

// Includes
#include "XdmfCore.hpp"
#include "XdmfHDF5Controller.hpp"
#include "XdmfDSMManager.hpp"
#include "XdmfDSMBuffer.hpp"

/**
 * @brief Couples an XdmfArray with HDF5 data stored in a DSM buffer.
 *
 * Serves as an interface between data stored in XdmfArrays and data
 * stored in DSM buffers.  When an Xdmf file is read from or written
 * to a DSM buffer an XdmfHDF5ControllerDSM is attached to XdmfArrays.
 * This allows data to be released from memory but still be accessible
 * or have its location written to light data.
 */
class XDMFCORE_EXPORT XdmfHDF5ControllerDSM : public XdmfHDF5Controller {

public:

  virtual ~XdmfHDF5ControllerDSM();

  /**
   * Create a new controller for an DSM data set.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfDSM.cpp
   * @skipline providedThreading
   * @until MPI_Comm_size
   * @skipline Create
   * @until dsmBuffer
   * @skipline newPath
   * @skipline readController
   * @skipline newSetPath
   * @skip //ensure
   * @skipline startVector
   * @until datasizeVector
   * @skipline startVector
   * @until datasizeVector
   * @skipline readController
   * @until );
   * @skipline readArray
   * @until setHeavyDataController
   * @skipline read()
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMStandalone.py
   * @skipline comm
   * @until size*4
   * @skipline coreSize
   * @until getBuffer
   * @skipline setMode
   * @until setHeavyDataController
   * @skipline accept
   * @until readDataSize
   * @skipline readStarts
   * @until readDataSize
   * @skipline readArray
   * @until setHeavyDataController
   * @skip print
   * @skipline readArray
   * @skipline segfault
   * @skipline writeController
   *
   * @param	hdf5FilePath		The path to the hdf5 file that the controller will be accessing
   * @param	dataSetPath		The location within the file of the data the controller with be accessing
   * @param	type			The data type of the data Ex: XdmfArrayType::Int32()
   * @param	start			A vector of the start indexes for all dimensions of the data
   * @param	stride			A vector of the distance between reads for all dimensions of the data
   * @param	dimensions		A vector of the number of values read from all dimensions of the data
   * @param	dataspaceDimensions	A vecotr containing the total size of the dimension in the data space
   * @param	dsmBuffer		A pointer to the dsm buffer
   */
  static shared_ptr<XdmfHDF5ControllerDSM>
  New(const std::string & hdf5FilePath,
      const std::string & dataSetPath,
      const shared_ptr<const XdmfArrayType> type,
      const std::vector<unsigned int> & start,
      const std::vector<unsigned int> & stride,
      const std::vector<unsigned int> & dimensions,
      const std::vector<unsigned int> & dataspaceDimensions,
      H5FDdsmBuffer * const dsmBuffer);

  /**
   * Create a new controller for an DSM data set. This version creates its own DSM buffer
   *
   * When created the manager has the following defaults:
   * IsStandAlone = H5FD_DSM_TRUE
   * H5FD_DSM_LOCK_ASYNCHRONOUS
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfDSMSelfcontained.cpp
   * @skipline providedThreading
   * @until MPI_Comm_size
   * @skipline testArray
   * @until 4*size
   * @skipline writeController
   * @until dsmSize
   * @skipline XdmfHDF5WriterDSM::New
   * @skipline dsmManager
   * @skipline writeController->getManager()
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMStandalone.py
   * @skipline comm
   * @until size*4
   * @skipline coreSize
   * @until If
   * @skip delete
   * @skipline writeController
   * @until exampleWriter
   * @skipline setMode
   * @until setHeavyDataController
   * @skipline accept
   * @until readDataSize
   * @skipline readStarts
   * @until readDataSize
   * @skipline readArray
   * @until setHeavyDataController
   * @skip print
   * @skipline readArray
   * @skipline segfault
   * @skipline writeController
   *
   * @param     hdf5FilePath            The path to the hdf5 file that the controller will be accessing
   * @param     dataSetPath             The location within the file of the data the controller with be accessing
   * @param     type                    The data type of the data Ex: XdmfArrayType::Int32()
   * @param     start                   A vector of the start indexes for all dimensions of the data
   * @param     start                   A vector of the start indexes for all dimensions of the data
   * @param     stride                  A vector of the distance between reads for all dimensions of the data
   * @param     dimensions              A vector of the number of values read from all dimensions of the data
   * @param     dataspaceDimensions     A vecotr containing the total size of the dimension in the data space
   * @param     comm			The communicator that the DSM buffer will reference
   * @param	bufferSize		The size of the buffer to be created on the core calling this function               
   */
  static shared_ptr<XdmfHDF5ControllerDSM>
  New(const std::string & hdf5FilePath,
      const std::string & dataSetPath,
      const shared_ptr<const XdmfArrayType> type,
      const std::vector<unsigned int> & start,
      const std::vector<unsigned int> & stride,
      const std::vector<unsigned int> & dimensions,
      const std::vector<unsigned int> & dataspaceDimensions,
      MPI_Comm comm,
      unsigned int bufferSize);

  /**
   * Create a new controller for an DSM data set.
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
   * @skipline writeController
   * @until size-1
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until exampleWriter
   * @skipline if
   * @skipline writeController
   * @until size-1
   *
   * @param     hdf5FilePath            The path to the hdf5 file that the controller will be accessing
   * @param     dataSetPath             The location within the file of the data the controller with be accessing
   * @param     type                    The data type of the data Ex: XdmfArrayType::Int32()
   * @param     start                   A vector of the start indexes for all dimensions of the data
   * @param     stride                  A vector of the distance between reads for all dimensions of the data
   * @param     dimensions              A vector of the number of values read from all dimensions of the data
   * @param     dataspaceDimensions     A vecotr containing the total size of the dimension in the data space
   * @param     dsmBuffer               A pointer to the dsm buffer
   * @param	startCoreIndex		The index at which the server cores for the buffer start
   * @param	endCoreIndex		The index at which the server cores for the buffer end
   */
  static shared_ptr<XdmfHDF5ControllerDSM>
  New(const std::string & hdf5FilePath,
      const std::string & dataSetPath,
      const shared_ptr<const XdmfArrayType> type,
      const std::vector<unsigned int> & start,
      const std::vector<unsigned int> & stride,
      const std::vector<unsigned int> & dimensions,
      const std::vector<unsigned int> & dataspaceDimensions,
      XdmfDSMBuffer * const dsmBuffer,
      int startCoreIndex,
      int endCoreIndex);

  /**
   * Create a new controller for an DSM data set.
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
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until workerComm
   * @skipline exampleController
   * @until size-1
   *
   * @param     hdf5FilePath            The path to the hdf5 file that the controller will be accessing
   * @param     dataSetPath             The location within the file of the data the controller with be accessing
   * @param     type                    The data type of the data Ex: XdmfArrayType::Int32()
   * @param     start                   A vector of the start indexes for all dimensions of the data
   * @param     start                   A vector of the start indexes for all dimensions of the data
   * @param     stride                  A vector of the distance between reads for all dimensions of the data
   * @param     dimensions              A vector of the number of values read from all dimensions of the data
   * @param     dataspaceDimensions     A vecotr containing the total size of the dimension in the data space
   * @param     comm                    The communicator that the DSM buffer will reference
   * @param     bufferSize              The size of the buffer to be created on the core calling this function
   * @param	startCoreIndex		The index at which the server cores for the buffer start
   * @param	endCoreIndex		The index at which the server cores for the buffer end
   */
  static shared_ptr<XdmfHDF5ControllerDSM>
  New(const std::string & hdf5FilePath,
      const std::string & dataSetPath,
      const shared_ptr<const XdmfArrayType> type,
      const std::vector<unsigned int> & start,
      const std::vector<unsigned int> & stride,
      const std::vector<unsigned int> & dimensions,
      const std::vector<unsigned int> & dataspaceDimensions,
      MPI_Comm comm,
      unsigned int bufferSize,
      int startCoreIndex,
      int endCoreIndex);

  /**
   * Deletes the manager that the controller contains.
   * Used during cleanup.
   *
   * Example of Use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfDSMSelfcontained.cpp
   * @skipline providedThreading
   * @until MPI_Comm_size
   * @skipline testArray
   * @until 4*size
   * @skipline writeController
   * @until dsmSize
   * @skipline dsmManager
   * @skipline writeController->deleteManager()
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMStandalone.py
   * @skipline comm
   * @until size*4
   * @skipline coreSize
   * @until getBuffer
   * @skipline setMode
   * @until setHeavyDataController
   * @skipline accept
   * @skipline segfault
   * @skipline writeController
   */
  void deleteManager();

  /**
   * Returns the current dsmManager for the Controller. If there is no manager then it returns null
   *
   * Example of Use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfDSMSelfcontained.cpp
   * @skipline providedThreading
   * @until MPI_Comm_size
   * @skipline testArray
   * @until 4*size
   * @skipline writeController
   * @until dsmSize
   * @skipline XdmfHDF5WriterDSM::New
   * @skipline dsmManager
   * @skipline writeController->getManager()
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMStandalone.py
   * @skipline comm
   * @until size*4
   * @skipline coreSize
   * @until setManager
   * @skipline setMode
   * @until setHeavyDataController
   * @skipline accept
   * @skipline segfault
   * @skipline writeController
   *
   * @return	the dsmManager of the controller
   */
  H5FDdsmManager * getManager();

  /**
   * Returns the current dsmBuffer the Controller. If there is no manager then it returns null
   *
   * Example of Use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfDSMSelfcontained.cpp
   * @skipline providedThreading
   * @until MPI_Comm_size
   * @skipline testArray
   * @until 4*size
   * @skipline writeController
   * @until dsmSize
   * @skipline XdmfHDF5WriterDSM::New
   * @skipline dsmManager
   * @skipline writeController->getManager()
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMStandalone.py
   * @skipline comm
   * @until size*4
   * @skipline coreSize
   * @until setBuffer
   * @skipline setMode
   * @until setHeavyDataController
   * @skipline accept
   * @skipline segfault
   * @skipline writeController
   *
   * @return    the dsmBuffer of the controller
   */
  H5FDdsmBuffer * getBuffer();

  /**
   * Gets the buffer for the non-threaded version of DSM
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
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until workerComm
   * @skipline exampleController
   * @until size-1
   * @skipline if
   * @skip exampleBuffer
   * @skipline exampleController
   *
   * @return	The XdmfDSMBuffer that is controlling the data for the DSM
   */
  XdmfDSMBuffer * getServerBuffer();

  /**
   * Gets the communicator that the servers use to communicate between themselves.
   * Will be MPI_COMM_NULL on worker cores.
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
   * @skipline getServerComm
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until workerComm
   * @skipline exampleController
   * @until size-1
   * @skipline if
   * @skipline exampleController
   *
   * @return	The comm that the servers are using.
   */
  MPI_Comm getServerComm();

  /**
   * Gets the manager for the non-threaded version of DSM
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
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until workerComm
   * @skipline exampleController
   * @until size-1
   * @skipline if
   * @skip exampleManager
   * @skipline exampleController
   *
   * @return	The XdmfDSMManager that is controlling the DSM
   */
  XdmfDSMManager * getServerManager();

  /**
   * Checks if the DSM is in server mode or not.
   * True is server mode, false is threaded
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfDSMNoThread.cpp
   * @skipline size
   * @until MPI_Comm_size
   * @skipline exampleWriter
   * @skipline writeController
   * @until size-1
   * @skipline if
   * @until {
   * @skipline getServerMode
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until exampleWriter
   * @skipline if
   * @skipline writeController
   * @until size-1
   * @skipline exampleControllerServerMode
   *
   * @return	If the DSM is in server mode or not
   */
  bool getServerMode();

  std::string getName() const;

  /**
   * Gets the Communicator that the workers are using to communicate between themselves
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
   * @skipline getWorkerComm
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until workerComm
   * @skipline exampleController
   * @until size-1
   * @skipline if
   * @skipline exampleController
   * @skipline getworkerComm
   *
   * @return	The comm that the workers are using.
   */
  MPI_Comm getWorkerComm();

  /**
   * Sets the controller's dsmBuffer to the provided buffer
   *
   * Example of Use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfDSMSelfcontained.cpp
   * @skipline providedThreading
   * @until MPI_Comm_size
   * @skipline testArray
   * @until 4*size
   * @skipline writeController
   * @until dsmSize
   * @skipline XdmfHDF5WriterDSM::New
   * @until However
   * @skipline dsmManager
   * @skipline writeController->getManager()
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMStandalone.py
   * @skipline comm
   * @until size*4
   * @skipline coreSize
   * @until If
   * @skip delete
   * @skipline writeController
   * @until setBuffer
   * @skipline setMode
   * @until setHeavyDataController
   * @skipline accept
   * @skipline segfault
   * @skipline writeController
   *
   * @param     newBuffer       the buffer to be set
   */
  void setBuffer(H5FDdsmBuffer * newBuffer);

  /**
   * Sets the controller's dsmBuffer to the provided buffer
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
   * @skipline setBuffer
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until workerComm
   * @skipline exampleController
   * @until size-1
   * @skipline if
   * @skip exampleBuffer
   * @skipline exampleController
   * @until setBuffer
   *
   * @param	newBuffer	A pointer to the buffer to be set
   */
  void setBuffer(XdmfDSMBuffer * newBuffer);

  /**
   * Sets the controller's dsmManager to the provided manager.
   * Then the dsmBuffer controlled by the manager is set to the controller
   *
   * Example of Use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfDSMSelfcontained.cpp
   * @skipline providedThreading
   * @until MPI_Comm_size
   * @skipline testArray
   * @until 4*size
   * @skipline writeController
   * @until dsmSize
   * @skipline XdmfHDF5WriterDSM::New
   * @until setManager
   * @skipline dsmManager
   * @skipline writeController->getManager()
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMStandalone.py
   * @skipline comm
   * @until size*4
   * @skipline coreSize
   * @until If
   * @skip delete
   * @skipline writeController
   * @until setManager
   * @skipline setMode
   * @until setHeavyDataController
   * @skipline accept
   * @skipline segfault
   * @skipline writeController
   *
   * @param	newManager	the manager to be set
   */
  void setManager(H5FDdsmManager * newManager);

  /**
   * Sets the controller's dsmManager to the provided manager.
   * Then the dsmBuffer controlled by the manager is set to the Writer
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
   * @skipline setManager
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until workerComm
   * @skipline exampleController
   * @until size-1
   * @skipline if
   * @skip exampleManager
   * @skipline exampleController
   * @until setManager
   *
   * @param	newManager	A pointer the the manager to be set.
   */
  void setManager(XdmfDSMManager * newManager);

  /**
   * Sets the comm that the servers will use to communicate with the other server cores.
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
   * @skipline getServerComm
   * @skipline setServerComm
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until workerComm
   * @skipline exampleController
   * @until size-1
   * @skipline if
   * @skipline exampleController
   * @skipline setServerComm
   *
   * @param	comm	The communicator that the server will be using to communicate with the other server cores.
   */
  void setServerComm(MPI_Comm comm);

  /**
   * Used to switch between server and threaded mode.
   * True is server mode, false is threaded mode.
   * 
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfDSMNoThread.cpp
   * @skipline size
   * @until MPI_Comm_size
   * @skipline exampleWriter
   * @skipline writeController
   * @until size-1
   * @skipline if
   * @until {
   * @skipline setServerMode
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until exampleWriter
   * @skipline if
   * @skipline writeController
   * @until size-1
   * @skipline setServerMode
   *
   * @param	newMode		The mode that the writer is to be set to.
   */
  void setServerMode(bool newMode);

  /**
   * Sets the comm that the workers will use to communicate with other worker cores
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
   * @skipline getWorkerComm
   * @skipline setWorkerComm
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until workerComm
   * @skipline exampleController
   * @until size-1
   * @skipline if
   * @skip exampleController
   * @skipline getWorkerComm
   * @skipline setWorkerComm
   *
   * @param	comm	The communicator that the worker will be using to communicate with the other worker cores.
   */
  void setWorkerComm(MPI_Comm comm);

  /**
   * Sends a stop command to all the server cores that the controller is connected to, ending the DSM.
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
   * @skipline exampleController
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until workerComm
   * @skipline exampleController
   * @until size-1
   * @skip Section
   * @skipline if
   * @skipline exampleController
   *
   */
  void stopDSM();

  void read(XdmfArray * const array);

  /**
   * Restarts the DSM when called on server cores.
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
   * @skipline exampleController
   * @skipline }
   * @skipline exampleController
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until workerComm
   * @skipline exampleController
   * @until size-1
   * @skip Section
   * @skipline if
   * @skipline exampleController
   * @skip restartDSM
   * @skipline exampleController
   *
   */
  void restartDSM();

protected:

  XdmfHDF5ControllerDSM(const std::string & hdf5FilePath,
                        const std::string & dataSetPath,
                        const shared_ptr<const XdmfArrayType> type,
                        const std::vector<unsigned int> & start,
                        const std::vector<unsigned int> & stride,
                        const std::vector<unsigned int> & dimensions,
                        const std::vector<unsigned int> & dataspaceDimensions,
                        H5FDdsmBuffer * const dsmBuffer);

  XdmfHDF5ControllerDSM(const std::string & hdf5FilePath,
                        const std::string & dataSetPath,
                        const shared_ptr<const XdmfArrayType> type,
                        const std::vector<unsigned int> & start,
                        const std::vector<unsigned int> & stride,
                        const std::vector<unsigned int> & dimensions,
                        const std::vector<unsigned int> & dataspaceDimensions,
                        MPI_Comm comm,
                        unsigned int bufferSize);

  XdmfHDF5ControllerDSM(const std::string & hdf5FilePath,
                        const std::string & dataSetPath,
                        const shared_ptr<const XdmfArrayType> type,
                        const std::vector<unsigned int> & start,
                        const std::vector<unsigned int> & stride,
                        const std::vector<unsigned int> & dimensions,
                        const std::vector<unsigned int> & dataspaceDimensions,
                        MPI_Comm comm,
                        unsigned int bufferSize,
                        int startCoreIndex,
                        int endCoreIndex);

  XdmfHDF5ControllerDSM(const std::string & hdf5FilePath,
                        const std::string & dataSetPath,
                        const shared_ptr<const XdmfArrayType> type,
                        const std::vector<unsigned int> & start,
                        const std::vector<unsigned int> & stride,
                        const std::vector<unsigned int> & dimensions,
                        const std::vector<unsigned int> & dataspaceDimensions,
                        XdmfDSMBuffer * const dsmBuffer,
                        int startCoreIndex,
                        int endCoreIndex);

private:

  XdmfHDF5ControllerDSM(const XdmfHDF5Controller &);  // Not implemented.
  void operator=(const XdmfHDF5Controller &);  // Not implemented.

  H5FDdsmBuffer * mDSMBuffer;
  H5FDdsmManager * mDSMManager;

  XdmfDSMBuffer * mDSMServerBuffer;
  XdmfDSMManager * mDSMServerManager;
  MPI_Comm mGroupComm;
  MPI_Comm mServerComm;
  MPI_Comm mWorkerComm;
  int mStartCoreIndex;
  int mEndCoreIndex;
  int mRank;
  int mGroupSize;
  bool mServerMode;
};

#endif /* XDMFHDF5CONTROLLER_HPP_ */
