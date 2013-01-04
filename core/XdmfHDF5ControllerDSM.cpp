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

#include <H5FDdsm.h>
#include <H5FDdsmManager.h>
#include <hdf5.h>
#include "XdmfHDF5ControllerDSM.hpp"

shared_ptr<XdmfHDF5ControllerDSM>
XdmfHDF5ControllerDSM::New(const std::string & hdf5FilePath,
                           const std::string & dataSetPath,
                           const shared_ptr<const XdmfArrayType> type,
                           const std::vector<unsigned int> & start,
                           const std::vector<unsigned int> & stride,
                           const std::vector<unsigned int> & dimensions,
                           const std::vector<unsigned int> & datspaceDimensions,
                           H5FDdsmBuffer * const dsmBuffer)
{
  shared_ptr<XdmfHDF5ControllerDSM> 
    p(new XdmfHDF5ControllerDSM(hdf5FilePath,
                                dataSetPath,
                                type,
                                start,
                                stride,
                                dimensions,
                                datspaceDimensions,
                                dsmBuffer));
  return p;
}

shared_ptr<XdmfHDF5ControllerDSM>
XdmfHDF5ControllerDSM::New(const std::string & hdf5FilePath,
                           const std::string & dataSetPath,
                           const shared_ptr<const XdmfArrayType> type,
                           const std::vector<unsigned int> & start,
                           const std::vector<unsigned int> & stride,
                           const std::vector<unsigned int> & dimensions,
                           const std::vector<unsigned int> & datspaceDimensions,
                           MPI_Comm comm,
                           unsigned int bufferSize)
{
  shared_ptr<XdmfHDF5ControllerDSM>
    p(new XdmfHDF5ControllerDSM(hdf5FilePath,
                                dataSetPath,
                                type,
                                start,
                                stride,
                                dimensions,
                                datspaceDimensions,
                                comm,
                                bufferSize));
  return p;
}


XdmfHDF5ControllerDSM::XdmfHDF5ControllerDSM(const std::string & hdf5FilePath,
                                             const std::string & dataSetPath,
                                             const shared_ptr<const XdmfArrayType> type,
                                             const std::vector<unsigned int> & start,
                                             const std::vector<unsigned int> & stride,
                                             const std::vector<unsigned int> & dimensions,
                                             const std::vector<unsigned int> & dataspaceDimensions,
                                             H5FDdsmBuffer * const dsmBuffer) :
  XdmfHDF5Controller(hdf5FilePath, 
                     dataSetPath, 
                     type, 
                     start,
                     stride,
                     dimensions, 
                     dataspaceDimensions),
  mDSMBuffer(dsmBuffer)
{
}

XdmfHDF5ControllerDSM::XdmfHDF5ControllerDSM(const std::string & hdf5FilePath,
                                             const std::string & dataSetPath,
                                             const shared_ptr<const XdmfArrayType> type,
                                             const std::vector<unsigned int> & start,
                                             const std::vector<unsigned int> & stride,
                                             const std::vector<unsigned int> & dimensions,
                                             const std::vector<unsigned int> & dataspaceDimensions,
                                             MPI_Comm comm,
                                             unsigned int bufferSize) :
  XdmfHDF5Controller(hdf5FilePath,
                     dataSetPath,
                     type,
                     start,
                     stride,
                     dimensions,
                     dataspaceDimensions)
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


XdmfHDF5ControllerDSM::~XdmfHDF5ControllerDSM()
{
}

std::string XdmfHDF5ControllerDSM::getName() const
{
  return "HDFDSM";
}

H5FDdsmManager * XdmfHDF5ControllerDSM::getManager()
{
	return mDSMManager;
}

H5FDdsmBuffer * XdmfHDF5ControllerDSM::getBuffer()
{
        return mDSMBuffer;
}

void XdmfHDF5ControllerDSM::setManager(H5FDdsmManager * newManager)
{
  H5FDdsmBuffer * newBuffer = newManager->GetDsmBuffer();
  mDSMManager = newManager;
  mDSMBuffer = newBuffer;
}

void XdmfHDF5ControllerDSM::setBuffer(H5FDdsmBuffer * newBuffer)
{
	mDSMBuffer = newBuffer;
}

void XdmfHDF5ControllerDSM::deleteManager()
{
	if (mDSMManager != NULL)
	{
		delete mDSMManager;
	}
}

void XdmfHDF5ControllerDSM::read(XdmfArray * const array)
{
  // Set file access property list for DSM
  hid_t fapl = H5Pcreate(H5P_FILE_ACCESS);

  // Use DSM driver
  H5Pset_fapl_dsm(fapl, MPI_COMM_WORLD, mDSMBuffer, 0);

  // Read from DSM Buffer
  XdmfHDF5Controller::read(array, fapl);

  // Close file access property list
  herr_t status = H5Pclose(fapl);
}
