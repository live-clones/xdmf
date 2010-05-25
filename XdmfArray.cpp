/*
 * XdmfArray.cpp
 *
 *  Created on: Jan 29, 2010
 *      Author: kleiter
 */

#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>
#include <sstream>
#include "XdmfArray.hpp"
#include "XdmfHDF5Controller.hpp"
#include "XdmfVisitor.hpp"

class XdmfArray::Clear : public boost::static_visitor <void> {
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

class XdmfArray::CopyArrayValues : public boost::static_visitor <void> {
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

class XdmfArray::GetCapacity : public boost::static_visitor <unsigned int> {
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

class XdmfArray::GetPrecision : public boost::static_visitor <unsigned int> {
public:

	GetPrecision()
	{
	}

	unsigned int getPrecision(const char * const) const
	{
		return 1;
	}

	unsigned int getPrecision(const short * const) const
	{
		return 2;
	}

	unsigned int getPrecision(const int * const) const
	{
		return 4;
	}

	unsigned int getPrecision(const long * const) const
	{
		return 8;
	}

	unsigned int getPrecision(const float * const) const
	{
		return 4;
	}

	unsigned int getPrecision(const double * const) const
	{
		return 8;
	}

	unsigned int getPrecision(const unsigned char * const) const
	{
		return 1;
	}

	unsigned int getPrecision(const unsigned short * const) const
	{
		return 2;
	}

	unsigned int getPrecision(const unsigned int * const) const
	{
		return 4;
	}

	template<typename T>
	unsigned int operator()(const boost::shared_ptr<std::vector<T> > & array) const
	{
		return this->getPrecision(&(array.get()->operator[](0)));
	}

	template<typename T>
	unsigned int operator()(const boost::shared_array<const T> & array) const
	{
		return this->getPrecision(array.get());
	}
};

class XdmfArray::GetType : public boost::static_visitor <std::string> {
public:

	GetType()
	{
	}

	std::string getType(const char * const) const
	{
		return "Char";
	}

	std::string getType(const short * const) const
	{
		return "Short";
	}

	std::string getType(const int * const) const
	{
		return "Int";
	}

	std::string getType(const long * const) const
	{
		return "Int";
	}

	std::string getType(const float * const) const
	{
		return "Float";
	}

	std::string getType(const double * const) const
	{
		return "Float";
	}

	std::string getType(const unsigned char * const) const
	{
		return "UChar";
	}

	std::string getType(const unsigned short * const) const
	{
		return "UShort";
	}

	std::string getType(const unsigned int * const) const
	{
		return "UInt";
	}

	template<typename T>
	std::string operator()(const boost::shared_ptr<std::vector<T> > & array) const
	{
		return this->getType(&(array.get()->operator[](0)));
	}

	template<typename T>
	std::string operator()(const boost::shared_array<const T> & array) const
	{
		return this->getType(array.get());
	}
};

class XdmfArray::GetSize : public boost::static_visitor <unsigned int> {
public:

	GetSize()
	{
	}

	template<typename T>
	unsigned int operator()(const boost::shared_ptr<std::vector<T> > & array) const
	{
		return array->size();
	}
};

class XdmfArray::GetValuesPointer : public boost::static_visitor <const void * const> {
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

class XdmfArray::GetValuesString : public boost::static_visitor <std::string> {
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

class XdmfArray::InternalizeArrayPointer : public boost::static_visitor <void> {
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

class XdmfArray::NewArray : public boost::static_visitor <void> {
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

class XdmfArray::Reserve : public boost::static_visitor <void> {
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

XdmfArray::XdmfArray() :
	mHaveArray(false),
	mHaveArrayPointer(false),
	mArrayPointerNumValues(0),
	mHDF5Controller(boost::shared_ptr<XdmfHDF5Controller>()),
	mTmpReserveSize(0)
{
	std::cout << "Created Array " << this << std::endl;
}

XdmfArray::~XdmfArray()
{
	std::cout << "Deleted Array " << this << std::endl;
}

std::string XdmfArray::ItemTag = "DataItem";

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

void XdmfArray::clear()
{
	if(mHaveArray)
	{
		return boost::apply_visitor(Clear(), mArray);
	}
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
	return mHDF5Controller;
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
	arrayProperties["DataType"] = this->getType();
	std::stringstream precision;
	precision <<  this->getPrecision();
	arrayProperties["Precision"] = precision.str();
	std::stringstream size;
	size <<  this->getSize();
	arrayProperties["Dimensions"] = size.str();
	return arrayProperties;
}

std::string XdmfArray::getItemTag() const
{
	return ItemTag;
}

unsigned int XdmfArray::getPrecision() const
{
	if(mHaveArray)
	{
		return boost::apply_visitor(GetPrecision(), mArray);
	}
	else if(mHaveArrayPointer)
	{
		return boost::apply_visitor(GetPrecision(), mArrayPointer);
	}
	else if(mHDF5Controller)
	{
		return mHDF5Controller->getPrecision();
	}
	return 0;
}

unsigned int XdmfArray::getSize() const
{
	if(mHaveArray)
	{
		return boost::apply_visitor(GetSize(), mArray);
	}
	else if(mHaveArrayPointer)
	{
		return mArrayPointerNumValues;
	}
	else if(mHDF5Controller)
	{
		return mHDF5Controller->getSize();
	}
	return 0;
}

std::string XdmfArray::getType() const
{
	if(mHaveArray)
	{
		return boost::apply_visitor(GetType(), mArray);
	}
	else if(mHaveArrayPointer)
	{
		return boost::apply_visitor(GetType(), mArrayPointer);
	}
	else if(mHDF5Controller)
	{
		return mHDF5Controller->getType();
	}
	return "None";
}

const void * const XdmfArray::getValuesPointer() const
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

void XdmfArray::initialize(const std::string & type, const unsigned int precision)
{
	if(type.compare("Char") == 0 && precision == 1)
	{
		this->initialize<char>();
	}
	else if(type.compare("Short") == 0 && precision == 2)
	{
		this->initialize<short>();
	}
	else if(type.compare("Int") == 0 && precision == 4)
	{
		this->initialize<int>();
	}
	else if(type.compare("Long") == 0 && precision == 8)
	{
		this->initialize<long>();
	}
	else if(type.compare("Float") == 0 && precision == 4)
	{
		this->initialize<float>();
	}
	else if(type.compare("Float") == 0 && precision == 8)
	{
		this->initialize<double>();
	}
	else if(type.compare("UChar") == 0 && precision == 1)
	{
		this->initialize<unsigned char>();
	}
	else if(type.compare("UShort") == 0 && precision == 2)
	{
		this->initialize<unsigned short>();
	}
	else if(type.compare("UInt") == 0 && precision == 4)
	{
		this->initialize<unsigned int>();
	}
	else if(type.compare("None") == 0)
	{
		// No-op uninitialized!
	}
	else
	{
		assert(false);
	}
}

bool XdmfArray::isInitialized()
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
	int precisionVal;
	int sizeVal;
	std::string typeVal;

	std::map<std::string, std::string>::const_iterator content = itemProperties.find("Content");
	if(content != itemProperties.end())
	{
		contentVal = content->second;
	}
	else
	{
		assert(false);
	}
	std::map<std::string, std::string>::const_iterator precision = itemProperties.find("Precision");
	if(precision != itemProperties.end())
	{
		precisionVal = atoi(precision->second.c_str());
	}
	else
	{
		assert(false);
	}
	std::map<std::string, std::string>::const_iterator size = itemProperties.find("Dimensions");
	if(size != itemProperties.end())
	{
		sizeVal = atoi(size->second.c_str());
	}
	else
	{
		assert(false);
	}
	std::map<std::string, std::string>::const_iterator type = itemProperties.find("DataType");
	if(type != itemProperties.end())
	{
		typeVal = type->second;
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
			mHDF5Controller = XdmfHDF5Controller::New(contentVal, precisionVal, sizeVal, typeVal);
		}
		else if(format->second.compare("XML") == 0)
		{
			this->initialize(typeVal, precisionVal);
			this->reserve(sizeVal);
			boost::char_separator<char> sep(" ");
			boost::tokenizer<boost::char_separator<char> > tokens(contentVal, sep);
			BOOST_FOREACH(std::string t, tokens)
			{
				double val = atof(t.c_str());
				this->pushBack(val);
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

void XdmfArray::setHDF5Controller(boost::shared_ptr<XdmfHDF5Controller> hdf5Controller)
{
	mHDF5Controller = hdf5Controller;
}

void XdmfArray::swap(boost::shared_ptr<XdmfArray> array)
{
	ArrayVariant tmpArray = array->mArray;
	ArrayPointerVariant tmpArrayPointer = array->mArrayPointer;
	int tmpArrayPointerNumValues = array->mArrayPointerNumValues;
	bool tmpHaveArray = array->mHaveArray;
	bool tmpHaveArrayPointer = array->mHaveArrayPointer;

	array->mArray = mArray;
	array->mArrayPointer = mArrayPointer;
	array->mArrayPointerNumValues = mArrayPointerNumValues;
	array->mHaveArray = mHaveArray;
	array->mHaveArrayPointer = mHaveArrayPointer;

	mArray = tmpArray;
	mArrayPointer = tmpArrayPointer;
	mArrayPointerNumValues = tmpArrayPointerNumValues;
	mHaveArray = tmpHaveArray;
	mHaveArrayPointer = tmpHaveArrayPointer;
}
