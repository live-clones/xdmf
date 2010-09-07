#include <sstream>
#include "XdmfArrayType.hpp"
#include "XdmfHDF5Controller.hpp"
#include "XdmfSystemUtils.hpp"

int main(int, char *)
{
	boost::shared_ptr<XdmfHDF5Controller> controller = XdmfHDF5Controller::New("output.h5", "/foo/data1", 10, XdmfArrayType::Int8());
	assert(controller->getDataSetPath().compare("/foo/data1") == 0);
	assert(controller->getFilePath().compare(XdmfSystemUtils::getRealPath("output.h5")) == 0);
	assert(controller->getSize() == 10);
	assert(controller->getType() == XdmfArrayType::Int8());
	return 0;
}
