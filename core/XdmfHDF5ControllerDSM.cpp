// Kenneth Leiter
// Xdmf Smart Pointer Test

#include <H5FDdsm.h>
#include <hdf5.h>
#include "XdmfHDF5ControllerDSM.hpp"

boost::shared_ptr<XdmfHDF5ControllerDSM>
XdmfHDF5ControllerDSM::New(const std::string & hdf5FilePath,
                           const std::string & dataSetPath,
                           const unsigned int size,
                           const boost::shared_ptr<const XdmfArrayType> type,
                           H5FDdsmBuffer * const dsmBuffer)
{
  boost::shared_ptr<XdmfHDF5ControllerDSM>
    p(new XdmfHDF5ControllerDSM(hdf5FilePath,
                                dataSetPath,
                                size,
                                type,
                                dsmBuffer));
  return p;
}

XdmfHDF5ControllerDSM::XdmfHDF5ControllerDSM(const std::string & hdf5FilePath,
                                             const std::string & dataSetPath,
                                             const unsigned int size,
                                             const boost::shared_ptr<const XdmfArrayType> type,
                                             H5FDdsmBuffer * const dsmBuffer) :
  XdmfHDF5Controller(hdf5FilePath, dataSetPath, size, type),
  mDSMBuffer(dsmBuffer)
{
}

XdmfHDF5ControllerDSM::~XdmfHDF5ControllerDSM()
{
}

std::string XdmfHDF5ControllerDSM::getName() const
{
  return "HDFDSM";
}

void XdmfHDF5ControllerDSM::read(XdmfArray * const array)
{
  // Set file access property list for DSM
  hid_t fapl = H5Pcreate(H5P_FILE_ACCESS);

  // Use DSM driver
  H5Pset_fapl_dsm(fapl, MPI_COMM_WORLD, mDSMBuffer);

  // Read from DSM Buffer
  XdmfHDF5Controller::read(array, fapl);

  // Close file access property list
  herr_t status = H5Pclose(fapl);
}
