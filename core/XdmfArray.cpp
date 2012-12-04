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

#include <boost/tokenizer.hpp>
#include <sstream>
#include <utility>
#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfHDF5Controller.hpp"
#include "XdmfHeavyDataController.hpp"
#include "XdmfVisitor.hpp"
#include "XdmfError.hpp"

class XdmfArray::Clear : public boost::static_visitor<void> {
public:

  Clear(XdmfArray * const array) :
    mArray(array)
  {
  }

  void
  operator()(const boost::blank & array) const
  {
    return;
  }

  template<typename T>
  void
  operator()(const shared_ptr<std::vector<T> > & array) const
  {
    array->clear();
  }

  template<typename T>
  void
  operator()(const boost::shared_array<const T> & array) const
  {
    mArray->internalizeArrayPointer();
    boost::apply_visitor(*this,
                         mArray->mArray);
  }

private:
  
  XdmfArray * const mArray;
};

class XdmfArray::Erase : public boost::static_visitor<void> {
public:

  Erase(XdmfArray * const array,
        const unsigned int index) :
    mArray(array),
    mIndex(index)
  {
  }

  void
  operator()(const boost::blank & array) const
  {
    return;
  }

  template<typename T>
  void
  operator()(const shared_ptr<std::vector<T> > & array) const
  {
    array->erase(array->begin() + mIndex);
  }

  template<typename T>
  void
  operator()(const boost::shared_array<const T> & array) const
  {
    mArray->internalizeArrayPointer();
    boost::apply_visitor(*this,
                         mArray->mArray);
  }

private:

  XdmfArray * const mArray;
  const unsigned int mIndex;
};

class XdmfArray::GetArrayType :
  public boost::static_visitor<shared_ptr<const XdmfArrayType> > {
public:

  GetArrayType(const shared_ptr<XdmfHeavyDataController> & heavyDataController) :
    mHeavyDataController(heavyDataController)
  {
  }

  shared_ptr<const XdmfArrayType>
  getArrayType(const char * const) const
  {
    return XdmfArrayType::Int8();
  }

  shared_ptr<const XdmfArrayType>
  getArrayType(const short * const) const
  {
    return XdmfArrayType::Int16();
  }

  shared_ptr<const XdmfArrayType>
  getArrayType(const int * const) const
  {
    return XdmfArrayType::Int32();
  }

  shared_ptr<const XdmfArrayType>
  getArrayType(const long * const) const
  {
    return XdmfArrayType::Int64();
  }

  shared_ptr<const XdmfArrayType>
  getArrayType(const float * const) const
  {
    return XdmfArrayType::Float32();
  }

  shared_ptr<const XdmfArrayType>
  getArrayType(const double * const) const
  {
    return XdmfArrayType::Float64();
  }

  shared_ptr<const XdmfArrayType>
  getArrayType(const unsigned char * const) const
  {
    return XdmfArrayType::UInt8();
  }

  shared_ptr<const XdmfArrayType>
  getArrayType(const unsigned short * const) const
  {
    return XdmfArrayType::UInt16();
  }

  shared_ptr<const XdmfArrayType>
  getArrayType(const unsigned int * const) const
  {
    return XdmfArrayType::UInt32();
  }

  shared_ptr<const XdmfArrayType>
  getArrayType(const std::string * const) const
  {
    return XdmfArrayType::String();
  }

  shared_ptr<const XdmfArrayType>
  operator()(const boost::blank & array) const
  {
    if(mHeavyDataController) {
      return mHeavyDataController->getType();
    }
    return XdmfArrayType::Uninitialized();
  }

  template<typename T>
  shared_ptr<const XdmfArrayType>
  operator()(const shared_ptr<std::vector<T> > & array) const
  {
    return this->getArrayType(&(array.get()->operator[](0)));
  }

  template<typename T>
  shared_ptr<const XdmfArrayType>
  operator()(const boost::shared_array<const T> & array) const
  {
    return this->getArrayType(array.get());
  }

private:

  const shared_ptr<XdmfHeavyDataController> mHeavyDataController;
};

class XdmfArray::GetCapacity : public boost::static_visitor<unsigned int> {
public:

  GetCapacity()
  {
  }

  unsigned int
  operator()(const boost::blank & array) const
  {
    return 0;
  }

  template<typename T>
  unsigned int
  operator()(const shared_ptr<std::vector<T> > & array) const
  {
    return array->capacity();
  }

  template<typename T>
  unsigned int
  operator()(const boost::shared_array<const T> & array) const
  {
    return 0;
  }
};

class XdmfArray::GetValuesPointer :
  public boost::static_visitor<const void *> {
public:

  GetValuesPointer()
  {
  }

  const void *
  operator()(const boost::blank & array) const
  {
    return NULL;
  }

  template<typename T>
  const void *
  operator()(const shared_ptr<std::vector<T> > & array) const
  {
    return &array->operator[](0);
  }

  template<typename T>
  const void *
  operator()(const boost::shared_array<const T> & array) const
  {
    return array.get();
  }
};

class XdmfArray::GetValuesString : public boost::static_visitor<std::string> {
public:

  GetValuesString(const int arrayPointerNumValues) :
    mArrayPointerNumValues(arrayPointerNumValues)
  {
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

    std::stringstream toReturn;
    for(int i=0; i<lastIndex; ++i) {
      toReturn << (U)array[i] << " ";
    }
    toReturn << (U)array[lastIndex];
    return toReturn.str();
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
  operator()(const boost::blank & array) const
  {
    return "";
  }

  template<typename T>
  std::string
  operator()(const shared_ptr<std::vector<T> > & array) const
  {
    return getValuesString(&(array->operator[](0)), array->size());
  }

  template<typename T>
  std::string
  operator()(const boost::shared_array<const T> & array) const
  {
    return getValuesString(array.get(), mArrayPointerNumValues);
  }

private:

  const unsigned int mArrayPointerNumValues;
};

class XdmfArray::InsertArray : public boost::static_visitor<void> {
public:

  InsertArray(XdmfArray * const array,
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
  }

  void
  operator()(const boost::blank & array) const
  {
    mArray->initialize(mArrayToCopy->getArrayType());
    boost::apply_visitor(*this,
                         mArray->mArray);
  }

  template<typename T>
  void
  operator()(const shared_ptr<std::vector<T> > & array) const
  {
    unsigned int size = mStartIndex + mNumValues;
    if(mArrayStride > 1) {
      size = mStartIndex + mNumValues * mArrayStride - 1;
    }
    if(array->size() < size) {
      array->resize(size);
      mDimensions.clear();
    }
    mArrayToCopy->getValues(mValuesStartIndex,
                            &(array->operator[](mStartIndex)),
                            mNumValues,
                            mValuesStride,
                            mArrayStride);
  }

  template<typename T>
  void
  operator()(const boost::shared_array<const T> & array) const
  {
    mArray->internalizeArrayPointer();
    boost::apply_visitor(*this,
                         mArray->mArray);
  }

private:

  XdmfArray * const mArray;
  const unsigned int mStartIndex;
  const unsigned int mValuesStartIndex;
  const unsigned int mNumValues;
  const unsigned int mArrayStride;
  const unsigned int mValuesStride;
  std::vector<unsigned int> & mDimensions;
  const shared_ptr<const XdmfArray> mArrayToCopy;
};

class XdmfArray::InternalizeArrayPointer : public boost::static_visitor<void> {
public:

  InternalizeArrayPointer(XdmfArray * const array) :
    mArray(array)
  {
  }

  void
  operator()(const boost::blank & array) const
  {
    return;
  }

  template<typename T>
  void
  operator()(const shared_ptr<std::vector<T> > & array) const
  {
    return;
  }

  template<typename T>
  void
  operator()(const boost::shared_array<const T> & array) const
  {
    const T * const pointer = array.get();
    shared_ptr<std::vector<T> > newArray(new std::vector<T>(pointer,
                                                            pointer + mArray->mArrayPointerNumValues));
    mArray->mArray = newArray;
    mArray->mArrayPointerNumValues = 0;
  }

private:

  XdmfArray * const mArray;
};

class XdmfArray::Reserve : public boost::static_visitor<void> {
public:

  Reserve(XdmfArray * const array,
          const unsigned int size):
    mArray(array),
    mSize(size)
  {
  }

  void
  operator()(const boost::blank & array) const
  {
    mArray->mTmpReserveSize = mSize;
  }

  template<typename T>
  void
  operator()(shared_ptr<std::vector<T> > & array) const
  {
    array->reserve(mSize);
  }

  template<typename T>
  void
  operator()(const boost::shared_array<const T> & array) const
  {
    mArray->internalizeArrayPointer();
    boost::apply_visitor(*this,
                         mArray->mArray);
  }

private:

  XdmfArray * const mArray;
  const unsigned int mSize;
};

class XdmfArray::Size : public boost::static_visitor<unsigned int> {
public:

  Size(const XdmfArray * const array) :
    mArray(array)
  {
  }

  unsigned int
  operator()(const boost::blank & array) const
  {
    if(mArray->mHeavyDataController) {
      return mArray->mHeavyDataController->getSize();
    }
    return 0;
  }

  template<typename T>
  unsigned int
  operator()(const shared_ptr<std::vector<T> > & array) const
  {
    return array->size();
  }

  template<typename T>
  unsigned int
  operator()(const boost::shared_array<const T> & array) const
  {
    return mArray->mArrayPointerNumValues;
  }

private:

  const XdmfArray * const mArray; 
};

shared_ptr<XdmfArray>
XdmfArray::New()
{
  shared_ptr<XdmfArray> p(new XdmfArray());
  return p;
}

XdmfArray::XdmfArray() :
  mArrayPointerNumValues(0),
  mHeavyDataController(shared_ptr<XdmfHeavyDataController>()),
  mName(""),
  mTmpReserveSize(0)
{
}

XdmfArray::~XdmfArray()
{
}

const std::string XdmfArray::ItemTag = "DataItem";

void
XdmfArray::clear()
{
  boost::apply_visitor(Clear(this), 
                       mArray);
  mDimensions.clear();
}

void
XdmfArray::erase(const unsigned int index)
{
  boost::apply_visitor(Erase(this,
                             index),
                       mArray);
  mDimensions.clear();
}

shared_ptr<const XdmfArrayType>
XdmfArray::getArrayType() const
{
  return boost::apply_visitor(GetArrayType(mHeavyDataController), 
                              mArray);
}

unsigned int
XdmfArray::getCapacity() const
{
  return boost::apply_visitor(GetCapacity(), 
                              mArray);
}

std::vector<unsigned int>
XdmfArray::getDimensions() const
{
  if(mDimensions.size() == 0) {
    if(!this->isInitialized() && mHeavyDataController) {
      return mHeavyDataController->getDimensions();
    }
    const unsigned int size = this->getSize();
    return std::vector<unsigned int>(1, size);
  }
  return mDimensions;
}

std::string
XdmfArray::getDimensionsString() const
{
  const std::vector<unsigned int> dimensions = this->getDimensions();
  return GetValuesString(dimensions.size()).getValuesString(&dimensions[0],
                                                            dimensions.size());
}

shared_ptr<XdmfHeavyDataController>
XdmfArray::getHeavyDataController()
{
  return boost::const_pointer_cast<XdmfHeavyDataController>
    (static_cast<const XdmfArray &>(*this).getHeavyDataController());
}

shared_ptr<const XdmfHeavyDataController>
XdmfArray::getHeavyDataController() const
{
  return mHeavyDataController;
}

std::map<std::string, std::string>
XdmfArray::getItemProperties() const
{
  std::map<std::string, std::string> arrayProperties;
  if(mHeavyDataController) {
    arrayProperties.insert(std::make_pair("Format",
                                          mHeavyDataController->getName()));
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

unsigned int
XdmfArray::getSize() const
{
  return boost::apply_visitor(Size(this), 
                              mArray);
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
  return boost::apply_visitor(GetValuesPointer(), 
                              mArray);
}

std::string
XdmfArray::getValuesString() const
{
  return boost::apply_visitor(GetValuesString(mArrayPointerNumValues), 
                              mArray);
}

void
XdmfArray::initialize(const shared_ptr<const XdmfArrayType> arrayType,
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
}

void
XdmfArray::initialize(const shared_ptr<const XdmfArrayType> arrayType,
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
  boost::apply_visitor(InsertArray(this,
                                   startIndex,
                                   valuesStartIndex,
                                   numValues,
                                   arrayStride,
                                   valuesStride,
                                   mDimensions,
                                   values),
                       mArray);
}

bool
XdmfArray::isInitialized() const
{
  try {
    boost::get<boost::blank>(mArray);
    return false;
  }
  catch(const boost::bad_get & exception) {
  }
  return true;
}

void
XdmfArray::internalizeArrayPointer()
{
  boost::apply_visitor(InternalizeArrayPointer(this), 
                       mArray);
}

void
XdmfArray::populateItem(const std::map<std::string, std::string> & itemProperties,
                        const std::vector<shared_ptr<XdmfItem> > & childItems,
                        const XdmfCoreReader * const reader)
{
  XdmfItem::populateItem(itemProperties, childItems, reader);

  const shared_ptr<const XdmfArrayType> arrayType = 
    XdmfArrayType::New(itemProperties);

  std::map<std::string, std::string>::const_iterator content =
    itemProperties.find("Content");
  if(content == itemProperties.end()) {
    XdmfError::message(XdmfError::FATAL, 
                       "'Content' not found in itemProperties in "
                       "XdmfArray::populateItem");
  }
  const std::string & contentVal = content->second;

  std::map<std::string, std::string>::const_iterator dimensions =
    itemProperties.find("Dimensions");
  if(dimensions == itemProperties.end()) {
    XdmfError::message(XdmfError::FATAL, 
                       "'Dimensions' not found in itemProperties in "
                       "XdmfArray::populateItem");
  }
   
  boost::tokenizer<> tokens(dimensions->second);
  for(boost::tokenizer<>::const_iterator iter = tokens.begin();
      iter != tokens.end();
      ++iter) {
    mDimensions.push_back(atoi((*iter).c_str()));
  }

  std::map<std::string, std::string>::const_iterator format =
    itemProperties.find("Format");
  if(format == itemProperties.end()) {
    XdmfError::message(XdmfError::FATAL, 
                       "'Format' not found in itemProperties in "
                       "XdmfArray::populateItem");
  }
  const std::string & formatVal = format->second;

  if(formatVal.compare("HDF") == 0) {
    size_t colonLocation = contentVal.find(":");
    if(colonLocation == std::string::npos) {
      XdmfError::message(XdmfError::FATAL, 
                         "':' not found in content in "
                         "XdmfArray::populateItem -- double check an HDF5 "
                         "data set is specified for the file");
    }
    
    std::string hdf5Path = contentVal.substr(0, colonLocation);
    const std::string dataSetPath =
      contentVal.substr(colonLocation + 1,
                        contentVal.size() - colonLocation - 1);

    // FIXME: for other OS (e.g. windows)
    if(hdf5Path.size() > 0 && hdf5Path[0] != '/') {
      // Dealing with a relative path for hdf5 location
      std::map<std::string, std::string>::const_iterator xmlDir =
        itemProperties.find("XMLDir");
      if(xmlDir == itemProperties.end()) {
        XdmfError::message(XdmfError::FATAL, 
                           "'XMLDir' not found in itemProperties in "
                           "XdmfArray::populateItem");
      }

      std::stringstream newHDF5Path;
      newHDF5Path << xmlDir->second << hdf5Path;
      hdf5Path = newHDF5Path.str();
    }
    mHeavyDataController =
      XdmfHDF5Controller::New(hdf5Path,
                              dataSetPath,
                              arrayType,
                              std::vector<unsigned int>(mDimensions.size(),
                                                        0),
                              std::vector<unsigned int>(mDimensions.size(),
                                                        1),
                              mDimensions,
                              mDimensions);
  }
  else if(formatVal.compare("XML") == 0) {
    this->initialize(arrayType,
                     mDimensions);
    unsigned int index = 0;
    boost::char_separator<char> sep(" \t\n");
    boost::tokenizer<boost::char_separator<char> > tokens(contentVal, sep);
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
  }
  else {
    XdmfError::message(XdmfError::FATAL, 
                       "Neither 'HDF' nor 'XML' specified as 'Format' "
                       "in XdmfArray::populateItem");
  }

  std::map<std::string, std::string>::const_iterator name =
    itemProperties.find("Name");
  if(name != itemProperties.end()) {
    mName = name->second;
  }
  else {
    mName = "";
  }
}

void
XdmfArray::read()
{
  if(mHeavyDataController) {
    mHeavyDataController->read(this);
  }
}

void
XdmfArray::release()
{
  mArray = boost::blank();
  mArrayPointerNumValues = 0;
  mDimensions.clear();
}

void
XdmfArray::reserve(const unsigned int size)
{
  boost::apply_visitor(Reserve(this,
                               size),
                       mArray);
}

void
XdmfArray::setHeavyDataController(const shared_ptr<XdmfHeavyDataController> heavyDataController)
{
  mHeavyDataController = heavyDataController;
}

void
XdmfArray::setName(const std::string & name)
{
  mName = name;
}

void
XdmfArray::swap(const shared_ptr<XdmfArray> array)
{
  std::swap(mArray, array->mArray);
  std::swap(mArrayPointerNumValues, array->mArrayPointerNumValues);
  std::swap(mDimensions, array->mDimensions);
  std::swap(mHeavyDataController, array->mHeavyDataController);
}
