#ifndef XDMFWRITER_HPP_
#define XDMFWRITER_HPP_

// Forward Declarations
class XdmfArray;
class XdmfAttribute;
class XdmfDomain;
class XdmfGeometry;
class XdmfGrid;
class XdmfTopology;

// Includes
#include <hdf5.h>
#include <libxml/tree.h>
#include <sstream>
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
	public Loki::Visitor<XdmfArray>,
	public Loki::Visitor<XdmfAttribute>,
	public Loki::Visitor<XdmfDomain>,
	public Loki::Visitor<XdmfGeometry>,
	public Loki::Visitor<XdmfGrid>,
	public Loki::Visitor<XdmfTopology> {

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
	 * Write an XdmfAttribute to disk
	 *
	 * @param attribute a pointer to an XdmfAttribute to write to disk.
	 * @param visitor a smart pointer to this XdmfVisitor --- aids in grid traversal.
	 */
	void visit(XdmfAttribute & attribute, boost::shared_ptr<Loki::BaseVisitor> visitor);

	/**
	 * Write an XdmfDomain to disk
	 *
	 * @param domain a pointer to an XdmfDomain to write to disk.
	 * @param visitor a smart pointer to this XdmfVisitor --- aids in grid traversal.
	 */
	void visit(XdmfDomain & domain, boost::shared_ptr<Loki::BaseVisitor> visitor);

	/**
	 * Write an XdmfGeometry to disk
	 *
	 * @param geometry a pointer to an XdmfGeometry to write to disk.
	 * @param visitor a smart pointer to this XdmfVisitor --- aids in grid traversal.
	 */
	void visit(XdmfGeometry & geometry, boost::shared_ptr<Loki::BaseVisitor> visitor);

	/**
	 * Write an XdmfGrid to disk
	 *
	 * @param grid a pointer to an XdmfGrid to write to disk.
	 * @param visitor a smart pointer to this XdmfVisitor --- aids in grid traversal.
	 */
	void visit(XdmfGrid & grid, boost::shared_ptr<Loki::BaseVisitor> visitor);

	/**
	 * Write an XdmfTopology to disk
	 *
	 * @param topology a pointer to an XdmfTopology to write to disk.
	 * @param visitor a smart pointer to this XdmfVisitor --- aids in grid traversal.
	 */
	void visit(XdmfTopology & topology, boost::shared_ptr<Loki::BaseVisitor> visitor);

protected:

	XdmfWriter();
	virtual ~XdmfWriter();

private:

	XdmfWriter(const XdmfWriter & writer);  // Not implemented.
	void operator=(const XdmfWriter & writer);  // Not implemented.

	/**
	 * Uses the dataHierarchy to construct an appropriate hdf5 group name at the current point in writing.
	 *
	 * @return a string containing the hdf5 group name.
	 */
	std::string getHDF5GroupName();

	std::vector<std::string> dataHierarchy;
	hid_t hdf5Handle;
	std::string mHeavyFileName;
	unsigned int mLightDataLimit;
	xmlDocPtr xmlDocument;
	xmlNodePtr xmlCurrentNode;
};

#endif /* XDMFWRITER_HPP_ */
