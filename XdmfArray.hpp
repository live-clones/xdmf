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

template <typename T>
class XdmfArrayGetValues : public boost::static_visitor <boost::shared_ptr<std::vector<T> > > {
public:

    XdmfArrayGetValues()
    {
    }

    boost::shared_ptr<std::vector<T> > operator()(const boost::shared_ptr<std::vector<T> > array) const
    {
        return array;
    }

    template <typename U>
    boost::shared_ptr<std::vector<T> > operator()(const boost::shared_ptr<std::vector<U> > array) const
    {
        return boost::shared_ptr<std::vector<T> >();
    }
};

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

	template<typename U> void operator()(boost::shared_ptr<std::vector<U> > array) const
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
			array->operator[](mStartIndex + i*mArrayStride) = (U)mValuesPointer[i*mValuesStride];
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
	 * Copy the values from a vector into this array
	 *
	 * @param array the vector to copy into this array.
	 */
    template<typename T> void copyValues(std::vector<T> & array)
    {
    	boost::shared_ptr<std::vector<T> > newArray(new std::vector<T>(array));
    	mArray = newArray;
    }

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
	 * Get a smart pointer to the values stored in this array
	 *
	 * @return a smart pointer to the internal vector of values stored in this array
	 */
	template <typename T>
	boost::shared_ptr<std::vector<T> > getValues()
	{
		return boost::apply_visitor( XdmfArrayGetValues<T>(), mArray);
	}

	/**
	 * Get a smart pointer to the values stored in this array (const version)
	 *
	 * @return a smart pointer to the internal vector of values stored in this array
	 */
	//virtual const boost::shared_ptr<std::vector<void> > getValues() const;

	/**
	 * Get a pointer to the values stored in this array (const version).
	 *
	 * @return a void pointer to the first value stored in this array.
	 */
	virtual const void* const getValuesPointer() const;

	/**
	 * Get the values stored in this array as a string.
	 *
	 * @return a string containing the contents of the array.
	 */
	virtual std::string getValuesString() const;

	/**
	 * Insert the values from a pointer into this array.
	 *
	 * @param startIndex the index in this array to begin insertion.
	 * @param valuesPointer a pointer to the values to copy into this array.
	 * @param numValues the number of values to copy into this array.
	 * @param arrayStride number of values to stride in this array between each copy.
	 * @param valuesStride number of values to stride in the pointer between each copy.
	 */
	template<typename T> void insertValues(int startIndex, T* valuesPointer, int numValues = 1, int arrayStride = 1, int valuesStride = 1)
	{
		if(!mInitialized)
		{
			// Set type of variant to type of pointer
			boost::shared_ptr<std::vector<T> > newArray(new std::vector<T>());
			mArray = newArray;
			mInitialized = true;
		}
		boost::apply_visitor( XdmfArraySetValues<T>(startIndex, valuesPointer, numValues, arrayStride, valuesStride), mArray);
	}

	virtual std::string printSelf() const;

	/**
	 * Sets the values of this array to the values stored in the vector.  No copy is made.  This array shares ownership with
	 * other references to the smart pointer.
	 *
	 * @param a smart pointer to a vector to store in this array.
	 */
	template<typename T> void setValues(boost::shared_ptr<std::vector<T> > array)
	{
		mArray = array;
	}

	virtual void write(boost::shared_ptr<XdmfVisitor>) const;

protected:

	XdmfArray();
	virtual ~XdmfArray();

private:

	bool mInitialized;

	boost::variant<boost::shared_ptr<std::vector<char> >,
				   boost::shared_ptr<std::vector<short> >,
				   boost::shared_ptr<std::vector<int> >,
				   boost::shared_ptr<std::vector<long> >,
				   boost::shared_ptr<std::vector<float> >,
				   boost::shared_ptr<std::vector<double> >,
				   boost::shared_ptr<std::vector<unsigned char> >,
				   boost::shared_ptr<std::vector<unsigned short> >,
				   boost::shared_ptr<std::vector<unsigned int> > > mArray;
};

#endif /* XDMFARRAY_HPP_ */
