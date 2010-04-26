/*
 * XdmfArray.cpp
 *
 *  Created on: Jan 29, 2010
 *      Author: kleiter
 */

#include "XdmfArray.hpp"
#include "XdmfVisitor.hpp"
#include <sstream>

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

	CopyArrayValues(const int startIndex, const int valuesStartIndex, const int numValues, const int arrayStride, const int valuesStride) :
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
		int size = mStartIndex + mNumValues;
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

	const int mStartIndex;
	const int mValuesStartIndex;
	const int mNumValues;
	const int mArrayStride;
	const int mValuesStride;
};

class XdmfArray::GetHDF5Type : public boost::static_visitor <hid_t> {
public:

	GetHDF5Type()
	{
	}

	hid_t getHDF5Type(const char * const) const
	{
		return H5T_NATIVE_CHAR;
	}

	hid_t getHDF5Type(const short * const) const
	{
		return H5T_NATIVE_SHORT;
	}

	hid_t getHDF5Type(const int * const) const
	{
		return H5T_NATIVE_INT;
	}

	hid_t getHDF5Type(const long * const) const
	{
		return H5T_NATIVE_LONG;
	}

	hid_t getHDF5Type(const float * const) const
	{
		return H5T_NATIVE_FLOAT;
	}

	hid_t getHDF5Type(const double * const) const
	{
		return H5T_NATIVE_DOUBLE;
	}

	hid_t getHDF5Type(const unsigned char * const) const
	{
		return H5T_NATIVE_UCHAR;
	}

	hid_t getHDF5Type(const unsigned short * const) const
	{
		return H5T_NATIVE_USHORT;
	}

	hid_t getHDF5Type(const unsigned int * const) const
	{
		return H5T_NATIVE_UINT;
	}

	template<typename T>
	hid_t operator()(const boost::shared_ptr<std::vector<T> > & array) const
	{
		return this->getHDF5Type(&(array.get()->operator[](0)));
	}

	template<typename T>
	hid_t operator()(const boost::shared_array<const T> & array) const
	{
		return this->getHDF5Type(array.get());
	}
};

class XdmfArray::GetPrecision : public boost::static_visitor <int> {
public:

	GetPrecision()
	{
	}

	int getPrecision(const char * const) const
	{
		return 1;
	}

	int getPrecision(const short * const) const
	{
		return 2;
	}

	int getPrecision(const int * const) const
	{
		return 4;
	}

	int getPrecision(const long * const) const
	{
		return 8;
	}

	int getPrecision(const float * const) const
	{
		return 4;
	}

	int getPrecision(const double * const) const
	{
		return 8;
	}

	int getPrecision(const unsigned char * const) const
	{
		return 1;
	}

	int getPrecision(const unsigned short * const) const
	{
		return 2;
	}

	int getPrecision(const unsigned int * const) const
	{
		return 4;
	}

	template<typename T>
	int operator()(const boost::shared_ptr<std::vector<T> > & array) const
	{
		return this->getPrecision(&(array.get()->operator[](0)));
	}

	template<typename T>
	int operator()(const boost::shared_array<const T> & array) const
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

class XdmfArray::GetSize : public boost::static_visitor <int> {
public:

	GetSize()
	{
	}

	template<typename T>
	int operator()(const boost::shared_ptr<std::vector<T> > & array) const
	{
		return array->size();
	}
};

class XdmfArray::GetValuesPointer : public boost::static_visitor <const void* const> {
public:

	GetValuesPointer()
	{
	}

	template<typename T>
	const void* const operator()(const boost::shared_ptr<std::vector<T> > & array) const
	{
		return &array->operator[](0);
	}

	template<typename T>
	const void* const operator()(const boost::shared_array<const T> & array) const
	{
		return array.get();
	}
};

class XdmfArray::GetValuesString : public boost::static_visitor <std::string> {
public:

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

	const int mArrayPointerNumValues;
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

XdmfArray::XdmfArray() :
	mHaveArray(false),
	mHaveArrayPointer(false),
	mArrayPointerNumValues(0)
{
	std::cout << "Created Array " << this << std::endl;
}

XdmfArray::~XdmfArray()
{
	std::cout << "Deleted Array " << this << std::endl;
}

void XdmfArray::copyValues(const int startIndex, const boost::shared_ptr<const XdmfArray> values, const int valuesStartIndex, const int numValues, const int arrayStride, const int valuesStride)
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

hid_t XdmfArray::getHDF5Type() const
{
	if(mHaveArray)
	{
		return boost::apply_visitor(GetHDF5Type(), mArray);
	}
	else if(mHaveArrayPointer)
	{
		return boost::apply_visitor(GetHDF5Type(), mArrayPointer);
	}
	return -1;
}

int XdmfArray::getPrecision() const
{
	if(mHaveArray)
	{
		return boost::apply_visitor(GetPrecision(), mArray);
	}
	else if(mHaveArrayPointer)
	{
		return boost::apply_visitor(GetPrecision(), mArrayPointer);
	}
	return 0;
}

int XdmfArray::getSize() const
{
	if(mHaveArray)
	{
		return boost::apply_visitor(GetSize(), mArray);
	}
	else if(mHaveArrayPointer)
	{
		return mArrayPointerNumValues;
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
	return "";
}

const void* const XdmfArray::getValuesPointer() const
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
		return boost::apply_visitor(GetValuesString(mArrayPointerNumValues), mArray);
	}
	else if(mHaveArrayPointer)
	{
		return boost::apply_visitor(GetValuesString(mArrayPointerNumValues), mArrayPointer);
	}
	return "";
}

void XdmfArray::internalizeArrayPointer()
{
	if(mHaveArrayPointer)
	{
		boost::apply_visitor(InternalizeArrayPointer(this), mArrayPointer);
	}
}

std::string XdmfArray::printSelf() const
{
	return "";
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

void XdmfArray::write(boost::shared_ptr<XdmfVisitor> visitor) const
{
	visitor->visit(this, visitor);
}
