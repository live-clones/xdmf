/*****************************************************************************/
/*                                    Xdmf                                   */
/*                       Extensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfArray.tpp                                                       */
/*                                                                           */
/*  Author:                                                                  */
/*     Kenneth Leiter                                                        */
/*     kenneth.leiter@arl.army.mil                                           */
/*     US Army Research Laboratory                                           */
/*     Aberdeen Proving Ground, MD                                           */
/*                                                                           */
/*     Copyright @ 2011 US Army Research Laboratory                          */
/*     All Rights Reserved                                                   */
/*     See Copyright.txt for details                                         */
/*                                                                           */
/*     This software is distributed WITHOUT ANY WARRANTY; without            */
/*     even the implied warranty of MERCHANTABILITY or FITNESS               */
/*     FOR A PARTICULAR PURPOSE.  See the above copyright notice             */
/*     for more information.                                                 */
/*                                                                           */
/*****************************************************************************/

#include <functional>
#include <numeric>
#include <limits>
#include <sstream>
#include <string.h>
//#include <type_traits>
#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfError.hpp"

#include <stdio.h>
#include <iostream>
#include <typeinfo>

template <typename T>
class XdmfArray::XdmfVisitor
{
  public:
    // Parameters other than the array pointer will be passed via the constructor
    // This includes output parameters
    XdmfVisitor()
    {
      mName = "none";
    }

    XdmfVisitor(const XdmfVisitor<T> & orig)
    {
      mName = orig.mName;
    }

    ~XdmfVisitor()
    {
    }

    XdmfVisitor& operator=(const XdmfVisitor &orig)
    {
      mName = orig.mName;
    }

    template <typename U>
    T
    operator()(U * array)
    {
      printf("blank operator\n");
      return NULL;
    }

    virtual
    T
    operator()() 
    {
      printf("blank operator\n");
      return NULL;
    }

  std::string mName;
};

template <>
class XdmfArray::XdmfVisitor<void>
{
  public:
    // Parameters other than the array pointer will be passed via the constructor
    // This includes output parameters
    XdmfVisitor()
    {
      mName = "none";
    }

    XdmfVisitor(const XdmfVisitor<void> & orig)
    {
      mName = orig.mName;
    }

    ~XdmfVisitor()
    {
    }

    template <typename U>
    void
    operator()(U * array)
    {
      printf("blank operator\n");
    }

    virtual
    void
    operator()()
    {
      printf("blank operator\n");
    }

  std::string mName;
};

template <typename T, typename U>
T
XdmfArray::ApplyVisitor(const XdmfArray * const source, XdmfVisitor<T> & visitor, void * array, U * internal)
{
  // class the array to the appropriate type
  // Then pass to the function that is the visitor
  shared_ptr<const XdmfArrayType> compareType = source->getArrayType();
  if (compareType == XdmfArrayType::UInt8())
  {
    return ApplyVisitorFunction(visitor, (unsigned char *)array, internal);
  }
  else if (compareType == XdmfArrayType::UInt16())
  {
    return ApplyVisitorFunction(visitor, (unsigned short *)array, internal);
  }
  else if (compareType == XdmfArrayType::UInt32())
  {
    return ApplyVisitorFunction(visitor, (unsigned int *)array, internal);
  }
  else if (compareType == XdmfArrayType::Int8())
  {
    return ApplyVisitorFunction(visitor, (char *)array, internal);
  }
  else if (compareType == XdmfArrayType::Int16())
  {
    return ApplyVisitorFunction(visitor, (short *)array, internal);
  }
  else if (compareType == XdmfArrayType::Int32())
  {
    int * classedArray = static_cast<int *>(array);
    return ApplyVisitorFunction(visitor, classedArray, internal);
  }
  else if (compareType == XdmfArrayType::Int64())
  {
    return ApplyVisitorFunction(visitor, (long *)array, internal);
  }
  else if (compareType == XdmfArrayType::Float32())
  {
    return ApplyVisitorFunction(visitor, (float *)array, internal);
  }
  else if (compareType == XdmfArrayType::Float64())
  {
    return ApplyVisitorFunction(visitor, (double *)array, internal);
  }
  else if (compareType == XdmfArrayType::String())
  {
    return ApplyVisitorFunction(visitor, (std::string *)array, internal);
  }
  // Case where pointer is null? TODO currently fails terribly
  else if (compareType == XdmfArrayType::Uninitialized())
  {
    return ApplyVisitorFunction(visitor, (void *)array, internal);
  }
  else
  {
    XdmfError::message(XdmfError::FATAL,
                       "Error in ApplyVisitor: Invalid ArrayType.");
  }
}


class XdmfArray::Erase : public XdmfArray::XdmfVisitor<void> {
public:

  Erase(XdmfArray * const array,
        const unsigned int index) :
    mArray(array),
    mIndex(index)
  {
      mName = "Erase";
  }

  void
  operator()() const
  {
    return;
  }

  template<typename T>
  void
  operator()(T * array) const
  {
    for (unsigned int i = mIndex; i < mArray->getSize() - 1; ++i)
    {
      array[i] = array[i+1];
    }
    array[mArray->getSize() - 1] = 0;
  }

  void
  operator()(void * array) const
  {
    this->operator()<char>((char *)array);
  }

  void
  operator()(std::string * array) const
  {
    // TODO STRING
    // Class to char[100][]
    // Then shift all characters forward 100
    // And set the last set of characters to 100
    for (unsigned int i = mIndex; i < mArray->getSize() - 1; ++i)
    {
      array[i] = array[i+1];
    }
    array[mArray->getSize() - 1] = "";
  }

private:

  XdmfArray * const mArray;
  const unsigned int mIndex;
};

template <typename T>
class XdmfArray::GetValue : public XdmfArray::XdmfVisitor<T> {
public:

  GetValue(const unsigned int index) :
    mIndex(index)
  {
     this->mName = "GetValue";
  }

  T
  operator()(const void * array) const
  {
    return this->operator()<char>((char *)array);
  }

  T
  operator()(const std::string * array) const
  {
    // TODO
    if (array == NULL)
    {
      return 0;
    }
    else
    {
      // TODO STRING
      return atof(array[mIndex].c_str());
    }

  }

  template<typename U>
  T
  operator()(const U * array) const
  {
    if (array == NULL)
    {
      return 0;
    }
    else
    {
      return (T)array[mIndex];
    }
  }

private:

  const unsigned int mIndex;
};

template <>
class XdmfArray::GetValue<void> : public XdmfArray::XdmfVisitor<void> {
public:

  GetValue(const unsigned int index) :
    mIndex(index)
  {
     this->mName = "GetValue";
  }

  template<typename U>
  void
  operator()(const U * array) const
  {
  }

private:

  const unsigned int mIndex;
};


template <>
class XdmfArray::GetValue<std::string> :
  public XdmfArray::XdmfVisitor<std::string> {
public:

  GetValue(const unsigned int index) :
    mIndex(index)
  {
      mName = "GetValue";
  }

  std::string
  operator()() const
  {
    return "";
  }

  std::string
  operator()(const std::string * array) const
  {
    if (array == NULL)
    {
      return "";
    }
    else
    {
      // TODO STRING
      return array[mIndex];
    }
  }

  std::string
  operator()(const void * array) const
  {
    return this->operator()((char *)array);
  }

  template<typename U>
  std::string
  operator()(const U * array) const
  {
    if (array == NULL)
    {
      return "";
    }
    else
    {
      std::stringstream value;
      value << array[mIndex];
      return value.str();
    }
  }

private:

  const unsigned int mIndex;
};

template <typename T>
class XdmfArray::GetValues : public XdmfArray::XdmfVisitor<void> {
public:

  GetValues(const XdmfArray * array,
            const unsigned int startIndex,
            T * valuesPointer,
            const unsigned int numValues,
            const unsigned int arrayStride,
            const unsigned int valuesStride) :
    mArray(array),
    mStartIndex(startIndex),
    mValuesPointer(valuesPointer),
    mNumValues(numValues),
    mArrayStride(arrayStride),
    mValuesStride(valuesStride)
  {
    mName = "GetValues";
  }

  void
  operator()(const void * array) const
  {
    if (array == NULL)
    {
      return;
    }
    else
    {
      for(unsigned int i=0; i<mNumValues; ++i) {
        mValuesPointer[i*mValuesStride] =
          (T)((char *)array)[mStartIndex + i*mArrayStride];
      }
    }
  }

  void
  operator()(const std::string * array) const
  {
    if (array == NULL)
    {
      return;
    }
    else
    {
      // TODO STRING
      for(unsigned int i=0; i<mNumValues; ++i) {
        mValuesPointer[i*mValuesStride] =
          (T)atof(array[mStartIndex + i*mArrayStride].c_str());
      }
    }
  }

  template<typename U>
  void
  operator()(const U * array) const
  {
    if (array == NULL)
    {
      return;
    }
    else
    {
      for(unsigned int i=0; i<mNumValues; ++i) {
        U copyValue = array[mStartIndex + i*mArrayStride];
        T setValue = (T)copyValue;
        mValuesPointer[i*mValuesStride] = setValue;
/*
        mValuesPointer[i*mValuesStride] =
          (T)array[mStartIndex + i*mArrayStride];
*/
      }
    }
  }

private:

  const XdmfArray * mArray;
  const unsigned int mStartIndex;
  T * mValuesPointer;
  const unsigned int mNumValues;
  const unsigned int mArrayStride;
  const unsigned int mValuesStride;
};

template <>
class XdmfArray::GetValues<std::string> : public XdmfArray::XdmfVisitor<void> {
public:

  GetValues(const XdmfArray * array,
            const unsigned int startIndex,
            std::string * valuesPointer,
            const unsigned int numValues,
            const unsigned int arrayStride,
            const unsigned int valuesStride) :
    mArray(array),
    mStartIndex(startIndex),
    mValuesPointer(valuesPointer),
    mNumValues(numValues),
    mArrayStride(arrayStride),
    mValuesStride(valuesStride)
  {
    mName = "GetValues";
  }

  template<typename U>
  void
  operator()(const U * array) const
  {
    if (array == NULL)
    {
      return;
    }
    else
    {
      for(unsigned int i=0; i<mNumValues; ++i) {
        std::stringstream value;
        value << array[mStartIndex + i*mArrayStride];
        mValuesPointer[i*mValuesStride] = value.str();
      }
    }
  }

private:

  const XdmfArray * mArray;
  const unsigned int mStartIndex;
  std::string * mValuesPointer;
  const unsigned int mNumValues;
  const unsigned int mArrayStride;
  const unsigned int mValuesStride;
};

template <>
class XdmfArray::GetValues<void> : public XdmfArray::XdmfVisitor<void> {
public:

  GetValues(const XdmfArray * array,
            const unsigned int startIndex,
            void * valuesPointer,
            const unsigned int numValues,
            const unsigned int arrayStride,
            const unsigned int valuesStride) :
    mArray(array),
    mStartIndex(startIndex),
    mValuesPointer(valuesPointer),
    mNumValues(numValues),
    mArrayStride(arrayStride),
    mValuesStride(valuesStride)
  {
    mName = "GetValues";
  }

  // void pointers are considered char pointers
  void
  operator()(void * array) const
  {
    XdmfArray::GetValues<char> visitor = GetValues<char>(mArray,
                                                           mStartIndex,
                                                           (char *) mValuesPointer,
                                                           mNumValues,
                                                           mArrayStride,
                                                           mValuesStride);
    ApplyVisitor(mArray,
                 visitor,
                 array,
                 (char *) array);
  }

private:

  const XdmfArray * mArray;
  const unsigned int mStartIndex;
  void * mValuesPointer;
  const unsigned int mNumValues;
  const unsigned int mArrayStride;
  const unsigned int mValuesStride;
};

template<>
void
XdmfArray::GetValues<std::string>::operator()<void>(const void * array) const;

template <typename T>
class XdmfArray::Insert : public XdmfArray::XdmfVisitor<void> {
public:

  Insert(XdmfArray * const array,
         const unsigned int startIndex,
         const T * const valuesPointer,
         const unsigned int numValues,
         const unsigned int arrayStride,
         const unsigned int valuesStride,
         std::vector<unsigned int> & dimensions) :
    mArray(array),
    mStartIndex(startIndex),
    mValuesPointer(valuesPointer),
    mNumValues(numValues),
    mArrayStride(arrayStride),
    mValuesStride(valuesStride),
    mDimensions(dimensions)
  {
    mName = "Insert";
  }

  Insert(const XdmfArray::Insert<T> & orig):
    mArray(orig.mArray),
    mStartIndex(orig.mStartIndex),
    mValuesPointer(orig.mValuesPointer),
    mNumValues(orig.mNumValues),
    mArrayStride(orig.mArrayStride),
    mValuesStride(orig.mValuesStride),
    mDimensions(orig.mDimensions)
  {
  }

  // If not initialized, initialize to the type being inserted
  void
  operator()(void * array) const
  {
    mArray->initialize<T>();
    XdmfArray::Insert<T> visitor = *this;
    ApplyVisitor(mArray,
                 visitor,
                 mArray->mArray,
                 (T*) array);
  }

  void
  operator()(std::string * array) const
  {
    // TODO STRING
    unsigned int size = mStartIndex + (mNumValues - 1) * mArrayStride + 1;
// TODO resize pointer
    if(array->size() < size) {
      array->resize(size);
      mDimensions.clear();
    }
    for(unsigned int i=0; i<mNumValues; ++i) {
      std::stringstream value;
// TODO
//      value << mValuesPointer[i*mValuesStride];
      array[mStartIndex + i*mArrayStride] = value.str();
    }
  }

  template<typename U>
  void
  operator()(U * array) const
  {
    unsigned int size = mStartIndex + (mNumValues - 1) * mArrayStride + 1;
// TODO resize pointer
    if(mArray->getSize() < size) {
      T resizeValue = 0;
      mArray->resize(size, resizeValue);
    }
    array = (U *) mArray->mArray;
    for(unsigned int i=0; i<mNumValues; ++i) {
      T copyValue = mValuesPointer[i*mValuesStride];
      array[mStartIndex + i*mArrayStride] = (U)copyValue;
/*
      array[mStartIndex + i*mArrayStride] =
        ((U *)mValuesPointer)[i*mValuesStride];
*/
    }
  }

private:

  XdmfArray * const mArray;
  const unsigned int mStartIndex;
  const T * const mValuesPointer;
  const unsigned int mNumValues;
  const unsigned int mArrayStride;
  const unsigned int mValuesStride;
  std::vector<unsigned int> & mDimensions;
};

template <>
class XdmfArray::Insert<std::string> : public XdmfArray::XdmfVisitor<void> {
public:

  Insert(XdmfArray * const array,
         const unsigned int startIndex,
         const std::string * const valuesPointer,
         const unsigned int numValues,
         const unsigned int arrayStride,
         const unsigned int valuesStride,
         std::vector<unsigned int> & dimensions) :
    mArray(array),
    mStartIndex(startIndex),
    mValuesPointer(valuesPointer),
    mNumValues(numValues),
    mArrayStride(arrayStride),
    mValuesStride(valuesStride),
    mDimensions(dimensions)
  {
    mName = "Insert";
  }

  void
  operator()(void * array) const
  {
    mArray->initialize<std::string>();
    XdmfArray::Insert<std::string> visitor = *this;
    ApplyVisitor(mArray,
                 visitor,
                 mArray->mArray,
                 (std::string *) array);
  }

  void
  operator()(std::string * array) const
  {
    unsigned int size = mStartIndex + (mNumValues - 1) * mArrayStride + 1;
// TODO resize pointer
    if(array->size() < size) {
      array->resize(size);
      mDimensions.clear();
    }
    for(unsigned int i=0; i<mNumValues; ++i) {
      array[mStartIndex + i*mArrayStride] =
        mValuesPointer[i*mValuesStride].c_str();
    }
  }

  template<typename U>
  void
  operator()(U * array) const
  {
    unsigned int size = mStartIndex + (mNumValues - 1) * mArrayStride + 1;
// TODO resize pointer
    if(mArray->getSize() < size) {
// TODO resize for std::string needs to be fixed
//      mArray->resize(size, std::string(""));
      mDimensions.clear();
    }
    for(unsigned int i=0; i<mNumValues; ++i) {
      array[mStartIndex + i*mArrayStride] =
        (U)atof(mValuesPointer[i*mValuesStride].c_str());
    }
  }

private:

  XdmfArray * const mArray;
  const unsigned int mStartIndex;
  const std::string * const mValuesPointer;
  const unsigned int mNumValues;
  const unsigned int mArrayStride;
  const unsigned int mValuesStride;
  std::vector<unsigned int> & mDimensions;
};

template <>
class XdmfArray::Insert<void> : public XdmfArray::XdmfVisitor<void> {
public:

  Insert(XdmfArray * const array,
         const unsigned int startIndex,
         const void * const valuesPointer,
         const unsigned int numValues,
         const unsigned int arrayStride,
         const unsigned int valuesStride,
         std::vector<unsigned int> & dimensions) :
    mArray(array),
    mStartIndex(startIndex),
    mValuesPointer(valuesPointer),
    mNumValues(numValues),
    mArrayStride(arrayStride),
    mValuesStride(valuesStride),
    mDimensions(dimensions)
  {
    mName = "Insert";
  }

  // void pointers are considered char pointers
  void
  operator()(void * array) const
  {
    mArray->initialize<char>();
    XdmfArray::Insert<char> visitor = Insert<char>(mArray,
                                                        mStartIndex,
                                                        (char *) mValuesPointer,
                                                        mNumValues,
                                                        mArrayStride,
                                                        mValuesStride,
                                                        mDimensions);
    ApplyVisitor(mArray,
                 visitor,
                 mArray->mArray,
                 (char *) array);
  }


private:

  XdmfArray * const mArray;
  const unsigned int mStartIndex;
  const void * const mValuesPointer;
  const unsigned int mNumValues;
  const unsigned int mArrayStride;
  const unsigned int mValuesStride;
  std::vector<unsigned int> & mDimensions;
};

template<typename T>
class XdmfArray::GetValuesString : public XdmfArray::XdmfVisitor<T> {
public:

  GetValuesString(const int arrayPointerNumValues) :
    mArrayPointerNumValues(arrayPointerNumValues)
  {
    mName = "GetValuesString";
  }

  template<typename U>
  T
  operator()(U * array)
  {
    return NULL;
  }

private:

  const unsigned int mArrayPointerNumValues;
};

template<>
class XdmfArray::GetValuesString<void> : public XdmfArray::XdmfVisitor<void> {
public:

  GetValuesString(const int arrayPointerNumValues) :
    mArrayPointerNumValues(arrayPointerNumValues)
  {
    mName = "GetValuesString";
  }

  template<typename U>
  void
  operator()(U * array)
  {
  }

private:

  const unsigned int mArrayPointerNumValues;
};

template<>
class XdmfArray::GetValuesString<std::string> : public XdmfArray::XdmfVisitor<std::string> {
public:

  GetValuesString(const int arrayPointerNumValues) :
    mArrayPointerNumValues(arrayPointerNumValues)
  {
    mName = "GetValuesString";
  }

  std::string
  ToString(std::string value) const
  {
    return value;
  }

  std::string
  ToString(char value) const
  {
    char * returnstring = new char[100];
    sprintf(returnstring, "%d", value);
    return std::string(returnstring);
  }

  std::string
  ToString(short value) const
  {
    char * returnstring = new char[100];
    sprintf(returnstring, "%d", value);
    return std::string(returnstring);
  }

  std::string
  ToString(int value) const
  {
    char * returnstring = new char[100];
    sprintf(returnstring, "%d", value);
    return std::string(returnstring);
  }

  std::string
  ToString(long value) const
  {
    char * returnstring = new char[100];
    sprintf(returnstring, "%ld", value);
    return std::string(returnstring);
  }

  std::string
  ToString(unsigned char value) const
  {
    char * returnstring = new char[100];
    sprintf(returnstring, "%u", value);
    return std::string(returnstring);
  }

  std::string
  ToString(unsigned short value) const
  {
    char * returnstring = new char[100];
    sprintf(returnstring, "%u", value);
    return std::string(returnstring);
  }

  std::string
  ToString(unsigned int value) const
  {
    char * returnstring = new char[100];
    sprintf(returnstring, "%u", value);
    return std::string(returnstring);
  }

  std::string
  ToString(float value) const
  {
    char * returnstring = new char[100];
    sprintf(returnstring, "%g", value);
    return std::string(returnstring);
  }

  std::string
  ToString(double value) const
  {
    char * returnstring = new char[100];
    sprintf(returnstring, "%g", value);
    return std::string(returnstring);
  }

  template<typename T, typename U>
  std::string
  getValuesString(const T * const array,
                  const int numValues) const
  {
    const int lastIndex = numValues - 1;
    if(lastIndex < 0) {
      return "";
    }
    std::string tempReturnString = "";
    for(int i=0; i<lastIndex; ++i) {
      tempReturnString = tempReturnString + ToString((U)(array[i])) + " ";
    }
    tempReturnString = tempReturnString + ToString((U)(array[lastIndex]));
    return tempReturnString;
  }

  std::string
  getValuesString(const char * const array,
                  const int numValues) const
  {
    return getValuesString<char, int>(array, numValues);
  }

  std::string
  getValuesString(const unsigned char * const array,
                  const int numValues) const
  {
    return getValuesString<unsigned char, int>(array, numValues);
  }

  template<typename T>
  std::string
  getValuesString(const T * const array,
                  const int numValues) const
  {
    return getValuesString<T, T>(array, numValues);
  }

  std::string
  operator()() const
  {
    return "";
  }

  template<typename T>
  std::string
  operator()(T * array)
  {
    return getValuesString(array, mArrayPointerNumValues);
  }

private:

  const unsigned int mArrayPointerNumValues;
};

template<>
std::string
XdmfArray::GetValuesString<std::string>::getValuesString<void, void>(const void * const array,
                                                                     const int numValues) const;
/*
{
  const int lastIndex = numValues - 1;

  if(lastIndex < 0) {
    return "";
  }

  std::stringstream toReturn;
  toReturn.precision(std::numeric_limits<char>::digits10 + 2);
  for(int i=0; i<lastIndex; ++i) {
    toReturn << (char)((char *)array)[i] << " ";
  }
  toReturn << (char)((char *)array)[lastIndex];
  return toReturn.str();
}
*/


/*
template <>
class XdmfArray::Insert<std::string> : public XdmfArray::XdmfVisitor<void> {
public:

  Insert(XdmfArray * const array,
         const unsigned int startIndex,
         const std::string * const valuesPointer,
         const unsigned int numValues,
         const unsigned int arrayStride,
         const unsigned int valuesStride,
         std::vector<unsigned int> & dimensions) :
    mArray(array),
    mStartIndex(startIndex),
    mValuesPointer(valuesPointer),
    mNumValues(numValues),
    mArrayStride(arrayStride),
    mValuesStride(valuesStride),
    mDimensions(dimensions)
  {
      mName = "Insert";
  }

  void
  operator()(void * array) const
  {
    mArray->initialize<std::string>();
    ApplyVisitor(mArray,
                 this),
                 mArray->mArray,
                 (std::string *) array);
  }

  void
  operator()(std::string * array) const
  {
    unsigned int size = mStartIndex + (mNumValues - 1) * mArrayStride + 1;
// TODO resize pointer
    if(array->size() < size) {
      array->resize(size);
      mDimensions.clear();
    }
    for(unsigned int i=0; i<mNumValues; ++i) {
      array[mStartIndex + i*mArrayStride] =
        mValuesPointer[i*mValuesStride].c_str();
    }
  }

  template<typename U>
  void
  operator()(U * array) const
  {
    unsigned int size = mStartIndex + (mNumValues - 1) * mArrayStride + 1;
// TODO resize pointer
    if(mArray->getSize() < size) {
      mArray->resize(size, std::string(""));
      mDimensions.clear();
    }
    for(unsigned int i=0; i<mNumValues; ++i) {
      array[mStartIndex + i*mArrayStride] =
        (U)atof(mValuesPointer[i*mValuesStride].c_str());
    }
  }

private:

  XdmfArray * const mArray;
  const unsigned int mStartIndex;
  const std::string * const mValuesPointer;
  const unsigned int mNumValues;
  const unsigned int mArrayStride;
  const unsigned int mValuesStride;
  std::vector<unsigned int> & mDimensions;
};
*/

template<>
void
XdmfArray::Insert<std::string>::operator()<void>(void * array) const;
/*
{
  unsigned int size = mStartIndex + (mNumValues - 1) * mArrayStride + 1;
// TODO resize pointer
  if(mArray->getSize() < size) {
    mArray->resize(size, std::string(""));
    mDimensions.clear();
  }
  for(unsigned int i=0; i<mNumValues; ++i) {
    ((char *)array)[mStartIndex + i*mArrayStride] =
      (char)atof(mValuesPointer[i*mValuesStride].c_str());
  }
}
*/

class XdmfArray::InsertArray : public XdmfArray::XdmfVisitor<void> {
public:

  InsertArray(XdmfArray * array,
              const unsigned int startIndex,
              const unsigned int valuesStartIndex,
              const unsigned int numValues,
              const unsigned int arrayStride,
              const unsigned int valuesStride,
              std::vector<unsigned int> & dimensions,
              const shared_ptr<const XdmfArray> & arrayToCopy) :
    mArray(array),
    mStartIndex(startIndex),
    mValuesStartIndex(valuesStartIndex),
    mNumValues(numValues),
    mArrayStride(arrayStride),
    mValuesStride(valuesStride),
    mDimensions(dimensions),
    mArrayToCopy(arrayToCopy)
  {
      mArray = array;
      mName = "InsertArray";
  }

  void
  operator()(void * array ) const
  {
    mArray->initialize(mArrayToCopy->getArrayType());
    XdmfArray::InsertArray visitor = *this;
    ApplyVisitor(mArray,
                 visitor, // *this,
                 mArray->mArray,
                 array);
  }

  template<typename T>
  void
  operator()(T * array) const
  {
    unsigned int size = mStartIndex + (mNumValues - 1) * mArrayStride + 1;
    if(mArray->getSize() < size) {
      mArray->resize(size, (T)0);
    }
    mArrayToCopy->getValues(mValuesStartIndex,
                            &(((T*)mArray->getValuesInternal())[mStartIndex]),
                            mNumValues,
                            mValuesStride,
                            mArrayStride);
  }

private:

  XdmfArray * mArray;
  const unsigned int mStartIndex;
  const unsigned int mValuesStartIndex;
  const unsigned int mNumValues;
  const unsigned int mArrayStride;
  const unsigned int mValuesStride;
  std::vector<unsigned int> & mDimensions;
  const shared_ptr<const XdmfArray> mArrayToCopy;
};

template <typename T>
class XdmfArray::PushBack : public XdmfArray::XdmfVisitor<void> {
public:

  PushBack(const T & val,
           XdmfArray * const array) :
    mVal(val),
    mArray(array)
  {
      mName = "PushBack";
  }

  void
  operator()(void * array) const
  {
    mArray->initialize<T>();
    XdmfArray::PushBack<T> visitor = *this;
    ApplyVisitor(mArray,
                 visitor,
                 mArray->mArray,
                 (T *)array);
  }

  void
  operator()(std::string * array) const
  {
    mArray->insert(mArray->getSize(), mVal);
/*
//resize if outside capacity
    std::stringstream value;
    value << mVal;
    unsigned int endpoint = mArray->getSize();// TODO
    mArray->resize<T>(endpoint + 1);
    array = (std::string *) mArray->mArray;
    array[endpoint + 1] = value.str();
    mArray->mDimensions.clear();
    mArray->mDimensions.push_back(endpoint + 1);
*/
  }

  template<typename U>
  void
  operator()(U * array) const
  {
printf("pushback to index %d\n", mArray->getSize());
    mArray->insert(mArray->getSize(), mVal);
/*
    unsigned int endpoint = mArray->getSize();// TODO
    mArray->resize<T>(endpoint + 1);
    array = (U *) mArray->mArray;
    array[endpoint]=(U)mVal;
    mArray->mDimensions.clear();
    mArray->mDimensions.push_back(endpoint + 1);
*/
  }

private:

  const T & mVal;
  XdmfArray * const mArray;
};

template <>
class XdmfArray::PushBack<void> : public XdmfArray::XdmfVisitor<void> {
public:

  PushBack(const void * val,
           XdmfArray * const array) :
    mVal(val),
    mArray(array)
  {
      mName = "PushBack";
  }

  void
  operator()(void * array) const
  {
    mArray->initialize<std::string>();
    XdmfArray::PushBack<void> visitor = *this;
    ApplyVisitor(mArray,
                 visitor,
                 mArray->mArray,
                 (std::string *)array);
  }

  void
  operator()(std::string * array) const
  {
//resize if outside capacity
    unsigned int endpoint = 0;// TODO
// TODO
//    array[endpoint+1] = mVal;
    mArray->mDimensions.clear();
  }

  template<typename U>
  void
  operator()(U * array) const
  {
//resize if outside capacity
    unsigned int endpoint = 0;// TODO
// TODO
//    array[endpoint+1]=(U)mVal;
    mArray->mDimensions.clear();
  }

private:

  const void * mVal;
  XdmfArray * const mArray;
};

template <>
class XdmfArray::PushBack<std::string> : public XdmfArray::XdmfVisitor<void> {
public:

  PushBack(const std::string & val,
           XdmfArray * const array) :
    mVal(val),
    mArray(array)
  {
      mName = "PushBack";
  }

  void
  operator()(void * array) const
  {
    mArray->initialize<std::string>();
    XdmfArray::PushBack<std::string> visitor = *this;
    ApplyVisitor(mArray,
                 visitor,
                 mArray->mArray,
                 (std::string *)array);
  }

  void
  operator()(std::string * array) const
  {
//resize if outside capacity
    unsigned int endpoint = 0;// TODO
    array[endpoint+1] = mVal;
    mArray->mDimensions.clear();
  }

  template<typename U>
  void
  operator()(U * array) const
  {
//resize if outside capacity
    unsigned int endpoint = 0;// TODO
    array[endpoint+1] = (U)atof(mVal.c_str());
    mArray->mDimensions.clear();
  }

private:

  const std::string & mVal;
  XdmfArray * const mArray;
};

// TODO Replace this with something else?
template <typename T>
class XdmfArray::Resize : public XdmfArray::XdmfVisitor<void> {
public:

  Resize(XdmfArray * const array,
         const unsigned int numValues,
         const T & val) :
    mArray(array),
    mNumValues(numValues),
    mVal(val)
  {
    mName = "Resize";
  }

  void
  operator()(void * array) const
  {
    mArray->initialize<T>();
    XdmfArray::Resize<T> visitor = *this;
    ApplyVisitor(mArray,
                 visitor,
                 mArray->mArray,
                 (T *) array);
  }

  void
  operator()(std::string * array) const
  {
    std::stringstream value;
    value << mVal;
    // need to determine how we want to do this TODO
    mArray->mDimensions.clear();
  }

  template<typename U>
  void
  operator()(U * array) const
  {
    unsigned int originalSize = mArray->getSize();
    if (mNumValues > mArray->mCapacity[0])
    {
      mArray->initialize<U>(mNumValues);
    }
    // overwrite newly created data with "val" // GETBACKHERE
    for (unsigned int i = originalSize; i < mArray->mCapacity[0]; ++i)
    {
      ((U*)(mArray->mArray))[i] = (U)mVal;
    }
    // Dimensions vector is no longer valid, so get rid of it.
    mArray->mDimensions.clear();
    mArray->mDimensions.push_back(mNumValues);
  }

private:

  XdmfArray * mArray;
  const unsigned int mNumValues;
  const T & mVal;
};

template <>
class XdmfArray::Resize<void> : public XdmfArray::XdmfVisitor<void> {
public:

  Resize(XdmfArray * const array,
         const unsigned int numValues,
         const void * & val) :
    mArray(array),
    mNumValues(numValues),
    mVal(val)
  {
    mName = "Resize";
  }

  void
  operator()() const
  {
    mArray->initialize<std::string>();
    std::string * array = NULL;
    XdmfArray::Resize<void> visitor = *this;
    ApplyVisitor(mArray,
                 visitor,
                 mArray->mArray,
                 array);
  }

  void
  operator()(std::string * & array) const
  {
// TODO String specific version
//    array->resize(mNumValues, mVal);
    mArray->mDimensions.clear();
  }

  template<typename U>
  void
  operator()(U * array) const
  {
// TODO String specific resize
//    array->resize(mNumValues, (U)atof(mVal.c_str()));
    mArray->mDimensions.clear();
  }

private:

  XdmfArray * mArray;
  const unsigned int mNumValues;
  const void * & mVal;
};

//TODO, string is a special case due to dynamic size.
template <>
class XdmfArray::Resize<std::string> : public XdmfArray::XdmfVisitor<void> {
public:

  Resize(XdmfArray * const array,
         const unsigned int numValues,
         const std::string & val) :
    mArray(array),
    mNumValues(numValues),
    mVal(val)
  {
    mName = "Resize";
  }

  void
  operator()() const
  {
    mArray->initialize<std::string>();
    std::string * array = NULL;
    XdmfArray::Resize<std::string> visitor = *this;
    ApplyVisitor(mArray,
                 visitor,
                 mArray->mArray,
                 array);
  }

  void
  operator()(std::string * & array) const
  {
// TODO String specific version 
//    array->resize(mNumValues, mVal);
    mArray->mDimensions.clear();
  }

  template<typename U>
  void
  operator()(U * array) const
  {
// TODO String specific resize
//    array->resize(mNumValues, (U)atof(mVal.c_str()));
    mArray->mDimensions.clear();
  }

private:

  XdmfArray * mArray;
  const unsigned int mNumValues;
  const std::string & mVal;
};

struct XdmfArray::NullDeleter
{
  void
  operator()(void const *) const
  {
  }
};

class XdmfArray::Reserve : public XdmfArray::XdmfVisitor<void> {
public:

  Reserve(XdmfArray * const array,
          const unsigned int size):
    mArray(array),
    mSize(size)
  {
      mName = "Reserve";
  }

  void
  operator()(const void * array) const
  {
    mArray->mTmpReserveSize = mSize;
  }

  template<typename T>
  void
  operator()(T * & array) const
  {
    mArray->initialize<T>(mSize);
  }

private:

  XdmfArray * const mArray;
  const unsigned int mSize;
};

template <typename T>
T
XdmfArray::getValue(const unsigned int index) const
{
  T * internalRef = NULL;
  XdmfArray::GetValue<T> visitor = GetValue<T>(index);
  return ApplyVisitor(this,
                      visitor,
                      mArray,
                      internalRef);
}

template <>
std::string
XdmfArray::getValue<std::string>(const unsigned int index) const;
/*
{
  XdmfArray::XdmfVisitor<std::string> visitor = GetValue<std::string>(index);
  return ApplyVisitor(this,
                      visitor,
                      mArray);
}
*/

template <typename T>
void
XdmfArray::getValues(const unsigned int startIndex,
                     T * const valuesPointer,
                     const unsigned int numValues,
                     const unsigned int arrayStride,
                     const unsigned int valuesStride) const
{
  XdmfArray::GetValues<T> visitor = GetValues<T>(this,
                                                 startIndex,
                                                 valuesPointer,
                                                 numValues,
                                                 arrayStride,
                                                 valuesStride);
  ApplyVisitor(this,
               visitor,
               mArray,
               valuesPointer);
}

class XdmfArray::Clear : public XdmfArray::XdmfVisitor<void> {
public:

  Clear(XdmfArray * const array) :
    mArray(array)
  {
      mName = "Clear";
  }

  template<typename T>
  void
  operator()(T * array) const
  {
    if (array != NULL)
    {
      // 0 out all values and clear dims
      memset(array, 0, mArray->getSize() * mArray->getArrayType()->getElementSize());
      mArray->mDimensions.clear();
//      array->clear();
    }
  }

private:
  XdmfArray * const mArray;
};


template <typename T>
shared_ptr<std::vector<T> >
XdmfArray::getValuesInternal()
{
  this->internalizeArrayPointer();
  try {
    std::vector<T> currVector((T *)mArray, (T *)mArray + this->getSize());
    shared_ptr<std::vector<T> > currArray = shared_ptr<std::vector<T> >(&(currVector));
    return currArray;
  }
  catch (...) {
    return shared_ptr<std::vector<T> >();
  }
}

template <>
void *
XdmfArray::initialize<void>(const unsigned int size);

template <typename T>
T *
XdmfArray::initialize(const unsigned int size)
{
  // Set type of variant to type of pointer
  if (mCapacity.size() == 0)
  {
    mCapacity.push_back(0);
  }
  if (size * sizeof(T) > mCapacity[0] * getArrayType()->getElementSize())
  {
    if (size >= mTmpReserveSize)
    {
      if (mArray)
      {
        void * newArray = calloc(size, sizeof(T));
        memcpy(newArray, mArray, mCapacity[0] * getArrayType()->getElementSize());
        std::swap(mArray, newArray);
        free(newArray);
      }
      else
      {
        void * newArray = calloc(size, sizeof(T));
        std::swap(mArray, newArray);
      }
      if (mArray == NULL)
      {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Allocation Failed.");        
      }
      mCapacity[0] = size;
    }
    else
    {
      if (mArray)
      {
        void * newArray = calloc(mTmpReserveSize, sizeof(T));
        memcpy(newArray, mArray, mCapacity[0] * getArrayType()->getElementSize());
        std::swap(mArray, newArray);
        free(newArray);
      }
      else
      {
        void * newArray = calloc(mTmpReserveSize, sizeof(T));
        std::swap(mArray, newArray);
      }
      mCapacity[0] = mTmpReserveSize;
      mTmpReserveSize = 0;
    }
  }
  this->setArrayType<T>();
  this->setIsChanged(true);
  return (T *)mArray;
}

template <typename T>
T *
XdmfArray::initialize(const std::vector<unsigned int> & dimensions)
{
  const unsigned int size = std::accumulate(dimensions.begin(),
                                            dimensions.end(),
                                            1,
                                            std::multiplies<unsigned int>());
  T * returnpointer = this->initialize<T>(size);
  mDimensions = dimensions;
  return returnpointer;
}

template<typename T>
void
XdmfArray::insert(const unsigned int index,
                  const T & value)
{
  if (!isInitialized())
  {
    initialize<T>(index+1);
  }
  XdmfArray::Insert<T> visitor = Insert<T>(this,
                                           index,
                                           &value,
                                           1,
                                           0,
                                           0,
                                           mDimensions);
  T tempValue = value;
  ApplyVisitor(this,
               visitor,
               mArray,
               &tempValue);
}

template <typename T>
void
XdmfArray::insert(const unsigned int startIndex,
                  const T * const valuesPointer,
                  const unsigned int numValues,
                  const unsigned int arrayStride,
                  const unsigned int valuesStride)
{
  if (!isInitialized())
  {
    initialize<T>(startIndex + (numValues * arrayStride));
  }
  XdmfArray::Insert<T> visitor = Insert<T>(this,
                                           startIndex,
                                           valuesPointer,
                                           numValues,
                                           arrayStride,
                                           valuesStride,
                                           mDimensions);
  T * tempPointer = NULL;
  ApplyVisitor(this,
               visitor,
               mArray,
               tempPointer);
  this->setIsChanged(true);
}

template <typename T>
void
XdmfArray::pushBack(const T & value)
{
  XdmfArray::PushBack<T> visitor = PushBack<T>(value, this);
  T tempValue = value;
  return ApplyVisitor(this,
                      visitor,
                      mArray,
                      &tempValue);
  this->setIsChanged(true);
}

template<typename T>
void
XdmfArray::resize(const unsigned int numValues,
                  const T & value)
{
/*
  // if numValues is greater than the current size
  if (numValues > this->getSize())
  {
    mArray = realloc(mArray, numValues * sizeof(T));
    // all newly assigned values are set to value
  }
*/
  XdmfArray::Resize<T> visitor = Resize<T>(this,
                                           numValues,
                                           value);
  T * valuePointer = NULL;
  ApplyVisitor(this,
               visitor,
               mArray,
               valuePointer);
  mDimensions.clear();
  mDimensions.push_back(numValues);
  this->setIsChanged(true);
}

template<typename T>
void
XdmfArray::resize(const std::vector<unsigned int> & dimensions,
                  const T & value)
{
  const unsigned int size = std::accumulate(dimensions.begin(),
                                            dimensions.end(),
                                            1,
                                            std::multiplies<unsigned int>());
  T valuecopy = value;
  this->resize(size, valuecopy);
  mDimensions = dimensions;
  this->setIsChanged(true);
}

template <typename T>
void
XdmfArray::setValuesInternal(T * const arrayPointer,
                             const unsigned int numValues,
                             const bool transferOwnership)
{
  // Remove contents of internal array.
// Free old internal array if required
  if(transferOwnership) {
// In this case Xdmf will free the pointer when done with it
    mArray = arrayPointer;
  }
  else {
// The user has to free the pointer
    mArray = arrayPointer;
  }
  mArrayPointerNumValues = numValues;
  mDimensions.clear();
  mDimensions.push_back(numValues);
  this->setArrayType<T>();
  this->setIsChanged(true);
}

template <typename T>
void
XdmfArray::setValuesInternal(std::vector<T> & array,
                             const bool transferOwnership)
{
  if(transferOwnership) {
// Extract the vector's internal pointer, but leave control to original source
    mArray = *(array[0]);
  }
  else {
// Completely extract the internal pointer
    mArray = *(array[0]);
  }
  mDimensions.clear();
  mDimensions.push_back(array.size());
  this->setArrayType<T>();
  this->setIsChanged(true);
}

template <typename T>
void
XdmfArray::setValuesInternal(const shared_ptr<std::vector<T> > array)
{
// TODO set sizes and capacity
  std::vector<T> tempvector = *(array.get());
  void * newArray = &((*(array.get()))[0]);
  std::swap(this->mArray, newArray);
  mDimensions.clear();
  mDimensions.push_back(array->size());
  this->setArrayType<T>();
  this->setIsChanged(true);
}

template <typename T>
bool
XdmfArray::swap(std::vector<T> & array)
{
  // TODO this needs to be reworked.
  this->internalizeArrayPointer();
  if(!this->isInitialized()) {
    this->initialize<T>();
  }
/*
  try {
    shared_ptr<std::vector<T> > currArray =
      boost::get<shared_ptr<std::vector<T> > >(mArray);
    currArray->swap(array);
    return true;
  }
  catch(const boost::bad_get & exception) {
    return false;
  }
*/
  this->setIsChanged(true);


return false;
}

template <typename T>
bool
XdmfArray::swap(const shared_ptr<std::vector<T> > array)
{
  return this->swap(*array.get());
}

template<typename T, typename U, typename V>
T
XdmfArray::ApplyVisitorFunction(XdmfArray::XdmfVisitor<T> & visitor, U * array, V* internal)
{
  if (strcmp(visitor.mName.c_str(), "Insert") == 0)
  {
    (*((Insert<V> *)(&visitor)))((U *)array);
    return T();
  }
  else if (strcmp(visitor.mName.c_str(), "GetValues") == 0)
  {
    (*((GetValues<V> *)(&visitor)))((U *)array);
    return T();
  }
  else if (strcmp(visitor.mName.c_str(), "Resize") == 0)
  {
    (*((Resize<V> *)(&visitor)))((U *)array);
    return T();
  }
  else if (strcmp(visitor.mName.c_str(), "InsertArray") == 0)
  {
    (*((InsertArray *)(&visitor)))((U *)array);
    return T();
  }
  else if (strcmp(visitor.mName.c_str(), "PushBack") == 0)
  {
    (*((PushBack<V> *)(&visitor)))((U *)array);
    return T();
  }
  else if (strcmp(visitor.mName.c_str(), "GetValuesString")== 0)
  {
    return (*((GetValuesString<T> *)(&visitor)))((U *)array);
  }
  else if (strcmp(visitor.mName.c_str(), "GetValue")== 0)
  {
    return (*((GetValue<T> *)(&visitor)))((U *)array);
  }
  else if (strcmp(visitor.mName.c_str(), "Reserve") == 0)
  {
    (*((Reserve *)(&visitor)))((U *)array);
    return T();
  }
  else if (strcmp(visitor.mName.c_str(), "Clear") == 0)
  {
    (*((Clear *)(&visitor)))((U *)array);
    return T();
  }
  else if (strcmp(visitor.mName.c_str(), "Erase") == 0)
  {
    (*((Erase *)(&visitor)))((U *)array);
    return T();
  }
  else
  {
    XdmfError::message(XdmfError::FATAL,
                       "Error: Invalid Xdmf Visitor.");
  }
}
