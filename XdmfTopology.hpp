#ifndef XDMFTOPOLOGY_HPP_
#define XDMFTOPOLOGY_HPP_

// Forward Declarations
class XdmfTopologyType;

// Includes
#include "XdmfDataItem.hpp"

/**
 * @brief Handles the connectivity information in an XdmfGrid.
 *
 * XdmfTopology is a required part of an XdmfGrid.  It stores the connectivity information
 * between all points contained in an XdmfGrid.  XdmfTopology contains an XdmfTopologyType property
 * which should be set that specifies the element type stored.
 */
class XdmfTopology : public XdmfDataItem {

public:

	XdmfNewMacro(XdmfTopology);
	virtual ~XdmfTopology();

	LOKI_DEFINE_VISITABLE(XdmfTopology, XdmfDataItem)
	static const std::string ItemTag;

	std::map<std::string, std::string> getItemProperties() const;

	std::string getItemTag() const;

	/**
	 * Get the number of elements this Topology contains.
	 *
	 * @return int of number elements in the Topology.
	 */
	unsigned int getNumberElements() const;

	/**
	 * Get the XdmfTopologyType associated with this topology.
	 *
	 * @return XdmfTopologyType of the topology.
	 */
	boost::shared_ptr<const XdmfTopologyType> getType() const;

	/**
	 * Set the XdmfTopologyType associated with this topology.
	 *
	 * @param topologyType the XdmfTopologyType to set.
	 */
	void setType(const boost::shared_ptr<const XdmfTopologyType> topologyType);

protected:

	XdmfTopology();
	virtual void populateItem(const std::map<std::string, std::string> & itemProperties, std::vector<boost::shared_ptr<XdmfItem> > & childItems);

private:

	XdmfTopology(const XdmfTopology & topology);  // Not implemented.
	void operator=(const XdmfTopology & topology);  // Not implemented.

	int mNumberElements;
	boost::shared_ptr<const XdmfTopologyType> mTopologyType;
};

#endif /* XDMFTOPOLOGY_HPP_ */
