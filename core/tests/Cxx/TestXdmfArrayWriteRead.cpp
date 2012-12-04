#include "XdmfArray.hpp"
#include "XdmfHDF5Writer.hpp"

int main(int, char **)
{
  int values[] = {1, 2, 3, 4};

  shared_ptr<XdmfArray> array = XdmfArray::New();
  array->insert(0, &values[0], 4, 1, 1);
  assert(array->getSize() == 4);
  assert(array->getValuesString().compare("1 2 3 4") == 0);

  shared_ptr<XdmfHDF5Writer> writer = XdmfHDF5Writer::New("test.h5");
  array->accept(writer);

  assert(array->getSize() == 4);
  assert(array->getValuesString().compare("1 2 3 4") == 0);

  array->release();
  assert(array->getValuesString() == "");
  assert(array->getSize() == 4);

  array->read();
  assert(array->getValuesString().compare("1 2 3 4") == 0);

  shared_ptr<XdmfArray> stringArray = XdmfArray::New();
  stringArray->pushBack<std::string>("foo");
  stringArray->pushBack<std::string>("bar");
  stringArray->pushBack<std::string>("cat");
  stringArray->pushBack<std::string>("dog");
  stringArray->pushBack<std::string>("blah");
  assert(stringArray->getSize() == 5);
  assert(stringArray->getValuesString().compare("foo bar cat dog blah") == 0);

  shared_ptr<XdmfHDF5Writer> stringWriter = 
    XdmfHDF5Writer::New("testString.h5");
  stringArray->accept(stringWriter);

  stringArray->release();
  stringArray->read();
  assert(stringArray->getSize() == 5);
  assert(stringArray->getValuesString().compare("foo bar cat dog blah") == 0);
  assert(stringArray->getValue<std::string>(0).compare("foo") == 0);
  
  /*
  assert(array->getSize() == 4);
  assert(array->getValuesString().compare("1 2 3 4") == 0);

  array->release();
  assert(array->getValuesString() == "");
  assert(array->getSize() == 4);

  array->read();
  assert(array->getValuesString().compare("1 2 3 4") == 0);
  */

}
