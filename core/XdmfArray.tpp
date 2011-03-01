/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
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
#include "XdmfArray.hpp"

template <typename T>
class XdmfArray::GetValues : public boost::static_visitor<void> {
public:

  GetValues(const unsigned int startIndex,
            T * valuesPointer,
            const unsigned int numValues,
            const unsigned int arrayStride,
            const unsigned int valuesStride) :
    mStartIndex(startIndex),
    mValuesPointer(valuesPointer),
    mNumValues(numValues),
    mArrayStride(arrayStride),
    mValuesStride(valuesStride)
  {
  }

  template<typename U>
  void
  operator()(const boost::shared_array<const U> & array) const
  {
    for(unsigned int i=0; i<mNumValues; ++i) {
      mValuesPointer[i*mValuesStride] = (T)array[mStartIndex + i*mArrayStride];
    }
  }

  template<typename U>
  void
  operator()(const boost::shared_ptr<std::vector<U> > & array) const
  {
    for(unsigned int i=0; i<mNumValues; ++i) {
      mValuesPointer[i*mValuesStride] =
        (T)array->operator[](mStartIndex + i*mArrayStride);
    }
  }

private:

  const unsigned int mStartIndex;
  T * mValuesPointer;
  const unsigned int mNumValues;
  const unsigned int mArrayStride;
  const unsigned int mValuesStride;
};

template <typename T>
class XdmfArray::Insert : public boost::static_visitor<void> {
public:

  Insert(const unsigned int startIndex,
         const T * const valuesPointer,
         const unsigned int numValues,
         const unsigned int arrayStride,
         const unsigned int valuesStride,
         std::vector<unsigned int> & dimensions) :
    mStartIndex(startIndex),
    mValuesPointer(valuesPointer),
    mNumValues(numValues),
    mArrayStride(arrayStride),
    mValuesStride(valuesStride),
    mDimensions(dimensions)
  {
  }

  template<typename U>
  void
  operator()(boost::shared_ptr<std::vector<U> > & array) const
  {
    unsigned int size = mStartIndex + mNumValues;
    if(mArrayStride > 1) {
      size = mStartIndex + mNumValues * mArrayStride - 1;
    }
    if(array->size() < size) {
      array->resize(size);
      mDimensions.clear();
    }
    for(unsigned int i=0; i<mNumValues; ++i) {
      array->operator[](mStartIndex + i*mArrayStride) =
        (U)mValuesPointer[i*mValuesStride];
    }
  }

private:

  const unsigned int mStartIndex;
  const T * const mValuesPointer;
  const unsigned int mNumValues;
  const unsigned int mArrayStride;
  const unsigned int mValuesStride;
  std::vector<unsigned int> & mDimensions;
};

template <typename T>
class XdmfArray::PushBack : public boost::static_visitor<void> {
public:

  PushBack(const T & val) :
    mVal(val)
  {
  }

  template<typename U>
  void
  operator()(boost::shared_ptr<std::vector<U> > & array) const
  {
    array->push_back((U)mVal);
  }

private:

  const T & mVal;
};

template <typename T>
class XdmfArray::Resize : public boost::static_visitor<void> {
public:

  Resize(const unsigned int numValues,
         const T & val) :
    mNumValues(numValues),
    mVal(val)
  {
  }

  template<typename U>
  void
  operator()(boost::shared_ptr<std::vector<U> > & array) const
  {
    array->resize(mNumValues, (U)mVal);
  }

private:

  const unsigned int mNumValues;
  const T & mVal;
};

struct XdmfArray::NullDeleter
{
  void
  operator()(void const *) const
  {
  }
};

template <typename T>
T
XdmfArray::getValue(const unsigned int index) const
{
  T toReturn;
  this->getValues(index, &toReturn, 1);
  return toReturn;
}

template <typename T>
void
XdmfArray::getValues(const unsigned int startIndex,
                     T * const valuesPointer,
                     const unsigned int numValues,
                     const unsigned int arrayStride,
                     const unsigned int valuesStride) const
{
  if(mHaveArray) {
    boost::apply_visitor(GetValues<T>(startIndex,
                                      valuesPointer,
                                      numValues,
                                      arrayStride,
                                      valuesStride),
                         mArray);
  }
  else if(mHaveArrayPointer) {
      boost::apply_visitor(GetValues<T>(startIndex,
                                        valuesPointer,
                                        numValues,
                                        arrayStride,
                                        valuesStride),
                           mArrayPointer);
  }
}

template <typename T>
boost::shared_ptr<std::vector<T> >
XdmfArray::getValuesInternal()
{
  if(mHaveArrayPointer) {
    internalizeArrayPointer();
  }
  try {
    boost::shared_ptr<std::vector<T> > currArray =
      boost::get<boost::shared_ptr<std::vector<T> > >(mArray);
    return currArray;
  }
  catch(const boost::bad_get & exception) {
    return boost::shared_ptr<std::vector<T> >();
  }
}

template <typename T>
boost::shared_ptr<std::vector<T> >
XdmfArray::initialize(const unsigned int size)
{
  if(mHaveArrayPointer) {
    releaseArrayPointer();
  }
  // Set type of variant to type of pointer
  boost::shared_ptr<std::vector<T> > newArray(new std::vector<T>(size));
  if(mTmpReserveSize > 0) {
    newArray->reserve(mTmpReserveSize);
    mTmpReserveSize = 0;
  }
  mArray = newArray;
  mHaveArray = true;
  return newArray;
}

template <typename T>
boost::shared_ptr<std::vector<T> >
XdmfArray::initialize(const std::vector<unsigned int> & dimensions)
{
  mDimensions = dimensions;
  const unsigned int size = std::accumulate(dimensions.begin(),
                                            dimensions.end(),
                                            1,
                                            std::multiplies<unsigned int>());
  return this->initialize<T>(size);
}

template<typename T>
void
XdmfArray::insert(const unsigned int index,
                  const T & value)
{
  if(mHaveArrayPointer) {
    internalizeArrayPointer();
  }
  if(!mHaveArray) {
    initialize<T>();
  }
  boost::apply_visitor(Insert<T>(index, &value, 1, 0, 0, mDimensions), mArray);
}

template <typename T>
void
XdmfArray::insert(const unsigned int startIndex,
                  const T * const valuesPointer,
                  const unsigned int numValues,
                  const unsigned int arrayStride,
                  const unsigned int valuesStride)
{
  if(mHaveArrayPointer) {
    internalizeArrayPointer();
  }
  if(!mHaveArray) {
    initialize<T>();
  }
  boost::apply_visitor(Insert<T>(startIndex,
                                 valuesPointer,
                                 numValues,
                                 arrayStride,
                                 valuesStride,
                                 mDimensions),
                       mArray);
}

template <typename T>
void
XdmfArray::pushBack(const T & value)
{
  if(mHaveArrayPointer) {
    internalizeArrayPointer();
  }
  if(!mHaveArray) {
    initialize<T>();
  }
  mDimensions.clear();
  return boost::apply_visitor(PushBack<T>(value), mArray);
}

template<typename T>
void
XdmfArray::resize(const unsigned int numValues,
                  const T & value)
{
  if(mHaveArrayPointer) {
    internalizeArrayPointer();
  }
  if(!mHaveArray) {
    initialize<T>();
  }
  mDimensions.clear();
  return boost::apply_visitor(Resize<T>(numValues, value), mArray);
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
  this->resize(size, value);
  mDimensions = dimensions;
}

template <typename T>
void
XdmfArray::setValuesInternal(const T * const arrayPointer,
                             const unsigned int numValues,
                             const bool transferOwnership)
{
  // Remove contents of internal array.
  if(mHaveArray) {
    releaseArray();
  }
  if(transferOwnership) {
    const boost::shared_array<const T> newArrayPointer(arrayPointer);
    mArrayPointer = newArrayPointer;
  }
  else {
    const boost::shared_array<const T> newArrayPointer(arrayPointer,
                                                       NullDeleter());
    mArrayPointer = newArrayPointer;
  }
  mHaveArrayPointer = true;
  mArrayPointerNumValues = numValues;
}

template <typename T>
void
XdmfArray::setValuesInternal(std::vector<T> & array,
                             const bool transferOwnership)
{
  if(mHaveArrayPointer) {
    releaseArrayPointer();
  }
  if(transferOwnership) {
    boost::shared_ptr<std::vector<T> > newArray(&array);
    mArray = newArray;
  }
  else {
    boost::shared_ptr<std::vector<T> > newArray(&array, NullDeleter());
    mArray = newArray;
  }
  mHaveArray = true;
}

template <typename T>
void
XdmfArray::setValuesInternal(const boost::shared_ptr<std::vector<T> > array)
{
  if(mHaveArrayPointer) {
    releaseArrayPointer();
  }
  mArray = array;
  mHaveArray = true;
}

template <typename T>
bool
XdmfArray::swap(std::vector<T> & array)
{
  if(mHaveArrayPointer) {
    internalizeArrayPointer();
  }
  if(!mHaveArray) {
    initialize<T>();
  }
  try {
    boost::shared_ptr<std::vector<T> > currArray =
      boost::get<boost::shared_ptr<std::vector<T> > >(mArray);
    currArray->swap(array);
    return true;
  }
  catch(const boost::bad_get& exception) {
    return false;
  }
}

template <typename T>
bool
XdmfArray::swap(const boost::shared_ptr<std::vector<T> > array)
{
  return this->swap(*array.get());
}
