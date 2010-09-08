#ifndef XDMFGEOMETRYTYPE_HPP_
#define XDMFGEOMETRYTYPE_HPP_

// Includes
#include "XdmfItemProperty.hpp"

/**
 * @brief Property describing the types of coordinate values stored in an XdmfGeometry.
 *
 * XdmfGeometryType is a property used by XdmfGeometry to specify the type of coordinate values
 * stored in the XdmfGeometry.  A specific XdmfGeometryType can be created by calling on of the static methods
 * in the class, i.e. XdmfAttributeType::XYZ().
 *
 * Xdmf supports the following geometry types:
 * 	NoGeometryType
 * 	XYZ
 * 	XY
 * 	X_Y_Z
 * 	X_Y
 * 	VXVYVZ
 * 	Origin_DXDYDZ
 * 	VXVY
 * 	Origin_DXDY
 */
class XdmfGeometryType : public XdmfItemProperty {

public:

	virtual ~XdmfGeometryType();

	friend class XdmfGeometry;

	// Supported Xdmf Geometry Types
	static boost::shared_ptr<const XdmfGeometryType> NoGeometryType();
	static boost::shared_ptr<const XdmfGeometryType> XYZ();
	static boost::shared_ptr<const XdmfGeometryType> XY();
	static boost::shared_ptr<const XdmfGeometryType> X_Y_Z();
	static boost::shared_ptr<const XdmfGeometryType> X_Y();
	static boost::shared_ptr<const XdmfGeometryType> VXVYVZ();
	static boost::shared_ptr<const XdmfGeometryType> VXVY();

	/**
	 * Get the dimensions of this geometry type - i.e. XYZ = 3.
	 *
	 * @return an int containing number of dimensions.
	 */
	virtual unsigned int getDimensions() const;

	/**
	 * Get the name of this geometry type.
	 *
	 * @return the name of this geometry type.
	 */
	std::string getName() const;

	virtual void getProperties(std::map<std::string, std::string> & collectedProperties) const;

	/*
	 * Compare two XdmfGeometryTypes for equality.
	 *
	 * @param geometryType a XdmfGeometryType to compare equality to.
	 * @return true if the XdmfGeometryTypes are equal.
	 */
	bool operator==(const XdmfGeometryType & geometryType) const;

	/**
	 * Compare two XdmfGeometryTypes for inequality.
	 *
	 * @param geometryType a XdmfGeometryType to compare inequality to.
	 * @return true if the XdmfGeometryTypes are not equal.
	 */
	bool operator!=(const XdmfGeometryType & geometryType) const;

        /**
         * Compare two XdmfGeometryType for equality (for wrapping)
         *
         * @param geometryType a boost shared pointer to an XdmfGeometryType to compare equality to.
         * @return true if the XdmfGeometryType are equal.
         */
        bool IsEqual(boost::shared_ptr<XdmfGeometryType> geometryType);

protected:

	/**
	 * Protected constructor for XdmfGeometryType.  The constructor is protected because all geometry types supported
	 * by Xdmf should be accessed through more specific static methods that construct XdmfGeometryTypes -
	 * i.e. XdmfGeometryType::XYZ().
	 *
	 * @param name a std::string containing the name of the XdmfGeometryType.
	 * @param dimensions an int containing the dimensions of the XdmfGeometryType.
	 */
	XdmfGeometryType(const std::string & name, const int & dimensions);

private:

	XdmfGeometryType(const XdmfGeometryType & geometryType); // Not implemented.
	void operator=(const XdmfGeometryType & geometryType); // Not implemented.

	static boost::shared_ptr<const XdmfGeometryType> New(const std::map<std::string, std::string> & itemProperties);

	unsigned int mDimensions;
	std::string mName;
};

#endif /* XDMFGEOMETRYTYPE_HPP_ */
