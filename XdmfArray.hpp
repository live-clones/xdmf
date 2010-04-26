#ifndef XDMFARRAY_HPP_
#define XDMFARRAY_HPP_

// Includes
#include "XdmfItem.hpp"
#include <boost/shared_array.hpp>
#include <boost/variant.hpp>
#include <hdf5.h>
#include <vector>

/**
 * @brief Provides a single interface for storing a wide variety of data types.
 *
 * XdmfArray stores data values that are read in or will be written to heavy data on disk. The
 * data type stored is determined by the type initially inserted into the XdmfArray.  XdmfArray
 * allows for insertion and retrieval of data in two fundamental ways:
 *
 * By Copy:
 *
 *  copyValues
 * 	getCopyValues
 *
 * 	XdmfArray stores its own copy of the data.  Modifications to the data stored in the XdmfArray will
 * 	not change values stored in the original array.
 *
 * By Shared Reference:
 *
 * 	setValues
 * 	getValues
 *
 * 	XdmfArray shares a reference to the data.  No copy is made. XdmfArray holds a shared pointer to the original
 * 	data.  Modifications to the data stored in the XdmfArray also causes modification to values stored in the original
 * 	array.
 *
 * Xdmf supports the following datatypes:
 * 	Char
 * 	Short
 * 	Int
 * 	Long
 * 	Float
 * 	Double
 * 	Unsigned Char
 * 	Unsigned Short
 * 	Unsigned Int
 */
class XdmfArray : public XdmfItem {
public:

	XdmfNewMacro(XdmfArray);

	/**
	 * Copy values from an XdmfArray into this array.
	 *
	 * @param startIndex the index in this array to begin insertion.
	 * @param values a shared pointer to an XdmfArray to copy into this array.
	 * @param valuesStartIndex the index in the XdmfArray to begin copying.
	 * @param numValues the number of values to copy into this array.
	 * @param arrayStride number of values to stride in this array between each copy.
	 * @param valuesStride number of values to stride in the XdmfArray between each copy.
	 */
	void copyValues(const int startIndex, const boost::shared_ptr<const XdmfArray> values, const int valuesStartIndex= 0, const int numValues = 1, const int arrayStride = 1, const int valuesStride = 1);

	/**
	 * Copy values from an array into this array.
	 *
	 * @param startIndex the index in this array to begin insertion.
	 * @param valuesPointer a pointer to the values to copy into this array.
	 * @param numValues the number of values to copy into this array.
	 * @param arrayStride number of values to stride in this array between each copy.
	 * @param valuesStride number of values to stride in the pointer between each copy.
	 */
	template<typename T>
	void copyValues(const int startIndex, const T * const valuesPointer, const int numValues = 1, const int arrayStride = 1, const int valuesStride = 1);

	/**
	 * Remove all values from this array
	 */
	virtual void clear();

	/**
	 * Get the hdf5 data type of this array.
	 *
	 * @return a hid_t value containing the hdf5 data type for the array.
	 */
	virtual hid_t getHDF5Type() const;

	/**
	 * Get the precision, in bytes, of the data type of this array.
	 *
	 * @return an int containing the precision, in bytes, of the data type of this array.
	 */
	virtual int getPrecision() const;

	/**
	 * Get the number of values stored in this array.
	 *
	 * @return an int containing the number of values stored in this array.
	 */
	virtual int getSize() const;

	/**
	 * Get the data type of this array.
	 *
	 * @return a string containing the Xdmf data type for the array, this is one of
	 *      Char, Short, Int, Float, UChar, UShort, UInt.
	 */
	virtual std::string getType() const;

	/**
	 * Get a smart pointer to the values stored in this array.
	 *
	 * @return a smart pointer to the internal vector of values stored in this array.
	 */
	template <typename T>
	boost::shared_ptr<std::vector<T> > getValues();

	/**
	 * Get a smart pointer to the values stored in this array (const version).
	 *
	 * @return a smart pointer to the internal vector of values stored in this array.
	 */
	//template <typename T>
	//const boost::shared_ptr<const std::vector<T> > getValues() const;

	/**
	 * Get a pointer to the values stored in this array.
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
	 * Initializes the array to contain an empty container of a particular type.
	 *
	 * @return a smart pointer to the internal vector of values initialized in this array.
	 */
	template <typename T>
	boost::shared_ptr<std::vector<T> > initialize();

	virtual std::string printSelf() const;

	/**
	 * Release all data held by this XdmfArray.
	 */
	void releaseData()
	{
		releaseArray();
		releaseArrayPointer();
	}

	/**
	 * Resizes the XdmfArray to contain numValues.  If numValues is larger than the current size, append values to end equal
	 * to val.  If numValues is less than the current size, values at indices larger than numValues are removed.
	 *
	 * @param numValues the number of values to resize this array to.
	 * @param val the number to initialized newly created values to, if needed.
	 */
	template<typename T>
	void resize(const unsigned int numValues, const T & val);

	/**
	 * Sets the values of this array to the values stored in the arrayPointer array.  No copy is made.  Modifications to the array are
	 * not permitted through the XdmfArray API.  Any calls through the XdmfArray API to modify the array (i.e. any non-const function)
	 * will result in the array being copied into internal storage.  The internal copy is then modified.
	 * This prevents situations where a realloc of the pointer could cause other references to become invalid.
	 * The caller of this method can continue to modify the values stored in arrayPointer on its own.
	 * This function is meant for applications that have their own array data structures that merely use Xdmf to output the data,
	 * an operation that should not require a copy.  Other applications that use Xdmf for in memory data storage should avoid
	 * this function.
	 *
	 * @param arrayPointer a pointer to an array to store in this XdmfArray.
	 * @param numValues the number of values in the array.
	 * @param transferOwnership whether to transfer responsibility for deletion of the array to XdmfArray.
	 */
	template<typename T>
	void setValues(const T * const arrayPointer, const int numValues, const bool transferOwnership = 0);

	/**
	 * Sets the values of this array to the values stored in the vector.  No copy is made.  The caller of this method retains
	 * ownership of the data and must ensure that the array is still valid for the entire time Xdmf needs it.
	 *
	 * @param array a vector to store in this array.
	 * @param transferOwnership whether to transfer responsibility for deletion of the array to XdmfArray.
	 */
	template<typename T>
	void setValues(std::vector<T> & array, const bool transferOwnership = 0);

	/**
	 * Sets the values of this array to the values stored in the vector.  No copy is made.  This array shares ownership with
	 * other references to the smart pointer.
	 *
	 * @param array a smart pointer to a vector to store in this array.
	 */
	template<typename T>
	void setValues(boost::shared_ptr<std::vector<T> > array);

	/**
	 * Exchange the contents of the vector with the contents of this XdmfArray.  No copy is made.  The internal arrays are swapped.
	 *
	 * @param array a vector to exchange values with.
	 * @return bool whether the swap was successful.
	 */
	template<typename T>
	bool swap(std::vector<T> & array);

	/**
	 * Exchange the contents of the vector with the contents of this XdmfArray.  No copy is made.  The internal arrays are swapped.
	 *
	 * @param array a smart pointer to a vector to exchange values with.
	 */
	template<typename T>
	bool swap(boost::shared_ptr<std::vector<T> > array);

	/**
	 * Exchange the contents of an XdmfArray with the contents of this XdmfArray.  No copy is made.  The internal arrays are swapped.
	 *
	 * @param array a smart pointer to a vector to exchange values with.
	 * @return bool whether the swap was successful.
	 */
	void swap(boost::shared_ptr<XdmfArray> array);

	/**
	 *
	 */
	virtual void write(boost::shared_ptr<XdmfVisitor>) const;

protected:

	XdmfArray();
	virtual ~XdmfArray();

private:

	// Variant Visitor Operations
	class Clear;
	class CopyArrayValues;

	template<typename T>
	class CopyValues;

	class GetHDF5Type;
	class GetPrecision;
	class GetSize;
	class GetType;
	class GetValuesPointer;
	class GetValuesString;
	class InternalizeArrayPointer;
	class NewArray;

	template<typename T>
	class Resize;


	struct NullDeleter;

	/**
	 * After setValues(const T * const array) is called, XdmfArray stores a pointer that is not allowed to be modified through
	 * the XdmfArray API.  If the user desires to modify the contents of the pointer, they must do so without calling any
	 * non-const functions of XdmfArray.  If they do call non-const functions of XdmfArray, we try to accommodate by copying
	 * the array pointer into internal data structures.
	 */
	void internalizeArrayPointer();

	/**
	 * Release references to internal data.
	 */
	void releaseArray();

	/**
	 * Release references to held array pointer internal data;
	 */
	void releaseArrayPointer();

	typedef boost::variant<
		boost::shared_ptr<std::vector<char> >,
		boost::shared_ptr<std::vector<short> >,
		boost::shared_ptr<std::vector<int> >,
		boost::shared_ptr<std::vector<long> >,
		boost::shared_ptr<std::vector<float> >,
		boost::shared_ptr<std::vector<double> >,
		boost::shared_ptr<std::vector<unsigned char> >,
		boost::shared_ptr<std::vector<unsigned short> >,
		boost::shared_ptr<std::vector<unsigned int> > > ArrayVariant;

	  typedef boost::variant<
	    boost::shared_array<const char>,
	    boost::shared_array<const short>,
	    boost::shared_array<const int>,
	    boost::shared_array<const long>,
	    boost::shared_array<const float>,
	    boost::shared_array<const double>,
	    boost::shared_array<const unsigned char>,
	    boost::shared_array<const unsigned short>,
	    boost::shared_array<const unsigned int> > ArrayPointerVariant;

	ArrayVariant mArray;
	ArrayPointerVariant mArrayPointer;
	int mArrayPointerNumValues;
	bool mHaveArray;
	bool mHaveArrayPointer;
};

#include "XdmfArray.tpp"

#endif /* XDMFARRAY_HPP_ */
