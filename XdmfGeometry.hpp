/*
 * XdmfGeometry.hpp
 *
 *  Created on: Jan 25, 2010
 *      Author: kleiter
 */

#ifndef XDMFGEOMETRY_HPP_
#define XDMFGEOMETRY_HPP_

class XdmfDataItem;

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
	 * Get the name of the XdmfGeometryType associated with this Geometry.
	 *
	 * @return std::string containing name of the GeometryType.
	 */
	std::string getGeometryTypeAsString() const;

	/**
	 * Get the number of dimensions associated with this Geometry.
	 *
	 * @return int giving number of dimensions: i.e. XY = 2, XYZ = 3.
	 */
	int getDimensions() const;

	/**
	 * Get the point data associated with this Geometry;
	 */
	boost::shared_ptr<XdmfDataItem> getData();

	/**
	 * Get the data associated with this Geometry (const version)
	 */
	boost::shared_ptr<const XdmfDataItem> getData() const;

	/**
	 * Set the point data associated with this Geometry
	 */
	void setData(boost::shared_ptr<XdmfDataItem> pointData);

	virtual std::string printSelf() const;

	virtual void write(boost::shared_ptr<XdmfVisitor> visitor) const;

protected:

	XdmfGeometry();
	virtual ~XdmfGeometry();

private:

	XdmfGeometry(const XdmfGeometry&);  // Not implemented.
	void operator=(const XdmfGeometry&);  // Not implemented.

	XdmfGeometryType mGeometryType;
	boost::shared_ptr<XdmfDataItem> mPointData;

};

#endif /* XDMFGEOMETRY_HPP_ */
