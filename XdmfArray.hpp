/*
 * XdmfArray.hpp
 *
 *  Created on: Jan 25, 2010
 *      Author: kleiter
 */

#ifndef XDMFARRAY_HPP_
#define XDMFARRAY_HPP_

// Includes
#include "XdmfItem.hpp"
#include <boost/variant.hpp>
#include <hdf5.h>
#include <vector>

template<typename T>
class XdmfArraySetValues : public boost::static_visitor <void> {
public:

	XdmfArraySetValues(int startIndex, T* valuesPointer, int numValues, int arrayStride = 1, int valuesStride = 1) :
		mStartIndex(startIndex),
		mValuesPointer(valuesPointer),
		mNumValues(numValues),
		mArrayStride(arrayStride),
		mValuesStride(valuesStride)
	{
	}

	template<typename U> void operator()(std::vector<U> & array) const
	{
		int size = mStartIndex + mNumValues;
		if(mArrayStride > 1)
		{
			size = mStartIndex + mNumValues * mArrayStride - 1;
		}
		if(array.size() < size)
		{
			array.resize(size);
		}
		for(int i=0; i<mNumValues; ++i)
		{
			array[mStartIndex + i*mArrayStride] = (U)mValuesPointer[i*mValuesStride];
		}
	}

private:

	int mStartIndex;
	T* mValuesPointer;
	int mNumValues;
	int mArrayStride;
	int mValuesStride;
};

class XdmfArray : public XdmfItem {
public:

	XdmfNewMacro(XdmfArray);

	/**
     * Get the data type of this array.
     *
     * @return a string containing the Xdmf data type for the array, this is one of
     *      Char, Short, Int, Float, UChar, UShort, UInt.
     */
	virtual std::string getType() const;

	/**
	 * Get the hdf5 data type of this array.
	 *
	 * @return a hid_t value containing the hdf5 data type for the array.
	 */
	virtual hid_t getHDF5Type() const;

	/**
	 * Get the precision, in bytes, of the data type of this array.
	 *
	 * @return a int containing the precision, in bytes, of the data type of this array.
	 */
	virtual int getPrecision() const;

	/**
	 * Get the number of values stored in this array.
	 *
	 * @return a int containing the number of values stored in this array.
	 */
	virtual int getSize() const;

	/**
	 * Get the values stored in this array as a string.
	 *
	 * @return a string containing the contents of the array.
	 */
	virtual std::string getValues() const;

	/**
	 * Get a pointer to the values stored in this array.
	 *
	 * @return a void pointer to the first value stored in this array.
	 */
	virtual void* getValuesPointer();

	/**
	 * Get a pointer to the values stored in this array (const version).
	 *
	 * @return a void pointer to the first value stored in this array.
	 */
	virtual const void* const getValuesPointer() const;

	virtual std::string printSelf() const;

	/**
	 * Copy the values from a pointer into this array.
	 *
	 * @param startIndex the index in this array to begin insertion.
	 * @param valuesPointer a pointer to the values to copy into this array.
	 * @param numValues the number of values to copy into this array.
	 * @param arrayStride number of values to stride in this array between each copy.
	 * @param valuesStride number of values to stride in the pointer between each copy.
	 */
	template<typename T> void setValues(int startIndex, T* valuesPointer, int numValues = 1, int arrayStride = 1, int valuesStride = 1)
	{
		if(!mInitialized)
		{
			// Set type of variant to type of pointer
			mArray = std::vector<T>();
			mInitialized = true;
	    }
	    boost::apply_visitor( XdmfArraySetValues<T>(startIndex, valuesPointer, numValues, arrayStride, valuesStride), mArray);
	}

	/**
	 * Copy the values from a vector into this array
	 *
	 * @param array the vector to copy into this array.
	 */
	template<typename T> void setValues(std::vector<T> & array)
	{
		mArray = array;
	}

	virtual void write(boost::shared_ptr<XdmfVisitor>) const;

protected:

	XdmfArray();
	virtual ~XdmfArray();

private:

	bool mInitialized;

	boost::variant<std::vector<char>,
				   std::vector<short>,
				   std::vector<int>,
				   std::vector<long>,
				   std::vector<float>,
				   std::vector<double>,
				   std::vector<unsigned char>,
				   std::vector<unsigned short>,
				   std::vector<unsigned int> > mArray;
};

#endif /* XDMFARRAY_HPP_ */
