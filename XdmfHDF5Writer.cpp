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
		mHDF5Handle(H5Fcreate("output.h5", H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT)),
		mHeavyFileName("output.h5")
	{
	};
	~XdmfHDF5WriterImpl()
	{
		herr_t status = H5Fclose(mHDF5Handle);
	};
	std::vector<std::string> mDataHierarchy;
	hid_t mHDF5Handle;
	std::string mHeavyFileName;
};

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

std::string XdmfHDF5Writer::createHDF5Group(std::stringstream & groupPath, int index)
{
	groupPath << "/" << mImpl->mDataHierarchy[index];
	hid_t handle = H5Gopen(mImpl->mHDF5Handle, groupPath.str().c_str(), H5P_DEFAULT);
	if(handle < 0)
	{
		// Open failed, create a new group
		handle = H5Gcreate(mImpl->mHDF5Handle, groupPath.str().c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	}
	H5Gclose(handle);

	// + 2 because the last value in mDataHierarchy == dataset name
	if(index + 2 < mImpl->mDataHierarchy.size())
	{
		return createHDF5Group(groupPath, index + 1);
	}
	return groupPath.str().c_str();
}

std::string XdmfHDF5Writer::getHDF5GroupHandle()
{
	if(mImpl->mDataHierarchy.size() > 1)
	{
		// Save old error handler and turn off error handling for now
		H5E_auto_t old_func;
		void* old_client_data;
		H5Eget_auto(0, &old_func, &old_client_data);
		H5Eset_auto2(0, NULL, NULL);

		std::stringstream emptyPath;
		std::string handle = createHDF5Group(emptyPath);

		// Restore previous error handler
		H5Eset_auto2(0, old_func, old_client_data);

		return handle;
	}
	return "";
}

void XdmfHDF5Writer::popDataHierarchy()
{
	mImpl->mDataHierarchy.pop_back();
}

void XdmfHDF5Writer::pushDataHierarchy(const XdmfItem & item)
{
	const std::map<std::string, std::string> itemProperties = item.getItemProperties();
	std::map<std::string, std::string>::const_iterator name = itemProperties.find("Name");
	if(name == itemProperties.end())
	{
		mImpl->mDataHierarchy.push_back(item.getItemTag());
	}
	else
	{
		mImpl->mDataHierarchy.push_back(name->second);
	}
}

std::string XdmfHDF5Writer::visit(XdmfArray & array, boost::shared_ptr<Loki::BaseVisitor> visitor)
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
		herr_t status;
		hsize_t size = array.getSize();
		hid_t dataspace = H5Screate_simple(1, &size, NULL);
		hid_t handle = mImpl->mHDF5Handle;
		std::string groupName = getHDF5GroupHandle();
		if(groupName.compare("") != 0)
		{
			handle = H5Gopen(mImpl->mHDF5Handle, groupName.c_str(), H5P_DEFAULT);
		}

		hid_t dataset = H5Dcreate(handle, mImpl->mDataHierarchy.back().c_str(), datatype, dataspace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
		status = H5Dwrite(dataset, datatype, H5S_ALL, H5S_ALL, H5P_DEFAULT, array.getValuesPointer());
		if(groupName.compare("") != 0)
		{
			H5Gclose(handle);
		}
		status = H5Dclose(dataset);
		status = H5Sclose(dataspace);

		std::stringstream dataSetName;
		dataSetName << mImpl->mHeavyFileName << ":" << groupName << "/" << mImpl->mDataHierarchy.back();
		return dataSetName.str();
	}
	return "";
}

void XdmfHDF5Writer::visit(XdmfItem & item, boost::shared_ptr<Loki::BaseVisitor> visitor)
{
	this->pushDataHierarchy(item);
	item.traverse(visitor);
	this->popDataHierarchy();
}
