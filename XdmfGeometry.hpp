/*
 * XdmfGeometry.hpp
 *
 *  Created on: Jan 25, 2010
 *      Author: kleiter
 */

#ifndef XDMFGEOMETRY_HPP_
#define XDMFGEOMETRY_HPP_

#include "XdmfItem.hpp"

class XdmfGeometryType {

public:

	const int dimensions;
	const std::string name;

	// Supported Xdmf Geometry Types
	static const XdmfGeometryType NoGeometry;
	static const XdmfGeometryType XYZ;
	static const XdmfGeometryType XY;
	static const XdmfGeometryType X_Y_Z;
	static const XdmfGeometryType X_Y;
	static const XdmfGeometryType VXVYVZ;
	static const XdmfGeometryType Origin_DXDYDZ;
	static const XdmfGeometryType VXVY;
	static const XdmfGeometryType Origin_DXDY;

	/*
	 * Compare two XdmfGeometry types for equality. Each geometry type has a single static instance meaning that
	 * equality can be determined by determining if they have the same pointer value.
	 *
	 * @param an XdmfGeometryType to compare equality to.
	 * @return true iff the XdmfGeometryTypes are equal.
	 */
	bool operator==(const XdmfGeometryType& top) const
	{
		return (this == &top) ? true : false;
	}

	/**
	 * Compare two XdmfGeometry types for inequality. Each geometry type has a single static instance meaning that
	 * equality can be determined by determining if they have the same pointer value.
	 *
	 * @param XdmfGeometryType to compare inequality to.
	 * @return true iff the XdmfGeometryTypes are not equal.
	 */
	bool operator!=(const XdmfGeometryType& top) const
	{
		return (this != &top) ? true : false;
	}

protected:

	/**
	 * Protected constructor for XdmfGeometryType.  The constructor is protected because all geometry types supported
	 * by Xdmf should be accessed through static public instances of XdmfGeometryType - i.e. XdmfGeometryType::XYZ.
	 */
	XdmfGeometryType(const std::string& name, const int& dimensions) :
		name(name),
		dimensions(dimensions)
	{};

private:

	//XdmfTopologyType(const XdmfTopologyType&);  // Not implemented.
	//void operator=(const XdmfTopologyType&);  // Not implemented.

};

class XdmfGeometry : public XdmfItem {

public:

	XdmfNewMacro(XdmfGeometry);

	virtual void write(boost::shared_ptr<XdmfVisitor>) const;

	/**
	 * Get the XdmfGeometryType associated with this Geometry.
	 *
	 * @return XdmfGeometryType.
	 */
	const XdmfGeometryType& getGeometryType() const;

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
	const std::string getGeometryTypeAsString() const;

	/**
	 * Get the number of dimensions for this Geometry.
	 *
	 * @return int giving number of dimensions: i.e. XY = 2, XYZ = 3.
	 */
	const int getDimensions() const;

	virtual const std::string printSelf() const;

protected:

	XdmfGeometry();
	virtual ~XdmfGeometry();

private:

  XdmfGeometry(const XdmfGeometry&);  // Not implemented.
  void operator=(const XdmfGeometry&);  // Not implemented.

  const XdmfGeometryType * mGeometryType;

};

#endif /* XDMFGEOMETRY_HPP_ */
