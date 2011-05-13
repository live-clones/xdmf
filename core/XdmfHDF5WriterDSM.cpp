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

XdmfHDF5WriterDSM::XdmfHDF5WriterDSM(const std::string & filePath,
                                     H5FDdsmBuffer * const dsmBuffer) :
  XdmfHDF5Writer(filePath),
  mDSMBuffer(dsmBuffer)
{
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
                                        const std::vector<unsigned int> & count)
{
  return XdmfHDF5ControllerDSM::New(hdf5FilePath,
                                    dataSetPath,
                                    type,
                                    start,
                                    stride,
                                    count,
                                    mDSMBuffer);
}

void XdmfHDF5WriterDSM::visit(XdmfArray & array,
                              const shared_ptr<XdmfBaseVisitor>)
{
  // Set file access property list for DSM
  hid_t fapl = H5Pcreate(H5P_FILE_ACCESS);

  // Use DSM driver
  H5Pset_fapl_dsm(fapl, MPI_COMM_WORLD, mDSMBuffer);

  // Write to DSM Buffer
  this->write(array, fapl);

  // Close file access property list
  herr_t status = H5Pclose(fapl);
}
