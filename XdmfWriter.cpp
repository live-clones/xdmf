// Kenneth Leiter
// Xdmf Smart Pointer Test

#include "XdmfArray.hpp"
#include "XdmfSet.hpp"
#include "XdmfWriter.hpp"

#include <iostream>

boost::shared_ptr<XdmfWriter> XdmfWriter::New(const std::string & xmlFilePath)
{
	boost::shared_ptr<XdmfWriter> p(new XdmfWriter(xmlFilePath));
	return p;
}

boost::shared_ptr<XdmfWriter> XdmfWriter::New(const std::string & xmlFilePath, const boost::shared_ptr<XdmfHDF5Writer> hdf5Writer)
{
	boost::shared_ptr<XdmfWriter> p(new XdmfWriter(xmlFilePath, hdf5Writer));
	return p;
}

XdmfWriter::XdmfWriter(const std::string & xmlFilePath) :
	XdmfCoreWriter(xmlFilePath)
{
}

XdmfWriter::XdmfWriter(const std::string & xmlFilePath, boost::shared_ptr<XdmfHDF5Writer> hdf5Writer) :
	XdmfCoreWriter(xmlFilePath, hdf5Writer)
{
}

XdmfWriter::~XdmfWriter()
{
}

void XdmfWriter::visit(XdmfSet & set, const boost::shared_ptr<XdmfBaseVisitor> visitor)
{
	XdmfCoreWriter::visit(dynamic_cast<XdmfItem &>(set), visitor);

	boost::shared_ptr<XdmfArray> setValues = XdmfArray::New();
	if(set.isInitialized())
	{
		setValues->reserve(set.size());
		for(XdmfSet::const_iterator iter = set.begin(); iter != set.end(); ++iter)
		{
			setValues->pushBack(*iter);
		}
	}

	setValues->setHDF5Controller(set.getHDF5Controller());
	XdmfCoreWriter::moveToLastWrittenNode();
	XdmfCoreWriter::visit(*setValues.get(), visitor);
	XdmfCoreWriter::moveToParentNode();
}
