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
	bool operator==(const XdmfGeometryType& top) const
	{
		return mDimensions == top.mDimensions && mName.compare(top.mName) == 0;
	}

	/**
	 * Compare two XdmfGeometry types for inequality.
	 *
	 * @param XdmfGeometryType to compare inequality to.
	 * @return true iff the XdmfGeometryTypes are not equal.
	 */
	bool operator!=(const XdmfGeometryType& top) const
	{
		return !this->operator ==(top);
	}

	XdmfGeometryType(const XdmfGeometryType&);
	XdmfGeometryType& operator=(const XdmfGeometryType&);

protected:

	/**
	 * Protected constructor for XdmfGeometryType.  The constructor is protected because all geometry types supported
	 * by Xdmf should be accessed through more specific static methods that construct XdmfGeometryTypes - i.e. XdmfGeometryType::XYZ().
	 */
	XdmfGeometryType(const std::string& name, const int& dimensions) :
		mName(name),
		mDimensions(dimensions)
	{};

private:

	int mDimensions;
	std::string mName;
};

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

	virtual const std::string printSelf() const;

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
