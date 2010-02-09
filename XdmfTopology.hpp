/*
 * XdmfTopology.hpp
 *
 *  Created on: Jan 25, 2010
 *      Author: kleiter
 */

#ifndef XDMFTOPOLOGY_HPP_
#define XDMFTOPOLOGY_HPP_

// Includes
#include "XdmfItem.hpp"

class XdmfTopologyType {

public:

	enum CellType {
		NoCellType, Linear, Quadratic, Arbitrary, Structured
	};

	// Supported Xdmf Topology Types
	static XdmfTopologyType NoTopologyType();
	static XdmfTopologyType Polyvertex();
	static XdmfTopologyType Polyline();
	static XdmfTopologyType Polygon(const int&);
	static XdmfTopologyType Triangle();
	static XdmfTopologyType Quadrilateral();
	static XdmfTopologyType Tetrahedron();
	static XdmfTopologyType Pyramid();
	static XdmfTopologyType Wedge();
	static XdmfTopologyType Hexahedron();
	static XdmfTopologyType Edge_3();
	static XdmfTopologyType Triangle_6();
	static XdmfTopologyType Quadrilateral_8();
	static XdmfTopologyType Tetrahedron_10();
	static XdmfTopologyType Pyramid_13();
	static XdmfTopologyType Wedge_15();
	static XdmfTopologyType Hexahedron_20();
	static XdmfTopologyType Mixed();
	static XdmfTopologyType TwoDSMesh();
	static XdmfTopologyType TwoDRectMesh();
	static XdmfTopologyType TwoDCoRectMesh();
	static XdmfTopologyType ThreeDSMesh();
	static XdmfTopologyType ThreeDRectMesh();
	static XdmfTopologyType ThreeDCoRectMesh();

	/**
	 * Get the number of nodes per element associated with this topology type.
	 *
	 * @return an int containing number of nodes per element.
	 */
	int getNodesPerElement() const;

	/**
	 * Get the name of this topology type
	 *
	 * @return a string containing the name.
	 */
	std::string getName() const;

	/**
	 * Get the cell type associated with this topology type
	 *
	 * @return a CellType containing the cell type.
	 */
	CellType getCellType() const;

	/*
	 * Compare two XdmfTopology types for equality.
	 *
	 * @param an XdmfTopologyType to compare equality to.
	 * @return true iff the XdmfTopologyTypes are equal.
	 */
	bool operator==(const XdmfTopologyType& top) const
	{
		return mNodesPerElement == top.mNodesPerElement && mName.compare(top.mName) == 0 && mCellType == top.mCellType;
	}

	/**
	 * Compare two XdmfTopology types for inequality
	 *
	 * @param XdmfTopologyType to compare inequality to.
	 * @return true iff the XdmfTopologyTypes are not equal.
	 */
	bool operator!=(const XdmfTopologyType& top) const
	{
		return !this->operator ==(top);
	}

	XdmfTopologyType(const XdmfTopologyType&);
	XdmfTopologyType& operator=(const XdmfTopologyType&);

protected:

	/**
	 * Protected constructor for XdmfTopologyType.  The constructor is protected because all topology types supported
	 * by Xdmf should be accessed through more specific static methods that construct XdmfTopologyType - i.e. XdmfTopologyType::Tetrahedron()
	 */
	XdmfTopologyType(const int& nodesPerElement, const std::string& name, const CellType& cellType) :
		mNodesPerElement(nodesPerElement),
		mName(name),
		mCellType(cellType)
	{};

private:

	int mNodesPerElement;
	std::string mName;
	CellType mCellType;
};

class XdmfTopology : public XdmfItem {

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
	void setNumberElements(int&);

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

	/**
	 *
	 */
	virtual const std::string printSelf() const;

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
