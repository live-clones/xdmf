/*
 * XdmfGrid.cpp
 *
 *  Created on: Jan 29, 2010
 *      Author: kleiter
 */

#include "XdmfTopology.hpp"

const XdmfTopologyType XdmfTopologyType::NoTopology(0, "NoTopology");
const XdmfTopologyType XdmfTopologyType::Polyvertex(1, "Polyvertex");
const XdmfTopologyType XdmfTopologyType::Polyline(1, "Polyline");
const XdmfTopologyType XdmfTopologyType::Polygon(1, "Polygon");
const XdmfTopologyType XdmfTopologyType::Triangle(3, "Triangle");
const XdmfTopologyType XdmfTopologyType::Quadrilateral(4, "Quadrilateral");
const XdmfTopologyType XdmfTopologyType::Tetrahedron(4, "Tetrahedron");
const XdmfTopologyType XdmfTopologyType::Pyramid(5, "Pyramid");
const XdmfTopologyType XdmfTopologyType::Wedge(6, "Wedge");
const XdmfTopologyType XdmfTopologyType::Hexahedron(8, "Hexahedron");
const XdmfTopologyType XdmfTopologyType::Edge_3(3, "Edge_3");
const XdmfTopologyType XdmfTopologyType::Triangle_6(6, "Triangle_6");
const XdmfTopologyType XdmfTopologyType::Quadrilateral_8(8, "Quadrilateral_8");
const XdmfTopologyType XdmfTopologyType::Tetrahedron_10(10, "Tetrahedron_10");
const XdmfTopologyType XdmfTopologyType::Pyramid_13(13, "Pyramid_13");
const XdmfTopologyType XdmfTopologyType::Wedge_15(15, "Wedge_15");
const XdmfTopologyType XdmfTopologyType::Hexahedron_20(20, "Hexahedron_20");
const XdmfTopologyType XdmfTopologyType::Mixed(0, "Mixed");
const XdmfTopologyType XdmfTopologyType::TwoDSMesh(1, "2DSMesh");
const XdmfTopologyType XdmfTopologyType::TwoDRectMesh(1, "2DRectMesh");
const XdmfTopologyType XdmfTopologyType::TwoDCoRectMesh(1, "2DCoRectMesh");
const XdmfTopologyType XdmfTopologyType::ThreeDSMesh(1, "3DSMesh");
const XdmfTopologyType XdmfTopologyType::ThreeDRectMesh(1, "3DRectMesh");
const XdmfTopologyType XdmfTopologyType::ThreeDCoRectMesh(1, "3DCoRectMesh");

XdmfTopology::XdmfTopology():
mTopologyType(&XdmfTopologyType::Tetrahedron)
{
	std::cout << "Created Topology " << this << std::endl;
}

XdmfTopology::~XdmfTopology()
{
	std::cout << "Deleted Topology " << this << std::endl;
}

const std::string XdmfTopology::getTopologyTypeAsString() const
{
	return mTopologyType->name;
}

const std::string XdmfTopology::printSelf() const
{
	return "XdmfTopology";
}
