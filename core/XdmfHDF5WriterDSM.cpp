// Kenneth Leiter
// Xdmf Smart Pointer Test

#include <H5FDdsm.h>
#include <hdf5.h>
#include "XdmfHDF5ControllerDSM.hpp"
#include "XdmfHDF5WriterDSM.hpp"

boost::shared_ptr<XdmfHDF5WriterDSM>
XdmfHDF5WriterDSM::New(const std::string & filePath,
                       H5FDdsmBuffer * const dsmBuffer)
{
  boost::shared_ptr<XdmfHDF5WriterDSM> p(new XdmfHDF5WriterDSM(filePath,
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

boost::shared_ptr<XdmfHDF5Controller>
XdmfHDF5WriterDSM::createHDF5Controller(const std::string & hdf5FilePath,
                                        const std::string & dataSetPath,
                                        const unsigned int size,
                                        const boost::shared_ptr<const XdmfArrayType> type)
{
  return XdmfHDF5ControllerDSM::New(hdf5FilePath,
                                    dataSetPath,
                                    size,
                                    type,
                                    mDSMBuffer);
}

void XdmfHDF5WriterDSM::visit(XdmfArray & array,
                              const boost::shared_ptr<XdmfBaseVisitor>)
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
