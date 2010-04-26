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

	hid_t operator()(const boost::shared_ptr<std::vector<char> > & array) const
	{
		return H5T_NATIVE_CHAR;
	}

	hid_t operator()(const boost::shared_ptr<std::vector<short> > & array) const
	{
		return H5T_NATIVE_SHORT;
	}

	hid_t operator()(const boost::shared_ptr<std::vector<int> > & array) const
	{
		return H5T_NATIVE_INT;
	}

	hid_t operator()(const boost::shared_ptr<std::vector<long> > & array) const
	{
		return H5T_NATIVE_LONG;
	}

	hid_t operator()(const boost::shared_ptr<std::vector<float> > & array) const
	{
		return H5T_NATIVE_FLOAT;
	}

	hid_t operator()(const boost::shared_ptr<std::vector<double> > & array) const
	{
		return H5T_NATIVE_DOUBLE;
	}

	hid_t operator()(const boost::shared_ptr<std::vector<unsigned char> > & array) const
	{
		return H5T_NATIVE_UCHAR;
	}

	hid_t operator()(const boost::shared_ptr<std::vector<unsigned short> > & array) const
	{
		return H5T_NATIVE_USHORT;
	}

	hid_t operator()(const boost::shared_ptr<std::vector<unsigned int> > & array) const
	{
		return H5T_NATIVE_UINT;
	}
};

class XdmfArray::GetPrecision : public boost::static_visitor <int> {
public:
	GetPrecision()
	{
	}

	int operator()(const boost::shared_ptr<std::vector<char> > & array) const
	{
		return 1;
	}

	int operator()(const boost::shared_ptr<std::vector<short> > & array) const
	{
		return 2;
	}

	int operator()(const boost::shared_ptr<std::vector<int> > & array) const
	{
		return 4;
	}

	int operator()(const boost::shared_ptr<std::vector<long> > & array) const
	{
		return 8;
	}

	int operator()(const boost::shared_ptr<std::vector<float> > & array) const
	{
		return 4;
	}

	int operator()(const boost::shared_ptr<std::vector<double> > & array) const
	{
		return 8;
	}

	int operator()(const boost::shared_ptr<std::vector<unsigned char> > & array) const
	{
		return 1;
	}

	int operator()(const boost::shared_ptr<std::vector<unsigned short> > & array) const
	{
		return 2;
	}

	int operator()(const boost::shared_ptr<std::vector<unsigned int> > & array) const
	{
		return 4;
	}
};

class XdmfArray::GetType : public boost::static_visitor <std::string> {
public:
	GetType()
	{
	}

	std::string operator()(const boost::shared_ptr<std::vector<char> > & array) const
	{
		return "Char";
	}

	std::string operator()(const boost::shared_ptr<std::vector<short> > & array) const
	{
		return "Short";
	}

	std::string operator()(const boost::shared_ptr<std::vector<int> > & array) const
	{
		return "Int";
	}

	std::string operator()(const boost::shared_ptr<std::vector<long> > & array) const
	{
		return "Int";
	}

	std::string operator()(const boost::shared_ptr<std::vector<float> > & array) const
	{
		return "Float";
	}

	std::string operator()(const boost::shared_ptr<std::vector<double> > & array) const
	{
		return "Float";
	}

	std::string operator()(const boost::shared_ptr<std::vector<unsigned char> > & array) const
	{
		return "UChar";
	}

	std::string operator()(const boost::shared_ptr<std::vector<unsigned short> > & array) const
	{
		return "UShort";
	}

	std::string operator()(const boost::shared_ptr<std::vector<unsigned int> > & array) const
	{
		return "UInt";
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
};

class XdmfArray::GetValuesString : public boost::static_visitor <std::string> {
public:

	GetValuesString()
	{
	}

	std::string operator()(const boost::shared_ptr<std::vector<char> > & array) const
	{
		std::stringstream toReturn;
		for(int i=0; i<array->size(); ++i)
		{
			toReturn << (int)(array->operator[](i)) << " ";
		}
		return toReturn.str();
	}

	template<typename T>
	std::string operator()(const boost::shared_ptr<std::vector<T> > & array) const
	{
		std::stringstream toReturn;
		for(int i=0; i<array->size(); ++i)
		{
			toReturn << array->operator[](i) << " ";
		}
		return toReturn.str();
	}
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
	mInitialized(false)
{
	std::cout << "Created Array " << this << std::endl;
}

XdmfArray::~XdmfArray()
{
	std::cout << "Deleted Array " << this << std::endl;
}

void XdmfArray::copyValues(int startIndex, boost::shared_ptr<XdmfArray> values, int valuesStartIndex, int numValues, int arrayStride, int valuesStride)
{
	if(!mInitialized)
	{
		// Copy the values variant in order to get the type (only taking smart pointer so no worries about large copies)
		mArray = values->mArray;
		// Reinitialize variant array to contain new array with same type.
		boost::apply_visitor( NewArray(), mArray);
		mInitialized = true;
	}
	boost::apply_visitor( CopyArrayValues(startIndex, valuesStartIndex, numValues, arrayStride, valuesStride), mArray, values->mArray);
}

void XdmfArray::clear()
{
	mInitialized = false;
	return boost::apply_visitor( Clear(), mArray);
}

hid_t XdmfArray::getHDF5Type() const
{
	return boost::apply_visitor( GetHDF5Type(), mArray);
}

int XdmfArray::getPrecision() const
{
	return boost::apply_visitor( GetPrecision(), mArray);
}

int XdmfArray::getSize() const
{
	return boost::apply_visitor( GetSize(), mArray);
}

std::string XdmfArray::getType() const
{
	return boost::apply_visitor( GetType(), mArray);
}

const void* const XdmfArray::getValuesPointer() const
{
	return boost::apply_visitor( GetValuesPointer(), mArray);
}

std::string XdmfArray::getValuesString() const
{
	return boost::apply_visitor( GetValuesString(), mArray);
}

std::string XdmfArray::printSelf() const
{
	return "";
}

void XdmfArray::swap(boost::shared_ptr<XdmfArray> array)
{
	array->mArray.swap(mArray);
	bool initialized = array->mInitialized;
	array->mInitialized = mInitialized;
	mInitialized = initialized;
}

void XdmfArray::write(boost::shared_ptr<XdmfVisitor> visitor) const
{
	visitor->visit(this, visitor);
}
