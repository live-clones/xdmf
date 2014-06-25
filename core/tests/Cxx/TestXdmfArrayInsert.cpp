#include "XdmfArray.hpp"

#include <iostream>

int main(int, char **)
{

  // Insert from another array
  shared_ptr<XdmfArray> array1 = XdmfArray::New();
  shared_ptr<XdmfArray> insertArray1 = XdmfArray::New();
  shared_ptr<XdmfArray> insertArray2 = XdmfArray::New();
  shared_ptr<XdmfArray> insertArray3 = XdmfArray::New();
  shared_ptr<XdmfArray> insertArray4 = XdmfArray::New();

  insertArray1->resize<int>(10, 1);
  insertArray2->resize<int>(10, 2);
  insertArray3->resize<int>(10, 3);
  insertArray4->resize<int>(10, 4);

  array1->insert(0, insertArray1, 0, 10, 4, 1);
  array1->insert(1, insertArray2, 0, 10, 4, 1);
  array1->insert(2, insertArray3, 0, 10, 4, 1);
  array1->insert(3, insertArray4, 0, 10, 4, 1);

  std::cout << array1->getValuesString() << " ?= "
            << "1 2 3 4 1 2 3 4 1 2 3 4 1 2 3 4 1 "
            << "2 3 4 1 2 3 4 1 2 3 4 1 2 3 4 1 2 "
            << "3 4 1 2 3 4" << std::endl;

  assert(array1->getValuesString().compare("1 2 3 4 1 2 3 4 1 2 3 4 1 2 3 4 1 "
                                           "2 3 4 1 2 3 4 1 2 3 4 1 2 3 4 1 2 "
                                           "3 4 1 2 3 4") == 0);

  // Insert from vector
  std::vector<int> insertArray5(10, 1);
  std::vector<int> insertArray6(10, 2);
  std::vector<int> insertArray7(10, 3);
  std::vector<int> insertArray8(10, 4);

  shared_ptr<XdmfArray> array2 = XdmfArray::New();

  array2->insert(0, &(insertArray5[0]), 10, 4, 1);
  array2->insert(1, &(insertArray6[0]), 10, 4, 1);
  array2->insert(2, &(insertArray7[0]), 10, 4, 1);
  array2->insert(3, &(insertArray8[0]), 10, 4, 1);

  std::cout << array2->getValuesString() << " ?= "
            << "1 2 3 4 1 2 3 4 1 2 3 4 1 2 3 4 1 "
            << "2 3 4 1 2 3 4 1 2 3 4 1 2 3 4 1 2 "
            << "3 4 1 2 3 4" << std::endl;

  assert(array2->getValuesString().compare("1 2 3 4 1 2 3 4 1 2 3 4 1 2 3 4 1 "
                                           "2 3 4 1 2 3 4 1 2 3 4 1 2 3 4 1 2 "
                                           "3 4 1 2 3 4") == 0);

  // Insert from vector string
  std::vector<std::string> insertArray9(10, "1");
  std::vector<std::string> insertArray10(10, "2");
  std::vector<std::string> insertArray11(10, "3");
  std::vector<std::string> insertArray12(10, "4");

  shared_ptr<XdmfArray> array3 = XdmfArray::New();

  array3->insert(0, &(insertArray9[0]), 10, 4, 1);
  array3->insert(1, &(insertArray10[0]), 10, 4, 1);
  array3->insert(2, &(insertArray11[0]), 10, 4, 1);
  array3->insert(3, &(insertArray12[0]), 10, 4, 1);

  std::cout << array3->getValuesString() << " ?= "
            << "1 2 3 4 1 2 3 4 1 2 3 4 1 2 3 4 1 "
            << "2 3 4 1 2 3 4 1 2 3 4 1 2 3 4 1 2 "
            << "3 4 1 2 3 4" << std::endl;

  assert(array3->getValuesString().compare("1 2 3 4 1 2 3 4 1 2 3 4 1 2 3 4 1 "
                                           "2 3 4 1 2 3 4 1 2 3 4 1 2 3 4 1 2 "
                                           "3 4 1 2 3 4") == 0);

  return 0;
}
