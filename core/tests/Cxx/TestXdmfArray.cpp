#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"

#include <iostream>

int main(int, char **)
{

  int values[] = {1, 2, 3, 4};

  //
  // COPIES
  //

  /**
   * Array stride = 1, Values stride = 1
   */
  shared_ptr<XdmfArray> array = XdmfArray::New();
  assert(array->getSize() == 0);
  assert(array->getArrayType() == XdmfArrayType::Uninitialized());
  assert(array->getValuesString() == "");
  assert(array->getValuesInternal() == NULL);
  std::vector<unsigned int> dimensions = array->getDimensions();
  assert(dimensions.size() == 1);
  assert(dimensions[0] == 0);
  assert(array->getDimensionsString().compare("0") == 0);
  array->insert(0, &values[0], 4, 1, 1);
  assert(array->getSize() == 4);
  assert(array->getArrayType() == XdmfArrayType::Int32());
  assert(array->getValuesString().compare("1 2 3 4") == 0);
  const int * const arrayPointer = 
    static_cast<int *>(array->getValuesInternal());
  assert(arrayPointer[0] == 1);
  assert(arrayPointer[1] == 2);
  assert(arrayPointer[2] == 3);
  assert(arrayPointer[3] == 4);
  // Assert we copied values correctly
  shared_ptr<std::vector<int> > storedValues =
    array->getValuesInternal<int>();
  assert((*storedValues)[0] == 1);
  assert((*storedValues)[1] == 2);
  assert((*storedValues)[2] == 3);
  assert((*storedValues)[3] == 4);
  // Assert we can copy values out correctly
  std::vector<int> outValues(4);
  array->getValues(0, &outValues[0], 4);
  for(unsigned int i=0; i<outValues.size(); ++i) {
    assert(outValues[i] == values[i]);
  }

  /**
   * Array stride = 2, Values stride = 1
   */
  shared_ptr<XdmfArray> array2 = XdmfArray::New();
  array2->insert(0, &values[0], 2, 2, 1);
  assert(array2->getSize() == 3);
  assert(array2->getArrayType() == XdmfArrayType::Int32());
  assert(array2->getValuesString().compare("1 0 2") == 0);
  storedValues = array2->getValuesInternal<int>();
  assert((*storedValues)[0] == 1);
  assert((*storedValues)[1] == 0);
  assert((*storedValues)[2] == 2);

  /**
   * Array stride = 1, Values stride = 2
   */
  shared_ptr<XdmfArray> array3 = XdmfArray::New();
  array3->insert(0, &values[0], 2, 1, 2);
  assert(array3->getSize() == 2);
  assert(array3->getArrayType() == XdmfArrayType::Int32());
  assert(array3->getValuesString().compare("1 3") == 0);
  storedValues = array3->getValuesInternal<int>();
  assert((*storedValues)[0] == 1);
  assert((*storedValues)[1] == 3);

  /**
   * Array stride = 2, Values stride = 2
   */
  shared_ptr<XdmfArray> array4 = XdmfArray::New();
  array4->insert(0, &values[0], 2, 2, 2);
  assert(array4->getSize() == 3);
  assert(array4->getValuesString().compare("1 0 3") == 0);
  storedValues = array4->getValuesInternal<int>();
  assert((*storedValues)[0] == 1);
  assert((*storedValues)[1] == 0);
  assert((*storedValues)[2] == 3);

  /**
   * Copy values from another XdmfArray
   * Array stride = 1, Values stride = 1
   */
  shared_ptr<XdmfArray> array5 = XdmfArray::New();
  array5->insert(0, array, 1, 3);
  assert(array5->getSize() == 3);
  assert(array5->getArrayType() == array->getArrayType());
  storedValues = array5->getValuesInternal<int>();
  assert(array5->getSize() == 3);
  assert(array->getSize() == 4);
  assert(array5->getValuesString().compare("2 3 4") == 0);
  array5->insert(1, array, 1, 3);
  assert(array5->getValuesString().compare("2 2 3 4") == 0);
  array5->clear();
  assert(array5->getSize() == 0);
  array5->insert(0, array, 1, 3, 2, 1);
  assert(array5->getValuesString().compare("2 0 3 0 4") == 0);
  array5->clear();
  array5->insert(0, array, 1, 2, 2, 2);
  assert(array5->getValuesString().compare("2 0 4") == 0);

  /**
   * Single Insertion
   */
  shared_ptr<XdmfArray> array10 = XdmfArray::New();
  array10->insert<unsigned int>(0, 1);
  assert(array10->getSize() == 1);
  assert(array10->getValue<unsigned int>(0) == 1);

  //
  // PUSHBACK
  //
  shared_ptr<XdmfArray> pushBackArray = XdmfArray::New();
  assert(pushBackArray->getSize() == 0);
  pushBackArray->pushBack(10);
  assert(pushBackArray->getSize() == 1);

  //
  // SETS
  //

  /**
   * Simple Set
   */
  array5->setValuesInternal(values, 2, 0);
  assert(array5->getSize() == 2);
  assert(array5->getArrayType() == XdmfArrayType::Int32());
  assert(array5->getValuesString().compare("1 2") == 0);
  const int * const array5Pointer = 
    static_cast<int *>(array5->getValuesInternal());
  assert(array5Pointer[0] == 1);
  assert(array5Pointer[1] == 2);
  // Assert we can copy values out correctly
  array->getValues(1, &outValues[0], 3);
  assert(outValues[0] == values[1]);

  /**
   * Copy after Set
   */
  array5->setValuesInternal(&values[1], 3, 0);
  assert(array5->getSize() == 3);
  assert(array5->getValuesString().compare("2 3 4") == 0);
   int zero = 0;
   array5->insert(3, &zero, 1, 1, 0);
  assert(array5->getSize() == 4);
  assert(array5->getValuesString().compare("2 3 4 0") == 0);

  /**
   * Set and Ownership transfer
   */
  double * doubleValues = new double[3];
  doubleValues[0] = 0;
  doubleValues[1] = 1.1;
  doubleValues[2] = 10.1;
  array5->setValuesInternal(doubleValues, 3, 1);
  assert(array5->getSize() == 3);
  assert(array5->getArrayType() == XdmfArrayType::Float64());

  //
  // SHARED ASSIGNMENTS
  //

  /**
   * Shared vector assignment
   */
  shared_ptr<std::vector<char> > values2(new std::vector<char>());
  values2->push_back(-2);
  values2->push_back(-1);
  values2->push_back(0);
  values2->push_back(1);
  values2->push_back(2);
  shared_ptr<XdmfArray> array6 = XdmfArray::New();
  array6->setValuesInternal(values2);
  assert(array6->getSize() == 5);
  assert(array6->getArrayType() == XdmfArrayType::Int8());
  assert(array6->getValuesString().compare("-2 -1 0 1 2") == 0);
  // Assert we have the same values!
  shared_ptr<std::vector<char> > storedValues2 =
    array6->getValuesInternal<char>();
  int i = 0;
  for(std::vector<char>::const_iterator iter = storedValues2->begin();
      iter != storedValues2->end();
      ++iter, ++i) {
    assert(*iter == values2->operator[](i));
  }
  // Now modify original array
  values2->push_back(8);
  assert(array6->getSize() == 6);
  assert(array6->getValuesString().compare("-2 -1 0 1 2 8") == 0);
  // Assert we have the same values!
  i = 0;
  for(std::vector<char>::const_iterator iter = storedValues2->begin();
      iter != storedValues2->end();
      ++iter, ++i) {
    assert(*iter == values2->operator[](i));
  }
  // Assert we can't get an int vector out of our array.
  shared_ptr<std::vector<int> > storedValues2Int =
    array6->getValuesInternal<int>();
  assert(storedValues2Int == NULL);

  //
  // SWAPS
  //

  /**
   * Swap values from a vector
   */
  std::vector<short> values3;
  values3.push_back(-1);
  values3.push_back(0);
  values3.push_back(1);
  shared_ptr<XdmfArray> array7 = XdmfArray::New();
  array7->swap(values3);
  assert(values3.size() == 0);
  assert(array7->getSize() == 3);
  assert(array7->getArrayType() == XdmfArrayType::Int16());
  shared_ptr<std::vector<short> > storedValues3 =
    array7->getValuesInternal<short>();
  assert((*storedValues3)[0] == -1);
  assert((*storedValues3)[1] == 0);
  assert((*storedValues3)[2] == 1);

  /**
   * Swap values from a shared vector
   */
  array7->release();
  array7->swap(values2);
  assert(storedValues2->size() == 0);
  assert(array7->getSize() == 6);
  assert(array7->getArrayType() == XdmfArrayType::Int8());

  /**
   * Swap values from an XdmfArray (with copy)
   */
  array7->release();
  array7->swap(array4);
  assert(array4->getSize() == 0);
  assert(array7->getSize() == 3);

  /**
   * Swap values from an XdmfArray (with pointer)
   */
  array5->setValuesInternal(&values[1], 3, 0);
  assert(array5->getSize() == 3);
  array7->release();
  array7->swap(array5);
  assert(array5->getSize() == 0);
  assert(array7->getSize() == 3);

  //
  // Various STL like functions
  //

  /**
   * Resize
   */
  shared_ptr<XdmfArray> array8 = XdmfArray::New();
  array8->insert(0, &values[0], 4, 1, 1);
  array8->resize(5, 0);
  assert(array8->getValuesString().compare("1 2 3 4 0") == 0);
  array8->resize(3, 0);
  assert(array8->getValuesString().compare("1 2 3") == 0);
  array8->resize(8, 1.1);
  assert(array8->getValuesString().compare("1 2 3 1 1 1 1 1") == 0);

  /**
   * Erase
   */
  array8->erase(0);
  assert(array8->getValuesString().compare("2 3 1 1 1 1 1") == 0);

  /**
   * Reserve / Capacity
   */
  array8->reserve(50);
  assert(array8->getCapacity() >= 50);

  assert(array8->getName().compare("") == 0);
  array8->setName("BLAH");
  assert(array8->getName().compare("BLAH") == 0);

  /**
   * Dimensions
   */
  dimensions.resize(2);
  dimensions[0] = 3;
  dimensions[1] = 3;
  shared_ptr<XdmfArray> array9 = XdmfArray::New();
  array9->initialize(XdmfArrayType::Float64(),
                     dimensions);
  assert(array9->getDimensionsString() == "3 3");
  assert(array9->getSize() == 9);
  array9->insert<double>(9, 1.0);
  assert(array9->getDimensionsString() == "10");

  //
  // STRINGS
  //
  shared_ptr<XdmfArray> stringArray = XdmfArray::New();
  stringArray->resize<std::string>(3, "");
  std::string firstValue = stringArray->getValue<std::string>(0);
  assert(firstValue.compare("") == 0);
  stringArray->insert<std::string>(0, "foo");
  stringArray->insert<std::string>(1, "bar");
  stringArray->insert<std::string>(2, "cat");
  firstValue = stringArray->getValue<std::string>(0);
  assert(firstValue.compare("foo") == 0);
  std::string allValues = stringArray->getValuesString();
  assert(allValues.compare("foo bar cat") == 0);
  stringArray->insert<int>(3, 1);
  allValues = stringArray->getValuesString();
  assert(allValues.compare("foo bar cat 1") == 0);
  int num = stringArray->getValue<int>(3);
  assert(num == 1);

  /**
   * ArrayType compatibility
   */
  std::vector<shared_ptr<const XdmfArrayType> > typeVector;
  typeVector.push_back(XdmfArrayType::Int8());
  typeVector.push_back(XdmfArrayType::Int16());
  typeVector.push_back(XdmfArrayType::Int32());
  typeVector.push_back(XdmfArrayType::Int64());
  typeVector.push_back(XdmfArrayType::UInt8());
  typeVector.push_back(XdmfArrayType::UInt16());
  typeVector.push_back(XdmfArrayType::UInt32());
  typeVector.push_back(XdmfArrayType::Float32());
  typeVector.push_back(XdmfArrayType::Float64());
  typeVector.push_back(XdmfArrayType::String());

  for (unsigned int i = 0; i < typeVector.size(); ++i) {
    for (unsigned int j = 0; j < typeVector.size(); ++j) {
      shared_ptr<XdmfArray> valTypeArray1 = XdmfArray::New();
      shared_ptr<XdmfArray> valTypeArray2 = XdmfArray::New();
      shared_ptr<const XdmfArrayType> valType1 = typeVector[i];
      shared_ptr<const XdmfArrayType> valType2 = typeVector[j];
      valTypeArray1->initialize(valType1);
      valTypeArray2->initialize(valType2);
      valTypeArray1->pushBack(-1.25);
      valTypeArray2->pushBack(-1.25);
      shared_ptr<XdmfArray> valIntersectArray = XdmfArray::New();
      shared_ptr<const XdmfArrayType> valIntersectType = XdmfArrayType::comparePrecision(typeVector[i], typeVector[j]);
      valIntersectArray->initialize(valIntersectType);
      valIntersectArray->insert(0, valTypeArray1, 0, 1, 1);
      valIntersectArray->insert(1, valTypeArray2, 0, 1, 1);
      std::stringstream output1;
      std::stringstream output2;
      if (valType1 == XdmfArrayType::Int8() && valType2 == XdmfArrayType::String()) {
        output1 << valTypeArray1->getValue<char>(0) << " " << valTypeArray2->getValuesString() << std::endl;
      }
      else if (valType1 == XdmfArrayType::UInt8() && valType2 == XdmfArrayType::String()) {
        output1 << valTypeArray1->getValue<unsigned char>(0) << " " << valTypeArray2->getValuesString() << std::endl;
      }
      else if (valType2 == XdmfArrayType::Int8() && valType1 == XdmfArrayType::String()) {
        output1 << valTypeArray1->getValuesString() << " " << valTypeArray2->getValue<char>(0) << std::endl;
      }
      else if (valType2 == XdmfArrayType::UInt8() && valType1 == XdmfArrayType::String()) {
        output1 << valTypeArray1->getValuesString() << " " << valTypeArray2->getValue<unsigned char>(0) << std::endl;
      }
      else {
        output1 << valTypeArray1->getValuesString() << " " << valTypeArray2->getValuesString() << std::endl;
      }
      output2 << valIntersectArray->getValuesString() << std::endl;
      //std::cout << output1.str() << output2.str();
      assert(output1.str() == output2.str());
    }
  }

  return 0;
}
