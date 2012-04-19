/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfHDF5Writer.cpp                                                  */
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

#include <hdf5.h>
#include <sstream>
#include <cstdio>
#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfError.hpp"
#include "XdmfHDF5Controller.hpp"
#include "XdmfHDF5Writer.hpp"

/**
 * PIMPL
 */
class XdmfHDF5Writer::XdmfHDF5WriterImpl {

public:

  XdmfHDF5WriterImpl():
    mHDF5Handle(-1)
  {
  };

  ~XdmfHDF5WriterImpl()
  {
    closeFile();
  };

  void
  closeFile()
  {
    if(mHDF5Handle >= 0) {
      herr_t status = H5Fclose(mHDF5Handle);
      mHDF5Handle = -1;
    }
  };  

  void
  openFile(const std::string & filePath)
  {

    if(mHDF5Handle >= 0) {
      // Perhaps we should throw a warning.
      closeFile();
    }

    // Save old error handler and turn off error handling for now
    H5E_auto_t old_func;
    void * old_client_data;
    H5Eget_auto(0, &old_func, &old_client_data);
    H5Eset_auto2(0, NULL, NULL);
  
    int fapl = H5P_DEFAULT;

    if(H5Fis_hdf5(filePath.c_str()) > 0) {
      mHDF5Handle = H5Fopen(filePath.c_str(), 
                            H5F_ACC_RDWR, 
                            fapl);
    }
    else {
      mHDF5Handle = H5Fcreate(filePath.c_str(),
                              H5F_ACC_TRUNC,
                              H5P_DEFAULT,
                              fapl);
    }

    // Restore previous error handler
    H5Eset_auto2(0, old_func, old_client_data);

  }

  hid_t mHDF5Handle;

};

shared_ptr<XdmfHDF5Writer>
XdmfHDF5Writer::New(const std::string & filePath,
                    const bool clobberFile)
{
  if(clobberFile) {
    std::remove(filePath.c_str());
  }
  shared_ptr<XdmfHDF5Writer> p(new XdmfHDF5Writer(filePath));
  return p;
}

XdmfHDF5Writer::XdmfHDF5Writer(const std::string & filePath) :
  XdmfHeavyDataWriter(filePath),
  mImpl(new XdmfHDF5WriterImpl())
{
}

XdmfHDF5Writer::~XdmfHDF5Writer()
{
  delete mImpl;
}

shared_ptr<XdmfHDF5Controller>
XdmfHDF5Writer::createHDF5Controller(const std::string & hdf5FilePath,
                                     const std::string & dataSetPath,
                                     const shared_ptr<const XdmfArrayType> type,
                                     const std::vector<unsigned int> & start,
                                     const std::vector<unsigned int> & stride,
                                     const std::vector<unsigned int> & count)
{
  return XdmfHDF5Controller::New(hdf5FilePath,
                                 dataSetPath,
                                 type,
                                 start,
                                 stride,
                                 count);
}

void 
XdmfHDF5Writer::closeFile()
{
  mImpl->closeFile();
}

void 
XdmfHDF5Writer::openFile()
{
  mImpl->openFile(mFilePath);
}

void
XdmfHDF5Writer::visit(XdmfArray & array,
                      const shared_ptr<XdmfBaseVisitor> visitor)
{
  this->write(array, H5P_DEFAULT);
}

void
XdmfHDF5Writer::write(XdmfArray & array,
                      const int fapl)
{

  hid_t datatype = -1;

  if(array.isInitialized()) {
    if(array.getArrayType() == XdmfArrayType::Int8()) {
      datatype = H5T_NATIVE_CHAR;
    }
    else if(array.getArrayType() == XdmfArrayType::Int16()) {
      datatype = H5T_NATIVE_SHORT;
    }
    else if(array.getArrayType() == XdmfArrayType::Int32()) {
      datatype = H5T_NATIVE_INT;
    }
    else if(array.getArrayType() == XdmfArrayType::Int64()) {
      datatype = H5T_NATIVE_LONG;
    }
    else if(array.getArrayType() == XdmfArrayType::Float32()) {
      datatype = H5T_NATIVE_FLOAT;
    }
    else if(array.getArrayType() == XdmfArrayType::Float64()) {
      datatype = H5T_NATIVE_DOUBLE;
    }
    else if(array.getArrayType() == XdmfArrayType::UInt8()) {
      datatype = H5T_NATIVE_UCHAR;
    }
    else if(array.getArrayType() == XdmfArrayType::UInt16()) {
      datatype = H5T_NATIVE_USHORT;
    }
    else if(array.getArrayType() == XdmfArrayType::UInt32()) {
      datatype = H5T_NATIVE_UINT;
    }
    else {
      XdmfError::message(XdmfError::FATAL,
                         "Array of unsupported type in "
                         "XdmfHDF5Writer::write");
    }
  }

  if(datatype != -1) {
    std::string hdf5FilePath = mFilePath;

    std::stringstream dataSetPath;

    if((mMode == Overwrite || mMode == Append)
       && array.getHeavyDataController()) {
      // Write to the previous dataset
      dataSetPath << array.getHeavyDataController()->getDataSetPath();
      hdf5FilePath = array.getHeavyDataController()->getFilePath();
    }
    else {
      dataSetPath << "Data" << mDataSetId;
    }

    const std::vector<unsigned int> & dimensions = array.getDimensions();

    // Open a hdf5 dataset and write to it on disk.
    herr_t status;
    hsize_t size = array.getSize();

    // Save old error handler and turn off error handling for now
    H5E_auto_t old_func;
    void * old_client_data;
    H5Eget_auto(0, &old_func, &old_client_data);
    H5Eset_auto2(0, NULL, NULL);
   
    bool closeFile = false;
    if(mImpl->mHDF5Handle < 0) {
      mImpl->openFile(hdf5FilePath);
      closeFile = true;
    }

    hid_t dataset = H5Dopen(mImpl->mHDF5Handle,
                            dataSetPath.str().c_str(),
                            H5P_DEFAULT);

    // if default mode find a new data set to write to (keep
    // incrementing dataSetId)
    while(dataset >= 0 && mMode == Default) {
      dataSetPath.str(std::string());
      dataSetPath << "Data" << ++mDataSetId;
      dataset = H5Dopen(mImpl->mHDF5Handle,
                        dataSetPath.str().c_str(),
                        H5P_DEFAULT);
    }

    // Restore previous error handler
    H5Eset_auto2(0, old_func, old_client_data);

    hid_t dataspace = H5S_ALL;
    hid_t memspace = H5S_ALL;

    std::vector<hsize_t> current_dims(dimensions.begin(), dimensions.end());

    if(dataset < 0) {
      std::vector<hsize_t> maximum_dims(dimensions.size(), H5S_UNLIMITED);
      memspace = H5Screate_simple(dimensions.size(),
                                  &current_dims[0],
                                  &maximum_dims[0]);
      hid_t property = H5Pcreate(H5P_DATASET_CREATE);
      std::vector<hsize_t> chunk_size(dimensions.size(), 1024);
      status = H5Pset_chunk(property, dimensions.size(), &chunk_size[0]);
      dataset = H5Dcreate(mImpl->mHDF5Handle,
                          dataSetPath.str().c_str(),
                          datatype,
                          memspace,
                          H5P_DEFAULT,
                          property,
                          H5P_DEFAULT);
      status = H5Pclose(property);
    }
    else {
      // Need to resize dataset to fit new data
      if(mMode == Append) {
        // Get size of old dataset
        dataspace = H5Dget_space(dataset);
        hssize_t datasize = H5Sget_simple_extent_npoints(dataspace);
        status = H5Sclose(dataspace);

        // Resize to fit size of old and new data.
        hsize_t newSize = size + datasize;
        status = H5Dset_extent(dataset, &newSize);

        // Select hyperslab to write to.
        memspace = H5Screate_simple(1, &size, NULL);
        dataspace = H5Dget_space(dataset);
        hsize_t start = datasize;
        status = H5Sselect_hyperslab(dataspace,
                                     H5S_SELECT_SET,
                                     &start,
                                     NULL,
                                     &size,
                                     NULL) ;
      }
      else {
        // Overwriting - dataset rank must remain the same (hdf5 constraint)
        hid_t dataspace = H5Dget_space(dataset);

        const unsigned int ndims = H5Sget_simple_extent_ndims(dataspace);
        if(ndims != current_dims.size())
          XdmfError::message(XdmfError::FATAL, \
                             "Data set rank different -- ndims != "
                             "current_dims.size() -- in "
                             "XdmfHDF5Writer::write");

        status = H5Dset_extent(dataset, &current_dims[0]);

        if(status < 0) {
          XdmfError::message(XdmfError::FATAL,
                            "H5Dset_extent returned failure in "
                             "XdmfHDF5Writer::write -- status: " + status);
        }
      }
    }
    status = H5Dwrite(dataset,
                      datatype,
                      memspace,
                      dataspace,
                      H5P_DEFAULT,
                      array.getValuesInternal());

    if(status < 0) {
      XdmfError::message(XdmfError::FATAL, 
                         "H5Dwrite returned failure in XdmfHDF5Writer::write "
                         "-- status: " + status);
    }

    if(dataspace != H5S_ALL) {
      status = H5Sclose(dataspace);
    }
    if(memspace != H5S_ALL) {
      status = H5Sclose(memspace);
    }
    status = H5Dclose(dataset);
    if(closeFile) {
      mImpl->closeFile();
    }

    // Attach a new controller to the array
    shared_ptr<XdmfHDF5Controller> newDataController =
      shared_ptr<XdmfHDF5Controller>();

    unsigned int newSize = array.getSize();
    if(mMode == Append && array.getHeavyDataController()) {
      newSize = array.getSize() + array.getHeavyDataController()->getSize();
      newDataController =
        this->createHDF5Controller(hdf5FilePath,
                                   dataSetPath.str(),
                                   array.getArrayType(),
                                   std::vector<unsigned int>(1, 0),
                                   std::vector<unsigned int>(1, 1),
                                   std::vector<unsigned int>(1, newSize));
    }

    if(mMode == Default || !array.getHeavyDataController()) {
      ++mDataSetId;
    }

    if(!newDataController) {
      newDataController =
        this->createHDF5Controller(hdf5FilePath,
                                   dataSetPath.str(),
                                   array.getArrayType(),
                                   std::vector<unsigned int>(dimensions.size(),
                                                             0),
                                   std::vector<unsigned int>(dimensions.size(),
                                                             1),
                                   dimensions);
    }
    array.setHeavyDataController(newDataController);

    if(mReleaseData) {
      array.release();
    }
  }
}
