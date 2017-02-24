#include "XdmfArray.hpp"
#include "XdmfHDF5Writer.hpp"
#include "XdmfInformation.hpp"
#include <iostream>
#include <assert.h>

int main(int, char **)
{
  
  shared_ptr<XdmfInformation> information = XdmfInformation::New();
  information->setKey("foo");
  information->setValue("bar");
  
  shared_ptr<XdmfArray> array1 = XdmfArray::New();
  array1->pushBack(0);
  array1->pushBack(1);
  array1->pushBack(2);

  shared_ptr<XdmfArray> array2 = XdmfArray::New();
  array2->pushBack(3);
  array2->pushBack(4);
  array2->pushBack(5);
  
  information->insert(array1);
  information->insert(array2);
  
  shared_ptr<XdmfHDF5Writer> writer = 
    XdmfHDF5Writer::New("hdf5WriterTestTree.h5");
  information->accept(writer);

  if (array1->getHeavyDataController())
  {
    std::cout << "array 1 does not have a heavy data controller" << std::endl;
  }
  else
  {
    std::cout << "array 1 has a heavy data controller" << std::endl;
  }

  if (array2->getHeavyDataController())
  {
    std::cout << "array 2 does not have a heavy data controller" << std::endl;
  }
  else
  {
    std::cout << "array 2 has a heavy data controller" << std::endl;
  }

  assert(array1->getHeavyDataController());
  assert(array2->getHeavyDataController());
  
  array1->release();
  array2->release();

  array1->read();
  array2->read();

  for (unsigned int i = 0; i < 3; ++i)
  {
    std::cout << array1->getValue<int>(i) << " ?= " << i << std::endl;
  }

  for (unsigned int i = 0; i < 3; ++i)
  {
    std::cout << array2->getValue<int>(i) << " ?= " << i+3 << std::endl;
  }

  assert(array1->getValue<int>(0) == 0);
  assert(array1->getValue<int>(1) == 1);
  assert(array1->getValue<int>(2) == 2);
  assert(array2->getValue<int>(0) == 3);
  assert(array2->getValue<int>(1) == 4);
  assert(array2->getValue<int>(2) == 5);

  return 0;
}
