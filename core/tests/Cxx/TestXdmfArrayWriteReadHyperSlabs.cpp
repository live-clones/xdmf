#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfHDF5Controller.hpp"
#include "XdmfHDF5Writer.hpp"

int main(int, char **)
{
  int values1[] = {1, 2};
  int values2[] = {3, 4};

  shared_ptr<XdmfArray> array1 = XdmfArray::New();
  array1->insert(0, &values1[0], 2, 1, 1);
  assert(array1->getSize() == 2);
  assert(array1->getValuesString().compare("1 2") == 0);

  shared_ptr<XdmfArray> array2 = XdmfArray::New();
  array2->insert(0, &values2[0], 2, 1, 1);
  assert(array2->getSize() == 2);
  assert(array2->getValuesString().compare("3 4") == 0);

  //
  // write array1 to first part of dataset
  //
  shared_ptr<XdmfHDF5Controller> controller1 = 
    XdmfHDF5Controller::New("testHyperslab.h5",
                            "data",
                            XdmfArrayType::Int32(),
                            std::vector<unsigned int>(1, 0),
                            std::vector<unsigned int>(1, 1),
                            std::vector<unsigned int>(1, 2),
                            std::vector<unsigned int>(1, 4));
  array1->setHeavyDataController(controller1);
  shared_ptr<XdmfHDF5Writer> writer = XdmfHDF5Writer::New("testHyperslab.h5");
  writer->setMode(XdmfHeavyDataWriter::Hyperslab);
  array1->accept(writer);

  //
  // write array2 to second part of dataset
  //
  shared_ptr<XdmfHDF5Controller> controller2 = 
    XdmfHDF5Controller::New("testHyperslab.h5",
                            "data",
                            XdmfArrayType::Int32(),
                            std::vector<unsigned int>(1, 2),
                            std::vector<unsigned int>(1, 1),
                            std::vector<unsigned int>(1, 2),
                            std::vector<unsigned int>(1, 4));
  array2->setHeavyDataController(controller2);
  array2->accept(writer);

  //
  // assert we can release and read same data we wrote
  //
  array1->release();
  array1->read();
  assert(array1->getValuesString().compare("1 2") == 0);
  array2->release();
  array2->read();
  assert(array2->getValuesString().compare("3 4") == 0);
}
