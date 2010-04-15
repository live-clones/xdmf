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

template <typename T>
class XdmfArrayGetValues : public boost::static_visitor <boost::shared_ptr<std::vector<T> > > {
public:

    XdmfArrayGetValues()
    {
    }

    boost::shared_ptr<std::vector<T> > operator()(const boost::shared_ptr<std::vector<T> > & array) const
    {
        return array;
    }

    template <typename U>
    boost::shared_ptr<std::vector<T> > operator()(const boost::shared_ptr<std::vector<U> > & array) const
    {
        return boost::shared_ptr<std::vector<T> >();
    }
};

template <typename T>
class XdmfArrayGetValuesConst : public boost::static_visitor <const boost::shared_ptr<const std::vector<T> > > {
public:

    XdmfArrayGetValuesConst()
    {
    }

    const boost::shared_ptr<const std::vector<T> > operator()(const boost::shared_ptr<const std::vector<T> > & array) const
    {
        return array;
    }

    template <typename U>
    const boost::shared_ptr<const std::vector<T> > operator()(const boost::shared_ptr<const std::vector<U> > & array) const
    {
        return boost::shared_ptr<std::vector<T> >();
    }
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
	return boost::apply_visitor( XdmfArrayGetValues<T>(), mArray);
}

template <typename T>
const boost::shared_ptr<const std::vector<T> > XdmfArray::getValues() const
{
	return boost::apply_visitor( XdmfArrayGetValuesConst<T>(), mArray);
}

template<typename T>
void XdmfArray::setValues(boost::shared_ptr<std::vector<T> > array)
{
	mArray = array;
}
