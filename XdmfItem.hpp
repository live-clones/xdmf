/**
 * An XdmfItem replaces the XdmfElement class in the previous version of Xdmf.  An XdmfItem represents an item that can be
 * visited by an XdmfVisitor and have it's contents added to an xdmf file.  These include XdmfGrids, XdmfSets,
 * XdmfTopologies, etc.
 *
 * This is an abstract base class.
 */

#ifndef XDMFITEM_HPP_
#define XDMFITEM_HPP_

// Forward Declarations
class XdmfVisitor;

// Includes
#include <loki/Visitor.h>
#include "XdmfObject.hpp"

/**
 * @brief Base class of any object that is able to be added to an Xdmf structure.
 *
 * XdmfItem is an abstract base class.  It represents any item that can be visited and traversed by an XdmfVisitor
 * and have its contents added to an Xdmf file.
 */
class XdmfItem : public XdmfObject,
	public Loki::BaseVisitable<void> {

public:

	LOKI_DEFINE_VISITABLE_BASE()
	virtual void traverse(boost::shared_ptr<Loki::BaseVisitor> visitor);

protected:

	XdmfItem();
	virtual ~XdmfItem();

private:

	XdmfItem(const XdmfItem & item);  // Not implemented.
	void operator=(const XdmfItem & item);  // Not implemented.
};

#endif /* XDMFITEM_HPP_ */
