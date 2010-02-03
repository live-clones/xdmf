// Kenneth Leiter
// Xdmf Smart Pointer Test

#ifndef XDMFGRID_HPP_
#define XDMFGRID_HPP_

// Forward Declarations
class XdmfAttribute;
class XdmfAttributeVector;
class XdmfGeometry;
class XdmfTopology;

// Includes
#include "XdmfObject.hpp"

#include <vector>

class XdmfGrid : public XdmfObject {

public:

	XdmfNewMacro(XdmfGrid);

	// Get the XdmfGeometry for this grid
	XdmfGetValueMacro(Geometry, boost::shared_ptr<XdmfGeometry>);
	// Set the XdmfGeometry for this grid
	XdmfSetValueMacro(Geometry, boost::shared_ptr<XdmfGeometry>);

	// Get the XdmfTopology for this grid
	XdmfGetValueMacro(Topology, boost::shared_ptr<XdmfTopology>);
	// Set the XdmfTopology for this grid
	XdmfSetValueMacro(Topology, boost::shared_ptr<XdmfTopology>);

	virtual std::string printSelf() const;

protected:

	XdmfGrid();
	virtual ~XdmfGrid();

private:

  XdmfGrid(const XdmfGrid&);  // Not implemented.
  void operator=(const XdmfGrid&);  // Not implemented.

	boost::shared_ptr<XdmfGeometry> mGeometry;
	boost::shared_ptr<XdmfTopology> mTopology;
	std::vector<boost::shared_ptr<XdmfAttribute> > mAttributes;

};

#endif /* XDMFGRID_HPP_ */
