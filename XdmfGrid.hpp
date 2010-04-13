// Kenneth Leiter

#ifndef XDMFGRID_HPP_
#define XDMFGRID_HPP_

// Forward Declarations
class XdmfAttribute;
class XdmfGeometry;
class XdmfTopology;

// Includes
#include "XdmfItem.hpp"
#include <vector>

class XdmfGrid : public XdmfItem {

public:

	XdmfNewMacro(XdmfGrid);

	/**
	 * Get the geometry associated with this grid.
	 *
	 * @return a smart pointer to the geometry.
	 */
	boost::shared_ptr<XdmfGeometry> getGeometry();

	/**
	 * Get the geometry associated with this grid (const version).
	 *
	 * @return a smart pointer to the geometry.
	 */
	boost::shared_ptr<const XdmfGeometry> getGeometry() const;

	/**
	 * Set the geometry associated with this grid.
	 *
	 * @param a smart pointer to the geometry to attach to this grid.
	 */
	void setGeometry(boost::shared_ptr<XdmfGeometry>);

	/**
	 * Get the topology associated with this grid.
	 *
	 * @return a smart pointer to the topology.
	 */
	boost::shared_ptr<XdmfTopology> getTopology();

	/**
	 * Get the topology associated with this grid (const version).
	 *
	 * @return a smart pointer to the topology.
	 */
	boost::shared_ptr<const XdmfTopology> getTopology() const;

	/**
	 * Set the geometry associated with this grid.
	 *
	 * @param a smart pointer to the topology to attach to this grid.
	 */
	void setTopology(boost::shared_ptr<XdmfTopology>);

	/**
	 * Get the name of the grid.
	 *
	 * @return a string containing the name of the grid.
	 */
	std::string getName() const;

	/**
	 * Set the name of the grid.
	 *
	 * @param a string containing the name to set.
	 */
	void setName(const std::string&);

	/**
	 * Insert an attribute into the grid
	 */
	void insert(boost::shared_ptr<XdmfAttribute>);

	/**
	 * Get an attribute attached to this grid.
	 *
	 * @param index of the attribute to retrieve
	 * @return pointer to the attribute attached to this grid
	 */
	boost::shared_ptr<XdmfAttribute> getAttribute(unsigned int index);

	/**
	 * Get an attribute attached to this grid (const version).
	 *
	 * @param index of the attribute to retrieve
	 * @return pointer to the attribute attached to this grid
	 */
	boost::shared_ptr<const XdmfAttribute> getAttribute(unsigned int index) const;

	/**
	 * Get the number of attributes attached to this grid
	 */
	unsigned int getNumberOfAttributes() const;

	virtual std::string printSelf() const;

	virtual void traverse(boost::shared_ptr<XdmfVisitor> visitor) const;

	virtual void write(boost::shared_ptr<XdmfVisitor> visitor) const;

protected:

	XdmfGrid();
	virtual ~XdmfGrid();

private:

	XdmfGrid(const XdmfGrid&);  // Not implemented.
	void operator=(const XdmfGrid&);  // Not implemented.

	boost::shared_ptr<XdmfGeometry> mGeometry;
	boost::shared_ptr<XdmfTopology> mTopology;
	std::vector<boost::shared_ptr<XdmfAttribute> > mAttributes;
	std::string mName;
};

#endif /* XDMFGRID_HPP_ */
