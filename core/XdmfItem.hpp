#ifndef XDMFITEM_HPP_
#define XDMFITEM_HPP_

// Forward Declarations
class XdmfCoreReader;
class XdmfInformation;
class XdmfVisitor;

// Includes
#include <boost/shared_ptr.hpp>
#include <loki/Visitor.h>
#include <map>
#include <string>
#include <vector>

/**
 * @brief Base class of any object that is able to be added to an Xdmf structure.
 *
 * XdmfItem is an abstract base class.  It represents any item that can be visited and traversed by an XdmfVisitor
 * and have its contents added to an Xdmf file.
 */
class XdmfItem : public Loki::BaseVisitable<void> {

public:

	virtual ~XdmfItem() = 0;

	LOKI_DEFINE_VISITABLE_BASE()
	friend class XdmfCoreReader;

	/**
	 * Get an information attached to this item by index.
	 *
	 * @param index of the information to retrieve.
	 * @return requested information.  If not found a NULL pointer is returned.
	 */
	boost::shared_ptr<XdmfInformation> getInformation(const unsigned int index);

	/**
	 * Get an information attached to this item by index (const version).
	 *
	 * @param index of the information to retrieve.
	 * @return requested information.  If not found a NULL pointer is returned.
	 */
	boost::shared_ptr<const XdmfInformation> getInformation(const unsigned int index) const;

	/**
	 * Get an information attached to this item by key.
	 *
	 * @param key of the information to retrieve.
	 * @return requested information.  If not found a NULL pointer is returned.
	 */
	boost::shared_ptr<XdmfInformation> getInformation(const std::string & key);

	/**
	 * Get an information attached to this grid by key (const version).
	 *
	 * @param key of the information to retrieve.
	 * @return requested information.  If not found a NULL pointer is returned.
	 */
	boost::shared_ptr<const XdmfInformation> getInformation(const std::string & key) const;

	/**
	 * Get the tag for this XdmfItem.  This is equivalent to tags in XML parlance.
	 */
	virtual std::string getItemTag() const = 0;

	/**
	 * Get the key/value property pairs for this XdmfItem.  These are equivalent to attributes in XML parlance.
	 *
	 * @return a map of key/value properties associated with this XdmfItem.
	 */
	virtual std::map<std::string, std::string> getItemProperties() const = 0;

	/**
	 * Get the number of informations attached to this item.
	 *
	 * @return the number of informations attached to this item.
	 */
	unsigned int getNumberInformations() const;

	/**
	 * Insert an information into the item.
	 *
	 * @param information an XdmfInformation to attach to this item.
	 */
	virtual void insert(const boost::shared_ptr<XdmfInformation> information);

	/**
	 * Remove an information from the item by index.  If no information is at that index, no information are removed.
	 *
	 * @param index of the information to remove.
	 */
	void removeInformation(const unsigned int index);

	/**
	 * Remove an information from the item by key.  If no information having the key is found, no informations are removed.
	 *
	 * @param key of the attribute to remove.
	 */
	void removeInformation(const std::string & key);

	/**
	 * Traverse this XdmfItem by passing the visitor to its children XdmfItems.
	 *
	 * @param a visitor to pass to this XdmfItem's children.
	 */
	virtual void traverse(const boost::shared_ptr<XdmfBaseVisitor> visitor);

protected:

	XdmfItem();

	/**
	 * Populates an item using a map of key/value property pairs and a vector of its child items.  This is used to
	 * support generic reading of XdmfItems from disk.
	 *
	 * @param itemProperties a map of key/value properties associated with this XdmfItem.
	 * @param childItems a vector of child items to be added to this XdmfItem.
	 */
	virtual void populateItem(const std::map<std::string, std::string> & itemProperties, std::vector<boost::shared_ptr<XdmfItem > > & childItems, const XdmfCoreReader * const reader);

	std::vector<boost::shared_ptr<XdmfInformation> > mInformations;

private:

	XdmfItem(const XdmfItem & item);  // Not implemented.
	void operator=(const XdmfItem & item);  // Not implemented.

};

#endif /* XDMFITEM_HPP_ */
