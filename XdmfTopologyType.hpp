#ifndef XDMFTOPOLOGYTYPE_HPP_
#define XDMFTOPOLOGYTYPE_HPP_

// Includes
#include "XdmfItemProperty.hpp"
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
 * 	Quadrilateral_9
 * 	Tetrahedron_10
 * 	Pyramid_13
 * 	Wedge_15
 * 	Hexahedron_20
 * 	Hexahedron_24
 * 	Hexahedron_27
 * 	Hexahedron_64
 * 	Hexahedron_125
 * 	Mixed
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
	static boost::shared_ptr<const XdmfTopologyType> Hexahedron_125();
	static boost::shared_ptr<const XdmfTopologyType> Mixed();
	static boost::shared_ptr<const XdmfTopologyType> TwoDSMesh();
	static boost::shared_ptr<const XdmfTopologyType> TwoDRectMesh();
	static boost::shared_ptr<const XdmfTopologyType> TwoDCoRectMesh();
	static boost::shared_ptr<const XdmfTopologyType> ThreeDSMesh();
	static boost::shared_ptr<const XdmfTopologyType> ThreeDRectMesh();
	static boost::shared_ptr<const XdmfTopologyType> ThreeDCoRectMesh();

	/**
	 * Get the cell type associated with this topology type
	 *
	 * @return a CellType containing the cell type.
	 */
	CellType getCellType() const;

	/**
	 * Get the number of nodes per element associated with this topology type.
	 *
	 * @return an unsinged int containing number of nodes per element.
	 */
	unsigned int getNodesPerElement() const;

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

protected:

	/**
	 * Protected constructor for XdmfTopologyType.  The constructor is protected because all topology types supported
	 * by Xdmf should be accessed through more specific static methods that construct XdmfTopologyType -
	 * i.e. XdmfTopologyType::Tetrahedron()
	 */
	XdmfTopologyType(const int & nodesPerElement, const std::string & name, const CellType & cellType);

private:

	XdmfTopologyType(const XdmfTopologyType & topologyType); // Not implemented.
	void operator=(const XdmfTopologyType & topologyType); // Not implemented.

	static boost::shared_ptr<const XdmfTopologyType> New(const std::map<std::string, std::string> & itemProperties);

	CellType mCellType;
	std::string mName;
	int mNodesPerElement;
};

#endif /* XDMFTOPOLOGYTYPE_HPP_ */
