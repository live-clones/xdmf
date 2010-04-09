// Kenneth Leiter
// Xdmf Smart Pointer Test

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

class XdmfVisitor : public XdmfObject {

public:

	XdmfNewMacro(XdmfVisitor);

	virtual void visit(const XdmfArray * const array);

	virtual void visit(const XdmfAttribute * const attribute);

	virtual void visit(const XdmfDomain * const domain);

	virtual void visit(const XdmfGeometry * const geometry);

	virtual void visit(const XdmfGrid * const grid);

	virtual void visit(const XdmfTopology * const topology);

	virtual std::string printSelf() const;

	/**
	 * Get the number of values that this visitor writes to light data (XML) before switching to a heavy data format.
	 *
	 * @return an int containing the number of values.
	 */
	int getLightDataLimit() const;

	/**
	 * Set the number of values that this visitor writes to light data (XML) before switching to a heavy data format.
	 *
	 * @param an int containing the number of values
	 */
	void setLightDataLimit(int numValues);

protected:

	XdmfVisitor();
	virtual ~XdmfVisitor();

private:

	XdmfVisitor(const XdmfVisitor&);  // Not implemented.
	void operator=(const XdmfVisitor&);  // Not implemented.

	std::string getHDF5GroupName();

	hid_t hdf5Handle;
	std::vector<std::string> dataHierarchy;
	std::string mHeavyFileName;
	int mLightDataLimit;
	xmlDocPtr xmlDocument;
	xmlNodePtr xmlCurrentNode;

};

#endif /* XDMFVISITOR_HPP_ */
