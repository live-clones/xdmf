#include "XdmfArray.hpp"
#include "XdmfHDF5Writer.hpp"
#include "XdmfHDF5Controller.hpp"
#include <iostream>

int main(int, char **)
{
  int values[] = {1, 2, 3, 4};

  shared_ptr<XdmfArray> array = XdmfArray::New();
  array->insert(0, &values[0], 4, 1, 1);
  std::cout << array->getSize() << " ?= " << 4 << std::endl;
  assert(array->getSize() == 4);
  std::cout << array->getValuesString() << " ?= " << "1 2 3 4" << std::endl;
  assert(array->getValuesString().compare("1 2 3 4") == 0);

  shared_ptr<XdmfHDF5Writer> writer = XdmfHDF5Writer::New("test.h5");
  array->accept(writer);

  std::cout << array->getSize() << " ?= " << 4 << std::endl;
  assert(array->getSize() == 4);
  std::cout << array->getValuesString() << " ?= " << "1 2 3 4" << std::endl;
  assert(array->getValuesString().compare("1 2 3 4") == 0);

  array->release();
  std::cout << array->getValuesString() << " ?= " << "" << std::endl;
  assert(array->getValuesString() == "");
  std::cout << array->getSize() << " ?= " << 4 << std::endl;
  assert(array->getSize() == 4);

  array->read();
  std::cout << array->getValuesString() << " ?= " << "1 2 3 4" << std::endl;
  assert(array->getValuesString().compare("1 2 3 4") == 0);

  shared_ptr<XdmfArray> stringArray = XdmfArray::New();
  stringArray->pushBack<std::string>("foo");
  stringArray->pushBack<std::string>("bar");
  stringArray->pushBack<std::string>("cat");
  stringArray->pushBack<std::string>("dog");
  stringArray->pushBack<std::string>("blah");
  std::cout << stringArray->getSize() << " ?= " << 5 << std::endl;
  assert(stringArray->getSize() == 5);
  std::cout << stringArray->getValuesString() << " ?= " << "foo bar cat dog blah" << std::endl;
  assert(stringArray->getValuesString().compare("foo bar cat dog blah") == 0);

  shared_ptr<XdmfHDF5Writer> stringWriter = 
    XdmfHDF5Writer::New("testString.h5");
  stringArray->accept(stringWriter);

  stringArray->release();
  stringArray->read();
  std::cout << stringArray->getSize() << " ?= " << 5 << std::endl;
  assert(stringArray->getSize() == 5);
  std::cout << stringArray->getValuesString() << " ?= " << "foo bar cat dog blah" << std::endl;
  assert(stringArray->getValuesString().compare("foo bar cat dog blah") == 0);
  std::cout << stringArray->getValue<std::string>(0) << " ?= " << "foo" << std::endl;
  assert(stringArray->getValue<std::string>(0).compare("foo") == 0);
  
  shared_ptr<XdmfArray> dimensionsArray = XdmfArray::New();
  std::vector<unsigned int> dimensions(3);
  dimensions[0] = 2;
  dimensions[1] = 3;
  dimensions[2] = 4;
  dimensionsArray->resize<double>(dimensions);
  double data[24] = {0.0, 1.0, 2.0, 3.0, 4.0,
                     5.0, 6.0, 7.0, 8.0, 9.0,
                     10.0, 11.0, 12.0, 13.0, 14.0,
                     15.0, 16.0, 17.0, 18.0, 19.0,
                     20.0, 21.0, 22.0, 23.0};
  dimensionsArray->insert<double>(0, data, 24);
  shared_ptr<XdmfHDF5Writer> dimensionsWriter = 
    XdmfHDF5Writer::New("testDimensions.h5");
  dimensionsWriter->setChunkSize(4);
  dimensionsArray->accept(dimensionsWriter);

  dimensionsArray->release();
  dimensionsArray->read();
  std::cout << dimensionsArray->getSize() << " ?= " << 24 << std::endl;
  assert(dimensionsArray->getSize() == 24);
  std::vector<unsigned int> readDimensions = dimensionsArray->getDimensions();
  std::cout << readDimensions.size() << " ?= " << 3 << std::endl;
  assert(readDimensions.size() == 3);
  std::cout << readDimensions[0] << " ?= " << 2 << std::endl;
  std::cout << readDimensions[1] << " ?= " << 3 << std::endl;
  std::cout << readDimensions[2] << " ?= " << 4 << std::endl;
  assert(readDimensions[0] == 2);
  assert(readDimensions[1] == 3);
  assert(readDimensions[2] == 4);

  shared_ptr<XdmfHDF5Writer> largeArrayWriter = 
    XdmfHDF5Writer::New("testLargeArray.h5");
  largeArrayWriter->setChunkSize(1500);
  shared_ptr<XdmfArray> largeArray = XdmfArray::New();
  std::vector<unsigned int> largeDimensions(2);
  largeDimensions[0] = 1000;
  largeDimensions[1] = 3;
  largeArray->resize<double>(largeDimensions);
  largeArray->accept(largeArrayWriter);

}
