#ifndef XDMFWRITER_HPP_
#define XDMFWRITER_HPP_

// Forward Declarations
class XdmfArray;
class XdmfHDF5Writer;

// Includes
#include "XdmfVisitor.hpp"

/**
 * @brief Traverse the Xdmf graph and write light and heavy data stored to disk.
 *
 * XdmfWriter visits each node of an Xdmf graph structure and writes the data to disk.  Writing begins by calling the
 * accept() operation on any XdmfItem and supplying this writer as the parameter.  The XdmfItem as well as all children
 * attached to the XdmfItem are written to disk.  Heavy data is written to HDF5 format using the XdmfHDF5Writer and light
 * data is written to XML.
 */
class XdmfWriter : public XdmfVisitor,
	public Loki::Visitor<XdmfArray> {

public:

	template <typename T> friend void boost::checked_delete(T * x);

	/**
	 * Create a new XdmfWriter to write Xdmf data to disk.  This will create its own hdf5 writer based on the xmlFileName.
	 * For example, if supplied "output.xmf" the created hdf5 writer would write to file "output.h5".
	 *
	 * @param xmlFilePath the path to the xml file to write to.
	 * @return the new XdmfWriter.
	 */
	static boost::shared_ptr<XdmfWriter> New(const std::string & xmlFilePath)
	{
		boost::shared_ptr<XdmfWriter> p(new XdmfWriter(xmlFilePath));
		return p;
	}

	/**
	 * Create a new XdmfWriter to write Xdmf data to disk.  This will utilize the supplied hdf5Writer to write any
	 * heavy data to disk.
	 *
	 * @param xmlFilePath the path to the xml file to write to.
	 * @param hdf5Writer the heavy data writer to use when writing.
	 * @return the new XdmfWriter.
	 */
	static boost::shared_ptr<XdmfWriter> New(const std::string & xmlFilePath, boost::shared_ptr<XdmfHDF5Writer> hdf5Writer)
	{
		boost::shared_ptr<XdmfWriter> p(new XdmfWriter(xmlFilePath, hdf5Writer));
		return p;
	}

	/**
	 * Get the number of values that this writer writes to light data (XML) before switching to a heavy data format.
	 *
	 * @return an unsigned int containing the number of values.
	 */
	unsigned int getLightDataLimit() const;

	/**
	 * Set the number of values that this writer writes to light data (XML) before switching to a heavy data format.
	 *
	 * @param numValues an unsigned int containing the number of values.
	 */
	void setLightDataLimit(unsigned int numValues);

	/**
	 * Write an XdmfArray to disk
	 *
	 * @param array an XdmfArray to write to disk.
	 * @param visitor a smart pointer to this visitor --- aids in grid traversal.
	 */
	void visit(XdmfArray & array, boost::shared_ptr<Loki::BaseVisitor> visitor);

	/**
	 * Write an XdmfItem to disk
	 *
	 * @param item an XdmfItem to write to disk.
	 * @param visitor a smart pointer to this visitor --- aids in grid traversal.
	 */
	void visit(XdmfItem & item, boost::shared_ptr<Loki::BaseVisitor> visitor);

protected:

	XdmfWriter(const std::string & xmlFilePath);
	XdmfWriter(const std::string & xmlFilePath, boost::shared_ptr<XdmfHDF5Writer> hdf5Writer);
	virtual ~XdmfWriter();

private:

	/**
	 * PIMPL
	 */
	class XdmfWriterImpl;

	XdmfWriter(const XdmfWriter & writer);  // Not implemented.
	void operator=(const XdmfWriter & writer);  // Not implemented.

	XdmfWriterImpl * mImpl;
};

#endif /* XDMFWRITER_HPP_ */
