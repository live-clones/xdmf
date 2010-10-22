// Kenneth Leiter
// Xdmf Smart Pointer Test

#include "XdmfHeavyDataWriter.hpp"
#include "XdmfSystemUtils.hpp"

XdmfHeavyDataWriter::XdmfHeavyDataWriter() :
	mDataSetId(0),
	mFilePath(""),
	mMode(Default)
{
}

XdmfHeavyDataWriter::XdmfHeavyDataWriter(const std::string & filePath) :
	mDataSetId(0),
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
