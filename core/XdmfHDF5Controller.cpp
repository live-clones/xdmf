/*****************************************************************************/
/*                                    Xdmf                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfHDF5Controller.cpp                                              */
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
#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfError.hpp"
#include "XdmfHDF5Controller.hpp"
#include "XdmfSystemUtils.hpp"

shared_ptr<XdmfHDF5Controller>
XdmfHDF5Controller::New(const std::string & hdf5FilePath,
                        const std::string & dataSetPath,
                        const shared_ptr<const XdmfArrayType> type,
                        const std::vector<unsigned int> & start,
                        const std::vector<unsigned int> & stride,
                        const std::vector<unsigned int> & dimensions,
                        const std::vector<unsigned int> & dataspaceDimensions)
{
  shared_ptr<XdmfHDF5Controller> 
    p(new XdmfHDF5Controller(hdf5FilePath,
                             dataSetPath,
                             type,
                             start,
                             stride,
                             dimensions,
                             dataspaceDimensions));
  return p;
}

XdmfHDF5Controller::XdmfHDF5Controller(const std::string & hdf5FilePath,
                                       const std::string & dataSetPath,
                                       const shared_ptr<const XdmfArrayType> type,
                                       const std::vector<unsigned int> & start,
                                       const std::vector<unsigned int> & stride,
                                       const std::vector<unsigned int> & dimensions,
                                       const std::vector<unsigned int> & dataspaceDimensions) :
  XdmfHeavyDataController(hdf5FilePath,
                          dataSetPath,
                          type,
                          start,
                          stride,
                          dimensions,
                          dataspaceDimensions)
{
}

XdmfHDF5Controller::~XdmfHDF5Controller()
{
}

std::string
XdmfHDF5Controller::getName() const
{
  return "HDF";
}

void
XdmfHDF5Controller::read(XdmfArray * const array)
{
  this->read(array, H5P_DEFAULT);
}

void
XdmfHDF5Controller::read(XdmfArray * const array, const int fapl)
{
  herr_t status;

  hid_t hdf5Handle = H5Fopen(mFilePath.c_str(), H5F_ACC_RDONLY, fapl);
  hid_t dataset = H5Dopen(hdf5Handle, mDataSetPath.c_str(), H5P_DEFAULT);
  hid_t dataspace = H5Dget_space(dataset);

  std::vector<hsize_t> start(mStart.begin(), mStart.end());
  std::vector<hsize_t> stride(mStride.begin(), mStride.end());
  std::vector<hsize_t> count(mDimensions.begin(), mDimensions.end());

  status = H5Sselect_hyperslab(dataspace,
                               H5S_SELECT_SET,
                               &start[0],
                               &stride[0],
                               &count[0],
                               NULL);
  hssize_t numVals = H5Sget_select_npoints(dataspace);

  hid_t memspace = H5Screate_simple(mDimensions.size(),
                                    &count[0],
                                    NULL);

  /* status = H5Sselect_hyperslab(memspace,
     H5S_SELECT_SET,
     &memStart[0],
     &memStride[0],
     &memCount[0],
     NULL);*/

  hid_t datatype = H5T_NO_CLASS;
  bool closeDatatype = false;
  if(mType == XdmfArrayType::Int8()) {
    datatype = H5T_NATIVE_CHAR;
  }
  else if(mType == XdmfArrayType::Int16()) {
    datatype = H5T_NATIVE_SHORT;
  }
  else if(mType == XdmfArrayType::Int32()) {
    datatype = H5T_NATIVE_INT;
  }
  else if(mType == XdmfArrayType::Int64()) {
    datatype = H5T_NATIVE_LONG;
  }
  else if(mType == XdmfArrayType::Float32()) {
    datatype = H5T_NATIVE_FLOAT;
  }
  else if(mType == XdmfArrayType::Float64()) {
    datatype = H5T_NATIVE_DOUBLE;
  }
  else if(mType == XdmfArrayType::UInt8()) {
    datatype = H5T_NATIVE_UCHAR;
  }
  else if(mType == XdmfArrayType::UInt16()) {
    datatype = H5T_NATIVE_USHORT;
  }
  else if(mType == XdmfArrayType::UInt32()) {
    datatype = H5T_NATIVE_UINT;
  }
  else if(mType == XdmfArrayType::String()) {
    datatype = H5Tcopy(H5T_C_S1);
    H5Tset_size(datatype, H5T_VARIABLE);
    closeDatatype = true;
  }
  else {
    XdmfError::message(XdmfError::FATAL,
                       "Unknown XdmfArrayType encountered in hdf5 "
                       "controller.");
  }

  array->initialize(mType, mDimensions);

  if(numVals != array->getSize()) {
    XdmfError::message(XdmfError::FATAL,
                       "Number of values in hdf5 dataset does not match "
                       "allocated size in XdmfArray.");
  }

  if(closeDatatype) {
    char ** data = new char*[numVals];
    status = H5Dread(dataset, 
                     datatype, 
                     memspace,
                     dataspace,
                     H5P_DEFAULT, 
                     data);
    for(hssize_t i=0; i<numVals; ++i) {
      array->insert<std::string>(i, data[i]);
    }
    status = H5Dvlen_reclaim(datatype, 
                             dataspace, 
                             H5P_DEFAULT, 
                             data);
    delete [] data;
  }
  else {
    status = H5Dread(dataset,
                     datatype,
                     memspace,
                     dataspace,
                     H5P_DEFAULT,
                     array->getValuesInternal());
  }

  status = H5Sclose(dataspace);
  status = H5Sclose(memspace);
  status = H5Dclose(dataset);
  if(closeDatatype) {
    status = H5Tclose(datatype);
  }
  status = H5Fclose(hdf5Handle);
}
