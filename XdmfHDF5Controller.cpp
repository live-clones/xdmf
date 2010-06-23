// Kenneth Leiter
// Xdmf Smart Pointer Test

#include <hdf5.h>
#include <sstream>
#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfHDF5Controller.hpp"

XdmfHDF5Controller::XdmfHDF5Controller(const std::string & dataSetPath, const unsigned int size, const boost::shared_ptr<const XdmfArrayType> type) :
		mSize(size),
		mType(type)
{
	size_t colonLocation = dataSetPath.find(":");
	if(colonLocation != std::string::npos)
	{
		mHDF5FilePath = dataSetPath.substr(0, colonLocation);
		if(colonLocation + 1  != mHDF5FilePath.size())
		{
			mDataSetName = dataSetPath.substr(colonLocation + 1, dataSetPath.size());
		}
		else
		{
			assert(false);
		}
	}
	else
	{
		assert(false);
	}
}

XdmfHDF5Controller::~XdmfHDF5Controller()
{
}

std::string XdmfHDF5Controller::getDataSetName() const
{
	return mDataSetName;
}

std::string XdmfHDF5Controller::getDataSetPath() const
{
	std::stringstream toReturn;
	toReturn << mHDF5FilePath << ":" << mDataSetName;
	return toReturn.str();
}

std::string XdmfHDF5Controller::getHDF5FilePath() const
{
	return mHDF5FilePath;
}

unsigned int XdmfHDF5Controller::getSize() const
{
	return mSize;
}

boost::shared_ptr<const XdmfArrayType> XdmfHDF5Controller::getType() const
{
	return mType;
}

void XdmfHDF5Controller::read(XdmfArray * const array)
{
	hid_t hdf5Handle = H5Fopen(mHDF5FilePath.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
	hid_t dataset = H5Dopen(hdf5Handle, mDataSetName.c_str(), H5P_DEFAULT);
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
