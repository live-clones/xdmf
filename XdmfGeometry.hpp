#ifndef XDMFGEOMETRY_HPP_
#define XDMFGEOMETRY_HPP_

// Includes
#include "XdmfDataItem.hpp"
#include "XdmfGeometryType.hpp"

/**
 * @brief Handles the coordinate positions of points in an XdmfGrid.
 *
 * XdmfGeometry is a required part of an XdmfGrid.  It stores the coordinate values
 * of all points contained in an XdmfGrid.  XdmfGeometry contains an XdmfGeometryType property
 * which should be set that specifies the types of coordinate values stored.
 */
class XdmfGeometry : public XdmfDataItem {

public:

	XdmfNewMacro(XdmfGeometry);
	LOKI_DEFINE_VISITABLE(XdmfGeometry, XdmfDataItem)

	/**
	 * Get the XdmfGeometryType associated with this geometry.
	 *
	 * @return XdmfGeometryType of this geometry.
	 */
	XdmfGeometryType getGeometryType() const;

	std::map<std::string, std::string> getItemProperties() const;

	std::string getItemTag() const;

	/**
	 * Set the XdmfGeometryType associated with this geometry.
	 *
	 * @param geometryType the XdmfGeometryType to set.
	 */
	void setGeometryType(const XdmfGeometryType & geometryType);

protected:

	XdmfGeometry();
	virtual ~XdmfGeometry();

private:

	XdmfGeometry(const XdmfGeometry & geometry);  // Not implemented.
	void operator=(const XdmfGeometry & geometry);  // Not implemented.

	XdmfGeometryType mGeometryType;
	int mNumberPoints;
};

#endif /* XDMFGEOMETRY_HPP_ */
