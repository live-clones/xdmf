// Kenneth Leiter
// Xdmf Smart Pointer Test

#include "XdmfItemFactory.hpp"
#include "XdmfReader.hpp"

XdmfReader::XdmfReader() :
	XdmfCoreReader(XdmfItemFactory::New())
{
}

XdmfReader::~XdmfReader()
{
}
