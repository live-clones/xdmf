/*
 * XdmfGrid.cpp
 *
 *  Created on: Jan 29, 2010
 *      Author: kleiter
 */

#include "XdmfTopology.hpp"
#include "XdmfVisitor.hpp"

const XdmfTopologyType XdmfTopologyType::NoTopology(0, "NoTopology", NoCellType);
const XdmfTopologyType XdmfTopologyType::Polyvertex(1, "Polyvertex", Linear);
const XdmfTopologyType XdmfTopologyType::Polyline(1, "Polyline", Linear);
const XdmfTopologyType XdmfTopologyType::Polygon(1, "Polygon", Linear);
const XdmfTopologyType XdmfTopologyType::Triangle(3, "Triangle", Linear);
const XdmfTopologyType XdmfTopologyType::Quadrilateral(4, "Quadrilateral", Linear);
const XdmfTopologyType XdmfTopologyType::Tetrahedron(4, "Tetrahedron", Linear);
const XdmfTopologyType XdmfTopologyType::Pyramid(5, "Pyramid", Linear);
const XdmfTopologyType XdmfTopologyType::Wedge(6, "Wedge", Linear);
const XdmfTopologyType XdmfTopologyType::Hexahedron(8, "Hexahedron", Linear);
const XdmfTopologyType XdmfTopologyType::Edge_3(3, "Edge_3", Quadratic);
const XdmfTopologyType XdmfTopologyType::Triangle_6(6, "Triangle_6", Quadratic);
const XdmfTopologyType XdmfTopologyType::Quadrilateral_8(8, "Quadrilateral_8", Quadratic);
const XdmfTopologyType XdmfTopologyType::Tetrahedron_10(10, "Tetrahedron_10", Quadratic);
const XdmfTopologyType XdmfTopologyType::Pyramid_13(13, "Pyramid_13", Quadratic);
const XdmfTopologyType XdmfTopologyType::Wedge_15(15, "Wedge_15", Quadratic);
const XdmfTopologyType XdmfTopologyType::Hexahedron_20(20, "Hexahedron_20", Quadratic);
const XdmfTopologyType XdmfTopologyType::Mixed(0, "Mixed", Arbitrary);
const XdmfTopologyType XdmfTopologyType::TwoDSMesh(1, "2DSMesh", Structured);
const XdmfTopologyType XdmfTopologyType::TwoDRectMesh(1, "2DRectMesh", Structured);
const XdmfTopologyType XdmfTopologyType::TwoDCoRectMesh(1, "2DCoRectMesh", Structured);
const XdmfTopologyType XdmfTopologyType::ThreeDSMesh(1, "3DSMesh", Structured);
const XdmfTopologyType XdmfTopologyType::ThreeDRectMesh(1, "3DRectMesh", Structured);
const XdmfTopologyType XdmfTopologyType::ThreeDCoRectMesh(1, "3DCoRectMesh", Structured);

XdmfTopology::XdmfTopology() :
mTopologyType(&XdmfTopologyType::NoTopology)
{
	std::cout << "Created Topology " << this << std::endl;
}

XdmfTopology::~XdmfTopology()
{
	std::cout << "Deleted Topology " << this << std::endl;
}

void XdmfTopology::write(boost::shared_ptr<XdmfVisitor> visitor) const
{
	visitor->visit(this);
}

const XdmfTopologyType & XdmfTopology::getTopologyType() const
{
	return *mTopologyType;
}

void XdmfTopology::setTopologyType(const XdmfTopologyType& topType)
{
	mTopologyType = &topType;
	return;
}

const int XdmfTopology::getNodesPerElement() const
{
	return mTopologyType->nodesPerElement;
}

const std::string XdmfTopology::getTopologyTypeAsString() const
{
	return mTopologyType->name;
}

const std::string XdmfTopology::printSelf() const
{
	return "XdmfTopology";
}
