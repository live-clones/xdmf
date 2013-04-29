#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"

#include <iostream>

int main(int, char **)
{
  shared_ptr<XdmfArray> resultArray = XdmfArray::New();
  shared_ptr<XdmfArray> insertArray1 = XdmfArray::New();
  shared_ptr<XdmfArray> insertArray2 = XdmfArray::New();
  shared_ptr<XdmfArray> insertArray3 = XdmfArray::New();
  shared_ptr<XdmfArray> insertArray4 = XdmfArray::New();

  for (int i = 0; i< 10; i++)
  {
   insertArray1->pushBack(1);
  }

  for (int i = 0; i< 10; i++)
  {
   insertArray2->pushBack(2);
  }

  for (int i = 0; i< 10; i++)
  {
   insertArray3->pushBack(3);
  }

  for (int i = 0; i< 10; i++)
  {
   insertArray4->pushBack(4);
  }

  resultArray->insert(0, insertArray1, 0, 10, 4, 1);
  resultArray->insert(1, insertArray2, 0, 10, 4, 1);
  resultArray->insert(2, insertArray3, 0, 10, 4, 1);
  resultArray->insert(3, insertArray4, 0, 10, 4, 1);

  printf("result array contains:\n%s\n", resultArray->getValuesString());
  printf("result should be:\n%s\n", "1 2 3 4 1 2 3 4 1 2 3 4 1 2 3 4 1 2 3 4 1 2 3 4 1 2 3 4 1 2 3 4 1 2 3 4 1 2 3 4");

  assert(resultArray->getValuesString().compare("1 2 3 4 1 2 3 4 1 2 3 4 1 2 3 4 1 2 3 4 1 2 3 4 1 2 3 4 1 2 3 4 1 2 3 4 1 2 3 4") == 0);

  return 0;
}
