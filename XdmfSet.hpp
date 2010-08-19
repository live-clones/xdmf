#ifndef XDMFSET_HPP_
#define XDMFSET_HPP_

// Forward Declarations
class XdmfAttribute;
class XdmfHDF5Controller;
class XdmfSetType;

// Includes
#include "XdmfArray.hpp"

/**
 * @brief Holds a collection of individual nodes, cells, faces, or edges that are part of an XdmfGrid.
 *
 * An XdmfSet holds a collection of nodes, cells, faces, or edges that are part of an XdmfGrid.  For instance,
 * a simulation may want to hold a set of nodes along a boundary.  The individual elements making up the
 * set are determined by their id.  An XdmfSet can have XdmfAttributes attached that contain extra values
 * attached to the elements in the set.
 */
class XdmfSet : public XdmfArray {

public:

	/**
	 * Create a new XdmfSet.
	 *
	 * @return constructed XdmfSet.
	 */
	static boost::shared_ptr<XdmfSet> New();

	virtual ~XdmfSet();

	LOKI_DEFINE_VISITABLE(XdmfSet, XdmfArray)
	static const std::string ItemTag;

	/**
	 * Get an attribute attached to this set by index.
	 *
	 * @param index of the attribute to retrieve.
	 * @return requested attribute.  If not found a NULL pointer is returned.
	 */
	boost::shared_ptr<XdmfAttribute> getAttribute(const unsigned int index);

	/**
	 * Get an attribute attached to this set by index (const version).
	 *
	 * @param index of the attribute to retrieve.
	 * @return requested attribute.  If not found a NULL pointer is returned.
	 */
	boost::shared_ptr<const XdmfAttribute> getAttribute(const unsigned int index) const;

	/**
	 * Get an attribute attached to this set by name.
	 *
	 * @param name of the attribute to retrieve.
	 * @return requested attribute.  If not found a NULL pointer is returned.
	 */
	boost::shared_ptr<XdmfAttribute> getAttribute(const std::string & name);

	/**
	 * Get an attribute attached to this set by name (const version).
	 *
	 * @param name the name of the attribute to retrieve.
	 * @return requested attribute.  If not found a NULL pointer is returned.
	 */
	boost::shared_ptr<const XdmfAttribute> getAttribute(const std::string & name) const;

	std::map<std::string, std::string> getItemProperties() const;

	std::string getItemTag() const;

	/**
	 * Get the name of the set.
	 *
	 * @return a string containing the name of the set.
	 */
	std::string getName() const;

	/**
	 * Get the number of attributes attached to this grid.
	 *
	 * @return the number of attributes attached to this grid.
	 */
	unsigned int getNumberAttributes() const;

	/**
	 * Get the XdmfSetType associated with this set.
	 *
	 * @return XdmfSetType of this set.
	 */
	boost::shared_ptr<const XdmfSetType> getType() const;

	/**
	 * Insert an attribute into the set.
	 *
	 * @param attribute an XdmfAttribute to attach to this set.
	 */
	void insert(const boost::shared_ptr<XdmfAttribute> attribute);

	/**
	 * Remove an attribute from the set by index.  If no attribute is at that index, no attributes are removed.
	 *
	 * @param index of the attribute to remove.
	 */
	void removeAttribute(const unsigned int index);

	/**
	 * Remove an attribute from the set by name.  If no attribute having the name is found, no attributes are removed.
	 *
	 * @param name of the attribute to remove.
	 */
	void removeAttribute(const std::string & name);

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

	void traverse(const boost::shared_ptr<XdmfBaseVisitor> visitor);

protected:

	XdmfSet();
	virtual void populateItem(const std::map<std::string, std::string> & itemProperties, std::vector<boost::shared_ptr<XdmfItem> > & childItems, const XdmfCoreReader * const reader);

private:

	XdmfSet(const XdmfSet & set);  // Not implemented.
	void operator=(const XdmfSet & set);  // Not implemented.

	std::vector<boost::shared_ptr<XdmfAttribute> > mAttributes;
	std::string mName;
	boost::shared_ptr<const XdmfSetType> mSetType;
};

#endif /* XDMFSET_HPP_ */
