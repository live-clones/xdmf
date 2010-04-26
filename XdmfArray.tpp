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

struct XdmfArray::NullDeleter
{
    void operator()(void const *) const
    {
    }
};

template<typename T>
void XdmfArray::copyValues(const int startIndex, const T * const valuesPointer, const int numValues, const int arrayStride, const int valuesStride)
{
	if(!mInitialized)
	{
		initialize<T>();
	}
	boost::apply_visitor( CopyValues<T>(startIndex, valuesPointer, numValues, arrayStride, valuesStride), mArray);
}

template <typename T>
boost::shared_ptr<std::vector<T> > XdmfArray::getValues()
{
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

template <typename T>
const boost::shared_ptr<const std::vector<T> > XdmfArray::getValues() const
{
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

template <typename T>
boost::shared_ptr<std::vector<T> > XdmfArray::initialize()
{
	// Set type of variant to type of pointer
	boost::shared_ptr<std::vector<T> > newArray(new std::vector<T>());
	mArray = newArray;
	mInitialized = true;
	return newArray;
}

template<typename T>
void XdmfArray::setValues(std::vector<T> & array)
{
	boost::shared_ptr<std::vector<T> > newArray(&array, NullDeleter());
	mArray = newArray;
	mInitialized = true;
}

template<typename T>
void XdmfArray::setValues(boost::shared_ptr<std::vector<T> > array)
{
	mArray = array;
	mInitialized = true;
}

template<typename T>
bool XdmfArray::swap(std::vector<T> & array)
{
	if(!mInitialized)
	{
		initialize<T>();
	}
	try
	{
		boost::shared_ptr<std::vector<T> > currArray = boost::get<boost::shared_ptr<std::vector<T> > >(mArray);
		currArray->swap(array);
		if(currArray->size() == 0)
		{
			mInitialized = false;
		}
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
