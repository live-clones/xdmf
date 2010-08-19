#ifndef XDMFITEM_HPP_
#define XDMFITEM_HPP_

// Forward Declarations
class XdmfCoreReader;
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

	virtual ~XdmfItem();

	LOKI_DEFINE_VISITABLE_BASE()
	friend class XdmfCoreReader;

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
	virtual void populateItem(const std::map<std::string, std::string> & itemProperties, std::vector<boost::shared_ptr<XdmfItem > > & childItems, const XdmfCoreReader * const reader) = 0;

private:

	XdmfItem(const XdmfItem & item);  // Not implemented.
	void operator=(const XdmfItem & item);  // Not implemented.

};

#endif /* XDMFITEM_HPP_ */
