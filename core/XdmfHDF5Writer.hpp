#ifndef XDMFHDF5WRITER_HPP_
#define XDMFHDF5WRITER_HPP_

// Forward Declarations
class XdmfArray;

// Includes
#include "XdmfVisitor.hpp"

/**
 * @brief Traverse the Xdmf graph and write heavy data stored in XdmfArrays to HDF5 on disk.
 *
 * XdmfHDF5Writer traverses an Xdmf graph structure and writes data stored in XdmfArrays to HDF5.
 * Writing begins by calling the accept() operation on any XdmfItem and supplying this writer
 * as the parameter.  The writer will write all XdmfArrays under the XdmfItem to an hdf5 file on
 * disk.  It will also attach an XdmfHDF5Controller to all XdmfArrays that it writes to disk.
 *
 * There are three modes of operation for this writer:
 * 	Default - All initialized XdmfArrays are written to new hdf5 datasets regardless of whether they
 * 		are attached to another hdf5 dataset on disk via an XdmfHDF5Controller.
 * 	Overwrite - If an initialized XdmfArray is attached to an hdf5 dataset via an XdmfHDF5Contoller the writer
 * 		will write values to that location, overwriting all previous written values.
 *  Append - If an initialized XdmfArray is attached to an hdf5 dataset via an XdmfHDF5Controller the writer
 *  	will append the values to the end of the dataset on disk.
 */
class XdmfHDF5Writer : public XdmfVisitor,
	public Loki::Visitor<XdmfArray> {

public:

	enum Mode {
		Default, Overwrite, Append
	};

	/**
	 * Construct XdmfHDF5Writer
	 *
	 * @param hdf5FilePath the location of the hdf5 file to output to on disk.
	 * @return new XdmfHDF5Writer
	 */
	static boost::shared_ptr<XdmfHDF5Writer> New(const std::string & hdf5FilePath);

	virtual ~XdmfHDF5Writer();

	/**
	 * Get the path to the hdf5 file on disk this writer is writing to.
	 *
	 * @return a std::string containing the path to the hdf5 file on disk this writer is writing to.
	 */
	std::string getFilePath() const;

	/**
	 * Get the Mode of operation for this writer.
	 *
	 * @return the Mode of operation for this writer.
	 */
	Mode getMode() const;

	/**
	 * Set the mode of operation for this writer.
	 *
	 * @param mode the Mode of operation for this writer.
	 */
	void setMode(const Mode mode);

	/**
	 * Write an XdmfArray to HDF5.
	 *
	 * @param array an XdmfArray to write to HDF5.
	 * @param visitor a smart pointer to this visitor --- aids in grid traversal.
	 */
	void visit(XdmfArray & array, const boost::shared_ptr<XdmfBaseVisitor> visitor);

protected:

	XdmfHDF5Writer(const std::string & hdf5FilePath);

private:

	/**
	 * PIMPL
	 */
	class XdmfHDF5WriterImpl;

	XdmfHDF5Writer(const XdmfHDF5Writer & hdf5Writer);  // Not implemented.
	void operator=(const XdmfHDF5Writer & hdf5Writer);  // Not implemented.

	XdmfHDF5WriterImpl * const mImpl;
};

#endif /* XDMFHDF5WRITER_HPP_ */
