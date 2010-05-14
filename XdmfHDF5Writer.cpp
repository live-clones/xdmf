// Kenneth Leiter
// Xdmf Smart Pointer Test

#include <sstream>
#include "XdmfArray.hpp"
#include "XdmfItem.hpp"
#include "XdmfHDF5Writer.hpp"

XdmfHDF5Writer::XdmfHDF5Writer() :
	mHeavyFileName("output.h5"),
	mHDF5Handle(H5Fcreate("output.h5", H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT))
{
	std::cout << "Created XdmfHDF5Writer " << this << std::endl;
}

XdmfHDF5Writer::~XdmfHDF5Writer()
{
	herr_t status = H5Fclose(mHDF5Handle);
	std::cout << "Deleted XdmfHDF5Writer " << this << std::endl;
}

std::string XdmfHDF5Writer::createHDF5Group(std::stringstream & groupPath, int index)
{
	groupPath << "/" << mDataHierarchy[index];
	hid_t handle = H5Gopen(mHDF5Handle, groupPath.str().c_str(), H5P_DEFAULT);
	if(handle < 0)
	{
		// Open failed, create a new group
		handle = H5Gcreate(mHDF5Handle, groupPath.str().c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	}
	H5Gclose(handle);

	// + 2 because the last value in mDataHierarchy == dataset name
	if(index + 2 < mDataHierarchy.size())
	{
		return createHDF5Group(groupPath, index + 1);
	}
	return groupPath.str().c_str();
}

std::string XdmfHDF5Writer::getHDF5GroupHandle()
{
	if(mDataHierarchy.size() > 1)
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
	mDataHierarchy.pop_back();
}

void XdmfHDF5Writer::pushDataHierarchy(const XdmfItem & item)
{
	const std::map<std::string, std::string> itemProperties = item.getItemProperties();
	std::map<std::string, std::string>::const_iterator name = itemProperties.find("Name");
	if(name == itemProperties.end())
	{
		mDataHierarchy.push_back(item.getItemTag());
	}
	else
	{
		mDataHierarchy.push_back(name->second);
	}
}

std::string XdmfHDF5Writer::visit(XdmfArray & array, boost::shared_ptr<Loki::BaseVisitor> visitor)
{
	herr_t status;
	hsize_t size = array.getSize();
	hid_t dataspace = H5Screate_simple(1, &size, NULL);
	hid_t handle = mHDF5Handle;
	std::string groupName = getHDF5GroupHandle();
	if(groupName.compare("") != 0)
	{
		handle = H5Gopen(mHDF5Handle, groupName.c_str(), H5P_DEFAULT);
	}
	hid_t dataset = H5Dcreate(handle, mDataHierarchy.back().c_str(), array.getHDF5Type(), dataspace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	status = H5Dwrite(dataset, array.getHDF5Type(), H5S_ALL, H5S_ALL, H5P_DEFAULT, array.getValuesPointer());
	if(groupName.compare("") != 0)
	{
		H5Gclose(handle);
	}
	status = H5Dclose(dataset);
	status = H5Sclose(dataspace);

	std::stringstream dataSetName;
	dataSetName << mHeavyFileName << ":" << groupName << "/" << mDataHierarchy.back();
	return dataSetName.str();
}

void XdmfHDF5Writer::visit(XdmfItem & item, boost::shared_ptr<Loki::BaseVisitor> visitor)
{
	this->pushDataHierarchy(item);
	item.traverse(visitor);
	this->popDataHierarchy();
}
