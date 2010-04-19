// Includes
#include "XdmfArray.hpp"

template<typename T>
class XdmfArrayCopyValues : public boost::static_visitor <void> {
public:

	XdmfArrayCopyValues(int startIndex, T * valuesPointer, int numValues = 1, int arrayStride = 1, int valuesStride = 1) :
		mStartIndex(startIndex),
		mValuesPointer(valuesPointer),
		mNumValues(numValues),
		mArrayStride(arrayStride),
		mValuesStride(valuesStride)
	{
	}

	template<typename U> void operator()(boost::shared_ptr<std::vector<U> > & array) const
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

template<typename T>
void XdmfArray::copyValues(int startIndex, T * valuesPointer, int numValues, int arrayStride, int valuesStride)
{
	if(!mInitialized)
	{
		// Set type of variant to type of pointer
		boost::shared_ptr<std::vector<T> > newArray(new std::vector<T>());
		mArray = newArray;
		mInitialized = true;
	}
	boost::apply_visitor( XdmfArrayCopyValues<T>(startIndex, valuesPointer, numValues, arrayStride, valuesStride), mArray);
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
		// Set type of variant to type of pointer
		boost::shared_ptr<std::vector<T> > newArray(new std::vector<T>());
		mArray = newArray;
		mInitialized = true;
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
