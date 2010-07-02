#ifndef XDMFTOPOLOGYCONVERTER_HPP_
#define XDMFTOPOLOGYCONVERTER_HPP_

// Forward Declarations
class XdmfGrid;
class XdmfTopologyType;

// Includes
#include "XdmfObject.hpp"

/*!
 * @brief XdmfTopologyConverter converts an XdmfGrid to different topology types.  XdmfAttributes and XdmfSets attached to the XdmfGrid
 * are adjusted to remain valid for the new topology type.
 *
 * When converting from a lower order topology to a higher order topology type (e.g. Hexahedron to Hexahedron_64) additional points are added to the mesh, no additional elements are added.
 * When converting from a higher order topology to a lower order topology type (e.h. Hexahedron_64 to Hexahedron) elements are tesselated to form the new topology, no additional points are added.
 *
 * Currently supported conversions:
 * 	Hexahedron to Hexahedron_64
 * 	Hexahedron_64 to Hexahedron
 */
class XdmfTopologyConverter : public XdmfObject {

public:

	XdmfNewMacro(XdmfTopologyConverter);
	virtual ~XdmfTopologyConverter();

	/**
	 * Converts an XdmfGrid to a different topology type
	 *
	 * @param gridToConvert the XdmfGrid to convert to a different topology
	 * @param topologyType the XdmfTopologyType to convert to.
	 *
	 * @return the converted XdmfGrid.
	 */
	boost::shared_ptr<XdmfGrid> convert(const boost::shared_ptr<XdmfGrid> gridToConvert, const boost::shared_ptr<const XdmfTopologyType> topologyType) const;

protected:

	XdmfTopologyConverter();

private:

	XdmfTopologyConverter(const XdmfTopologyConverter & converter);  // Not implemented.
	void operator=(const XdmfTopologyConverter & converter);  // Not implemented.

	// Specific Topology Converters
	class HexahedronToHexahedron64;
	class Hexahedron64ToHexahedron;

	// Point comparator
	class PointComparison;
};

#endif /* XDMFPARTITIONER_HPP_ */
