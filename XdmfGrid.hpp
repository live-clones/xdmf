#ifndef XDMFGRID_HPP_
#define XDMFGRID_HPP_

// Forward Declarations
class XdmfAttribute;
class XdmfGeometry;
class XdmfTopology;

// Includes
#include "XdmfItem.hpp"
#include <vector>

/**
 * @brief A mesh that consists of elements, points, and values attached to the mesh.
 *
 * XdmfGrid represents a mesh.  It is required to contain two other Xdmf data structures, an XdmfGeometry
 * that stores point locations and an XdmfTopology that store connectivity information.  XdmfAttributes can be inserted
 * into the XdmfGrid to specify values attached at various parts of the mesh.
 */
class XdmfGrid : public XdmfItem {

public:

	XdmfNewMacro(XdmfGrid);
	LOKI_DEFINE_VISITABLE(XdmfGrid, XdmfItem)
	static std::string ItemTag;

	/**
	 * Get an XdmfAttribute attached to this grid.
	 *
	 * @param index of the XdmfAttribute to retrieve.
	 * @return smart pointer to the XdmfAttribute attached to this grid.
	 */
	boost::shared_ptr<XdmfAttribute> getAttribute(unsigned int index);

	/**
	 * Get an XdmfAttribute attached to this grid (const version).
	 *
	 * @param index of the XdmfAttribute to retrieve.
	 * @return pointer to the XdmfAttribute attached to this grid.
	 */
	boost::shared_ptr<const XdmfAttribute> getAttribute(unsigned int index) const;

	/**
	 * Get the XdmfGeometry associated with this grid.
	 *
	 * @return a smart pointer to the XdmfGeometry.
	 */
	boost::shared_ptr<XdmfGeometry> getGeometry();

	/**
	 * Get the XdmfGeometry associated with this grid (const version).
	 *
	 * @return a smart pointer to the XdmfGeometry.
	 */
	boost::shared_ptr<const XdmfGeometry> getGeometry() const;

	std::map<std::string, std::string> getItemProperties() const;

	std::string getItemTag() const;

	/**
	 * Get the name of the grid.
	 *
	 * @return a string containing the name of the grid.
	 */
	std::string getName() const;

	/**
	 * Get the number of XdmfAttributes attached to this grid.
	 *
	 * @return an unsigned int containing the number of XdmfAttributes attached to this grid.
	 */
	unsigned int getNumberOfAttributes() const;

	/**
	 * Get the XdmfTopology associated with this grid.
	 *
	 * @return a smart pointer to the XdmfTopology.
	 */
	boost::shared_ptr<XdmfTopology> getTopology();

	/**
	 * Get the XdmfTopology associated with this grid (const version).
	 *
	 * @return a smart pointer to the XdmfTopology.
	 */
	boost::shared_ptr<const XdmfTopology> getTopology() const;

	/**
	 * Insert an XdmfAttribute into the grid.
	 *
	 * @param attribute a smart pointer to the XdmfAttribute to attach to this grid.
	 */
	void insert(boost::shared_ptr<XdmfAttribute> attribute);

	/**
	 * Set the XdmfGeometry associated with this grid.
	 *
	 * @param geometry a smart pointer to the XdmfGeometry to attach to this grid.
	 */
	void setGeometry(boost::shared_ptr<XdmfGeometry> geometry);

	/**
	 * Set the XdmfTopology associated with this grid.
	 *
	 * @param topology a smart pointer to the XdmfTopology to attach to this grid.
	 */
	void setTopology(boost::shared_ptr<XdmfTopology> topology);

	/**
	 * Set the name of the grid.
	 *
	 * @param name a string containing the name to set.
	 */
	void setName(const std::string & name);

	virtual void traverse(boost::shared_ptr<Loki::BaseVisitor> visitor);

protected:

	XdmfGrid();
	virtual ~XdmfGrid();
	virtual void populateItem(const std::map<std::string, std::string> & itemProperties, std::vector<boost::shared_ptr<XdmfItem> > & childItems);

private:

	XdmfGrid(const XdmfGrid & grid);  // Not implemented.
	void operator=(const XdmfGrid & grid);  // Not implemented.

	std::vector<boost::shared_ptr<XdmfAttribute> > mAttributes;
	boost::shared_ptr<XdmfGeometry> mGeometry;
	boost::shared_ptr<XdmfTopology> mTopology;
	std::string mName;
};

#endif /* XDMFGRID_HPP_ */
