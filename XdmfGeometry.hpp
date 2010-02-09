/*
 * XdmfGeometry.hpp
 *
 *  Created on: Jan 25, 2010
 *      Author: kleiter
 */

#ifndef XDMFGEOMETRY_HPP_
#define XDMFGEOMETRY_HPP_

// Includes
#include "XdmfItem.hpp"
#include "XdmfGeometryType.hpp"

class XdmfGeometry : public XdmfItem {

public:

	XdmfNewMacro(XdmfGeometry);

	/**
	 * Get the XdmfGeometryType associated with this Geometry.
	 *
	 * @return XdmfGeometryType.
	 */
	XdmfGeometryType getGeometryType() const;

	/**
	 * Set the XdmfGeometryType associated with this Geometry.
	 *
	 * @param XdmfGeometryType to set.
	 */
	void setGeometryType(const XdmfGeometryType&);

	/**
	 * Get the name of the GeometryType associated with this Geometry.
	 *
	 * @return std::string containing name of the GeometryType.
	 */
	std::string getGeometryTypeAsString() const;

	/**
	 * Get the number of dimensions for this Geometry.
	 *
	 * @return int giving number of dimensions: i.e. XY = 2, XYZ = 3.
	 */
	int getDimensions() const;

	virtual std::string printSelf() const;

	virtual void write(boost::shared_ptr<XdmfVisitor> visitor) const;

protected:

	XdmfGeometry();
	virtual ~XdmfGeometry();

private:

	XdmfGeometry(const XdmfGeometry&);  // Not implemented.
	void operator=(const XdmfGeometry&);  // Not implemented.

	XdmfGeometryType mGeometryType;

};

#endif /* XDMFGEOMETRY_HPP_ */
