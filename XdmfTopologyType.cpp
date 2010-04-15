/*
 * XdmfTopologyType.cpp
 *
 *  Created on: Jan 29, 2010
 *      Author: kleiter
 */

#include "XdmfTopologyType.hpp"

// Supported XdmfTopologyTypes
XdmfTopologyType XdmfTopologyType::NoTopologyType()
{
	return XdmfTopologyType(0, "NoTopology", NoCellType);
}

XdmfTopologyType XdmfTopologyType::Polyvertex()
{
	return XdmfTopologyType(1, "Polyvertex", Linear);
}

XdmfTopologyType XdmfTopologyType::Polyline()
{
	return XdmfTopologyType(1, "Polyline", Linear);
}

XdmfTopologyType XdmfTopologyType::Polygon(const int& nodesPerElement)
{
	return XdmfTopologyType(nodesPerElement, "Polygon", Linear);
}

XdmfTopologyType XdmfTopologyType::Triangle()
{
	return XdmfTopologyType(3, "Triangle", Linear);
}

XdmfTopologyType XdmfTopologyType::Quadrilateral()
{
	return XdmfTopologyType(4, "Quadrilateral", Linear);
}

XdmfTopologyType XdmfTopologyType::Tetrahedron()
{
	return XdmfTopologyType(4, "Tetrahedron", Linear);
}

XdmfTopologyType XdmfTopologyType::Pyramid()
{
	return XdmfTopologyType(5, "Pyramid", Linear);
}

XdmfTopologyType XdmfTopologyType::Wedge()
{
	return XdmfTopologyType(6, "Wedge", Linear);
}

XdmfTopologyType XdmfTopologyType::Hexahedron()
{
	return XdmfTopologyType(8, "Hexahedron", Linear);
}

XdmfTopologyType XdmfTopologyType::Edge_3()
{
	return XdmfTopologyType(3, "Edge_3", Quadratic);
}

XdmfTopologyType XdmfTopologyType::Triangle_6()
{
	return XdmfTopologyType(6, "Triangle_6", Quadratic);
}

XdmfTopologyType XdmfTopologyType::Quadrilateral_8()
{
	return XdmfTopologyType(8, "Quadrilateral_8", Quadratic);
}

XdmfTopologyType XdmfTopologyType::Tetrahedron_10()
{
	return XdmfTopologyType(10, "Tetrahedron_10", Quadratic);
}

XdmfTopologyType XdmfTopologyType::Pyramid_13()
{
	return XdmfTopologyType(13, "Pyramid_13", Quadratic);
}

XdmfTopologyType XdmfTopologyType::Wedge_15()
{
	return XdmfTopologyType(15, "Wedge_15", Quadratic);
}

XdmfTopologyType XdmfTopologyType::Hexahedron_20()
{
	return XdmfTopologyType(20, "Hexahedron_20", Quadratic);
}

XdmfTopologyType XdmfTopologyType::Mixed()
{
	return XdmfTopologyType(0, "Mixed", Arbitrary);
}

XdmfTopologyType XdmfTopologyType::TwoDSMesh()
{
	return XdmfTopologyType(1, "2DSMesh", Structured);
}

XdmfTopologyType XdmfTopologyType::TwoDRectMesh()
{
	return XdmfTopologyType(1, "2DRectMesh", Structured);
}

XdmfTopologyType XdmfTopologyType::TwoDCoRectMesh()
{
	return XdmfTopologyType(1, "2DCoRectMesh", Structured);
}

XdmfTopologyType XdmfTopologyType::ThreeDSMesh()
{
	return XdmfTopologyType(1, "3DSMesh", Structured);
}

XdmfTopologyType XdmfTopologyType::ThreeDRectMesh()
{
	return XdmfTopologyType(1, "3DRectMesh", Structured);
}

XdmfTopologyType XdmfTopologyType::ThreeDCoRectMesh()
{
	return XdmfTopologyType(1, "3DCoRectMesh", Structured);
}

XdmfTopologyType::XdmfTopologyType(const int& nodesPerElement, const std::string& name, const CellType& cellType) :
	mNodesPerElement(nodesPerElement),
	mName(name),
	mCellType(cellType)
{}

XdmfTopologyType::XdmfTopologyType(const XdmfTopologyType& topologyType):
	mNodesPerElement(topologyType.mNodesPerElement),
	mName(topologyType.mName),
	mCellType(topologyType.mCellType)
{
}

XdmfTopologyType& XdmfTopologyType::operator=(const XdmfTopologyType& topologyType)
{
	if(this != &topologyType)
	{
		mNodesPerElement = topologyType.mNodesPerElement;
		mName = topologyType.mName;
		mCellType = topologyType.mCellType;
	}
	return *this;
}

bool XdmfTopologyType::operator==(const XdmfTopologyType& topologyType) const
{
	return mNodesPerElement == topologyType.mNodesPerElement && mName.compare(topologyType.mName) == 0 && mCellType == topologyType.mCellType;
}

bool XdmfTopologyType::operator!=(const XdmfTopologyType& topologyType) const
{
	return !this->operator==(topologyType);
}

XdmfTopologyType::CellType XdmfTopologyType::getCellType() const
{
	return mCellType;
}

std::string XdmfTopologyType::getName() const
{
	return mName;
}

unsigned int XdmfTopologyType::getNodesPerElement() const
{
	return mNodesPerElement;
}
