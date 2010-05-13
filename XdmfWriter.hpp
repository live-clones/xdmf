#ifndef XDMFWRITER_HPP_
#define XDMFWRITER_HPP_

// Forward Declarations
class XdmfArray;

// Includes
#include <hdf5.h>
#include <libxml/tree.h>
#include <vector>
#include "XdmfVisitor.hpp"

/**
 * @brief Traverse the Xdmf graph and write data stored to disk.
 *
 * XdmfWriter visits each node of an Xdmf graph structure and writes the data to disk.  Writing begins by calling the
 * accept() operation on any XdmfItem and supplying an XdmfWriter as the parameter.
 * The XdmfItem as well as all children attached to the XdmfItem are written to disk.
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
	 * @param array a pointer to an XdmfArray to write to disk.
	 * @param visitor a smart pointer to this XdmfVisitor --- aids in grid traversal.
	 */
	void visit(XdmfArray & array, boost::shared_ptr<Loki::BaseVisitor> visitor);

	/**
	 * Write an XdmfItem to disk
	 *
	 * @param item a pointer to an XdmfItem to write to disk.
	 * @param visitor a smart pointer to this XdmfVisitor --- aids in grid traversal.
	 */
	void visit(XdmfItem & item, boost::shared_ptr<Loki::BaseVisitor> visitor);

protected:

	XdmfWriter();
	virtual ~XdmfWriter();

private:

	XdmfWriter(const XdmfWriter & writer);  // Not implemented.
	void operator=(const XdmfWriter & writer);  // Not implemented.

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

	std::vector<std::string> mDataHierarchy;
	hid_t mHDF5Handle;
	std::string mHeavyFileName;
	unsigned int mLightDataLimit;
	xmlDocPtr mXMLDocument;
	xmlNodePtr mXMLCurrentNode;
};

#endif /* XDMFWRITER_HPP_ */
