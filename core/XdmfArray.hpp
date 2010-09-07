#ifndef XDMFARRAY_HPP_
#define XDMFARRAY_HPP_

// Forward Declarations
class XdmfArrayType;
class XdmfHDF5Controller;

// Includes
#include "XdmfItem.hpp"
#include <boost/shared_array.hpp>
#include <boost/variant.hpp>

/**
 * @brief Provides a single interface for storing a variety of data types.
 *
 * XdmfArray stores data values that are read in from or will be written to heavy data on disk. The
 * data type stored is determined by the type initially inserted into the XdmfArray.  XdmfArray
 * allows for insertion and retrieval of data in two fundamental ways:
 *
 * By Copy:
 *
 *	getValue
 * 	getValues
 *	insert
 *
 * 	XdmfArray stores its own copy of the data.  Modifications to the data stored in the XdmfArray will
 * 	not change values stored in the original array.
 *
 * By Shared Reference:
 *
 *	getValuesInternal
 * 	setValuesInternal
 *
 * 	XdmfArray shares a reference to the data.  No copy is made. XdmfArray holds a shared pointer to the original
 * 	data.  Modifications to the data stored in the XdmfArray also causes modification to values stored in the original
 * 	array.
 *
 * Xdmf supports the following datatypes:
 * 	Int8
 * 	Int16
 * 	Int32
 * 	Int64
 * 	Float32
 * 	Float64
 * 	UInt8
 * 	UInt16
 * 	UInt32
 */
class XdmfArray : public XdmfItem {

public:

	/**
	 * Create a new XdmfArray.
	 *
	 * @return constructed XdmfArray.
	 */
	static boost::shared_ptr<XdmfArray> New();

	virtual ~XdmfArray();

	LOKI_DEFINE_VISITABLE(XdmfArray, XdmfItem)
	static const std::string ItemTag;

	/**
	 * Remove all values from this array.
	 */
	void clear();

	/**
	 * Remove a value from this array.
	 */
	void erase(const unsigned int index);

	/**
	 * Get the data type of this array.
	 *
	 * @return a string containing the Xdmf data type for the array, this is one of
	 *      Char, Short, Int, Float, UChar, UShort, UInt.
	 */
	boost::shared_ptr<const XdmfArrayType> getArrayType() const;

	/**
	 * Get the capacity of this array (the number of values this array can store without reallocation).
	 *
	 * @return the capacity of this array.
	 */
	unsigned int getCapacity() const;

	std::map<std::string, std::string> getItemProperties() const;

	std::string getItemTag() const;

	/**
	 * Get the hdf5 controller attached to this XdmfArray.
	 *
	 * @return the hdf5 controller attached to this XdmfArray.
	 */
	boost::shared_ptr<XdmfHDF5Controller> getHDF5Controller();

	/**
	 * Get the hdf5 controller attached to this XdmfArray (const version).
	 *
	 * @return the hdf5 controller attached to this XdmfArray.
	 */
	boost::shared_ptr<const XdmfHDF5Controller> getHDF5Controller() const;

	/**
	 * Get the name of the array.
	 *
	 * @return a string containing the name of the array.
	 */
	std::string getName() const;

	/**
	 * Get the number of values stored in this array.
	 *
	 * @return the number of values stored in this array.
	 */
	unsigned int getSize() const;

	/**
	 * Get a copy of a single value stored in this array.
	 *
	 * @return the requested value.
	 */
	template <typename T>
	T getValue(const unsigned int index) const;

	/**
	 * Get a copy of the values stored in this array
	 *
	 * @param startIndex the index in this XdmfArray to begin copying from.
	 * @param valuesPointer a pointer to an array to copy into.
	 * @param numValues the number of values to copy.
	 * @param arrayStride number of values to stride in this XdmfArray between each copy.
	 * @param valuesStride number of values to stride in the pointer between each copy.
	 */
	template <typename T>
	void getValues(const unsigned int startIndex, T * const valuesPointer, const unsigned int numValues = 1, const unsigned int arrayStride = 1, const unsigned int valuesStride = 1) const;

	/**
	 * Get a smart pointer to the internal values stored in this array.
	 *
	 * @return a smart pointer to the internal vector of values stored in this array.
	 */
	template <typename T>
	boost::shared_ptr<std::vector<T> > getValuesInternal();

	/**
	 * Get a pointer to the internal values stored in this array.
	 *
	 * @return a void pointer to the first value stored in this array.
	 */
	void * getValuesInternal();

	/**
	 * Get a pointer to the internal values stored in this array (const version).
	 *
	 * @return a void pointer to the first value stored in this array.
	 */
	const void * getValuesInternal() const;

	/**
	 * Get the values stored in this array as a string.
	 *
	 * @return a string containing the contents of the array.
	 */
	std::string getValuesString() const;

	/**
	 * Initializes the array to contain an empty container of a particular type.
	 *
	 * @param size the number of values in the initialized array.
	 *
	 * @return a smart pointer to the internal vector of values initialized in this array.
	 */
	template <typename T>
	boost::shared_ptr<std::vector<T> > initialize(const unsigned int size = 0);

	/**
	 * Initializes the array to contain an empty container of a particular XdmfArrayType.
	 *
	 * @param arrayType the type of array to initialize.
	 * @param size the number of values in the initialized array.
	 */
	void initialize(const boost::shared_ptr<const XdmfArrayType> arrayType, const unsigned int size = 0);

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
	void insert(const unsigned int startIndex, const boost::shared_ptr<const XdmfArray> values, const unsigned int valuesStartIndex= 0, const unsigned int numValues = 1, const unsigned int arrayStride = 1, const unsigned int valuesStride = 1);

	/**
	 * Copy values into this array.
	 *
	 * @param startIndex the index in this XdmfArray to begin insertion.
	 * @param valuesPointer a pointer to the values to copy into this XdmfArray.
	 * @param numValues the number of values to copy into this array.
	 * @param arrayStride number of values to stride in this XdmfArray between each copy.
	 * @param valuesStride number of values to stride in the pointer between each copy.
	 */
	template<typename T>
	void insert(const unsigned int startIndex, const T * const valuesPointer, const unsigned int numValues = 1, const unsigned int arrayStride = 1, const unsigned int valuesStride = 1);

	/**
	 * Returns whether the array is initialized (contains values in memory).
	 */
	bool isInitialized() const;

	/**
	 * Copy a value to the back of this array
	 */
	template <typename T>
	void pushBack(const T & value);

	/**
	 * Read data from disk into memory.
	 */
	void read();

	/**
	 * Release all data currently held in memory.
	 */
	void release();

	/**
	 * Set the capacity of the XdmfArray to at least size.
	 *
	 * @param size the capacity to set this XdmfArray to.
	 */
	void reserve(const unsigned int size);

	/**
	 * Resizes the XdmfArray to contain numValues.  If numValues is larger than the current size, append values to the end equal
	 * to val.  If numValues is less than the current size, values at indices larger than numValues are removed.
	 *
	 * @param numValues the number of values to resize this array to.
	 * @param value the number to initialize newly created values to, if needed.
	 */
	template<typename T>
	void resize(const unsigned int numValues, const T & value = 0);

	/**
	 * Attach an hdf5 controller to this XdmfArray.
	 *
	 * @param hdf5Controller the hdf5 controller to attach to this XdmfArray.
	 */
	void setHDF5Controller(const boost::shared_ptr<XdmfHDF5Controller> hdf5Controller);

	/**
	 * Set the name of the array.
	 *
	 * @param name a string containing the name to set.
	 */
	void setName(const std::string & name);

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
	void setValuesInternal(const T * const arrayPointer, const unsigned int numValues, const bool transferOwnership = 0);

	/**
	 * Sets the values of this array to the values stored in the vector.  No copy is made.  The caller of this method retains
	 * ownership of the data and must ensure that the array is still valid for the entire time Xdmf needs it.
	 *
	 * @param array a vector to store in this array.
	 * @param transferOwnership whether to transfer responsibility for deletion of the array to XdmfArray.
	 */
	template<typename T>
	void setValuesInternal(std::vector<T> & array, const bool transferOwnership = 0);

	/**
	 * Sets the values of this array to the values stored in the vector.  No copy is made.  This array shares ownership with
	 * other references to the smart pointer.
	 *
	 * @param array a smart pointer to a vector to store in this array.
	 */
	template<typename T>
	void setValuesInternal(const boost::shared_ptr<std::vector<T> > array);

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
	bool swap(const boost::shared_ptr<std::vector<T> > array);

	/**
	 * Exchange the contents of an XdmfArray with the contents of this XdmfArray.  No copy is made.  The internal arrays are swapped.
	 *
	 * @param array a smart pointer to a vector to exchange values with.
	 * @return bool whether the swap was successful.
	 */
	void swap(const boost::shared_ptr<XdmfArray> array);

protected:

	XdmfArray();
	virtual void populateItem(const std::map<std::string, std::string> & itemProperties, std::vector<boost::shared_ptr<XdmfItem> > & childItems, const XdmfCoreReader * const reader);

private:

	XdmfArray(const XdmfArray & array);  // Not implemented.
	void operator=(const XdmfArray & array);  // Not implemented.

	// Variant Visitor Operations
	class Clear;
	class Erase;
	class GetArrayType;
	class GetCapacity;
	class GetHDF5Type;

	template <typename T>
	class GetValues;

	class GetValuesPointer;
	class GetValuesString;

	template <typename T>
	class Insert;

	class InsertArray;
	class InternalizeArrayPointer;
	class NewArray;
	struct NullDeleter;

	template <typename T>
	class PushBack;

	class Reserve;

	template <typename T>
	class Resize;

	class Size;

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
	unsigned int mArrayPointerNumValues;
	bool mHaveArray;
	bool mHaveArrayPointer;
	boost::shared_ptr<XdmfHDF5Controller> mHDF5Controller;
	std::string mName;
	unsigned int mTmpReserveSize;
};

#include "XdmfArray.tpp"

#endif /* XDMFARRAY_HPP_ */
