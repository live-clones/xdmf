// Kenneth Leiter
// Xdmf Smart Pointer Test

#include "XdmfHeavyDataWriter.hpp"
#include "XdmfSystemUtils.hpp"

XdmfHeavyDataWriter::XdmfHeavyDataWriter(const std::string & filePath) :
	mFilePath(XdmfSystemUtils::getRealPath(filePath)),
	mMode(Default)
{
}

XdmfHeavyDataWriter::~XdmfHeavyDataWriter()
{
}

std::string XdmfHeavyDataWriter::getFilePath() const
{
	return mFilePath;
}

XdmfHeavyDataWriter::Mode XdmfHeavyDataWriter::getMode() const
{
	return mMode;
}

void XdmfHeavyDataWriter::setMode(const Mode mode)
{
	mMode = mode;
}
