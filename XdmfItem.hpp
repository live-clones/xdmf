/**
 * An XdmfItem replaces the XdmfElement class in the previous version of Xdmf.  An XdmfItem represents an item that can be
 * visited by an XdmfVisitor and have it's contents added to an xdmf file.  These include XdmfGrids, XdmfSets,
 * XdmfTopologies, etc.
 *
 * This is an abstract base class.
 */

#ifndef XDMFITEM_HPP_
#define XDMFITEM_HPP_

class XdmfVisitor;

// Includes
#include "XdmfObject.hpp"

class XdmfItem : public XdmfObject {

public:

	virtual void traverse(boost::shared_ptr<XdmfVisitor> visitor) const;
	virtual void write(boost::shared_ptr<XdmfVisitor> visitor) const = 0;

protected:

	XdmfItem();
	virtual ~XdmfItem();

private:

	XdmfItem(const XdmfItem&);  // Not implemented.
	void operator=(const XdmfItem&);  // Not implemented.

};

#endif /* XDMFITEM_HPP_ */
