// Kenneth Leiter
// Xdmf Smart Pointer Test

#include "XdmfItemFactory.hpp"
#include "XdmfReader.hpp"

XdmfReader::XdmfReader() :
	XdmfCoreReader(XdmfItemFactory::New())
{
	std::cout << "Created XdmfReader " << this << std::endl;
}

XdmfReader::~XdmfReader()
{
	std::cout << "Deleted XdmfReader " << this << std::endl;
}

