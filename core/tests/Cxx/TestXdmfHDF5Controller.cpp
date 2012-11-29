#include <sstream>
#include "XdmfArrayType.hpp"
#include "XdmfHDF5Controller.hpp"
#include "XdmfSystemUtils.hpp"

int main(int, char **)
{
  shared_ptr<XdmfHDF5Controller> controller =
    XdmfHDF5Controller::New("output.h5",
                            "/foo/data1",
                            XdmfArrayType::Int8(),
                            std::vector<unsigned int>(1, 0),
                            std::vector<unsigned int>(1, 1),
                            std::vector<unsigned int>(1, 10),
                            std::vector<unsigned int>(1, 10));
  assert(controller->getDataSetPath().compare("/foo/data1") == 0);
  assert(controller->getSize() == 10);
  assert(controller->getType() == XdmfArrayType::Int8());

  return 0;
}
