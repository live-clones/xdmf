// Kenneth Leiter
// Xdmf Smart Pointer Test

#include "XdmfItemFactory.hpp"
#include "XdmfReader.hpp"

boost::shared_ptr<XdmfReader> XdmfReader::New()
{
	boost::shared_ptr<XdmfReader> p(new XdmfReader());
	return p;
}

XdmfReader::XdmfReader() :
	XdmfCoreReader(XdmfItemFactory::New())
{
}

XdmfReader::~XdmfReader()
{
}

// Implemented to make SWIG wrapping work correctly (typemaps to return specific subclass instances of XdmfItems)
boost::shared_ptr<XdmfItem> XdmfReader::read(const std::string & filePath) const
{
	return XdmfCoreReader::read(filePath);
}
