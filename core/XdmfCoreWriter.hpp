#ifndef XDMFCOREWRITER_HPP_
#define XDMFCOREWRITER_HPP_

// Forward Declarations
class XdmfArray;
class XdmfHDF5Writer;

// Includes
#include "XdmfVisitor.hpp"

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
class XdmfCoreWriter : public XdmfVisitor,
	public Loki::Visitor<XdmfArray> {

public:

	enum Mode {
		Default, DistributedHeavyData
	};

	virtual ~XdmfCoreWriter();

	/**
	 * Get the absolute path to the XML file on disk this writer is writing to.
	 *
	 * @return a std::string containing the path to the XML file on disk this writer is writing to.
	 */
	std::string getFilePath() const;

	/**
	 * Get the hdf5 writer that this XdmfWriter uses to write heavy data to disk.
	 *
	 * @return the requested hdf5 writer.
	 */
	boost::shared_ptr<XdmfHDF5Writer> getHDF5Writer();

	/**
	 * Get the hdf5 writer that this XdmfWriter uses to write heavy data to disk (const version).
	 *
	 * @return the requested hdf5 writer.
	 */
	boost::shared_ptr<const XdmfHDF5Writer> getHDF5Writer() const;

	/**
	 * Get the number of values that this writer writes to light data (XML) before switching to a heavy data format.
	 *
	 * @return an unsigned int containing the number of values.
	 */
	unsigned int getLightDataLimit() const;

	/**
	 * Get the Mode of operation for this writer.
	 *
	 * @return the Mode of operation for this writer.
	 */
	Mode getMode() const;

	/**
	 * Set the number of values that this writer writes to light data (XML) before switching to a heavy data format.
	 *
	 * @param numValues an unsigned int containing the number of values.
	 */
	void setLightDataLimit(const unsigned int numValues);

	/**
	 * Set the mode of operation for this writer.
	 *
	 * @param mode the Mode of operation for this writer.
	 */
	void setMode(const Mode mode);

	/**
	 * Write an XdmfArray to disk
	 *
	 * @param array an XdmfArray to write to disk.
	 * @param visitor a smart pointer to this visitor --- aids in grid traversal.
	 */
	virtual void visit(XdmfArray & array, const boost::shared_ptr<XdmfBaseVisitor> visitor);

	/**
	 * Write an XdmfItem to disk
	 *
	 * @param item an XdmfItem to write to disk.
	 * @param visitor a smart pointer to this visitor --- aids in grid traversal.
	 */
	virtual void visit(XdmfItem & item, const boost::shared_ptr<XdmfBaseVisitor> visitor);


protected:

	XdmfCoreWriter(const std::string & xmlFilePath);
	XdmfCoreWriter(const std::string & xmlFilePath, boost::shared_ptr<XdmfHDF5Writer> hdf5Writer);

	void moveToLastWrittenNode();
	void moveToParentNode();

private:

	/**
	 * PIMPL
	 */
	class XdmfCoreWriterImpl;

	XdmfCoreWriter(const XdmfCoreWriter & coreWriter);  // Not implemented.
	void operator=(const XdmfCoreWriter & coreWriter);  // Not implemented.

	XdmfCoreWriterImpl * mImpl;
};

#endif /* XDMFCOREWRITER_HPP_ */
