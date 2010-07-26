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
