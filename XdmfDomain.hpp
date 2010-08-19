#ifndef XDMFDOMAIN_HPP_
#define XDMFDOMAIN_HPP_

// Forward Declarations
class XdmfGrid;
class XdmfGridCollection;

// Includes
#include "XdmfItem.hpp"

/**
 * @brief The root XdmfItem that contains XdmfGrids.
 *
 * XdmfDomain is the top XdmfItem in an Xdmf structure.  It can store a number of grids and
 * provides methods to insert, retrieve, and remove these grids.
 */
class XdmfDomain : public virtual XdmfItem {

public:

	/**
	 * Create a new XdmfDomain.
	 *
	 * @return constructed XdmfDomain.
	 */
	static boost::shared_ptr<XdmfDomain> New();
	virtual ~XdmfDomain();

	LOKI_DEFINE_VISITABLE(XdmfDomain, XdmfItem)
	static const std::string ItemTag;

	/**
	 * Get a XdmfGrid attached to the domain.
	 *
	 * @param index of the grid to retrieve.
	 * @return grid at the specified index attached to this domain.
	 */
	boost::shared_ptr<XdmfGrid> getGrid(const unsigned int index);

	/**
	 * Get a XdmfGrid attached to this domain (const version).
	 *
	 * @param index of the grid to retrieve.
	 * @return grid at the specified index attached to this domain.
	 */
	boost::shared_ptr<const XdmfGrid> getGrid(const unsigned int index) const;

	/**
	 * Get a XdmfGridCollection attached to the domain.
	 *
	 * @param index of the grid collection to retrieve.
	 * @return grid collection at the specified index attached to this domain.
	 */
	boost::shared_ptr<XdmfGridCollection> getGridCollection(const unsigned int index);

	/**
	 * Get a XdmfGridCollection attached to the domain (const version).
	 *
	 * @param index of the grid collection to retrieve.
	 * @return grid collection at the specified index attached to this domain.
	 */
	boost::shared_ptr<const XdmfGridCollection> getGridCollection(const unsigned int index) const;

	std::map<std::string, std::string> getItemProperties() const;

	virtual std::string getItemTag() const;

	/**
	 * Get the number of XdmfGrids attached to this domain.
	 *
	 * @return unsigned int containing the number of XdmfGrids attached to this domain.
	 */
	unsigned int getNumberGrids() const;

	/**
	 * Get the number of XdmfGridCollections attached to this domain.
	 *
	 * @return unsigned int containing the number of XdmfGridCollections attached to this domain.
	 */
	unsigned int getNumberGridCollections() const;

	/**
	 * Insert a XdmfGrid into the domain.
	 *
	 * @param grid a shared pointer to an XdmfGrid to insert into the domain.
	 */
	void insert(const boost::shared_ptr<XdmfGrid> grid);

	/**
	 * Insert a XdmfGridCollection into the domain.
	 *
	 * @param gridCollection a shared pointer to an XdmfGridCollection to insert into the domain.
	 */
	void insert(const boost::shared_ptr<XdmfGridCollection> gridCollection);

	/**
	 * Remove an XdmfGrid from the domain.
	 *
	 * @param index of the XdmfGrid to remove.
	 */
	void removeGrid(const unsigned int index);

	/**
	 * Remove an XdmfGridCollection from the domain.
	 *
	 * @param index of the XdmfGridCollection to remove.
	 */
	void removeGridCollection(const unsigned int index);

	virtual void traverse(const boost::shared_ptr<XdmfBaseVisitor> visitor);

protected:

	XdmfDomain();
	virtual void populateItem(const std::map<std::string, std::string> & itemProperties, std::vector<boost::shared_ptr<XdmfItem> > & childItems, const XdmfCoreReader * const reader);

private:

	XdmfDomain(const XdmfDomain & domain);  // Not implemented.
	void operator=(const XdmfDomain & domain);  // Not implemented.

	std::vector<boost::shared_ptr<XdmfGrid> > mGrids;
	std::vector<boost::shared_ptr<XdmfGridCollection> > mGridCollections;
};

#endif /* XDMFDOMAIN_HPP_ */
