// Kenneth Leiter
// Xdmf Smart Pointer Test

#include "XdmfHDF5Controller.hpp"

XdmfHDF5Controller::XdmfHDF5Controller(const std::string & dataSetPath, const int precision,
	const int size, const std::string & type) :
		mDataSetPath(dataSetPath),
		mPrecision(precision),
		mSize(size),
		mType(type)
{
}

XdmfHDF5Controller::~XdmfHDF5Controller()
{
}

std::string XdmfHDF5Controller::getDataSetPath() const
{
	return mDataSetPath;
}

int XdmfHDF5Controller::getPrecision() const
{
	return mPrecision;
}

int XdmfHDF5Controller::getSize() const
{
	return mSize;
}

std::string XdmfHDF5Controller::getType() const
{
	return mType;
}
