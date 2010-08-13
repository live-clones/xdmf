/*
 * XdmfArray.cpp
 *
 *  Created on: Jan 29, 2010
 *      Author: kleiter
 */

#include <boost/tokenizer.hpp>
#include <sstream>
#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfHDF5Controller.hpp"
#include "XdmfSystemUtils.hpp"
#include "XdmfVisitor.hpp"

class XdmfArray::Clear : public boost::static_visitor<void> {
public:

	Clear()
	{
	}

	template<typename T>
	void operator()(const boost::shared_ptr<std::vector<T> > & array) const
	{
		return array->clear();
	}
};

class XdmfArray::CopyArrayValues : public boost::static_visitor<void> {
public:

	CopyArrayValues(const unsigned int startIndex, const unsigned int valuesStartIndex, const unsigned int numValues, const unsigned int arrayStride, const unsigned int valuesStride) :
		mStartIndex(startIndex),
		mValuesStartIndex(valuesStartIndex),
		mNumValues(numValues),
		mArrayStride(arrayStride),
		mValuesStride(valuesStride)
	{
	}

	template<typename T, typename U>
	void operator()(const boost::shared_ptr<std::vector<T> > & array, const boost::shared_ptr<std::vector<U> > & arrayToCopy) const
	{
		unsigned int size = mStartIndex + mNumValues;
		if(mArrayStride > 1)
		{
			size = mStartIndex + mNumValues * mArrayStride - 1;
		}
		if(array->size() < size)
		{
			array->resize(size);
		}
		for(int i=0; i<mNumValues; ++i)
		{
			array->operator[](mStartIndex + i*mArrayStride) = (T)arrayToCopy->operator[](mValuesStartIndex + i*mValuesStride);
		}
	}

private:

	const unsigned int mStartIndex;
	const unsigned int mValuesStartIndex;
	const unsigned int mNumValues;
	const unsigned int mArrayStride;
	const unsigned int mValuesStride;
};

class XdmfArray::Erase : public boost::static_visitor<void> {
public:

	Erase(const unsigned int index) :
		mIndex(index)
	{
	}

	template<typename T>
	void operator()(const boost::shared_ptr<std::vector<T> > & array) const
	{
		array->erase(array->begin() + mIndex);
	}

private:

	const unsigned int mIndex;
};

class XdmfArray::GetArrayType : public boost::static_visitor<boost::shared_ptr<const XdmfArrayType> > {
public:

	GetArrayType()
	{
	}

	boost::shared_ptr<const XdmfArrayType> getArrayType(const char * const) const
	{
		return XdmfArrayType::Int8();
	}

	boost::shared_ptr<const XdmfArrayType> getArrayType(const short * const) const
	{
		return XdmfArrayType::Int16();
	}

	boost::shared_ptr<const XdmfArrayType> getArrayType(const int * const) const
	{
		return XdmfArrayType::Int32();
	}

	boost::shared_ptr<const XdmfArrayType> getArrayType(const long * const) const
	{
		return XdmfArrayType::Int64();
	}

	boost::shared_ptr<const XdmfArrayType> getArrayType(const float * const) const
	{
		return XdmfArrayType::Float32();
	}

	boost::shared_ptr<const XdmfArrayType> getArrayType(const double * const) const
	{
		return XdmfArrayType::Float64();
	}

	boost::shared_ptr<const XdmfArrayType> getArrayType(const unsigned char * const) const
	{
		return XdmfArrayType::UInt8();
	}

	boost::shared_ptr<const XdmfArrayType> getArrayType(const unsigned short * const) const
	{
		return XdmfArrayType::UInt16();
	}

	boost::shared_ptr<const XdmfArrayType> getArrayType(const unsigned int * const) const
	{
		return XdmfArrayType::UInt32();
	}

	template<typename T>
	boost::shared_ptr<const XdmfArrayType> operator()(const boost::shared_ptr<std::vector<T> > & array) const
	{
		return this->getArrayType(&(array.get()->operator[](0)));
	}

	template<typename T>
	boost::shared_ptr<const XdmfArrayType> operator()(const boost::shared_array<const T> & array) const
	{
		return this->getArrayType(array.get());
	}
};

class XdmfArray::GetCapacity : public boost::static_visitor<unsigned int> {
public:

	GetCapacity()
	{
	}

	template<typename T>
	unsigned int operator()(const boost::shared_ptr<std::vector<T> > & array) const
	{
		return array->capacity();
	}
};

class XdmfArray::GetValuesPointer : public boost::static_visitor<const void * const> {
public:

	GetValuesPointer()
	{
	}

	template<typename T>
	const void * const operator()(const boost::shared_ptr<std::vector<T> > & array) const
	{
		return &array->operator[](0);
	}

	template<typename T>
	const void * const operator()(const boost::shared_array<const T> & array) const
	{
		return array.get();
	}
};

class XdmfArray::GetValuesString : public boost::static_visitor<std::string> {
public:

	GetValuesString() :
		mArrayPointerNumValues(0)
	{
	}

	GetValuesString(const int arrayPointerNumValues) :
		mArrayPointerNumValues(arrayPointerNumValues)
	{
	}

	std::string getValuesString(const char * const array, const int numValues) const
	{
		std::stringstream toReturn;
		for(int i=0; i<numValues; ++i)
		{
			toReturn << (int)array[i] << " ";
		}
		return toReturn.str();
	}

	std::string getValuesString(const unsigned char * const array, const int numValues) const
	{
		std::stringstream toReturn;
		for(int i=0; i<numValues; ++i)
		{
			toReturn << (int)array[i] << " ";
		}
		return toReturn.str();
	}

	template<typename T>
	std::string getValuesString(const T * const array, const int numValues) const
	{
		std::stringstream toReturn;
		for(int i=0; i<numValues; ++i)
		{
			toReturn << array[i] << " ";
		}
		return toReturn.str();
	}

	template<typename T>
	std::string operator()(const boost::shared_ptr<std::vector<T> > & array) const
	{
		return getValuesString(&(array->operator[](0)), array->size());
	}

	template<typename T>
	std::string operator()(const boost::shared_array<const T> & array) const
	{
		return getValuesString(array.get(), mArrayPointerNumValues);
	}

private:

	const unsigned int mArrayPointerNumValues;
};

class XdmfArray::InternalizeArrayPointer : public boost::static_visitor<void> {
public:

	InternalizeArrayPointer(XdmfArray * const array) :
		mArray(array)
	{
	}

	template<typename T>
	void operator()(const boost::shared_array<const T> & array) const
	{
		mArray->mHaveArrayPointer = false;
		mArray->copyValues(0, array.get(), mArray->mArrayPointerNumValues);
		mArray->mArrayPointer = boost::shared_array<const T>();
		mArray->mArrayPointerNumValues = 0;
	}

private:

	XdmfArray * const mArray;
};

class XdmfArray::NewArray : public boost::static_visitor<void> {
public:

	NewArray()
	{
	}

	template<typename T>
	void operator()(boost::shared_ptr<std::vector<T> > & array) const
	{
		boost::shared_ptr<std::vector<T> > newArray(new std::vector<T>());
		array = newArray;
	}
};

class XdmfArray::Reserve : public boost::static_visitor<void> {
public:

	Reserve(const unsigned int size):
		mSize(size)
	{
	}

	template<typename T>
	void operator()(boost::shared_ptr<std::vector<T> > & array) const
	{
		array->reserve(mSize);
	}

private:

	const unsigned int mSize;
};

class XdmfArray::Size : public boost::static_visitor<unsigned int> {
public:

	Size()
	{
	}

	template<typename T>
	unsigned int operator()(const boost::shared_ptr<std::vector<T> > & array) const
	{
		return array->size();
	}
};

boost::shared_ptr<XdmfArray> XdmfArray::New()
{
	boost::shared_ptr<XdmfArray> p(new XdmfArray());
	return p;
}

XdmfArray::XdmfArray() :
	mHaveArray(false),
	mHaveArrayPointer(false),
	mArrayPointerNumValues(0),
	mHDF5Controller(boost::shared_ptr<XdmfHDF5Controller>()),
	mName(""),
	mTmpReserveSize(0)
{
}

XdmfArray::~XdmfArray()
{
}

const std::string XdmfArray::ItemTag = "DataItem";

void XdmfArray::clear()
{
	if(mHaveArray)
	{
		return boost::apply_visitor(Clear(), mArray);
	}
}

void XdmfArray::copyValues(const unsigned int startIndex, const boost::shared_ptr<const XdmfArray> values, const unsigned int valuesStartIndex, const unsigned int numValues, const unsigned int arrayStride, const unsigned int valuesStride)
{
	if(mHaveArrayPointer)
	{
		internalizeArrayPointer();
	}
	if(!mHaveArray)
	{
		// Copy the values variant in order to get the type (only taking smart pointer so no worries about large copies)
		mArray = values->mArray;
		// Reinitialize variant array to contain new array with same type.
		boost::apply_visitor(NewArray(), mArray);
		mHaveArray = true;
	}
	boost::apply_visitor(CopyArrayValues(startIndex, valuesStartIndex, numValues, arrayStride, valuesStride), mArray, values->mArray);
}

void XdmfArray::erase(const unsigned int index)
{
	if(mHaveArray)
	{
		return boost::apply_visitor(Erase(index), mArray);
	}
}


boost::shared_ptr<const XdmfArrayType> XdmfArray::getArrayType() const
{
	if(mHaveArray)
	{
		return boost::apply_visitor(GetArrayType(), mArray);
	}
	else if(mHaveArrayPointer)
	{
		return boost::apply_visitor(GetArrayType(), mArrayPointer);
	}
	else if(mHDF5Controller)
	{
		return mHDF5Controller->getType();
	}
	return XdmfArrayType::Uninitialized();
}

unsigned int XdmfArray::getCapacity() const
{
	if(mHaveArray)
	{
		return boost::apply_visitor(GetCapacity(), mArray);
	}
	return 0;
}

boost::shared_ptr<XdmfHDF5Controller> XdmfArray::getHDF5Controller()
{
	return boost::const_pointer_cast<XdmfHDF5Controller>(static_cast<const XdmfArray &>(*this).getHDF5Controller());
}

boost::shared_ptr<const XdmfHDF5Controller> XdmfArray::getHDF5Controller() const
{
	return mHDF5Controller;
}

std::map<std::string, std::string> XdmfArray::getItemProperties() const
{
	std::map<std::string, std::string> arrayProperties;
	if(mHDF5Controller)
	{
		arrayProperties["Format"] = "HDF";
	}
	else
	{
		arrayProperties["Format"] = "XML";
	}
	std::stringstream size;
	size <<  this->size();
	arrayProperties["Dimensions"] = size.str();
	if(mName.compare("") != 0)
	{
		arrayProperties["Name"] = mName;
	}
	boost::shared_ptr<const XdmfArrayType> type = this->getArrayType();
	type->getProperties(arrayProperties);
	return arrayProperties;
}

std::string XdmfArray::getItemTag() const
{
	return ItemTag;
}

std::string XdmfArray::getName() const
{
	return mName;
}

void * XdmfArray::getValuesPointer()
{
	return const_cast<void *>(static_cast<const XdmfArray &>(*this).getValuesPointer());
}

const void * XdmfArray::getValuesPointer() const
{
	if(mHaveArray)
	{
		return boost::apply_visitor(GetValuesPointer(), mArray);
	}
	else if(mHaveArrayPointer)
	{
		return boost::apply_visitor(GetValuesPointer(), mArrayPointer);
	}
	return NULL;
}

std::string XdmfArray::getValuesString() const
{
	if(mHaveArray)
	{
		return boost::apply_visitor(GetValuesString(), mArray);
	}
	else if(mHaveArrayPointer)
	{
		return boost::apply_visitor(GetValuesString(mArrayPointerNumValues), mArrayPointer);
	}
	return "";
}

void XdmfArray::initialize(const boost::shared_ptr<const XdmfArrayType> arrayType, const unsigned int size)
{
	if(arrayType == XdmfArrayType::Int8())
	{
		this->initialize<char>(size);
	}
	else if(arrayType == XdmfArrayType::Int16())
	{
		this->initialize<short>(size);
	}
	else if(arrayType == XdmfArrayType::Int32())
	{
		this->initialize<int>(size);
	}
	else if(arrayType == XdmfArrayType::Int64())
	{
		this->initialize<long>(size);
	}
	else if(arrayType == XdmfArrayType::Float32())
	{
		this->initialize<float>(size);
	}
	else if(arrayType == XdmfArrayType::Float64())
	{
		this->initialize<double>(size);
	}
	else if(arrayType == XdmfArrayType::UInt8())
	{
		this->initialize<unsigned char>(size);
	}
	else if(arrayType == XdmfArrayType::UInt16())
	{
		this->initialize<unsigned short>(size);
	}
	else if(arrayType == XdmfArrayType::UInt32())
	{
		this->initialize<unsigned int>(size);
	}
	else if(arrayType == XdmfArrayType::Uninitialized())
	{
		this->release();
	}
	else
	{
		assert(false);
	}
}

bool XdmfArray::isInitialized() const
{
	return mHaveArray || mHaveArrayPointer;
}

void XdmfArray::internalizeArrayPointer()
{
	if(mHaveArrayPointer)
	{
		boost::apply_visitor(InternalizeArrayPointer(this), mArrayPointer);
	}
}

void XdmfArray::populateItem(const std::map<std::string, std::string> & itemProperties, std::vector<boost::shared_ptr<XdmfItem> > & childItems)
{
	std::string contentVal;
	unsigned int sizeVal = 1;

	const boost::shared_ptr<const XdmfArrayType> arrayType = XdmfArrayType::New(itemProperties);
	std::map<std::string, std::string>::const_iterator content = itemProperties.find("Content");
	if(content != itemProperties.end())
	{
		contentVal = content->second;
	}
	else
	{
		assert(false);
	}
	std::map<std::string, std::string>::const_iterator size = itemProperties.find("Dimensions");
	if(size != itemProperties.end())
	{
		boost::tokenizer<> tokens(size->second);
		for(boost::tokenizer<>::const_iterator iter = tokens.begin(); iter != tokens.end(); ++iter)
		{
			sizeVal *= atoi((*iter).c_str());
		}
	}
	else
	{
		assert(false);
	}
	std::map<std::string, std::string>::const_iterator format = itemProperties.find("Format");
	if(format != itemProperties.end())
	{
		if(format->second.compare("HDF") == 0)
		{
			std::map<std::string, std::string>::const_iterator xmlDir = itemProperties.find("XMLDir");
			if(xmlDir == itemProperties.end())
			{
				assert(false);
			}
			size_t colonLocation = contentVal.find(":");
			if(colonLocation != std::string::npos)
			{
				std::string hdf5Path = contentVal.substr(0, colonLocation);
				std::string dataSetPath = contentVal.substr(colonLocation + 1, contentVal.size() - colonLocation - 1);
				if(hdf5Path.compare(XdmfSystemUtils::getRealPath(hdf5Path)) != 0)
				{
					// Dealing with a relative path for hdf5 location
					std::stringstream newHDF5Path;
					newHDF5Path << xmlDir->second << hdf5Path;
					hdf5Path = newHDF5Path.str();
				}
				mHDF5Controller = XdmfHDF5Controller::New(hdf5Path, dataSetPath, sizeVal, arrayType);
			}
			else
			{
				assert(false);
			}
		}
		else if(format->second.compare("XML") == 0)
		{
			this->initialize(arrayType);
			this->reserve(sizeVal);
			boost::char_separator<char> sep(" \t\n");
			boost::tokenizer<boost::char_separator<char> > tokens(contentVal, sep);
			for(boost::tokenizer<boost::char_separator<char> >::const_iterator iter = tokens.begin(); iter != tokens.end(); ++iter)
			{
				this->pushBack(atof((*iter).c_str()));

			}
		}
		else
		{
			assert(false);
		}
	}
	else
	{
		assert(false);
	}
	std::map<std::string, std::string>::const_iterator name = itemProperties.find("Name");
	if(name != itemProperties.end())
	{
		mName = name->second;
	}
	else
	{
		mName = "";
	}
}

void XdmfArray::read()
{
	if(mHDF5Controller)
	{
		mHDF5Controller->read(this);
	}
}

void XdmfArray::release()
{
	releaseArray();
	releaseArrayPointer();
}

void XdmfArray::releaseArray()
{
	boost::shared_ptr<std::vector<char> > emptyArray;
	mArray = emptyArray;
	mHaveArray = false;
}

void XdmfArray::releaseArrayPointer()
{
	boost::shared_array<const char> emptyArrayPointer;
	mArrayPointer = emptyArrayPointer;
	mHaveArrayPointer = false;
}

void XdmfArray::reserve(const unsigned int size)
{
	if(mHaveArrayPointer)
	{
		internalizeArrayPointer();
	}
	if(!mHaveArray)
	{
		mTmpReserveSize = size;
	}
	else
	{
		boost::apply_visitor(Reserve(size), mArray);
	}
}

void XdmfArray::setHDF5Controller(const boost::shared_ptr<XdmfHDF5Controller> hdf5Controller)
{
	mHDF5Controller = hdf5Controller;
}

void XdmfArray::setName(const std::string & name)
{
	mName = name;
}

std::size_t XdmfArray::size() const
{
	if(mHaveArray)
	{
		return boost::apply_visitor(Size(), mArray);
	}
	else if(mHaveArrayPointer)
	{
		return mArrayPointerNumValues;
	}
	else if(mHDF5Controller)
	{
		return mHDF5Controller->size();
	}
	return 0;
}

void XdmfArray::swap(const boost::shared_ptr<XdmfArray> array)
{
	ArrayVariant tmpArray = array->mArray;
	ArrayPointerVariant tmpArrayPointer = array->mArrayPointer;
	int tmpArrayPointerNumValues = array->mArrayPointerNumValues;
	bool tmpHaveArray = array->mHaveArray;
	bool tmpHaveArrayPointer = array->mHaveArrayPointer;
	boost::shared_ptr<XdmfHDF5Controller> tmpHDF5Controller = array->mHDF5Controller;

	array->mArray = mArray;
	array->mArrayPointer = mArrayPointer;
	array->mArrayPointerNumValues = mArrayPointerNumValues;
	array->mHaveArray = mHaveArray;
	array->mHaveArrayPointer = mHaveArrayPointer;
	array->mHDF5Controller = mHDF5Controller;

	mArray = tmpArray;
	mArrayPointer = tmpArrayPointer;
	mArrayPointerNumValues = tmpArrayPointerNumValues;
	mHaveArray = tmpHaveArray;
	mHaveArrayPointer = tmpHaveArrayPointer;
	mHDF5Controller = tmpHDF5Controller;
}
