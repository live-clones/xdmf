/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfHDF5WriterDSM.cpp                                               */
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

#include <H5FDdsm.h>
#include <hdf5.h>
#include <H5FDdsmManager.h>
#include <H5FDdsmBuffer.h>
#include "XdmfHDF5ControllerDSM.hpp"
#include "XdmfHDF5WriterDSM.hpp"

shared_ptr<XdmfHDF5WriterDSM>
XdmfHDF5WriterDSM::New(const std::string & filePath,
                       H5FDdsmBuffer * const dsmBuffer)
{
  shared_ptr<XdmfHDF5WriterDSM> p(new XdmfHDF5WriterDSM(filePath,
                                                        dsmBuffer));
  return p;
}

shared_ptr<XdmfHDF5WriterDSM>
XdmfHDF5WriterDSM::New(const std::string & filePath,
                       MPI_Comm comm,
                       unsigned int bufferSize)
{
  shared_ptr<XdmfHDF5WriterDSM> p(new XdmfHDF5WriterDSM(filePath,
                                                        comm,
                                                        bufferSize));
  return p;
}

XdmfHDF5WriterDSM::XdmfHDF5WriterDSM(const std::string & filePath,
                                     H5FDdsmBuffer * const dsmBuffer) :
  XdmfHDF5Writer(filePath),
  mDSMBuffer(dsmBuffer),
  mFAPL(-1)
{
}

XdmfHDF5WriterDSM::XdmfHDF5WriterDSM(const std::string & filePath,
                                     MPI_Comm comm,
                                     unsigned int bufferSize) :
  XdmfHDF5Writer(filePath),
  mFAPL(-1)
{
        H5FDdsmManager * newManager = new H5FDdsmManager();
        newManager->SetMpiComm(comm);
        newManager->SetLocalBufferSizeMBytes(bufferSize);
        newManager->SetIsStandAlone(H5FD_DSM_TRUE);
        newManager->Create();

        H5FD_dsm_set_manager(newManager);

        H5FD_dsm_set_options(H5FD_DSM_LOCK_ASYNCHRONOUS);

        H5FDdsmBuffer * newBuffer = newManager->GetDsmBuffer();
	mDSMManager = newManager;
	mDSMBuffer = newBuffer;
}

XdmfHDF5WriterDSM::~XdmfHDF5WriterDSM()
{
}

shared_ptr<XdmfHDF5Controller>
XdmfHDF5WriterDSM::createHDF5Controller(const std::string & hdf5FilePath,
                                        const std::string & dataSetPath,
                                        const shared_ptr<const XdmfArrayType> type,
                                        const std::vector<unsigned int> & start,
                                        const std::vector<unsigned int> & stride,
                                        const std::vector<unsigned int> & dimensions,
                                        const std::vector<unsigned int> & dataspaceDimensions)
{
  return XdmfHDF5ControllerDSM::New(hdf5FilePath,
                                    dataSetPath,
                                    type,
                                    start,
                                    stride,
                                    dimensions,
                                    dataspaceDimensions,
                                    mDSMBuffer);
}

H5FDdsmManager * XdmfHDF5WriterDSM::getManager()
{
  return mDSMManager;
}

H5FDdsmBuffer * XdmfHDF5WriterDSM::getBuffer()
{
  return mDSMBuffer;
}

void XdmfHDF5WriterDSM::setManager(H5FDdsmManager * newManager)
{
  H5FDdsmBuffer * newBuffer = newManager->GetDsmBuffer();
  mDSMManager = newManager;
  mDSMBuffer = newBuffer;
}

void XdmfHDF5WriterDSM::setBuffer(H5FDdsmBuffer * newBuffer)
{
  mDSMBuffer = newBuffer;
}

void XdmfHDF5WriterDSM::deleteManager()
{
	if (mDSMManager != NULL)
	{
		delete mDSMManager;
	}
}

void 
XdmfHDF5WriterDSM::closeFile()
{
  if(mFAPL >= 0) {
    herr_t status = H5Pclose(mFAPL);
    mFAPL = -1;
  }
  XdmfHDF5Writer::closeFile();
}

void 
XdmfHDF5WriterDSM::openFile()
{
  if(mFAPL >= 0) {
    this->closeFile();
  }

  // Set file access property list for DSM
  mFAPL = H5Pcreate(H5P_FILE_ACCESS);

  // Use DSM driver
  H5Pset_fapl_dsm(mFAPL, MPI_COMM_WORLD, mDSMBuffer, 0);

  XdmfHDF5Writer::openFile(mFAPL);
}

void XdmfHDF5WriterDSM::visit(XdmfArray & array,
                              const shared_ptr<XdmfBaseVisitor>)
{
  bool closeFAPL = false;
  
  if(mFAPL < 0) {

    // Set file access property list for DSM
    mFAPL = H5Pcreate(H5P_FILE_ACCESS);

    // Use DSM driver
    H5Pset_fapl_dsm(mFAPL, MPI_COMM_WORLD, mDSMBuffer, 0);

    closeFAPL = true;
  }

  // Write to DSM Buffer
  this->write(array, mFAPL);

  if(closeFAPL) {
    // Close file access property list
    herr_t status = H5Pclose(mFAPL);
    mFAPL = -1;
  }
}
