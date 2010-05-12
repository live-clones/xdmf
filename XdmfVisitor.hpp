#ifndef XDMFVISITOR_HPP_
#define XDMFVISITOR_HPP_

// Forward Declarations
class XdmfItem;

// Includes
#include <loki/Visitor.h>
#include "XdmfObject.hpp"

/**
 * @brief Perform an operation on an Xdmf graph structure.
 *
 * XdmfVisitor is an abstract base class for any operation that operates on an Xdmf graph structure.  These operations could
 * involve writing to disk or modifying the structure in some way.
 */
class XdmfVisitor : public XdmfObject,
	public Loki::BaseVisitor,
	public Loki::Visitor<XdmfItem> {

public:

	virtual void visit(XdmfItem & item, boost::shared_ptr<Loki::BaseVisitor> visitor);

protected:

	XdmfVisitor();
	virtual ~XdmfVisitor();

private:

	XdmfVisitor(const XdmfVisitor & visitor);  // Not implemented.
	void operator=(const XdmfVisitor & visitor);  // Not implemented.

};

#endif /* XDMFVISITOR_HPP_ */
