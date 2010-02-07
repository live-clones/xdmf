// Kenneth Leiter
// Xdmf Smart Pointer Test

#ifndef XDMFVISITOR_HPP_
#define XDMFVISITOR_HPP_

// Forward Declarations
class XdmfItem;

// Includes
#include <sstream>
#include "XdmfObject.hpp"

class XdmfVisitor : public XdmfObject {

public:

	XdmfNewMacro(XdmfVisitor);

	virtual void visit(const XdmfItem * const);

	const std::string printSelf() const;

protected:

	XdmfVisitor();
	virtual ~XdmfVisitor();

private:

	std::stringstream xmlData;

  XdmfVisitor(const XdmfVisitor&);  // Not implemented.
  void operator=(const XdmfVisitor&);  // Not implemented.

};

#endif /* XDMFVISITOR_HPP_ */
