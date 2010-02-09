/*
 * XdmfGeometryType.hpp
 *
 *  Created on: Jan 25, 2010
 *      Author: kleiter
 */

#ifndef XDMFGEOMETRYTYPE_HPP_
#define XDMFGEOMETRYTYPE_HPP_

#include <string>

class XdmfGeometryType {

public:

	// Supported Xdmf Geometry Types
	static XdmfGeometryType NoGeometryType();
	static XdmfGeometryType XYZ();
	static XdmfGeometryType XY();
	static XdmfGeometryType X_Y_Z();
	static XdmfGeometryType X_Y();
	static XdmfGeometryType VXVYVZ();
	static XdmfGeometryType Origin_DXDYDZ();
	static XdmfGeometryType VXVY();
	static XdmfGeometryType Origin_DXDY();

	/**
	 * Get the dimensions of this geometry type - i.e. XYZ = 3.
	 *
	 * @return an int containing number of dimensions.
	 */
	int getDimensions() const;

	/**
	 * Get the name of this geometry type
	 *
	 * @return a string containing the name.
	 */
	std::string getName() const;

	/*
	 * Compare two XdmfGeometry types for equality.
	 *
	 * @param an XdmfGeometryType to compare equality to.
	 * @return true iff the XdmfGeometryTypes are equal.
	 */
	bool operator==(const XdmfGeometryType& geometryType) const;

	/**
	 * Compare two XdmfGeometry types for inequality.
	 *
	 * @param XdmfGeometryType to compare inequality to.
	 * @return true iff the XdmfGeometryTypes are not equal.
	 */
	bool operator!=(const XdmfGeometryType& geometryType) const;

	XdmfGeometryType(const XdmfGeometryType& geometryType);
	XdmfGeometryType& operator=(const XdmfGeometryType& geometryType);

protected:

	/**
	 * Protected constructor for XdmfGeometryType.  The constructor is protected because all geometry types supported
	 * by Xdmf should be accessed through more specific static methods that construct XdmfGeometryTypes - i.e. XdmfGeometryType::XYZ().
	 */
	XdmfGeometryType(const std::string& name, const int& dimensions);

private:

	int mDimensions;
	std::string mName;
};

#endif /* XDMFATTRIBUTETYPE_HPP_ */
