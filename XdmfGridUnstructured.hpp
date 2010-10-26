#ifndef XDMFGRIDUNSTRUCTURED_HPP_
#define XDMFGRIDUNSTRUCTURED_HPP_

// Includes
#include "XdmfGrid.hpp"

/**
 * @brief An unstructured grid consisting of elements, points, and fields attached to the mesh.
 *
 * After creating an unstructured grid, the XdmfGeometry and XdmfTopology must be set.  The XdmfTopology
 * describes the element types contained in the grid and their connectivity.  The XdmfGeometry describes the
 * location of nodes.
 */
class XdmfGridUnstructured : public XdmfGrid {

public:

	/**
	 * Create a new XdmfGridUnstructured.
	 *
	 * @return constructed XdmfGridUnstructured.
	 */
	static boost::shared_ptr<XdmfGridUnstructured> New();

	virtual ~XdmfGridUnstructured();

	static const std::string ItemTag;

	/**
	 * Get the geometry associated with this grid.
	 *
	 * @return the geometry associated with this grid.
	 */
	boost::shared_ptr<XdmfGeometry> getGeometry();

	virtual std::string getItemTag() const;

	/**
	 * Get the topology associated with this grid.
	 *
	 * @return the topology associated with this grid.
	 */
	boost::shared_ptr<XdmfTopology> getTopology();

	/**
	 * Set the geometry associated with this grid.
	 *
	 * @param geometry an XdmfGeometry to associate with this grid.
	 */
	void setGeometry(const boost::shared_ptr<XdmfGeometry> geometry);

	/**
	 * Set the topology associated with this grid.
	 *
	 * @param topology an XdmfTopology to associate with this grid.
	 */
	void setTopology(const boost::shared_ptr<XdmfTopology> topology);

protected:

	XdmfGridUnstructured();

private:

	XdmfGridUnstructured(const XdmfGridUnstructured & grid);  // Not implemented.
	void operator=(const XdmfGridUnstructured & grid);  // Not implemented.

};

#endif /* XDMFGRID_HPP_ */
