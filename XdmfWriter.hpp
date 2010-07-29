#ifndef XDMFWRITER_HPP_
#define XDMFWRITER_HPP_

// Forward Declarations
class XdmfSet;

// Includes
#include "XdmfCoreWriter.hpp"

/**
 * @brief Traverse the Xdmf graph and write light and heavy data stored to disk.
 *
 * XdmfWriter visits each node of an Xdmf graph structure and writes data to disk.  Writing begins by calling the
 * accept() operation on any XdmfItem and supplying this writer as the parameter.  The XdmfItem as well as all children
 * attached to the XdmfItem are written to disk.  Heavy data is written to HDF5 format using the XdmfHDF5Writer and light
 * data is written to XML.
 *
 * By default, the XdmfWriter writes all heavy data to a single heavy data file specified by the XdmfHDF5Writer.
 * If a dataset is encountered that resides in a different heavy data file on disk, the dataset is read from disk and written
 * to the new hdf5 file.  If this is undesired, the XdmfWriter can be set to DistributedHeavyData mode in which the writer
 * will automatically reference any hdf5 dataset even if it resides in a different file than the one currently being written to.
 * written.
 */
class XdmfWriter : public XdmfCoreWriter,
	public Loki::Visitor<XdmfSet> {

public:

	/**
	 * Create a new XdmfWriter to write Xdmf data to disk.  This will create its own hdf5 writer based on the xmlFileName.
	 * For example, if supplied "output.xmf" the created hdf5 writer would write to file "output.h5".
	 *
	 * @param xmlFilePath the path to the xml file to write to.
	 * @return the new XdmfWriter.
	 */
	static boost::shared_ptr<XdmfWriter> New(const std::string & xmlFilePath);

	/**
	 * Create a new XdmfWriter to write Xdmf data to disk.  This will utilize the supplied hdf5Writer to write any
	 * heavy data to disk.
	 *
	 * @param xmlFilePath the path to the xml file to write to.
	 * @param hdf5Writer the heavy data writer to use when writing.
	 * @return the new XdmfWriter.
	 */
	static boost::shared_ptr<XdmfWriter> New(const std::string & xmlFilePath, const boost::shared_ptr<XdmfHDF5Writer> hdf5Writer);

	virtual ~XdmfWriter();

	/**
	 * Write an XdmfSet to disk
	 *
	 * @param set an XdmfSet to write to disk.
	 * @param visitor a smart pointer to this visitor --- aids in grid traversal.
	 */
	void visit(XdmfSet & set, const boost::shared_ptr<XdmfBaseVisitor> visitor);

protected:

	XdmfWriter(const std::string & xmlFilePath);
	XdmfWriter(const std::string & xmlFilePath, boost::shared_ptr<XdmfHDF5Writer> hdf5Writer);

private:

	XdmfWriter(const XdmfWriter & writer);  // Not implemented.
	void operator=(const XdmfWriter & writer);  // Not implemented.
};

#endif /* XDMFWRITER_HPP_ */
