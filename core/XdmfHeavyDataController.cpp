// Kenneth Leiter
// Xdmf Smart Pointer Test

#include "XdmfHeavyDataController.hpp"
#include "XdmfSystemUtils.hpp"

XdmfHeavyDataController::XdmfHeavyDataController(const std::string & filePath, const std::string & dataSetPath, const unsigned int size, const boost::shared_ptr<const XdmfArrayType> type) :
	mDataSetPath(dataSetPath),
	mFilePath(XdmfSystemUtils::getRealPath(filePath)),
	mSize(size),
	mType(type)
{
}

XdmfHeavyDataController::~XdmfHeavyDataController()
{
}

std::string XdmfHeavyDataController::getDataSetPath() const
{
	return mDataSetPath;
}

std::string XdmfHeavyDataController::getFilePath() const
{
	return mFilePath;
}

unsigned int XdmfHeavyDataController::getSize() const
{
	return mSize;
}

boost::shared_ptr<const XdmfArrayType> XdmfHeavyDataController::getType() const
{
	return mType;
}
