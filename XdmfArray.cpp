/*
 * XdmfArray.cpp
 *
 *  Created on: Jan 29, 2010
 *      Author: kleiter
 */

#include "XdmfArray.hpp"
#include "XdmfVisitor.hpp"
#include <sstream>

class XdmfArrayClear : public boost::static_visitor <void> {
public:

	XdmfArrayClear()
	{
	}

	template<typename T> void operator()(const boost::shared_ptr<std::vector<T> > & array) const
	{
		return array->clear();
	}
};

class XdmfArrayCopyArrayValues : public boost::static_visitor <void> {
public:

	XdmfArrayCopyArrayValues(int startIndex, int valuesStartIndex, int numValues, int arrayStride, int valuesStride) :
		mStartIndex(startIndex),
		mValuesStartIndex(valuesStartIndex),
		mNumValues(numValues),
		mArrayStride(arrayStride),
		mValuesStride(valuesStride)
	{
	}

	template<typename T, typename U> void operator()(const boost::shared_ptr<std::vector<T> > & array, const boost::shared_ptr<std::vector<U> > & arrayToCopy) const
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

	int mStartIndex;
	int mValuesStartIndex;
	int mNumValues;
	int mArrayStride;
	int mValuesStride;
};

class XdmfArrayGetHDF5Type : public boost::static_visitor <hid_t> {
public:
	XdmfArrayGetHDF5Type()
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

class XdmfArrayGetPrecision : public boost::static_visitor <int> {
public:
	XdmfArrayGetPrecision()
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

class XdmfArrayGetType : public boost::static_visitor <std::string> {
public:
	XdmfArrayGetType()
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

class XdmfArrayGetSize : public boost::static_visitor <int> {
public:

	XdmfArrayGetSize()
	{
	}

	template<typename T> int operator()(const boost::shared_ptr<std::vector<T> > & array) const
	{
		return array->size();
	}
};

class XdmfArrayGetValuesPointer : public boost::static_visitor <const void* const> {
public:

	XdmfArrayGetValuesPointer()
	{
	}

	template<typename T> const void* const operator()(const boost::shared_ptr<std::vector<T> > & array) const
	{
		return &array->operator[](0);
	}
};

class XdmfArrayGetValuesString : public boost::static_visitor <std::string> {
public:

	XdmfArrayGetValuesString()
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

	template<typename T> std::string operator()(const boost::shared_ptr<std::vector<T> > & array) const
	{
		std::stringstream toReturn;
		for(int i=0; i<array->size(); ++i)
		{
			toReturn << array->operator[](i) << " ";
		}
		return toReturn.str();
	}
};

class XdmfArrayNewArray : public boost::static_visitor <void> {
public:

	XdmfArrayNewArray()
	{
	}

	template<typename T> void operator()(boost::shared_ptr<std::vector<T> > & array) const
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
		mArray = values->getVariant();
		// Reinitialize variant array to contain new array with same type.
		boost::apply_visitor( XdmfArrayNewArray(), mArray);
		//boost::shared_ptr<std::vector<double> > newArray(new std::vector<double>());
		//mArray = newArray;

		//std::cout << boost::get<boost::shared_ptr<std::vector<double> > >(mArray)->operator[](0) << std::endl;
		//std::cout << this->getValuesString() << std::endl;
	}
	boost::apply_visitor( XdmfArrayCopyArrayValues(startIndex, valuesStartIndex, numValues, arrayStride, valuesStride), mArray, values->getVariant());
}

void XdmfArray::clear()
{
	return boost::apply_visitor( XdmfArrayClear(), mArray);
}

hid_t XdmfArray::getHDF5Type() const
{
	return boost::apply_visitor( XdmfArrayGetHDF5Type(), mArray);
}

int XdmfArray::getPrecision() const
{
	return boost::apply_visitor( XdmfArrayGetPrecision(), mArray);
}

int XdmfArray::getSize() const
{
	return boost::apply_visitor( XdmfArrayGetSize(), mArray);
}

std::string XdmfArray::getType() const
{
	return boost::apply_visitor( XdmfArrayGetType(), mArray);
}

const void* const XdmfArray::getValuesPointer() const
{
	return boost::apply_visitor( XdmfArrayGetValuesPointer(), mArray);
}

std::string XdmfArray::getValuesString() const
{
	return boost::apply_visitor( XdmfArrayGetValuesString(), mArray);
}

std::string XdmfArray::printSelf() const
{
	return "";
}

void XdmfArray::write(boost::shared_ptr<XdmfVisitor> visitor) const
{
	visitor->visit(this, visitor);
}
