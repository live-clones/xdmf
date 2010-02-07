/*
 * XdmfTopology.hpp
 *
 *  Created on: Jan 25, 2010
 *      Author: kleiter
 */

#ifndef XDMFTOPOLOGY_HPP_
#define XDMFTOPOLOGY_HPP_

#include "XdmfItem.hpp"

class XdmfTopologyType {

public:

	enum CellType {
		NoCellType, Linear, Quadratic, Arbitrary, Structured
	};

	const int nodesPerElement;
	const std::string name;
	const CellType cellType;

	// Supported Xdmf Topology Type
	// TODO: How do we deal with Polygon datatypes and others that can have variable nodesPerElement?
	static const XdmfTopologyType NoTopology;
	static const XdmfTopologyType Polyvertex;
	static const XdmfTopologyType Polyline;
	static const XdmfTopologyType Polygon;
	static const XdmfTopologyType Triangle;
	static const XdmfTopologyType Quadrilateral;
	static const XdmfTopologyType Tetrahedron;
	static const XdmfTopologyType Pyramid;
	static const XdmfTopologyType Wedge;
	static const XdmfTopologyType Hexahedron;
	static const XdmfTopologyType Edge_3;
	static const XdmfTopologyType Triangle_6;
	static const XdmfTopologyType Quadrilateral_8;
	static const XdmfTopologyType Tetrahedron_10;
	static const XdmfTopologyType Pyramid_13;
	static const XdmfTopologyType Wedge_15;
	static const XdmfTopologyType Hexahedron_20;
	static const XdmfTopologyType Mixed;
	static const XdmfTopologyType TwoDSMesh;
	static const XdmfTopologyType TwoDRectMesh;
	static const XdmfTopologyType TwoDCoRectMesh;
	static const XdmfTopologyType ThreeDSMesh;
	static const XdmfTopologyType ThreeDRectMesh;
	static const XdmfTopologyType ThreeDCoRectMesh;

	/*
	 * Compare two XdmfTopology types for equality. Each topology type has a single static instance meaning that
	 * equality can be determined by determining if they have the same pointer value.
	 *
	 * @param an XdmfTopologyType to compare equality to.
	 * @return true iff the XdmfTopologyTypes are equal.
	 */
	bool operator==(const XdmfTopologyType& top) const
	{
		return (this == &top) ? true : false;
	}

	/**
	 * Compare two XdmfTopology types for inequality. Each topology type has a single static instance meaning that
	 * equality can be determined by determining if they have the same pointer value.
	 *
	 * @param XdmfTopologyType to compare inequality to.
	 * @return true iff the XdmfTopologyTypes are not equal.
	 */
	bool operator!=(const XdmfTopologyType& top) const
	{
		return (this != &top) ? true : false;
	}

protected:

	/**
	 * Protected constructor for XdmfTopologyType.  The constructor is protected because all topology types supported
	 * by Xdmf should be accessed through static public instances of XdmfTopologyType - i.e. XdmfTopologyType::Tetrahedron.
	 */
	XdmfTopologyType(const int& nodesPerElement, const std::string& name, const CellType& cellType) :
		nodesPerElement(nodesPerElement),
		name(name),
		cellType(cellType)
	{};

private:

	//XdmfTopologyType(const XdmfTopologyType&);  // Not implemented.
	//void operator=(const XdmfTopologyType&);  // Not implemented.

};

class XdmfTopology : public XdmfItem {

public:

	XdmfNewMacro(XdmfTopology);

	virtual void write(boost::shared_ptr<XdmfVisitor>) const;

	/**
	 * Get the XdmfTopologyType associated with this Topology.
	 *
	 * @return XdmfTopologyType.
	 */
	const XdmfTopologyType& getTopologyType() const;

	/**
	 * Set the XdmfTopologyType associated with this Topology.
	 *
	 * @param XdmfTopologyType to set.
	 */
	void setTopologyType(const XdmfTopologyType&);

	/**
	 * Get the name of the TopologyType associated with this Topology.
	 *
	 * @return std::string containing name of the TopologyType.
	 */
	const std::string getTopologyTypeAsString() const;

	/**
	 * Get the number of nodes per element for this Topology.
	 *
	 * @return int of number of nodes per element.
	 */
	const int getNodesPerElement() const;

	/**
	 *
	 */
	virtual const std::string printSelf() const;

protected:

	XdmfTopology();
	virtual ~XdmfTopology();

private:

  XdmfTopology(const XdmfTopology&);  // Not implemented.
  void operator=(const XdmfTopology&);  // Not implemented.

  const XdmfTopologyType * mTopologyType;
};

#endif /* XDMFTOPOLOGY_HPP_ */
