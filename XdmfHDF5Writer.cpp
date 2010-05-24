// Kenneth Leiter
// Xdmf Smart Pointer Test

#include <hdf5.h>
#include <sstream>
#include "XdmfArray.hpp"
#include "XdmfHDF5Controller.hpp"
#include "XdmfHDF5Writer.hpp"

/**
 * PIMPL
 */
class XdmfHDF5Writer::XdmfHDF5WriterImpl {

public:

	XdmfHDF5WriterImpl(const std::string & hdf5FilePath) :
		mHDF5FilePath(hdf5FilePath),
		mLastWrittenDataSet("")
	{
	};
	~XdmfHDF5WriterImpl()
	{
	};
	std::string mHDF5FilePath;
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

XdmfHDF5Writer::XdmfHDF5Writer(const std::string & hdf5FilePath) :
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
	mImpl->mLastWrittenDataSet = "";

	if(array.mHaveArray)
	{
		datatype = boost::apply_visitor(GetHDF5Type(), array.mArray);
	}
	else if(array.mHaveArrayPointer)
	{
		datatype = boost::apply_visitor(GetHDF5Type(), array.mArrayPointer);
	}
	else if(array.mHDF5Controller)
	{
		mImpl->mLastWrittenDataSet = array.mHDF5Controller->getDataSetPath();
	}

	if(datatype != -1)
	{
		std::stringstream dataSetName;
		dataSetName << "Data" << mImpl->mDataSetId;

		// Open a hdf5 dataset and write to it on disk.
		herr_t status;
		hsize_t size = array.getSize();
		hid_t hdf5Handle;

		// Save old error handler and turn off error handling for now
		H5E_auto_t old_func;
		void * old_client_data;
		H5Eget_auto(0, &old_func, &old_client_data);
		H5Eset_auto2(0, NULL, NULL);

		if(H5Fis_hdf5(mImpl->mHDF5FilePath.c_str()) > 0)
		{
			hdf5Handle = H5Fopen(mImpl->mHDF5FilePath.c_str(), H5F_ACC_RDWR, H5P_DEFAULT);
		}
		else
		{
			hdf5Handle = H5Fcreate(mImpl->mHDF5FilePath.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
		}
		hid_t dataset = H5Dopen(hdf5Handle, dataSetName.str().c_str(), H5P_DEFAULT);
		if(dataset < 0)
		{
			hid_t dataspace = H5Screate_simple(1, &size, NULL);
			dataset = H5Dcreate(hdf5Handle, dataSetName.str().c_str(), datatype, dataspace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
			status = H5Sclose(dataspace);
		}
		status = H5Dwrite(dataset, datatype, H5S_ALL, H5S_ALL, H5P_DEFAULT, array.getValuesPointer());
		status = H5Dclose(dataset);
		status = H5Fclose(hdf5Handle);

		// Restore previous error handler
		H5Eset_auto2(0, old_func, old_client_data);

		std::stringstream writtenDataSet;
		writtenDataSet << mImpl->mHDF5FilePath << ":" << dataSetName.str();

		mImpl->mLastWrittenDataSet = writtenDataSet.str();
		mImpl->mDataSetId++;

		boost::shared_ptr<XdmfHDF5Controller> newDataSetController = XdmfHDF5Controller::New(writtenDataSet.str(), array.getPrecision(),
				array.getSize(), array.getType());
		array.setHDF5Controller(newDataSetController);
	}
}
