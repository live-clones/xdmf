#ifndef XDMFSET_HPP_
#define XDMFSET_HPP_

// Forward Declarations
class XdmfHDF5Controller;
class XdmfSetType;

// Includes
#include <set>
#include "XdmfItem.hpp"

/**
 * @brief Holds a collection of individual nodes, cells, faces, or edges that are part of an XdmfGrid.
 *
 * An XdmfSet holds a collection of nodes, cells, faces, or edges that are part of an XdmfGrid.  For instance,
 * a simulation may want to hold a set of nodes along a boundary.  The individual elements making up the
 * set are determined by their id.  An XdmfSet can have XdmfAttributes attached that contain extra values
 * attached to the elements in the set.
 */
class XdmfSet : public XdmfItem,
	public std::set<unsigned int> {

public:

	/**
	 * Create a new XdmfSet.
	 *
	 * @return constructed XdmfSet.
	 */
	static boost::shared_ptr<XdmfSet> New();

	virtual ~XdmfSet();

	LOKI_DEFINE_VISITABLE(XdmfSet, XdmfItem)
	static const std::string ItemTag;

	/**
	 * Get the hdf5 controller attached to this set.
	 *
	 * @return the hdf5 controller attached to this set.
	 */
	boost::shared_ptr<XdmfHDF5Controller> getHDF5Controller();

	/**
	 * Get the hdf5 controller attached to this set (const version).
	 *
	 * @return the hdf5 controller attached to this set.
	 */
	boost::shared_ptr<const XdmfHDF5Controller> getHDF5Controller() const;

	std::map<std::string, std::string> getItemProperties() const;

	std::string getItemTag() const;

	/**
	 * Get the name of the set.
	 *
	 * @return a string containing the name of the set.
	 */
	std::string getName() const;

	/**
	 * Get the XdmfSetType associated with this set.
	 *
	 * @return XdmfSetType of this set.
	 */
	boost::shared_ptr<const XdmfSetType> getType() const;

	/**
	 * Read data from disk into memory.
	 */
	void read();

	/**
	 * Release all data held in memory.  The HDF5Controller remains attached.
	 */
	void release();

	/**
	 * Attach an hdf5 controller to this set.
	 *
	 * @param hdf5Controller to attach to this set.
	 */
	void setHDF5Controller(const boost::shared_ptr<XdmfHDF5Controller> hdf5Controller);

	/**
	 * Set the name of the set.
	 *
	 * @param name a string containing the name to set.
	 */
	void setName(const std::string & name);

	/**
	 * Set the XdmfSetType associated with this set.
	 *
	 * @param setType the XdmfSetType to set.
	 */
	void setType(const boost::shared_ptr<const XdmfSetType> setType);

	std::size_t size() const;

	void traverse(const boost::shared_ptr<XdmfBaseVisitor> visitor);

	XdmfSet();

protected:


	virtual void populateItem(const std::map<std::string, std::string> & itemProperties, std::vector<boost::shared_ptr<XdmfItem> > & childItems);

private:

	XdmfSet(const XdmfSet & set);  // Not implemented.
	void operator=(const XdmfSet & set);  // Not implemented.

	std::string mName;
	boost::shared_ptr<XdmfHDF5Controller> mHDF5Controller;
	boost::shared_ptr<const XdmfSetType> mSetType;
};

#endif /* XDMFSET_HPP_ */
