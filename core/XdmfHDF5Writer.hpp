#ifndef XDMFHDF5WRITER_HPP_
#define XDMFHDF5WRITER_HPP_

// Includes
#include "XdmfHeavyDataWriter.hpp"

/**
 * @brief Traverse the Xdmf graph and write heavy data stored in XdmfArrays to HDF5 on disk.
 *
 * XdmfHDF5Writer traverses an Xdmf graph structure and writes data stored in XdmfArrays to HDF5.
 * Writing begins by calling the accept() operation on any XdmfItem and supplying this writer
 * as the parameter.  The writer will write all XdmfArrays under the XdmfItem to an hdf5 file on
 * disk.  It will also attach an XdmfHDF5Controller to all XdmfArrays that it writes to disk.
 *
 * This writer supports all heavy data writing modes listed in XdmfHeavyDataWriter.
 */
class XdmfHDF5Writer : public XdmfHeavyDataWriter {

public:

	/**
	 * Construct XdmfHDF5Writer
	 *
	 * @param filePath the location of the hdf5 file to output to on disk.
	 * @return new XdmfHDF5Writer
	 */
	static boost::shared_ptr<XdmfHDF5Writer> New(const std::string & filePath);

	virtual ~XdmfHDF5Writer();

	void visit(XdmfArray & array, const boost::shared_ptr<XdmfBaseVisitor> visitor);

protected:

	XdmfHDF5Writer(const std::string & filePath);

private:

	XdmfHDF5Writer(const XdmfHDF5Writer & hdf5Writer);  // Not implemented.
	void operator=(const XdmfHDF5Writer & hdf5Writer);  // Not implemented.

	int mDataSetId;
};

#endif /* XDMFHDF5WRITER_HPP_ */
