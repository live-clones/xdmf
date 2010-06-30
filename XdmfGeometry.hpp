#ifndef XDMFGEOMETRY_HPP_
#define XDMFGEOMETRY_HPP_

// Forward Declarations
class XdmfGeometryType;

// Includes
#include "XdmfDataItem.hpp"

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
	virtual ~XdmfGeometry();

	LOKI_DEFINE_VISITABLE(XdmfGeometry, XdmfDataItem)
	static const std::string ItemTag;

	/**
	 * Get the XdmfGeometryType associated with this geometry.
	 *
	 * @return XdmfGeometryType of this geometry.
	 */
	boost::shared_ptr<const XdmfGeometryType> getGeometryType() const;

	std::map<std::string, std::string> getItemProperties() const;

	std::string getItemTag() const;

	/**
	 * Get the number of points stored in this geometry.
	 */
	unsigned int getNumberPoints() const;

	/**
	 * Set the XdmfGeometryType associated with this geometry.
	 *
	 * @param geometryType the XdmfGeometryType to set.
	 */
	void setGeometryType(const boost::shared_ptr<const XdmfGeometryType> geometryType);

protected:

	XdmfGeometry();
	virtual void populateItem(const std::map<std::string, std::string> & itemProperties, std::vector<boost::shared_ptr<XdmfItem> > & childItems);

private:

	XdmfGeometry(const XdmfGeometry & geometry);  // Not implemented.
	void operator=(const XdmfGeometry & geometry);  // Not implemented.

	boost::shared_ptr<const XdmfGeometryType> mGeometryType;
	int mNumberPoints;
};

#endif /* XDMFGEOMETRY_HPP_ */
