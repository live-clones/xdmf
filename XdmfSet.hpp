#ifndef XDMFSET_HPP_
#define XDMFSET_HPP_

// Forward Declarations
class XdmfSetType;

// Includes
#include "XdmfDataItem.hpp"

/**
 * @brief Holds a collection of individual nodes, cells, faces, or edges that are part of an XdmfGrid.
 *
 * An XdmfSet holds a collection of nodes, cells, faces, or edges that are part of an XdmfGrid.  For instance,
 * a simulation may want to hold a set of nodes along a boundary.  The individual elements making up the
 * set are determined by their id.  An XdmfSet can have XdmfAttributes attached that contain extra values
 * attached to the elements in the set.
 */
class XdmfSet : public XdmfDataItem {

public:

	XdmfNewMacro(XdmfSet);
	virtual ~XdmfSet();

	LOKI_DEFINE_VISITABLE(XdmfSet, XdmfDataItem)
	static const std::string ItemTag;

	/**
	 * Get the XdmfSetType associated with this set.
	 *
	 * @return XdmfSetType of this set.
	 */
	boost::shared_ptr<const XdmfSetType> getSetType() const;

	std::map<std::string, std::string> getItemProperties() const;

	std::string getItemTag() const;

	/**
	 * Get the name of the set.
	 *
	 * @return a string containing the name of the set.
	 */
	std::string getName() const;

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
	void setSetType(const boost::shared_ptr<const XdmfSetType> setType);

protected:

	XdmfSet();
	virtual void populateItem(const std::map<std::string, std::string> & itemProperties, std::vector<boost::shared_ptr<XdmfItem> > & childItems);

private:

	XdmfSet(const XdmfSet & set);  // Not implemented.
	void operator=(const XdmfSet & set);  // Not implemented.

	std::string mName;
	boost::shared_ptr<const XdmfSetType> mSetType;
};

#endif /* XDMFSET_HPP_ */
