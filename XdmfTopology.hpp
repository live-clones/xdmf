/*
 * XdmfTopology.hpp
 *
 *  Created on: Jan 25, 2010
 *      Author: kleiter
 */

#ifndef XDMFTOPOLOGY_HPP_
#define XDMFTOPOLOGY_HPP_

// Includes
#include "XdmfDataItem.hpp"
#include "XdmfTopologyType.hpp"

class XdmfTopology : public XdmfDataItem {

public:

	XdmfNewMacro(XdmfTopology);

	/**
	 * Get the XdmfTopologyType associated with this Topology.
	 *
	 * @return XdmfTopologyType.
	 */
	XdmfTopologyType getTopologyType() const;

	/**
	 * Set the XdmfTopologyType associated with this Topology.
	 *
	 * @param XdmfTopologyType to set.
	 */
	void setTopologyType(const XdmfTopologyType& topType);

	/**
	 * Get the number of elements this Topology contains.
	 *
	 * @return int of number elements in the Topology.
	 */
	int getNumberElements() const;

	/**
	 * Set the number of elements this Topology contains.
	 *
	 * @param int of number elements to set.
	 */
	void setNumberElements(int numberElements);

	/**
	 * Get the name of the TopologyType associated with this Topology.
	 *
	 * @return std::string containing name of the TopologyType.
	 */
	std::string getTopologyTypeAsString() const;

	/**
	 * Get the number of nodes per element for this Topology.
	 *
	 * @return int of number of nodes per element.
	 */
	int getNodesPerElement() const;

/*	*
	 * Get the point data associated with this Topology;

	boost::shared_ptr<XdmfDataItem> getData();

	*
	 * Get the data associated with this Topology (const version)

	boost::shared_ptr<const XdmfDataItem> getData() const;

	*
	 * Set the point data associated with this Topology

	void setData(boost::shared_ptr<XdmfDataItem> connectivityData);*/

	/**
	 *
	 */
	virtual std::string printSelf() const;

	virtual void write(boost::shared_ptr<XdmfVisitor> visitor) const;

protected:

	XdmfTopology();
	virtual ~XdmfTopology();

private:

	XdmfTopology(const XdmfTopology&);  // Not implemented.
	void operator=(const XdmfTopology&);  // Not implemented.

	XdmfTopologyType mTopologyType;
	int mNumberElements;
};

#endif /* XDMFTOPOLOGY_HPP_ */
