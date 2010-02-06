// Kenneth Leiter
// Xdmf Smart Pointer Test

#ifndef XDMFVISITOR_HPP_
#define XDMFVISITOR_HPP_

// Forward Declarations

// Includes
#include "XdmfObject.hpp"


class XdmfVisitor : public XdmfObject {

public:

	XdmfNewMacro(XdmfVisitor);
	//virtual void visit(boost::shared_ptr<XdmfItem>);

	const std::string printSelf() const;

protected:

	XdmfVisitor();
	virtual ~XdmfVisitor();

private:

  XdmfVisitor(const XdmfVisitor&);  // Not implemented.
  void operator=(const XdmfVisitor&);  // Not implemented.

};

#endif /* XDMFVISITOR_HPP_ */
