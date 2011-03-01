#include "XdmfArray.hpp"
#include "XdmfHDF5Controller.hpp"
#include "XdmfHDF5Writer.hpp"

int main(int, char *)
{
  boost::shared_ptr<XdmfArray> array = XdmfArray::New();
  array->pushBack(0);
  array->pushBack(1);
  array->pushBack(2);

  //
  // Default operation - Always write to separate datasets.
  //
  assert(array->getHeavyDataController() == NULL);
  boost::shared_ptr<XdmfHDF5Writer> writer =
    XdmfHDF5Writer::New("hdf5WriterTest.h5");
  array->accept(writer);
  boost::shared_ptr<XdmfHeavyDataController> firstController =
    array->getHeavyDataController();
  std::string firstPath = firstController->getDataSetPath();
  array->accept(writer);
  boost::shared_ptr<XdmfHeavyDataController> secondController =
    array->getHeavyDataController();
  std::string secondPath = secondController->getDataSetPath();
  assert(firstController != secondController);
  assert(firstPath.compare(secondPath) != 0);

  //
  // Overwrite operation - Always write to the same datasets.
  //
  writer->setMode(XdmfHDF5Writer::Overwrite);
  array->accept(writer);
  boost::shared_ptr<XdmfHeavyDataController> thirdController =
    array->getHeavyDataController();
  std::string thirdPath = thirdController->getDataSetPath();
  assert(secondController == thirdController);
  assert(secondPath.compare(thirdPath) == 0);

  array->pushBack(3);
  array->accept(writer);
  boost::shared_ptr<XdmfHeavyDataController> fourthController =
    array->getHeavyDataController();
  std::string fourthPath = fourthController->getDataSetPath();
  assert(thirdController == fourthController);
  assert(thirdPath.compare(fourthPath) == 0);

  array->erase(0);
  array->erase(0);
  array->accept(writer);
  boost::shared_ptr<XdmfHeavyDataController> fifthController =
    array->getHeavyDataController();
  std::string fifthPath = fifthController->getDataSetPath();
  assert(fourthController == fifthController);
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
  assert(array->getSize() == 2);
  array->read();
  assert(array->getSize() == 10);
  for(int i=0; i<5; ++i)
    {
      assert(array->getValue<int>(i*2) == 2);
      assert(array->getValue<int>(i*2 + 1) == 3);
    }

  return 0;
}
