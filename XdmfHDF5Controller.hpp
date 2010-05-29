#ifndef XDMFHDF5CONTROLLER_HPP_
#define XDMFHDF5CONTROLLER_HPP_

// Forward Declarations
class XdmfArray;
class XdmfArrayType;

// Includes
#include "XdmfObject.hpp"

/**
 * @brief Couples an XdmfArray with HDF5 data stored on disk.
 *
 * Serves as an interface between data stored in XdmfArrays and data stored on disk.  When an Xdmf file is read from or
 * written to disk an XdmfHDF5Controller is attached to XdmfArrays.  This allows data to be released
 * from memory but still be accessible or have its location written to light data.
 */
class XdmfHDF5Controller : public XdmfObject {

public:

	template <typename T> friend void boost::checked_delete(T * x);

	/**
	 * Create a new controller for an hdf5 data set on disk.
	 */
	static boost::shared_ptr<XdmfHDF5Controller> New(const std::string & dataSetPath, const unsigned int size, const boost::shared_ptr<const XdmfArrayType> type)
	{
		boost::shared_ptr<XdmfHDF5Controller> p(new XdmfHDF5Controller(dataSetPath, size, type));
		return p;
	}

	/**
	 * Get the name of the data set owned by this controller.
	 *
	 * @return a std::string containing the name of the data set.
	 */
	std::string getDataSetName() const;

	/**
	 * Get the path to the hdf5 data set on disk owned by this controller.
	 *
	 * @return a std::string to the hdf5 data set location on disk.
	 */
	std::string getDataSetPath() const;

	/**
	 * Get the path to the hdf5 file on disk where the data set owned by this controller resides.
	 *
	 * @return a std::string containing the path to the hdf5 file.
	 */
	std::string getHDF5FilePath() const;

	/**
	 * Get the size of the hdf5 data set owned by this controller.
	 *
	 * @return a int containing the size of the hdf5 data set.
	 */
	unsigned int getSize() const;

	/**
	 * Get the array type of the hdf5 data set owned by this controller.
	 *
	 * @return an XdmfArrayType containing the array type of the hdf5 data set.
	 */
	boost::shared_ptr<const XdmfArrayType> getType() const;

	/**
	 * Read data owned by this controller on disk into the passed XdmfArray.
	 *
	 * @param array and XdmfArray to read data into.
	 */
	void read(XdmfArray * const array);

protected:

	XdmfHDF5Controller(const std::string & dataSetPath, const unsigned int size, const boost::shared_ptr<const XdmfArrayType> type);
	virtual ~XdmfHDF5Controller();

private:

	XdmfHDF5Controller(const XdmfHDF5Controller & hdf5Controller);  // Not implemented.
	void operator=(const XdmfHDF5Controller & hdf5Controller);  // Not implemented.

	std::string mDataSetName;
	std::string mHDF5FilePath;
	unsigned int mSize;
	boost::shared_ptr<const XdmfArrayType> mType;
};

#endif /* XDMFHDF5CONTROLLER_HPP_ */
