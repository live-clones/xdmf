#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"

#include <iostream>

int main(int, char *)
{

  //
  // Create 1D arrays
  //
  boost::shared_ptr<XdmfArray> array = XdmfArray::New();
  array->initialize(XdmfArrayType::UInt32(), 2);
  assert(array->getArrayType() == XdmfArrayType::UInt32());
  assert(array->getSize() == 2);
  std::vector<unsigned int> dimensions = array->getDimensions();
  assert(dimensions.size() == 1);
  assert(dimensions[0] == 2);
  std::string dimensionsString = array->getDimensionsString();
  assert(dimensionsString.compare("2 ") == 0);
  array->resize<unsigned int>(3);
  assert(array->getSize() == 3);
  dimensions = array->getDimensions();
  assert(dimensions.size() == 1);
  assert(dimensions[0] == 3);
  dimensionsString = array->getDimensionsString();
  assert(dimensionsString.compare("3 ") == 0);
  
  //
  // Create 2D arrays
  //
  boost::shared_ptr<XdmfArray> array2 = XdmfArray::New();
  std::vector<unsigned int> newDimensions(2, 2);
  array2->initialize<unsigned short>(newDimensions);
  assert(array2->getArrayType() == XdmfArrayType::UInt16());
  assert(array2->getSize() == 4);
  dimensions = array2->getDimensions();
  assert(dimensions.size() == 2);
  assert(dimensions[0] == 2 && dimensions[1] == 2);
  dimensionsString = array2->getDimensionsString();
  assert(dimensionsString.compare("2 2 ") == 0);
  std::vector<unsigned int> newDimensions2(3,3);
  array2->resize<unsigned short>(newDimensions2);
  assert(array2->getSize() == 27);
  dimensions = array2->getDimensions();
  assert(dimensions.size() == 3);
  assert(dimensions[0] == 3 && dimensions[1] == 3 && dimensions[2] == 3);
  dimensionsString = array2->getDimensionsString();
  assert(dimensionsString.compare("3 3 3 ") == 0);

  int values[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  array2->insert(0, &values[0], 11);
  assert(array2->getSize() == 27);
  dimensions = array2->getDimensions();
  assert(dimensions.size() == 3);
  assert(dimensions[0] == 3 && dimensions[1] == 3 && dimensions[2] == 3);

  array2->pushBack(10);
  assert(array2->getSize() == 28);
  dimensions = array2->getDimensions();
  assert(dimensions.size() == 1);
  assert(dimensions[0] == 28);
  
  return 0;
}
