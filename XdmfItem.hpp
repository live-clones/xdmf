/**
 * An XdmfItem replaces the XdmfElement class in the previous version of Xdmf.  An XdmfElement represents and item that can be
 * visited by an XdmfLightDataVisitor and have it's contents added to an XdmfFile.  These include XdmfGrids, XdmfSets,
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

	//virtual const std::string printSelf() const = 0;
	virtual void write(boost::shared_ptr<XdmfVisitor>) const = 0;

protected:

	XdmfItem();
	virtual ~XdmfItem();

private:

  //XdmfItem(const XdmfItem&);  // Not implemented.
  //void operator=(const XdmfItem&);  // Not implemented.

};

#endif /* XDMFITEM_HPP_ */
