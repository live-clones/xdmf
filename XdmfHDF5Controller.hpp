#ifndef XDMFHDF5CONTROLLER_HPP_
#define XDMFHDF5CONTROLLER_HPP_

// Forward Declarations
class XdmfArray;

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
	static boost::shared_ptr<XdmfHDF5Controller> New(const std::string & dataSetPath, const unsigned int precision, const unsigned int size, const std::string & type)
	{
		boost::shared_ptr<XdmfHDF5Controller> p(new XdmfHDF5Controller(dataSetPath, precision, size, type));
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
	 * Get the precision of the hdf5 data set owned by this controller.
	 *
	 * @return a int containing the precision of the hdf5 data set.
	 */
	unsigned int getPrecision() const;

	/**
	 * Get the size of the hdf5 data set owned by this controller.
	 *
	 * @return a int containing the size of the hdf5 data set.
	 */
	unsigned int getSize() const;

	/**
	 * Get the data type of the hdf5 data set owned by this controller.
	 *
	 * @return a std::string containing the name of the data type of the hdf5 data set.
	 */
	std::string getType() const;

	/**
	 * Read data owned by this controller on disk into the passed XdmfArray.
	 *
	 * @param array and XdmfArray to read data into.
	 */
	void read(XdmfArray * const array);

protected:

	XdmfHDF5Controller(const std::string & dataSetPath, const unsigned int precision, const unsigned int size, const std::string & type);
	virtual ~XdmfHDF5Controller();

private:

	XdmfHDF5Controller(const XdmfHDF5Controller & hdf5Controller);  // Not implemented.
	void operator=(const XdmfHDF5Controller & hdf5Controller);  // Not implemented.

	std::string mDataSetName;
	std::string mHDF5FilePath;
	unsigned int mPrecision;
	unsigned int mSize;
	std::string mType;
};

#endif /* XDMFHDF5CONTROLLER_HPP_ */
