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
	public Loki::Visitor<XdmfArray, std::string> {

public:

	XdmfNewMacro(XdmfHDF5Writer);

	/**
	 * Pop a string from the data hierarchy.  This should be called whenever an XdmfItem is finished
	 * traversal, including in light data writers that work in conjunction with this heavy data writer.
	 */
	void popDataHierarchy();

	/**
	 * Push a string associated with this XdmfItem to the dataHierarchy.  This should be called
	 * whenever an XdmfItem is traversed, including in light data writers that work in conjunction
	 * with this heavy data writer.  The data hierarchy ensures that HDF5 datasets are created at
	 * correct locations in the hdf5 file.  This string is either the item property "Name" or the
	 * item tag.
	 *
	 * @param item the XdmfItem being traversed.
	 */
	void pushDataHierarchy(const XdmfItem & item);

	/**
	 * Write an XdmfArray to HDF5.
	 *
	 * @param array an XdmfArray to write to HDF5.
	 * @param visitor a smart pointer to this visitor --- aids in grid traversal.
	 *
	 * @return a string containing the path to the hdf5 dataset written.
	 */
	std::string visit(XdmfArray & array, boost::shared_ptr<Loki::BaseVisitor> visitor);

	/**
	 * Traverse an XdmfItem structure in order to find XdmfArrays to write to HDF5.
	 *
	 * @param item an XdmfItem to traverse.
	 * @param visitor a smart pointer to this visitor --- aids in grid traversal.
	 */
	void visit(XdmfItem & item, boost::shared_ptr<Loki::BaseVisitor> visitor);

protected:

	XdmfHDF5Writer();
	virtual ~XdmfHDF5Writer();

private:

	/**
	 * PIMPL
	 */
	class XdmfHDF5WriterImpl;

	XdmfHDF5Writer(const XdmfHDF5Writer & hdf5Writer);  // Not implemented.
	void operator=(const XdmfHDF5Writer & hdf5Writer);  // Not implemented.

	/**
	 * Create a new HDF5 group if needed based on the dataHierarchy.  This is a recursive function used by getHDF5GroupHandle to construct
	 * new hdf5 groups.
	 *
	 * @param groupPath the current place in the dataHierarchy being processed.
	 * @param index the index in the dataHierarchy being processed.
	 *
	 * @return a string containing the path to the created group.
	 */
	std::string createHDF5Group(std::stringstream & groupPath, int index = 0);

	/**
	 * Get a handle to a hdf5 group to write into.  Uses the dataHierarchy to construct an appropriate hdf5 group name
	 * at the current point in writing.
	 *
	 * @return a string containing the path to the created group.
	 */
	std::string getHDF5GroupHandle();

	XdmfHDF5WriterImpl * mImpl;
};

#endif /* XDMFHDF5WRITER_HPP_ */
