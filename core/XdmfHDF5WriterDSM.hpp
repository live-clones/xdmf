/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfHDF5WriterDSM.hpp                                               */
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

#ifndef XDMFHDF5WRITERDSM_HPP_
#define XDMFHDF5WRITERDSM_HPP_

// Forward Declarations
class H5FDdsmBuffer;
class H5FDdsmManager;

// Includes
#include "XdmfHDF5Writer.hpp"
#include <XdmfDSMCommMPI.hpp>
#include <XdmfDSMBuffer.hpp>
#include <XdmfDSMManager.hpp>

/**
 * @brief Traverse the Xdmf graph and write heavy data stored in
 * XdmfArrays to a DSM buffer.
 *
 * XdmfHDF5WriterDSM traverses an Xdmf graph structure and writes data
 * stored in XdmfArrays to a DSM buffer. Writing begins by calling the
 * accept() operation on any XdmfItem and supplying this writer as the
 * parameter. The writer will write all XdmfArrays under the XdmfItem
 * to a DSM Buffer. It will also attach an XdmfHDF5ControllerDSM to
 * all XdmfArrays.
 *
 * This writer supports all heavy data writing modes listed in
 * XdmfHeavyDataWriter.
 */
class XdmfHDF5WriterDSM : public XdmfHDF5Writer {

public:

  /**
   * Construct XdmfHDF5WriterDSM
   *
   * Currently the write requires all cores to write and will freeze otherwise.
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
   * @until readController
   * @skipline newSetPath
   * @skipline array
   * @until initialize
   * @skipline for
   * @until insert
   * @skipline accept
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
   * @until insert
   * @skipline accept
   * @skipline segfault
   * @skipline exampleWriter
   *
   * @param filePath the location of the hdf5 file to output to on disk.
   * @param dsmBuffer the dsm buffer to write to.
   * @return new XdmfHDF5WriterDSM.
   */
  static shared_ptr<XdmfHDF5WriterDSM>
  New(const std::string & filePath,
      H5FDdsmBuffer * const dsmBuffer);

  /**
   * Construct XdmfHDF5WriterDSM
   *
   * Currently the write requires all cores to write and will freeze otherwise. This version creates a DSM buffer in the provided com of the size provided.
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
   * @until newSetPath
   * @skipline exampleWriter
   * @skip /*
   * @skipline writeController
   * @until getBuffer
   * @skipline setMode
   * @until insert
   * @skipline accept
   * @skipline dsmManager
   * @skipline exampleWriter->getManager()
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMStandalone.py
   * @skipline comm
   * @until size*4
   * @skipline coreSize
   * @until getBuffer
   * @skipline setMode
   * @until insert
   * @skipline accept
   * @skipline segfault
   * @skipline exampleWriter
   *
   * @param	filePath	the location of the hdf5 file to output to on disk.
   * @param	comm		the communicator that the buffer will be created in.
   * @param	bufferSize	the size of the created buffer.
   * @return new XdmfHDF5WriterDSM.
   */
  static shared_ptr<XdmfHDF5WriterDSM>
  New(const std::string & filePath,
            MPI_Comm comm,
            unsigned int bufferSize);
  /**
   * Contruct XdmfHDF5WriterDSM, nonthreaded version
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
   * @skipline exampleWriter2
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until exampleWriter
   * @skipline if
   * @skipline exampleWriter2
   *
   * @param	filePath	The location of the hdf5 file to output to on disk.
   * @param	dsmBuffer	The Buffer to write to.
   * @param	startCoreIndex	The index of the first core in the server block
   * @param	endCoreIndex	The index of the last core in the server block.
   * @return			a New XdmfHDF5WriterDSM
   */
  static shared_ptr<XdmfHDF5WriterDSM>
  New(const std::string & filePath,
            XdmfDSMBuffer * const dsmBuffer,
            int startCoreIndex,
            int endCoreIndex);

  /**
   * Contruct XdmfHDF5WriterDSM, nonthreaded version
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfDSMNoThread.cpp
   * @skipline size
   * @until MPI_Comm_size
   * @skipline exampleWriter
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline size
   * @until MPI_Comm_size
   * @skipline exampleWriter
   *
   * @param	filePath	The location of the hdf5 file to output to on disk.
   * @param     comm            the communicator that the buffer will be created in.
   * @param     bufferSize      the size of the created buffer.
   * @param	startCoreIndex	The index of the first core in the server block
   * @param	endCoreIndex	The index of the last core in the server block
   * @return			a New XdmfHDF5WriterDSM
   */
  static shared_ptr<XdmfHDF5WriterDSM>
  New(const std::string & filePath,
            MPI_Comm comm,
            unsigned int bufferSize,
            int startCoreIndex,
            int endCoreIndex);


  virtual ~XdmfHDF5WriterDSM();

  /**
   * Deletes the manager that the writer contains.
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
   * @until newSetPath
   * @skipline exampleWriter
   * @skip /*
   * @skipline writeController
   * @until getBuffer
   * @skipline exampleWriter->deleteManager
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMStandalone.py
   * @skipline comm
   * @until size*4
   * @skipline coreSize
   * @until getBuffer
   * @skipline setMode
   * @until insert
   * @skipline accept
   * @skipline segfault
   * @skipline exampleWriter
   */
  void deleteManager();

  void closeFile();

  /**
   * Returns the current dsmBuffer the Writer. If there is no manager then it returns null
   *
   * Example of Use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfDSMSelfcontained.cpp
   * @skipline providedThreading
   * @until MPI_Comm_size
   * @skipline testArray
   * @until newSetPath
   * @skipline exampleWriter
   * @skip /*
   * @skipline writeController
   * @until getBuffer
   * @skipline exampleWriter->getManager()
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMStandalone.py
   * @skipline comm
   * @until size*4
   * @skipline coreSize
   * @until setBuffer
   * @skipline setMode
   * @until insert
   * @skipline accept
   * @skipline segfault
   * @skipline exampleWriter
   *
   * @return    the dsmBuffer of the Writer
   */
  H5FDdsmBuffer * getBuffer();

  /**
   * Returns the current dsmManager for the Writer. If there is no manager then it returns null
   *
   * Example of Use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfDSMSelfcontained.cpp
   * @skipline providedThreading
   * @until MPI_Comm_size
   * @skipline exampleWriter
   * @skipline dsmManager
   * @skipline exampleWriter->getManager()
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMStandalone.py
   * @skipline comm
   * @until size*4
   * @skipline coreSize
   * @until setManager
   * @skipline setMode
   * @until insert
   * @skipline accept
   * @skipline segfault
   * @skipline exampleWriter
   *
   * @return    the dsmManager of the Writer
   */
  H5FDdsmManager * getManager();

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
   * @skipline exampleWriter
   * @skipline if
   * @until {
   * @skipline exampleBuffer
   * @skipline exampleWriter
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
   * @skipline exampleWriter
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
   * @until exampleWriter
   * @skipline if
   * @skipline getServerComm
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
   * @skipline exampleWriter
   * @skipline if
   * @until {
   * @skipline exampleManager
   * @skipline exampleWriter
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
   * @skipline getServerMode
   *
   * @return	If the DSM is in server mode or not
   */
  bool getServerMode();

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
   * @skipline exampleWriter
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
   * @until exampleWriter
   * @skipline if
   * @skipline getWorkerComm
   *
   * @return	The comm that the workers are using.
   */
  MPI_Comm getWorkerComm();

  /**
   * Sets the Writer's dsmBuffer to the provided buffer
   *
   * Example of Use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfDSMSelfcontained.cpp
   * @skipline providedThreading
   * @until MPI_Comm_size
   * @skipline testArray
   * @until newSetPath
   * @skipline exampleWriter
   * @skip /*
   * @skipline writeController
   * @until However
   * @skipline exampleWriter->getManager()
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
   * @until insert
   * @skipline accept
   * @skipline segfault
   * @skipline exampleWriter
   *
   * @param     newBuffer       the buffer to be set
   */
  void setBuffer(H5FDdsmBuffer * newBuffer);

  /**
   * Sets the Writer's dsmBuffer to the provided buffer
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
   * @skipline exampleBuffer
   * @skipline exampleWriter
   * @skipline setBuffer
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
   * @skipline SetBuffer
   *
   * @param	newBuffer	A pointer to the buffer to be set
   */
  void setBuffer(XdmfDSMBuffer * newBuffer);

  /**
   * Sets the Writer's dsmManager to the provided manager.
   * Then the dsmBuffer controlled by the manager is set to the Writer
   *
   * Example of Use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfDSMSelfcontained.cpp
   * @skipline providedThreading
   * @until MPI_Comm_size
   * @skipline testArray
   * @until newSetPath
   * @skipline exampleWriter
   * @skip /*
   * @skipline writeController
   * @until setManager
   * @skipline exampleWriter->getManager()
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
   * @until insert
   * @skipline accept
   * @skipline segfault
   * @skipline exampleWriter
   *
   * @param     newManager      the manager to be set
   */
  void setManager(H5FDdsmManager * newManager);

  /**
   * Sets the Writer's dsmManager to the provided manager.
   * Then the dsmBuffer controlled by the manager is set to the Writer
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
   * @skipline setManager
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
   * @skipline SetManager
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
   * @skipline exampleWriter
   * @skipline if
   * @until {
   * @skipline getServerComm
   * @until setServerComm
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until exampleWriter
   * @skipline if
   * @skipline getServerComm
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
   * @skipline exampleWriter
   * @skipline if
   * @until {
   * @skipline getWorkerComm
   * @until setWorkerComm
   * @skip Section
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until exampleWriter
   * @skipline if
   * @skipline getWorkerComm
   * @skipline setWorkerComm
   *
   * @param	comm	The communicator that the worker will be using to communicate with the other worker cores.
   */
  void setWorkerComm(MPI_Comm comm);

  /**
   * Sends a stop command to all the server cores that the writer is connected to, ending the DSM.
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
   * @until stopDSM
   * @skipline }
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until exampleWriter
   * @skip Section
   * @skipline if
   * @skipline exampleWriter
   *
   */
  void stopDSM();

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
   * @until stopDSM
   * @skipline }
   * @skipline restartDSM
   *
   * Python
   *
   * @dontinclude XdmfExampleDSMNoThread.py
   * @skipline total
   * @until exampleWriter
   * @skip Section
   * @skipline if
   * @skipline exampleWriter
   * @skipline restartDSM
   *
   */
  void restartDSM();

  void openFile();

  using XdmfHeavyDataWriter::visit;
  void visit(XdmfArray & array,
             const shared_ptr<XdmfBaseVisitor> visitor);

protected:

  XdmfHDF5WriterDSM(const std::string & filePath,
                    H5FDdsmBuffer * const dsmBuffer);

  XdmfHDF5WriterDSM(const std::string & filePath,
                    MPI_Comm comm,
                    unsigned int bufferSize);

  XdmfHDF5WriterDSM(const std::string & filePath,
                    XdmfDSMBuffer * const dsmBuffer,
                    int startCoreIndex,
                    int endCoreIndex);

  XdmfHDF5WriterDSM(const std::string & filePath,
                    MPI_Comm comm,
                    unsigned int bufferSize,
                    int startCoreIndex,
                    int endCoreIndex);

  virtual shared_ptr<XdmfHDF5Controller>
  createHDF5Controller(const std::string & hdf5FilePath,
                       const std::string & dataSetPath,
                       const shared_ptr<const XdmfArrayType> type,
                       const std::vector<unsigned int> & start,
                       const std::vector<unsigned int> & stride,
                       const std::vector<unsigned int> & dimensions,
                       const std::vector<unsigned int> & dataspaceDimensions);

private:

  XdmfHDF5WriterDSM(const XdmfHDF5WriterDSM &);  // Not implemented.
  void operator=(const XdmfHDF5WriterDSM &);  // Not implemented.

  H5FDdsmBuffer * mDSMBuffer;
  H5FDdsmManager * mDSMManager;
  int mFAPL;

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

#endif /* XDMFHDF5WRITERDSM_HPP_ */
