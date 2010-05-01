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
#include "XdmfObject.hpp"

/**
 * @brief Base class of any object that is able to be added to an Xdmf structure.
 *
 * XdmfItem is an abstract base class.  It represents any item that can be visited and traversed by an XdmfVisitor
 * and have its contents added to an Xdmf file.
 */
class XdmfItem : public XdmfObject {

public:

	virtual void accept(boost::shared_ptr<XdmfVisitor> visitor) const = 0;
	virtual void traverse(boost::shared_ptr<XdmfVisitor> visitor) const;

protected:

	XdmfItem();
	virtual ~XdmfItem();

private:

	XdmfItem(const XdmfItem&);  // Not implemented.
	void operator=(const XdmfItem&);  // Not implemented.

};

#endif /* XDMFITEM_HPP_ */
