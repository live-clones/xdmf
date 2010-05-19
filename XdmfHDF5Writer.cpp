// Kenneth Leiter
// Xdmf Smart Pointer Test

#include <hdf5.h>
#include <sstream>
#include "XdmfArray.hpp"
#include "XdmfItem.hpp"
#include "XdmfHDF5Writer.hpp"

/**
 * PIMPL
 */
class XdmfHDF5Writer::XdmfHDF5WriterImpl {

public:

	XdmfHDF5WriterImpl(std::string & hdf5FilePath) :
		mHDF5Handle(H5Fcreate(hdf5FilePath.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT)),
		mHeavyFileName(hdf5FilePath),
		mLastWrittenDataSet("")
	{
	};
	~XdmfHDF5WriterImpl()
	{
		herr_t status = H5Fclose(mHDF5Handle);
	};
	hid_t mHDF5Handle;
	std::string mHeavyFileName;
	std::string mLastWrittenDataSet;
	static int mDataSetId;
};

int XdmfHDF5Writer::XdmfHDF5WriterImpl::mDataSetId = 0;

class XdmfHDF5Writer::GetHDF5Type : public boost::static_visitor <hid_t> {
public:

	GetHDF5Type()
	{
	}

	hid_t getHDF5Type(const char * const) const
	{
		return H5T_NATIVE_CHAR;
	}

	hid_t getHDF5Type(const short * const) const
	{
		return H5T_NATIVE_SHORT;
	}

	hid_t getHDF5Type(const int * const) const
	{
		return H5T_NATIVE_INT;
	}

	hid_t getHDF5Type(const long * const) const
	{
		return H5T_NATIVE_LONG;
	}

	hid_t getHDF5Type(const float * const) const
	{
		return H5T_NATIVE_FLOAT;
	}

	hid_t getHDF5Type(const double * const) const
	{
		return H5T_NATIVE_DOUBLE;
	}

	hid_t getHDF5Type(const unsigned char * const) const
	{
		return H5T_NATIVE_UCHAR;
	}

	hid_t getHDF5Type(const unsigned short * const) const
	{
		return H5T_NATIVE_USHORT;
	}

	hid_t getHDF5Type(const unsigned int * const) const
	{
		return H5T_NATIVE_UINT;
	}

	template<typename T>
	hid_t operator()(const boost::shared_ptr<std::vector<T> > & array) const
	{
		return this->getHDF5Type(&(array.get()->operator[](0)));
	}

	template<typename T>
	hid_t operator()(const boost::shared_array<const T> & array) const
	{
		return this->getHDF5Type(array.get());
	}
};

XdmfHDF5Writer::XdmfHDF5Writer(std::string & hdf5FilePath) :
	mImpl(new XdmfHDF5WriterImpl(hdf5FilePath))
{
	std::cout << "Created XdmfHDF5Writer " << this << std::endl;
}

XdmfHDF5Writer::~XdmfHDF5Writer()
{
	delete mImpl;
	std::cout << "Deleted XdmfHDF5Writer " << this << std::endl;
}

std::string XdmfHDF5Writer::getLastWrittenDataSet() const
{
	return mImpl->mLastWrittenDataSet;
}

void XdmfHDF5Writer::visit(XdmfArray & array, boost::shared_ptr<Loki::BaseVisitor> visitor)
{
	hid_t datatype = -1;
	if(array.mHaveArray)
	{
		datatype = boost::apply_visitor(GetHDF5Type(), array.mArray);
	}
	else if(array.mHaveArrayPointer)
	{
		datatype = boost::apply_visitor(GetHDF5Type(), array.mArrayPointer);
	}

	if(datatype != -1)
	{
		std::stringstream dataSetName;
		dataSetName << "Data" << mImpl->mDataSetId;

		// Open a hdf5 dataset and write to it on disk.
		herr_t status;
		hsize_t size = array.getSize();
		hid_t dataspace = H5Screate_simple(1, &size, NULL);
		hid_t dataset = H5Dcreate(mImpl->mHDF5Handle, dataSetName.str().c_str(), datatype, dataspace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
		status = H5Dwrite(dataset, datatype, H5S_ALL, H5S_ALL, H5P_DEFAULT, array.getValuesPointer());
		status = H5Dclose(dataset);
		status = H5Sclose(dataspace);

		std::stringstream writtenDataSetName;
		writtenDataSetName << mImpl->mHeavyFileName << ":" << dataSetName.str();
		mImpl->mLastWrittenDataSet = writtenDataSetName.str();
		mImpl->mDataSetId++;
	}
	else
	{
		mImpl->mLastWrittenDataSet = "";
	}
}
