/*
 * XdmfArray.cpp
 *
 *  Created on: Jan 29, 2010
 *      Author: kleiter
 */

#include "XdmfArray.hpp"
#include "XdmfVisitor.hpp"
#include <sstream>

class XdmfArrayGetPrecision : public boost::static_visitor <int> {
public:
	XdmfArrayGetPrecision()
	{
	}

	int operator()(const std::vector<char> & array) const
	{
		return 1;
	}

	int operator()(const std::vector<short> & array) const
	{
		return 2;
	}

	int operator()(const std::vector<int> & array) const
	{
		return 4;
	}

	int operator()(const std::vector<long> & array) const
	{
		return 8;
	}

	int operator()(const std::vector<float> & array) const
	{
		return 4;
	}

	int operator()(const std::vector<double> & array) const
	{
		return 8;
	}

	int operator()(const std::vector<unsigned char> & array) const
	{
		return 1;
	}

	int operator()(const std::vector<unsigned short> & array) const
	{
		return 2;
	}

	int operator()(const std::vector<unsigned int> & array) const
	{
		return 4;
	}
};

class XdmfArrayGetType : public boost::static_visitor <std::string> {
public:
	XdmfArrayGetType()
	{
	}

	std::string operator()(const std::vector<char> & array) const
	{
		return "Char";
	}

	std::string operator()(const std::vector<short> & array) const
	{
		return "Short";
	}

	std::string operator()(const std::vector<int> & array) const
	{
		return "Int";
	}

	std::string operator()(const std::vector<long> & array) const
	{
		return "Int";
	}

	std::string operator()(const std::vector<float> & array) const
	{
		return "Float";
	}

	std::string operator()(const std::vector<double> & array) const
	{
		return "Float";
	}

	std::string operator()(const std::vector<unsigned char> & array) const
	{
		return "UChar";
	}

	std::string operator()(const std::vector<unsigned short> & array) const
	{
		return "UShort";
	}

	std::string operator()(const std::vector<unsigned int> & array) const
	{
		return "UInt";
	}
};

class XdmfArrayGetHDF5Type : public boost::static_visitor <hid_t> {
public:
	XdmfArrayGetHDF5Type()
	{
	}

	hid_t operator()(const std::vector<char> & array) const
	{
		return H5T_NATIVE_CHAR;
	}

	hid_t operator()(const std::vector<short> & array) const
	{
		return H5T_NATIVE_SHORT;
	}

	hid_t operator()(const std::vector<int> & array) const
	{
		return H5T_NATIVE_INT;
	}

	hid_t operator()(const std::vector<long> & array) const
	{
		return H5T_NATIVE_LONG;
	}

	hid_t operator()(const std::vector<float> & array) const
	{
		return H5T_NATIVE_FLOAT;
	}

	hid_t operator()(const std::vector<double> & array) const
	{
		return H5T_NATIVE_DOUBLE;
	}

	hid_t operator()(const std::vector<unsigned char> & array) const
	{
		return H5T_NATIVE_UCHAR;
	}

	hid_t operator()(const std::vector<unsigned short> & array) const
	{
		return H5T_NATIVE_USHORT;
	}

	hid_t operator()(const std::vector<unsigned int> & array) const
	{
		return H5T_NATIVE_UINT;
	}
};

class XdmfArrayGetSize : public boost::static_visitor <int> {
public:

	XdmfArrayGetSize()
	{
	}

	template<typename T> int operator()(const std::vector<T> & array) const
	{
		return array.size();
	}
};

class XdmfArrayGetValues : public boost::static_visitor <std::string> {
public:

	XdmfArrayGetValues()
	{
	}

	std::string operator()(const std::vector<char> & array) const
	{
		std::stringstream toReturn;
		for(int i=0; i<array.size(); ++i)
		{
			toReturn << (int)(array[i]) << " ";
		}
		return toReturn.str();
	}

	template<typename T> std::string operator()(const std::vector<T> & array) const
	{
		std::stringstream toReturn;
		for(int i=0; i<array.size(); ++i)
		{
			toReturn << array[i] << " ";
		}
		return toReturn.str();
	}
};

class XdmfArrayGetValuesPointer : public boost::static_visitor <void*> {
public:
	XdmfArrayGetValuesPointer()
	{
	}

	template<typename T> void* operator()(std::vector<T> & array) const
	{
		return &array[0];
	}
};

class XdmfArrayGetValuesPointerConst : public boost::static_visitor <const void* const> {
public:

	XdmfArrayGetValuesPointerConst()
	{
	}

	template<typename T> const void* const operator()(const std::vector<T> & array) const
	{
		return &array[0];
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

std::string XdmfArray::getValues() const
{
	return boost::apply_visitor( XdmfArrayGetValues(), mArray);
}

void* XdmfArray::getValuesPointer()
{
	return boost::apply_visitor( XdmfArrayGetValuesPointer(), mArray);
}

const void* const XdmfArray::getValuesPointer() const
{
	return boost::apply_visitor( XdmfArrayGetValuesPointerConst(), mArray);
}

std::string XdmfArray::printSelf() const
{
	return "";
}

void XdmfArray::write(boost::shared_ptr<XdmfVisitor> visitor) const
{
	visitor->visit(this);
}
