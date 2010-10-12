#ifndef XDMFHDF5CONTROLLER_HPP_
#define XDMFHDF5CONTROLLER_HPP_

// Includes
#include "XdmfHeavyDataController.hpp"

/**
 * @brief Couples an XdmfArray with HDF5 data stored on disk.
 *
 * Serves as an interface between data stored in XdmfArrays and data stored on disk.  When an Xdmf file is read from or
 * written to disk an XdmfHDF5Controller is attached to XdmfArrays.  This allows data to be released
 * from memory but still be accessible or have its location written to light data.
 */
class XdmfHDF5Controller : public XdmfHeavyDataController {

public:

	virtual ~XdmfHDF5Controller();

	/**
	 * Create a new controller for an hdf5 data set on disk.
	 */
	static boost::shared_ptr<XdmfHDF5Controller> New(const std::string & hdf5FilePath, const std::string & dataSetPath, const unsigned int size, const boost::shared_ptr<const XdmfArrayType> type);

	std::string getName() const;

	void read(XdmfArray * const array);

protected:

	XdmfHDF5Controller(const std::string & hdf5FilePath, const std::string & dataSetPath, const unsigned int size, const boost::shared_ptr<const XdmfArrayType> type);

private:

	XdmfHDF5Controller(const XdmfHDF5Controller & hdf5Controller);  // Not implemented.
	void operator=(const XdmfHDF5Controller & hdf5Controller);  // Not implemented.

};

#endif /* XDMFHDF5CONTROLLER_HPP_ */
