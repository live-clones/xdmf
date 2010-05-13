// Kenneth Leiter
// Xdmf Smart Pointer Test

#include <sstream>
#include "XdmfArray.hpp"
#include "XdmfItem.hpp"
#include "XdmfWriter.hpp"

XdmfWriter::XdmfWriter() :
	mLightDataLimit(100),
	mHeavyFileName("output.h5"),
	mHDF5Handle(H5Fcreate("output.h5", H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT)),
	mXMLDocument(xmlNewDoc((xmlChar*)"1.0")),
	mXMLCurrentNode(xmlNewNode(NULL, (xmlChar*)"Xdmf"))
{
	xmlDocSetRootElement(mXMLDocument, mXMLCurrentNode);
	std::cout << "Created visitor " << this << std::endl;
}

XdmfWriter::~XdmfWriter()
{
	xmlSaveFormatFile("output.xmf", mXMLDocument, 1);
	xmlFreeDoc(mXMLDocument);
	xmlCleanupParser();
	herr_t status = H5Fclose(mHDF5Handle);
	std::cout << "Deleted visitor " << this << std::endl;
}

std::string XdmfWriter::createHDF5Group(std::stringstream & groupPath, int index)
{
	groupPath << "/" << mDataHierarchy[index];
	hid_t handle = H5Gopen(mHDF5Handle, groupPath.str().c_str(), H5P_DEFAULT);
	if(handle < 0)
	{
		// Open failed, create a new group
		handle = H5Gcreate(mHDF5Handle, groupPath.str().c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	}
	H5Gclose(handle);

	// + 2 because we ignore last value in mDataHierarchy (== dataset name)
	if(index + 2 < mDataHierarchy.size())
	{
		return createHDF5Group(groupPath, index + 1);

	}
	return groupPath.str().c_str();
}

std::string XdmfWriter::getHDF5GroupHandle()
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

unsigned int XdmfWriter::getLightDataLimit() const
{
	return mLightDataLimit;
}

void XdmfWriter::setLightDataLimit(unsigned int numValues)
{
	mLightDataLimit = numValues;
}

void XdmfWriter::visit(XdmfArray & array, boost::shared_ptr<Loki::BaseVisitor> visitor)
{
	this->visit(dynamic_cast<XdmfItem &>(array), visitor);

	xmlNodePtr parentNode = mXMLCurrentNode;
	mXMLCurrentNode = mXMLCurrentNode->children;

	std::stringstream xmlTextValues;
	if(array.getSize() > mLightDataLimit)
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
		xmlTextValues << mHeavyFileName << ":" << groupName << "/" << mDataHierarchy.back();
		status = H5Dwrite(dataset, array.getHDF5Type(), H5S_ALL, H5S_ALL, H5P_DEFAULT, array.getValuesPointer());
		if(groupName.compare("") != 0)
		{
			H5Gclose(handle);
		}
		status = H5Dclose(dataset);
		status = H5Sclose(dataspace);
	}
	else
	{
		xmlTextValues << array.getValuesString();
	}

	xmlAddChild(mXMLCurrentNode, xmlNewText((xmlChar*)xmlTextValues.str().c_str()));
	mXMLCurrentNode = parentNode;
}

void XdmfWriter::visit(XdmfItem & item, boost::shared_ptr<Loki::BaseVisitor> visitor)
{
	xmlNodePtr parentNode = mXMLCurrentNode;
	mXMLCurrentNode = xmlNewChild(mXMLCurrentNode, NULL, (xmlChar*)item.getItemTag().c_str(), NULL);
	const std::map<std::string, std::string> itemProperties = item.getItemProperties();
	bool pushedHDF5Name = false;
	for(std::map<std::string, std::string>::const_iterator iter = itemProperties.begin(); iter != itemProperties.end(); ++iter)
	{
		if(iter->first.compare("Name") == 0)
		{
			mDataHierarchy.push_back(iter->second);
			pushedHDF5Name = true;
		}
		xmlNewProp(mXMLCurrentNode, (xmlChar*)iter->first.c_str(), (xmlChar*)iter->second.c_str());
	}
	if(!pushedHDF5Name)
	{
		mDataHierarchy.push_back(item.getItemTag());
	}

	item.traverse(visitor);
	mXMLCurrentNode = parentNode;
	mDataHierarchy.pop_back();
}
