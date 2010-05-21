// Kenneth Leiter
// Xdmf Smart Pointer Test

#include <hdf5.h>
#include <sstream>
#include <vector>
#include "XdmfArray.hpp"
#include "XdmfHDF5Controller.hpp"

XdmfHDF5Controller::XdmfHDF5Controller(const std::string & hdf5FilePath, const std::string & dataSetName, const int precision,
	const int size, const std::string & type) :
		mHDF5FilePath(hdf5FilePath),
		mDataSetName(dataSetName),
		mPrecision(precision),
		mSize(size),
		mType(type)
{
}

XdmfHDF5Controller::~XdmfHDF5Controller()
{
}

std::string XdmfHDF5Controller::getDataSetPath() const
{
	std::stringstream toReturn;
	toReturn << mHDF5FilePath << ":" << mDataSetName;
	return toReturn.str();
}

int XdmfHDF5Controller::getPrecision() const
{
	return mPrecision;
}

int XdmfHDF5Controller::getSize() const
{
	return mSize;
}

std::string XdmfHDF5Controller::getType() const
{
	return mType;
}

void XdmfHDF5Controller::read(XdmfArray * const array)
{
	hid_t hdf5Handle = H5Fopen(mHDF5FilePath.c_str(), H5F_ACC_RDWR, H5P_DEFAULT);
	hid_t dataset = H5Dopen(hdf5Handle, mDataSetName.c_str(), H5P_DEFAULT);
	hid_t dataspace = H5Dget_space(dataset);
	hssize_t numVals = H5Sget_simple_extent_npoints(dataspace);
	hid_t datatype = H5Dget_type(dataset);

	void * valuesPointer;
	if(H5Tequal(datatype, H5T_NATIVE_CHAR))
	{
		boost::shared_ptr<std::vector<char> > arrayValues = array->initialize<char>();
		array->resize(numVals, 0);
		valuesPointer = &arrayValues->operator[](0);
	}
	else if(H5Tequal(datatype, H5T_NATIVE_SHORT))
	{
		boost::shared_ptr<std::vector<short> > arrayValues = array->initialize<short>();
		array->resize(numVals, 0);
		valuesPointer = &arrayValues->operator[](0);
	}
	else if(H5Tequal(datatype, H5T_NATIVE_INT))
	{
		boost::shared_ptr<std::vector<int> > arrayValues = array->initialize<int>();
		array->resize(numVals, 0);
		valuesPointer = &arrayValues->operator[](0);
	}
	else if(H5Tequal(datatype, H5T_NATIVE_LONG))
	{
		boost::shared_ptr<std::vector<long> > arrayValues = array->initialize<long>();
		array->resize(numVals, 0);
		valuesPointer = &arrayValues->operator[](0);
	}
	else if(H5Tequal(datatype, H5T_NATIVE_FLOAT))
	{
		boost::shared_ptr<std::vector<float> > arrayValues = array->initialize<float>();
		array->resize(numVals, 0);
		valuesPointer = &arrayValues->operator[](0);
	}
	else if(H5Tequal(datatype, H5T_NATIVE_DOUBLE))
	{
		boost::shared_ptr<std::vector<double> > arrayValues = array->initialize<double>();
		array->resize(numVals, 0);
		valuesPointer = &arrayValues->operator[](0);
	}
	else if(H5Tequal(datatype, H5T_NATIVE_UCHAR))
	{
		boost::shared_ptr<std::vector<unsigned char> > arrayValues = array->initialize<unsigned char>();
		array->resize(numVals, 0);
		valuesPointer = &arrayValues->operator[](0);
	}
	else if(H5Tequal(datatype, H5T_NATIVE_USHORT))
	{
		boost::shared_ptr<std::vector<unsigned short> > arrayValues = array->initialize<unsigned short>();
		array->resize(numVals, 0);
		valuesPointer = &arrayValues->operator[](0);
	}
	else if(H5Tequal(datatype, H5T_NATIVE_UINT))
	{
		boost::shared_ptr<std::vector<unsigned int> > arrayValues = array->initialize<unsigned int>();
		array->resize(numVals, 0);
		valuesPointer = &arrayValues->operator[](0);
	}
	else
	{
		assert("Unsupported HDF5 Data Type");
	}

	H5Dread(dataset, datatype, H5S_ALL, H5S_ALL, H5P_DEFAULT, valuesPointer);

	herr_t status;
	status = H5Tclose(datatype);
	status = H5Sclose(dataspace);
	status = H5Dclose(dataset);
	status = H5Fclose(hdf5Handle);
}
