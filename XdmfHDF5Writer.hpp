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
 * as the parameter.
 */
class XdmfHDF5Writer : public XdmfVisitor,
	public Loki::Visitor<XdmfArray> {

public:

	template <typename T> friend void boost::checked_delete(T * x);

	/**
	 * Construct XdmfHDF5Writer
	 *
	 * @param hdf5FilePath the location of the hdf5 file to output to on disk.
	 */
	static boost::shared_ptr<XdmfHDF5Writer> New(const std::string & hdf5FilePath)
	{
		boost::shared_ptr<XdmfHDF5Writer> p(new XdmfHDF5Writer(hdf5FilePath));
		return p;
	}

	/**
	 * Get the path to the last written data set written by this writer.
	 *
	 * @return the path to the last written data set.
	 */
	std::string getLastWrittenDataSet() const;

	/**
	 * Write an XdmfArray to HDF5.
	 *
	 * @param array an XdmfArray to write to HDF5.
	 * @param visitor a smart pointer to this visitor --- aids in grid traversal.
	 */
	void visit(XdmfArray & array, boost::shared_ptr<XdmfBaseVisitor> visitor);

protected:

	XdmfHDF5Writer(const std::string & hdf5FilePath);
	virtual ~XdmfHDF5Writer();

private:

	/**
	 * PIMPL
	 */
	class XdmfHDF5WriterImpl;

	// Variant Visitor Operation
	class GetHDF5Type;

	XdmfHDF5Writer(const XdmfHDF5Writer & hdf5Writer);  // Not implemented.
	void operator=(const XdmfHDF5Writer & hdf5Writer);  // Not implemented.

	XdmfHDF5WriterImpl * mImpl;
};

#endif /* XDMFHDF5WRITER_HPP_ */
