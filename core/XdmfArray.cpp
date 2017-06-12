/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfArray.cpp                                                       */
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

#include <limits>
#include <sstream>
#include <utility>
#include <stack>
#include <math.h>
#include <cstring>
#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfArrayReference.hpp"
#include "XdmfBinaryController.hpp"
#include "XdmfCoreReader.hpp"
#include "XdmfFunction.hpp"
#include "XdmfSubset.hpp"
#include "XdmfHeavyDataController.hpp"
#include "XdmfVisitor.hpp"
#include "XdmfError.hpp"

XDMF_CHILDREN_IMPLEMENTATION(XdmfArray,
                             XdmfHeavyDataController,
                             HeavyDataController,
                             Name)

template<>
void
XdmfArray::setArrayType<std::string>();

template<>
XDMFCORE_EXPORT
std::string
XdmfArray::GetValuesString<std::string>::getValuesString<void, void>(const void * const array,
                                                                     const int numValues) const
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

template <>
XDMFCORE_EXPORT
std::string
XdmfArray::getValue<std::string>(const unsigned int index) const
{
  std::string * internalRef = NULL;
  XdmfArray::GetValue<std::string> visitor = GetValue<std::string>(this, index);
  return ApplyVisitor(this,
                      visitor,
                      mArray,
                      internalRef);
}

template <>
XDMFCORE_EXPORT
void *
XdmfArray::initialize<void>(const unsigned int size)
{
  return this->initialize<char>(size);
}

template <>
XDMFCORE_EXPORT
std::string *
XdmfArray::initialize<std::string>(const unsigned int size)
{
  // Set type of variant to type of pointer
  if (mCapacity.size() == 0)
  {
    mCapacity.push_back(0);
  }
  if (size * mStringSize > mCapacity[0] * getArrayType()->getElementSize())
  {
    if (size >= mTmpReserveSize)
    {
      if (mArray)
      {
        void * newArray = calloc(size, mStringSize);
        memcpy(newArray, mArray, mCapacity[0] * getArrayType()->getElementSize());
        std::swap(mArray, newArray);
        free(newArray);
      }
      else
      {
        void * newArray = calloc(size, mStringSize);
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
        void * newArray = calloc(mTmpReserveSize, mStringSize);
        memcpy(newArray, mArray, mCapacity[0] * getArrayType()->getElementSize());
        std::swap(mArray, newArray);
        free(newArray);
      }
      else
      {
        void * newArray = calloc(mTmpReserveSize, mStringSize);
        std::swap(mArray, newArray);
      }
      mCapacity[0] = mTmpReserveSize;
      mTmpReserveSize = 0;
    }
  }
  this->setArrayType<std::string>();
  this->setIsChanged(true);
  return (std::string *)mArray;
}

//These void calls should never be called
template<>
XDMFCORE_EXPORT
void
XdmfArray::Insert<std::string>::operator()<void>(void * array) const
{
  unsigned int size = mStartIndex + (mNumValues - 1) * mArrayStride + 1;
  if(mArray->getSize() < size) {
    mArray->resize(size, std::string(""));
    mDimensions.clear();
  }
  for(unsigned int i=0; i<mNumValues; ++i) {
    ((char *)array)[mStartIndex + i*mArrayStride] =
      (char)atof(mValuesPointer[i*mValuesStride].c_str());
  }
}

template<>
XDMFCORE_EXPORT
void
XdmfArray::GetValues<std::string>::operator()<void>(const void * array) const
{
  if (array == NULL)
  {
    return;
  }
  else
  {
    for(unsigned int i=0; i<mNumValues; ++i) {
      std::stringstream value;
      value << ((char *)array)[mStartIndex + i*mArrayStride];
      mValuesPointer[i*mValuesStride] = value.str();
    }
  }
}

shared_ptr<XdmfArray>
XdmfArray::New()
{
  shared_ptr<XdmfArray> p(new XdmfArray());
  return p;
}

XdmfArray::XdmfArray() :
  mArrayPointerNumValues(0),
  mStringSize(256),
  mName(""),
  mTmpReserveSize(256),
  mReadMode(XdmfArray::Controller),
  mHasArrayOwnership(true),
  mArray(NULL)
{
  // Most intances use at most 3 dimensions
  mDimensions.reserve(3);
  mArrayType = XdmfArrayType::Uninitialized();
  mCapacity = std::vector<unsigned int>(1, 0);
}

XdmfArray::XdmfArray(XdmfArray & refArray):
  XdmfItem(refArray),
  mDimensions(refArray.getDimensions()),
  mStringSize(refArray.mStringSize),
  mName(refArray.getName()),
  mReadMode(refArray.getReadMode()),
  mHasArrayOwnership(true),
  mArray(NULL),
  mArrayType(refArray.mArrayType)
{
  if (refArray.getArrayType() != XdmfArrayType::Uninitialized()) {
    this->initialize(refArray.getArrayType(), 0);
    if (refArray.getSize() > 0) {
      shared_ptr<const XdmfArray> tempPointer = shared_ptr<const XdmfArray>(&refArray, XdmfNullDeleter());
      this->insert(0, tempPointer, 0, tempPointer->getSize());
    }
  }
  if (refArray.getNumberHeavyDataControllers() > 0) {
    for (unsigned int i = 0; i < refArray.getNumberHeavyDataControllers(); ++i) {
      this->insert(refArray.getHeavyDataController(i));	
    }
  }
  if (refArray.mReference) {
    this->setReference(refArray.getReference());
  }
}

XdmfArray::~XdmfArray()
{
  this->release();
}

const std::string XdmfArray::ItemTag = "DataItem";

void
XdmfArray::clear()
{
  XdmfArray::Clear visitor = Clear(this);
  this->ApplyVisitor(this,
                     visitor,
                     mArray,
                     mArray);
  mDimensions.clear();
  this->setIsChanged(true);
}

void
XdmfArray::erase(const unsigned int index)
{
  XdmfArray::Erase visitor = Erase(this, index);
  this->ApplyVisitor(this,
                     visitor,
                     mArray,
                     mArray);
  unsigned int newSize = this->getSize() - 1;
  mDimensions.clear();
  mDimensions.push_back(newSize);
  this->setIsChanged(true);
}

shared_ptr<const XdmfArrayType>
XdmfArray::getArrayType() const
{
  if (!this->isInitialized())
  {
    if (mHeavyDataControllers.size() > 0)
    {
      return mHeavyDataControllers[0]->getType();
    }
    else
    {
      return mArrayType;
    }
  }
  else
  {
    return mArrayType;
  }
}

unsigned int
XdmfArray::getCapacity() const
{
  return mCapacity[0];
}

std::vector<unsigned int>
XdmfArray::getDimensions() const
{
  if(mDimensions.size() == 0) {
    if(!this->isInitialized() && mHeavyDataControllers.size() > 0) {
      std::vector<unsigned int> returnDimensions;
      std::vector<unsigned int> tempDimensions;
      // Find the controller with the most dimensions
      int dimControllerIndex = 0;
      unsigned int dimSizeMax = 0;
      unsigned int dimTotal = 0;
      for (unsigned int i = 0; i < mHeavyDataControllers.size(); ++i) {
        dimTotal += mHeavyDataControllers[i]->getSize();
        if (mHeavyDataControllers[i]->getSize() > dimSizeMax) {
          dimSizeMax = mHeavyDataControllers[i]->getSize();
          dimControllerIndex = i;
        }
      }
      // Total up the size of the lower dimensions
      int controllerDimensionSubtotal = 1;
      for (unsigned int i = 0;
           i < mHeavyDataControllers[dimControllerIndex]->getDimensions().size() - 1;
           ++i) {
        returnDimensions.push_back(mHeavyDataControllers[dimControllerIndex]->getDimensions()[i]);
        controllerDimensionSubtotal *= mHeavyDataControllers[dimControllerIndex]->getDimensions()[i];
      }
      // Divide the total contained by the dimensions by the size of the lower dimensions
      returnDimensions.push_back(dimTotal/controllerDimensionSubtotal);
      return returnDimensions;
    }
    const unsigned int size = this->getSize();
    return std::vector<unsigned int>(1, size);
  }
  return std::vector<unsigned int>(mDimensions);
}

std::string
XdmfArray::getDimensionsString() const
{
  if (mDimensions.size() == 0)
  {
    if (mHeavyDataControllers.size() > 0)
    {
      std::vector<unsigned int> dimensions = this->getDimensions();
      return GetValuesString<std::string>(this,
                                          dimensions.size()).getValuesString(&dimensions[0],
                                                                             dimensions.size());
    }
    else
    {
      return "0";
    }
  }
  else
  {
    return GetValuesString<std::string>(this,
                                        mDimensions.size()).getValuesString(&mDimensions[0],
                                                                            mDimensions.size());
  }
}

std::map<std::string, std::string>
XdmfArray::getItemProperties() const
{
  std::map<std::string, std::string> arrayProperties;
  if(mHeavyDataControllers.size() > 0) {
    mHeavyDataControllers[0]->getProperties(arrayProperties);
  }
  else {
    arrayProperties.insert(std::make_pair("Format", "XML"));
  }
  arrayProperties.insert(std::make_pair("Dimensions", 
                                        this->getDimensionsString()));
  if(mName.compare("") != 0) {
    arrayProperties.insert(std::make_pair("Name", mName));
  }
  shared_ptr<const XdmfArrayType> type = this->getArrayType();
  type->getProperties(arrayProperties);
  return arrayProperties;
}

std::string
XdmfArray::getItemTag() const
{
  return ItemTag;
}

std::string
XdmfArray::getName() const
{
  return mName;
}

XdmfArray::ReadMode
XdmfArray::getReadMode() const
{
  return mReadMode;
}

unsigned int
XdmfArray::getSize() const
{
  // Accumulate total Size of Array
  unsigned int total = 0;
  if (!isInitialized())
  {
    for (unsigned int i = 0; i < mHeavyDataControllers.size(); ++i)
    {
      total += mHeavyDataControllers[i]->getSize();
    }
  }
  else
  {
    if (mDimensions.size() > 0)
    {
      total = 1;
    }
    for (unsigned int i = 0; i < mDimensions.size(); ++i)
    {
      total *= mDimensions[i];
    }
  }
  return total;
}

shared_ptr<XdmfArrayReference>
XdmfArray::getReference()
{
  if (mReference) {
    return mReference;
  }
  else {
    // Returning arbitrary Reference since one isn't defined
    return shared_ptr<XdmfArrayReference>();
  }
}

void *
XdmfArray::getValuesInternal()
{
  return const_cast<void *>
    (static_cast<const XdmfArray &>(*this).getValuesInternal());
}

const void *
XdmfArray::getValuesInternal() const
{
  return mArray;
}

std::string
XdmfArray::getValuesString() const
{
    if (isInitialized())
    {
      XdmfArray::GetValuesString<std::string> visitor = GetValuesString<std::string>(this, getSize());
      return ApplyVisitor<std::string>(this,
                                       visitor,
                                       mArray,
                                       mArray);
    }
    else
    {
      return "";
    }
}

shared_ptr<XdmfHeavyDataController>
XdmfArray::getHeavyDataController()
{
  return const_pointer_cast<XdmfHeavyDataController>
    (static_cast<const XdmfArray &>(*this).getHeavyDataController(0));
}

shared_ptr<const XdmfHeavyDataController>
XdmfArray::getHeavyDataController() const
{
  if (mHeavyDataControllers.size() > 0) {
    return mHeavyDataControllers[0];
  }
  else {
    return shared_ptr<XdmfHeavyDataController>();
  }
}

void
XdmfArray::initialize(const shared_ptr<const XdmfArrayType> & arrayType,
                      const unsigned int size)
{
  if(arrayType == XdmfArrayType::Int8()) {
    this->initialize<char>(size);
  }
  else if(arrayType == XdmfArrayType::Int16()) {
    this->initialize<short>(size);
  }
  else if(arrayType == XdmfArrayType::Int32()) {
    this->initialize<int>(size);
  }
  else if(arrayType == XdmfArrayType::Int64()) {
    this->initialize<long>(size);
  }
  else if(arrayType == XdmfArrayType::Float32()) {
    this->initialize<float>(size);
  }
  else if(arrayType == XdmfArrayType::Float64()) {
    this->initialize<double>(size);
  }
  else if(arrayType == XdmfArrayType::UInt8()) {
    this->initialize<unsigned char>(size);
  }
  else if(arrayType == XdmfArrayType::UInt16()) {
    this->initialize<unsigned short>(size);
  }
  else if(arrayType == XdmfArrayType::UInt32()) {
    this->initialize<unsigned int>(size);
  }
  else if(arrayType == XdmfArrayType::String()) {
    this->initialize<std::string>(size);
  }
  else if(arrayType == XdmfArrayType::Uninitialized()) {
    this->release();
  }
  else {
    XdmfError::message(XdmfError::FATAL, 
                       "Array of unsupported type in XdmfArray::initialize");
  }
  this->setIsChanged(true);
}

void
XdmfArray::initialize(const shared_ptr<const XdmfArrayType> & arrayType,
                      const std::vector<unsigned int> & dimensions)
{
  mDimensions = dimensions;
  const unsigned int size = std::accumulate(dimensions.begin(),
                                            dimensions.end(),
                                            1,
                                            std::multiplies<unsigned int>());
  return this->initialize(arrayType, size);
}

void
XdmfArray::insert(const unsigned int startIndex,
                  const shared_ptr<const XdmfArray> values,
                  const unsigned int valuesStartIndex,
                  const unsigned int numValues,
                  const unsigned int arrayStride,
                  const unsigned int valuesStride)
{
  XdmfArray::InsertArray visitor = InsertArray(this,
                                               startIndex,
                                               valuesStartIndex,
                                               numValues,
                                               arrayStride,
                                               valuesStride,
                                               mDimensions,
                                               values);
  this->ApplyVisitor(this,
                     visitor,
                     mArray,
                     mArray);
  this->setIsChanged(true);
}


void
XdmfArray::insert(const std::vector<unsigned int> startIndex,
                  const shared_ptr<const XdmfArray> values,
                  const std::vector<unsigned int> valuesStartIndex,
                  const std::vector<unsigned int> numValues,
                  const std::vector<unsigned int> numInserted,
                  const std::vector<unsigned int> arrayStride,
                  const std::vector<unsigned int> valuesStride)
{
  // Ensuring dimensions match up when pulling data
  if ((values->getDimensions().size() == valuesStartIndex.size()
      && valuesStartIndex.size() == numValues.size()
      && numValues.size() == valuesStride.size())
      && (numInserted.size() == startIndex.size()
      && startIndex.size() == this->getDimensions().size()
      && this->getDimensions().size() == arrayStride.size())) {
    // Pull data from values
    std::vector<unsigned int > dimTotalVector;
    unsigned int dimTotal = 1;
    for (unsigned int i = 0; i < values->getDimensions().size(); ++i) {
      dimTotalVector.push_back(dimTotal);
      dimTotal *= values->getDimensions()[i];
    }
    std::vector<unsigned int> indexVector;
    for (unsigned int i = 0; i < values->getDimensions().size(); ++i) {
      indexVector.push_back(0);
    }
    shared_ptr<XdmfArray> holderArray = XdmfArray::New();
    unsigned int holderoffset = 0;
    // End when the last index is incremented
    while (indexVector[indexVector.size()-1] < 1) {
      // Initialize the section of the array you're pulling from
      unsigned int startTotal = 0;
      dimTotal = 1;
      for (unsigned int i = 0; i < values->getDimensions().size(); ++i) {
        // Stride doesn't factor in to the first dimension
        // Since it's being used with the insert call
        if (i == 0) {
          startTotal += valuesStartIndex[i] * dimTotal;
        }
        else {
          startTotal += valuesStartIndex[i] * dimTotal
                        + valuesStride[i] * dimTotal * indexVector[i-1];
        }
        dimTotal *= values->getDimensions()[i];
      }
      // Insert the subsection
      holderArray->insert(holderoffset,
                          values,
                          startTotal,
                          numValues[0],
                          1,
                          valuesStride[0]);
      holderoffset+=numValues[0];
      // Increment up the vector
      bool increment = true;
      for (unsigned int i = 0; i < indexVector.size() && increment; ++i) {
        indexVector[i]++;
        // To keep the loop from breaking at the end
        if (i+1 < numValues.size()) {
          if (indexVector[i] >= numValues[i+1]) {
            indexVector[i] = indexVector[i] % numValues[i+1];
          }
          else {
            increment = false;
          }
        }
      }
    }
    // Values being inserted retrieved
    // Use an variation of the last loop to insert into this array

    indexVector.clear();
    for (unsigned int i = 0; i < this->getDimensions().size(); ++i) {
      indexVector.push_back(0);
    }
    holderoffset = 0;
    // End when the last index is incremented
    while (indexVector[indexVector.size()-1] < 1) {
      // Initialize the section of the array you're pulling from
      unsigned int startTotal = 0;
      dimTotal = 1;
      for (unsigned int i = 0; i < this->getDimensions().size(); ++i) {
        if (i == 0) {
          // Stride doesn't factor in to the first dimension
          // Since it's being used with the insert call
          startTotal += startIndex[i] * dimTotal;
        }
        else {
          startTotal += startIndex[i] * dimTotal + arrayStride[i] * dimTotal * indexVector[i-1];
        }
        dimTotal *= this->getDimensions()[i];
      }
      // Insert the subsection
      this->insert(startTotal, holderArray, holderoffset, numInserted[0], arrayStride[0], 1);
      holderoffset+=numInserted[0];
      // Increment up the vector
      bool increment = true;
      for (unsigned int i = 0; i < indexVector.size() && increment; ++i) {
        indexVector[i]++;
        if (i+1 < numInserted.size()) {
          // To keep the loop from breaking at the end
          if (indexVector[i] >= numInserted[i+1]) {
            indexVector[i] = indexVector[i] % numInserted[i+1];
          }
          else {
            increment = false;
          }
        }
      }
    }
    this->setIsChanged(true);
  }
  else {
    // Throw an error
    if (!(values->getDimensions().size() == valuesStartIndex.size()
          && valuesStartIndex.size() == numValues.size()
          && numValues.size() == valuesStride.size())) {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Number of starts, strides, and/or values "
                         "retrieved does not match up with the dimensions "
                         "of the array being retrieved from");
    }
    else if (!(numInserted.size() == startIndex.size()
               && startIndex.size() == this->getDimensions().size()
               && this->getDimensions().size() == arrayStride.size())) {
      XdmfError::message(XdmfError::FATAL,
                         "Error: Number of starts, strides, and/or values "
                         "written does not match up with the dimensions "
                         "of the array being inserted into");
    }
  }
}

bool
XdmfArray::isInitialized() const
{
  if (mArray != NULL)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void
XdmfArray::internalizeArrayPointer()
{
  // This call is defunct
}

void
XdmfArray::populateArray(const shared_ptr<XdmfArray> array)
{
  if (array->isInitialized())
  {
    this->insert(0, array, 0, array->getSize());
  }
  for (unsigned int i = 0; i < array->getNumberHeavyDataControllers(); ++i)
  {
    this->insert(array->getHeavyDataController(i));
  }
  if (array->getReference()) {
    this->setReference(array->getReference());
    this->setReadMode(XdmfArray::Reference);
  }
}

void
XdmfArray::populateItem(const std::map<std::string, std::string> & itemProperties,
                        const std::vector<shared_ptr<XdmfItem> > & childItems,
                        const XdmfCoreReader * const reader)
{
  // This inserts any XdmfInformation in childItems into the object.
  XdmfItem::populateItem(itemProperties, childItems, reader);

  bool filled = false;

  // Check for Function
  std::map<std::string, std::string>::const_iterator itemType =
    itemProperties.find("ItemType");

  if (itemType !=  itemProperties.end()) {
    if (itemType->second.compare("Function") == 0) {
      std::map<std::string, std::string>::const_iterator expressionLocation =
        itemProperties.find("Function");
      if (expressionLocation ==  itemProperties.end()) {
        XdmfError::message(XdmfError::FATAL,
                           "'Function' not found in itemProperties for Function"
                           " ItemType in XdmfArray::populateItem");
      }
      std::string expression = expressionLocation->second;

      // Convert from old format to new Variable format
      // $X -> ValX
      size_t loc = expression.find("$");

      while (loc != std::string::npos) {
        expression.replace(loc, 1, "Val");
        loc = expression.find("$", loc);
     }

      // Create Variable list

      std::map<std::string, shared_ptr<XdmfArray> > variableMap;

      unsigned int variableIndex = 0;
      for(std::vector<shared_ptr<XdmfItem> >::const_iterator iter =
            childItems.begin();
          iter != childItems.end();
          ++iter) {
        if(shared_ptr<XdmfArray> array = shared_dynamic_cast<XdmfArray>(*iter)) {
          std::stringstream variableKey;
          variableKey << "Val" << variableIndex;
          variableMap[variableKey.str()] = array;
          variableIndex++;
        }
      }

      shared_ptr<XdmfFunction> function = XdmfFunction::New(expression, variableMap);

      this->setReference(function);
      this->setReadMode(XdmfArray::Reference);
      filled = true;
    }
    else if (itemType->second.compare("HyperSlab") == 0) {

      shared_ptr<XdmfArray> dimArray;
      shared_ptr<XdmfArray> valArray;

      unsigned int foundArrayIndex = 0;

      for(std::vector<shared_ptr<XdmfItem> >::const_iterator iter =
            childItems.begin();
          iter != childItems.end();
          ++iter) {
        if(shared_ptr<XdmfArray> array = shared_dynamic_cast<XdmfArray>(*iter)) {
          if (foundArrayIndex == 0)
          {
            dimArray = array;
            foundArrayIndex++;
          }
          else if (foundArrayIndex == 1)
          {
            valArray = array;
            foundArrayIndex++;
          }
        }
      }

      if (!(dimArray))
      {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Hyperslab description missing");
      }
      if (!(valArray))
      {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Hyperslab values missing");
      }

      if (dimArray->getSize() % 3 != 0)
      {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Hyperslab description structured improperly");
      }

      // A start, stride, and dimension need to be
      // specified for each dimension
      unsigned int numDims = dimArray->getSize() / 3;

      // Start, stride, and dims are set via the first array provided
      std::vector<unsigned int> start;
      std::vector<unsigned int> stride;
      std::vector<unsigned int> dimensions;

      unsigned int i = 0;

      while (i < dimArray->getSize() / 3)
      {
        start.push_back(dimArray->getValue<unsigned int>(i));
        ++i;
      }

      while (i < 2 * (dimArray->getSize() / 3))
      {
        stride.push_back(dimArray->getValue<unsigned int>(i));
        ++i;
      }

      while (i < dimArray->getSize())
      {
        dimensions.push_back(dimArray->getValue<unsigned int>(i));
        ++i;
      }

      shared_ptr<XdmfSubset> subset =
        XdmfSubset::New(valArray,
                        start,
                        stride,
                        dimensions);
      this->setReference(subset);
      this->setReadMode(XdmfArray::Reference);
      filled = true;
    }
  }

  if (!filled) {
    std::vector<shared_ptr<XdmfHeavyDataController> > readControllers = reader->generateHeavyDataControllers(itemProperties);

    mHeavyDataControllers.clear();

    for (unsigned int i = 0; i < readControllers.size(); ++i) {
      mHeavyDataControllers.push_back(readControllers[i]);
    }

    const shared_ptr<const XdmfArrayType> arrayType = 
      XdmfArrayType::New(itemProperties);

    std::map<std::string, std::string>::const_iterator content =
      itemProperties.find("Content");
    if(content == itemProperties.end()) {
      XdmfError::message(XdmfError::FATAL,
                         "'Content' not found in itemProperties in "
                         "XdmfArray::populateItem");
    }

    unsigned int contentIndex;

    const std::string & contentVal = content->second;

    std::vector<std::string> contentVals;

    // Split the content based on "|" characters
    size_t barSplit = 0;
    std::string splitString(contentVal);
    std::string subcontent;
    while (barSplit != std::string::npos) {
      barSplit = 0;
      barSplit = splitString.find_first_of("|", barSplit);
      if (barSplit == std::string::npos) {
        subcontent = splitString;
      }
      else {
        subcontent = splitString.substr(0, barSplit);
        splitString = splitString.substr(barSplit+1);
        barSplit++;
      }
      contentVals.push_back(subcontent);
    }

    std::map<std::string, std::string>::const_iterator format =
      itemProperties.find("Format");
    if(format == itemProperties.end()) {
      XdmfError::message(XdmfError::FATAL, 
                         "'Format' not found in itemProperties in "
                         "XdmfArray::populateItem");
    }
    const std::string & formatVal = format->second;

    if (readControllers.size() == 0) {
      if(formatVal.compare("XML") == 0) {
        this->initialize(arrayType,
                         mDimensions);
        unsigned int index = 0;
        for (contentIndex = 0; contentIndex < contentVals.size(); ++contentIndex)
        {
#ifdef HAVE_CXX11_SHARED_PTR
          char * valuesString = strdup(contentVals[contentIndex].c_str());
          char * token = std::strtok(valuesString, " \t\n");
          if(arrayType == XdmfArrayType::String()) {
            while (token != NULL)
            {
              this->insert(index, std::string(token));
              token = std::strtok(NULL, " \t\n");
              ++index;
            }
          }
          else {
            while (token != NULL)
            {
              this->insert(index, atof(token));
              token = std::strtok(NULL, " \t\n");
              ++index;
            }
          }
#else
          boost::char_separator<char> sep(" \t\n");
          boost::tokenizer<boost::char_separator<char> > tokens(contentVals[contentIndex], sep);
          if(arrayType == XdmfArrayType::String()) {
            for(boost::tokenizer<boost::char_separator<char> >::const_iterator
                  iter = tokens.begin();
                iter != tokens.end();
                ++iter, ++index) {
              this->insert(index, *iter);
            }
          }
          else {
            for(boost::tokenizer<boost::char_separator<char> >::const_iterator
                  iter = tokens.begin();
                iter != tokens.end();
                ++iter, ++index) {
              this->insert(index, atof((*iter).c_str()));
            }
          }
#endif
        }
      }
      else
      {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Invalid Data Format "
                           "in XdmfArray::populateItem");
      }
    }

    std::map<std::string, std::string>::const_iterator dimensions =
      itemProperties.find("Dimensions");
    if(dimensions == itemProperties.end()) {
      XdmfError::message(XdmfError::FATAL,
                         "'Dimensions' not found in itemProperties in "
                         "XdmfArray::populateItem");
    }

    mDimensions.clear();
#ifdef HAVE_CXX11_SHARED_PTR
    char * dimsString = strdup(dimensions->second.c_str());
    char * token = std::strtok(dimsString, " ");
    while (token != NULL)
    {
      mDimensions.push_back(atoi(token));
      token = std::strtok(NULL, " ");
    }
#else
    boost::tokenizer<> tokens(dimensions->second);
    for(boost::tokenizer<>::const_iterator iter = tokens.begin();
        iter != tokens.end();
        ++iter) {
      mDimensions.push_back(atoi((*iter).c_str()));
    }
#endif
  }

  std::map<std::string, std::string>::const_iterator name =
    itemProperties.find("Name");
  if(name != itemProperties.end()) {
    mName = name->second;
  }
  else {
    mName = "";
  }
  this->setIsChanged(true);
}

void
XdmfArray::read()
{
  switch (mReadMode)
  {
    case XdmfArray::Controller:
      this->readController();
      break;
    case XdmfArray::Reference:
      this->readReference();
      break;
    default:
      XdmfError::message(XdmfError::FATAL,
                         "Error: Invalid Read Mode");
  }
}

void
XdmfArray::readController()
{
  if(mHeavyDataControllers.size() > 1) {
    this->release();
    for (unsigned int i = 0; i < mHeavyDataControllers.size(); ++i) {
      shared_ptr<XdmfArray> tempArray = XdmfArray::New();
      mHeavyDataControllers[i]->read(tempArray.get());
      unsigned int dimTotal = 1;
      for (unsigned int j = 0; j < mHeavyDataControllers[i]->getDimensions().size(); ++j) {
        dimTotal *= mHeavyDataControllers[i]->getDimensions()[j];
      }
      this->insert(mHeavyDataControllers[i]->getArrayOffset(), tempArray, 0, dimTotal, 1, 1);
    }
    std::vector<unsigned int> returnDimensions;
    std::vector<unsigned int> tempDimensions;
    // Find the controller with the most dimensions
    int dimControllerIndex = 0;
    unsigned int dimSizeMax = 0;
    unsigned int dimTotal = 0;
    for (unsigned int i = 0; i < mHeavyDataControllers.size(); ++i) {
        dimTotal += mHeavyDataControllers[i]->getSize();
        if (mHeavyDataControllers[i]->getSize() > dimSizeMax) {
          dimSizeMax = mHeavyDataControllers[i]->getSize();
          dimControllerIndex = i;
        }
    }
    // Total up the size of the lower dimensions
    int controllerDimensionSubtotal = 1;
    for (unsigned int i = 0;
         i < mHeavyDataControllers[dimControllerIndex]->getDimensions().size() - 1;
         ++i) {
      returnDimensions.push_back(mHeavyDataControllers[dimControllerIndex]->getDimensions()[i]);
      controllerDimensionSubtotal *= mHeavyDataControllers[dimControllerIndex]->getDimensions()[i];
    }
    // Divide the total contained by the dimensions by the size of the lower dimensions
    returnDimensions.push_back(dimTotal/controllerDimensionSubtotal);
    mDimensions = returnDimensions;
  }
  else if (mHeavyDataControllers.size() == 1 && mHeavyDataControllers[0]->getArrayOffset() == 0) {
    this->release();
    mHeavyDataControllers[0]->read(this);
    mDimensions = mHeavyDataControllers[0]->getDimensions();
  }
  else if (mHeavyDataControllers.size() == 1 && mHeavyDataControllers[0]->getArrayOffset() > 0) {
    this->release();
    shared_ptr<XdmfArray> tempArray = XdmfArray::New();
    mHeavyDataControllers[0]->read(tempArray.get());
    this->insert(mHeavyDataControllers[0]->getArrayOffset(), tempArray, 0, mHeavyDataControllers[0]->getSize(), 1, 1);
    std::vector<unsigned int> initDims;
    unsigned int initSize = 1;
    for (unsigned int i = 0; i < mHeavyDataControllers[0]->getDimensions().size(); ++i)
    {
      initSize *= mHeavyDataControllers[0]->getDimensions()[i];
    }
    initSize += mHeavyDataControllers[0]->getArrayOffset();
    initDims.push_back(initSize);
    mDimensions = initDims;
  }
  this->setIsChanged(true);
}

void
XdmfArray::readReference()
{
  shared_ptr<XdmfArray> tempArray = mReference->read();
  this->swap(tempArray);
  this->setIsChanged(true);
}

void
XdmfArray::release()
{
  if (mArray && mHasArrayOwnership)
  {
    free(mArray);
    mArray = NULL;
  }
  mArrayType = XdmfArrayType::Uninitialized();
  mArrayPointerNumValues = 0;
  mDimensions.clear();
  mCapacity.clear();
}

void
XdmfArray::reserve(const unsigned int size)
{
  if (!this->isInitialized())
  {
    // if not initialized, don't reserve until necessary
    mTmpReserveSize = size;
  }
  else
  {
    XdmfArray::Reserve visitor = Reserve(this, size);
    ApplyVisitor(this,
                 visitor,
                 mArray,
                 mArray);
  }
  this->setIsChanged(true);
}

template <typename T>
void
XdmfArray::setArrayType()
{
printf("invalid array type\n");
}

template <>
XDMFCORE_EXPORT
void
XdmfArray::setArrayType<unsigned char>()
{
  mArrayType = XdmfArrayType::UInt8();
}

template <>
XDMFCORE_EXPORT
void
XdmfArray::setArrayType<unsigned short>()
{
  mArrayType = XdmfArrayType::UInt16();
}

template <>
XDMFCORE_EXPORT
void
XdmfArray::setArrayType<unsigned int>()
{
  mArrayType = XdmfArrayType::UInt32();
}

template <>
XDMFCORE_EXPORT
void
XdmfArray::setArrayType<char>()
{
  mArrayType = XdmfArrayType::Int8();
}

template <>
XDMFCORE_EXPORT
void
XdmfArray::setArrayType<short>()
{
  mArrayType = XdmfArrayType::Int16();
}

template <>
XDMFCORE_EXPORT
void
XdmfArray::setArrayType<int>()
{
  this->mArrayType = XdmfArrayType::Int32();
}

template <>
XDMFCORE_EXPORT
void
XdmfArray::setArrayType<long>()
{
  mArrayType = XdmfArrayType::Int64();
}

template <>
XDMFCORE_EXPORT
void
XdmfArray::setArrayType<long long>()
{
  mArrayType = XdmfArrayType::Int64();
}

#ifdef WIN32
template <>
XDMFCORE_EXPORT
void
XdmfArray::setArrayType<__int64>()
{
  mArrayType = XdmfArrayType::Int64();
}
#endif

template <>
XDMFCORE_EXPORT
void
XdmfArray::setArrayType<float>()
{
  mArrayType = XdmfArrayType::Float32();
}

template <>
XDMFCORE_EXPORT
void
XdmfArray::setArrayType<double>()
{
  mArrayType = XdmfArrayType::Float64();
}

template <>
XDMFCORE_EXPORT
void
XdmfArray::setArrayType<std::string>()
{
  mArrayType = XdmfArrayType::String();
}

template <>
XDMFCORE_EXPORT
void
XdmfArray::setArrayType<std::string const* const>()
{
  mArrayType = XdmfArrayType::String();
}

template <>
XDMFCORE_EXPORT
void
XdmfArray::setArrayType<std::string const*>()
{
  mArrayType = XdmfArrayType::String();
}


void
XdmfArray::setHeavyDataController(shared_ptr<XdmfHeavyDataController> newController)
{
  // Since this is replacing the previous version which was designed to
  // completely replace the controller of the array
  // It will clear the current controllers before adding the new one in
  mHeavyDataControllers.clear();
  mHeavyDataControllers.push_back(newController);
  this->setIsChanged(true);
}

void
XdmfArray::setHeavyDataController(std::vector<shared_ptr<XdmfHeavyDataController> > & newControllers)
{
  if (mHeavyDataControllers.size() != newControllers.size()) {
    mHeavyDataControllers.resize(newControllers.size());
  }
  for (unsigned int i = 0; i < newControllers.size(); ++i) {
    mHeavyDataControllers[i] = newControllers[i];
  }
  this->setIsChanged(true);
}


void
XdmfArray::setName(const std::string & name)
{
  mName = name;
  this->setIsChanged(true);
}

void
XdmfArray::setReadMode(XdmfArray::ReadMode newStatus)
{
  mReadMode = newStatus;
  this->setIsChanged(true);
}

void
XdmfArray::setReference(shared_ptr<XdmfArrayReference> newReference)
{
  mReference = newReference;
  this->setIsChanged(true);
}

void
XdmfArray::swap(const shared_ptr<XdmfArray> array)
{
  std::swap(mArray, array->mArray);
  std::swap(mArrayPointerNumValues, array->mArrayPointerNumValues);
  std::swap(mDimensions, array->mDimensions);
  std::swap(mHeavyDataControllers, array->mHeavyDataControllers);
  std::swap(mArrayType, array->mArrayType);
  this->setIsChanged(true);
}

void
XdmfArray::traverse(const shared_ptr<XdmfBaseVisitor> visitor)
{
  XdmfItem::traverse(visitor);
  if (mReference) {
    mReference->accept(visitor);
  }
}

// C wrappers

XDMFARRAY *
XdmfArrayNew()
{
  try
  { 
    XDMFARRAY * returnPointer;
    shared_ptr<XdmfArray> generatedArray = XdmfArray::New();
    returnPointer = (XDMFARRAY *)((void *)(new XdmfArray(*generatedArray.get())));
    generatedArray.reset();
    return returnPointer;
  }
  catch (...)
  { 
    XDMFARRAY * returnPointer;
    shared_ptr<XdmfArray> generatedArray = XdmfArray::New();
    returnPointer = (XDMFARRAY *)((void *)(new XdmfArray(*generatedArray.get())));
    generatedArray.reset();
    return returnPointer;
  }
}

void XdmfArrayClear(XDMFARRAY * array)
{
  ((XdmfArray *)(array))->clear();
}

void XdmfArrayErase(XDMFARRAY * array, unsigned int index)
{
  ((XdmfArray *)(array))->erase(index);
}

int XdmfArrayGetArrayType(XDMFARRAY * array, int * status)
{
  XDMF_ERROR_WRAP_START(status)
  shared_ptr<const XdmfArrayType> compareType = ((XdmfArray *)(array))->getArrayType();
  std::string typeName = compareType->getName();
  unsigned int typePrecision = compareType->getElementSize();
  if (typeName == XdmfArrayType::UInt8()->getName())
  {
      return XDMF_ARRAY_TYPE_UINT8;
  }
  else if (typeName == XdmfArrayType::UInt16()->getName())
  {
      return XDMF_ARRAY_TYPE_UINT16;
  }
  else if (typeName == XdmfArrayType::UInt32()->getName())
  {
      return XDMF_ARRAY_TYPE_UINT32;
  }
  else if (typeName == XdmfArrayType::Int8()->getName())
  {
      return XDMF_ARRAY_TYPE_INT8;
  }
  else if (typeName == XdmfArrayType::Int16()->getName())
  {
      return XDMF_ARRAY_TYPE_INT16;
  }
  else if (typeName == XdmfArrayType::Int32()->getName() || typeName == XdmfArrayType::Int64()->getName())
  {
    if (typePrecision == 4)
    {
      return XDMF_ARRAY_TYPE_INT32;
    }
    else if (typePrecision == 8)
    {
      return XDMF_ARRAY_TYPE_INT64;
    }
    else
    {
    }
  }
  else if (typeName == XdmfArrayType::Float32()->getName() || typeName == XdmfArrayType::Float64()->getName())
  {
    if (typePrecision == 4)
    {
      return XDMF_ARRAY_TYPE_FLOAT32;
    }
    else if (typePrecision == 8)
    {
      return XDMF_ARRAY_TYPE_FLOAT64;
    }
    else
    {
    }
  }
  else if (typeName == XdmfArrayType::String()->getName())
  {
    //This shouldn't be used from C bindings
    XdmfError::message(XdmfError::FATAL,
                       "Error: String type not usable from C.");
  }
  else
  {
    XdmfError::message(XdmfError::FATAL,
                       "Error: Invalid ArrayType.");
  }
  XDMF_ERROR_WRAP_END(status)
  return -1;
}

unsigned int XdmfArrayGetCapacity(XDMFARRAY * array)
{
  return ((XdmfArray *)(array))->getCapacity();
}

unsigned int *
XdmfArrayGetDimensions(XDMFARRAY * array)
{
  try
  {
    std::vector<unsigned int> tempVector = ((XdmfArray *)(array))->getDimensions();
    unsigned int returnSize = tempVector.size();
    unsigned int * returnArray = new unsigned int[returnSize]();
    for (unsigned int i = 0; i < returnSize; ++i) {
      returnArray[i] = tempVector[i];
    }
    return returnArray;
  }
  catch (...)
  {
    std::vector<unsigned int> tempVector = ((XdmfArray *)(array))->getDimensions();
    unsigned int returnSize = tempVector.size();
    unsigned int * returnArray = new unsigned int[returnSize]();
    for (unsigned int i = 0; i < returnSize; ++i) {
      returnArray[i] = tempVector[i];
    }
    return returnArray;
  }
}

char *
XdmfArrayGetDimensionsString(XDMFARRAY * array)
{
  try
  {
    char * returnPointer = strdup(((XdmfArray *)(array))->getDimensionsString().c_str());
    return returnPointer;
  }
  catch (...)
  {
    char * returnPointer = strdup(((XdmfArray *)(array))->getDimensionsString().c_str());
    return returnPointer;
  }
}

XDMFHEAVYDATACONTROLLER *
XdmfArrayGetHeavyDataController(XDMFARRAY * array, unsigned int index)
{
  return (XDMFHEAVYDATACONTROLLER *)((void *)(((XdmfArray *)(array))->getHeavyDataController(index).get()));
}

char *
XdmfArrayGetName(XDMFARRAY * array)
{
  try
  {
    char * returnPointer = strdup(((XdmfArray *)(array))->getName().c_str());
    return returnPointer;
  }
  catch (...)
  {
    char * returnPointer = strdup(((XdmfArray *)(array))->getName().c_str());
    return returnPointer;
  }
}

unsigned int
XdmfArrayGetNumberDimensions(XDMFARRAY * array)
{
  return ((XdmfArray *)(array))->getDimensions().size();
}

unsigned int
XdmfArrayGetNumberHeavyDataControllers(XDMFARRAY * array)
{
  return ((XdmfArray *)(array))->getNumberHeavyDataControllers();
}

unsigned int
XdmfArrayGetSize(XDMFARRAY * array)
{
  return ((XdmfArray *)(array))->getSize();
}

int
XdmfArrayGetReadMode(XDMFARRAY * array, int * status)
{
  XDMF_ERROR_WRAP_START(status)
  int readMode = ((XdmfArray *)(array))->getReadMode();
  switch (readMode) {
    case XdmfArray::Controller:
      return XDMF_ARRAY_READ_MODE_CONTROLLER;
      break;
    case XdmfArray::Reference:
      return XDMF_ARRAY_READ_MODE_REFERENCE;
      break;
    default:
      XdmfError::message(XdmfError::FATAL,
                         "Error: Invalid ReadMode.");
      break;
  }
  XDMF_ERROR_WRAP_END(status)
  return -1;
}

XDMFARRAYREFERENCE *
XdmfArrayGetReference(XDMFARRAY * array)
{
  return (XDMFARRAYREFERENCE *)((void *)(((XdmfArray *)(array))->getReference().get()));
}

void *
XdmfArrayGetValue(XDMFARRAY * array, unsigned int index, int arrayType, int * status)
{
  XDMF_ERROR_WRAP_START(status)
  try
  {
    void * returnVal;
    switch (arrayType) {
      case XDMF_ARRAY_TYPE_UINT8:
        returnVal = new unsigned char();
        *((unsigned char *)returnVal) = ((XdmfArray *)(array))->getValue<unsigned char>(index);
        return returnVal;
        break;
      case XDMF_ARRAY_TYPE_UINT16:
        returnVal = new unsigned short();
        *((unsigned short *)returnVal) = ((XdmfArray *)(array))->getValue<unsigned short>(index);
        return returnVal;
        break;
      case XDMF_ARRAY_TYPE_UINT32:
        returnVal = new unsigned int();
        *((unsigned int *)returnVal) = ((XdmfArray *)(array))->getValue<unsigned int>(index);
        return returnVal;
        break;
      case XDMF_ARRAY_TYPE_INT8:
        returnVal = new char();
        *((char *)returnVal) = ((XdmfArray *)(array))->getValue<char>(index);
        return returnVal;
        break;
      case XDMF_ARRAY_TYPE_INT16:
        returnVal = new short();
        *((short *)returnVal) = ((XdmfArray *)(array))->getValue<short>(index);
        return returnVal;
        break;
      case XDMF_ARRAY_TYPE_INT32:
        returnVal = new int();
        *((int *)returnVal) = ((XdmfArray *)(array))->getValue<int>(index);
        return returnVal;
        break;
      case XDMF_ARRAY_TYPE_INT64:
        returnVal = new long();
        *((long *)returnVal) = ((XdmfArray *)(array))->getValue<long>(index);
        return returnVal;
        break;
      case XDMF_ARRAY_TYPE_FLOAT32:
        returnVal = new float();
        *((float *)returnVal) = ((XdmfArray *)(array))->getValue<float>(index);
        return returnVal;
        break;
      case XDMF_ARRAY_TYPE_FLOAT64:
        returnVal = new double();
        *((double *)returnVal) = ((XdmfArray *)(array))->getValue<double>(index);
        return returnVal;
        break;
      default:
        XdmfError::message(XdmfError::FATAL,
                           "Error: Invalid ArrayType.");
        break;
    }
  }
  catch (...)
  {
    void * returnVal;
    switch (arrayType) {
      case XDMF_ARRAY_TYPE_UINT8:
        returnVal = new unsigned char();
        *((unsigned char *)returnVal) = ((XdmfArray *)(array))->getValue<unsigned char>(index);
        return returnVal;
        break;
      case XDMF_ARRAY_TYPE_UINT16:
        returnVal = new unsigned short();
        *((unsigned short *)returnVal) = ((XdmfArray *)(array))->getValue<unsigned short>(index);
        return returnVal;
        break;
      case XDMF_ARRAY_TYPE_UINT32:
        returnVal = new unsigned int();
        *((unsigned int *)returnVal) = ((XdmfArray *)(array))->getValue<unsigned int>(index);
        return returnVal;
        break;
      case XDMF_ARRAY_TYPE_INT8:
        returnVal = new char();
        *((char *)returnVal) = ((XdmfArray *)(array))->getValue<char>(index);
        return returnVal;
        break;
      case XDMF_ARRAY_TYPE_INT16:
        returnVal = new short();
        *((short *)returnVal) = ((XdmfArray *)(array))->getValue<short>(index);
        return returnVal;
        break;
      case XDMF_ARRAY_TYPE_INT32:
        returnVal = new int();
        *((int *)returnVal) = ((XdmfArray *)(array))->getValue<int>(index);
        return returnVal;
        break;
      case XDMF_ARRAY_TYPE_INT64:
        returnVal = new long();
        *((long *)returnVal) = ((XdmfArray *)(array))->getValue<long>(index);
        return returnVal;
        break;
      case XDMF_ARRAY_TYPE_FLOAT32:
        returnVal = new float();
        *((float *)returnVal) = ((XdmfArray *)(array))->getValue<float>(index);
        return returnVal;
        break;
      case XDMF_ARRAY_TYPE_FLOAT64:
        returnVal = new double();
        *((double *)returnVal) = ((XdmfArray *)(array))->getValue<double>(index);
        return returnVal;
        break;
      default:
        XdmfError::message(XdmfError::FATAL,
                           "Error: Invalid ArrayType.");
        break;
    }
  }
  XDMF_ERROR_WRAP_END(status)
  return NULL;
}

void *
XdmfArrayGetValues(XDMFARRAY * array, unsigned int startIndex, int arrayType, unsigned int numValues, unsigned int arrayStride, unsigned int valueStride, int * status)
{
  XDMF_ERROR_WRAP_START(status)
  try
  {
    void * returnVal;
    switch (arrayType) {
      case XDMF_ARRAY_TYPE_UINT8:
        returnVal = new unsigned char[numValues]();
        ((XdmfArray *)(array))->getValues<unsigned char>(startIndex, (unsigned char *)returnVal, numValues, arrayStride, valueStride);
        return returnVal;
        break;
      case XDMF_ARRAY_TYPE_UINT16:
        returnVal = new unsigned short[numValues]();
        ((XdmfArray *)(array))->getValues<unsigned short>(startIndex, (unsigned short *)returnVal, numValues, arrayStride, valueStride);
        return returnVal;
        break;
      case XDMF_ARRAY_TYPE_UINT32:
        returnVal = new unsigned int[numValues]();
        ((XdmfArray *)(array))->getValues<unsigned int>(startIndex, (unsigned int *)returnVal, numValues, arrayStride, valueStride);
        return returnVal;
        break;
      case XDMF_ARRAY_TYPE_INT8:
        returnVal = new char[numValues]();
        ((XdmfArray *)(array))->getValues<char>(startIndex, (char *)returnVal, numValues, arrayStride, valueStride);
        return returnVal;
        break;
      case XDMF_ARRAY_TYPE_INT16:
        returnVal = new short[numValues]();
        ((XdmfArray *)(array))->getValues<short>(startIndex, (short *)returnVal, numValues, arrayStride, valueStride);
        return returnVal;
        break;
      case XDMF_ARRAY_TYPE_INT32:
        returnVal = new int[numValues]();
        ((XdmfArray *)(array))->getValues<int>(startIndex, (int *)returnVal, numValues, arrayStride, valueStride);
        return returnVal;
        break;
      case XDMF_ARRAY_TYPE_INT64:
        returnVal = new long[numValues]();
        ((XdmfArray *)(array))->getValues<long>(startIndex, (long *)returnVal, numValues, arrayStride, valueStride);
        return returnVal;
        break;
      case XDMF_ARRAY_TYPE_FLOAT32:
        returnVal = new float[numValues]();
        ((XdmfArray *)(array))->getValues<float>(startIndex, (float *)returnVal, numValues, arrayStride, valueStride);
        return returnVal;
        break;
      case XDMF_ARRAY_TYPE_FLOAT64:
        returnVal = new double[numValues]();
        ((XdmfArray *)(array))->getValues<double>(startIndex, (double *)returnVal, numValues, arrayStride, valueStride);
        return returnVal;
        break;
      default:
        XdmfError::message(XdmfError::FATAL,
                           "Error: Invalid ArrayType.");
        break;
    }
  }
  catch (...)
  {
    void * returnVal;
    switch (arrayType) {
      case XDMF_ARRAY_TYPE_UINT8:
        returnVal = new unsigned char[numValues]();
        ((XdmfArray *)(array))->getValues<unsigned char>(startIndex, (unsigned char *)returnVal, numValues, arrayStride, valueStride);
        return returnVal;
        break;
      case XDMF_ARRAY_TYPE_UINT16:
        returnVal = new unsigned short[numValues]();
        ((XdmfArray *)(array))->getValues<unsigned short>(startIndex, (unsigned short *)returnVal, numValues, arrayStride, valueStride);
        return returnVal;
        break;
      case XDMF_ARRAY_TYPE_UINT32:
        returnVal = new unsigned int[numValues]();
        ((XdmfArray *)(array))->getValues<unsigned int>(startIndex, (unsigned int *)returnVal, numValues, arrayStride, valueStride);
        return returnVal;
        break;
      case XDMF_ARRAY_TYPE_INT8:
        returnVal = new char[numValues]();
        ((XdmfArray *)(array))->getValues<char>(startIndex, (char *)returnVal, numValues, arrayStride, valueStride);
        return returnVal;
        break;
      case XDMF_ARRAY_TYPE_INT16:
        returnVal = new short[numValues]();
        ((XdmfArray *)(array))->getValues<short>(startIndex, (short *)returnVal, numValues, arrayStride, valueStride);
        return returnVal;
        break;
      case XDMF_ARRAY_TYPE_INT32:
        returnVal = new int[numValues]();
        ((XdmfArray *)(array))->getValues<int>(startIndex, (int *)returnVal, numValues, arrayStride, valueStride);
        return returnVal;
        break;
      case XDMF_ARRAY_TYPE_INT64:
        returnVal = new long[numValues]();
        ((XdmfArray *)(array))->getValues<long>(startIndex, (long *)returnVal, numValues, arrayStride, valueStride);
        return returnVal;
        break;
      case XDMF_ARRAY_TYPE_FLOAT32:
        returnVal = new float[numValues]();
        ((XdmfArray *)(array))->getValues<float>(startIndex, (float *)returnVal, numValues, arrayStride, valueStride);
        return returnVal;
        break;
      case XDMF_ARRAY_TYPE_FLOAT64:
        returnVal = new double[numValues]();
        ((XdmfArray *)(array))->getValues<double>(startIndex, (double *)returnVal, numValues, arrayStride, valueStride);
        return returnVal;
        break;
      default:
        XdmfError::message(XdmfError::FATAL,
                           "Error: Invalid ArrayType.");
        break;
    }
  }
  XDMF_ERROR_WRAP_END(status)
  return NULL;
}

void *
XdmfArrayGetValuesInternal(XDMFARRAY * array)
{
  return ((XdmfArray *)(array))->getValuesInternal();
}

char *
XdmfArrayGetValuesString(XDMFARRAY * array)
{
  try
  {
    char * returnPointer = strdup(((XdmfArray *)(array))->getValuesString().c_str());
    return returnPointer;
  }
  catch (...)
  {
    char * returnPointer = strdup(((XdmfArray *)(array))->getValuesString().c_str());
    return returnPointer;
  }
}

void
XdmfArrayInitialize(XDMFARRAY * array, int * dims, int numDims, int arrayType, int * status)
{
  XDMF_ERROR_WRAP_START(status)
  std::vector<unsigned int> dimVector((int *)dims, (int *)dims + numDims);
  shared_ptr<const XdmfArrayType> tempPointer = XdmfArrayType::Uninitialized();
  switch (arrayType) {
    case XDMF_ARRAY_TYPE_UINT8:
      tempPointer = XdmfArrayType::UInt8();
      break;
    case XDMF_ARRAY_TYPE_UINT16:
      tempPointer = XdmfArrayType::UInt16();
      break;
    case XDMF_ARRAY_TYPE_UINT32:
      tempPointer = XdmfArrayType::UInt32();
      break;
    case XDMF_ARRAY_TYPE_INT8:
      tempPointer = XdmfArrayType::Int8();
      break;
    case XDMF_ARRAY_TYPE_INT16:
      tempPointer = XdmfArrayType::Int16();
      break;
    case XDMF_ARRAY_TYPE_INT32:
      tempPointer = XdmfArrayType::Int32();
      break;
    case XDMF_ARRAY_TYPE_INT64:
      tempPointer = XdmfArrayType::Int64();
      break;
    case XDMF_ARRAY_TYPE_FLOAT32:
      tempPointer = XdmfArrayType::Float32();
      break;
    case XDMF_ARRAY_TYPE_FLOAT64:
      tempPointer = XdmfArrayType::Float64();
      break;
    default:
      XdmfError::message(XdmfError::FATAL,
                         "Error: Invalid ArrayType.");
      break;
  }
  ((XdmfArray *)(array))->initialize(tempPointer, dimVector);
  XDMF_ERROR_WRAP_END(status)
}

void
XdmfArrayInsertDataFromPointer(XDMFARRAY * array, void * values, int arrayType, unsigned int startIndex, unsigned int numVals, unsigned int arrayStride, unsigned int valueStride, int * status)
{
  XDMF_ERROR_WRAP_START(status)
  try
  {
    switch (arrayType) {
      case XDMF_ARRAY_TYPE_UINT8:
        ((XdmfArray *)(array))->insert<unsigned char>(startIndex, (unsigned char *)values, numVals, arrayStride, valueStride);
        break;
      case XDMF_ARRAY_TYPE_UINT16:
        ((XdmfArray *)(array))->insert<unsigned short>(startIndex, (unsigned short *)values, numVals, arrayStride, valueStride);
        break;
      case XDMF_ARRAY_TYPE_UINT32:
        ((XdmfArray *)(array))->insert<unsigned int>(startIndex, (unsigned int *)values, numVals, arrayStride, valueStride);
        break;
      case XDMF_ARRAY_TYPE_INT8:
        ((XdmfArray *)(array))->insert<char>(startIndex, (char *)values, numVals, arrayStride, valueStride);
        break;
      case XDMF_ARRAY_TYPE_INT16:
        ((XdmfArray *)(array))->insert<short>(startIndex, (short *)values, numVals, arrayStride, valueStride);
        break;
      case XDMF_ARRAY_TYPE_INT32:
        ((XdmfArray *)(array))->insert<int>(startIndex, (int *)values, numVals, arrayStride, valueStride);
        break;
      case XDMF_ARRAY_TYPE_INT64:
        ((XdmfArray *)(array))->insert<long>(startIndex, (long *)values, numVals, arrayStride, valueStride);
        break;
      case XDMF_ARRAY_TYPE_FLOAT32:
        ((XdmfArray *)(array))->insert<float>(startIndex, (float *)values, numVals, arrayStride, valueStride);
        break;
      case XDMF_ARRAY_TYPE_FLOAT64:
        ((XdmfArray *)(array))->insert<double>(startIndex, (double *)values, numVals, arrayStride, valueStride);
        break;
      default:
        XdmfError::message(XdmfError::FATAL,
                           "Error: Invalid ArrayType.");
        break;
    }
  }
  catch (...)
  {
    switch (arrayType) {
      case XDMF_ARRAY_TYPE_UINT8:
        ((XdmfArray *)(array))->insert<unsigned char>(startIndex, (unsigned char *)values, numVals, arrayStride, valueStride);
        break;
      case XDMF_ARRAY_TYPE_UINT16:
        ((XdmfArray *)(array))->insert<unsigned short>(startIndex, (unsigned short *)values, numVals, arrayStride, valueStride);
        break;
      case XDMF_ARRAY_TYPE_UINT32:
        ((XdmfArray *)(array))->insert<unsigned int>(startIndex, (unsigned int *)values, numVals, arrayStride, valueStride);
        break;
      case XDMF_ARRAY_TYPE_INT8:
        ((XdmfArray *)(array))->insert<char>(startIndex, (char *)values, numVals, arrayStride, valueStride);
        break;
      case XDMF_ARRAY_TYPE_INT16:
        ((XdmfArray *)(array))->insert<short>(startIndex, (short *)values, numVals, arrayStride, valueStride);
        break;
      case XDMF_ARRAY_TYPE_INT32:
        ((XdmfArray *)(array))->insert<int>(startIndex, (int *)values, numVals, arrayStride, valueStride);
        break;
      case XDMF_ARRAY_TYPE_INT64:
        ((XdmfArray *)(array))->insert<long>(startIndex, (long *)values, numVals, arrayStride, valueStride);
        break;
      case XDMF_ARRAY_TYPE_FLOAT32:
        ((XdmfArray *)(array))->insert<float>(startIndex, (float *)values, numVals, arrayStride, valueStride);
        break;
      case XDMF_ARRAY_TYPE_FLOAT64:
        ((XdmfArray *)(array))->insert<double>(startIndex, (double *)values, numVals, arrayStride, valueStride);
        break;
      default:
        XdmfError::message(XdmfError::FATAL,
                           "Error: Invalid ArrayType.");
        break;
    }
  }
  XDMF_ERROR_WRAP_END(status)
}

void XdmfArrayInsertDataFromXdmfArray(XDMFARRAY * array, XDMFARRAY * valArray, int * arrayStarts, int * valueStarts, int * arrayCounts, int * valueCounts, int * arrayStrides, int * valueStrides, int * status)
{
  XDMF_ERROR_WRAP_START(status)
  try
  {
    shared_ptr<XdmfArray> tempPointer((XdmfArray *)valArray, XdmfNullDeleter());
    std::vector<unsigned int> arrayStartVector((int *)arrayStarts, (int *)arrayStarts + ((XdmfArray *)(array))->getDimensions().size());
    std::vector<unsigned int> valueStartVector((int *)valueStarts, (int *)valueStarts + tempPointer->getDimensions().size());
    std::vector<unsigned int> arrayCountVector((int *)arrayCounts, (int *)arrayCounts + ((XdmfArray *)(array))->getDimensions().size());
    std::vector<unsigned int> valueCountVector((int *)valueCounts, (int *)valueCounts + tempPointer->getDimensions().size());
    std::vector<unsigned int> arrayStrideVector((int *)arrayStrides, (int *)arrayStrides + ((XdmfArray *)(array))->getDimensions().size());
    std::vector<unsigned int> valueStrideVector((int *)valueStrides, (int *)valueStrides + tempPointer->getDimensions().size());
    ((XdmfArray *)(array))->insert(arrayStartVector, tempPointer, valueStartVector, arrayCountVector, valueCountVector, arrayStrideVector, valueStrideVector);
  }
  catch (...)
  {
    shared_ptr<XdmfArray> tempPointer((XdmfArray *)valArray, XdmfNullDeleter());
    std::vector<unsigned int> arrayStartVector((int *)arrayStarts, (int *)arrayStarts + ((XdmfArray *)(array))->getDimensions().size());
    std::vector<unsigned int> valueStartVector((int *)valueStarts, (int *)valueStarts + tempPointer->getDimensions().size());
    std::vector<unsigned int> arrayCountVector((int *)arrayCounts, (int *)arrayCounts + ((XdmfArray *)(array))->getDimensions().size());
    std::vector<unsigned int> valueCountVector((int *)valueCounts, (int *)valueCounts + tempPointer->getDimensions().size());
    std::vector<unsigned int> arrayStrideVector((int *)arrayStrides, (int *)arrayStrides + ((XdmfArray *)(array))->getDimensions().size());
    std::vector<unsigned int> valueStrideVector((int *)valueStrides, (int *)valueStrides + tempPointer->getDimensions().size());
    ((XdmfArray *)(array))->insert(arrayStartVector, tempPointer, valueStartVector, arrayCountVector, valueCountVector, arrayStrideVector, valueStrideVector);
  }
  XDMF_ERROR_WRAP_END(status)
}

void
XdmfArrayInsertHeavyDataController(XDMFARRAY * array, XDMFHEAVYDATACONTROLLER * controller, int passControl)
{
  if (passControl == 0) {
    ((XdmfArray *)(array))->insert(shared_ptr<XdmfHeavyDataController>((XdmfHeavyDataController *) controller, XdmfNullDeleter()));
  }
  else {
    ((XdmfArray *)(array))->insert(shared_ptr<XdmfHeavyDataController>((XdmfHeavyDataController *) controller));
  }
}

void
XdmfArrayInsertValue(XDMFARRAY * array, unsigned int index, void * value, int arrayType, int * status)
{
  XDMF_ERROR_WRAP_START(status)
  try
  {
    switch (arrayType) {
      case XDMF_ARRAY_TYPE_UINT8:
        ((XdmfArray *)(array))->insert(index, *((unsigned char *)value));
        break;
      case XDMF_ARRAY_TYPE_UINT16:
        ((XdmfArray *)(array))->insert(index, *((unsigned short *)value));
        break;
      case XDMF_ARRAY_TYPE_UINT32:
        ((XdmfArray *)(array))->insert(index, *((unsigned int *)value));
        break;
      case XDMF_ARRAY_TYPE_INT8:
        ((XdmfArray *)(array))->insert(index, *((char *)value));
        break;
      case XDMF_ARRAY_TYPE_INT16:
        ((XdmfArray *)(array))->insert(index, *((short *)value));
        break;
      case XDMF_ARRAY_TYPE_INT32:
        ((XdmfArray *)(array))->insert(index, *((int *)value));
        break;
      case XDMF_ARRAY_TYPE_INT64:
        ((XdmfArray *)(array))->insert(index, *((long *)value));
        break;
      case XDMF_ARRAY_TYPE_FLOAT32:
        ((XdmfArray *)(array))->insert(index, *((float *)value));
        break;
      case XDMF_ARRAY_TYPE_FLOAT64:
        ((XdmfArray *)(array))->insert(index, *((double *)value));
        break;
      default:
        XdmfError::message(XdmfError::FATAL,
                           "Error: Invalid ArrayType.");
        break;
    }
  }
  catch (...)
  {
    switch (arrayType) {
      case XDMF_ARRAY_TYPE_UINT8:
        ((XdmfArray *)(array))->insert(index, *((unsigned char *)value));
        break;
      case XDMF_ARRAY_TYPE_UINT16:
        ((XdmfArray *)(array))->insert(index, *((unsigned short *)value));
        break;
      case XDMF_ARRAY_TYPE_UINT32:
        ((XdmfArray *)(array))->insert(index, *((unsigned int *)value));
        break;
      case XDMF_ARRAY_TYPE_INT8:
        ((XdmfArray *)(array))->insert(index, *((char *)value));
        break;
      case XDMF_ARRAY_TYPE_INT16:
        ((XdmfArray *)(array))->insert(index, *((short *)value));
        break;
      case XDMF_ARRAY_TYPE_INT32:
        ((XdmfArray *)(array))->insert(index, *((int *)value));
        break;
      case XDMF_ARRAY_TYPE_INT64:
        ((XdmfArray *)(array))->insert(index, *((long *)value));
        break;
      case XDMF_ARRAY_TYPE_FLOAT32:
        ((XdmfArray *)(array))->insert(index, *((float *)value));
        break;
      case XDMF_ARRAY_TYPE_FLOAT64:
        ((XdmfArray *)(array))->insert(index, *((double *)value));
        break;
      default:
        XdmfError::message(XdmfError::FATAL,
                           "Error: Invalid ArrayType.");
        break;
    }
  }
  XDMF_ERROR_WRAP_END(status)
}

int
XdmfArrayIsInitialized(XDMFARRAY * array)
{
  return ((XdmfArray *)(array))->isInitialized();
}

void
XdmfArrayPushBack(XDMFARRAY * array, void * value, int arrayType, int * status)
{
  XDMF_ERROR_WRAP_START(status)
  try
  {
    switch (arrayType) {
      case XDMF_ARRAY_TYPE_UINT8:
        ((XdmfArray *)(array))->pushBack<unsigned char>(*((unsigned char *)value));
        break;
      case XDMF_ARRAY_TYPE_UINT16:
        ((XdmfArray *)(array))->pushBack<unsigned short>(*((unsigned short *)value));
        break;
      case XDMF_ARRAY_TYPE_UINT32:
        ((XdmfArray *)(array))->pushBack<unsigned int>(*((unsigned int *)value));
        break;
      case XDMF_ARRAY_TYPE_INT8:
        ((XdmfArray *)(array))->pushBack<char>(*((char *)value));
        break;
      case XDMF_ARRAY_TYPE_INT16:
        ((XdmfArray *)(array))->pushBack<short>(*((short *)value));
        break;
      case XDMF_ARRAY_TYPE_INT32:
        ((XdmfArray *)(array))->pushBack<int>(*((int *)value));
        break;
      case XDMF_ARRAY_TYPE_INT64:
        ((XdmfArray *)(array))->pushBack<long>(*((long *)value));
        break;
      case XDMF_ARRAY_TYPE_FLOAT32:
        ((XdmfArray *)(array))->pushBack<float>(*((float *)value));
        break;
      case XDMF_ARRAY_TYPE_FLOAT64:
        ((XdmfArray *)(array))->pushBack<double>(*((double *)value));
        break;
      default:
        XdmfError::message(XdmfError::FATAL,
                           "Error: Invalid ArrayType.");
        break;
    }
  }
  catch (...)
  {
    switch (arrayType) {
      case XDMF_ARRAY_TYPE_UINT8:
        ((XdmfArray *)(array))->pushBack<unsigned char>(*((unsigned char *)value));
        break;
      case XDMF_ARRAY_TYPE_UINT16:
        ((XdmfArray *)(array))->pushBack<unsigned short>(*((unsigned short *)value));
        break;
      case XDMF_ARRAY_TYPE_UINT32:
        ((XdmfArray *)(array))->pushBack<unsigned int>(*((unsigned int *)value));
        break;
      case XDMF_ARRAY_TYPE_INT8:
        ((XdmfArray *)(array))->pushBack<char>(*((char *)value));
        break;
      case XDMF_ARRAY_TYPE_INT16:
        ((XdmfArray *)(array))->pushBack<short>(*((short *)value));
        break;
      case XDMF_ARRAY_TYPE_INT32:
        ((XdmfArray *)(array))->pushBack<int>(*((int *)value));
        break;
      case XDMF_ARRAY_TYPE_INT64:
        ((XdmfArray *)(array))->pushBack<long>(*((long *)value));
        break;
      case XDMF_ARRAY_TYPE_FLOAT32:
        ((XdmfArray *)(array))->pushBack<float>(*((float *)value));
        break;
      case XDMF_ARRAY_TYPE_FLOAT64:
        ((XdmfArray *)(array))->pushBack<double>(*((double *)value));
        break;
      default:
        XdmfError::message(XdmfError::FATAL,
                           "Error: Invalid ArrayType.");
        break;
    }
  }
  XDMF_ERROR_WRAP_END(status)
}

void
XdmfArrayRead(XDMFARRAY * array, int * status)
{
  XDMF_ERROR_WRAP_START(status)
  try
  {
    ((XdmfArray *)(array))->read();
  }
  catch (...)
  {
    ((XdmfArray *)(array))->read();
  }
  XDMF_ERROR_WRAP_END(status)
}

void
XdmfArrayReadController(XDMFARRAY * array, int * status)
{
  XDMF_ERROR_WRAP_START(status)
  try
  {
    ((XdmfArray *)(array))->readController();
  }
  catch (...)
  {
    ((XdmfArray *)(array))->readController();
  }
  XDMF_ERROR_WRAP_END(status)
}

void
XdmfArrayReadReference(XDMFARRAY * array, int * status)
{
  XDMF_ERROR_WRAP_START(status)
  try
  {
    ((XdmfArray *)(array))->readReference();
  }
  catch (...)
  {
    ((XdmfArray *)(array))->readReference();
  }
  XDMF_ERROR_WRAP_END(status)
}

void
XdmfArrayRelease(XDMFARRAY * array)
{
  ((XdmfArray *)(array))->release();
}

void
XdmfArrayRemoveHeavyDataController(XDMFARRAY * array, unsigned int index)
{
  ((XdmfArray *)(array))->removeHeavyDataController(index);
}

void
XdmfArrayReserve(XDMFARRAY * array, int size)
{
  ((XdmfArray *)(array))->reserve(size);
}

void
XdmfArrayResize(XDMFARRAY * array, int * dims, int numDims, int arrayType, int * status)
{
  XDMF_ERROR_WRAP_START(status)
  try
  {
    std::vector<unsigned int> dimVector((int *)dims, (int *)dims + numDims);
    switch (arrayType) {
      case XDMF_ARRAY_TYPE_UINT8:
      {
        XdmfArray * classedArray = (XdmfArray *)((void *) array);
        classedArray->resize(dimVector, (unsigned char) 0);
        break;
      }
      case XDMF_ARRAY_TYPE_UINT16:
      {
        XdmfArray * classedArray = (XdmfArray *)((void *) array);
        classedArray->resize(dimVector, (unsigned short) 0);
        break;
      }
      case XDMF_ARRAY_TYPE_UINT32:
      {
        XdmfArray * classedArray = (XdmfArray *)((void *) array);
        classedArray->resize(dimVector, (unsigned int) 0);
        break;
      }
      case XDMF_ARRAY_TYPE_INT8:
      {
        XdmfArray * classedArray = (XdmfArray *)((void *) array);
        classedArray->resize(dimVector, (char) 0);
        break;
      }
      case XDMF_ARRAY_TYPE_INT16:
      {
        XdmfArray * classedArray = (XdmfArray *)((void *) array);
        classedArray->resize(dimVector, (short) 0);
        break;
      }
      case XDMF_ARRAY_TYPE_INT32:
      {
        XdmfArray * classedArray = (XdmfArray *)((void *) array);
        classedArray->resize(dimVector, (int) 0);
        break;
      }
      case XDMF_ARRAY_TYPE_INT64:
      {
        XdmfArray * classedArray = (XdmfArray *)((void *) array);
        classedArray->resize(dimVector, (long) 0);
        break;
      }
      case XDMF_ARRAY_TYPE_FLOAT32:
      {
        XdmfArray * classedArray = (XdmfArray *)((void *) array);
        classedArray->resize(dimVector, (float) 0);
        break;
      }
      case XDMF_ARRAY_TYPE_FLOAT64:
      {
        XdmfArray * classedArray = (XdmfArray *)((void *) array);
        classedArray->resize(dimVector, (double) 0);
        break;
      }
      default:
      {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Invalid ArrayType.");
        break;
      }
    }
    dimVector.clear();
  }
  catch (...)
  {
    std::vector<unsigned int> dimVector((int *)dims, (int *)dims + numDims);
    switch (arrayType) {
      case XDMF_ARRAY_TYPE_UINT8:
      {
        XdmfArray * classedArray = (XdmfArray *)((void *) array);
        classedArray->resize(dimVector, (unsigned char) 0);
        break;
      }
      case XDMF_ARRAY_TYPE_UINT16:
      {
        XdmfArray * classedArray = (XdmfArray *)((void *) array);
        classedArray->resize(dimVector, (unsigned short) 0);
        break;
      }
      case XDMF_ARRAY_TYPE_UINT32:
      {
        XdmfArray * classedArray = (XdmfArray *)((void *) array);
        classedArray->resize(dimVector, (unsigned int) 0);
        break;
      }
      case XDMF_ARRAY_TYPE_INT8:
      {
        XdmfArray * classedArray = (XdmfArray *)((void *) array);
        classedArray->resize(dimVector, (char) 0);
        break;
      }
      case XDMF_ARRAY_TYPE_INT16:
      {
        XdmfArray * classedArray = (XdmfArray *)((void *) array);
        classedArray->resize(dimVector, (short) 0);
        break;
      }
      case XDMF_ARRAY_TYPE_INT32:
      {
        XdmfArray * classedArray = (XdmfArray *)((void *) array);
        classedArray->resize(dimVector, (int) 0);
        break;
      }
      case XDMF_ARRAY_TYPE_INT64:
      {
        XdmfArray * classedArray = (XdmfArray *)((void *) array);
        classedArray->resize(dimVector, (long) 0);
        break;
      }
      case XDMF_ARRAY_TYPE_FLOAT32:
      {
        XdmfArray * classedArray = (XdmfArray *)((void *) array);
        classedArray->resize(dimVector, (float) 0);
        break;
      }

      case XDMF_ARRAY_TYPE_FLOAT64:
      {
        XdmfArray * classedArray = (XdmfArray *)((void *) array);
        classedArray->resize(dimVector, (double) 0);
        break;
      }
      default:
      {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Invalid ArrayType.");
        break;
      }
    }
    dimVector.clear();
  }
  XDMF_ERROR_WRAP_END(status)
}

void
XdmfArraySetReadMode(XDMFARRAY * array, int readMode, int * status)
{
  XDMF_ERROR_WRAP_START(status)
  switch (readMode) {
    case XDMF_ARRAY_READ_MODE_CONTROLLER:
      ((XdmfArray *)(array))->setReadMode(XdmfArray::Controller);
      break;
    case XDMF_ARRAY_READ_MODE_REFERENCE:
      ((XdmfArray *)(array))->setReadMode(XdmfArray::Reference);
      break;
    default:
      XdmfError::message(XdmfError::FATAL,
                         "Error: Invalid ReadMode.");
      break;
  }
  XDMF_ERROR_WRAP_END(status)
}

void
XdmfArraySetReference(XDMFARRAY * array, XDMFARRAYREFERENCE * reference, int passControl)
{
  if (passControl) {
    ((XdmfArray *)(array))->setReference(shared_ptr<XdmfArrayReference>((XdmfArrayReference *)reference));
  }
  else {
    ((XdmfArray *)(array))->setReference(shared_ptr<XdmfArrayReference>((XdmfArrayReference *)reference, XdmfNullDeleter()));
  }
}

void
XdmfArraySetName(XDMFARRAY * array, char * name, int * status)
{
  XDMF_ERROR_WRAP_START(status)
  ((XdmfArray *)(array))->setName(name);
  XDMF_ERROR_WRAP_END(status)
}

void
XdmfArraySetValuesInternal(XDMFARRAY * array, void * pointer, unsigned int numValues, int arrayType, int transferOwnership, int * status)
{
  XDMF_ERROR_WRAP_START(status)
  switch (arrayType) {
    case XDMF_ARRAY_TYPE_UINT8:
      ((XdmfArray *)array)->setValuesInternal((unsigned char *)pointer, numValues, transferOwnership);
      break;
    case XDMF_ARRAY_TYPE_UINT16:
      ((XdmfArray *)array)->setValuesInternal((unsigned short *)pointer, numValues, transferOwnership);
      break;
    case XDMF_ARRAY_TYPE_UINT32:
      ((XdmfArray *)array)->setValuesInternal((unsigned int *)pointer, numValues, transferOwnership);
      break;
    case XDMF_ARRAY_TYPE_INT8:
      ((XdmfArray *)array)->setValuesInternal((char *)pointer, numValues, transferOwnership);
      break;
    case XDMF_ARRAY_TYPE_INT16:
      ((XdmfArray *)array)->setValuesInternal((short *)pointer, numValues, transferOwnership);
     break;
    case XDMF_ARRAY_TYPE_INT32:
      ((XdmfArray *)array)->setValuesInternal((int *)pointer, numValues, transferOwnership);
      break;
    case XDMF_ARRAY_TYPE_INT64:
      ((XdmfArray *)array)->setValuesInternal((long *)pointer, numValues, transferOwnership);
      break;
    case XDMF_ARRAY_TYPE_FLOAT32:
      ((XdmfArray *)array)->setValuesInternal((float *)pointer, numValues, transferOwnership);
      break;
    case XDMF_ARRAY_TYPE_FLOAT64:
      ((XdmfArray *)array)->setValuesInternal((double *)pointer, numValues, transferOwnership);
      break;
    default:
      XdmfError::message(XdmfError::FATAL,
                         "Error: Invalid ArrayType.");
      break;
  }
  XDMF_ERROR_WRAP_END(status)
}

void
XdmfArraySwapWithXdmfArray(XDMFARRAY * array, XDMFARRAY * swapArray)
{
  shared_ptr<XdmfArray> pointerToSwap((XdmfArray *) swapArray, XdmfNullDeleter());
  ((XdmfArray *)array)->swap(pointerToSwap);
}

// C Wrappers for parent classes are generated by macros

XDMF_ITEM_C_CHILD_WRAPPER(XdmfArray, XDMFARRAY)
