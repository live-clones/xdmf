#include "XdmfArray.hpp"
#include "XdmfHDF5Controller.hpp"
#include "XdmfHDF5Writer.hpp"
#include <iostream>

int main(int, char **)
{
  shared_ptr<XdmfArray> array = XdmfArray::New();
  array->pushBack(0);
  array->pushBack(1);
  array->pushBack(2);

  //
  // Default operation - Always write to separate datasets.
  //

  std::cout << array->getHeavyDataController() << " ?= " << NULL << std::endl; 

  assert(array->getHeavyDataController() == NULL);
  shared_ptr<XdmfHDF5Writer> writer = XdmfHDF5Writer::New("hdf5WriterTest.h5");
  array->accept(writer);
  shared_ptr<XdmfHDF5Controller> firstController =
    shared_dynamic_cast<XdmfHDF5Controller>(array->getHeavyDataController());
  std::string firstPath = firstController->getDataSetPath();
  array->accept(writer);
  shared_ptr<XdmfHDF5Controller> secondController =
    shared_dynamic_cast<XdmfHDF5Controller>(array->getHeavyDataController());
  std::string secondPath = secondController->getDataSetPath();

  std::cout << firstPath << " ?!= " << secondPath << std::endl;

  assert(firstPath.compare(secondPath) != 0);

  //
  // Overwrite operation - Always write to the same datasets.
  //
  writer->setMode(XdmfHDF5Writer::Overwrite);
  array->accept(writer);
  shared_ptr<XdmfHDF5Controller> thirdController =
    shared_dynamic_cast<XdmfHDF5Controller>(array->getHeavyDataController());
  std::string thirdPath = thirdController->getDataSetPath();

  std::cout << secondPath << " ?= " << thirdPath << std::endl;

  assert(secondPath.compare(thirdPath) == 0);

  array->pushBack(3);
  array->accept(writer);
  shared_ptr<XdmfHDF5Controller> fourthController =
    shared_dynamic_cast<XdmfHDF5Controller>(array->getHeavyDataController());
  std::string fourthPath = fourthController->getDataSetPath();

  std::cout << thirdPath << " ?= " << fourthPath << std::endl;

  assert(thirdPath.compare(fourthPath) == 0);

  array->erase(0);
  array->erase(0);
  array->accept(writer);
  shared_ptr<XdmfHDF5Controller> fifthController = 
    shared_dynamic_cast<XdmfHDF5Controller>(array->getHeavyDataController());
  std::string fifthPath = fifthController->getDataSetPath();

  std::cout << fourthPath << " ?= " << fifthPath << std::endl;

  assert(fourthPath.compare(fifthPath) == 0);

  //
  // Append operation - Append data to same dataset.
  //
  writer->setMode(XdmfHDF5Writer::Append);
  // Append 4 times
  array->accept(writer);
  array->accept(writer);
  array->accept(writer);
  array->accept(writer);

  std::cout << array->getSize() << " ?= " << 2 << std::endl;

  assert(array->getSize() == 2);
  array->read();

  std::cout << array->getSize() << " ?= " << 10 << std::endl;

  assert(array->getSize() == 10);
  for(int i=0; i<5; ++i) {
    std::cout << array->getValue<int>(i*2) << " ?= " << 2 << std::endl;
    std::cout << array->getValue<int>(i*2 + 1) << " ?= " << 3 << std::endl;

    assert(array->getValue<int>(i*2) == 2);
    assert(array->getValue<int>(i*2 + 1) == 3);
  }

  return 0;
}
