#ifndef XDMFTIME_HPP_
#define XDMFTIME_HPP_

// Includes
#include "XdmfItem.hpp"

/**
 * @brief Time specification for an XdmfGrid Item.
 *
 * An XdmfTime sets a time value for an XdmfGrid item.
 */
class XdmfTime : public XdmfItem {

public:

	/**
	 * Create a new XdmfTime.
	 *
	 * @param timeValue the value of the XdmfTime to create.
	 * @return the new XdmfTime.
	 */
	static boost::shared_ptr<XdmfTime> New(const double & value = 0);

	virtual ~XdmfTime();

	LOKI_DEFINE_VISITABLE(XdmfTime, XdmfItem)
	static const std::string ItemTag;

	std::map<std::string, std::string> getItemProperties() const;

	std::string getItemTag() const;

	/**
	 * Get the time value associated with this XdmfTime.
	 *
	 * @return a double containing the time value.
	 */
	double getValue() const;

	/**
	 * Set the time value associated with this XdmfTime.
	 *
	 * @return a double containing the time value.
	 */
	void setValue(const double & time);

protected:

	XdmfTime(const double & value);
	virtual void populateItem(const std::map<std::string, std::string> & itemProperties, std::vector<boost::shared_ptr<XdmfItem> > & childItems);

private:

	XdmfTime(const XdmfTime & time);  // Not implemented.
	void operator=(const XdmfTime & time);  // Not implemented.

	double mValue;
};

#endif /* XDMFTIME_HPP_ */
