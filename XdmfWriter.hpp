#ifndef XDMFWRITER_HPP_
#define XDMFWRITER_HPP_

// Forward Declarations
class XdmfArray;

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

	XdmfNewMacro(XdmfWriter);

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

	XdmfWriter();
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
