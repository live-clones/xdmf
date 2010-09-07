// Kenneth Leiter
// Xdmf Smart Pointer Test

#include <hdf5.h>
#include <sstream>
#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfHDF5Controller.hpp"
#include "XdmfHDF5Writer.hpp"
#include "XdmfSystemUtils.hpp"

/**
 * PIMPL
 */
class XdmfHDF5Writer::XdmfHDF5WriterImpl {

public:

	XdmfHDF5WriterImpl(const std::string & hdf5FilePath) :
		mDataSetId(0),
		mFilePath(XdmfSystemUtils::getRealPath(hdf5FilePath)),
		mMode(Default)
	{
	};

	~XdmfHDF5WriterImpl()
	{
	};

	int mDataSetId;
	std::string mFilePath;
	Mode mMode;
};

boost::shared_ptr<XdmfHDF5Writer> XdmfHDF5Writer::New(const std::string & hdf5FilePath)
{
	boost::shared_ptr<XdmfHDF5Writer> p(new XdmfHDF5Writer(hdf5FilePath));
	return p;
}

XdmfHDF5Writer::XdmfHDF5Writer(const std::string & hdf5FilePath) :
	mImpl(new XdmfHDF5WriterImpl(hdf5FilePath))
{
}

XdmfHDF5Writer::~XdmfHDF5Writer()
{
	delete mImpl;
}

std::string XdmfHDF5Writer::getFilePath() const
{
	return mImpl->mFilePath;
}

XdmfHDF5Writer::Mode XdmfHDF5Writer::getMode() const
{
	return mImpl->mMode;
}

void XdmfHDF5Writer::setMode(const Mode mode)
{
	mImpl->mMode = mode;
}

void XdmfHDF5Writer::visit(XdmfArray & array, const boost::shared_ptr<XdmfBaseVisitor>)
{
	hid_t datatype = -1;

	if(array.isInitialized())
	{
		if(array.getArrayType() == XdmfArrayType::Int8())
		{
			datatype = H5T_NATIVE_CHAR;
		}
		else if(array.getArrayType() == XdmfArrayType::Int16())
		{
			datatype = H5T_NATIVE_SHORT;
		}
		else if(array.getArrayType() == XdmfArrayType::Int32())
		{
			datatype = H5T_NATIVE_INT;
		}
		else if(array.getArrayType() == XdmfArrayType::Int64())
		{
			datatype = H5T_NATIVE_LONG;
		}
		else if(array.getArrayType() == XdmfArrayType::Float32())
		{
			datatype = H5T_NATIVE_FLOAT;
		}
		else if(array.getArrayType() == XdmfArrayType::Float64())
		{
			datatype = H5T_NATIVE_DOUBLE;
		}
		else if(array.getArrayType() == XdmfArrayType::UInt8())
		{
			datatype = H5T_NATIVE_UCHAR;
		}
		else if(array.getArrayType() == XdmfArrayType::UInt16())
		{
			datatype = H5T_NATIVE_USHORT;
		}
		else if(array.getArrayType() == XdmfArrayType::UInt32())
		{
			datatype = H5T_NATIVE_UINT;
		}
		else
		{
			assert(false);
		}
	}

	if(datatype != -1)
	{
		std::string hdf5FilePath = mImpl->mFilePath;
		std::stringstream dataSetPath;

		if((mImpl->mMode == Overwrite || mImpl->mMode == Append) && array.getHDF5Controller())
		{
			// Write to the previous dataset
			dataSetPath << array.getHDF5Controller()->getDataSetPath();
			hdf5FilePath = array.getHDF5Controller()->getFilePath();
		}
		else
		{
			dataSetPath << "Data" << mImpl->mDataSetId;
		}

		// Open a hdf5 dataset and write to it on disk.
		herr_t status;
		hsize_t size = array.getSize();
		hid_t hdf5Handle;

		// Save old error handler and turn off error handling for now
		H5E_auto_t old_func;
		void * old_client_data;
		H5Eget_auto(0, &old_func, &old_client_data);
		H5Eset_auto2(0, NULL, NULL);

		if(H5Fis_hdf5(hdf5FilePath.c_str()) > 0)
		{
			hdf5Handle = H5Fopen(hdf5FilePath.c_str(), H5F_ACC_RDWR, H5P_DEFAULT);
		}
		else
		{
			hdf5Handle = H5Fcreate(hdf5FilePath.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
		}
		hid_t dataset = H5Dopen(hdf5Handle, dataSetPath.str().c_str(), H5P_DEFAULT);

		hid_t dataspace = H5S_ALL;
		hid_t memspace = H5S_ALL;

		if(dataset < 0)
		{
			hsize_t unlimited = H5S_UNLIMITED;
			memspace = H5Screate_simple(1, &size, &unlimited);
			hid_t property = H5Pcreate(H5P_DATASET_CREATE);
			hsize_t chunkSize = 1024;
			status = H5Pset_chunk(property, 1, &chunkSize);
			dataset = H5Dcreate(hdf5Handle, dataSetPath.str().c_str(), datatype, memspace, H5P_DEFAULT, property, H5P_DEFAULT);
			status = H5Pclose(property);
		}
		else
		{
			// Need to resize dataset to fit new data
			if(mImpl->mMode == Append)
			{
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
				status = H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, &start, NULL, &size, NULL) ;
			}
			else
			{
				status = H5Dset_extent(dataset, &size);
			}
		}
		status = H5Dwrite(dataset, datatype, memspace, dataspace, H5P_DEFAULT, array.getValuesInternal());
		if(dataspace != H5S_ALL)
		{
			status = H5Sclose(dataspace);
		}
		if(memspace != H5S_ALL)
		{
			status = H5Sclose(memspace);
		}
		status = H5Dclose(dataset);
		status = H5Fclose(hdf5Handle);

		// Restore previous error handler
		H5Eset_auto2(0, old_func, old_client_data);

		// Attach a new controller to the array if needed.
		if(mImpl->mMode == Default || !array.getHDF5Controller())
		{
			boost::shared_ptr<XdmfHDF5Controller> newDataSetController = XdmfHDF5Controller::New(hdf5FilePath, dataSetPath.str(), array.getSize(), array.getArrayType());
			array.setHDF5Controller(newDataSetController);
			mImpl->mDataSetId++;
		}
	}
}
