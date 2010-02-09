// Kenneth Leiter
// Xdmf Smart Pointer Test

#ifndef XDMFVISITOR_HPP_
#define XDMFVISITOR_HPP_

// Forward Declarations
class XdmfAttribute;
class XdmfGeometry;
class XdmfGrid;
class XdmfTopology;

// Includes
#include <sstream>
#include "XdmfObject.hpp"

class XdmfVisitor : public XdmfObject {

public:

	XdmfNewMacro(XdmfVisitor);

	virtual void visit(const XdmfAttribute * const attribute);

	virtual void visit(const XdmfGeometry * const geometry);

	virtual void visit(const XdmfGrid * const grid);

	virtual void visit(const XdmfTopology * const topology);

	const std::string printSelf() const;

protected:

	XdmfVisitor();
	virtual ~XdmfVisitor();

private:

	XdmfVisitor(const XdmfVisitor&);  // Not implemented.
	void operator=(const XdmfVisitor&);  // Not implemented.

	int mTabIndex;
	std::stringstream xmlData;



};

#endif /* XDMFVISITOR_HPP_ */
