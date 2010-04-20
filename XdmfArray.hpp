#ifndef XDMFARRAY_HPP_
#define XDMFARRAY_HPP_

// Includes
#include "XdmfItem.hpp"
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
	void copyValues(int startIndex, boost::shared_ptr<XdmfArray> values, int valuesStartIndex= 0, int numValues = 1, int arrayStride = 1, int valuesStride = 1);

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
	void copyValues(int startIndex, T * valuesPointer, int numValues = 1, int arrayStride = 1, int valuesStride = 1);

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
	template <typename T>
	const boost::shared_ptr<const std::vector<T> > getValues() const;

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
	 * Sets the values of this array to the values stored in the vector.  No copy is made.  The caller of this method retains
	 * ownership of the data and must ensure that the array is still valid for the entire time Xdmf needs it.
	 *
	 * @param array a vector to store in this array.
	 */
	template<typename T>
	void setValues(std::vector<T> & array);

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

	ArrayVariant mArray;
	bool mInitialized;
};

#include "XdmfArray.tpp"

#endif /* XDMFARRAY_HPP_ */
