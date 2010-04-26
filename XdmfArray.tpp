// Includes
#include "XdmfArray.hpp"

template<typename T>
class XdmfArray::CopyValues : public boost::static_visitor <void> {
public:

	CopyValues(const int startIndex, const T * const valuesPointer, const int numValues, const int arrayStride, const int valuesStride) :
		mStartIndex(startIndex),
		mValuesPointer(valuesPointer),
		mNumValues(numValues),
		mArrayStride(arrayStride),
		mValuesStride(valuesStride)
	{
	}

	template<typename U>
	void operator()(boost::shared_ptr<std::vector<U> > & array) const
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

	const int mStartIndex;
	const T * const mValuesPointer;
	const int mNumValues;
	const int mArrayStride;
	const int mValuesStride;
};

template<typename T>
class XdmfArray::Resize : public boost::static_visitor <void> {
public:

	Resize(const unsigned int numValues, const T & val) :
		mNumValues(numValues),
		mVal(val)
	{
	}

	template<typename U>
	void operator()(boost::shared_ptr<std::vector<U> > & array) const
	{
		array->resize(mNumValues, (U)mVal);
	}

private:

	const unsigned int mNumValues;
	const T & mVal;
};

struct XdmfArray::NullDeleter
{
    void operator()(void const *) const
    {
    }
};

template<typename T>
void XdmfArray::copyValues(const int startIndex, const T * const valuesPointer, const int numValues, const int arrayStride, const int valuesStride)
{
	if(mHaveArrayPointer)
	{
		internalizeArrayPointer();
	}
	if(!mHaveArray)
	{
		initialize<T>();
	}
	boost::apply_visitor( CopyValues<T>(startIndex, valuesPointer, numValues, arrayStride, valuesStride), mArray);
}

template <typename T>
boost::shared_ptr<std::vector<T> > XdmfArray::getValues()
{
	if(mHaveArrayPointer)
	{
		internalizeArrayPointer();
	}
	try
	{
		boost::shared_ptr<std::vector<T> > currArray = boost::get<boost::shared_ptr<std::vector<T> > >(mArray);
		return currArray;
	}
	catch(const boost::bad_get& exception)
	{
		return boost::shared_ptr<std::vector<T> >();
	}
}

//template <typename T>
//const boost::shared_ptr<const std::vector<T> > XdmfArray::getValues() const
//{
//	try
//	{
//		boost::shared_ptr<std::vector<T> > currArray = boost::get<boost::shared_ptr<std::vector<T> > >(mArray);
//		return currArray;
//	}
//	catch(const boost::bad_get& exception)
//	{
//		return boost::shared_ptr<std::vector<T> >();
//	}
//}

template <typename T>
boost::shared_ptr<std::vector<T> > XdmfArray::initialize()
{
	if(mHaveArrayPointer)
	{
		releaseArrayPointer();
	}
	// Set type of variant to type of pointer
	boost::shared_ptr<std::vector<T> > newArray(new std::vector<T>());
	mArray = newArray;
	mHaveArray = true;
	return newArray;
}

template<typename T>
void XdmfArray::resize(const unsigned int numValues, const T & val)
{
	if(mHaveArrayPointer)
	{
		internalizeArrayPointer();
	}
	if(!mHaveArray)
	{
		initialize<T>();
	}
	return boost::apply_visitor(Resize<T>(numValues, val), mArray);
}

template<typename T>
void XdmfArray::setValues(const T * const arrayPointer, const int numValues, const bool transferOwnership)
{
	// Remove contents of internal array.
	if(mHaveArray)
	{
		releaseArray();
	}
	if(transferOwnership)
	{
		const boost::shared_array<const T> newArrayPointer(arrayPointer);
		mArrayPointer = newArrayPointer;
	}
	else
	{
		const boost::shared_array<const T> newArrayPointer(arrayPointer, NullDeleter());
		mArrayPointer = newArrayPointer;
	}
	mHaveArrayPointer = true;
	mArrayPointerNumValues = numValues;
}

template<typename T>
void XdmfArray::setValues(std::vector<T> & array, const bool transferOwnership)
{
	if(mHaveArrayPointer)
	{
		releaseArrayPointer();
	}
	if(transferOwnership)
	{
		boost::shared_ptr<std::vector<T> > newArray(&array);
		mArray = newArray;
	}
	else
	{
		boost::shared_ptr<std::vector<T> > newArray(&array, NullDeleter());
		mArray = newArray;
	}
	mHaveArray = true;
}

template<typename T>
void XdmfArray::setValues(boost::shared_ptr<std::vector<T> > array)
{
	if(mHaveArrayPointer)
	{
		releaseArrayPointer();
	}
	mArray = array;
	mHaveArray = true;
}

template<typename T>
bool XdmfArray::swap(std::vector<T> & array)
{
	if(mHaveArrayPointer)
	{
		internalizeArrayPointer();
	}
	if(!mHaveArray)
	{
		initialize<T>();
	}
	try
	{
		boost::shared_ptr<std::vector<T> > currArray = boost::get<boost::shared_ptr<std::vector<T> > >(mArray);
		currArray->swap(array);
		return true;
	}
	catch(const boost::bad_get& exception)
	{
		return false;
	}
}

template<typename T>
bool XdmfArray::swap(boost::shared_ptr<std::vector<T> > array)
{
	return this->swap(*array.get());
}
