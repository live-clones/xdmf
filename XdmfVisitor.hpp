#ifndef XDMFVISITOR_HPP_
#define XDMFVISITOR_HPP_

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
#include "XdmfObject.hpp"

/**
 * @brief Traverses the Xdmf graph and writes the data stored to disk.
 *
 * XdmfVisitor visits each node of an Xdmf graph structure and writes the data to disk.  Xdmf files are written to disk
 * by performing a visitor operation.  Writing begins by calling the write() operation on any XdmfItem and supplying an
 * XdmfVisitor as the parameter.  The XdmfItem as well as all children attached to the XdmfItem are written to disk.
 */
class XdmfVisitor : public XdmfObject {

public:

	XdmfNewMacro(XdmfVisitor);

	/**
	 * Get the number of values that this visitor writes to light data (XML) before switching to a heavy data format.
	 *
	 * @return an unsigned int containing the number of values.
	 */
	unsigned int getLightDataLimit() const;

	virtual std::string printSelf() const;

	/**
	 * Set the number of values that this visitor writes to light data (XML) before switching to a heavy data format.
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
	virtual void visit(const XdmfArray * const array, boost::shared_ptr<XdmfVisitor> visitor);

	/**
	 * Write an XdmfAttribute to disk
	 *
	 * @param attribute a pointer to an XdmfAttribute to write to disk.
	 * @param visitor a smart pointer to this XdmfVisitor --- aids in grid traversal.
	 */
	virtual void visit(const XdmfAttribute * const attribute, boost::shared_ptr<XdmfVisitor> visitor);

	/**
	 * Write an XdmfDomain to disk
	 *
	 * @param domain a pointer to an XdmfDomain to write to disk.
	 * @param visitor a smart pointer to this XdmfVisitor --- aids in grid traversal.
	 */
	virtual void visit(const XdmfDomain * const domain, boost::shared_ptr<XdmfVisitor> visitor);

	/**
	 * Write an XdmfGeometry to disk
	 *
	 * @param geometry a pointer to an XdmfGeometry to write to disk.
	 * @param visitor a smart pointer to this XdmfVisitor --- aids in grid traversal.
	 */
	virtual void visit(const XdmfGeometry * const geometry, boost::shared_ptr<XdmfVisitor> visitor);

	/**
	 * Write an XdmfGrid to disk
	 *
	 * @param grid a pointer to an XdmfGrid to write to disk.
	 * @param visitor a smart pointer to this XdmfVisitor --- aids in grid traversal.
	 */
	virtual void visit(const XdmfGrid * const grid, boost::shared_ptr<XdmfVisitor> visitor);

	/**
	 * Write an XdmfTopology to disk
	 *
	 * @param topology a pointer to an XdmfTopology to write to disk.
	 * @param visitor a smart pointer to this XdmfVisitor --- aids in grid traversal.
	 */
	virtual void visit(const XdmfTopology * const topology, boost::shared_ptr<XdmfVisitor> visitor);

protected:

	XdmfVisitor();
	virtual ~XdmfVisitor();

private:

	XdmfVisitor(const XdmfVisitor & visitor);  // Not implemented.
	void operator=(const XdmfVisitor & visitor);  // Not implemented.

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

#endif /* XDMFVISITOR_HPP_ */
