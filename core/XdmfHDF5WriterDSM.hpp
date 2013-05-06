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
   * @until setHeavyDataController
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
   * @until setHeavyDataController
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
   * IsStandAlone =H5FD_DSM_TRUE
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
   * @until setHeavyDataController
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
   * @until setHeavyDataController
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


  virtual ~XdmfHDF5WriterDSM();

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
   * @until setHeavyDataController
   * @skipline accept
   * @skipline segfault
   * @skipline exampleWriter
   *
   * @return    the dsmManager of the Writer
   */
  H5FDdsmManager * getManager();

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
   * @until setHeavyDataController
   * @skipline accept
   * @skipline segfault
   * @skipline exampleWriter
   *
   * @return    the dsmBuffer of the Writer
   */
  H5FDdsmBuffer * getBuffer();

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
   * @until setHeavyDataController
   * @skipline accept
   * @skipline segfault
   * @skipline exampleWriter
   *
   * @param     newManager      the manager to be set
   */
  void setManager(H5FDdsmManager * newManager);

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
   * @until setHeavyDataController
   * @skipline accept
   * @skipline segfault
   * @skipline exampleWriter
   *
   * @param     newBuffer       the buffer to be set
   */
  void setBuffer(H5FDdsmBuffer * newBuffer);

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
   * @until setHeavyDataController
   * @skipline accept
   * @skipline segfault
   * @skipline exampleWriter
   */
  void deleteManager();

  void closeFile();

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

};

#endif /* XDMFHDF5WRITERDSM_HPP_ */
