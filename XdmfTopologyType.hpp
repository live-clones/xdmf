#ifndef XDMFTOPOLOGYTYPE_HPP_
#define XDMFTOPOLOGYTYPE_HPP_

// Includes
#include <string>

/**
 * @brief Property describing the types of elements stored in an XdmfTopology.
 *
 * XdmfTopologyType is a property used by XdmfTopology to specify the element types stored in the XdmfTopology.
 * A specific XdmfTopologyType can be created by calling on of the static methods in the class, i.e.
 * XdmfTopologyType::Tetrahedron().
 *
 * Xdmf supports the following topology types:
 * 	NoTopologyType
 * 	Polyvertex
 * 	Polyline
 * 	Polygon
 * 	Triangle
 * 	Quadrilateral
 * 	Tetrahedron
 * 	Wedge
 * 	Hexahedron
 * 	Edge_3
 * 	Triangle_6
 * 	Quadrilateral_8
 * 	Tetrahedron_10
 * 	Pyramid_13
 * 	Wedge_15
 * 	Hexahedron_20
 * 	Mixed
 * 	TwoDSMesh
 * 	TwoDRectMesh
 * 	TwoDCoRectMesh
 * 	ThreeDSMesh
 * 	ThreeDRectMesh
 *	ThreeDCoRectMesh
 */
class XdmfTopologyType {

public:

	enum CellType {
		NoCellType, Linear, Quadratic, Arbitrary, Structured
	};

	// Supported Xdmf Topology Types
	static XdmfTopologyType NoTopologyType();
	static XdmfTopologyType Polyvertex();
	static XdmfTopologyType Polyline();
	static XdmfTopologyType Polygon(const int & numNodes);
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
	 * Get the cell type associated with this topology type
	 *
	 * @return a CellType containing the cell type.
	 */
	CellType getCellType() const;

	/**
	 * Get the name of this topology type
	 *
	 * @return a string containing the name.
	 */
	std::string getName() const;

	/**
	 * Get the number of nodes per element associated with this topology type.
	 *
	 * @return an unsinged int containing number of nodes per element.
	 */
	unsigned int getNodesPerElement() const;

	/*
	 * Compare two XdmfTopologyTypes for equality.
	 *
	 * @param topologyType an XdmfTopologyType to compare equality to.
	 * @return true if the XdmfTopologyTypes are equal.
	 */
	bool operator==(const XdmfTopologyType & topologyType) const;

	/**
	 * Compare two XdmfTopologyTypes for inequality
	 *
	 * @param topologyType XdmfTopologyType to compare inequality to.
	 * @return true if the XdmfTopologyTypes are not equal.
	 */
	bool operator!=(const XdmfTopologyType & topologyType) const;

	XdmfTopologyType(const XdmfTopologyType & topologyType);
	XdmfTopologyType& operator=(const XdmfTopologyType & topologyType);

protected:

	/**
	 * Protected constructor for XdmfTopologyType.  The constructor is protected because all topology types supported
	 * by Xdmf should be accessed through more specific static methods that construct XdmfTopologyType -
	 * i.e. XdmfTopologyType::Tetrahedron()
	 */
	XdmfTopologyType(const int & nodesPerElement, const std::string & name, const CellType & cellType);

private:

	CellType mCellType;
	std::string mName;
	int mNodesPerElement;
};

#endif /* XDMFTOPOLOGYTYPE_HPP_ */
