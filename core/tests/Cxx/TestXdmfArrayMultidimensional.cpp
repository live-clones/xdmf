#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"

#include <iostream>

int main(int, char **)
{

  //
  // Create 1D arrays
  //
  shared_ptr<XdmfArray> array = XdmfArray::New();
  array->resize<unsigned int>(2);
  std::cout << array->getArrayType() << " ?= " << XdmfArrayType::UInt32() << std::endl;
  assert(array->getArrayType() == XdmfArrayType::UInt32());
  std::cout << array->getSize() << " ?= " << 2 << std::endl;
  assert(array->getSize() == 2);
  std::vector<unsigned int> dimensions = array->getDimensions();
  std::cout << dimensions.size() << " ?= " << 1 << std::endl;
  std::cout << dimensions[0] << " ?= " << 2 << std::endl;
  assert(dimensions.size() == 1);
  assert(dimensions[0] == 2);
  std::string dimensionsString = array->getDimensionsString();
  std::cout << dimensionsString << " ?= " << "2" << std::endl;
  assert(dimensionsString.compare("2") == 0);
  array->resize<unsigned int>(3);
  std::cout << array->getSize() << " ?= " << 3 << std::endl;
  assert(array->getSize() == 3);
  dimensions = array->getDimensions();
  std::cout << dimensions.size() << " ?= " << 1 << std::endl;
  std::cout << dimensions[0] << " ?= " << 3 << std::endl;
  assert(dimensions.size() == 1);
  assert(dimensions[0] == 3);
  dimensionsString = array->getDimensionsString();
  std::cout << dimensionsString << " ?= " << "3" << std::endl;
  assert(dimensionsString.compare("3") == 0);
  
  //
  // Create 2D arrays
  //
  shared_ptr<XdmfArray> array2 = XdmfArray::New();
  std::vector<unsigned int> newDimensions(2, 2);
  array2->initialize<unsigned short>(newDimensions);
  std::cout << array2->getArrayType() << " ?= " << XdmfArrayType::UInt16() << std::endl;
  std::cout << array2->getSize() << " ?= " << 4 << std::endl;
  assert(array2->getArrayType() == XdmfArrayType::UInt16());
  assert(array2->getSize() == 4);
  dimensions = array2->getDimensions();
  std::cout << dimensions.size() << " ?= " << 2 << std::endl;
  std::cout << dimensions[0] << " ?= " << 2 << std::endl;
  std::cout << dimensions[1] << " ?= " << 2 << std::endl;
  assert(dimensions.size() == 2);
  assert(dimensions[0] == 2 && dimensions[1] == 2);
  dimensionsString = array2->getDimensionsString();
  std::cout << dimensionsString << " ?= " << "2 2" << std::endl;
  assert(dimensionsString.compare("2 2") == 0);
  std::vector<unsigned int> newDimensions2(3,3);
  array2->resize<unsigned short>(newDimensions2);
  std::cout << array2->getSize() << " ?= " << 27 << std::endl;
  assert(array2->getSize() == 27);
  dimensions = array2->getDimensions();
  std::cout << dimensions.size() << " ?= " << 3 << std::endl;
  std::cout << dimensions[0] << " ?= " << 3 << std::endl;
  std::cout << dimensions[1] << " ?= " << 3 << std::endl;
  std::cout << dimensions[2] << " ?= " << 3 << std::endl;
  assert(dimensions.size() == 3);
  assert(dimensions[0] == 3 && dimensions[1] == 3 && dimensions[2] == 3);
  dimensionsString = array2->getDimensionsString();
  std::cout << dimensionsString << " ?= " << "3 3 3" << std::endl;
  assert(dimensionsString.compare("3 3 3") == 0);

  int values[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  array2->insert(0, &values[0], 11);
  std::cout << array2->getSize() << " ?= " << 27 << std::endl;
  assert(array2->getSize() == 27);
  dimensions = array2->getDimensions();
  std::cout << dimensions.size() << " ?= " << 3 << std::endl;
  std::cout << dimensions[0] << " ?= " << 3 << std::endl;
  std::cout << dimensions[1] << " ?= " << 3 << std::endl;
  std::cout << dimensions[2] << " ?= " << 3 << std::endl;
  assert(dimensions.size() == 3);
  assert(dimensions[0] == 3 && dimensions[1] == 3 && dimensions[2] == 3);

  array2->pushBack(10);
  std::cout << array2->getSize() << " ?= " << 28 << std::endl;
  assert(array2->getSize() == 28);
  dimensions = array2->getDimensions();
  std::cout << dimensions.size() << " ?= " << 1 << std::endl;
  std::cout << dimensions[0] << " ?= " << 28 << std::endl;
  assert(dimensions.size() == 1);
  assert(dimensions[0] == 28);
  
  return 0;
}
