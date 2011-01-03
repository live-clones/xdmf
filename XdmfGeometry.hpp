#ifndef XDMFGEOMETRY_HPP_
#define XDMFGEOMETRY_HPP_

// Forward Declarations
class XdmfGeometryType;

// Includes
#include "Xdmf.hpp"
#include "XdmfArray.hpp"

/**
 * @brief Handles the coordinate positions of points in an XdmfGrid.
 *
 * XdmfGeometry is a required part of an XdmfGrid.  It stores the coordinate values
 * of all points contained in an XdmfGrid.  XdmfGeometry contains an XdmfGeometryType property
 * which should be set that specifies the types of coordinate values stored.
 */
class XDMF_EXPORT XdmfGeometry : public XdmfArray {

public:

	/**
	 * Create a new XdmfGeometry.
	 *
	 * @return constructed XdmfGeometry.
	 */
	static boost::shared_ptr<XdmfGeometry> New();

	virtual ~XdmfGeometry();

	LOKI_DEFINE_VISITABLE(XdmfGeometry, XdmfArray)
	static const std::string ItemTag;

	std::map<std::string, std::string> getItemProperties() const;

	std::string getItemTag() const;

	/**
	 * Get the number of points stored in this geometry.
	 */
	virtual unsigned int getNumberPoints() const;

	/**
	 * Get the XdmfGeometryType associated with this geometry.
	 *
	 * @return XdmfGeometryType of this geometry.
	 */
	boost::shared_ptr<const XdmfGeometryType> getType() const;

	/**
	 * Set the XdmfGeometryType associated with this geometry.
	 *
	 * @param type the XdmfGeometryType to set.
	 */
	void setType(const boost::shared_ptr<const XdmfGeometryType> type);

protected:

	XdmfGeometry();
	virtual void populateItem(const std::map<std::string, std::string> & itemProperties, std::vector<boost::shared_ptr<XdmfItem> > & childItems, const XdmfCoreReader * const reader);

private:

	XdmfGeometry(const XdmfGeometry & geometry);  // Not implemented.
	void operator=(const XdmfGeometry & geometry);  // Not implemented.

	int mNumberPoints;
	boost::shared_ptr<const XdmfGeometryType> mType;
};

#ifdef _WIN32
    XDMF_TEMPLATE template class XDMF_EXPORT boost::shared_ptr<const XdmfGeometryType>;
#endif

#endif /* XDMFGEOMETRY_HPP_ */
