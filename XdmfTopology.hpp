#ifndef XDMFTOPOLOGY_HPP_
#define XDMFTOPOLOGY_HPP_

// Includes
#include "XdmfDataItem.hpp"
#include "XdmfTopologyType.hpp"

/**
 * @brief Handles the connectivity information in an XdmfGrid.
 *
 * XdmfTopology is a required part of an XdmfGrid.  It stores the connectivty information
 * between all points contained in an XdmfGrid.  XdmfTopology contains an XdmfTopologyType property
 * which should be set that specifies the element type stored.
 */
class XdmfTopology : public XdmfDataItem {

public:

	XdmfNewMacro(XdmfTopology);
	LOKI_DEFINE_VISITABLE(XdmfTopology, XdmfDataItem)

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
	XdmfTopologyType getTopologyType() const;

	/**
	 * Set the XdmfTopologyType associated with this topology.
	 *
	 * @param topologyType the XdmfTopologyType to set.
	 */
	void setTopologyType(const XdmfTopologyType & topologyType);

protected:

	XdmfTopology();
	virtual ~XdmfTopology();

private:

	XdmfTopology(const XdmfTopology & topology);  // Not implemented.
	void operator=(const XdmfTopology & topology);  // Not implemented.

	int mNumberElements;
	XdmfTopologyType mTopologyType;
};

#endif /* XDMFTOPOLOGY_HPP_ */
