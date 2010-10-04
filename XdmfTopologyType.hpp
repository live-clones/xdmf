#ifndef XDMFTOPOLOGYTYPE_HPP_
#define XDMFTOPOLOGYTYPE_HPP_

// Includes
#include "XdmfItemProperty.hpp"
/**
 * @brief Property describing the types of elements stored in an XdmfTopology.
 *
 * XdmfTopologyType is a property used by XdmfTopology to specify the element types stored in the XdmfTopology.
 * A specific XdmfTopologyType can be created by calling on one of the static methods in the class, i.e.
 * XdmfTopologyType::Tetrahedron().
 *
 * Xdmf supports the following topology types:
 * 	NoTopologyType
 * 	Polyvertex - Unconnected Points
 * 	Polyline - Line Segments
 * 	Polygon - N Edge Polygon
 * 	Triangle - 3 Edge Polygon
 * 	Quadrilateral - 4 Edge Polygon
 * 	Tetrahedron - 4 Triangular Faces
 * 	Wedge - 4 Triangular Faces, Quadrilateral Base
 * 	Hexahedron - 6 Quadrilateral Faces
 * 	Edge_3 - 3 Node Quadratic Line
 * 	Triangle_6 - 6 Node Quadratic Triangle
 * 	Quadrilateral_8 - 8 Node Quadratic Quadrilateral
 * 	Quadrilateral_9 - 9 Node Bi-Quadratic Quadrilateral
 * 	Tetrahedron_10 - 10 Node Quadratic Tetrahedron
 * 	Pyramid_13 - 13 Node Quadratic Pyramid
 * 	Wedge_15 - 15 Node Quadratic Wedge
 * 	Wedge_18 - 18 Node Bi-Quadratic Wedge
 * 	Hexahedron_20 - 20 Node Quadratic Hexahedron
 * 	Hexahedron_24 - 24 Node Bi-Quadratic Hexahedron
 * 	Hexahedron_27 - 27 Node Tri-Quadratic Hexahedron
 * 	Hexahedron_64 - 64 Node Tri-Cubic Hexahedron
 * 	Hexahedron_64_GLL - 64 Node Spectral Tri-Cubic Hexahedron with Gauss-Lobatto-Legendre points.
 * 	Hexahedron_125 - 125 Node Tri-Quartic Hexahedron
 * 	Hexahedron_125_GLL - 125 Node Spectral Tri-Quartic Hexahedron with Gauss-Lobatto-Legendre points.
 * 	Mixed - Mixture of Unstructured Topologies
 * 	TwoDSMesh
 * 	TwoDRectMesh
 * 	TwoDCoRectMesh
 * 	ThreeDSMesh
 * 	ThreeDRectMesh
 * 	ThreeDCoRectMesh
 */
class XdmfTopologyType : public XdmfItemProperty {

public:

	virtual ~XdmfTopologyType();

	friend class XdmfTopology;

	enum CellType {
		NoCellType, Linear, Quadratic, Cubic, Quartic, Arbitrary, Structured
	};

	// Supported Xdmf Topology Types
	static boost::shared_ptr<const XdmfTopologyType> NoTopologyType();
	static boost::shared_ptr<const XdmfTopologyType> Polyvertex();
	static boost::shared_ptr<const XdmfTopologyType> Polyline();
	static boost::shared_ptr<const XdmfTopologyType> Polygon(const unsigned int nodesPerElement);
	static boost::shared_ptr<const XdmfTopologyType> Triangle();
	static boost::shared_ptr<const XdmfTopologyType> Quadrilateral();
	static boost::shared_ptr<const XdmfTopologyType> Tetrahedron();
	static boost::shared_ptr<const XdmfTopologyType> Pyramid();
	static boost::shared_ptr<const XdmfTopologyType> Wedge();
	static boost::shared_ptr<const XdmfTopologyType> Hexahedron();
	static boost::shared_ptr<const XdmfTopologyType> Edge_3();
	static boost::shared_ptr<const XdmfTopologyType> Triangle_6();
	static boost::shared_ptr<const XdmfTopologyType> Quadrilateral_8();
	static boost::shared_ptr<const XdmfTopologyType> Quadrilateral_9();
	static boost::shared_ptr<const XdmfTopologyType> Tetrahedron_10();
	static boost::shared_ptr<const XdmfTopologyType> Pyramid_13();
	static boost::shared_ptr<const XdmfTopologyType> Wedge_15();
	static boost::shared_ptr<const XdmfTopologyType> Wedge_18();
	static boost::shared_ptr<const XdmfTopologyType> Hexahedron_20();
	static boost::shared_ptr<const XdmfTopologyType> Hexahedron_24();
	static boost::shared_ptr<const XdmfTopologyType> Hexahedron_27();
	static boost::shared_ptr<const XdmfTopologyType> Hexahedron_64();
	static boost::shared_ptr<const XdmfTopologyType> Hexahedron_64_GLL();
	static boost::shared_ptr<const XdmfTopologyType> Hexahedron_125();
	static boost::shared_ptr<const XdmfTopologyType> Hexahedron_125_GLL();
	static boost::shared_ptr<const XdmfTopologyType> Mixed();
	//static boost::shared_ptr<const XdmfTopologyType> TwoDRectMesh();
	//static boost::shared_ptr<const XdmfTopologyType> TwoDCoRectMesh();
	//static boost::shared_ptr<const XdmfTopologyType> ThreeDRectMesh();
	//static boost::shared_ptr<const XdmfTopologyType> ThreeDCoRectMesh();

	/**
	 * Get the cell type associated with this topology type.
	 *
	 * @return a CellType containing the cell type.
	 */
	CellType getCellType() const;

	/**
	 * Get the name of this topology type.
	 *
	 * @return the name of this topology type.
	 */
	virtual std::string getName() const;

	/**
	 * Get the number of nodes per element associated with this topology type.
	 *
	 * @return an unsigned int containing number of nodes per element.
	 */
	virtual unsigned int getNodesPerElement() const;

	void getProperties(std::map<std::string, std::string> & collectedProperties) const;

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

	/**
	 * Compare two XdmfTopologyType for equality (for wrapping)
	 *
	 * @param topologyType a boost shared pointer to an XdmfTopologyType to compare equality to.
	 * @return true if the XdmfTopologyType are equal.
	 */
	bool IsEqual(boost::shared_ptr<XdmfTopologyType> topologyType);

protected:

	/**
	 * Protected constructor for XdmfTopologyType.  The constructor is protected because all topology types supported
	 * by Xdmf should be accessed through more specific static methods that construct XdmfTopologyType -
	 * i.e. XdmfTopologyType::Tetrahedron()
	 */
	XdmfTopologyType(const unsigned int nodesPerElement, const std::string & name, const CellType cellType);

private:

	XdmfTopologyType(const XdmfTopologyType & topologyType); // Not implemented.
	void operator=(const XdmfTopologyType & topologyType); // Not implemented.

	static boost::shared_ptr<const XdmfTopologyType> New(const std::map<std::string, std::string> & itemProperties);

	CellType mCellType;
	std::string mName;
	unsigned int mNodesPerElement;
};

#endif /* XDMFTOPOLOGYTYPE_HPP_ */
