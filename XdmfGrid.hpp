#ifndef XDMFGRID_HPP_
#define XDMFGRID_HPP_

// Forward Declarations
class XdmfAttribute;
class XdmfGeometry;
class XdmfMap;
class XdmfSet;
class XdmfTime;
class XdmfTopology;

// Includes
#include "XdmfItem.hpp"

/**
 * @brief A mesh that consists of elements, points, and values attached to the mesh.
 *
 * XdmfGrid represents a mesh.  It is required to contain two other Xdmf data structures, an XdmfGeometry
 * that stores point locations and an XdmfTopology that store connectivity information.  XdmfAttributes can be inserted
 * into the XdmfGrid to specify values attached at various parts of the mesh.  XdmfSets can be inserted in the XdmfGrid
 * to specify collections of mesh elements.
 */
class XdmfGrid : public virtual XdmfItem {

public:

	/**
	 * Create a new XdmfGrid.
	 *
	 * @return constructed XdmfGrid.
	 */
	static boost::shared_ptr<XdmfGrid> New();

	virtual ~XdmfGrid();

	LOKI_DEFINE_VISITABLE(XdmfGrid, XdmfItem)
	static const std::string ItemTag;

	/**
	 * Get an attribute attached to this grid by index.
	 *
	 * @param index of the attribute to retrieve.
	 * @return requested attribute.  If not found a NULL pointer is returned.
	 */
	boost::shared_ptr<XdmfAttribute> getAttribute(const unsigned int index);

	/**
	 * Get an attribute attached to this grid by index (const version).
	 *
	 * @param index of the attribute to retrieve.
	 * @return requested attribute.  If not found a NULL pointer is returned.
	 */
	boost::shared_ptr<const XdmfAttribute> getAttribute(const unsigned int index) const;

	/**
	 * Get an attribute attached to this grid by name.
	 *
	 * @param name of the attribute to retrieve.
	 * @return requested attribute.  If not found a NULL pointer is returned.
	 */
	boost::shared_ptr<XdmfAttribute> getAttribute(const std::string & name);

	/**
	 * Get an attribute attached to this grid by name (const version).
	 *
	 * @param name the name of the attribute to retrieve.
	 * @return requested attribute.  If not found a NULL pointer is returned.
	 */
	boost::shared_ptr<const XdmfAttribute> getAttribute(const std::string & name) const;

	/**
	 * Get the geometry associated with this grid.
	 *
	 * @return the geometry associated with this grid.
	 */
	boost::shared_ptr<XdmfGeometry> getGeometry();

	/**
	 * Get the geometry associated with this grid (const version).
	 *
	 * @return the geometry associated with this grid.
	 */
	boost::shared_ptr<const XdmfGeometry> getGeometry() const;

	std::map<std::string, std::string> getItemProperties() const;

	virtual std::string getItemTag() const;

	/**
	 * Get the boundary communicator map associated with this grid.
	 *
	 * @return the boundary communicator map associated with this grid.
	 */
	boost::shared_ptr<XdmfMap > getMap();

	/**
	 * Get the boundary communicator map associated with this grid (const version).
	 *
	 * @return the boundary communicator map associated with this grid.
	 */
	boost::shared_ptr<const XdmfMap> getMap() const;

	/**
	 * Get the name of the grid.
	 *
	 * @return the name of the grid.
	 */
	std::string getName() const;

	/**
	 * Get the number of attributes attached to this grid.
	 *
	 * @return the number of attributes attached to this grid.
	 */
	unsigned int getNumberAttributes() const;

	/**
	 * Get the number of sets attached to this grid.
	 *
	 * @return the number of sets attached to this grid.
	 */
	unsigned int getNumberSets() const;

	/**
	 * Get a set attached to this grid by index.
	 *
	 * @param index of the set to retrieve.
	 * @return requested set.  If not found a NULL pointer is returned.
	 */
	boost::shared_ptr<XdmfSet> getSet(const unsigned int index);

	/**
	 * Get a set attached to this grid by index (const version).
	 *
	 * @param index of the set to retrieve.
	 * @return requested set.  If not found a NULL pointer is returned.
	 */
	boost::shared_ptr<const XdmfSet> getSet(const unsigned int index) const;

	/**
	 * Get a set attached to this grid by name.
	 *
	 * @param name of the set to retrieve.
	 * @return requested set.  If not found a NULL pointer is returned.
	 */
	boost::shared_ptr<XdmfSet> getSet(const std::string & name);

	/**
	 * Get a set attached to this grid by name (const version).
	 *
	 * @param name of the set to retrieve.
	 * @return requested set.  If not found a NULL pointer is returned.
	 */
	boost::shared_ptr<const XdmfSet> getSet(const std::string & name) const;

	/**
	 * Get the time associated with this grid.
	 *
	 * @return pointer to the XdmfTime attached to this grid.  If no XdmfTime is attached, return a NULL pointer.
	 */
	boost::shared_ptr<XdmfTime> getTime();

	/**
	 * Get the time associated with this grid (const version).
	 *
	 * @return pointer to the XdmfTime attached to this grid.  If no XdmfTime is attached, return a NULL pointer.
	 */
	boost::shared_ptr<const XdmfTime> getTime() const;

	/**
	 * Get the topology associated with this grid.
	 *
	 * @return the topology associated with this grid.
	 */
	boost::shared_ptr<XdmfTopology> getTopology();

	/**
	 * Get the topology associated with this grid (const version).
	 *
	 * @return the topology associated with this grid.
	 */
	boost::shared_ptr<const XdmfTopology> getTopology() const;

	/**
	 * Insert an attribute into the grid.
	 *
	 * @param attribute an XdmfAttribute to attach to this grid.
	 */
	void insert(const boost::shared_ptr<XdmfAttribute> attribute);

	/**
	 * Insert a set into the grid.
	 *
	 * @param set an XdmfSet to attach to this grid.
	 */
	void insert(const boost::shared_ptr<XdmfSet> set);

	/**
	 * Remove an attribute from the grid by index.  If no attribute is at that index, no attributes are removed.
	 *
	 * @param index of the attribute to remove.
	 */
	void removeAttribute(const unsigned int index);

	/**
	 * Remove an attribute from the grid by name.  If no attribute having the name is found, no attributes are removed.
	 *
	 * @param name of the attribute to remove.
	 */
	void removeAttribute(const std::string & name);

	/**
	 * Remove an set from the grid by index.  If no set is at that index, no sets are removed.
	 *
	 * @param index of the set to remove.
	 */
	void removeSet(const unsigned int index);

	/**
	 * Remove an set from the grid by name.  If no set having the name is found, no sets are removed.
	 *
	 * @param name of the set to remove.
	 */
	void removeSet(const std::string & name);

	/**
	 * Set the geometry associated with this grid.
	 *
	 * @param geometry an XdmfGeometry to associate with this grid.
	 */
	void setGeometry(const boost::shared_ptr<XdmfGeometry> geometry);

	/**
	 * Set the boundary communicator map associated with this grid.
	 *
	 * @param map a XdmfMap to associate with this grid.
	 */
	void setMap(boost::shared_ptr<XdmfMap> map);

	/**
	 * Set the name of the grid.
	 *
	 * @param name of the grid to set.
	 */
	void setName(const std::string & name);

	/**
	 * Set the time associated with this grid.
	 *
	 * @param time an XdmfTime to associate with this grid.
	 */
	void setTime(const boost::shared_ptr<XdmfTime> time);

	/**
	 * Set the topology associated with this grid.
	 *
	 * @param topology an XdmfTopology to associate with this grid.
	 */
	void setTopology(const boost::shared_ptr<XdmfTopology> topology);

	virtual void traverse(const boost::shared_ptr<XdmfBaseVisitor> visitor);

protected:

	XdmfGrid();
	virtual void populateItem(const std::map<std::string, std::string> & itemProperties, std::vector<boost::shared_ptr<XdmfItem> > & childItems);

	std::string mName;

private:

	XdmfGrid(const XdmfGrid & grid);  // Not implemented.
	void operator=(const XdmfGrid & grid);  // Not implemented.

	std::vector<boost::shared_ptr<XdmfAttribute> > mAttributes;
	boost::shared_ptr<XdmfGeometry> mGeometry;
	boost::shared_ptr<XdmfMap> mMap;
	std::vector<boost::shared_ptr<XdmfSet> > mSets;
	boost::shared_ptr<XdmfTime> mTime;
	boost::shared_ptr<XdmfTopology> mTopology;
};

#endif /* XDMFGRID_HPP_ */
