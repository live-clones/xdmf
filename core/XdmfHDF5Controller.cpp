// Kenneth Leiter
// Xdmf Smart Pointer Test

#include <hdf5.h>
#include <sstream>
#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfHDF5Controller.hpp"
#include "XdmfSystemUtils.hpp"

boost::shared_ptr<XdmfHDF5Controller> XdmfHDF5Controller::New(const std::string & hdf5FilePath, const std::string & dataSetPath, const unsigned int size, const boost::shared_ptr<const XdmfArrayType> type)
{
	boost::shared_ptr<XdmfHDF5Controller> p(new XdmfHDF5Controller(hdf5FilePath, dataSetPath, size, type));
	return p;
}

XdmfHDF5Controller::XdmfHDF5Controller(const std::string & hdf5FilePath, const std::string & dataSetPath, const unsigned int size, const boost::shared_ptr<const XdmfArrayType> type) :
	mDataSetPath(dataSetPath),
	mSize(size),
	mType(type)
{
	mFilePath = XdmfSystemUtils::getRealPath(hdf5FilePath);
}

XdmfHDF5Controller::~XdmfHDF5Controller()
{
}

std::string XdmfHDF5Controller::getDataSetPath() const
{
	return mDataSetPath;
}

std::string XdmfHDF5Controller::getFilePath() const
{
	return mFilePath;
}

boost::shared_ptr<const XdmfArrayType> XdmfHDF5Controller::getType() const
{
	return mType;
}

void XdmfHDF5Controller::read(XdmfArray * const array)
{
	hid_t hdf5Handle = H5Fopen(mFilePath.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
	hid_t dataset = H5Dopen(hdf5Handle, mDataSetPath.c_str(), H5P_DEFAULT);
	hid_t dataspace = H5Dget_space(dataset);
	hssize_t numVals = H5Sget_simple_extent_npoints(dataspace);
	hid_t datatype = H5Dget_type(dataset);

	array->initialize(mType);
	array->resize(numVals, 0);
	H5Dread(dataset, datatype, H5S_ALL, H5S_ALL, H5P_DEFAULT, array->getValuesPointer());

	herr_t status;
	status = H5Tclose(datatype);
	status = H5Sclose(dataspace);
	status = H5Dclose(dataset);
	status = H5Fclose(hdf5Handle);
}

unsigned int XdmfHDF5Controller::size() const
{
	return mSize;
}
